#include "MitAna/DataTree/interface/Jet.h"

#include <cstring>

ClassImp(mithep::Jet)

mithep::Jet::Jet() :
  fRawMom(),
  fAlpha(0.),
  fBeta(0.),
  fMatchedMCFlavor(0),
  fSigmaEta(0.),
  fSigmaPhi(0.),
  fBJetTagsDisc{},
  fCorrectionScale{},
  fJetArea(0.),
  fCorrections{}
{
  for (unsigned iC = 0; iC != nECorrs; ++iC)
    fCorrectionScale[iC] = -1.;
}

/*static*/
char const*
mithep::Jet::BTagAlgoName(UInt_t idx)
{
  switch(idx) {
  case kJetProbability:
    return "JetProbability";
  case kJetBProbability:
    return "JetBProbability";
  case kSimpleSecondaryVertexHighEff:
    return "SimpleSecondaryVertexHighEff";
  case kSimpleInclusiveSecondaryVertexHighEff:
    return "SimpleInclusiveSecondaryVertexHighEff";
  case kCombinedSecondaryVertexV2:
    return "CombinedSecondaryVertexV2";
  case kCombinedInclusiveSecondaryVertexV2:
    return "CombinedInclusiveSecondaryVertexV2";
  case kCombinedMVAV2:
    return "CombinedMVAV2";
  case kTrackCountingHighEff:
    return "TrackCountingHighEff";
  case kSoftPFMuon:
    return "SoftPFMuon";
  case kSoftPFElectron:
    return "SoftPFElectron";
  case kCombinedCvs:
    return "CombinedCvs";
  case kCombinedCvsLightFlavor:
    return "CombinedCvsL";
  default:
    return "";
  }
}

/*static*/
UInt_t
mithep::Jet::BTagAlgoIndex(char const* name)
{
  for (unsigned idx = 0; idx != nBTagAlgos; ++idx) {
    if (std::strcmp(name, BTagAlgoName(idx)) == 0)
      return idx;
  }
  return nBTagAlgos;
}
