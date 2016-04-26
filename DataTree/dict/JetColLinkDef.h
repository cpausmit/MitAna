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

// fBJetTagsLegacyDisc: array branch with a "!" (not written to file)
// For those branches, pragma read target only recognizes the array name, but GetDataMemberOffset requires the full array length.
// Therefore need to setup the target object by hand in the user code below.

#pragma read \
    sourceClass="mithep::Jet" \
    version="[-6]" \
    source="Double32_t fJetProbabilityBJetTagsDisc; \
            Double32_t fJetBProbabilityBJetTagsDisc; \
            Double32_t fSimpleSecondaryVertexHighEffBJetTagsDisc; \
            Double32_t fTrackCountingHighEffBJetTagsDisc; \
            Double32_t fSimpleSecondaryVertexBJetTagsDisc; \
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
    target="fBJetTagsDisc, fBJetTagsLegacyDisc" \
    code="{ if (&fBJetTagsLegacyDisc); \
      static const TString bname(Form(\"fBJetTagsLegacyDisc[%d]\", mithep::Jet::nBTagLegacyAlgos)); \
      static Long_t actualOffset = cls->GetDataMemberOffset(bname); \
      fBJetTagsLegacyDisc_t& fActualBJetTagsLegacyDisc = *(fBJetTagsLegacyDisc_t*)(target + actualOffset); \
      auto setDisc([&fBJetTagsDisc](UInt_t idx, Double_t value) { fBJetTagsDisc[idx] = value; }); \
      auto setLegacy([&fActualBJetTagsLegacyDisc](UInt_t idx, Double_t value) { fActualBJetTagsLegacyDisc[idx - mithep::Jet::nBTagAlgos] = value; }); \
      setDisc(mithep::Jet::kJetProbability, onfile.fJetProbabilityBJetTagsDisc); \
      setDisc(mithep::Jet::kJetBProbability, onfile.fJetBProbabilityBJetTagsDisc); \
      setDisc(mithep::Jet::kSimpleSecondaryVertexHighEff, onfile.fSimpleSecondaryVertexHighEffBJetTagsDisc); \
      setDisc(mithep::Jet::kTrackCountingHighEff, onfile.fTrackCountingHighEffBJetTagsDisc); \
      setLegacy(mithep::Jet::kSimpleSecondaryVertex, onfile.fSimpleSecondaryVertexBJetTagsDisc); \
      setLegacy(mithep::Jet::kSimpleSecondaryVertexHighPur, onfile.fSimpleSecondaryVertexHighPurBJetTagsDisc); \
      setLegacy(mithep::Jet::kCombinedSecondaryVertex, onfile.fCombinedSecondaryVertexBJetTagsDisc); \
      setLegacy(mithep::Jet::kCombinedMVA, onfile.fCombinedSecondaryVertexMVABJetTagsDisc); \
      setLegacy(mithep::Jet::kSoftMuon, onfile.fSoftMuonBJetTagsDisc); \
      setLegacy(mithep::Jet::kSoftMuonByIP3d, onfile.fSoftMuonByIP3dBJetTagsDisc); \
      setLegacy(mithep::Jet::kSoftMuonByPt, onfile.fSoftMuonByPtBJetTagsDisc); \
      setLegacy(mithep::Jet::kSoftElectronByIP3d, onfile.fSoftElectronByIP3dBJetTagsDisc); \
      setLegacy(mithep::Jet::kSoftElectronByPt, onfile.fSoftElectronByPtBJetTagsDisc); \
      setLegacy(mithep::Jet::kGhostTrack, onfile.fGhostTrackBJetTagsDisc); \
      setLegacy(mithep::Jet::kTrackCountingHighPur, onfile.fTrackCountingHighPurBJetTagsDisc); }" \

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

#pragma read \
    sourceClass="mithep::Jet" \
    version="[7]" \
    source="Double32_t fBJetTagsDisc[28];" \
    targetClass="mithep::Jet" \
    target="fBJetTagsDisc, fBJetTagsLegacyDisc" \
    code="{ if (&fBJetTagsLegacyDisc); \
      static const TString bname(Form(\"fBJetTagsLegacyDisc[%d]\", mithep::Jet::nBTagLegacyAlgos)); \
      static Long_t actualOffset = cls->GetDataMemberOffset(bname); \
      fBJetTagsLegacyDisc_t& fActualBJetTagsLegacyDisc = *(fBJetTagsLegacyDisc_t*)(target + actualOffset); \
      enum OldAlgo { \
        lJetProbability, \
        lJetProbabilityNegative, \
        lJetProbabilityPositive, \
        lJetBProbability, \
        lJetBProbabilityNegative, \
        lJetBProbabilityPositive, \
        lSimpleSecondaryVertexHighEff, \
        lSimpleSecondaryVertexHighEffNegative, \
        lSimpleSecondaryVertexHighPur, \
        lSimpleSecondaryVertexHighPurNegative, \
        lCombinedSecondaryVertex, \
        lCombinedSecondaryVertexV2, \
        lCombinedSecondaryVertexV2Positive, \
        lCombinedSecondaryVertexV2Negative, \
        lCombinedSecondaryVertexSoftLepton, \
        lCombinedInclusiveSecondaryVertexV2, \
        lCombinedInclusiveSecondaryVertexV2Positive, \
        lCombinedInclusiveSecondaryVertexV2Negative, \
        lCombinedMVA, \
        lTrackCountingHighEff, \
        lTrackCountingHighPur, \
        lSoftPFMuon, \
        lSoftPFMuonNegative, \
        lSoftPFMuonPositive, \
        lSoftPFElectron, \
        lSoftPFElectronNegative, \
        lSoftPFElectronPositive, \
        lDoubleSecondaryVertex \
      }; \
      auto copyBTag([&fBJetTagsDisc, &onfile](UInt_t newInd, UInt_t oldInd) { fBJetTagsDisc[newInd] = onfile.fBJetTagsDisc[oldInd]; } ); \
      auto copyLegacy([&fActualBJetTagsLegacyDisc, &onfile](unsigned newInd, unsigned oldInd){ fActualBJetTagsLegacyDisc[newInd - mithep::Jet::nBTagAlgos] = onfile.fBJetTagsDisc[oldInd]; }); \
      copyBTag(mithep::Jet::kJetProbability, lJetProbability); \
      copyBTag(mithep::Jet::kJetBProbability, lJetBProbability); \
      copyBTag(mithep::Jet::kSimpleSecondaryVertexHighEff, lSimpleSecondaryVertexHighEff); \
      copyBTag(mithep::Jet::kCombinedSecondaryVertexV2, lCombinedSecondaryVertexV2); \
      copyBTag(mithep::Jet::kCombinedInclusiveSecondaryVertexV2, lCombinedInclusiveSecondaryVertexV2); \
      copyBTag(mithep::Jet::kTrackCountingHighEff, lTrackCountingHighEff); \
      copyBTag(mithep::Jet::kSoftPFMuon, lSoftPFMuon); \
      copyBTag(mithep::Jet::kSoftPFElectron, lSoftPFElectron); \
      copyLegacy(mithep::Jet::kJetProbabilityNegative, lJetProbabilityNegative); \
      copyLegacy(mithep::Jet::kJetProbabilityPositive, lJetProbabilityPositive); \
      copyLegacy(mithep::Jet::kJetBProbabilityNegative, lJetBProbabilityNegative); \
      copyLegacy(mithep::Jet::kJetBProbabilityPositive, lJetBProbabilityPositive); \
      copyLegacy(mithep::Jet::kSimpleSecondaryVertexHighEffNegative, lSimpleSecondaryVertexHighEffNegative); \
      copyLegacy(mithep::Jet::kSimpleSecondaryVertexHighPur, lSimpleSecondaryVertexHighPur); \
      copyLegacy(mithep::Jet::kSimpleSecondaryVertexHighPurNegative, lSimpleSecondaryVertexHighPurNegative); \
      copyLegacy(mithep::Jet::kCombinedSecondaryVertex, lCombinedSecondaryVertex); \
      copyLegacy(mithep::Jet::kCombinedSecondaryVertexV2Positive, lCombinedSecondaryVertexV2Positive); \
      copyLegacy(mithep::Jet::kCombinedSecondaryVertexV2Negative, lCombinedSecondaryVertexV2Negative); \
      copyLegacy(mithep::Jet::kCombinedSecondaryVertexSoftLepton, lCombinedSecondaryVertexSoftLepton); \
      copyLegacy(mithep::Jet::kCombinedInclusiveSecondaryVertexV2Positive, lCombinedInclusiveSecondaryVertexV2Positive); \
      copyLegacy(mithep::Jet::kCombinedInclusiveSecondaryVertexV2Negative, lCombinedInclusiveSecondaryVertexV2Negative); \
      copyLegacy(mithep::Jet::kCombinedMVA, lCombinedMVA); \
      copyLegacy(mithep::Jet::kTrackCountingHighPur, lTrackCountingHighPur); \
      copyLegacy(mithep::Jet::kSoftPFMuonNegative, lSoftPFMuonNegative); \
      copyLegacy(mithep::Jet::kSoftPFMuonPositive, lSoftPFMuonPositive); \
      copyLegacy(mithep::Jet::kSoftPFElectronNegative, lSoftPFElectronNegative); \
      copyLegacy(mithep::Jet::kSoftPFElectronPositive, lSoftPFElectronPositive); \
      copyLegacy(mithep::Jet::kDoubleSecondaryVertex, lDoubleSecondaryVertex); }" \

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



