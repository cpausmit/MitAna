//--------------------------------------------------------------------------------------------------
// HLTFwkMod
//
// This TAM module reads the trigger and trigger objects information whenever it changes
// (ie on run boundaries). It then publishes this in a convenient way so that the real
// user HLTMod modules can make use of it. Note: This module gets automatically integrated
// by the Analysis steering class (unless you have called Analysis::SetUseHLT(kFALSE).
//
// Authors: C.Loizides
//--------------------------------------------------------------------------------------------------

#ifndef MITANA_TREEMOD_HLTFWKMOD_H
#define MITANA_TREEMOD_HLTFWKMOD_H

#include <string>
#include <TString.h>
#include "MitAna/TreeMod/interface/BaseMod.h" 
#include "MitAna/DataTree/interface/TriggerObjectFwd.h"
#include "MitAna/DataTree/interface/TriggerTable.h"
#include "MitAna/DataTree/interface/TriggerObjectsTable.h"
#include "MitAna/DataTree/interface/Names.h"

namespace mithep 
{
  class HLTFwkMod : public BaseMod {
  public:
    HLTFwkMod(const char *name="HLTFwkMod", const char *title="HLT framework module");
    ~HLTFwkMod();

    const char* L1ATabNamePub() const    { return fL1Algos.GetName(); }
    const char* L1TTabNamePub() const    { return fL1Techs.GetName(); }
    const char* HLTLabName() const       { return fHLTLabName;    }
    const char* HLTLabNamePub() const    { return fLabels.GetName(); }
    const char* HLTObjsName() const      { return fObjsName;      }
    const char* HLTObjArrNamePub() const { return fTrigObjArr.GetName(); }
    const char* HLTObjsNamePub() const   { return fTrigObjs.GetName(); }
    const char* HLTTabName() const       { return fHLTTabName;    }
    const char* HLTTabNamePub() const    { return fTriggers.GetName(); }
    const char* HLTTreeName() const      { return fHLTTreeName;   }

    void SetL1ATabName(const char *n)       { fL1Algos.SetName(n); }
    void SetL1TTabName(const char *n)       { fL1Techs.SetName(n); }
    void SetHLTLabName(const char *n)       { fHLTLabName    = n; }
    void SetHLTLabNamePub(const char *n)    { fLabels.SetName(n); }
    void SetHLTObjsName(const char *n)      { fObjsName      = n; }
    void SetHLTObjArrNamePub(const char *n) { fTrigObjArr.SetName(n); }
    void SetHLTObjsNamePub(const char *n)   { fTrigObjs.SetName(n); }
    void SetHLTTabName(const char *n)       { fHLTTabName    = n; }
    void SetHLTTabNamePub(const char *n)    { fTriggers.SetName(n); }
    void SetHLTTreeName(const char *n)      { fHLTTreeName   = n; }

    Bool_t GetAbortIfNoHLTTree() const   { return fAbortIfNoHLTTree; }
    void   SetAbortIfNoHLTTree(Bool_t a) { fAbortIfNoHLTTree = a; }

    Bool_t HasData() const { return fHLTTree != 0; }

  protected:
    Bool_t LoadTriggerTable();
    void   BeginRun() override;
    Bool_t Notify() override;
    void   Process() override;
    void   SlaveBegin() override;
    void   SlaveTerminate() override;

    TString fHLTTreeName{Names::gkHltTreeName}; //HLT tree name
    TString fHLTTabName{Names::gkHltTableBrn};  //HLT trigger names branch name
    TString fHLTLabName{Names::gkHltLabelBrn};  //HLT module labels branch name
    TString fObjsName{Names::gkHltObjBrn};      //trigger objects branch name
    TString fRelsName;                          //trigger to objects relation branch name

    // inputs
    Bool_t                    fReload{kFALSE};        //!=true then reload/rebuild tables
    TTree*                    fHLTTree = 0;       //!HLT tree in current file
    std::vector<std::string>* fHLTTab = 0;        //!HLT trigger names
    std::vector<std::string>* fHLTLab = 0;        //!HLT module labels
    Int_t                     fCurEnt{-2};        //!current entry in HLT tree (-2 for unset)

    // outputs
    const UInt_t        fNMaxTriggers{1024};  //maximum number of triggers
    TriggerTable        fTriggers{Int_t(fNMaxTriggers)};  //!exported published HLT trigger table
    TriggerTable        fLabels{Int_t(fNMaxTriggers) * 16}; //!exported published HLT module label table
    TriggerObjectArr    fTrigObjArr{};    //!buffer of published HLT trigger objects
    TriggerObjectsTable fTrigObjs;      //!exported published HLT trigger objects table
    TriggerTable        fL1Algos{Int_t(fNMaxTriggers)}; //!exported published L1 algorithm triggers table
    TriggerTable        fL1Techs{Int_t(fNMaxTriggers)}; //!exported published L1 technical triggers table

    Bool_t fAbortIfNoHLTTree = kTRUE;

    friend class OutputMod;

    ClassDef(HLTFwkMod, 1) // HLT framework TAM module
  };
}
#endif
