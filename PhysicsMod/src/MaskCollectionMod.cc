#include "MitAna/PhysicsMod/interface/MaskCollectionMod.h"
#include "MitAna/DataTree/interface/ParticleCol.h"
#include "MitAna/DataCont/interface/Types.h"

ClassImp(mithep::MaskCollectionMod)

void
mithep::MaskCollectionMod::Process()
{
  fOutput.Reset();

  auto* input = GetObject<mithep::ParticleCol>(fInputName);
  if (!input) {
    SendError(kAbortAnalysis, "Process", "No particle collection with name " + fInputName + " found.");
    return;
  }

  auto* mask = GetObject<mithep::NFArrBool>(fMaskName);
  if (!mask || mask->GetEntries() != input->GetEntries()) {
    SendError(kAbortAnalysis, "Process", "No mask with name " + fMaskName + " for collection " + fInputName + " found.");
    return;
  }

  for (unsigned iP = 0; iP != input->GetEntries(); ++iP) {
    if (mask->At(iP))
      fOutput.Add(input->At(iP));
  }
}

void
mithep::MaskCollectionMod::SlaveBegin()
{
  PublishObj(&fOutput);
}

void
mithep::MaskCollectionMod::SlaveTerminate()
{
  RetractObj(fOutput.GetName());
}
