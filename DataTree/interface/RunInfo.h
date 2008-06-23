//--------------------------------------------------------------------------------------------------
// $Id: RunInfo.h,v 1.2 2008/06/18 19:08:14 loizides Exp $
//
// RunInfo
//
//
// Authors: C.Loizides
//
//--------------------------------------------------------------------------------------------------

#ifndef DATATREE_RUNINFO_H
#define DATATREE_RUNINFO_H
 
#include "MitAna/DataTree/interface/Types.h"

namespace mithep 
{
  class RunInfo
  {
    public:
      RunInfo() : fRunNum(0) {}
      RunInfo(UInt_t run) : fRunNum(run) {}
      virtual ~RunInfo() {}

      UInt_t              RunNum()       const { return fRunNum; }
      void                SetRunNum(UInt_t i)  { fRunNum=i; }

    protected:
      UInt_t              fRunNum;          // run number
      
      ClassDef(RunInfo, 1) // Run info class
  };
}
#endif