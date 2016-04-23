#include "MitAna/DataTree/interface/PFTau.h"

#include <cstring>

ClassImp(mithep::PFTau)

/*static*/
char const*
mithep::PFTau::PFTauDiscriminantName(UInt_t idx)
{
  switch(idx) {
  case dAgainstElectronMVA6Raw:
    return "AgainstElectronMVA6Raw";
  case dAgainstElectronMVA6category:
    return "AgainstElectronMVA6category";
  case dByCombinedIsolationDeltaBetaCorrRaw3Hits:
    return "ByCombinedIsolationDeltaBetaCorrRaw3Hits";
  case dByIsolationMVArun2v1DBdR03oldDMwLTraw:
    return "ByIsolationMVArun2v1DBdR03oldDMwLTraw";
  case dByIsolationMVArun2v1DBnewDMwLTraw:
    return "ByIsolationMVArun2v1DBnewDMwLTraw";
  case dByIsolationMVArun2v1DBoldDMwLTraw:
    return "ByIsolationMVArun2v1DBoldDMwLTraw";
  case dByIsolationMVArun2v1PWdR03oldDMwLTraw:
    return "ByIsolationMVArun2v1PWdR03oldDMwLTraw";
  case dByIsolationMVArun2v1PWnewDMwLTraw:
    return "ByIsolationMVArun2v1PWnewDMwLTraw";
  case dByIsolationMVArun2v1PWoldDMwLTraw:
    return "ByIsolationMVArun2v1PWoldDMwLTraw";
  case dChargedIsoPtSum:
    return "ChargedIsoPtSum";
  case dChargedIsoPtSumdR03:
    return "ChargedIsoPtSumdR03";
  case dFootprintCorrection:
    return "FootprintCorrection";
  case dFootprintCorrectiondR03:
    return "FootprintCorrectiondR03";
  case dNeutralIsoPtSum:
    return "NeutralIsoPtSum";
  case dNeutralIsoPtSumWeight:
    return "NeutralIsoPtSumWeight";
  case dNeutralIsoPtSumWeightdR03:
    return "NeutralIsoPtSumWeightdR03";
  case dNeutralIsoPtSumdR03:
    return "NeutralIsoPtSumdR03";
  case dPhotonPtSumOutsideSignalCone:
    return "PhotonPtSumOutsideSignalCone";
  case dPhotonPtSumOutsideSignalConedR03:
    return "PhotonPtSumOutsideSignalConedR03";
  case dPuCorrPtSum:
    return "PuCorrPtSum";
  default:
    return "";
  }
}

/*static*/
UInt_t
mithep::PFTau::PFTauDiscriminantIndex(char const* name)
{
  for (unsigned idx = 0; idx != nDiscriminants; ++idx) {
    if (std::strcmp(name, PFTauDiscriminantName(idx)) == 0)
      return idx;
  }
  return nDiscriminants;
}

/*static*/
char const*
mithep::PFTau::PFTauIdentifierName(UInt_t idx)
{
  switch(idx) {
  case iAgainstElectronLooseMVA6:
    return "AgainstElectronLooseMVA6";
  case iAgainstElectronMediumMVA6:
    return "AgainstElectronMediumMVA6";
  case iAgainstElectronTightMVA6:
    return "AgainstElectronTightMVA6";
  case iAgainstElectronVLooseMVA6:
    return "AgainstElectronVLooseMVA6";
  case iAgainstElectronVTightMVA6:
    return "AgainstElectronVTightMVA6";
  case iAgainstMuonLoose3:
    return "AgainstMuonLoose3";
  case iAgainstMuonTight3:
    return "AgainstMuonTight3";
  case iByLooseCombinedIsolationDeltaBetaCorr3Hits:
    return "ByLooseCombinedIsolationDeltaBetaCorr3Hits";
  case iByLooseIsolationMVArun2v1DBdR03oldDMwLT:
    return "ByLooseIsolationMVArun2v1DBdR03oldDMwLT";
  case iByLooseIsolationMVArun2v1DBnewDMwLT:
    return "ByLooseIsolationMVArun2v1DBnewDMwLT";
  case iByLooseIsolationMVArun2v1DBoldDMwLT:
    return "ByLooseIsolationMVArun2v1DBoldDMwLT";
  case iByLooseIsolationMVArun2v1PWdR03oldDMwLT:
    return "ByLooseIsolationMVArun2v1PWdR03oldDMwLT";
  case iByLooseIsolationMVArun2v1PWnewDMwLT:
    return "ByLooseIsolationMVArun2v1PWnewDMwLT";
  case iByLooseIsolationMVArun2v1PWoldDMwLT:
    return "ByLooseIsolationMVArun2v1PWoldDMwLT";
  case iByMediumCombinedIsolationDeltaBetaCorr3Hits:
    return "ByMediumCombinedIsolationDeltaBetaCorr3Hits";
  case iByMediumIsolationMVArun2v1DBdR03oldDMwLT:
    return "ByMediumIsolationMVArun2v1DBdR03oldDMwLT";
  case iByMediumIsolationMVArun2v1DBnewDMwLT:
    return "ByMediumIsolationMVArun2v1DBnewDMwLT";
  case iByMediumIsolationMVArun2v1DBoldDMwLT:
    return "ByMediumIsolationMVArun2v1DBoldDMwLT";
  case iByMediumIsolationMVArun2v1PWdR03oldDMwLT:
    return "ByMediumIsolationMVArun2v1PWdR03oldDMwLT";
  case iByMediumIsolationMVArun2v1PWnewDMwLT:
    return "ByMediumIsolationMVArun2v1PWnewDMwLT";
  case iByMediumIsolationMVArun2v1PWoldDMwLT:
    return "ByMediumIsolationMVArun2v1PWoldDMwLT";
  case iByPhotonPtSumOutsideSignalCone:
    return "ByPhotonPtSumOutsideSignalCone";
  case iByTightCombinedIsolationDeltaBetaCorr3Hits:
    return "ByTightCombinedIsolationDeltaBetaCorr3Hits";
  case iByTightIsolationMVArun2v1DBdR03oldDMwLT:
    return "ByTightIsolationMVArun2v1DBdR03oldDMwLT";
  case iByTightIsolationMVArun2v1DBnewDMwLT:
    return "ByTightIsolationMVArun2v1DBnewDMwLT";
  case iByTightIsolationMVArun2v1DBoldDMwLT:
    return "ByTightIsolationMVArun2v1DBoldDMwLT";
  case iByTightIsolationMVArun2v1PWdR03oldDMwLT:
    return "ByTightIsolationMVArun2v1PWdR03oldDMwLT";
  case iByTightIsolationMVArun2v1PWnewDMwLT:
    return "ByTightIsolationMVArun2v1PWnewDMwLT";
  case iByTightIsolationMVArun2v1PWoldDMwLT:
    return "ByTightIsolationMVArun2v1PWoldDMwLT";
  case iByVLooseIsolationMVArun2v1DBdR03oldDMwLT:
    return "ByVLooseIsolationMVArun2v1DBdR03oldDMwLT";
  case iByVLooseIsolationMVArun2v1DBnewDMwLT:
    return "ByVLooseIsolationMVArun2v1DBnewDMwLT";
  case iByVLooseIsolationMVArun2v1DBoldDMwLT:
    return "ByVLooseIsolationMVArun2v1DBoldDMwLT";
  case iByVLooseIsolationMVArun2v1PWdR03oldDMwLT:
    return "ByVLooseIsolationMVArun2v1PWdR03oldDMwLT";
  case iByVLooseIsolationMVArun2v1PWnewDMwLT:
    return "ByVLooseIsolationMVArun2v1PWnewDMwLT";
  case iByVLooseIsolationMVArun2v1PWoldDMwLT:
    return "ByVLooseIsolationMVArun2v1PWoldDMwLT";
  case iByVTightIsolationMVArun2v1DBdR03oldDMwLT:
    return "ByVTightIsolationMVArun2v1DBdR03oldDMwLT";
  case iByVTightIsolationMVArun2v1DBnewDMwLT:
    return "ByVTightIsolationMVArun2v1DBnewDMwLT";
  case iByVTightIsolationMVArun2v1DBoldDMwLT:
    return "ByVTightIsolationMVArun2v1DBoldDMwLT";
  case iByVTightIsolationMVArun2v1PWdR03oldDMwLT:
    return "ByVTightIsolationMVArun2v1PWdR03oldDMwLT";
  case iByVTightIsolationMVArun2v1PWnewDMwLT:
    return "ByVTightIsolationMVArun2v1PWnewDMwLT";
  case iByVTightIsolationMVArun2v1PWoldDMwLT:
    return "ByVTightIsolationMVArun2v1PWoldDMwLT";
  case iByVVTightIsolationMVArun2v1DBdR03oldDMwLT:
    return "ByVVTightIsolationMVArun2v1DBdR03oldDMwLT";
  case iByVVTightIsolationMVArun2v1DBnewDMwLT:
    return "ByVVTightIsolationMVArun2v1DBnewDMwLT";
  case iByVVTightIsolationMVArun2v1DBoldDMwLT:
    return "ByVVTightIsolationMVArun2v1DBoldDMwLT";
  case iByVVTightIsolationMVArun2v1PWdR03oldDMwLT:
    return "ByVVTightIsolationMVArun2v1PWdR03oldDMwLT";
  case iByVVTightIsolationMVArun2v1PWnewDMwLT:
    return "ByVVTightIsolationMVArun2v1PWnewDMwLT";
  case iByVVTightIsolationMVArun2v1PWoldDMwLT:
    return "ByVVTightIsolationMVArun2v1PWoldDMwLT";
  case iDecayModeFinding:
    return "DecayModeFinding";
  case iDecayModeFindingNewDMs:
    return "DecayModeFindingNewDMs";
  default:
    return "";
  }
}

/*static*/
UInt_t
mithep::PFTau::PFTauIdentifierIndex(char const* name)
{
  for (unsigned idx = nDiscriminants; idx != nDiscriminators; ++idx) {
    if (std::strcmp(name, PFTauIdentifierName(idx)) == 0)
      return idx;
  }
  return nDiscriminators;
}
