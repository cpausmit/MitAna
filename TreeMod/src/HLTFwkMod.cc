#include "MitAna/TreeMod/interface/HLTFwkMod.h"
#include <TFile.h>
#include <TTree.h>
#include "MitAna/DataUtil/interface/Debug.h"
#include "MitAna/DataTree/interface/TriggerName.h"
#include "MitAna/DataTree/interface/TriggerObjectBaseCol.h"
#include "MitAna/DataTree/interface/TriggerObjectRelCol.h"
#include "MitAna/DataTree/interface/TriggerObjectCol.h"

using namespace mithep;

ClassImp(mithep::HLTFwkMod)


//--------------------------------------------------------------------------------------------------
HLTFwkMod::HLTFwkMod(const char *name, const char *title) : 
  BaseMod(name,title),
  fRelsName(Form("%sRelation",fObjsName.Data())),
  fTrigObjs(&fTriggers, fNMaxTriggers)
{
  fTriggers.SetName(fHLTTabName + "Fwk");
  fTriggers.SetOwner();
  fLabels.SetName(fHLTLabName + "Fwk");
  fLabels.SetOwner();
  fTrigObjArr.SetName(fObjsName + "Arr");
  fTrigObjs.SetName(fObjsName + "Fwk");
  fL1Algos.SetName("L1AlgoTableFwk");
  fL1Algos.SetOwner();
  fL1Techs.SetName("L1TechTableFwk");
  fL1Techs.SetOwner();
}

//--------------------------------------------------------------------------------------------------
HLTFwkMod::~HLTFwkMod() 
{
}

//--------------------------------------------------------------------------------------------------
void HLTFwkMod::BeginRun()
{
  // Get HLT tree and set branches if new file was opened. Read next entry in HLT key
  // depending on entry in RunInfo.

  if (fReload) {
    // reset to be (re-)loaded variables
    fReload  =  false;
    fHLTTree =  0;
    fHLTTab  =  0;
    fHLTLab  =  0;
    fCurEnt  = -2;

    // get current file
    TFile *file = GetCurrentFile();
    if (!file) 
      return;

    // get HLT tree
    fHLTTree = dynamic_cast<TTree*>(file->Get(fHLTTreeName));
    if (!fHLTTree) {
      if (fAbortIfNoHLTTree) {
        SendError(kAbortAnalysis, "BeginRun",
                  "Cannot find HLT tree with name %s.", fHLTTreeName.Data());
      }
      else
        return;
    }

    // get HLT trigger name branch
    if (fHLTTree->GetBranch(fHLTTabName)) {
      fHLTTree->SetBranchAddress(fHLTTabName, &fHLTTab);
    }
    else {
      SendError(kAbortAnalysis, "BeginRun",
                "Cannot find HLT tree branch with name %s.", fHLTTabName.Data());
    }

    // get HLT module labels branch
    if (fHLTTree->GetBranch(fHLTLabName)) {
      fHLTTree->SetBranchAddress(fHLTLabName, &fHLTLab);
    }
    else {
      SendError(kAbortAnalysis, "BeginRun",
                "Cannot find HLT tree branch with name %s.", fHLTLabName.Data());
    }
  }

  // get entry for HLT info
  const RunInfo *runinfo = GetRunInfo();
  if (!runinfo) {
    SendError(kAbortAnalysis, "BeginRun",
              "Cannot obtain run info object from selector.");
    return;
  }

  // load trigger table
  if (runinfo->HltEntry()!=fCurEnt) {
    MDB(kAnalysis, 1)
      Info("BeginRun", "Loading trigger table for run %u", runinfo->RunNum());
    
    fCurEnt = runinfo->HltEntry();
    Bool_t load = LoadTriggerTable();
    if (!load) {
      SendError(kAbortAnalysis, "BeginRun",
                "Cannot load trigger table for next entry (%ld).", fCurEnt);
      return;
    }

    MDB(kAnalysis, 2) {
      Info("BeginRun", "Printing tables for run %u", runinfo->RunNum());
      std::cout << " --- Trigger table ---" << std::endl;
      fTriggers.Print();
      std::cout << " --- Module lables ---" << std::endl;
      fLabels.Print();
      if (fL1Algos.GetEntries()) {
        std::cout << " --- L1 Algos ---" << std::endl;
        fL1Algos.Print();
      }
      if (fL1Techs.GetEntries()) {
        std::cout << " --- L1 Techs ---" << std::endl;
        fL1Techs.Print();
      }
    }
  }
}

//--------------------------------------------------------------------------------------------------
Bool_t HLTFwkMod::LoadTriggerTable()
{
  // Load next trigger (and module) table from HLT tree.

  if (fCurEnt<0)
    return kFALSE;

  // delete old tables
  fTriggers.Delete();
  fLabels.Delete();
  fL1Algos.Delete();
  fL1Techs.Delete();

  // load next event in HLT tree
  fHLTTab = 0;
  fHLTLab = 0;
  Int_t ret = fHLTTree->GetEvent(fCurEnt);
  if (ret<0 || fHLTTab==0 || fHLTTab==0 ) {
    SendError(kAbortAnalysis, "LoadTriggerTable", 
              "Could not get trigger data for next entry (%ld).", fCurEnt);
    return kFALSE;
  }

  // check size of trigger table
  if (fHLTTab->size() > fNMaxTriggers) {
    SendError(kAbortAnalysis, "LoadTriggerTable", 
              "Size of trigger table (%ld) larger than maximum (%ld).", 
              fHLTTab->size(), fNMaxTriggers);
    return kFALSE;
  }

  // add trigger names
  for (UInt_t i=0; i<fHLTTab->size(); ++i) {
    TriggerName *tname = new TriggerName(fHLTTab->at(i),i);
    fTriggers.Add(tname);
  } 

  // add module labels
  UInt_t counter = 0;
  UInt_t bitnum  = 0;
  UInt_t which   = 0;
  while (counter<fHLTLab->size()) {
    TString tmpn(fHLTLab->at(counter));
    ++counter;
    if (tmpn.CompareTo("xxx-L1AlgoNames-xxx",TString::kIgnoreCase)==0) {
      bitnum = 0;
      which  = 1;
      continue;
    } else if (tmpn.CompareTo("xxx-L1TechNames-xxx",TString::kIgnoreCase)==0) {
      bitnum = 0;
      which  = 2;
      continue;
    }
    TriggerName *tname = new TriggerName(tmpn,bitnum);
    if (which==0)
      fLabels.Add(tname);
    else if (which==1)
      fL1Algos.Add(tname);
    else 
      fL1Techs.Add(tname);
    ++bitnum;
  } 

  return kTRUE;
}  

//--------------------------------------------------------------------------------------------------
Bool_t HLTFwkMod::Notify()
{
  // Save notification of a new file, which will trigger the reloading of the tables and bitmasks.

  fReload = kTRUE;
  return kTRUE;
}

//--------------------------------------------------------------------------------------------------
void HLTFwkMod::Process()
{
  // Read trigger objects and relation branch and fill our object table.

  if (!fHLTTree)
    return;

  fTrigObjs.Clear();
  fTrigObjArr.Reset();

  auto* objs = GetObject<TriggerObjectBaseArr>(fObjsName);
  auto* rels = GetObject<TriggerObjectRelArr>(fRelsName);

  const UInt_t n = rels->GetEntries();
  for (UInt_t i=0; i<n; ++i) {
    const TriggerObjectRel *rel = rels->At(i);
    if (!rel) continue;

    const TriggerObjectBase *ob = objs->At(rel->ObjInd());
    if (!ob) continue;

    TriggerObject *obj = fTrigObjArr.Allocate();
    new (obj) TriggerObject(rel->TrgId(), rel->Type(), ob->Id(), 
                            ob->Pt(), ob->Eta(), ob->Phi(), ob->Mass());

    obj->SetTagInd(ob->TagInd());
    obj->SetTrigName(fHLTTab->at(rel->TrgId()).c_str());
    obj->SetModuleName(fHLTLab->at(rel->ModInd()).c_str());
    obj->SetFilterName(fHLTLab->at(rel->FilterInd()).c_str());
    if (obj->TagInd() >= 0)
      obj->SetTagName(fHLTLab->at(obj->TagInd()).c_str());
    else
      obj->SetTagName("Unknown");

    fTrigObjs.Add(obj);
  }
}

//--------------------------------------------------------------------------------------------------
void HLTFwkMod::SlaveBegin()
{
  // Request branches for trigger objects and relation, and publish our tables.

  if (fObjsName != Names::gkHltObjBrn)
    fRelsName = Form("%sRelation",fObjsName.Data());

  if (!PublishObj(&fTriggers)) {
    SendError(kAbortAnalysis, "SlaveBegin", 
              "Could not publish HLT trigger table with name %s.", fTriggers.GetName());
    return;
  }
  if (!PublishObj(&fTrigObjArr)) {
    SendError(kAbortAnalysis, "SlaveBegin", 
              "Could not publish HLT trigger objects array with name %s.", fTrigObjArr.GetName());
    return;
  }
  if (!PublishObj(&fTrigObjs)) {
    SendError(kAbortAnalysis, "SlaveBegin", 
              "Could not publish HLT trigger objects table with name %s.", fTrigObjs.GetName());
    return;
  }
  if (!PublishObj(&fLabels)) {
    SendError(kAbortAnalysis, "SlaveBegin", 
              "Could not publish HLT labels with name %s.", fLabels.GetName());
    return;
  }
  if (!PublishObj(&fL1Algos)) {
    SendError(kAbortAnalysis, "SlaveBegin", 
              "Could not publish L1 algo table with name %s.", fL1Algos.GetName());
    return;
  }
  if (!PublishObj(&fL1Techs)) {
    SendError(kAbortAnalysis, "SlaveBegin", 
              "Could not publish L1 tech table with name %s.", fL1Techs.GetName());
    return;
  }
}

//--------------------------------------------------------------------------------------------------
void HLTFwkMod::SlaveTerminate()
{
  // Retract our published objects.

  RetractObj(fTriggers.GetName());
  RetractObj(fLabels.GetName());
  RetractObj(fTrigObjArr.GetName());
  RetractObj(fTrigObjs.GetName());
  RetractObj(fL1Algos.GetName());
  RetractObj(fL1Techs.GetName());
}
