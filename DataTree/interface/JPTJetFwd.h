#ifndef MITANA_DATATREE_JPTJETFWD_H
#define MITANA_DATATREE_JPTJETFWD_H
 
#include "MitAna/DataCont/interface/CollectionFwd.h"
#include "MitAna/DataCont/interface/ArrayFwd.h"
#include "MitAna/DataCont/interface/ObjArrayFwd.h"

namespace mithep {
  class JPTJet;
  typedef Collection<JPTJet>             JPTJetCol;
  typedef Array<JPTJet>                  JPTJetArr;
  typedef ObjArray<JPTJet>               JPTJetOArr;
}
#endif
