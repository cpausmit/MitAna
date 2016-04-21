#ifndef MITANA_DATATREE_JETCOLLINKDEF_H
#define MITANA_DATATREE_JETCOLLINKDEF_H

#include "MitAna/DataCont/interface/Ref.h"
#include "MitAna/DataTree/interface/JetCol.h"
#endif

#ifndef __CLING__
#   define _R__UNIQUEIDENTIFIER_ JetCol
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
    sourceClass="mithep::Jet" \
    version="[-6]" \
    source="Double32_t fJetProbabilityBJetTagsDisc; \
            Double32_t fJetBProbabilityBJetTagsDisc; \
            Double32_t fSimpleSecondaryVertexHighEffBJetTagsDisc; \
            Double32_t fTrackCountingHighEffBJetTagsDisc;"
    targetClass="mithep::Jet" \
    target="fBJetTagsDisc" \
    code="{ fBJetTagsDisc[mithep::Jet::kJetProbability] = onfile.fJetProbabilityBJetTagsDisc; \
      fBJetTagsDisc[mithep::Jet::kJetBProbability] = onfile.fJetBProbabilityBJetTagsDisc; \
      fBJetTagsDisc[mithep::Jet::kSimpleSecondaryVertexHighEff] = onfile.fSimpleSecondaryVertexHighEffBJetTagsDisc; \
      fBJetTagsDisc[mithep::Jet::kTrackCountingHighEff] = onfile.fTrackCountingHighEffBJetTagsDisc; }" \

#pragma read \
    sourceClass="mithep::Jet" \
    version="[-6]" \
    source="Double32_t fSimpleSecondaryVertexBJetTagsDisc; \
            Double32_t fSimpleSecondaryVertexHighPurBJetTagsDisc; \
            Double32_t fCombinedSecondaryVertexBJetTagsDisc; \
            Double32_t fCombinedSecondaryVertexMVABJetTagsDisc; \
            Double32_t fSoftMuonBJetTagsDisc; \
            Double32_t fSoftMuonByIP3dBJetTagsDisc; \
            Double32_t fSoftMuonByPtBJetTagsDisc; \
            Double32_t fSoftElectronByIP3dBJetTagsDisc; \
            Double32_t fSoftElectronByPtBJetTagsDisc; \
            Double32_t fGhostTrackBJetTagsDisc; \
            Double32_t fTrackCountingHighPurBJetTagsDisc;" \
    targetClass="mithep::Jet" \
    target="fBJetTagsLegacyDisc" \
    code="{ unsigned const aOffset = mithep::Jet::nBTagAlgos; \
      fBJetTagsLegacyDisc[mithep::Jet::kSimpleSecondaryVertex - aOffset] = onfile.fSimpleSecondaryVertexBJetTagsDisc; \
      fBJetTagsLegacyDisc[mithep::Jet::kSimpleSecondaryVertexHighPur - aOffset] = onfile.fSimpleSecondaryVertexHighPurBJetTagsDisc; \
      fBJetTagsLegacyDisc[mithep::Jet::kCombinedSecondaryVertex - aOffset] = onfile.fCombinedSecondaryVertexBJetTagsDisc; \
      fBJetTagsLegacyDisc[mithep::Jet::kCombinedMVA - aOffset] = onfile.fCombinedSecondaryVertexMVABJetTagsDisc; \
      fBJetTagsLegacyDisc[mithep::Jet::kSoftMuon - aOffset] = onfile.fSoftMuonBJetTagsDisc; \
      fBJetTagsLegacyDisc[mithep::Jet::kSoftMuonByIP3d - aOffset] = onfile.fSoftMuonByIP3dBJetTagsDisc; \
      fBJetTagsLegacyDisc[mithep::Jet::kSoftMuonByPt - aOffset] = onfile.fSoftMuonByPtBJetTagsDisc; \
      fBJetTagsLegacyDisc[mithep::Jet::kSoftElectronByIP3d - aOffset] = onfile.fSoftElectronByIP3dBJetTagsDisc; \
      fBJetTagsLegacyDisc[mithep::Jet::kSoftElectronByPt - aOffset] = onfile.fSoftElectronByPtBJetTagsDisc; \
      fBJetTagsLegacyDisc[mithep::Jet::kGhostTrack - aOffset] = onfile.fGhostTrackBJetTagsDisc; \
      fBJetTagsLegacyDisc[mithep::Jet::kTrackCountingHighPur - aOffset] = onfile.fTrackCountingHighPurBJetTagsDisc; }" \

#pragma read \
    sourceClass="mithep::Jet" \
    version="[-6]" \
    source="Double32_t fL1OffsetCorrectionScale; \
            Double32_t fL2RelativeCorrectionScale; \
            Double32_t fL3AbsoluteCorrectionScale; \
            Double32_t fL4EMFCorrectionScale; \
            Double32_t fL5FlavorCorrectionScale; \
            Double32_t fL6LSBCorrectionScale; \
            Double32_t fL7PartonCorrectionScale; \
            Double32_t fCustomCorrectionScale;" \
    targetClass="mithep::Jet" \
    target="fCorrectionScale" \
    code="{ fCorrectionScale[mithep::Jet::L1] = onfile.fL1OffsetCorrectionScale; \
      fCorrectionScale[mithep::Jet::L2] = onfile.fL2RelativeCorrectionScale; \
      fCorrectionScale[mithep::Jet::L3] = onfile.fL3AbsoluteCorrectionScale; \
      fCorrectionScale[mithep::Jet::L4] = onfile.fL4EMFCorrectionScale; \
      fCorrectionScale[mithep::Jet::L5] = onfile.fL5FlavorCorrectionScale; \
      fCorrectionScale[mithep::Jet::L6] = onfile.fL6LSBCorrectionScale; \
      fCorrectionScale[mithep::Jet::L7] = onfile.fL7PartonCorrectionScale; \
      fCorrectionScale[mithep::Jet::Custom] = onfile.fCustomCorrectionScale; }"

/*
  BTag enum:
  Version 7

*/


#pragma read \
    sourceClass="mithep::Jet" \
    version="[7]" \
    source="Double32_t fBJetTagsDisc[28];" \
    targetClass="mithep::Jet" \
    target="fBJetTagsDisc" \
  code="{ enum OldBTagAlgo {lJetProbability,lJetProbabilityNegative,lJetProbabilityPositive,lJetBProbability,lJetBProbabilityNegative,lJetBProbabilityPositive,lSimpleSecondaryVertexHighEff,lSimpleSecondaryVertexHighEffNegative,lSimpleSecondaryVertexHighPur,lSimpleSecondaryVertexHighPurNegative,lCombinedSecondaryVertex,lCombinedSecondaryVertexV2,lCombinedSecondaryVertexV2Positive,lCombinedSecondaryVertexV2Negative,lCombinedSecondaryVertexSoftLepton,lCombinedInclusiveSecondaryVertexV2,lCombinedInclusiveSecondaryVertexV2Positive,lCombinedInclusiveSecondaryVertexV2Negative,lCombinedMVA,lTrackCountingHighEff,lTrackCountingHighPur,lSoftPFMuon,lSoftPFMuonNegative,lSoftPFMuonPositive,lSoftPFElectron,lSoftPFElectronNegative,lSoftPFElectronPositive,lDoubleSecondaryVertex}; \
      fBJetTagsDisc[mithep::Jet::kJetProbability] = onfile.fBJetTagsDisc[lJetProbability]; \
      fBJetTagsDisc[mithep::Jet::kJetBProbability] = onfile.fBJetTagsDisc[lJetBProbability]; \
      fBJetTagsDisc[mithep::Jet::kSimpleSecondaryVertexHighEff] = onfile.fBJetTagsDisc[lSimpleSecondaryVertexHighEff]; \
      fBJetTagsDisc[mithep::Jet::kCombinedSecondaryVertexV2] = onfile.fBJetTagsDisc[lCombinedSecondaryVertexV2]; \
      fBJetTagsDisc[mithep::Jet::kCombinedInclusiveSecondaryVertexV2] = onfile.fBJetTagsDisc[lCombinedInclusiveSecondaryVertexV2]; \
      fBJetTagsDisc[mithep::Jet::kTrackCountingHighEff] = onfile.fBJetTagsDisc[lTrackCountingHighEff]; \
      fBJetTagsDisc[mithep::Jet::kSoftPFMuon] = onfile.fBJetTagsDisc[lSoftPFMuon]; \
      fBJetTagsDisc[mithep::Jet::kSoftPFElectron] = onfile.fBJetTagsDisc[lSoftPFElectron]; }" \

#pragma read \
    sourceClass="mithep::Jet" \
    version="[7]" \
    source="Double32_t fBJetTagsDisc[28]; \
            Double32_t fBJetTagsLegacyDisc[28];" \
    targetClass="mithep::Jet" \
    target="fBJetTagsLegacyDisc" \
    code="{ enum OldBTagAlgo {lJetProbability,lJetProbabilityNegative,lJetProbabilityPositive,lJetBProbability,lJetBProbabilityNegative,lJetBProbabilityPositive,lSimpleSecondaryVertexHighEff,lSimpleSecondaryVertexHighEffNegative,lSimpleSecondaryVertexHighPur,lSimpleSecondaryVertexHighPurNegative,lCombinedSecondaryVertex,lCombinedSecondaryVertexV2,lCombinedSecondaryVertexV2Positive,lCombinedSecondaryVertexV2Negative,lCombinedSecondaryVertexSoftLepton,lCombinedInclusiveSecondaryVertexV2,lCombinedInclusiveSecondaryVertexV2Positive,lCombinedInclusiveSecondaryVertexV2Negative,lCombinedMVA,lTrackCountingHighEff,lTrackCountingHighPur,lSoftPFMuon,lSoftPFMuonNegative,lSoftPFMuonPositive,lSoftPFElectron,lSoftPFElectronNegative,lSoftPFElectronPositive,lDoubleSecondaryVertex,lSimpleSecondaryVertex,lSoftMuon,lSoftMuonByIP3d,lSoftMuonByPt,lSoftElectronByIP3d,lSoftElectronByPt,lGhostTrack}; \
      auto copyBtag([&fBJetTagsLegacyDisc, &onfile](unsigned newInd, unsigned oldInd){ \
        if (oldInd >= oOffset)                                          \
          fBJetTagsLegacyDisc[newInd - mithep::Jet::nBTagAlgos] = onfile.fBJetTagsLegacyDisc[oldInd - lSimpleSecondaryVertex]; \
        else                                                            \
          fBJetTagsLegacyDisc[newInd - mithep::Jet::nBTagAlgos] = onfile.fBJetTagsDisc[oldInd]; \
      });                                                               \
      copyBtag(mithep::Jet::kSimpleSecondaryVertex, lSimpleSecondaryVertex); \
      copyBtag(mithep::Jet::kSoftMuon, lSoftMuon); \
      copyBtag(mithep::Jet::kSoftMuonByIP3d, lSoftMuonByIP3d); \
      copyBtag(mithep::Jet::kSoftMuonByPt, lSoftMuonByPt); \
      copyBtag(mithep::Jet::kSoftElectronByIP3d, lSoftElectronByIP3d); \
      copyBtag(mithep::Jet::kSoftElectronByPt, lSoftElectronByPt); \
      copyBtag(mithep::Jet::kGhostTrack, lGhostTrack); \
      copyBtag(mithep::Jet::kJetProbabilityNegative, lJetProbabilityNegative); \
      copyBtag(mithep::Jet::kJetProbabilityPositive, lJetProbabilityPositive); \
      copyBtag(mithep::Jet::kJetBProbabilityNegative, lJetBProbabilityNegative); \
      copyBtag(mithep::Jet::kJetBProbabilityPositive, lJetBProbabilityPositive); \
      copyBtag(mithep::Jet::kSimpleSecondaryVertexHighEffNegative, lSimpleSecondaryVertexHighEffNegative); \
      copyBtag(mithep::Jet::kSimpleSecondaryVertexHighPur, lSimpleSecondaryVertexHighPur); \
      copyBtag(mithep::Jet::kSimpleSecondaryVertexHighPurNegative, lSimpleSecondaryVertexHighPurNegative); \
      copyBtag(mithep::Jet::kCombinedSecondaryVertex, lCombinedSecondaryVertex); \
      copyBtag(mithep::Jet::kCombinedSecondaryVertexV2Positive, lCombinedSecondaryVertexV2Positive); \
      copyBtag(mithep::Jet::kCombinedSecondaryVertexV2Negative, lCombinedSecondaryVertexV2Negative); \
      copyBtag(mithep::Jet::kCombinedSecondaryVertexSoftLepton, lCombinedSecondaryVertexSoftLepton); \
      copyBtag(mithep::Jet::kCombinedInclusiveSecondaryVertexV2Positive, lCombinedInclusiveSecondaryVertexV2Positive); \
      copyBtag(mithep::Jet::kCombinedInclusiveSecondaryVertexV2Negative, lCombinedInclusiveSecondaryVertexV2Negative); \
      copyBtag(mithep::Jet::kCombinedMVA, lCombinedMVA); \
      copyBtag(mithep::Jet::kTrackCountingHighPur, lTrackCountingHighPur); \
      copyBtag(mithep::Jet::kSoftPFMuonNegative, lSoftPFMuonNegative); \
      copyBtag(mithep::Jet::kSoftPFMuonPositive, lSoftPFMuonPositive); \
      copyBtag(mithep::Jet::kSoftPFElectronNegative, lSoftPFElectronNegative); \
      copyBtag(mithep::Jet::kSoftPFElectronPositive, lSoftPFElectronPositive); \
      copyBtag(mithep::Jet::kDoubleSecondaryVertex, lDoubleSecondaryVertex); }" \

#pragma link C++ class mithep::Jet+;
#pragma link C++ class mithep::Collection<mithep::Jet>+;
#pragma link C++ class mithep::Array<mithep::Jet>+;
#pragma link C++ class mithep::ObjArray<mithep::Jet>+;
#pragma link C++ class mithep::Ref<mithep::Jet>+;
#pragma link C++ typedef mithep::JetCol;
#pragma link C++ typedef mithep::JetArr;
#pragma link C++ typedef mithep::JetOArr;
#pragma link C++ enum mithep::Jet::BTagAlgo;
#pragma link C++ enum mithep::Jet::BTagLegacyAlgo;
#endif



