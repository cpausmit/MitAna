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

// fPFTauLegacyDiscriminator: array branch with a "!" (not written to file)
// For those branches, pragma read target only recognizes the array name, but GetDataMemberOffset requires the full array length.
// Therefore need to setup the target object by hand in the user code below.

#pragma read \
    sourceClass="mithep::PFTau" \
    version="[-7]" \
    source="Double32_t fRawCombinedIsolationDBSumPtCorr3Hits; \
            Double32_t fDiscriminationByDecayModeFinding; \
            Double32_t fLooseCombinedIsolationDBSumPtCorr3Hits; \
            Double32_t fMediumCombinedIsolationDBSumPtCorr3Hits; \
            Double32_t fTightCombinedIsolationDBSumPtCorr3Hits; \
            Double32_t fDiscriminationAgainstElectron; \
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
    target="fPFTauDiscriminants, fPFTauIdentifiers, fPFTauLegacyDiscriminator" \
    code="{ if (&fPFTauLegacyDiscriminator); \
      static const TString bname(Form(\"fPFTauLegacyDiscriminator[%d]\", mithep::PFTau::nLegacyDiscriminators)); \
      static Long_t actualOffset = cls->GetDataMemberOffset(bname); \
      fPFTauLegacyDiscriminator_t& fActualPFTauLegacyDiscriminator = *(fPFTauLegacyDiscriminator_t*)(target + actualOffset); \
      auto setIdentifier([&fPFTauIdentifiers](UInt_t idx, Double_t value) { fPFTauIdentifiers.SetBit(idx - mithep::PFTau::nDiscriminants, value > 0.5); }); \
      auto setLegacy([&fActualPFTauLegacyDiscriminator](UInt_t idx, Double_t value) { fActualPFTauLegacyDiscriminator[idx - mithep::PFTau::nDiscriminants] = value; }); \
      fPFTauDiscriminants[mithep::PFTau::dByCombinedIsolationDeltaBetaCorrRaw3Hits] = onfile.fRawCombinedIsolationDBSumPtCorr3Hits; \
      setIdentifier(mithep::PFTau::iDecayModeFinding, onfile.fDiscriminationByDecayModeFinding); \
      setIdentifier(mithep::PFTau::iByLooseCombinedIsolationDeltaBetaCorr3Hits, onfile.fLooseCombinedIsolationDBSumPtCorr3Hits); \
      setIdentifier(mithep::PFTau::iByMediumCombinedIsolationDeltaBetaCorr3Hits, onfile.fMediumCombinedIsolationDBSumPtCorr3Hits); \
      setIdentifier(mithep::PFTau::iByTightCombinedIsolationDeltaBetaCorr3Hits, onfile.fTightCombinedIsolationDBSumPtCorr3Hits); \
      setLegacy(mithep::PFTau::kDiscriminationAgainstElectron, onfile.fDiscriminationAgainstElectron); \
      setLegacy(mithep::PFTau::kDiscriminationAgainstMuon, onfile.fDiscriminationAgainstMuon); \
      setLegacy(mithep::PFTau::kDiscriminationByMVAElectronRejection, onfile.fDiscriminationByMVAElectronRejection); \
      setLegacy(mithep::PFTau::kDiscriminationByVLooseIsolation, onfile.fDiscriminationByVLooseIsolation); \
      setLegacy(mithep::PFTau::kDiscriminationByLooseIsolation, onfile.fDiscriminationByLooseIsolation); \
      setLegacy(mithep::PFTau::kDiscriminationByMediumIsolation, onfile.fDiscriminationByMediumIsolation); \
      setLegacy(mithep::PFTau::kDiscriminationByTightIsolation, onfile.fDiscriminationByTightIsolation); \
      setLegacy(mithep::PFTau::kMVA2rawElectronRejection, onfile.fMVA2rawElectronRejection); \
      setLegacy(mithep::PFTau::kMVA2rawElectronRejectionCategory, onfile.fMVA2rawElectronRejectionCategory); \
      setLegacy(mithep::PFTau::kMVA2LooseElectronRejection, onfile.fMVA2LooseElectronRejection); \
      setLegacy(mithep::PFTau::kMVA2MediumElectronRejection, onfile.fMVA2MediumElectronRejection); \
      setLegacy(mithep::PFTau::kMVA2TightElectronRejection, onfile.fMVA2TightElectronRejection); \
      setLegacy(mithep::PFTau::kMVA3rawElectronRejection, onfile.fMVA3rawElectronRejection); \
      setLegacy(mithep::PFTau::kMVA3rawElectronRejectionCategory, onfile.fMVA3rawElectronRejectionCategory); \
      setLegacy(mithep::PFTau::kMVA3LooseElectronRejection, onfile.fMVA3LooseElectronRejection); \
      setLegacy(mithep::PFTau::kMVA3MediumElectronRejection, onfile.fMVA3MediumElectronRejection); \
      setLegacy(mithep::PFTau::kMVA3TightElectronRejection, onfile.fMVA3TightElectronRejection); \
      setLegacy(mithep::PFTau::kMVA3VTightElectronRejection, onfile.fMVA3VTightElectronRejection); \
      setLegacy(mithep::PFTau::kDiscriminationByLooseElectronRejection, onfile.fDiscriminationByLooseElectronRejection); \
      setLegacy(mithep::PFTau::kDiscriminationByMediumElectronRejection, onfile.fDiscriminationByMediumElectronRejection); \
      setLegacy(mithep::PFTau::kDiscriminationByTightElectronRejection, onfile.fDiscriminationByTightElectronRejection); \
      setLegacy(mithep::PFTau::kDiscriminationByLooseMuonRejection, onfile.fDiscriminationByLooseMuonRejection); \
      setLegacy(mithep::PFTau::kDiscriminationByMediumMuonRejection, onfile.fDiscriminationByMediumMuonRejection); \
      setLegacy(mithep::PFTau::kDiscriminationByTightMuonRejection, onfile.fDiscriminationByTightMuonRejection); \
      setLegacy(mithep::PFTau::kDiscriminationByVLooseCombinedIsolationDBSumPtCorr, onfile.fDiscriminationByVLooseCombinedIsolationDBSumPtCorr); \
      setLegacy(mithep::PFTau::kDiscriminationByLooseCombinedIsolationDBSumPtCorr, onfile.fDiscriminationByLooseCombinedIsolationDBSumPtCorr); \
      setLegacy(mithep::PFTau::kDiscriminationByMediumCombinedIsolationDBSumPtCorr, onfile.fDiscriminationByMediumCombinedIsolationDBSumPtCorr); \
      setLegacy(mithep::PFTau::kDiscriminationByTightCombinedIsolationDBSumPtCorr, onfile.fDiscriminationByTightCombinedIsolationDBSumPtCorr); \
      setLegacy(mithep::PFTau::kDiscriminationByRawCombinedIsolationDBSumPtCorr, onfile.fDiscriminationByRawCombinedIsolationDBSumPtCorr); \
      setLegacy(mithep::PFTau::kDiscriminationByLooseMuonRejection2, onfile.fLooseMuonRejection2); \
      setLegacy(mithep::PFTau::kDiscriminationByMediumMuonRejection2, onfile.fMediumMuonRejection2); \
      setLegacy(mithep::PFTau::kDiscriminationByTightMuonRejection2, onfile.fTightMuonRejection2); }" \

// See above for fPFTauLegacyDiscriminator
#pragma read \
    sourceClass="mithep::PFTau" \
    version="[8]" \
    source="Double32_t fPFTauDiscriminator[30];" \
    targetClass="mithep::PFTau" \
    target="fPFTauDiscriminants, fPFTauIdentifiers, fPFTauLegacyDiscriminator" \
    code="{ if (&fPFTauLegacyDiscriminator); \
      static const TString bname(Form(\"fPFTauLegacyDiscriminator[%d]\", mithep::PFTau::nLegacyDiscriminators)); \
      static Long_t actualOffset = cls->GetDataMemberOffset(bname); \
      fPFTauLegacyDiscriminator_t& fActualPFTauLegacyDiscriminator = *(fPFTauLegacyDiscriminator_t*)(target + actualOffset); \
      enum OldDiscriminators { \
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
      auto setIdentifier([&fPFTauIdentifiers, &onfile](UInt_t idIdx, UInt_t disIdx) { fPFTauIdentifiers.SetBit(idIdx - mithep::PFTau::nDiscriminants, onfile.fPFTauDiscriminator[disIdx] > 0.5); }); \
      auto setLegacy([&fActualPFTauLegacyDiscriminator, &onfile](UInt_t legIdx, UInt_t disIdx) { fActualPFTauLegacyDiscriminator[legIdx - mithep::PFTau::nDiscriminators] = onfile.fPFTauDiscriminator[disIdx]; }); \
      fPFTauDiscriminants[mithep::PFTau::dByCombinedIsolationDeltaBetaCorrRaw3Hits] = onfile.fPFTauDiscriminator[lDiscriminationByRawCombinedIsolationDBSumPtCorr3Hits]; \
      setIdentifier(mithep::PFTau::iAgainstMuonLoose3, lDiscriminationByLooseMuonRejection3); \
      setIdentifier(mithep::PFTau::iAgainstMuonTight3, lDiscriminationByTightMuonRejection3); \
      setIdentifier(mithep::PFTau::iDecayModeFinding, lDiscriminationByDecayModeFinding); \
      setIdentifier(mithep::PFTau::iDecayModeFindingNewDMs, lDiscriminationByDecayModeFindingNewDMs); \
      setIdentifier(mithep::PFTau::iByLooseCombinedIsolationDeltaBetaCorr3Hits, lDiscriminationByLooseCombinedIsolationDBSumPtCorr3Hits); \
      setIdentifier(mithep::PFTau::iByMediumCombinedIsolationDeltaBetaCorr3Hits, lDiscriminationByMediumCombinedIsolationDBSumPtCorr3Hits); \
      setIdentifier(mithep::PFTau::iByTightCombinedIsolationDeltaBetaCorr3Hits, lDiscriminationByTightCombinedIsolationDBSumPtCorr3Hits); \
      setLegacy(mithep::PFTau::kDiscriminationByMVA5VLooseElectronRejection, lDiscriminationByMVA5VLooseElectronRejection); \
      setLegacy(mithep::PFTau::kDiscriminationByMVA5LooseElectronRejection, lDiscriminationByMVA5LooseElectronRejection); \
      setLegacy(mithep::PFTau::kDiscriminationByMVA5MediumElectronRejection, lDiscriminationByMVA5MediumElectronRejection); \
      setLegacy(mithep::PFTau::kDiscriminationByMVA5TightElectronRejection, lDiscriminationByMVA5TightElectronRejection); \
      setLegacy(mithep::PFTau::kDiscriminationByLooseElectronRejection, lDiscriminationByLooseElectronRejection); \
      setLegacy(mithep::PFTau::kDiscriminationByMediumElectronRejection, lDiscriminationByMediumElectronRejection); \
      setLegacy(mithep::PFTau::kDiscriminationByTightElectronRejection, lDiscriminationByTightElectronRejection); \
      setLegacy(mithep::PFTau::kDiscriminationByLooseMuonRejection, lDiscriminationByLooseMuonRejection); \
      setLegacy(mithep::PFTau::kDiscriminationByMediumMuonRejection, lDiscriminationByMediumMuonRejection); \
      setLegacy(mithep::PFTau::kDiscriminationByTightMuonRejection, lDiscriminationByTightMuonRejection); \
      setLegacy(mithep::PFTau::kDiscriminationByLooseMuonRejection2, lDiscriminationByLooseMuonRejection2); \
      setLegacy(mithep::PFTau::kDiscriminationByMediumMuonRejection2, lDiscriminationByMediumMuonRejection2); \
      setLegacy(mithep::PFTau::kDiscriminationByTightMuonRejection2, lDiscriminationByTightMuonRejection2); \
      setLegacy(mithep::PFTau::kDiscriminationByDecayModeFindingOldDMs, lDiscriminationByDecayModeFindingOldDMs); \
      setLegacy(mithep::PFTau::kDiscriminationByVLooseCombinedIsolationDBSumPtCorr, lDiscriminationByVLooseCombinedIsolationDBSumPtCorr); \
      setLegacy(mithep::PFTau::kDiscriminationByLooseCombinedIsolationDBSumPtCorr, lDiscriminationByLooseCombinedIsolationDBSumPtCorr); \
      setLegacy(mithep::PFTau::kDiscriminationByMediumCombinedIsolationDBSumPtCorr, lDiscriminationByMediumCombinedIsolationDBSumPtCorr); \
      setLegacy(mithep::PFTau::kDiscriminationByTightCombinedIsolationDBSumPtCorr, lDiscriminationByTightCombinedIsolationDBSumPtCorr); \
      setLegacy(mithep::PFTau::kDiscriminationByRawCombinedIsolationDBSumPtCorr, lDiscriminationByRawCombinedIsolationDBSumPtCorr); \
      setLegacy(mithep::PFTau::kMVA3IsolationChargedIsoPtSum, lMVA3IsolationChargedIsoPtSum); \
      setLegacy(mithep::PFTau::kMVA3IsolationNeutralIsoPtSum, lMVA3IsolationNeutralIsoPtSum); \
      setLegacy(mithep::PFTau::kMVA3IsolationPUcorrPtSum, lMVA3IsolationPUcorrPtSum); }" \

// See above for fPFTauLegacyDiscriminator
#pragma read \
    sourceClass="mithep::PFTau" \
    version="[9]" \
    source="Double32_t fPFTauDiscriminator[20];" \
    targetClass="mithep::PFTau" \
    target="fPFTauDiscriminants, fPFTauIdentifiers, fPFTauLegacyDiscriminator" \
    code="{ if (&fPFTauLegacyDiscriminator); \
      static const TString bname(Form(\"fPFTauLegacyDiscriminator[%d]\", mithep::PFTau::nLegacyDiscriminators)); \
      static Long_t actualOffset = cls->GetDataMemberOffset(bname); \
      fPFTauLegacyDiscriminator_t& fActualPFTauLegacyDiscriminator = *(fPFTauLegacyDiscriminator_t*)(target + actualOffset); \
      enum OldDiscriminators { \
        lDiscriminationByMVA5VLooseElectronRejection, \
        lDiscriminationByMVA5LooseElectronRejection, \
        lDiscriminationByMVA5MediumElectronRejection, \
        lDiscriminationByMVA5TightElectronRejection, \
        lDiscriminationByLooseMuonRejection3,\
        lDiscriminationByTightMuonRejection3, \
        lDiscriminationByDecayModeFinding, \
        lDiscriminationByDecayModeFindingNewDMs, \
        lDiscriminationByLooseCombinedIsolationDBSumPtCorr3Hits, \
        lDiscriminationByMediumCombinedIsolationDBSumPtCorr3Hits, \
        lDiscriminationByTightCombinedIsolationDBSumPtCorr3Hits, \
        lDiscriminationByRawCombinedIsolationDBSumPtCorr3Hits, \
        lDiscriminationByLoosePileupWeightedIsolation3Hits, \
        lDiscriminationByMediumPileupWeightedIsolation3Hits, \
        lDiscriminationByTightPileupWeightedIsolation3Hits, \
        lDiscriminationByRawPileupWeightedIsolation3Hits, \
        lDiscriminationByPhotonPtSumOutsideSignalCone, \
        lChargedIsoPtSum, \
        lNeutralIsoPtSum, \
        lPUcorrPtSum \
      }; \
      auto setDiscriminant([&fPFTauDiscriminants, &onfile](UInt_t newIdx, UInt_t oldIdx) { fPFTauDiscriminants[newIdx] = onfile.fPFTauDiscriminator[oldIdx]; }); \
      auto setIdentifier([&fPFTauIdentifiers, &onfile](UInt_t newIdx, UInt_t oldIdx) { fPFTauIdentifiers.SetBit(newIdx - mithep::PFTau::nDiscriminants, onfile.fPFTauDiscriminator[oldIdx] > 0.5); }); \
      auto setLegacy([&fActualPFTauLegacyDiscriminator, &onfile](UInt_t newIdx, UInt_t oldIdx) { fActualPFTauLegacyDiscriminator[newIdx - mithep::PFTau::nDiscriminators] = onfile.fPFTauDiscriminator[oldIdx]; }); \
      setDiscriminant(mithep::PFTau::dByCombinedIsolationDeltaBetaCorrRaw3Hits, lDiscriminationByRawCombinedIsolationDBSumPtCorr3Hits); \
      setDiscriminant(mithep::PFTau::dPhotonPtSumOutsideSignalCone, lDiscriminationByPhotonPtSumOutsideSignalCone); \
      setDiscriminant(mithep::PFTau::dChargedIsoPtSum, lChargedIsoPtSum); \
      setDiscriminant(mithep::PFTau::dNeutralIsoPtSum, lNeutralIsoPtSum); \
      setDiscriminant(mithep::PFTau::dPuCorrPtSum, lPUcorrPtSum); \
      setIdentifier(mithep::PFTau::iAgainstMuonLoose3, lDiscriminationByLooseMuonRejection3); \
      setIdentifier(mithep::PFTau::iAgainstMuonTight3, lDiscriminationByTightMuonRejection3); \
      setIdentifier(mithep::PFTau::iDecayModeFinding, lDiscriminationByDecayModeFinding); \
      setIdentifier(mithep::PFTau::iDecayModeFindingNewDMs, lDiscriminationByDecayModeFindingNewDMs); \
      setIdentifier(mithep::PFTau::iByLooseCombinedIsolationDeltaBetaCorr3Hits, lDiscriminationByLooseCombinedIsolationDBSumPtCorr3Hits); \
      setIdentifier(mithep::PFTau::iByMediumCombinedIsolationDeltaBetaCorr3Hits, lDiscriminationByMediumCombinedIsolationDBSumPtCorr3Hits); \
      setIdentifier(mithep::PFTau::iByTightCombinedIsolationDeltaBetaCorr3Hits, lDiscriminationByTightCombinedIsolationDBSumPtCorr3Hits); \
      setLegacy(mithep::PFTau::kDiscriminationByMVA5VLooseElectronRejection, lDiscriminationByMVA5VLooseElectronRejection); \
      setLegacy(mithep::PFTau::kDiscriminationByMVA5LooseElectronRejection, lDiscriminationByMVA5LooseElectronRejection); \
      setLegacy(mithep::PFTau::kDiscriminationByMVA5MediumElectronRejection, lDiscriminationByMVA5MediumElectronRejection); \
      setLegacy(mithep::PFTau::kDiscriminationByMVA5TightElectronRejection, lDiscriminationByMVA5TightElectronRejection); \
      setLegacy(mithep::PFTau::kDiscriminationByLoosePileupWeightedIsolation3Hits, lDiscriminationByLoosePileupWeightedIsolation3Hits); \
      setLegacy(mithep::PFTau::kDiscriminationByMediumPileupWeightedIsolation3Hits, lDiscriminationByMediumPileupWeightedIsolation3Hits); \
      setLegacy(mithep::PFTau::kDiscriminationByTightPileupWeightedIsolation3Hits, lDiscriminationByTightPileupWeightedIsolation3Hits); \
      setLegacy(mithep::PFTau::kDiscriminationByRawPileupWeightedIsolation3Hits, lDiscriminationByRawPileupWeightedIsolation3Hits); }" \

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
