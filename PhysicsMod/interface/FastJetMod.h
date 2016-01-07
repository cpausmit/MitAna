//--------------------------------------------------------------------------------------------------
// FastJetMod
//
// This module process a collection of input PFCandidates, cluster them
// using fastjet and then spits out Bambu PFJets collection(s)
//
// Authors: L.DiMatteo, Y.Iiyama
//--------------------------------------------------------------------------------------------------

#ifndef MITANA_MODS_FastJetMod_H
#define MITANA_MODS_FastJetMod_H

#include "MitAna/TreeMod/interface/BaseMod.h"
#include "MitAna/DataTree/interface/ParticleFwd.h"
#include "MitAna/DataTree/interface/PFJetFwd.h"
#include "MitAna/DataTree/interface/GenJetFwd.h"
#include "MitAna/DataTree/interface/Names.h"

namespace fastjet {
  class PseudoJet;
  class JetDefinition;
  class GhostedAreaSpec;
  class AreaDefinition;
}

namespace mithep
{
  class FastJetMod : public BaseMod {
  public:
    FastJetMod(const char* name = "FastJetMod", const char* title = "FastJet module") : BaseMod(name, title) {}
    ~FastJetMod() {}

    const char* GetOutputJetsName() const { return fOutputJetsName; }

    UInt_t GetJetAlgorithm() const { return fJetAlgorithm; }
    void SetJetAlgorithm(UInt_t n) { fJetAlgorithm = n; }

    void SetInputName(const char *n) { fInputName = n; }
    void SetOutputJetsName(const char *n) { fOutputJetsName = n; }
    void SetOutputType(UInt_t t) { fOutputType = t; }

    void SetConeSize(Double_t d) { fJetConeSize = d; }
    void SetNoActiveArea(Bool_t b) { fActiveAreaRepeats = 0; }
    void SetActiveAreaRepeats(UInt_t i) { fActiveAreaRepeats = i; }
    void SetGhostArea(Double_t d) { fGhostArea = d; }
    void SetGhostEtaMax(Double_t d) { fGhostEtaMax = d; }

    void SetParticleMinPt(Double_t d) { fParticleMinPt = d; }
    void SetJetMinPt(Double_t d) { fJetMinPt = d; }

  protected:
    void Process() override;
    void SlaveBegin() override;
    void SlaveTerminate() override;

    // PFJet filler helper
    void FillPFJet(fastjet::PseudoJet const&, PFJet&, ParticleCol const&);
    void FillGenJet(fastjet::PseudoJet const&, GenJet&, ParticleCol const&);

    enum JetAlgorithms {
      kCA,
      kKT,
      kAK
    };

    UInt_t fJetAlgorithm{kAK};

    TString fInputName{Names::gkPFCandidatesBrn}; //(i) name of input (jet constituents) collection
    UInt_t fOutputType{kPFJet};

    TString fOutputJetsName{"FastJet"};
    BaseCollection* fOutputJets{0}; //output jets collection (need to be ParticleOArr to accommodate gen jets)

    // Objects from fastjet we want to use
    Double_t fJetConeSize{0.5}; //fastjet clustering radius
    UInt_t fActiveAreaRepeats{1};
    Double_t fGhostArea{0.01};
    Double_t fGhostEtaMax{7.};

    fastjet::JetDefinition* fJetDef{0}; //fastjet clustering definition
    fastjet::GhostedAreaSpec* fActiveArea{0};
    fastjet::AreaDefinition* fAreaDefinition{0};

    Double_t fParticleMinPt{0.001}; //low cutoff to avoid including unphysical PFCands
    Double_t fJetMinPt{20.}; //low cutoff to carrying on soft jets

    ClassDef(FastJetMod, 0) //FastJet bambu producer
  };
}
#endif
