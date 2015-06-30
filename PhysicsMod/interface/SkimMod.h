//--------------------------------------------------------------------------------------------------
// SkimMod
//
// This module simply publishes an Array/ObjArray and copies all marked data objects from the mother
// branch into this published Array/ObjArray. It assumes that all marking is completed and the
// references will thus all resolve properly.
//
// Authors: L.DiMatteo, C.Paus
//--------------------------------------------------------------------------------------------------
#ifndef MITANA_PHYSICSMOD_SKIMMOD_H
#define MITANA_PHYSICSMOD_SKIMMOD_H

#include "MitAna/DataCont/interface/Array.h"
#include "MitAna/DataCont/interface/ObjArray.h"
#include "MitAna/TreeMod/interface/BaseMod.h" 

namespace mithep 
{
  template<class T>
  class SkimMod : public BaseMod 
  {
    public:
      SkimMod(const char *name  = "SkimMod", 
	      const char *title = "Skim module");
      ~SkimMod();

      const char              *GetBranchName()              const { return fBranchName; }
      void                     SetBranchName(const char *n)       { fBranchName = n;    }
      void                     SetColFromBranch(bool)             {} // obsolete
      void                     SetColMarkFilter(bool b)           { fColMarkFilter = b; }
      void                     SetPublishArray(bool b)            { fPublishArray = b;  }

    protected:
      TString                  fBranchName;    //name of collection
      bool                     fColMarkFilter; //input collection filter based on mark?
      bool                     fPublishArray;  //output collection is Array?
  
      ObjArray<T>             *fColSkm;        //!pointer to collection (skm - published object)
      Array<T>                *fArrSkm;        //!pointer to array (skm - published object)

      void                     Process();
      void                     SlaveBegin();
      void                     SlaveTerminate();

      ClassDef(SkimMod, 2) // Skim module
  };
}

//--------------------------------------------------------------------------------------------------
template<class T>
mithep::SkimMod<T>::SkimMod(const char *name, const char *title) : 
  BaseMod(name,title),
  fBranchName("SkmSetMe"),
  fColMarkFilter(kTRUE),
  fPublishArray(kFALSE),
  fColSkm(0),
  fArrSkm(0)
{
  // Constructor.
}

template<class T>
mithep::SkimMod<T>::~SkimMod() 
{
  // Destructor.
  if (fColSkm)
    delete fColSkm;

  if (fArrSkm)
    delete fArrSkm;
}

//--------------------------------------------------------------------------------------------------
template<class T>
void mithep::SkimMod<T>::Process()
{
  // make sure the collection is empty before starting
  if (!fPublishArray)
    fColSkm->Reset();
  else
    fArrSkm->Reset();

  // load the branch with the proper method
  auto* collection = GetObject<mithep::Collection<T>>(GetBranchName());

  // loop on the input collection and apply the filter on mark if required
  const UInt_t entries = collection->GetEntries();
  for (UInt_t i=0; i<entries; ++i) {

    // if marked or all objects requested
    if (!fColMarkFilter || collection->At(i)->IsMarked()) {

      // Make sure the mark is removed to avoid having the future objects already marked
      // - collections that are not skimmed will have dangeling marks but that us fine
      //   as marking is exclusively used by the skimmer
      if (fColMarkFilter)
	collection->At(i)->UnmarkMe();

      // fill the output Array/ObjArray
      if (!fPublishArray)
        fColSkm->Add(collection->At(i));
      else {        
        TObject *obj = fArrSkm->Allocate();
        new (obj) T(*collection->At(i));
      }
    }
  }
}

//--------------------------------------------------------------------------------------------------
template<class T>
void mithep::SkimMod<T>::SlaveBegin()
{
  // Request the branch to be published
  if (!fPublishArray) {
    fColSkm = new mithep::ObjArray<T>(0,TString("Skm")+GetBranchName());
    PublishObj(fColSkm);
  }
  else {
    fArrSkm = new mithep::Array<T>(0,TString("Skm")+GetBranchName());    
    PublishObj(fArrSkm);
  }
}

//--------------------------------------------------------------------------------------------------
template<class T>
void mithep::SkimMod<T>::SlaveTerminate()
{
}
#endif
