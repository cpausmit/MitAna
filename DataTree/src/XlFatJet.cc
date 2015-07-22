// $Id: XlFatJet.cc,v 1.1 2008/06/04 09:08:36 loizides Exp $

#include "MitAna/DataTree/interface/XlFatJet.h"

ClassImp(mithep::XlFatJet)

namespace mithep {

XlFatJet::~XlFatJet() {
}

void XlFatJet::ResetSubJetCol() {
  for(unsigned int i=0; i<XlSubJet::nSubJetTypes; ++i)
    fSubJets[i].Reset();
}


}
