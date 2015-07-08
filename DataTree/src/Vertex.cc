// $Id: Vertex.cc,v 1.1 2008/06/30 11:30:27 loizides Exp $

#include "MitAna/DataTree/interface/Vertex.h"
ClassImp(mithep::Vertex)

namespace mithep {
  FourVectorM Vertex::Mom4() const  {
    FourVectorM sum(0,0,0,0);
    UInt_t nTracks = NTracks();
    for(UInt_t i=0; i<nTracks; i++) {
      sum +=  Trk(i)->Mom4(0.13957018);
    }
    return sum;
  }

  FourVectorM Vertex::WeightedMom4() const {
    FourVectorM sum(0,0,0,0);
    UInt_t nTracks = NTracks();
    for(UInt_t i=0; i<nTracks; i++) {
      sum +=  Trk(i)->Mom4(0.13957018)*TrackWeight(i);
    }
    return sum;
  }
}
