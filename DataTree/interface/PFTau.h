//--------------------------------------------------------------------------------------------------
// PFTau
//
// This class holds information about reconstructed tau based on PFCandidates.
//
// Authors: J.Bendavid, C.Paus, Y.Iiyama
//--------------------------------------------------------------------------------------------------

#ifndef MITANA_DATATREE_PFTAU_H
#define MITANA_DATATREE_PFTAU_H

#include "MitAna/DataTree/interface/Tau.h"
#include "MitAna/DataCont/interface/RefArray.h"
#include "MitAna/DataCont/interface/Ref.h"
#include "MitAna/DataTree/interface/PFCandidate.h"
#include "MitAna/DataTree/interface/BasicCluster.h"
#include "MitAna/DataTree/interface/PFJet.h"

namespace mithep {

  class PFTau : public Tau {
  public:
    enum Discriminant {
      dAgainstElectronMVA6Raw,
      dAgainstElectronMVA6category,
      dByCombinedIsolationDeltaBetaCorrRaw3Hits,
      dByIsolationMVArun2v1DBdR03oldDMwLTraw,
      dByIsolationMVArun2v1DBnewDMwLTraw,
      dByIsolationMVArun2v1DBoldDMwLTraw,
      dByIsolationMVArun2v1PWdR03oldDMwLTraw,
      dByIsolationMVArun2v1PWnewDMwLTraw,
      dByIsolationMVArun2v1PWoldDMwLTraw,
      dChargedIsoPtSum,
      dChargedIsoPtSumdR03,
      dFootprintCorrection,
      dFootprintCorrectiondR03,
      dNeutralIsoPtSum,
      dNeutralIsoPtSumWeight,
      dNeutralIsoPtSumWeightdR03,
      dNeutralIsoPtSumdR03,
      dPhotonPtSumOutsideSignalCone,
      dPhotonPtSumOutsideSignalConedR03,
      dPuCorrPtSum,
      nDiscriminants
    };

    enum Identifier {
      iAgainstElectronLooseMVA6 = nDiscriminants,
      iAgainstElectronMediumMVA6,
      iAgainstElectronTightMVA6,
      iAgainstElectronVLooseMVA6,
      iAgainstElectronVTightMVA6,
      iAgainstMuonLoose3,
      iAgainstMuonTight3,
      iByLooseCombinedIsolationDeltaBetaCorr3Hits,
      iByLooseIsolationMVArun2v1DBdR03oldDMwLT,
      iByLooseIsolationMVArun2v1DBnewDMwLT,
      iByLooseIsolationMVArun2v1DBoldDMwLT,
      iByLooseIsolationMVArun2v1PWdR03oldDMwLT,
      iByLooseIsolationMVArun2v1PWnewDMwLT,
      iByLooseIsolationMVArun2v1PWoldDMwLT,
      iByMediumCombinedIsolationDeltaBetaCorr3Hits,
      iByMediumIsolationMVArun2v1DBdR03oldDMwLT,
      iByMediumIsolationMVArun2v1DBnewDMwLT,
      iByMediumIsolationMVArun2v1DBoldDMwLT,
      iByMediumIsolationMVArun2v1PWdR03oldDMwLT,
      iByMediumIsolationMVArun2v1PWnewDMwLT,
      iByMediumIsolationMVArun2v1PWoldDMwLT,
      iByPhotonPtSumOutsideSignalCone,
      iByTightCombinedIsolationDeltaBetaCorr3Hits,
      iByTightIsolationMVArun2v1DBdR03oldDMwLT,
      iByTightIsolationMVArun2v1DBnewDMwLT,
      iByTightIsolationMVArun2v1DBoldDMwLT,
      iByTightIsolationMVArun2v1PWdR03oldDMwLT,
      iByTightIsolationMVArun2v1PWnewDMwLT,
      iByTightIsolationMVArun2v1PWoldDMwLT,
      iByVLooseIsolationMVArun2v1DBdR03oldDMwLT,
      iByVLooseIsolationMVArun2v1DBnewDMwLT,
      iByVLooseIsolationMVArun2v1DBoldDMwLT,
      iByVLooseIsolationMVArun2v1PWdR03oldDMwLT,
      iByVLooseIsolationMVArun2v1PWnewDMwLT,
      iByVLooseIsolationMVArun2v1PWoldDMwLT,
      iByVTightIsolationMVArun2v1DBdR03oldDMwLT,
      iByVTightIsolationMVArun2v1DBnewDMwLT,
      iByVTightIsolationMVArun2v1DBoldDMwLT,
      iByVTightIsolationMVArun2v1PWdR03oldDMwLT,
      iByVTightIsolationMVArun2v1PWnewDMwLT,
      iByVTightIsolationMVArun2v1PWoldDMwLT,
      iByVVTightIsolationMVArun2v1DBdR03oldDMwLT,
      iByVVTightIsolationMVArun2v1DBnewDMwLT,
      iByVVTightIsolationMVArun2v1DBoldDMwLT,
      iByVVTightIsolationMVArun2v1PWdR03oldDMwLT,
      iByVVTightIsolationMVArun2v1PWnewDMwLT,
      iByVVTightIsolationMVArun2v1PWoldDMwLT,
      iDecayModeFinding,
      iDecayModeFindingNewDMs,
      nDiscriminators,
      nIdentifiers = nDiscriminators - nDiscriminants
    };

    enum LegacyDiscriminator {
      kDiscriminationAgainstElectron = nIdentifiers,
      kDiscriminationAgainstMuon,
      kDiscriminationByMVAElectronRejection,
      kDiscriminationByVLooseIsolation,
      kDiscriminationByLooseIsolation,
      kDiscriminationByMediumIsolation,
      kDiscriminationByTightIsolation,
      kMVA2rawElectronRejection,
      kMVA2rawElectronRejectionCategory,
      kMVA2LooseElectronRejection,
      kMVA2MediumElectronRejection,
      kMVA2TightElectronRejection,
      kMVA3rawElectronRejection,
      kMVA3rawElectronRejectionCategory,
      kMVA3LooseElectronRejection,
      kMVA3MediumElectronRejection,
      kMVA3TightElectronRejection,
      kMVA3VTightElectronRejection,
      kMVA3IsolationChargedIsoPtSum, // chargedIsoPtSum (<= 7_4_6 ?)
      kMVA3IsolationNeutralIsoPtSum, // chargedIsoPtSum
      kMVA3IsolationPUcorrPtSum, // puCorrPtSum
      kDiscriminationByLooseElectronRejection, // againstElectronLoose
      kDiscriminationByMediumElectronRejection, // againstElectronMedium
      kDiscriminationByTightElectronRejection, // againstElectronTight
      kDiscriminationByLooseMuonRejection, // againstMuonLoose
      kDiscriminationByMediumMuonRejection, // againstMuonMedium
      kDiscriminationByTightMuonRejection, // againstMuonTight
      kDiscriminationByLooseMuonRejection2, // againstMuonLoose2
      kDiscriminationByMediumMuonRejection2, // againstMuonMedium2
      kDiscriminationByTightMuonRejection2, // againstMuonTight2
      kDiscriminationByDecayModeFindingOldDMs, // decayModeFindingOldDMs
      kDiscriminationByVLooseCombinedIsolationDBSumPtCorr, // byVLooseCombinedIsolationDeltaBetaCorr
      kDiscriminationByLooseCombinedIsolationDBSumPtCorr, // byLooseCombinedIsolationDeltaBetaCorr
      kDiscriminationByMediumCombinedIsolationDBSumPtCorr, // byMediumCombinedIsolationDeltaBetaCorr
      kDiscriminationByTightCombinedIsolationDBSumPtCorr, // byTightCombinedIsolationDeltaBetaCorr
      kDiscriminationByRawCombinedIsolationDBSumPtCorr, // byCombinedIsolationDeltaBetaCorrRaw
      kDiscriminationByMVA5VLooseElectronRejection, // againstElectronVLooseMVA5
      kDiscriminationByMVA5LooseElectronRejection, // againstElectronLooseMVA5
      kDiscriminationByMVA5MediumElectronRejection, // againstElectronMediumMVA5
      kDiscriminationByMVA5TightElectronRejection, // againstElectronTightMVA5
      kDiscriminationByLoosePileupWeightedIsolation3Hits, // byLoosePileupWeightedIsolation3Hits
      kDiscriminationByMediumPileupWeightedIsolation3Hits, // byMediumPileupWeightedIsolation3Hits
      kDiscriminationByTightPileupWeightedIsolation3Hits, // byTightPileupWeightedIsolation3Hits
      kDiscriminationByRawPileupWeightedIsolation3Hits, // byPileupWeightedIsolationRaw3Hits
      nAllDiscriminators,
      nLegacyDiscriminators = nAllDiscriminators - nDiscriminators
    };

    // Taken from DataFormats/TauReco/interface/PFTaus.h
    enum HadronicDecayMode {
      kNull = -1,
      kOneProng0PiZero,
      kOneProng1PiZero,
      kOneProng2PiZero,
      kOneProng3PiZero,
      kOneProngNPiZero,
      kTwoProng0PiZero,
      kTwoProng1PiZero,
      kTwoProng2PiZero,
      kTwoProng3PiZero,
      kTwoProngNPiZero,
      kThreeProng0PiZero,
      kThreeProng1PiZero,
      kThreeProng2PiZero,
      kThreeProng3PiZero,
      kThreeProngNPiZero,
      kRareDecayMode,
      nHadronicDecayModes = kRareDecayMode + 2 // accounting the fact that the enum starts at -1
    };

    PFTau() : Tau() {}

    Double_t LeadPFCandSignD0Sig() const { return fLeadPFCandSignD0Sig; }
    Double_t HCalTotalEOverP() const { return fHCalTotalEOverP; }
    Double_t HCalMaxEOverP() const { return fHCalMaxEOverP; }
    Double_t HCal3x3EOverP() const { return fHCal3x3EOverP; }
    Double_t IsoChargedHadronPtSum() const { return fIsoChargedHadronPtSum; }
    Double_t IsoGammaEtSum() const { return fIsoGammaEtSum; }
    Double_t MaxHCalPFClusterEt() const { return fMaxHCalPFClusterEt; }
    Double_t EMFraction() const { return fEMFraction; }
    Double_t ECalStripSumEOverP() const { return fECalStripSumEOverP; }
    Double_t BremRecoveryEOverP() const { return fBremRecoveryEOverP; }
    Double_t ElectronPreIDOutput() const { return fElectronPreIDOutput; }
    Double_t CaloCompatibility() const { return fCaloCompatibility; }
    Double_t SegmentCompatibility() const { return fSegmentCompatibility; }
    Double_t SignalConeSize() const { return fSignalConeSize; }
    Double_t BendCorrMass() const { return fBendCorrMass; }
    Bool_t ElectronPreIDDecision() const { return fElectronPreIDDecision; }
    Bool_t MuonDecision() const { return fMuonDecision; }
    Double_t PFTauDiscriminator(UInt_t) const; // accepts discriminant, identifier, and legacy indices. returns 0.0 or 1.0 for identifiers
    Double_t PFTauDiscriminant(UInt_t i) const { return fPFTauDiscriminants[i]; }
    Bool_t PFTauIdentifier(UInt_t i) const { return fPFTauIdentifiers.TestBit(i - nDiscriminators); }
    HadronicDecayMode DecayMode() const { return fHadronicDecayMode; }
    PFCandidate const* LeadPFCand() const { return fLeadPFCand.Obj(); }
    PFCandidate const* LeadChargedHadronPFCand() const { return fLeadChargedHadPFCand.Obj(); }
    PFCandidate const* LeadNeutralHadronPFCand() const { return fLeadNeutralPFCand.Obj(); }
    PFJet const* SourcePFJet() const { return fPFJet.Obj(); }
    Jet const* SourceJet() const override { return SourcePFJet(); }
    Track const* ElectronTrack() const { return fElectronTrack.Obj(); }
    PFCandidate const* SignalPFCand(UInt_t i) const { return fSignalPFCands.At(i); }
    PFCandidate const* SignalPFChargedHadrCand(UInt_t i) const { return fSignalPFChargedHadrCands.At(i); }
    PFCandidate const* SignalPFNeutrHadrCand(UInt_t i) const { return fSignalPFNeutrHadrCands.At(i); }
    PFCandidate const* SignalPFGammaCand(UInt_t i) const { return fSignalPFGammaCands.At(i); }
    PFCandidate const* IsoPFCand(UInt_t i) const { return fIsoPFCands.At(i); }

    UInt_t NSignalPFCands() const { return fSignalPFCands.Entries(); }
    UInt_t NSignalPFChargedHadrCands() const { return fSignalPFChargedHadrCands.Entries(); }
    UInt_t NSignalPFNeutrHadrCands() const { return fSignalPFNeutrHadrCands.Entries(); }
    UInt_t NSignalPFGammaCands() const { return fSignalPFGammaCands.Entries(); }
    UInt_t NIsoPFCands() const { return fIsoPFCands.Entries(); }

    EObjType ObjType() const override { return kPFTau; }

    void SetCharge(Char_t x) { fCharge = x; ClearCharge(); }
    void SetLeadPFCandSignD0Sig(Double_t x) { fLeadPFCandSignD0Sig = x; }
    void SetHCalTotalEOverP(Double_t x) { fHCalTotalEOverP = x; }
    void SetHCalMaxEOverP(Double_t x) { fHCalMaxEOverP = x; }
    void SetHCal3x3EOverP(Double_t x) { fHCal3x3EOverP = x; }
    void SetIsoChargedHadronPtSum(Double_t x){ fIsoChargedHadronPtSum = x; }
    void SetIsoGammaEtSum(Double_t x) { fIsoGammaEtSum = x; }
    void SetMaxHCalPFClusterEt(Double_t x) { fMaxHCalPFClusterEt = x; }
    void SetEMFraction(Double_t x) { fEMFraction = x; }
    void SetECalStripSumEOverP(Double_t x) { fECalStripSumEOverP = x; }
    void SetBremRecoveryEOverP(Double_t x) { fBremRecoveryEOverP = x; }
    void SetElectronPreIDOutput(Double_t x) { fElectronPreIDOutput = x; }
    void SetCaloCompatibility(Double_t x) { fCaloCompatibility = x; }
    void SetSegmentCompatibility(Double_t x) { fSegmentCompatibility = x; }
    void SetSignalConeSize(Double_t x) { fSignalConeSize = x; }
    void SetBendCorrMass(Double_t x) { fBendCorrMass = x; }
    void SetElectronPreIDDecision(Bool_t b) { fElectronPreIDDecision = b; }
    void SetMuonDecision(Bool_t b) { fMuonDecision = b; }
    void SetPFTauDiscriminant(Double_t x, UInt_t i) { fPFTauDiscriminants[i] = x; }
    void SetPFTauIdentifier(Bool_t b, UInt_t i) { fPFTauIdentifiers.SetBit(i - nDiscriminants, b); }
    void SetLeadPFCand(PFCandidate const* p) { fLeadPFCand = p; }
    void SetLeadChargedHadronPFCand(PFCandidate const* p) { fLeadChargedHadPFCand = p; }
    void SetLeadNeutralPFCand(PFCandidate const* p) { fLeadNeutralPFCand = p; }
    void SetPFJet(PFJet const* j) { fPFJet = j; }
    void SetElectronTrack(Track const* t) { fElectronTrack = t; }
    void AddSignalPFCand(PFCandidate const* p) { ClearCharge(); fSignalPFCands.Add(p); }
    void AddSignalPFChargedHadrCand(PFCandidate const* p) { fSignalPFChargedHadrCands.Add(p); }
    void AddSignalPFNeutrHadrCand(PFCandidate const* p) { fSignalPFNeutrHadrCands.Add(p); }
    void AddSignalPFGammaCand(PFCandidate const* p) { fSignalPFGammaCands.Add(p); }
    void AddIsoPFCand(PFCandidate const* p) { fIsoPFCands.Add(p); }\
    void SetDecayMode(HadronicDecayMode mode) { fHadronicDecayMode = mode; }

    // Some structural tools
    void Mark(UInt_t i=1) const override;

    static char const* PFTauDiscriminantName(UInt_t);
    static UInt_t PFTauDiscriminantIndex(char const*);
    static char const* PFTauIdentifierName(UInt_t);
    static UInt_t PFTauIdentifierIndex(char const*);

  protected:
    Double_t GetCharge() const override;

    Char_t fCharge = 0; //stored charge
    Double32_t fLeadPFCandSignD0Sig = 0.; //[0,0,14]signed lead track D0 significance
    Double32_t fHCalTotalEOverP = 0.; //[0,0,14]total hcal e / lead ch had pfcand mom
    Double32_t fHCalMaxEOverP = 0.; //[0,0,14]max hcal e / lead ch had pfcand. mom
    Double32_t fHCal3x3EOverP = 0.; //[0,0,14]3x3 hcal e / lead ch hadron pfcand. mom
    Double32_t fIsoChargedHadronPtSum = 0.; //[0,0,14]sum pt of sel. ch had pfcands in iso cone
    Double32_t fIsoGammaEtSum = 0.; //[0,0,14]sum et of sel. photon pfcands in iso cone
    Double32_t fMaxHCalPFClusterEt = 0.; //[0,0,14]et of largest et hcal pfcluster
    Double32_t fEMFraction = 0.; //[0,0,14]em energy fraction
    Double32_t fECalStripSumEOverP = 0.; //[0,0,14]simple brem recovery e / lead ch had mom
    Double32_t fBremRecoveryEOverP = 0.; //[0,0,14]brem recovery E / lead charged hadron P
    Double32_t fElectronPreIDOutput = 0.; //[0,0,14]pfel pre id bdt output to be an el
    Double32_t fCaloCompatibility = 0.; //[0,0,14]calo comp. for this tau to be a muon
    Double32_t fSegmentCompatibility = 0.; //[0,0,14]segment comp. for this tau to be a muon
    Double32_t fSignalConeSize = 0.; // dynamic strip reconstruction has variable cone size
    Double32_t fBendCorrMass = 0.; // mass correction due to the use of dynamic strip
    Bool_t fElectronPreIDDecision = kFALSE; //pf electron pre id decision
    Bool_t fMuonDecision = kFALSE; //pf muon id decision
    Double32_t fPFTauDiscriminants[nDiscriminants]{}; //floating-point valued discriminants
    BitMask64 fPFTauIdentifiers{}; //cut results
    Double_t fPFTauLegacyDiscriminator[nLegacyDiscriminators]{}; //! only for reading old data

    HadronicDecayMode fHadronicDecayMode = nHadronicDecayModes; // decay mode

    Ref<PFCandidate> fLeadPFCand{}; //leading sig pf cand (charged or neutral)
    Ref<PFCandidate> fLeadChargedHadPFCand{}; //leading charged hadron signal pf cand
    Ref<PFCandidate> fLeadNeutralPFCand{}; //leading neutral signal pf cand
    Ref<PFJet> fPFJet{}; //original reconst* ucted pf jet
    Ref<Track> fElectronTrack{}; //track corresp. to pot. matching el cand
    RefArray<PFCandidate> fSignalPFCands{}; //selected pf candidates in signal cone
    RefArray<PFCandidate> fSignalPFChargedHadrCands{}; //signal pf charged hadron candidates
    RefArray<PFCandidate> fSignalPFNeutrHadrCands{}; //signal pf neutral hadron candidates
    RefArray<PFCandidate> fSignalPFGammaCands{}; //signal pf gamma candidates
    RefArray<PFCandidate> fIsoPFCands{}; //selected pf candidates in isolation annulus

    ClassDef(PFTau, 10) // PFTau class
  };
}

inline
Double_t
mithep::PFTau::PFTauDiscriminator(UInt_t idx) const
{
  if (idx < nDiscriminants)
    return fPFTauDiscriminants[idx];
  else if (idx < nDiscriminators)
    return fPFTauIdentifiers.TestBit(idx - nDiscriminants) ? 1. : 0.;
  else if (idx < nAllDiscriminators)
    return fPFTauLegacyDiscriminator[idx - nDiscriminators];
  else
    return 0.;
}

inline
void
mithep::PFTau::Mark(UInt_t ib) const
{
  // mark myself
  mithep::DataObject::Mark(ib);
  // mark my dependencies if they are there
  if (fLeadPFCand .IsValid())
    fLeadPFCand .Obj()->Mark(ib);
  if (fLeadChargedHadPFCand.IsValid())
    fLeadChargedHadPFCand.Obj()->Mark(ib);
  if (fLeadNeutralPFCand .IsValid())
    fLeadNeutralPFCand .Obj()->Mark(ib);
  if (fPFJet .IsValid())
    fPFJet .Obj()->Mark(ib);
  if (fElectronTrack .IsValid())
    fElectronTrack .Obj()->Mark(ib);

  fSignalPFCands .Mark(ib);
  fSignalPFChargedHadrCands.Mark(ib);
  fSignalPFNeutrHadrCands .Mark(ib);
  fSignalPFGammaCands .Mark(ib);
  fIsoPFCands .Mark(ib);
}

inline
Double_t
mithep::PFTau::GetCharge() const
{
  return fCharge;
}


#endif
