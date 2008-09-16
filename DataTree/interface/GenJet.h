//--------------------------------------------------------------------------------------------------
// $Id: $
//
// GenJet
//
// Details to be worked out...
//
// Authors: S.Xie
//--------------------------------------------------------------------------------------------------

#ifndef MITANA_DATATREE_GENJET_H
#define MITANA_DATATREE_GENJET_H
 
#include "MitAna/DataTree/interface/Particle.h"

namespace mithep 
{
  class GenJet : public Particle
  {
    public:
      GenJet() {}
      GenJet(Double_t px, Double_t py, Double_t pz, Double_t e) : fMom(px,py,pz,e)
      {}
      ~GenJet() {}

      FourVector	Mom()                         const { return fMom;                       }
      Double_t          Charge()                      const { return 0;                          }
      Double_t          HadEnergy()                   const { return fHadEnergy;                 } 
      Double_t          EmEnergy()                    const { return fEmEnergy;                  } 
      Double_t          InvisibleEnergy()             const { return fInvisibleEnergy;           }
      Double_t          AuxiliaryEnergy()             const { return fAuxiliaryEnergy;           }
      Int_t             MatchedMCFlavor()             const { return fMatchedMCFlavor;           }

      void              SetHadEnergy(Double_t val)          { fHadEnergy        = val;           } 
      void              SetEmEnergy(Double_t val)           { fEmEnergy         = val;           }
      void              SetInvisibleEnergy(Double_t val)    { fInvisibleEnergy  = val;           }
      void              SetAuxiliaryEnergy(Double_t val)    { fAuxiliaryEnergy  = val;           }
      void              SetMatchedMCFlavor(Int_t flavor)    { fMatchedMCFlavor  = flavor;        }

    protected:
      FourVector fMom;                    //four momentum of jet
      Double_t   fHadEnergy; 	          //hadronic energy
      Double_t   fEmEnergy; 	          //electromagnetic
      Double_t   fInvisibleEnergy; 	  //invisible energy (mu, nu)
      Double_t   fAuxiliaryEnergy; 	  //anything else (eg. undecayed sigmas)
      Int_t      fMatchedMCFlavor;        //pdgID of the quark flavor that the jet matched to

    ClassDef(GenJet, 1) // GenJet class
  };
}
#endif