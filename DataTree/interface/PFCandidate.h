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

#include "TMath.h"

namespace mithep 
{
  class Muon;
  class Electron;
  class Photon;
  class Track;
  class SuperCluster;

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
      void                ClearFlag(EPFFlags f)             { fPFFlags.ClearBit(f);            }
      void                ClearFlags()                      { fPFFlags.Clear();                }
      const PFCandidate  *Daughter(UInt_t i)       const;
      Double_t            ECalo()                  const    { return (TMath::Exp(fCompressedECalo) - 1.) * E(); }
      Double_t            EECal()                  const    { return fEECal < -99. ? fECalEFraction / 256. * ECalo() : fEECal; }
      Double_t            EHCal()                  const    { return fEHCal < -99. ? (1. - fECalEFraction / 256.) * ECalo() : fEHCal; }
      Double_t            PError()                 const    { return fPError;                     }
      PFCandidate*        MakeCopy()               const    { return new PFCandidate(*this);      }
      Double_t            EtaECal()                const
      { return fEtaECal < -99. ? fEtaECalFraction / 256. * 7. + -3.5 : fEtaECal; }
      Double_t            PhiECal()                const
      { return fPhiECal < -99. ? fPhiECalFraction / 256. * 6.28318530 - 3.14159265 : fPhiECal; }
      Bool_t              Flag(EPFFlags f)         const    { return fPFFlags.TestBit(f);      }
      Bool_t              HasMother()              const    { return fMother.IsValid();           }
      Bool_t              HasMother(const PFCandidate *m) const;
      const PFCandidate  *Mother()                 const    { return fMother.Obj();               }
      EObjType            ObjType()                const    { return kPFCandidate;                }
      EPFType             PFType()                 const    { return EPFType(fPFType);            }
      void                SetCharge(Double_t c)             { fCharge = c; ClearCharge();         }
      void                SetECalo(Double_t e, Double_t h);
      void                SetPError(Double_t err)           { fPError = err;                      }
      void                SetEtaECal(Double_t eta)          { fEtaECalFraction = (eta + 3.5) / 7. * 256.; }
      void                SetPhiECal(Double_t phi)          { fPhiECalFraction = (phi + 3.14159265) / 6.28318530 * 256.; }
      void                SetPFType(EPFType t)              { fPFType = t;                        }
      void                SetFlag(EPFFlags f, Bool_t b = kTRUE) { fPFFlags.SetBit(f, b);       }
      void                SetPtEtaPhiM(Double_t pt, Double_t eta, Double_t phi, Double_t m);
      void		  SetMom(Double_t px, Double_t py, Double_t pz, Double_t e);
      void		  SetMother(const PFCandidate *p)   { fMother = p;                        }
      void                SetVertex(Double_t x, Double_t y, Double_t z);
      const ThreeVector   SourceVertex()           const    { return fSourceVertex.V();           }

      Bool_t              HasTrackerTrk()          const;
      Bool_t              HasGsfTrk()              const;
      Bool_t              HasTrk()                 const
                            { return (HasTrackerTrk() || HasGsfTrk()); }
      Bool_t              HasSCluster()            const;
      void                SetMuon(const Muon *);
      void                SetElectron(const Electron *);
      void                SetPhoton(const Photon *);
      void                SetTrackerTrk(const Track *);
      void                SetGsfTrk(const Track *);
      void                SetSCluster(const SuperCluster *);
      const Muon         *Mu()                     const;
      const Electron     *Ele()                    const;
      const Photon       *Pho()                    const;
      const Track        *TrackerTrk()             const;
      const Track        *GsfTrk()                 const;
      const Track        *BestTrk()                const;
      const Track        *Trk()                    const    { return BestTrk();                   }
      const SuperCluster *SCluster()               const;
      
      // Some structural tools
      void                Mark(UInt_t i=1)         const;

    protected:
      Double_t            GetCharge()              const;
      void                GetMom()                 const;

      Vect4M              fMom;              //four momentum vector
      Vect3               fSourceVertex;     //pflow source vertex
      Double32_t          fCharge;           //[-1,1,2]charge
      Float16_t           fPError;           //[0,0,8]uncertainty on P (three-mom magnitude)
      Double32_t          fCompressedECalo;  //[0,0,14] log[(calo energy) / (energy) + 1]
      Byte_t              fECalEFraction;    //corrected Ecal energy fraction in 1/256 steps
      Byte_t              fEtaECalFraction;  //eta at ecal front face in 7/256 steps from -3.5 to 3.5
      Byte_t              fPhiECalFraction;  //phi at ecal front face in 2pi/256 steps from -pi to pi
      Byte_t              fPFType;           //particle flow type
      BitMask32           fPFFlags;          //various PF flags
      Ref<PFCandidate>    fMother;           //reference to mother
      Ref<Track>          fTrackerTrack;     //reference to (standard) track
      Ref<Track>          fGsfTrack;         //reference to gsf track (for electrons only)
      Ref<Muon>           fMuon;             //reference to corresponding reco muon
      Ref<SuperCluster>   fSCluster;         //reference to egamma supercluster
      Ref<Electron>       fElectron;         //reference to electron
      Ref<Photon>         fPhoton;           //reference to egamma photon
      Double32_t          fEECal{-100.};     //! (deprecated)
      Double32_t          fEHCal{-100.};     //! (deprecated)
      Double32_t          fEtaECal{-100.};   //! (deprecated)
      Double32_t          fPhiECal{-100.};   //! (deprecated)

    ClassDef(PFCandidate,6) // Particle-flow candidate class
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

inline
void
mithep::PFCandidate::SetECalo(Double_t e, Double_t h)
{
  if (e < 0.)
    e = 0.;
  if (h < 0.)
    h = 0.;

  fCompressedECalo = TMath::Log((e + h) / E() + 1.);
  fECalEFraction = e / (e + h) * 256.;
}

#endif
