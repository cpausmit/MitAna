#include "MitAna/DataTree/interface/FatJet.h"

#include <cstring>

ClassImp(mithep::FatJet)

Double_t
mithep::FatJet::DoubleBJetTagsDisc(UInt_t a) const
{
  if (a < nDoubleBTagAlgos)
    return fDoubleBJetTagsDisc[a];
  else
    return -9999.;
}

void
mithep::FatJet::SetDoubleBJetTagsDisc(Double_t d, UInt_t a)
{
  if (a < nDoubleBTagAlgos)
    fDoubleBJetTagsDisc[a] = d;
}

/*static*/
char const*
mithep::FatJet::DoubleBTagAlgoName(UInt_t idx)
{
  switch(idx) {
  case kBoostedDoubleSecondaryVertex:
    return "BoostedDoubleSecondaryVertex";
  default:
    return "";
  }
}

/*static*/
UInt_t
mithep::FatJet::DoubleBTagAlgoIndex(char const* name)
{
  for (unsigned idx = 0; idx != nDoubleBTagAlgos; ++idx) {
    if (std::strcmp(name, DoubleBTagAlgoName(idx)) == 0)
      return idx;
  }
  return nDoubleBTagAlgos;
}
