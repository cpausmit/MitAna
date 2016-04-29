#include "MitAna/PhysicsMod/interface/FastJetMod.h"
#include "MitAna/DataTree/interface/ParticleCol.h"
#include "MitAna/DataTree/interface/PFJetCol.h"
#include "MitAna/DataTree/interface/GenJetCol.h"
#include "MitAna/DataTree/interface/PFCandidateCol.h"
#include "MitAna/DataTree/interface/MCParticleCol.h"

#include "fastjet/PseudoJet.hh"
#include "fastjet/JetDefinition.hh"
#include "fastjet/GhostedAreaSpec.hh"
#include "fastjet/AreaDefinition.hh"
#include "fastjet/ClusterSequence.hh"
#include "fastjet/ClusterSequenceArea.hh"

using namespace mithep;

ClassImp(mithep::FastJetMod)

//--------------------------------------------------------------------------------------------------
void
FastJetMod::Process()
{
  fOutputJets->Reset();

  auto* input = GetObject<ParticleCol>(fInputName);
  if (!input) {
    SendError(kAbortModule, "Process", "Pointer to input collection %s null.", fInputName.Data());
    return;
  }

  std::vector<fastjet::PseudoJet> fjParts;
  fjParts.reserve(input->GetEntries());
  // Push all particle flow candidates of the input PFjet into fastjet particle collection
  for (UInt_t iP = 0; iP != input->GetEntries(); ++iP) {
    auto& part(*input->At(iP));

    bool typeOK = false;
    switch (fOutputType) {
    case kPFJet:
      if (part.ObjType() == kPFCandidate)
        typeOK = true;
      break;
    case kGenJet:
      if (part.ObjType() == kMCParticle) {
        typeOK = true;
        if (static_cast<MCParticle const&>(part).Status() != 1)
          continue;
      }
      break;
    default:
      break;
    }
    if (!typeOK) {
      Warning("Process", "Element %d of input collection %s has type %d that is incompatible with the output jet collection type %d.", iP, fInputName.Data(), part.ObjType(), fOutputType);
      continue;
    }

    // Exclude very soft (unphysical) particles
    if (part.Pt() < fParticleMinPt)
      continue;

    fjParts.emplace_back(part.Px(), part.Py(), part.Pz(), part.E());
    fjParts.back().set_user_index(iP);
  }	

  // Setup the clusters for fastjet
  fastjet::ClusterSequence* fjClustering = 0;
  if (fAreaDefinition)
    fjClustering = new fastjet::ClusterSequenceArea(fjParts, *fJetDef, *fAreaDefinition);
  else
    fjClustering = new fastjet::ClusterSequence(fjParts, *fJetDef);

  // ---- Fastjet is ready ----

  // Produce a new set of jets based on the fastjet particle collection and the defined clustering
  // Cut off jets with pt < fJetMinPt GeV
  std::vector<fastjet::PseudoJet> fjOutJets(sorted_by_pt(fjClustering->inclusive_jets(fJetMinPt)));

  // Now loop over PFJets and fill the output collection
  for (auto& fjet : fjOutJets) {
    switch (fOutputType) {
    case kPFJet:
      {
        auto* outJet = static_cast<PFJetArr*>(fOutputJets)->Allocate();
        new (outJet) PFJet;
        FillPFJet(fjet, *outJet, *input);
      }
      break;
    case kGenJet:
      {
        auto* outJet = static_cast<GenJetArr*>(fOutputJets)->Allocate();
        new (outJet) GenJet;
        FillGenJet(fjet, *outJet, *input);
      }
      break;
    default:
      continue;
    }
  }

  // some memory cleanup
  if (fjOutJets.size() > 0)
    fjClustering->delete_self_when_unused();

  delete fjClustering;
}

//--------------------------------------------------------------------------------------------------
void
FastJetMod::SlaveBegin()
{
  switch (fOutputType) {
  case kPFJet:
    fOutputJets = new PFJetArr;
  case kGenJet:
    fOutputJets = new GenJetArr;
    break;
  default:
    SendError(kAbortModule, "SlaveBegin", "Output collection type not supported.");
    return;
  }
  fOutputJets->SetName(fOutputJetsName);

  // Jet definition constructor
  switch (fJetAlgorithm) {
  case kCA:
    fJetDef = new fastjet::JetDefinition(fastjet::cambridge_algorithm, fJetConeSize);
    break;
  case kKT:
    fJetDef = new fastjet::JetDefinition(fastjet::kt_algorithm, fJetConeSize);
    break;
  case kAK:
    fJetDef = new fastjet::JetDefinition(fastjet::antikt_algorithm, fJetConeSize);
    break;
  };

  // // Initialize area caculation (done with ghost particles)
  if (fActiveAreaRepeats > 0) {
    fActiveArea = new fastjet::GhostedAreaSpec(fGhostEtaMax, fActiveAreaRepeats, fGhostArea);
    fAreaDefinition = new fastjet::AreaDefinition(fastjet::active_area_explicit_ghosts, *fActiveArea);
  }

  PublishObj(fOutputJets);
}

//--------------------------------------------------------------------------------------------------
void
FastJetMod::SlaveTerminate()
{
  RetractObj(fOutputJets->GetName());

  delete fOutputJets;
  fOutputJets = 0;

  delete fJetDef;
  delete fActiveArea;
  delete fAreaDefinition;

  fJetDef = 0;
  fActiveArea = 0;
  fAreaDefinition = 0;
}

//--------------------------------------------------------------------------------------------------
void
FastJetMod::FillPFJet(fastjet::PseudoJet const& inJet, PFJet& outJet, ParticleCol const& outConsts)
{
  outJet.SetRawPtEtaPhiM(inJet.pt(), inJet.eta(), inJet.phi(), inJet.m());

  outJet.SetJetArea(inJet.area());

  // Prepare the jet observables related to PFConstituents
  double chargedHadronEnergy = 0.;
  double neutralHadronEnergy = 0.;
  double chargedEmEnergy     = 0.;
  double chargedMuEnergy     = 0.;
  double neutralEmEnergy     = 0.;
  int chargedMultiplicity   = 0;
  int neutralMultiplicity   = 0;
  int muonMultiplicity      = 0;

  // Loop on input jet constituents vector and discard very soft particles (ghosts)
  for (auto& part : inJet.constituents()) {
    if (part.perp() < fParticleMinPt)
      continue;

    // First of all fix the linking between PFJets and PFCandidates
    PFCandidate const* pfCand = static_cast<PFCandidate const*>(outConsts.At(part.user_index()));
    // Check that the pfCandidate exists
    if (!pfCand) {
      Error("FillPFJet", "Input PFCand pointer is null, skipping this candidate.");
      continue;
    }

    outJet.AddPFCand(pfCand);

    // Now take care of energy fraction and multiplicities
    switch (pfCand->PFType()) {
    case PFCandidate::eHadron:
      chargedHadronEnergy += pfCand->E();
      ++chargedMultiplicity;
      break;
    case PFCandidate::eNeutralHadron:
      neutralHadronEnergy += pfCand->E();
      ++neutralMultiplicity;
      break;
    case PFCandidate::eGamma:
      neutralEmEnergy += pfCand->E();
      ++neutralMultiplicity;
      break;
    case PFCandidate::eElectron:
      chargedEmEnergy += pfCand->E();
      ++chargedMultiplicity;
      break;
    case PFCandidate::eMuon:
      chargedMuEnergy += pfCand->E();
      ++chargedMultiplicity;
      break;
    default:
      break;
    }
  }// end loop on jet constituents

  // Fill in the energy fractions and multiplicieties
  outJet.SetChargedHadronEnergy(chargedHadronEnergy);
  outJet.SetNeutralHadronEnergy(neutralHadronEnergy);
  outJet.SetChargedEmEnergy(chargedEmEnergy);
  outJet.SetChargedMuEnergy(chargedMuEnergy);
  outJet.SetNeutralEmEnergy(neutralEmEnergy);
  outJet.SetChargedMultiplicity(chargedMultiplicity);
  outJet.SetNeutralMultiplicity(neutralMultiplicity);
  outJet.SetMuonMultiplicity(muonMultiplicity);
}

void
FastJetMod::FillGenJet(fastjet::PseudoJet const& inJet, GenJet& outJet, ParticleCol const& outConsts)
{
  outJet.SetPtEtaPhiM(inJet.pt(), inJet.eta(), inJet.phi(), inJet.m());

  double hadEnergy = 0.;
  double emEnergy = 0.;
  double invEnergy = 0.;
  double auxEnergy = 0.;

  for (auto& part : inJet.constituents()) {
    if (part.perp() < fParticleMinPt)
      continue;

    // First of all fix the linking between PFJets and PFCandidates
    MCParticle const* genCand = static_cast<MCParticle const*>(outConsts.At(part.user_index()));
    // Check that the pfCandidate exists
    if (!genCand) {
      Error("FillGenJet", "Input MCParticle pointer is null, skipping this candidate.");
      continue;
    }

    // the following definitions of "EM", "Hadron", and "Invisible" taken from
    // RecoJets/JetProducers/src/JetSpecific.cc
    switch (std::abs(genCand->PdgId())) {
    case 11: // e
    case 22: // gamma
      emEnergy += genCand->E();
      break;
    case 211: // pi
    case 321: // K
    case 130: // KL
    case 2212: // p
    case 2112: // n
      hadEnergy += genCand->E();
      break;
    case 13: // muon
    case 12: // nu_e
    case 14: // nu_mu
    case 16: // nu_tau
      invEnergy += genCand->E();
      break;
    default:
      auxEnergy += genCand->E();
      break;
    }
  }

  outJet.SetHadEnergy(hadEnergy);
  outJet.SetEmEnergy(emEnergy);
  outJet.SetInvisibleEnergy(invEnergy);
  outJet.SetAuxiliaryEnergy(auxEnergy);
}
