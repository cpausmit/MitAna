//--------------------------------------------------------------------------------------------------
// $Id: Vertex.h,v 1.9 2009/03/18 15:44:32 loizides Exp $
//
// Vertex
//
// Vertex class derived from BaseVertex holding additional fit information.
//
// Authors: J.Bendavid
//--------------------------------------------------------------------------------------------------

#ifndef MITANA_DATATREE_VERTEX_H
#define MITANA_DATATREE_VERTEX_H
 
#include <TMath.h>
#include "MitAna/DataTree/interface/BaseVertex.h"

namespace mithep 
{
  class Vertex : public BaseVertex
  {
    public:
      Vertex() : fChi2(0), fNdof(0), fNTracks(0) {}
      Vertex(Double_t x, Double_t y, Double_t z) : 
        BaseVertex(x,y,z), fChi2(0), fNdof(0), fNTracks(0) {}
      Vertex(Double_t x, Double_t y, Double_t z, Double_t xErr, Double_t yErr, Double_t zErr) :
        BaseVertex(x,y,z,xErr,yErr,zErr), fChi2(0), fNdof(0), fNTracks(0) {}
      Vertex(const ThreeVector &pos) : 
        BaseVertex(pos), fChi2(0), fNdof(0), fNTracks(0) {}
      
      Double_t            Chi2()      const { return fChi2;                    } 
      UShort_t            Ndof()      const { return fNdof;                    }
      UInt_t              NTracks()   const { return fNTracks;                 }
      EObjType            ObjType()   const { return kVertex;                  }      
      Double_t            Prob()      const { return TMath::Prob(fChi2,fNdof); }
      void                SetChi2(Double_t chi2)     { fChi2 = chi2;     }
      void                SetNdof(UShort_t nDof)     { fNdof = nDof;     } 
      void                SetNTracks(UShort_t ntrks) { fNTracks = ntrks; }
            
    protected:
      Double32_t          fChi2;     //[0,0,12]chi squared of conversion vertex fit
      UShort_t            fNdof;     //number of degrees of freedom of conversion vertex fit
      UShort_t            fNTracks;  //number of tracks used for the fit
	
    ClassDef(Vertex, 1) // Vertex class
  };
}
#endif
