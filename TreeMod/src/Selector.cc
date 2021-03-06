#include "MitAna/TreeMod/interface/Selector.h"
#include "MitAna/TreeMod/interface/OutputMod.h"
#include "MitAna/DataTree/interface/Names.h"
#include "MitAna/DataTree/interface/EventHeader.h" 
#include "MitAna/DataTree/interface/RunInfo.h" 
#include "MitAna/DataTree/interface/MCRunInfo.h" 
#include "MitAna/DataTree/interface/LAHeader.h" 
#include "MitAna/DataUtil/interface/Cacher.h"
#include "MitAna/Utils/interface/StreamerCorrection.h"
#include "MitAna/TAM/interface/TAMVirtualBranchLoader.h"
#include <TProcessID.h>
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TROOT.h>

#include <stdexcept>

using namespace mithep;

ClassImp(mithep::Selector)

//--------------------------------------------------------------------------------------------------
Selector::Selector() : 
  fDoRunInfo     (kTRUE),
  fUseCacher     (0),
  fCacherTimeout (1800),
  fCacher        (0),
  fEvtHdrName    (Names::gkEvtHeaderBrn),
  fRunTreeName   (Names::gkRunTreeName),
  fRunInfoName   (Names::gkRunInfoBrn),
  fMCRunInfoName (Names::gkMCRunInfoBrn),
  fAllEvtHdrBrn  (Names::gkAllEvtHeaderBrn),
  fLATreeName    (Names::gkLATreeName),
  fLAHdrName     (Names::gkLAHeaderBrn),
  fAllEvtTreeName(Names::gkAllEvtTreeName),
  fRunTree       (0),
  fEventHeader   (0),
  fRunInfo       (0),
  fMCRunInfo     (0),
  fCurRunNum     (UInt_t(-1)),
  fTrash         (0)
{
  // Constructor.

  fOutputMods.SetOwner(kFALSE);
  fTrash.SetOwner(kTRUE);
  gROOT->GetListOfSpecials()->Add(this);
}

//--------------------------------------------------------------------------------------------------
Selector::~Selector()
{
  // Destructor.

  gROOT->GetListOfSpecials()->Remove(this);
}

//--------------------------------------------------------------------------------------------------
Bool_t Selector::ConsistentRunNum() const
{
  return (ValidRunNum() && fCurRunNum==fEventHeader->RunNum());
}

//--------------------------------------------------------------------------------------------------
Bool_t Selector::ValidRunInfo() const
{
  return (fRunInfo && fCurRunNum==fRunInfo->RunNum());
}

//--------------------------------------------------------------------------------------------------
Bool_t Selector::BeginRun()
{
  // Determines whether we are at beginning of a new run.

  if (!fDoRunInfo) 
    return kFALSE;

  LoadBranch(fEvtHdrName);
  if (!fEventHeader) 
    return kFALSE;

  if (ConsistentRunNum())
    return kFALSE;

  UpdateRunInfo();
  return ValidRunNum();
}

//--------------------------------------------------------------------------------------------------
Bool_t Selector::EndRun()
{
  // Determines whether we are at the end of a run. Also, do treat special case of output module 
  // here so that in any case it can process the event.

  if (IsAModAborted() || IsEventAborted()) {
    // deal with output module if needed: Do this here, avoids having to copy/rewrite large parts of
    // TAMSelector::Process and interaction with TAModule
    for (auto* mod : fOutputMods)
      static_cast<OutputMod*>(mod)->ProcessAll();
  }

  if (!fDoRunInfo) 
    return kFALSE;

  if (!ValidRunNum())
    return kFALSE;

  // determine if run will end
  if (fCurEvt + 1 == fTree->GetTree()->GetEntries())
    return kTRUE; // we are at last entry in current file

  if (fRunTransitions.back() == fCurEvt) {
    fRunTransitions.pop_back();
    return kTRUE;
  }

  return kFALSE;
}

//--------------------------------------------------------------------------------------------------
Bool_t Selector::Notify()
{
  // The Notify() function is called when a new file is opened.  Here, we take care of file caching and
  // check for a new run info tree.

  if (!GetCurrentFile()) 
    return kTRUE;

  if (GetCurrentFile()->GetVersion() < 60000) {
    // Bambu files written in ROOT 5 had wrong type names for templated class members, e.g. in Electron class
    //  mithep::Electron::Ref<Track> fGsfTrackRef
    // where it has to be
    //  mithep::Ref::<mithep::Track> fGsfTrackRef
    // Correct streamer info using the information from the first file. This is a one-time operation,
    // i.e. in principle doing this on the first file is sufficient. Nevertheless we call the function
    // in each Notify() in the interest of avoiding introducing another flag data member to the class.
    StreamerCorrection::CorrectStreamerInfo(GetCurrentFile());
  }

  if (fDoRunInfo) 
    UpdateRunInfoTree();

  // make sure to keep files cached
  if (fCacher) {
    if (!fCacher->NextCaching()) {
      Error("Notify", "File caching failed.");
      return kFALSE;
    }
  }
    
  return TAMSelector::Notify();
}

//--------------------------------------------------------------------------------------------------
Bool_t Selector::Process(Long64_t entry)
{
  // Process an element of the tree.

  Bool_t ret = TAMSelector::Process(entry);
  fTrash.Delete();

  // clear collections cached through GetObject<Collection<O>>
  auto* sItr = fObjInfoStore.MakeIterator();
  ObjInfo* info = 0;
  while ((info = static_cast<ObjInfo*>(sItr->Next()))) {
    for (auto&& col : info->fCollections) {
      col.first->Reset();
      col.second = false;
    }
  }

  if (fCacher && fTree && fTree->GetTree()->GetReadEntry() == fTree->GetTree()->GetEntries() - 1) {
    // process has reached the end of the current file
    if (!fCacher->WaitForNextFile()) {
      Error("Process", "Next file could not be cached.");
      AbortAnalysis();
    }
  }

  return ret;
}

//--------------------------------------------------------------------------------------------------
void Selector::SlaveBegin(TTree *tree)
{
  // The SlaveBegin() function is called after the Begin() function and can be used to setup
  // analysis on the slaves. Here, we request the event header branch.

  // perfrom initial caching before we get rolling
  if (fUseCacher > 0) {
    TChain* chain = dynamic_cast<TChain*>(tree);
    if (chain) {
      TList inputList;
      inputList.SetOwner();
      for (TObject* obj : *chain->GetListOfFiles())
        inputList.Add(new TObjString(obj->GetTitle()));

      fCacher = new Cacher(&inputList, fUseCacher == 2); // 2: full-local caching
      fCacher->SetNFilesAhead(1); // do not download too many files locally
      fCacher->SetTimeout(fCacherTimeout);
      if (!fCacher->InitialCaching()) {
        Error("SlaveBegin", "Initial cache failed.");
        AbortAnalysis();
      }
    }
  }

  if (fDoRunInfo)
    ReqBranch(fEvtHdrName, fEventHeader);

  TAMSelector::SlaveBegin(tree);
}

//--------------------------------------------------------------------------------------------------
void Selector::SlaveTerminate()
{
  // Clean leftovers in cache
  if (fCacher) {
    fCacher->CleanCache();
    delete fCacher;
  }

  TAMSelector::SlaveTerminate();
}

//--------------------------------------------------------------------------------------------------
void Selector::UpdateRunInfo() 
{
  // Update the run info to be consistent with the information from the event header.

  fRunInfo   = 0;
  fMCRunInfo = 0;

  if (!fRunTree) 
    return;

  // Int_t ret = fRunTree->GetEvent(fEventHeader->RunEntry());
  // if (ret<0 || fRunInfo==0) {
  //   Error("UpdateRunInfo", "Error updating run info for run %d, entry %d, return value %d", 
  //         fEventHeader->RunNum(), fEventHeader->RunEntry(), ret);
  //   return;
  // }

  // fCurRunNum = fEventHeader->RunNum();
  // if (!ValidRunInfo()) {
  //   Error("UpdateRunInfo", "Error updating run info, run values do not match %d %d", 
  //         fCurRunNum, fRunInfo->RunNum());
  //   return;
  // }

  // patch for 043: working around a bug in production
  Int_t runEntry = fEventHeader->RunEntry();
  do {
    fEventHeader->SetRunEntry(runEntry++);

    Int_t ret = fRunTree->GetEvent(fEventHeader->RunEntry());
    if (ret<0 || fRunInfo==0) {
      Error("UpdateRunInfo", "Error updating run info for run %d, entry %d, return value %d", 
            fEventHeader->RunNum(), fEventHeader->RunEntry(), ret);
      return;
    }

    fCurRunNum = fEventHeader->RunNum();
  }
  while (!ValidRunInfo());
}

//--------------------------------------------------------------------------------------------------
void Selector::UpdateRunInfoTree() 
{
  // Get the run info tree from current file and set our branches.

  // first erase previous entries
  fRunTree   = 0;
  fRunInfo   = 0;
  fMCRunInfo = 0;
  fCurRunNum = UInt_t(-1);

  // get current file and retrieve trees
  TFile *f = GetCurrentFile();
  if (!f) {
    Error("UpdateRunInfoTree", "Ptr to current file is null");
    return;
  }

  // run info tree
  fRunTree = dynamic_cast<TTree*>(f->Get(fRunTreeName));
  if (!fRunTree)
    Fatal("UpdateRunInfoTree", "Cannot find run info tree with name %s", fRunTreeName.Data());

  // set branches 
  if (fRunTree->GetBranch(fRunInfoName))
    fRunTree->SetBranchAddress(fRunInfoName, &fRunInfo);
  else
    Fatal("UpdateRunInfoTree", "Cannot find run info branch with name %s", fRunInfoName.Data());

  if (fRunTree->GetBranch(fMCRunInfoName))
    fRunTree->SetBranchAddress(fMCRunInfoName, &fMCRunInfo);

  fRunTransitions.clear();
  // look-ahead tree
  // to be deprecated in the future. Keep these lines for backward compatibility!
  auto* laTree = dynamic_cast<TTree*>(f->Get(fLATreeName));

  if (laTree) {
    if (!laTree->GetBranch(fLAHdrName))
      Fatal("UpdateRunInfoTree", "Cannot find look-ahead branch with name %s", fLAHdrName.Data());

    Long64_t iLAEntry = 0;    

    if (laTree->GetEntries() > 0) {
      auto* lah = new LAHeader;
      laTree->SetBranchAddress(fLAHdrName, &lah);

      if (laTree->GetEntry(iLAEntry) <= 0)
        Fatal("UpdateRunInfoTree", "Look-ahead tree empty or corrupted");

      UInt_t currentRun = lah->RunNum();

      ++iLAEntry;

      while (laTree->GetEntry(iLAEntry) > 0) {
        if (lah->RunNum() != currentRun) {
          // LATree count is off +1 wrt Events tree
          // Entry for the first event of a run in LATree is the entry for the last event of the previous run in Events
          // Insert entry numbers in reverse order so that back() corresponds to the next run transition event
          fRunTransitions.insert(fRunTransitions.begin(), iLAEntry);
          currentRun = lah->RunNum();
        }
        ++iLAEntry;
      }

      laTree->ResetBranchAddresses();
      delete lah;
    }

    fRunTransitions.insert(fRunTransitions.begin(), iLAEntry);

    delete laTree;
  }
  else {
    // TODO implement alternative here
    Fatal("UpdateRunInfoTree", "Cannot find look-ahead tree with name %s", fLATreeName.Data());
  }
}

TObject*
mithep::Selector::GetObjectImpl(TClass const* cl, char const* name, Bool_t warn)
{
  // Find object of the given name from the list of event objects,
  // public objects, and branches in this order.

  auto* info = static_cast<ObjInfo*>(fObjInfoStore.FindObject(name));

  TObject* obj = 0;

  if (info) {
    if (info->IsEvtObject()) {
      obj = FindObjThisEvt(name);
      if (obj && !obj->IsA()->InheritsFrom(cl))
        obj = 0;

      if (warn && !obj)
        Warning("GetObject", "Object %s of type %s is registered as an event object but was not found in the current event.", name, cl->GetName());

      // for event object, null result does not imply broken object info
      return obj;
    }
    else if (info->IsPublic() || info->IsBranch()) {
      if (!info->fObjType->InheritsFrom(cl)) {
        if (warn)
          Warning("GetObject", "Type mismatch (%s != %s) for requested object %s.", cl->GetName(), info->fObjType->GetName(), name);

        return 0;
      }

      if (info->IsBranch())
        LoadBranch(info->fBranchInfo);

      obj = reinterpret_cast<TObject*>(info->fAddr);

      if (obj)
        return obj;
    }

    // public or branch but null, or no source type set -> broken. Delete and create a new one
    fObjInfoStore.Remove(info);
    delete info;
  }

  info = new ObjInfo(name);

  obj = FindObjThisEvt(name);

  if (obj && obj->IsA()->InheritsFrom(cl)) {
    info->fSourceType = ObjInfo::kEvtObject;
  }
  else {
    obj = FindPublicObj(name);
    if (obj && obj->IsA()->InheritsFrom(cl)) {
      info->fSourceType = ObjInfo::kPublic;
      info->fAddr = reinterpret_cast<BranchAddr_t>(obj);
    }
    else {
      auto* branchInfo = static_cast<TAMBranchInfo*>(fBranchTable.FindObject(name));
      bool newBranchInfo = false;
      if (!branchInfo) {
        branchInfo = new TAMBranchInfo(name);
        newBranchInfo = true;
      }

      if (!branchInfo->GetLoader() && (!FindLoader(branchInfo) || !branchInfo->Notify(fTree))) {
        // warning already issued by FindLoader
        delete info;
        if (newBranchInfo)
          delete branchInfo;
        return 0;
      }

      // TAMBranchInfo::fClass loaded from the branch at Notify
      // We have the branch now. Does the type match?
      if (!branchInfo->GetClass() || !branchInfo->GetClass()->InheritsFrom(cl)) {
        delete info;
        if (newBranchInfo)
          delete branchInfo;
        return 0;
      }

      info->fSourceType = ObjInfo::kBranch;

      if (newBranchInfo)
        fBranchTable.Add(branchInfo);

      info->fBranchInfo = branchInfo;
      branchInfo->AddPtr(reinterpret_cast<TObject*&>(info->fAddr));
      if (branchInfo->IsLoaded())
        info->fAddr = branchInfo->GetLoader()->GetAddress();
      else
        LoadBranch(branchInfo);

      obj = reinterpret_cast<TObject*>(info->fAddr);
    }
  }

  if (obj) {
    info->fObjType = obj->IsA();
    fObjInfoStore.Add(info);
  }
  else {
    // should not happen
    delete info;
  }

  return obj;
}

BaseCollection*
Selector::GetCollectionImpl(TClass const* elemCl, TClass const* colCl, FillObjArray fillObjArray, char const* name, Bool_t warn)
{
  // Find object of the given name from the list of event objects,
  // public objects, and branches in this order.
  // If the located object is a collection of C whose base class is O,
  // fill ObjInfo::fCollection and return

  auto* info = static_cast<ObjInfo*>(fObjInfoStore.FindObject(name));
  std::pair<BaseCollection*, bool>* cache = 0;
  TObject* obj = 0;

  if (info) {
    for (auto& col : info->fCollections) {
      if (col.first->IsA()->InheritsFrom(colCl)) {
        if (col.second)
          return col.first;

        cache = &col;
      }
    }

    if (info->IsEvtObject()) {
      obj = FindObjThisEvt(name);

      if (!obj) {
        if (warn)
          Warning("GetObject", "Object %s was not found in the current event.", name);

        // null event object does not imply broken object info
        return 0;
      }
    }
    else if (info->IsPublic() || info->IsBranch()) {
      if (info->IsBranch())
        LoadBranch(info->fBranchInfo);

      obj = reinterpret_cast<TObject*>(info->fAddr);
    }

    if (!obj) {
      // public or branch but null, or no source type set -> somehow broken. Delete and create a new one
      fObjInfoStore.Remove(info);
      delete info;
    }
  }

  // lambda for casting obj to ReturnType
  auto castAndCache = [this, elemCl, colCl, &fillObjArray, &cache, warn](TObject* obj, ObjInfo* info) -> BaseCollection* {
    // First try direct cast    
    if (obj->IsA()->InheritsFrom(colCl))
      return static_cast<BaseCollection*>(obj);

    // If not a collection, warn and return 0
    if (!obj->IsA()->InheritsFrom(BaseCollection::Class())) {
      if (warn)
        this->Warning("GetObject", "Object %s is not a collection", info->GetName());

      return 0;
    }

    auto* col = static_cast<BaseCollection*>(obj);

    // Then check element type
    if (!col->ElementClass() || !col->ElementClass()->InheritsFrom(elemCl)) {
      if (warn)
        this->Warning("GetObject", "Could not cast elements of collection %s to array of %s", info->GetName(), elemCl->GetName());

      return 0;
    }

    if (!cache) {
      info->fCollections.push_back(std::pair<BaseCollection*, bool>(0, false));
      cache = &info->fCollections.back();
    }

    // cache obj array created in the function if cache->first is null
    fillObjArray(col, cache->first);
    cache->second = true;

    return cache->first;
  };

  if (obj) {
    // information of the object exists, but the product is not cached yet
    return castAndCache(obj, info);
  }

  info = new ObjInfo(name);

  BaseCollection* col = 0;

  obj = FindObjThisEvt(name);
  if (obj) {
    info->fSourceType = ObjInfo::kEvtObject;
    col = castAndCache(obj, info);
  }

  if (!col) {
    obj = FindPublicObj(name);
    if (obj) {
      info->fSourceType = ObjInfo::kPublic;
      info->fAddr = reinterpret_cast<BranchAddr_t>(obj);
      col = castAndCache(obj, info);
    }
  }

  if (!col) {
    auto* branchInfo = static_cast<TAMBranchInfo*>(fBranchTable.FindObject(name));
    bool newBranchInfo = false;
    if (!branchInfo) {
      branchInfo = new TAMBranchInfo(name);
      newBranchInfo = true;
    }

    if (!branchInfo->GetLoader() && (!FindLoader(branchInfo) || !branchInfo->Notify(fTree))) {
      // loader was not set yet, and failed to load / initialize
      // warning already issued by FindLoader
      delete info;
      if (newBranchInfo)
        delete branchInfo;
      return 0;
    }

    // TAMBranchInfo::fClass loaded from the branch at Notify
    // We have the branch now. Does the type match?
    if (!branchInfo->GetClass() || !branchInfo->GetClass()->InheritsFrom(BaseCollection::Class())) {
      delete info;
      if (newBranchInfo)
        delete branchInfo;
      return 0;
    }

    info->fSourceType = ObjInfo::kBranch;

    if (newBranchInfo)
      fBranchTable.Add(branchInfo);

    info->fBranchInfo = branchInfo;
    branchInfo->AddPtr(reinterpret_cast<TObject*&>(info->fAddr));
    if (branchInfo->IsLoaded())
      info->fAddr = branchInfo->GetLoader()->GetAddress();
    else
      LoadBranch(branchInfo);

    obj = reinterpret_cast<TObject*>(info->fAddr);
    if (obj)
      col = castAndCache(obj, info);
  }

  if (obj) {
    info->fObjType = obj->IsA();
    fObjInfoStore.Add(info);
  }
  else {
    // sufficient number of warning rows must have been printed by now
    delete info;
  }

  return col;
}

void
Selector::AddOutputMod(OutputMod* m)
{
  fOutputMods.Add(m);
}
