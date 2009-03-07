//--------------------------------------------------------------------------------------------------
// $Id: CacheFlag.h,v 1.2 2009/02/18 15:38:54 loizides Exp $
//
// CacheFlag
//
// Container for a transient Bool_t which is set by the reading streamer to kFALSE.
// This meant to be used as a cache valid flag for transient cached quantities.
//
// Authors: J.Bendavid
//--------------------------------------------------------------------------------------------------

#ifndef MITANA_DATATREE_CACHEFLAG_H
#define MITANA_DATATREE_CACHEFLAG_H

#include <TObject.h>
#include "MitAna/DataTree/interface/Types.h"

namespace mithep 
{
  class CacheFlag
  {
    public:
      CacheFlag() : fIsValid(kFALSE) {}

      void               ClearCache()               { fIsValid = kFALSE; }
      Bool_t             IsValid()            const { return fIsValid; }
      void               SetValid(Bool_t v = kTRUE) { fIsValid = v; }
      
    protected:
      Bool_t             fIsValid; //!flag whether cache is valid

    ClassDef(CacheFlag, 1) // Implementation of a transient Bool_t container
  };
}
#endif
