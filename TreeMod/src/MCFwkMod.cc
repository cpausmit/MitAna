#include "MitAna/TreeMod/interface/MCFwkMod.h"

#include "MitAna/DataTree/interface/MCRunInfo.h"
#include "MitAna/DataTree/interface/Names.h"

ClassImp(mithep::MCFwkMod)

mithep::MCFwkMod::MCFwkMod(char const* name, char const* title) :
  BaseMod(name, title),
  fEventInfoName(mithep::Names::gkMCEvtInfoBrn),
  fEventInfo(0),
  fReweightFactors(new mithep::MCReweightFactorArr)
{
  fReweightFactors->SetName("MCReweightFactors");
}

mithep::MCFwkMod::~MCFwkMod()
{
  delete fReweightFactors;
}

void
mithep::MCFwkMod::BeginRun()
{
  mithep::MCRunInfo const* runInfo = GetMCRunInfo();

  if (!runInfo)
    return;

  fWeightGroupCombination.resize(runInfo->NWeights());
  fWeightGroupType.resize(runInfo->NWeights());
  fWeightId.resize(runInfo->NWeights());
  fWeightDef.resize(runInfo->NWeights());

  for (unsigned iG = 0; iG != runInfo->NWeightGroups(); ++iG) {
    TString const& comb = runInfo->WeightGroupCombination(iG);
    TString const& type = runInfo->WeightGroupType(iG);

    for (unsigned iW = 0; iW != runInfo->NWeights(); ++iW) {
      if (runInfo->WeightGroup(iW) != iG)
        continue;

      unsigned pos = runInfo->WeightPositionInEvent(iW);

      fWeightGroupCombination[pos] = comb;
      fWeightGroupType[pos] = type;
      fWeightId[pos] = runInfo->WeightId(iW);
      fWeightDef[pos] = runInfo->WeightDefinition(iW);
    }
  }
}

void
mithep::MCFwkMod::Process()
{
  if (!GetEventHeader()->IsMC())
    return;

  fReweightFactors->Reset();

  LoadBranch(fEventInfoName);

  for (unsigned iW = 0; fEventInfo->NReweightScaleFactors(); ++iW) {
    auto* outFactor = fReweightFactors->AddNew();

    outFactor->SetId(fWeightId[iW]);
    outFactor->SetWeightGroupCombination(fWeightGroupCombination[iW]);
    outFactor->SetWeightGroupType(fWeightGroupType[iW]);
    outFactor->SetWeightDefinition(fWeightDef[iW]);
    outFactor->SetScaleFactor(fEventInfo->ReweightScaleFactor(iW));
  }
}

void
mithep::MCFwkMod::SlaveBegin()
{
  ReqBranch(fEventInfoName, fEventInfo);

  if (!PublishObj(fReweightFactors)) {
    SendError(kAbortAnalysis, "SlaveBegin", 
              "Could not publish MC reweight objects array with name %s.", fReweightFactors->GetName());
    return;
  }
}

void
mithep::MCFwkMod::SlaveTerminate()
{
  RetractObj(fReweightFactors->GetName());
}