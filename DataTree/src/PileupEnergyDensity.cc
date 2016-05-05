#include "MitAna/DataTree/interface/PileupEnergyDensity.h"

#include <cstring>

ClassImp(mithep::PileupEnergyDensity)

/*
  Defining the following functions non-inline for safety measure. Oddities seen in PFTau
  (uses similar scheme with enums and arrays) and gcc >= 5.3.0
*/

Double_t
mithep::PileupEnergyDensity::Rho(UInt_t a/* = mithep::PileupEnergyDensity::kFixedGridAll*/) const
{
  if (a < nAlgos)
    return fRho[a];
  else if (a < nAllAlgos)
    return fRhoLegacy[a - nAlgos];
  else
    return 0.;
}

void
mithep::PileupEnergyDensity::SetRho(Double_t r, UInt_t a/* = mithep::PileupEnergyDensity::kFixedGridAll*/)
{
  if (a < nAlgos)
    fRho[a] = r;
  else if (a < nAllAlgos)
    fRhoLegacy[a - nAlgos] = r;
}

/*static*/
char const*
mithep::PileupEnergyDensity::AlgoName(UInt_t a)
{
  switch (a) {
  case kFixedGridAll:
    return "FixedGridAll";
  case kFixedGridFastjetAll:
    return "FixedGridFastjetAll";
  case kFixedGridFastjetAllCalo:
    return "FixedGridFastjetAllCalo";
  case kFixedGridFastjetCentral:
    return "FixedGridFastjetCentral";
  case kFixedGridFastjetCentralCalo:
    return "FixedGridFastjetCentralCalo";
  case kFixedGridFastjetCentralChargedPileUp:
    return "FixedGridFastjetCentralChargedPileUp";
  case kFixedGridFastjetCentralNeutral:
    return "FixedGridFastjetCentralNeutral";
  case kKt6CaloJets:
    return "Kt6CaloJets";
  case kKt6CaloJetsCentral:
    return "Kt6CaloJetsCentral";
  case kKt6PFJets:
    return "Kt6PFJets";
  case kKt6PFJetsCentralChargedPileUp:
    return "Kt6PFJetsCentralChargedPileUp";
  case kKt6PFJetsCentralNeutral:
    return "Kt6PFJetsCentralNeutral";
  case kKt6PFJetsCentralNeutralTight:
    return "Kt6PFJetsCentralNeutralTight";
  case kHighEta:
    return "HighEta";
  case kLowEta:
    return "LowEta";
  case kRandom:
    return "Random";
  case kRandomLowEta:
    return "RandomLowEta";
  default:
    return "";
  }
}

/*static*/
UInt_t
mithep::PileupEnergyDensity::AlgoIndex(char const* name)
{
  for (unsigned idx = 0; idx != nAllAlgos; ++idx) {
    if (std::strcmp(name, AlgoName(idx)) == 0)
      return idx;
  }
  return nAllAlgos;
}
