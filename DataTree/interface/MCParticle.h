//--------------------------------------------------------------------------------------------------
// MCParticle
//
// Stores MC information for both generated and simulated particles.
//
// Authors: C.Loizides, J.Bendavid, C.Paus
//--------------------------------------------------------------------------------------------------

#ifndef MITANA_DATATREE_MCPARTICLE_H
#define MITANA_DATATREE_MCPARTICLE_H

#include <TDatabasePDG.h>
#include <TParticlePDG.h> 
#include "MitCommon/DataFormats/interface/Vect3.h"
#include "MitCommon/DataFormats/interface/Vect4M.h"
#include "MitAna/DataCont/interface/Ref.h"
#include "MitAna/DataCont/interface/BitMask.h"
#include "MitAna/DataTree/interface/CompositeParticle.h"

namespace mithep 
{
  class MCParticle : public CompositeParticle
  {
    public:
      enum EPartType {
        kUnknown=0, 
        kUp=1, kDown=2, kStrange=3, kCharm=4, kBottom=5, kTop=6,
        kEl=11, kMu=13, kTau=15, 
        kElNu=12, kMuNu=14, kTauNu=16,
        kGlu=21, kGamma=22, kZ=23, kW=24, kH=25,
        kZp=32, kZpp=33, kWp=34, kH0=35, kA0=36, kHp=37,
        kPi0=111, kEta=221, kKLong=130, kKShort=310, kK0=311,
        kD0=413, kB0=511, kB0Bar=513, kJPsi=443, kUpsilon=553,
        kProton=2122, kNeutron=2122, kDeltaPlusPlus = 2224,
        kDeltaPlus = 2214, kDelta0 = 2114, kDeltaMinus=1114
      };

      MCParticle() : fPdgId(0), fStatus(0), fIsGenerated(kFALSE), fIsSimulated(kFALSE) {}
      MCParticle(Double_t px, Double_t py, Double_t pz, Double_t e, Int_t id, Int_t s) : 
        fPdgId(id), fStatus(s), fMom(FourVector(px,py,pz,e)),
        fIsGenerated(kFALSE), fIsSimulated(kFALSE) {}

      Int_t               AbsPdgId()               const   { return (fPdgId<0 ? -fPdgId:fPdgId); }
      void		  AddDaughter(const MCParticle *p) { fDaughters.Add(p);                  }
      const ThreeVector   DecayVertex()            const   { return fDecayVertex.V();            }
      const MCParticle   *Daughter(UInt_t i)       const;
      const MCParticle   *DistinctMother()         const;
      using CompositeParticle::HasDaughter;
      const MCParticle   *FindDaughter(Int_t pid, 
                                       Bool_t checkCharge=kFALSE, const MCParticle *start=0) const;
      const MCParticle   *FindMother(Int_t pid, Bool_t checkCharge=kFALSE)  const;
      Bool_t              HasDaughter(Int_t pid, Bool_t checkCharge=kFALSE) const;
      Bool_t              HasMother()              const { return fMother.IsValid(); }
      Bool_t              HasMother(const MCParticle *m)                    const;
      Bool_t              HasMother(Int_t pid, Bool_t checkCharge=kFALSE)   const;
      Bool_t              Is(Int_t pid, Bool_t checkCharge=kFALSE)          const;
      Bool_t              IsCharged()              const { return !IsNeutral();      }
      Bool_t              IsGenerated()            const { return fIsGenerated;      }
      Bool_t              IsGluon()                const { return fPdgId == kGlu;    }
      Bool_t              IsLepton()               const;
      Bool_t              IsNeutral()              const;
      Bool_t              IsNeutrino()             const;
      Bool_t              IsNot(Int_t pid, Bool_t checkCharge=kFALSE)       const;
      Bool_t              IsParton()               const { return (IsGluon() || IsQuark());       }
      Bool_t              IsQuark()                const { return (AbsPdgId()>0 && AbsPdgId()<7); }
      Bool_t              IsSimulated()            const { return fIsSimulated;  }
      const MCParticle   *Mother()                 const { return fMother.Obj(); }
      EObjType            ObjType()                const { return kMCParticle;   }      
      void                SetIsGenerated(Bool_t t=kTRUE) { fIsGenerated = t;     }
      void                SetIsSimulated(Bool_t t=kTRUE) { fIsSimulated = t;     }
      TParticlePDG       *PdgEntry()               const;
      Int_t               PdgId()                  const { return fPdgId;  }
      Double_t            PdgMass()                const;
      Bool_t              StatusFlag(UInt_t i)     const { return fStatusFlags.TestBit(i); }
      void		  SetMom(Double_t px, Double_t py, Double_t pz, Double_t e);
      void		  SetMother(const MCParticle *p) { fMother = p;    }
      void                SetStatus(Int_t s)             { fStatus = s;    }
      void                SetVertex(Double_t x, Double_t y, Double_t z);
      void                SetPdgId(Int_t s)              {  fPdgId = s;    }
      void                SetStatusFlag(UInt_t i, Bool_t b) { fStatusFlags.SetBit(i, b); }
      Int_t               Status()                 const { return fStatus; }
      void                Print(Option_t *opt="")  const;

      // Some structural tools
      void                MarkMother(UInt_t i=1)   const;

    protected:
      Double_t            GetCharge()              const;
      void                GetMom()                 const;

      Int_t               fPdgId;        //pdg identifier
      Short_t             fStatus;       //status flag of generator or simulation
      Vect4M              fMom;          //four momentum vector
      Vect3               fDecayVertex;  //gen decay vertex
      Ref<MCParticle>     fMother;       //reference to mother
      Bool_t              fIsGenerated;  //=true if generated particle
      Bool_t              fIsSimulated;  //=true if simulated particle
      mithep::BitMask<2>  fStatusFlags;  //pythia8 status flags

    ClassDef(MCParticle,3) // Generated particle class
  };
}

//--------------------------------------------------------------------------------------------------
inline void mithep::MCParticle::MarkMother(UInt_t ib) const
{
  // mark myself
  mithep::DataObject::Mark(ib);
  // mark only the mother reference (as opposed to standard Mark() function which marks
  // only daughters, keep them seperate to avoid circular references
  if (fMother.IsValid())
    fMother.Obj()->Mark(ib);

}

//--------------------------------------------------------------------------------------------------
inline const mithep::MCParticle *mithep::MCParticle::Daughter(UInt_t i) const 
{ 
  // Return daughter corresponding to given index.

  return static_cast<const MCParticle*>(fDaughters.At(i)); 
}

//--------------------------------------------------------------------------------------------------
inline const mithep::MCParticle *mithep::MCParticle::DistinctMother() const 
{ 
  // Return mother, walking up the tree until a particle with a different pdg from this one
  // is found.

  const mithep::MCParticle *mother = Mother();
  
  if (!mother) 
    return 0;
  
  while (mother && mother->PdgId()==fPdgId)
    mother = mother->Mother();
    
  return mother;
}

//--------------------------------------------------------------------------------------------------
inline Double_t mithep::MCParticle::GetCharge() const
{
  // Get charge from pdg lookup.

  TParticlePDG *pdgEntry = PdgEntry();
  if (pdgEntry)
    return pdgEntry->Charge()/3.0;
  else {
    Error("Charge", "Pdg code for %i not found in table, returning charge=-99.0", fPdgId);
    return -99.0;
  }
}

//--------------------------------------------------------------------------------------------------
inline void mithep::MCParticle::GetMom() const
{
  // Get momentum values from stored values.

  fCachedMom.SetCoordinates(fMom.Pt(), fMom.Eta(), fMom.Phi(), fMom.M());
}

//--------------------------------------------------------------------------------------------------
inline Bool_t mithep::MCParticle::HasDaughter(Int_t pid, Bool_t checkCharge) const
{
  // Return true if a particle with given pdg code is found amoung daughters.
  // If checkCharge is false then just the type of particle is checked 
  // (ie particle and anti-particle).

  if (checkCharge) {
    for (UInt_t i=0; i<NDaughters(); ++i)
      if (Daughter(i)->PdgId()==pid) 
        return kTRUE;
  } else {
    Int_t apid = pid>0?pid:-pid;
    for (UInt_t i=0; i<NDaughters(); ++i)
      if (Daughter(i)->AbsPdgId()==apid) 
        return kTRUE;
  }
  return kFALSE;
}

//--------------------------------------------------------------------------------------------------
inline Bool_t mithep::MCParticle::HasMother(const MCParticle *m) const
{
  // Return true if the given particle is among mothers. (Note the comparison
  // is made on pointers and therefore will fail if you work on copies.)

  if (!m) 
    return kFALSE;

  const mithep::MCParticle *mother = Mother();
  while (mother && mother!=m)
    mother = mother->Mother();

  if (mother) 
    return kTRUE;
  return kFALSE;
}

//--------------------------------------------------------------------------------------------------
inline Bool_t mithep::MCParticle::HasMother(Int_t pid, Bool_t checkCharge) const
{
  // Return true if a particle with given pdg code is found amoung mothers.
  // If checkCharge is false then just the type of particle is checked 
  // (ie particle and anti-particle).

  const mithep::MCParticle *mother = Mother();
  
  if (checkCharge) {
    while (mother && mother->PdgId()!=pid)
      mother = mother->Mother();
  } else {
    Int_t apid = pid>0?pid:-pid;
    while (mother && mother->AbsPdgId()!=apid)
      mother = mother->Mother();
  }

  if (mother) 
    return kTRUE;
  return kFALSE;
}

//--------------------------------------------------------------------------------------------------
inline Bool_t mithep::MCParticle::Is(Int_t pid, Bool_t checkCharge) const 
{ 
  // Return true if particle is of given type. If checkCharge is false then just the type of 
  // particle is checked (ie particle and anti-particle).

  if (checkCharge) 
    return (PdgId() == pid);

  Int_t apid = pid>0?pid:-pid;
  return (AbsPdgId() == apid);
}

//--------------------------------------------------------------------------------------------------
inline Bool_t mithep::MCParticle::IsNot(Int_t pid, Bool_t checkCharge) const 
{ 
  // Return true if particle is not of given type. If checkCharge is false then just the type of 
  // particle is checked (ie particle and anti-particle).

  return !Is(pid, checkCharge);
}

//--------------------------------------------------------------------------------------------------
inline Bool_t mithep::MCParticle::IsLepton() const 
{ 
  // Return true if particle is a lepton.

  Int_t ap = AbsPdgId();
  if ((ap==kEl) ||
      (ap==kMu) ||
      (ap==kTau)) 
    return kTRUE;

  return kFALSE;
}

//--------------------------------------------------------------------------------------------------
inline Bool_t mithep::MCParticle::IsNeutral() const 
{ 
  // Return true if particle is a neutral.

  return (Charge()!=0);
}

//--------------------------------------------------------------------------------------------------
inline Bool_t mithep::MCParticle::IsNeutrino() const 
{ 
  // Return true if particle is a neutrino.

  Int_t ap = AbsPdgId();
  if ((ap==kElNu) ||
      (ap==kMuNu) ||
      (ap==kTauNu)) 
    return kTRUE;

  return kFALSE;
}

//--------------------------------------------------------------------------------------------------
inline TParticlePDG *mithep::MCParticle::PdgEntry() const 
{ 
  // Return entry to pdg database.

  return TDatabasePDG::Instance()->GetParticle(fPdgId); 
}

//--------------------------------------------------------------------------------------------------
inline Double_t mithep::MCParticle::PdgMass() const 
{ 
  // Return mass obtained from pdg database.

  TParticlePDG *pdg = PdgEntry();
  if (pdg)
    return pdg->Mass();
  return 0;
}

//--------------------------------------------------------------------------------------------------
inline void mithep::MCParticle::SetMom(Double_t px, Double_t py, Double_t pz, Double_t e)
{ 
  // Set four vector.

  fMom.SetXYZT(px, py, pz, e);
  ClearMom();
}

//--------------------------------------------------------------------------------------------------
inline void mithep::MCParticle::SetVertex(Double_t x, Double_t y, Double_t z)
{
  // Set decay vertex.

  fDecayVertex.SetXYZ(x,y,z);
}
#endif
