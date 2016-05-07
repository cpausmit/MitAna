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

/*
  Defining b-tag functions non-inline for safety measure. Oddities seen in PFTau
  (uses similar scheme with enums and arrays) and gcc >= 5.3.0
*/

Double_t
mithep::Jet::BJetTagsDisc(UInt_t a) const
{
  if (a < nBTagAlgos)
    return fBJetTagsDisc[a];
  else if (a < nAllBTagAlgos)
    return fBJetTagsLegacyDisc[a - nBTagAlgos];
  else
    return -9999.;
}

void
mithep::Jet::SetBJetTagsDisc(Double_t d, UInt_t a)
{
  if (a < nBTagAlgos)
    fBJetTagsDisc[a] = d;
  else if (a < nAllBTagAlgos)
    fBJetTagsLegacyDisc[a - nBTagAlgos] = d;
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
