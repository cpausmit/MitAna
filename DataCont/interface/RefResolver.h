//--------------------------------------------------------------------------------------------------
// RefResolver
//
// Helper class to resolve reference via TAM or via ROOT directly.
//
// Authors: C.Loizides
//--------------------------------------------------------------------------------------------------
#ifndef MITANA_DATACONT_REFRESOLVER_H
#define MITANA_DATACONT_REFRESOLVER_H

#include <TObject.h>
#include <TProcessID.h>

namespace mithep 
{
  class RefResolver
  {
    public:
      RefResolver() {}
      virtual ~RefResolver() {}

      static TObject *GetObjectWithID(UInt_t uid, TProcessID *pid);
  };
}
#endif
