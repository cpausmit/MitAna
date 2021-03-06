// $Id: TriggerObject.cc,v 1.4 2008/12/08 16:42:53 loizides Exp $

#include "MitAna/DataTree/interface/TriggerObject.h"

ClassImp(mithep::TriggerObject)

using namespace mithep;

//--------------------------------------------------------------------------------------------------
void TriggerObject::Print(Option_t *opt) const
{
  printf("%03d: %s %2d %.2f %.2f %.2f (%s/%s)\n", 
         TrgId(), TrigName(), Int_t(Type()), Pt(), Eta(), Phi(), ModuleName(), FilterName());
}
