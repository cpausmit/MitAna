// $Id: FatJet.cc,v 1.1 2008/06/04 09:08:36 loizides Exp $

#include "MitAna/DataTree/interface/FatJet.h"

ClassImp(mithep::FatJet)

using namespace std;

namespace mithep {

FatJet::~FatJet() {
}

const RefArray<XlSubJet>* FatJet::GetSubJets(XlSubJet::ESubJetType t) const {
  return fSubJets[(unsigned int)t];
}

Bool_t FatJet::HasSubJet(const XlSubJet*p, XlSubJet::ESubJetType t) const {
   return fSubJets[(unsigned int)t]->HasObject(p);
}

void FatJet::AddSubJet(const XlSubJet * sj, XlSubJet::ESubJetType t) {
  fSubJets[(unsigned int)t]->Add(sj);
}


void FatJet::AddSubJet(const XlSubJet * sj) {
  XlSubJet::ESubJetType t = sj->SubJetType();
  AddSubJet(sj,t);
}


}
