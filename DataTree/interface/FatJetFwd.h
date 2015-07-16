// $Id:$

#ifndef MITANA_DATATREE_FATJETFWD_H
#define MITANA_DATATREE_FATJETFWD_H

#include "MitAna/DataCont/interface/CollectionFwd.h"
#include "MitAna/DataCont/interface/ArrayFwd.h"
#include "MitAna/DataCont/interface/ObjArrayFwd.h"

namespace mithep {
  class FatJet;
  typedef Collection<FatJet> FatJetCol;
  typedef Array<FatJet> FatJetArr;
  typedef ObjArray<FatJet> FatJetOArr;
}
#endif
