#ifndef MITANA_DATATREE_PFTAUCOLLINKDEF_H
#define MITANA_DATATREE_PFTAUCOLLINKDEF_H

#include "MitAna/DataCont/interface/Ref.h"
#include "MitAna/DataTree/interface/PFTauCol.h"
#endif

#ifndef __CLING__
#   define _R__UNIQUEIDENTIFIER_ PFTauCol
#   define _R__JOIN3_(F,X,Y) _NAME3_(F,X,Y)
#   undef _R__UNIQUE_
#   define _R__UNIQUE_(X) _R__JOIN3_( _R__UNIQUEIDENTIFIER_,X,__LINE__)
#endif

#ifdef __CLING__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ nestedclass;
#pragma link C++ nestedtypedef;
#pragma link C++ namespace mithep;

// old discriminators mapped to current discriminants
#pragma read \
    sourceClass="mithep::PFTau" \
    version="[-7]" \
    source="Double32_t fRawCombinedIsolationDBSumPtCorr3Hits;" \
    targetClass="mithep::PFTau" \
    target="fPFTauDiscriminants" \
    code="{ fPFTauDiscriminants[mithep::PFTau::dByCombinedIsolationDeltaBetaCorrRaw3Hits] = onfile.fRawCombinedIsolationDBSumPtCorr3Hits; }" \

// old discriminators mapped to current identifiers
#pragma read \
    sourceClass="mithep::PFTau" \
    version="[-7]" \
    source="Double32_t fDiscriminationByDecayModeFinding; \
            Double32_t fLooseCombinedIsolationDBSumPtCorr3Hits; \
            Double32_t fMediumCombinedIsolationDBSumPtCorr3Hits; \
            Double32_t fTightCombinedIsolationDBSumPtCorr3Hits;" \
    targetClass="mithep::PFTau" \
    target="fPFTauIdentifiers" \
    code="{ auto set([&fPFTauIdentifiers](UInt_t idx, Double_t value) { fPFTauIdentifiers.SetBit(idx - mithep::PFTau::nDiscriminants, value > 0.5); }); \
      set(mithep::PFTau::iDecayModeFinding, onfile.fDiscriminationByDecayModeFinding); \
      set(mithep::PFTau::iByLooseCombinedIsolationDeltaBetaCorr3Hits, onfile.fLooseCombinedIsolationDBSumPtCorr3Hits); \
      set(mithep::PFTau::iByMediumCombinedIsolationDeltaBetaCorr3Hits, onfile.fMediumCombinedIsolationDBSumPtCorr3Hits); \
      set(mithep::PFTau::iByTightCombinedIsolationDeltaBetaCorr3Hits, onfile.fTightCombinedIsolationDBSumPtCorr3Hits); }" \

// old discriminators mapped to legacy discriminators
#pragma read \
    sourceClass="mithep::PFTau" \
    version="[-7]" \
    source="Double32_t fDiscriminationAgainstElectron; \
            Double32_t fDiscriminationAgainstMuon; \
            Double32_t fDiscriminationByMVAElectronRejection; \
            Double32_t fDiscriminationByVLooseIsolation; \
            Double32_t fDiscriminationByLooseIsolation; \
            Double32_t fDiscriminationByMediumIsolation; \
            Double32_t fDiscriminationByTightIsolation; \
            Double32_t fMVA2rawElectronRejection; \
            Double32_t fMVA2rawElectronRejectionCategory; \
            Double32_t fMVA2LooseElectronRejection; \
            Double32_t fMVA2MediumElectronRejection; \
            Double32_t fMVA2TightElectronRejection; \
            Double32_t fMVA3rawElectronRejection; \
            Double32_t fMVA3rawElectronRejectionCategory; \
            Double32_t fMVA3LooseElectronRejection; \
            Double32_t fMVA3MediumElectronRejection; \
            Double32_t fMVA3TightElectronRejection; \
            Double32_t fMVA3VTightElectronRejection; \
            Double32_t fDiscriminationByLooseElectronRejection; \
            Double32_t fDiscriminationByMediumElectronRejection; \
            Double32_t fDiscriminationByTightElectronRejection; \
            Double32_t fDiscriminationByLooseMuonRejection; \
            Double32_t fDiscriminationByMediumMuonRejection; \
            Double32_t fDiscriminationByTightMuonRejection; \
            Double32_t fDiscriminationByVLooseCombinedIsolationDBSumPtCorr; \
            Double32_t fDiscriminationByLooseCombinedIsolationDBSumPtCorr; \
            Double32_t fDiscriminationByMediumCombinedIsolationDBSumPtCorr; \
            Double32_t fDiscriminationByTightCombinedIsolationDBSumPtCorr; \
            Double32_t fDiscriminationByRawCombinedIsolationDBSumPtCorr; \
            Double32_t fLooseMuonRejection2; \
            Double32_t fMediumMuonRejection2; \
            Double32_t fTightMuonRejection2;" \
    targetClass="mithep::PFTau" \
    target="fPFTauLegacyDiscriminator" \
    code="{ auto set([&fPFTauLegacyDiscriminator](UInt_t idx, Double_t value) { fPFTauLegacyDiscriminator[idx - mithep::PFTau::nDiscriminants] = value; }); \
      set(mithep::PFTau::kDiscriminationAgainstElectron, onfile.fDiscriminationAgainstElectron); \
      set(mithep::PFTau::kDiscriminationAgainstMuon, onfile.fDiscriminationAgainstMuon); \
      set(mithep::PFTau::kDiscriminationByMVAElectronRejection, onfile.fDiscriminationByMVAElectronRejection); \
      set(mithep::PFTau::kDiscriminationByVLooseIsolation, onfile.fDiscriminationByVLooseIsolation); \
      set(mithep::PFTau::kDiscriminationByLooseIsolation, onfile.fDiscriminationByLooseIsolation); \
      set(mithep::PFTau::kDiscriminationByMediumIsolation, onfile.fDiscriminationByMediumIsolation); \
      set(mithep::PFTau::kDiscriminationByTightIsolation, onfile.fDiscriminationByTightIsolation); \
      set(mithep::PFTau::kMVA2rawElectronRejection, onfile.fMVA2rawElectronRejection); \
      set(mithep::PFTau::kMVA2rawElectronRejectionCategory, onfile.fMVA2rawElectronRejectionCategory); \
      set(mithep::PFTau::kMVA2LooseElectronRejection, onfile.fMVA2LooseElectronRejection); \
      set(mithep::PFTau::kMVA2MediumElectronRejection, onfile.fMVA2MediumElectronRejection); \
      set(mithep::PFTau::kMVA2TightElectronRejection, onfile.fMVA2TightElectronRejection); \
      set(mithep::PFTau::kMVA3rawElectronRejection, onfile.fMVA3rawElectronRejection); \
      set(mithep::PFTau::kMVA3rawElectronRejectionCategory, onfile.fMVA3rawElectronRejectionCategory); \
      set(mithep::PFTau::kMVA3LooseElectronRejection, onfile.fMVA3LooseElectronRejection); \
      set(mithep::PFTau::kMVA3MediumElectronRejection, onfile.fMVA3MediumElectronRejection); \
      set(mithep::PFTau::kMVA3TightElectronRejection, onfile.fMVA3TightElectronRejection); \
      set(mithep::PFTau::kMVA3VTightElectronRejection, onfile.fMVA3VTightElectronRejection); \
      set(mithep::PFTau::kDiscriminationByLooseElectronRejection, onfile.fDiscriminationByLooseElectronRejection); \
      set(mithep::PFTau::kDiscriminationByMediumElectronRejection, onfile.fDiscriminationByMediumElectronRejection); \
      set(mithep::PFTau::kDiscriminationByTightElectronRejection, onfile.fDiscriminationByTightElectronRejection); \
      set(mithep::PFTau::kDiscriminationByLooseMuonRejection, onfile.fDiscriminationByLooseMuonRejection); \
      set(mithep::PFTau::kDiscriminationByMediumMuonRejection, onfile.fDiscriminationByMediumMuonRejection); \
      set(mithep::PFTau::kDiscriminationByTightMuonRejection, onfile.fDiscriminationByTightMuonRejection); \
      set(mithep::PFTau::kDiscriminationByVLooseCombinedIsolationDBSumPtCorr, onfile.fDiscriminationByVLooseCombinedIsolationDBSumPtCorr); \
      set(mithep::PFTau::kDiscriminationByLooseCombinedIsolationDBSumPtCorr, onfile.fDiscriminationByLooseCombinedIsolationDBSumPtCorr); \
      set(mithep::PFTau::kDiscriminationByMediumCombinedIsolationDBSumPtCorr, onfile.fDiscriminationByMediumCombinedIsolationDBSumPtCorr); \
      set(mithep::PFTau::kDiscriminationByTightCombinedIsolationDBSumPtCorr, onfile.fDiscriminationByTightCombinedIsolationDBSumPtCorr); \
      set(mithep::PFTau::kDiscriminationByRawCombinedIsolationDBSumPtCorr, onfile.fDiscriminationByRawCombinedIsolationDBSumPtCorr); \
      set(mithep::PFTau::kDiscriminationByLooseMuonRejection2, onfile.fLooseMuonRejection2); \
      set(mithep::PFTau::kDiscriminationByMediumMuonRejection2, onfile.fMediumMuonRejection2); \
      set(mithep::PFTau::kDiscriminationByTightMuonRejection2, onfile.fTightMuonRejection2); }" \

// old discriminators mapped to current discriminants
#pragma read \
    sourceClass="mithep::PFTau" \
    version="[8]" \
    source="Double32_t fPFTauDiscriminator[30];" \
    targetClass="mithep::PFTau" \
    target="fPFTauDiscriminants" \
    code="{ enum OldDiscriminators { \
        lDiscriminationByRawCombinedIsolationDBSumPtCorr3Hits = 26 \
      }; \
      fPFTauDiscriminants[mithep::PFTau::dByCombinedIsolationDeltaBetaCorrRaw3Hits] = onfile.fPFTauDiscriminator[lDiscriminationByRawCombinedIsolationDBSumPtCorr3Hits]; }" \

// old discriminators mapped to current identifiers
#pragma read \
    sourceClass="mithep::PFTau" \
    version="[8]" \
    source="Double32_t fPFTauDiscriminator[30];" \
    targetClass="mithep::PFTau" \
    target="fPFTauIdentifiers" \
    code="{ enum OldDiscriminators { \
        lDiscriminationByLooseMuonRejection3 = 13, \
        lDiscriminationByTightMuonRejection3 = 14, \
        lDiscriminationByDecayModeFinding = 15, \
        lDiscriminationByDecayModeFindingNewDMs = 16, \
        lDiscriminationByLooseCombinedIsolationDBSumPtCorr3Hits = 23, \
        lDiscriminationByMediumCombinedIsolationDBSumPtCorr3Hits = 24, \
        lDiscriminationByTightCombinedIsolationDBSumPtCorr3Hits = 25, \
      }; \
      auto set([&fPFTauIdentifiers, &onfile](UInt_t idIdx, UInt_t disIdx) { fPFTauIdentifiers.SetBit(idIdx - mithep::PFTau::nDiscriminants, onfile.fPFTauDiscriminator[disIdx] > 0.5); }); \
      set(mithep::PFTau::iAgainstMuonLoose3, lDiscriminationByLooseMuonRejection3); \
      set(mithep::PFTau::iAgainstMuonTight3, lDiscriminationByTightMuonRejection3); \
      set(mithep::PFTau::iDecayModeFinding, lDiscriminationByDecayModeFinding); \
      set(mithep::PFTau::iDecayModeFindingNewDMs, lDiscriminationByDecayModeFindingNewDMs); \
      set(mithep::PFTau::iByLooseCombinedIsolationDeltaBetaCorr3Hits, lDiscriminationByLooseCombinedIsolationDBSumPtCorr3Hits); \
      set(mithep::PFTau::iByMediumCombinedIsolationDeltaBetaCorr3Hits, lDiscriminationByMediumCombinedIsolationDBSumPtCorr3Hits); \
      set(mithep::PFTau::iByTightCombinedIsolationDeltaBetaCorr3Hits, lDiscriminationByTightCombinedIsolationDBSumPtCorr3Hits); }" \

// old discriminators mapped to legacy discriminators
#pragma read \
    sourceClass="mithep::PFTau" \
    version="[8]" \
    source="Double32_t fPFTauDiscriminator[30];" \
    targetClass="mithep::PFTau" \
    target="fPFTauLegacyDiscriminator" \
    code="{ enum OldDiscriminators { \
        lDiscriminationByLooseElectronRejection, \
        lDiscriminationByMediumElectronRejection, \
        lDiscriminationByTightElectronRejection, \
        lDiscriminationByMVA5VLooseElectronRejection, \
        lDiscriminationByMVA5LooseElectronRejection, \
        lDiscriminationByMVA5MediumElectronRejection, \
        lDiscriminationByMVA5TightElectronRejection, \
        lDiscriminationByLooseMuonRejection, \
        lDiscriminationByMediumMuonRejection, \
        lDiscriminationByTightMuonRejection, \
        lDiscriminationByLooseMuonRejection2, \
        lDiscriminationByMediumMuonRejection2, \
        lDiscriminationByTightMuonRejection2, \
        lDiscriminationByLooseMuonRejection3, \
        lDiscriminationByTightMuonRejection3, \
        lDiscriminationByDecayModeFinding, \
        lDiscriminationByDecayModeFindingNewDMs, \
        lDiscriminationByDecayModeFindingOldDMs, \
        lDiscriminationByVLooseCombinedIsolationDBSumPtCorr, \
        lDiscriminationByLooseCombinedIsolationDBSumPtCorr, \
        lDiscriminationByMediumCombinedIsolationDBSumPtCorr, \
        lDiscriminationByTightCombinedIsolationDBSumPtCorr, \
        lDiscriminationByRawCombinedIsolationDBSumPtCorr, \
        lDiscriminationByLooseCombinedIsolationDBSumPtCorr3Hits, \
        lDiscriminationByMediumCombinedIsolationDBSumPtCorr3Hits, \
        lDiscriminationByTightCombinedIsolationDBSumPtCorr3Hits, \
        lDiscriminationByRawCombinedIsolationDBSumPtCorr3Hits, \
        lMVA3IsolationChargedIsoPtSum, \
        lMVA3IsolationNeutralIsoPtSum, \
        lMVA3IsolationPUcorrPtSum \
      }; \
      auto set([&fPFTauLegacyDiscriminator, &onfile](UInt_t legIdx, UInt_t disIdx) { fPFTauLegacyDiscriminator[legIdx - mithep::PFTau::nDiscriminators] = onfile.fPFTauDiscriminator[disIdx]; }); \
      set(mithep::PFTau::kDiscriminationByMVA5VLooseElectronRejection, lDiscriminationByMVA5VLooseElectronRejection); \
      set(mithep::PFTau::kDiscriminationByMVA5LooseElectronRejection, lDiscriminationByMVA5LooseElectronRejection); \
      set(mithep::PFTau::kDiscriminationByMVA5MediumElectronRejection, lDiscriminationByMVA5MediumElectronRejection); \
      set(mithep::PFTau::kDiscriminationByMVA5TightElectronRejection, lDiscriminationByMVA5TightElectronRejection); \
      set(mithep::PFTau::kDiscriminationByLooseElectronRejection, lDiscriminationByLooseElectronRejection); \
      set(mithep::PFTau::kDiscriminationByMediumElectronRejection, lDiscriminationByMediumElectronRejection); \
      set(mithep::PFTau::kDiscriminationByTightElectronRejection, lDiscriminationByTightElectronRejection); \
      set(mithep::PFTau::kDiscriminationByLooseMuonRejection, lDiscriminationByLooseMuonRejection); \
      set(mithep::PFTau::kDiscriminationByMediumMuonRejection, lDiscriminationByMediumMuonRejection); \
      set(mithep::PFTau::kDiscriminationByTightMuonRejection, lDiscriminationByTightMuonRejection); \
      set(mithep::PFTau::kDiscriminationByLooseMuonRejection2, lDiscriminationByLooseMuonRejection2); \
      set(mithep::PFTau::kDiscriminationByMediumMuonRejection2, lDiscriminationByMediumMuonRejection2); \
      set(mithep::PFTau::kDiscriminationByTightMuonRejection2, lDiscriminationByTightMuonRejection2); \
      set(mithep::PFTau::kDiscriminationByDecayModeFindingOldDMs, lDiscriminationByDecayModeFindingOldDMs); \
      set(mithep::PFTau::kDiscriminationByVLooseCombinedIsolationDBSumPtCorr, lDiscriminationByVLooseCombinedIsolationDBSumPtCorr); \
      set(mithep::PFTau::kDiscriminationByLooseCombinedIsolationDBSumPtCorr, lDiscriminationByLooseCombinedIsolationDBSumPtCorr); \
      set(mithep::PFTau::kDiscriminationByMediumCombinedIsolationDBSumPtCorr, lDiscriminationByMediumCombinedIsolationDBSumPtCorr); \
      set(mithep::PFTau::kDiscriminationByTightCombinedIsolationDBSumPtCorr, lDiscriminationByTightCombinedIsolationDBSumPtCorr); \
      set(mithep::PFTau::kDiscriminationByRawCombinedIsolationDBSumPtCorr, lDiscriminationByRawCombinedIsolationDBSumPtCorr); \
      set(mithep::PFTau::kMVA3IsolationChargedIsoPtSum, lMVA3IsolationChargedIsoPtSum); \
      set(mithep::PFTau::kMVA3IsolationNeutralIsoPtSum, lMVA3IsolationNeutralIsoPtSum); \
      set(mithep::PFTau::kMVA3IsolationPUcorrPtSum, lMVA3IsolationPUcorrPtSum); }" \

// old discriminators mapped to current discriminants
#pragma read \
    sourceClass="mithep::PFTau" \
    version="[9]" \
    source="Double32_t fPFTauDiscriminator[20];" \
    targetClass="mithep::PFTau" \
    target="fPFTauDiscriminants" \
    code="{ enum OldDiscriminators { \
        lDiscriminationByRawCombinedIsolationDBSumPtCorr3Hits = 11, \
        lDiscriminationByPhotonPtSumOutsideSignalCone = 16, \
        lChargedIsoPtSum = 17, \
        lNeutralIsoPtSum = 18, \
        lPUcorrPtSum = 19 \
      }; \
      auto set([&fPFTauDiscriminants, &onfile](UInt_t newIdx, UInt_t oldIdx) { fPFTauDiscriminants[newIdx] = onfile.fPFTauDiscriminator[oldIdx]; }); \
      set(mithep::PFTau::dByCombinedIsolationDeltaBetaCorrRaw3Hits, lDiscriminationByRawCombinedIsolationDBSumPtCorr3Hits); \
      set(mithep::PFTau::dPhotonPtSumOutsideSignalCone, lDiscriminationByPhotonPtSumOutsideSignalCone); \
      set(mithep::PFTau::dChargedIsoPtSum, lChargedIsoPtSum); \
      set(mithep::PFTau::dNeutralIsoPtSum, lNeutralIsoPtSum); \
      set(mithep::PFTau::dPuCorrPtSum, lPUcorrPtSum); }" \

// old discriminators mapped to current identifiers
#pragma read \
    sourceClass="mithep::PFTau" \
    version="[9]" \
    source="Double32_t fPFTauDiscriminator[20];" \
    targetClass="mithep::PFTau" \
    target="fPFTauIdentifiers" \
    code="{ enum OldDiscriminators { \
        lDiscriminationByLooseMuonRejection3 = 4, \
        lDiscriminationByTightMuonRejection3 = 5, \
        lDiscriminationByDecayModeFinding = 6, \
        lDiscriminationByDecayModeFindingNewDMs = 7, \
        lDiscriminationByLooseCombinedIsolationDBSumPtCorr3Hits = 8, \
        lDiscriminationByMediumCombinedIsolationDBSumPtCorr3Hits = 9, \
        lDiscriminationByTightCombinedIsolationDBSumPtCorr3Hits = 10, \
      }; \
      auto set([&fPFTauIdentifiers, &onfile](UInt_t newIdx, UInt_t oldIdx) { fPFTauIdentifiers.SetBit(newIdx - mithep::PFTau::nDiscriminants, onfile.fPFTauDiscriminator[oldIdx] > 0.5); }); \
      set(mithep::PFTau::iAgainstMuonLoose3, lDiscriminationByLooseMuonRejection3); \
      set(mithep::PFTau::iAgainstMuonTight3, lDiscriminationByTightMuonRejection3); \
      set(mithep::PFTau::iDecayModeFinding, lDiscriminationByDecayModeFinding); \
      set(mithep::PFTau::iDecayModeFindingNewDMs, lDiscriminationByDecayModeFindingNewDMs); \
      set(mithep::PFTau::iByLooseCombinedIsolationDeltaBetaCorr3Hits, lDiscriminationByLooseCombinedIsolationDBSumPtCorr3Hits); \
      set(mithep::PFTau::iByMediumCombinedIsolationDeltaBetaCorr3Hits, lDiscriminationByMediumCombinedIsolationDBSumPtCorr3Hits); \
      set(mithep::PFTau::iByTightCombinedIsolationDeltaBetaCorr3Hits, lDiscriminationByTightCombinedIsolationDBSumPtCorr3Hits); }" \


// old discriminators mapped to legacy discriminators
#pragma read \
    sourceClass="mithep::PFTau" \
    version="[9]" \
    source="Double32_t fPFTauDiscriminator[20];" \
    targetClass="mithep::PFTau" \
    target="fPFTauLegacyDiscriminator" \
    code="{ enum OldDiscriminators { \
        lDiscriminationByMVA5VLooseElectronRejection, \
        lDiscriminationByMVA5LooseElectronRejection, \
        lDiscriminationByMVA5MediumElectronRejection, \
        lDiscriminationByMVA5TightElectronRejection, \
        lDiscriminationByLoosePileupWeightedIsolation3Hits = 11, \
        lDiscriminationByMediumPileupWeightedIsolation3Hits, \
        lDiscriminationByTightPileupWeightedIsolation3Hits, \
        lDiscriminationByRawPileupWeightedIsolation3Hits \
      }; \
      auto set([&fPFTauLegacyDiscriminator, &onfile](UInt_t newIdx, UInt_t oldIdx) { fPFTauLegacyDiscriminator[newIdx - mithep::PFTau::nDiscriminators] = onfile.fPFTauDiscriminator[oldIdx]; }); \
      set(mithep::PFTau::kDiscriminationByMVA5VLooseElectronRejection, lDiscriminationByMVA5VLooseElectronRejection); \
      set(mithep::PFTau::kDiscriminationByMVA5LooseElectronRejection, lDiscriminationByMVA5LooseElectronRejection); \
      set(mithep::PFTau::kDiscriminationByMVA5MediumElectronRejection, lDiscriminationByMVA5MediumElectronRejection); \
      set(mithep::PFTau::kDiscriminationByMVA5TightElectronRejection, lDiscriminationByMVA5TightElectronRejection); \
      set(mithep::PFTau::kDiscriminationByLoosePileupWeightedIsolation3Hits, lDiscriminationByLoosePileupWeightedIsolation3Hits); \
      set(mithep::PFTau::kDiscriminationByMediumPileupWeightedIsolation3Hits, lDiscriminationByMediumPileupWeightedIsolation3Hits); \
      set(mithep::PFTau::kDiscriminationByTightPileupWeightedIsolation3Hits, lDiscriminationByTightPileupWeightedIsolation3Hits); \
      set(mithep::PFTau::kDiscriminationByRawPileupWeightedIsolation3Hits, lDiscriminationByRawPileupWeightedIsolation3Hits); }" \

#pragma link C++ class mithep::PFTau+;
#pragma link C++ class mithep::Collection<mithep::PFTau>+;
#pragma link C++ class mithep::Array<mithep::PFTau>+;
#pragma link C++ class mithep::ObjArray<mithep::PFTau>+;
#pragma link C++ class mithep::Ref<mithep::PFTau>+;
#pragma link C++ typedef mithep::PFTauCol;
#pragma link C++ typedef mithep::PFTauArr;
#pragma link C++ typedef mithep::PFTauOArr;
#pragma link C++ enum mithep::PFTau::Discriminator;
#pragma link C++ enum mithep::PFTau::LegacyDiscriminator;
#endif

