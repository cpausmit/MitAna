//--------------------------------------------------------------------------------------------------
// $Id: CompositeParticle.h,v 1.5 2008/07/13 08:37:50 loizides Exp $
//
// Composite Particle
//
// A composite particle class that holds other (daughter) particles.
//
// Authors: J.Bendavid, C.Loizides
//--------------------------------------------------------------------------------------------------

#ifndef DATATREE_COMPOSITEPARTICLE_H
#define DATATREE_COMPOSITEPARTICLE_H
 
#include "MitAna/DataTree/interface/Types.h"
#include "MitAna/DataTree/interface/RefArray.h"
#include "MitAna/DataTree/interface/Particle.h"

namespace mithep 
{
  class CompositeParticle : public Particle
  {
    public:
      CompositeParticle(UInt_t rsv=0) : fDaughters(rsv) { SetClearBit(); }
      ~CompositeParticle() {}
    
      void			AddDaughter(Particle *p)    { fDaughters.Add(p); }
      void                      Clear(Option_t *opt="")     { fDaughters.Clear(opt); }
      Int_t		        Charge()              const;
      const Particle           *GetDaughter(UInt_t i) const { return fDaughters.At(i); }
      UInt_t                    GetNDaughters()       const { return fDaughters.GetEntries(); }
      Bool_t			HasCommonDaughter(const CompositeParticle *p) const;
      Bool_t			HasSameDaughters(const CompositeParticle *p)  const;
      Bool_t			IsDaughter(const Particle *p)                 const;
      FourVector	        Mom()                 const;
      void                      Trim()                      { fDaughters.Trim(); }

    protected:
      RefArray<Particle>        fDaughters; //references to daughter particles
      
    ClassDef(CompositeParticle, 1) // Composite particle class
  };
}
#endif
