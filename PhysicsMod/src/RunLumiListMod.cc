#include "MitAna/PhysicsMod/interface/RunLumiListMod.h"

#include "MitAna/DataTree/interface/EventHeader.h"

ClassImp(mithep::RunLumiListMod)

void
mithep::RunLumiListMod::Process()
{
  auto* header = GetEventHeader();
  UInt_t run = header->RunNum();
  UInt_t lumi = header->LumiSec();

  if (run != fRunNumber || lumi != fLumiNumber) {
    if (fRunNumber != 0)
      fListTree->Fill();

    fRunNumber = run;
    fLumiNumber = lumi;
    fNEvents = 0;
  }

  ++fNEvents;
}

void
mithep::RunLumiListMod::SlaveBegin()
{
  fListTree = new TTree("RunLumiList", "RunLumiList");
  fListTree->Branch("run", &fRunNumber, "run/i");
  fListTree->Branch("lumi", &fLumiNumber, "lumi/i");
  fListTree->Branch("nevents", &fNEvents, "nevents/i");

  AddOutput(fListTree);
}

void
mithep::RunLumiListMod::SlaveTerminate()
{
  if (fRunNumber != 0)
    fListTree->Fill();
}
