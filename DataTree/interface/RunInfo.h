//--------------------------------------------------------------------------------------------------
// $Id: RunInfo.h,v 1.10 2009/02/18 15:38:54 loizides Exp $
//
// RunInfo
//
// This class holds run specific information. It is stored in a dedicated tree.
//
// Authors: C.Loizides
//--------------------------------------------------------------------------------------------------

#ifndef MITANA_DATATREE_RUNINFO_H
#define MITANA_DATATREE_RUNINFO_H
 
#include "MitAna/DataTree/interface/DataBase.h"

namespace mithep 
{
  class RunInfo : public DataBase
  {
    public:
      RunInfo() : fRunNum(0), fHltEntry(0), fEvtSelBitsEntry(-1) {}
      RunInfo(UInt_t run) : fRunNum(run), fHltEntry(0), fEvtSelBitsEntry(-1) {}

      Int_t               HltEntry()     const { return fHltEntry; }
      EObjType            ObjType()      const { return kRunInfo;  }      
      UInt_t              RunNum()       const { return fRunNum;   }
      void                SetHltEntry(Int_t i) { fHltEntry=i;      }
      void                SetRunNum(UInt_t i)  { fRunNum=i;        }
      void                SetEvtSelBitsEntry(Int_t i) { fEvtSelBitsEntry = i; }

    protected:
      UInt_t              fRunNum;          //run number
      Int_t               fHltEntry;        //entry for HLT block
      Int_t               fEvtSelBitsEntry; //entry for EvtSelData bit names

    ClassDef(RunInfo, 3) // Run info class
  };
}
#endif
