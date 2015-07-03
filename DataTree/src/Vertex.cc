// $Id: Vertex.cc,v 1.1 2008/06/30 11:30:27 loizides Exp $

#include "MitAna/DataTree/interface/Vertex.h"

FourVectorM mithep::Vertex::Mom4() {
  FourVectorM sum(0,0,0,0);
  UInt_t nTracks = NTracks();
  for(UInt_t i=0; i<nTracks; i++) {
    sum +=  Trk(i)->Mom4(0.13957018);
  }
  return sum;
}

FourVectorM mithep::Vertex::WeightedMom4() {
  FourVectorM sum(0,0,0,0);
  UInt_t nTracks = NTracks();
  for(UInt_t i=0; i<nTracks; i++) {
    sum +=  Trk(i)->Mom4(0.13957018)*TrackWeight(i);
  }
  return sum;
}

ClassImp(mithep::Vertex)
