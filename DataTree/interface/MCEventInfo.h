//--------------------------------------------------------------------------------------------------
// $Id: EventHeader.h,v 1.6 2008/07/08 14:41:01 loizides Exp $
//
// MCEventInfo
//
// Authors: C.Loizides
//--------------------------------------------------------------------------------------------------

#ifndef DATATREE_MCEVENTINFO_H
#define DATATREE_MCEVENTINFO_H
 
#include "MitAna/DataTree/interface/Types.h"

namespace mithep 
{
  class MCEventInfo
  {
    public:
      MCEventInfo(Double_t w=0) : fWeight(w)  {}
      virtual ~MCEventInfo() {}

      Double_t            Weight()       const { return fWeight; }
      void                SetWeight(Double_t w)  { fWeight=w; }

    protected:
      Double32_t          fWeight;          //event weight
      
    ClassDef(MCEventInfo, 1) // Monte-Carlo event info class
  };
}
#endif
