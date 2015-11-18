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

#pragma read \
    sourceClass="mithep::PFTau" \
    version="[-7]" \
    source="Double32_t fDiscriminationByDecayModeFinding; \
            Double32_t fLooseCombinedIsolationDBSumPtCorr3Hits; \
            Double32_t fMediumCombinedIsolationDBSumPtCorr3Hits; \
            Double32_t fTightCombinedIsolationDBSumPtCorr3Hits; \
            Double32_t fRawCombinedIsolationDBSumPtCorr3Hits;" \
    targetClass="mithep::PFTau" \
    target="fPFTauDiscriminator" \
    code="{ fPFTauDiscriminator[mithep::PFTau::kDiscriminationByDecayModeFinding] = onfile.fDiscriminationByDecayModeFinding; \
      fPFTauDiscriminator[mithep::PFTau::kDiscriminationByLooseCombinedIsolationDBSumPtCorr3Hits] = onfile.fLooseCombinedIsolationDBSumPtCorr3Hits; \
      fPFTauDiscriminator[mithep::PFTau::kDiscriminationByMediumCombinedIsolationDBSumPtCorr3Hits] = onfile.fMediumCombinedIsolationDBSumPtCorr3Hits; \
      fPFTauDiscriminator[mithep::PFTau::kDiscriminationByTightCombinedIsolationDBSumPtCorr3Hits] = onfile.fTightCombinedIsolationDBSumPtCorr3Hits; \
      fPFTauDiscriminator[mithep::PFTau::kDiscriminationByRawCombinedIsolationDBSumPtCorr3Hits] = onfile.fRawCombinedIsolationDBSumPtCorr3Hits; }" \

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
    code="{ unsigned const dOffset = mithep::PFTau::nDiscriminators; \
      fPFTauLegacyDiscriminator[mithep::PFTau::kDiscriminationAgainstElectron - dOffset] = onfile.fDiscriminationAgainstElectron; \
      fPFTauLegacyDiscriminator[mithep::PFTau::kDiscriminationAgainstMuon - dOffset] = onfile.fDiscriminationAgainstMuon; \
      fPFTauLegacyDiscriminator[mithep::PFTau::kDiscriminationByMVAElectronRejection - dOffset] = onfile.fDiscriminationByMVAElectronRejection; \
      fPFTauLegacyDiscriminator[mithep::PFTau::kDiscriminationByVLooseIsolation - dOffset] = onfile.fDiscriminationByVLooseIsolation; \
      fPFTauLegacyDiscriminator[mithep::PFTau::kDiscriminationByLooseIsolation - dOffset] = onfile.fDiscriminationByLooseIsolation; \
      fPFTauLegacyDiscriminator[mithep::PFTau::kDiscriminationByMediumIsolation - dOffset] = onfile.fDiscriminationByMediumIsolation; \
      fPFTauLegacyDiscriminator[mithep::PFTau::kDiscriminationByTightIsolation - dOffset] = onfile.fDiscriminationByTightIsolation; \
      fPFTauLegacyDiscriminator[mithep::PFTau::kMVA2rawElectronRejection - dOffset] = onfile.fMVA2rawElectronRejection; \
      fPFTauLegacyDiscriminator[mithep::PFTau::kMVA2rawElectronRejectionCategory - dOffset] = onfile.fMVA2rawElectronRejectionCategory; \
      fPFTauLegacyDiscriminator[mithep::PFTau::kMVA2LooseElectronRejection - dOffset] = onfile.fMVA2LooseElectronRejection; \
      fPFTauLegacyDiscriminator[mithep::PFTau::kMVA2MediumElectronRejection - dOffset] = onfile.fMVA2MediumElectronRejection; \
      fPFTauLegacyDiscriminator[mithep::PFTau::kMVA2TightElectronRejection - dOffset] = onfile.fMVA2TightElectronRejection; \
      fPFTauLegacyDiscriminator[mithep::PFTau::kMVA3rawElectronRejection - dOffset] = onfile.fMVA3rawElectronRejection; \
      fPFTauLegacyDiscriminator[mithep::PFTau::kMVA3rawElectronRejectionCategory - dOffset] = onfile.fMVA3rawElectronRejectionCategory; \
      fPFTauLegacyDiscriminator[mithep::PFTau::kMVA3LooseElectronRejection - dOffset] = onfile.fMVA3LooseElectronRejection; \
      fPFTauLegacyDiscriminator[mithep::PFTau::kMVA3MediumElectronRejection - dOffset] = onfile.fMVA3MediumElectronRejection; \
      fPFTauLegacyDiscriminator[mithep::PFTau::kMVA3TightElectronRejection - dOffset] = onfile.fMVA3TightElectronRejection; \
      fPFTauLegacyDiscriminator[mithep::PFTau::kMVA3VTightElectronRejection - dOffset] = onfile.fMVA3VTightElectronRejection; \
      fPFTauLegacyDiscriminator[mithep::PFTau::kDiscriminationByLooseElectronRejection - dOffset] = onfile.fDiscriminationByLooseElectronRejection; \
      fPFTauLegacyDiscriminator[mithep::PFTau::kDiscriminationByMediumElectronRejection - dOffset] = onfile.fDiscriminationByMediumElectronRejection; \
      fPFTauLegacyDiscriminator[mithep::PFTau::kDiscriminationByTightElectronRejection - dOffset] = onfile.fDiscriminationByTightElectronRejection; \
      fPFTauLegacyDiscriminator[mithep::PFTau::kDiscriminationByLooseMuonRejection - dOffset] = onfile.fDiscriminationByLooseMuonRejection; \
      fPFTauLegacyDiscriminator[mithep::PFTau::kDiscriminationByMediumMuonRejection - dOffset] = onfile.fDiscriminationByMediumMuonRejection; \
      fPFTauLegacyDiscriminator[mithep::PFTau::kDiscriminationByTightMuonRejection - dOffset] = onfile.fDiscriminationByTightMuonRejection; \
      fPFTauLegacyDiscriminator[mithep::PFTau::kDiscriminationByVLooseCombinedIsolationDBSumPtCorr - dOffset] = onfile.fDiscriminationByVLooseCombinedIsolationDBSumPtCorr; \
      fPFTauLegacyDiscriminator[mithep::PFTau::kDiscriminationByLooseCombinedIsolationDBSumPtCorr - dOffset] = onfile.fDiscriminationByLooseCombinedIsolationDBSumPtCorr; \
      fPFTauLegacyDiscriminator[mithep::PFTau::kDiscriminationByMediumCombinedIsolationDBSumPtCorr - dOffset] = onfile.fDiscriminationByMediumCombinedIsolationDBSumPtCorr; \
      fPFTauLegacyDiscriminator[mithep::PFTau::kDiscriminationByTightCombinedIsolationDBSumPtCorr - dOffset] = onfile.fDiscriminationByTightCombinedIsolationDBSumPtCorr; \
      fPFTauLegacyDiscriminator[mithep::PFTau::kDiscriminationByRawCombinedIsolationDBSumPtCorr - dOffset] = onfile.fDiscriminationByRawCombinedIsolationDBSumPtCorr; \
      fPFTauLegacyDiscriminator[mithep::PFTau::kDiscriminationByLooseMuonRejection2 - dOffset] = onfile.fLooseMuonRejection2; \
      fPFTauLegacyDiscriminator[mithep::PFTau::kDiscriminationByMediumMuonRejection2 - dOffset] = onfile.fMediumMuonRejection2; \
      fPFTauLegacyDiscriminator[mithep::PFTau::kDiscriminationByTightMuonRejection2 - dOffset] = onfile.fTightMuonRejection2; }" \


#pragma read \
    sourceClass="mithep::PFTau" \
    version="[8]" \
    source="Double32_t fPFTauDiscriminator[30];" \
    targetClass="mithep::PFTau" \
    target="fPFTauDiscriminator, fPFTauLegacyDiscriminator" \
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
      fPFTauDiscriminator[mithep::PFTau::kDiscriminationByMVA5VLooseElectronRejection] = onfile.fPFTauDiscriminator[lDiscriminationByMVA5VLooseElectronRejection]; \
      fPFTauDiscriminator[mithep::PFTau::kDiscriminationByMVA5LooseElectronRejection] = onfile.fPFTauDiscriminator[lDiscriminationByMVA5LooseElectronRejection]; \
      fPFTauDiscriminator[mithep::PFTau::kDiscriminationByMVA5MediumElectronRejection] = onfile.fPFTauDiscriminator[lDiscriminationByMVA5MediumElectronRejection]; \
      fPFTauDiscriminator[mithep::PFTau::kDiscriminationByMVA5TightElectronRejection] = onfile.fPFTauDiscriminator[lDiscriminationByMVA5TightElectronRejection]; \
      fPFTauDiscriminator[mithep::PFTau::kDiscriminationByLooseMuonRejection3] = onfile.fPFTauDiscriminator[lDiscriminationByLooseMuonRejection3]; \
      fPFTauDiscriminator[mithep::PFTau::kDiscriminationByTightMuonRejection3] = onfile.fPFTauDiscriminator[lDiscriminationByTightMuonRejection3]; \
      fPFTauDiscriminator[mithep::PFTau::kDiscriminationByDecayModeFinding] = onfile.fPFTauDiscriminator[lDiscriminationByDecayModeFinding]; \
      fPFTauDiscriminator[mithep::PFTau::kDiscriminationByDecayModeFindingNewDMs] = onfile.fPFTauDiscriminator[lDiscriminationByDecayModeFindingNewDMs]; \
      fPFTauDiscriminator[mithep::PFTau::kDiscriminationByLooseCombinedIsolationDBSumPtCorr3Hits] = onfile.fPFTauDiscriminator[lDiscriminationByLooseCombinedIsolationDBSumPtCorr3Hits]; \
      fPFTauDiscriminator[mithep::PFTau::kDiscriminationByMediumCombinedIsolationDBSumPtCorr3Hits] = onfile.fPFTauDiscriminator[lDiscriminationByMediumCombinedIsolationDBSumPtCorr3Hits]; \
      fPFTauDiscriminator[mithep::PFTau::kDiscriminationByTightCombinedIsolationDBSumPtCorr3Hits] = onfile.fPFTauDiscriminator[lDiscriminationByTightCombinedIsolationDBSumPtCorr3Hits]; \
      fPFTauDiscriminator[mithep::PFTau::kDiscriminationByRawCombinedIsolationDBSumPtCorr3Hits] = onfile.fPFTauDiscriminator[lDiscriminationByRawCombinedIsolationDBSumPtCorr3Hits]; \
      unsigned const dOffset = mithep::PFTau::nDiscriminators; \
      fPFTauLegacyDiscriminator[mithep::PFTau::kDiscriminationByLooseElectronRejection - dOffset] = onfile.fPFTauDiscriminator[lDiscriminationByLooseElectronRejection]; \
      fPFTauLegacyDiscriminator[mithep::PFTau::kDiscriminationByMediumElectronRejection - dOffset] = onfile.fPFTauDiscriminator[lDiscriminationByMediumElectronRejection]; \
      fPFTauLegacyDiscriminator[mithep::PFTau::kDiscriminationByTightElectronRejection - dOffset] = onfile.fPFTauDiscriminator[lDiscriminationByTightElectronRejection]; \
      fPFTauLegacyDiscriminator[mithep::PFTau::kDiscriminationByLooseMuonRejection - dOffset] = onfile.fPFTauDiscriminator[lDiscriminationByLooseMuonRejection]; \
      fPFTauLegacyDiscriminator[mithep::PFTau::kDiscriminationByMediumMuonRejection - dOffset] = onfile.fPFTauDiscriminator[lDiscriminationByMediumMuonRejection]; \
      fPFTauLegacyDiscriminator[mithep::PFTau::kDiscriminationByTightMuonRejection - dOffset] = onfile.fPFTauDiscriminator[lDiscriminationByTightMuonRejection]; \
      fPFTauLegacyDiscriminator[mithep::PFTau::kDiscriminationByLooseMuonRejection2 - dOffset] = onfile.fPFTauDiscriminator[lDiscriminationByLooseMuonRejection2]; \
      fPFTauLegacyDiscriminator[mithep::PFTau::kDiscriminationByMediumMuonRejection2 - dOffset] = onfile.fPFTauDiscriminator[lDiscriminationByMediumMuonRejection2]; \
      fPFTauLegacyDiscriminator[mithep::PFTau::kDiscriminationByTightMuonRejection2 - dOffset] = onfile.fPFTauDiscriminator[lDiscriminationByTightMuonRejection2]; \
      fPFTauLegacyDiscriminator[mithep::PFTau::kDiscriminationByDecayModeFindingOldDMs - dOffset] = onfile.fPFTauDiscriminator[lDiscriminationByDecayModeFindingOldDMs]; \
      fPFTauLegacyDiscriminator[mithep::PFTau::kDiscriminationByVLooseCombinedIsolationDBSumPtCorr - dOffset] = onfile.fPFTauDiscriminator[lDiscriminationByVLooseCombinedIsolationDBSumPtCorr]; \
      fPFTauLegacyDiscriminator[mithep::PFTau::kDiscriminationByLooseCombinedIsolationDBSumPtCorr - dOffset] = onfile.fPFTauDiscriminator[lDiscriminationByLooseCombinedIsolationDBSumPtCorr]; \
      fPFTauLegacyDiscriminator[mithep::PFTau::kDiscriminationByMediumCombinedIsolationDBSumPtCorr - dOffset] = onfile.fPFTauDiscriminator[lDiscriminationByMediumCombinedIsolationDBSumPtCorr]; \
      fPFTauLegacyDiscriminator[mithep::PFTau::kDiscriminationByTightCombinedIsolationDBSumPtCorr - dOffset] = onfile.fPFTauDiscriminator[lDiscriminationByTightCombinedIsolationDBSumPtCorr]; \
      fPFTauLegacyDiscriminator[mithep::PFTau::kDiscriminationByRawCombinedIsolationDBSumPtCorr - dOffset] = onfile.fPFTauDiscriminator[lDiscriminationByRawCombinedIsolationDBSumPtCorr]; \
      fPFTauLegacyDiscriminator[mithep::PFTau::kMVA3IsolationChargedIsoPtSum - dOffset] = onfile.fPFTauDiscriminator[lMVA3IsolationChargedIsoPtSum]; \
      fPFTauLegacyDiscriminator[mithep::PFTau::kMVA3IsolationNeutralIsoPtSum - dOffset] = onfile.fPFTauDiscriminator[lMVA3IsolationNeutralIsoPtSum]; \
      fPFTauLegacyDiscriminator[mithep::PFTau::kMVA3IsolationPUcorrPtSum - dOffset] = onfile.fPFTauDiscriminator[lMVA3IsolationPUcorrPtSum]; }" \

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

