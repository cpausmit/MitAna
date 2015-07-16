// $Id: XlFatJet.cc,v 1.1 2008/06/04 09:08:36 loizides Exp $

#include "MitAna/DataTree/interface/XlFatJet.h"

ClassImp(mithep::XlFatJet)

namespace mithep {

XlFatJet::~XlFatJet() {
  delete[] track_length;
  delete[] track_dist;
  delete[] track_dxy;
  delete[] track_dz;
  delete[] track_IP2D;
  delete[] track_IP2Dsig;
  delete[] track_IP;
  delete[] track_IPsig;
  delete[] track_IP2Derr;
  delete[] track_IPerr;
  delete[] track_Prob;
  delete[] track_pt;
  delete[] track_eta;
  delete[] track_chi2;
  delete[] track_charge;
  delete[] track_PVWeight;
  delete[] track_nHitAll;
  delete[] track_nHitPixel;
  delete[] track_nHitStrip;
  delete[] track_nHitTIB;
  delete[] track_nHitTID;
  delete[] track_nHitTOB;
  delete[] track_nHitTEC;
  delete[] track_nHitPXB;
  delete[] track_nHitPXF;
  delete[] track_isHitL1;
  delete[] track_PV;
  delete[] track_fromSV;
  delete[] track_SV;
}

XlFatJet::~XlFatJet() {
  for (TrackData *t : fTracks)
    delete t;
  for (SVData *s : fSVs)
    delete s;
}

const XlSubJet* XlFatJet::SubJet(UInt_t i, XlSubJet::ESubJetType t) const {
    UInt_t NJets = fSubJets.Entries();
    UInt_t counter = 0;
    for (UInt_t j=0;j<NJets;j++) {
        if (fSubJets.At(j)->SubJetType() == t) {
            counter++;
            if (counter == i) {
                return fSubJets.At(j);
            }
        }
    }
    return NULL;
}

UInt_t XlFatJet::NSubJets(XlSubJet::ESubJetType t) const {
    UInt_t NJets = fSubJets.Entries();
    UInt_t counter = 0;
    for (UInt_t j=0;j<NJets;j++) {
        if (fSubJets.At(j)->SubJetType() == t) {
            counter++;
        }
    }
    return counter;
}

UInt_t XlFatJet::NTopSubJets() const {  return NSubJets(XlSubJet::ESubJetType::eTop); }

UInt_t XlFatJet::NVSubJets() const {  return NSubJets(XlSubJet::ESubJetType::eV); }

}
