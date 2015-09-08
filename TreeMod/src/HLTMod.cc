#include "MitAna/TreeMod/interface/HLTMod.h"
#include "MitAna/TreeMod/interface/HLTFwkMod.h"
#include "MitAna/DataTree/interface/TriggerName.h"
#include "MitAna/DataTree/interface/TriggerMask.h"
#include "MitAna/DataTree/interface/TriggerObject.h"
#include "MitAna/DataTree/interface/TriggerObjectCol.h"
#include "MitAna/DataTree/interface/TriggerObjectsTable.h"
#include "MitAna/DataTree/interface/EventHeader.h"

using namespace mithep;

ClassImp(mithep::HLTMod)

//--------------------------------------------------------------------------------------------------
HLTMod::HLTMod(const char *name, const char *title) : 
  BaseMod(name,title),
  fMyObjsNamePub(Form("%sTrigObjs", name))
{
}

//--------------------------------------------------------------------------------------------------
HLTMod::~HLTMod() 
{
}

//--------------------------------------------------------------------------------------------------
void HLTMod::AddTrigger(const char *expr, UInt_t firstRun/* = 0*/, UInt_t lastRun/* = 0*/)
{
  // Add trigger search pattern to the list of patters. Each element of the list is logically
  // "ored". The given expression can contain several trigger names logically "anded" (using "&").
  // A "!" infront of a trigger name negates the bit. For example, valid expressions are: "A", "!A",
  // "A&B", "A&!B" or "A&B&C"
  //
  // To add both "A" and "!A" at the same time specify "!+A". This is a commnon pattern when you
  // need access to the Trigger objects of a particular trigger but do not want to reject the event.
  // If you just pass the event the trigger objects would not be stored as potentially no trigger
  // would fit, thus no trigger objects would eb written.

  TString tname(expr);
  RunRange runRange(firstRun, lastRun);

  // deal with the special case
  if (tname.Index("!+") == 0) {
    // add both, trigger and its negation
    TString subtname(tname(2, tname.Length() - 2));  // stripping off the special characters

    fTrigNames.emplace_back(subtname, runRange);
    fTrigNames.emplace_back("!" + subtname, runRange);
  }
  else
    fTrigNames.emplace_back(tname, runRange);
}

//--------------------------------------------------------------------------------------------------
void HLTMod::AddTrigObjs(TriggerObjectOArr& myTrgObjs, BitMask1024& bitsDone, UInt_t tid)
{
  // Add trigger objects corresponding to trigger id.
  const BitMask1024 &ba(fTrigBitsAnd.at(tid));
  const BitMask1024 &bm(fTrigBitsCmp.at(tid));
  for (UInt_t i=0; i<bm.Size(); ++i) {
    if (ba.TestBit(i)==0 && !fIgnoreBits)
      continue; // not an active trigger bit
    if (bitsDone.TestBit(i))
      continue; // objects for this bit are already obtained
    if (bm.TestBit(i)==0) 
      continue; // excluded trigger bit (ie a !trgname)

    const TList *list = fTrigObjs->GetList(i);
    if (list) {
      TIter iter(list->MakeIterator());
      const TriggerObject *to = dynamic_cast<const TriggerObject*>(iter.Next());
      while (to) {
        if (  (fObjMode == kAll)  ||
             ((fObjMode == kHlt) && (to->IsHLT())) ||
             ((fObjMode == kL1)  && (to->IsL1())) )
          myTrgObjs.Add(to);    
        to = dynamic_cast<const TriggerObject*>(iter.Next());
      }
    }
    bitsDone.SetBit(i);
  }
}
 
//--------------------------------------------------------------------------------------------------
void HLTMod::BeginRun()
{
  // Get HLT tree and set branches. Compute bitmasks to be used when comparing to the HLT bits.

  // HLTFwkMod must have the tree data by now
  if (!GetHltFwkMod()->HasData()) {
    if (fAbortIfNoData) {
      SendError(kAbortAnalysis, "BeginRun", "HLT info not available.");
      return;
    }
    else {
      fSkipModule = true; // skip all subsequent functions of this module
      return;
    }
  }

  fTrigBitsAnd.clear();
  fTrigBitsCmp.clear();

  if (fPrintTable) {
    Info("BeginRun", "Get trigger table for run %d", GetEventHeader()->RunNum());
    printf(" Trigger table for run %d:\n",GetEventHeader()->RunNum());
    fTriggers->Print();
  }
  
  UInt_t runNumber = GetEventHeader()->RunNum();

  for (auto& tnWithV : fTrigNames) {
    TString& names(tnWithV.first);
    RunRange& runRange(tnWithV.second);
    
    UInt_t firstRun = runRange.first;
    UInt_t lastRun  = runRange.second;
    
    // implement run dependence of the triggers
    if (!(firstRun==0 && lastRun==0) &&
        (runNumber < firstRun || runNumber > lastRun))
      continue;
    
    BitMask1024 tmask; // trigger mask
    BitMask1024 amask; // bitand  mask

    TObjArray *arr = names.Tokenize("&");
    if (arr) {
      for (Int_t j=0; j<arr->GetEntries(); j++){
        TObjString *s = dynamic_cast<TObjString*>(arr->At(j));
        if (!s) 
          continue;
        TString st = s->GetString();
        bool wildcard = false;
        if (st.EndsWith("*")) {
          st.ReplaceAll("*","");
          wildcard = true;
        }
        const char *sptr = st.Data();
        Bool_t invert = kFALSE;
        if (sptr[0] == '!') { 
          invert = kTRUE; //inverted bit set
          ++sptr;
        }
        const TriggerName *tn = 0;
        if (wildcard) {
          tn = fTriggers->GetWildcard(sptr);
        }
        else {
          tn = fTriggers->Get(sptr);
        }
        if (!tn) {
          Warning("BeginRun", "Trigger %s not found.", sptr);
          continue;
        }

        UShort_t bit = tn->Id();
        if (amask.TestBit(bit)) {
          if (tmask.TestBit(bit) == invert) {
            amask.ClearBit(bit);
            tmask.ClearBit(bit);
            Warning("BeginRun", "Trigger expression %s always false.", names.Data());
            break;
          }
        }
	else { //always set and-mask bit 
          amask.SetBit(bit); 
          if (!invert) 
            tmask.SetBit(bit); //set trigger bit
        }
      }
      delete arr;
    }
    if (amask.NBitsSet()) {
      fTrigBitsAnd.push_back(amask);
      fTrigBitsCmp.push_back(tmask);
    }
  }
}

//--------------------------------------------------------------------------------------------------
void HLTMod::Process()
{
  // Process trigger bits for this event. If trigger bits pass the given bit mask, then obtain
  // and publish the corresponding trigger objects. If OnAccepted or OnFailed is implemented
  // in a derived class, call it.

  if (fSkipModule)
    return;

  ++fNEvents; 

  auto* bits = GetObject<TriggerMask>(fBitsName);

  // match trigger bits to trigger mask and obtain trigger objects
  auto* myTrgObjs = new TriggerObjectOArr(0, fMyObjsNamePub);
  myTrgObjs->SetOwner(kFALSE); // the objects are owned by the object table

  BitMask1024 bitsDone{};

  Bool_t accept = kFALSE;
  for (UInt_t iB = 0; iB != fTrigBitsAnd.size(); ++iB) {
    BitMask1024 bitmask(bits->Get());
    bitmask &= fTrigBitsAnd.at(iB);
    if (bitmask == fTrigBitsCmp.at(iB)) {
      accept = kTRUE;
      AddTrigObjs(*myTrgObjs, bitsDone, iB);
    }
  }

  // take action if failed
  if (!accept) {
    ++fNFailed;
    OnFailed();
    delete myTrgObjs;
    if (fAbortIfNotAccepted)
      SkipEvent(); // abort processing of this event by sub-modules

    return;
  } 

  // take action if accepted
  ++fNAccepted;
  IncNEventsProcessed();
  OnAccepted();
  // here the trigger objects are attached to the event for further analysis
  // -- NOTE: all trigger objects not just objects corresponding to your trigger(s)
  if (!AddObjThisEvt(myTrgObjs)) {
    SendError(kAbortAnalysis, "Process", 
              "Could not add my trigger objects with name %s to event.",
              myTrgObjs->GetName());

    delete myTrgObjs;
    return;
  }
}

//--------------------------------------------------------------------------------------------------
void HLTMod::SlaveBegin()
{
  // Request trigger bit branch and obtain trigger table and objects.

  if (!HasHLTInfo()) {
    SendError(kAbortAnalysis, "SlaveBegin", "HLTFwkMod not available.");
    return;
  }

  fTriggers = GetHLTTable();
  if (!fTriggers) {
    SendError(kAbortAnalysis, "SlaveBegin", "Could not get HLT trigger table.");
    return;
  }
  fTrigObjs = GetHLTObjectsTable();
  if (!fTrigObjs) {
    SendError(kAbortAnalysis, "SlaveBegin", "Could not get HLT trigger objects table.");
    return;
  }
}

//--------------------------------------------------------------------------------------------------
void HLTMod::SlaveTerminate()
{
  // Save number of accepted events.

  printf(" %s - Accepted events: %d (/%d)\n",GetName(),fNAccepted,fNEvents);
  SaveNEventsProcessed("hDHLTEvents");
}
