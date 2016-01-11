//--------------------------------------------------------------------------------------------------
// PFCandidate
//
// Particle-flow candidate class, for now mostly mirroring the PFCandidate from CMSSW.
//
// Authors: J.Bendavid, Y.Iiyama
//--------------------------------------------------------------------------------------------------

#ifndef MITANA_DATATREE_PFCandidate_H
#define MITANA_DATATREE_PFCandidate_H

#include "MitCommon/DataFormats/interface/Vect3.h"
#include "MitCommon/DataFormats/interface/Vect4M.h"
#include "MitAna/DataCont/interface/Ref.h"
#include "MitAna/DataTree/interface/CompositeParticle.h"
#include "MitAna/DataTree/interface/Track.h"
#include "MitAna/DataTree/interface/SuperCluster.h"

namespace mithep 
{
  class Muon;
  class Electron;
  class Photon;

  class PFCandidate : public CompositeParticle
  {
    public:
      enum EPFType { 
        eX = 0,          //unidentified
        eHadron,         //charged hadron
        eElectron,       //electron
        eMuon,           //muon
        eGamma,          //photon
        eNeutralHadron,  //neutral hadron
        eHadronHF,       //hadron in HF
        eEGammaHF        //EM object in HF
      };

      enum EPFFlags { 
        eNormal = 0,
        eEMPhiSModules,
        eEMEta0,
        eEMEtaModules,
        eEMBarrelEndcap,
        eEMPreshowerEdge,
        eEMPreshower,
        eEMEndCapEdge,
        eHEta0,
        eHBarrelEndcap,
        eHEndcapVFCal,
        eHVFCalEdge,
        eToDispVtx,
        eFromDispVtx,
        eFromV0,
        eFromGammaConv,
        eToConversion,
	ePFNoPileup
      };

      PFCandidate() : fPFType(eX) {}

      void		  AddDaughter(const PFCandidate *p) { fDaughters.Add(p);                  }
      void                ClearFlag(EPFFlags f)             { fPFFlags.ClearBit(f);               }
      void                ClearFlags()                      { fPFFlags.Clear();                   }
      const PFCandidate  *Daughter(UInt_t i)       const;
      Double_t            EECal()                  const    { return fEECal;                      }
      Double_t            EHCal()                  const    { return fEHCal;                      }
      Double_t            PError()                 const    { return fPError;                     }
      PFCandidate*        MakeCopy()               const    { return new PFCandidate(*this);      }
      Double_t            MvaGamma()               const    { return fMvaGamma;                   }
      Double_t            EtaECal()                const    { return fEtaECal;                    }
      Double_t            PhiECal()                const    { return fPhiECal;                    }
      Bool_t              Flag(EPFFlags f)         const    { return fPFFlags.TestBit(f);         }
      Bool_t              HasMother()              const    { return fMother.IsValid();           }
      Bool_t              HasMother(const PFCandidate *m) const;
      Bool_t              HasTrackerTrk()          const    { return fTrackerTrack.IsValid();     }
      Bool_t              HasGsfTrk()              const    { return fGsfTrack.IsValid();         }
      Bool_t              HasTrk()                 const 
                            { return (HasTrackerTrk() || HasGsfTrk()); }
      Bool_t              HasSCluster()            const    { return fSCluster.IsValid();         }
      const PFCandidate  *Mother()                 const    { return fMother.Obj();               }
      const Muon         *Mu()                     const;
      const Electron     *Ele()                    const;
      const Photon       *Pho()                    const;
      EObjType            ObjType()                const    { return kPFCandidate;                }
      EPFType             PFType()                 const    { return fPFType;                     }
      const SuperCluster *SCluster()               const    { return fSCluster.Obj();             }
      void                SetCharge(Double_t c)             { fCharge = c; ClearCharge();         }
      void                SetEECal(Double_t e)              { fEECal = e;                         }
      void                SetEHCal(Double_t e)              { fEHCal = e;                         }
      void                SetPError(Double_t err)           { fPError = err;                      }
      void                SetMvaGamma(Double_t d)           { fMvaGamma = d;                      }
      void                SetEtaECal(Double_t eta)          { fEtaECal = eta;                     }
      void                SetPhiECal(Double_t phi)          { fPhiECal = phi;                     }
      void                SetPFType(EPFType t)              { fPFType = t;                        }
      void                SetFlag(EPFFlags f, Bool_t b=1)   { fPFFlags.SetBit(f,b);               }
      void                SetPtEtaPhiM(Double_t pt, Double_t eta, Double_t phi, Double_t m);
      void		  SetMom(Double_t px, Double_t py, Double_t pz, Double_t e);
      void		  SetMother(const PFCandidate *p)   { fMother = p;                        }
      void                SetTrackerTrk(const Track *t)     { fTrackerTrack = t;                  }
      void                SetGsfTrk(const Track *t)         { fGsfTrack = t;                      }
      void                SetMuon(const Muon *);
      void                SetElectron(const Electron *);
      void                SetPhoton(const Photon *);
      void                SetSCluster(const SuperCluster *s) { fSCluster = s;                     }
      void                SetVertex(Double_t x, Double_t y, Double_t z);
      const ThreeVector   SourceVertex()           const    { return fSourceVertex.V();           }
      const Track        *TrackerTrk()             const    { return fTrackerTrack.Obj();         }
      const Track        *GsfTrk()                 const    { return fGsfTrack.Obj();             }
      const Track        *BestTrk()                const;
      const Track        *Trk()                    const    { return BestTrk();                   }
      
      // Some structural tools
      void                Mark(UInt_t i=1)         const;

    protected:
      Double_t            GetCharge()              const;
      void                GetMom()                 const;

      Vect4M              fMom;              //four momentum vector
      Vect3               fSourceVertex;     //pflow source vertex
      Double32_t          fCharge;           //[-1,1,2]charge
      Double32_t          fEECal;            //[0,0,14]corrected Ecal energy
      Double32_t          fEHCal;            //[0,0,14]corrected Hcal energy
      Double32_t          fPError;           //[0,0,14]uncertainty on P (three-mom magnitude)
      Double32_t          fMvaGamma;         //[0,0,14]photon id discriminant
      Double32_t          fEtaECal;          //[0,0,12]eta at ecal front face
      Double32_t          fPhiECal;          //[0,0,12]phi at ecal front face
      EPFType             fPFType;           //particle flow type
      BitMask32           fPFFlags;          //various PF flags
      Ref<PFCandidate>    fMother;           //reference to mother
      Ref<Track>          fTrackerTrack;     //reference to (standard) track
      Ref<Track>          fGsfTrack;         //reference to gsf track (for electrons only)
      Ref<Muon>           fMuon;             //reference to corresponding reco muon
      Ref<SuperCluster>   fSCluster;         //reference to egamma supercluster
      Ref<Electron>       fElectron;         //reference to electron
      Ref<Photon>         fPhoton;           //reference to egamma photon

    ClassDef(PFCandidate,5) // Particle-flow candidate class
  };
}

//--------------------------------------------------------------------------------------------------
inline const mithep::Track *mithep::PFCandidate::BestTrk() const 
{ 
  // Return gsf track if present, or else tracker track if present

  if (HasGsfTrk())
    return GsfTrk();

  if (HasTrackerTrk())
    return TrackerTrk();

  return 0;

}

//--------------------------------------------------------------------------------------------------
inline const mithep::PFCandidate *mithep::PFCandidate::Daughter(UInt_t i) const 
{ 
  // Return daughter corresponding to given index.

  return static_cast<const PFCandidate*>(fDaughters.At(i)); 
}

//--------------------------------------------------------------------------------------------------
inline Double_t mithep::PFCandidate::GetCharge() const
{
  // Get stored charge

  return fCharge;
}

//--------------------------------------------------------------------------------------------------
inline void mithep::PFCandidate::GetMom() const
{
  // Get momentum values from stored values.

  fCachedMom.SetCoordinates(fMom.Pt(),fMom.Eta(),fMom.Phi(),fMom.M()); 
}

//--------------------------------------------------------------------------------------------------
inline Bool_t mithep::PFCandidate::HasMother(const PFCandidate *m) const
{
  // Return true if the given particle is among mothers. (Note the comparison
  // is made on pointers and therefore will fail if you work on copies.)

  if (!m) 
    return kFALSE;

  const mithep::PFCandidate *mother = Mother();
  while (mother && mother!=m)
    mother = mother->Mother();

  if (mother) 
    return kTRUE;
  return kFALSE;
}

//--------------------------------------------------------------------------------------------------
inline void mithep::PFCandidate::SetPtEtaPhiM(Double_t pt, Double_t eta, Double_t phi, Double_t m)
{ 
  // Set four vector.

  fMom.Set(pt, eta, phi, m);
  ClearMom();
}

//--------------------------------------------------------------------------------------------------
inline void mithep::PFCandidate::SetMom(Double_t px, Double_t py, Double_t pz, Double_t e)
{ 
  // Set four vector.

  fMom.SetXYZT(px, py, pz, e);
  ClearMom();
}

//--------------------------------------------------------------------------------------------------
inline void mithep::PFCandidate::SetVertex(Double_t x, Double_t y, Double_t z)
{
  // Set decay vertex.

  fSourceVertex.SetXYZ(x,y,z);
}
#endif
