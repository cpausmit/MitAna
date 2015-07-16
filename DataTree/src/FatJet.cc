// $Id: FatJet.cc,v 1.1 2008/06/04 09:08:36 loizides Exp $

#include "MitAna/DataTree/interface/FatJet.h"

ClassImp(mithep::FatJet)

using namespace std;

namespace mithep {

FatJet::~FatJet() {
  for (TrackData *t : fTracks)
    delete t;
  for (SVData *s : fSVs)
    delete s;
}

const RefArray<XlSubJet>* FatJet::GetSubJets(XlSubJet::ESubJetType t) const {
  try {
    return fSubJets.at(t);
  } catch(out_of_range& const e) {
    return NULL;
  }
}

Bool_t FatJet::HasSubJet(XlSubJet*p, XlSubJet::ESubJetType t) const {
   try {
     return fSubJets.at(t)->HasObject(p);
   } catch(out_of_range& const e) {
     fprintf(stderr,"Warning, searching for subjet of nonexistent type\n");
     return kFALSE;
   }
}

void FatJet::AddSubJet(const XlSubJet * sj; XlSubJet::ESubJetType t) {
  try {
    fSubJets.at(t)->Add(sj);
  } catch(out_of_range& const e) {
    fSubJets[t] = new RefArray<XlSubJet*>;
    fSubJets.at(t)->Add(sj);
  }
}


void FatJet::AddSubJet(const XlSubJet * sj) {
  XlSubJet::ESubJetType t = sj->SubJetType();
  AddSubJet(sj,t);
}


}
