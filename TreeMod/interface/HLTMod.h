//--------------------------------------------------------------------------------------------------
// HLTMod
//
// This module allows to select events according to given HLT trigger bits. The trigger bits are
// selected via their corresponding HLT trigger name.  The trigger name is added to the list of
// accepted triggers using AddTrigger(). Each member of this list will be logically "ored" to the
// search mask (see description of AddTrigger()).  For every accepted event the list of trigger
// objects will be published. The name of the published objects can be specified via
// SetTrigObjsName.
//
// The objects can be retrieved in sub-modules with BaseModule::GetHLTObjects().  HLTMod will abort
// processing the chain of sub-modules if the trigger bits did not match the given trigger mask,
// unless you call SetAbortIfNotAccepted(kFALSE).  For convenience HLTMod defines two virtual
// functions, OnAccepted() and OnFailed() that can be implemented in derived classes. OnAccepted()
// will be called if the event passes the trigger mask, on Failed() if it did not.
//
// An example of how this is supposed to work can be found in 
// $CMSSW_BASE/src/MitAna/macros/examples/runHLTExample.C
//
// Authors: C.Loizides, C.Paus
//--------------------------------------------------------------------------------------------------

#ifndef MITANA_TREEMOD_HLTMOD_H
#define MITANA_TREEMOD_HLTMOD_H

#include "MitAna/TreeMod/interface/BaseMod.h" 
#include "MitAna/DataTree/interface/TriggerObjectFwd.h"
#include "MitAna/DataTree/interface/Names.h"

#include "TString.h"

#include <utility>

namespace mithep 
{
  class TriggerTable;
  class TriggerObjectsTable;
  class TriggerMask;

  class HLTMod : public BaseMod {
  public:
    enum EObjMode { // which objects to get
      kAll =  0,
      kL1,
      kHlt
    };

    HLTMod(const char *name="HLTMod", const char *title="High-level trigger module");
    ~HLTMod();

    const char* GetBitsName() const     { return fBitsName;      }
    const char* GetOutputName() const   { return fMyObjsNamePub; }
    const char* GetTrigObjsName() const { return fMyObjsNamePub; }
    Int_t       GetNEvents() const      { return fNEvents;       }
    Int_t       GetNAccepted() const    { return fNAccepted;     }
    Int_t       GetNFailed() const      { return fNFailed;       }

    void AddTrigger(const char *expr, UInt_t firstRun = 0, UInt_t lastRun = 0);
    void SetAbortIfNotAccepted(Bool_t b) { fAbortIfNotAccepted = b; }
    void SetAbortIfNoData(Bool_t b)      { fAbortIfNoData = b; }
    void SetBitsName(const char *n)      { fBitsName = n; }
    void SetIgnoreBits(Bool_t b)         { fIgnoreBits = b; }
    void SetInputName(const char *n)     { fMyObjsNamePub = n; }
    void SetObjMode(EObjMode m )         { fObjMode = m; }
    void SetPrintTable(Bool_t b)         { fPrintTable = b; }
    void SetTrigObjsName(const char *n)  { fMyObjsNamePub = n; }

  protected:
    void         AddTrigObjs(TriggerObjectOArr&, BitMask1024& bitsDone, UInt_t tid);
    virtual void OnAccepted()  { /* could be implemented in derived classes */ }
    virtual void OnFailed()    { /* could be implemented in derived classes */ }
    void         BeginRun() override;
    void         Process() override;
    void         SlaveBegin() override;
    void         SlaveTerminate() override;

    typedef std::pair<UInt_t, UInt_t> RunRange;
    typedef std::pair<TString, RunRange> TriggerNameWithValidity;
    typedef std::vector<TriggerNameWithValidity> TriggerNames;

    Bool_t       fAbortIfNotAccepted{kTRUE};
    Bool_t       fAbortIfNoData{kTRUE}; //set to false for e.g. private MC with no HLT info
    Bool_t       fPrintTable{kFALSE};    //=true then print HLT trigger table in BeginRun
    Bool_t       fIgnoreBits{kFALSE};    //=true then try to get trigger objects (def=0)
    EObjMode     fObjMode{kHlt};       //defines which objects to get (def=kHlt)
    TString      fBitsName{Names::gkHltBitBrn}; //trigger bits branch name
    TString      fMyObjsNamePub; //name of exported trigger object array
    TriggerNames fTrigNames{};     //trigger names requested for test mask with valid run range

    Int_t fNEvents{0};       //!number of processed events
    Int_t fNAccepted{0};     //!number of accepted events
    Int_t fNFailed{0};       //!number of failed events

    Bool_t fSkipModule{kFALSE}; //set to true when HLTInfo is not found in SlaveBegin

    std::vector<BitMask1024>   fTrigBitsAnd{};   //!trigger bits used in mask
    std::vector<BitMask1024>   fTrigBitsCmp{};   //!trigger bits used for comparison
    const TriggerTable*        fTriggers = 0;      //!imported published HLT trigger table
    const TriggerObjectsTable* fTrigObjs = 0;      //!imported published HLT trigger objects table

    ClassDef(HLTMod, 1) // HLT TAM module
  };
}
#endif
