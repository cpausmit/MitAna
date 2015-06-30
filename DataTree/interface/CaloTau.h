//--------------------------------------------------------------------------------------------------
// CaloTau
//
// This class holds information about reconstructed Taus based on calorimeter towers, ecal basic
// clusters, and tracks.
//
// Authors: J.Bendavid
//--------------------------------------------------------------------------------------------------

#ifndef MITANA_DATATREE_CALOTAU_H
#define MITANA_DATATREE_CALOTAU_H
 
#include "MitAna/DataTree/interface/Tau.h"
#include "MitAna/DataCont/interface/RefArray.h"
#include "MitAna/DataCont/interface/Ref.h"
#include "MitAna/DataTree/interface/Track.h"
#include "MitAna/DataTree/interface/BasicCluster.h"
#include "MitAna/DataTree/interface/CaloJet.h"

namespace mithep 
{
  class CaloTau : public Tau
  {
    public:
      CaloTau() : Tau(), fLeadTrackSignD0Sig(0), fLeadTrack3x3HCalEt(0),
                  fLeadTrack3x3HCalDEta(0), fSignalTracksMass(0),
                  fTracksMass(0), fIsoTrackPtSum(0), fIsoECalEtSum(0),
                  fMaxHCalHitEt(0) {}

      void                   AddIsoTrack(const Track *t)         { fIsoTracks.Add(t);              }
      void                   AddNeutralBC(const BasicCluster *c) { fNeutralBCs.Add(c);             }
      void                   AddSignalTrack(const Track *t)      
                                { ClearCharge(); fSignalTracks.Add(t); }
      Double_t               IsoECalEtSum()                const { return fIsoECalEtSum;           }
      const Track           *IsoTrack(UInt_t i)            const { return fIsoTracks.At(i);        }
      Double_t               IsoTrackPtSum()               const { return fIsoTrackPtSum;          }
      const Track           *LeadTrack()                   const { return fLeadTrack.Obj();        }
      Double_t               LeadTrack3x3HCalEt()          const { return fLeadTrack3x3HCalEt;     }
      Double_t               LeadTrack3x3HCalDEta()        const { return fLeadTrack3x3HCalDEta;   }
      Double_t               LeadTrackSignedD0Sig()        const { return fLeadTrackSignD0Sig;     }
      Double_t               MaxHCalHitEt()                const { return fMaxHCalHitEt;           }
      UInt_t                 NIsoTracks()                  const { return fIsoTracks.Entries();    }
      const BasicCluster    *NeutralBC(UInt_t i)           const { return fNeutralBCs.At(i);       }
      UInt_t                 NNeutralBCs()                 const { return fNeutralBCs.Entries();   }
      UInt_t                 NSignalTracks()               const { return fSignalTracks.Entries(); }
      EObjType               ObjType()                     const { return kCaloTau;                }
      void                   SetCaloJet(const CaloJet *j)        { fCaloJet = j;                   }
      void                   SetIsoECalEtSum(Double_t x)         { fIsoECalEtSum = x;              }
      void                   SetIsoTrackPtSum(Double_t x)        { fIsoTrackPtSum = x;             }
      void                   SetLeadTrack(const Track *t)        { fLeadTrack = t;                 }
      void                   SetLeadTrack3x3HCalDEta(Double_t x) { fLeadTrack3x3HCalDEta = x;      }
      void                   SetLeadTrack3x3HCalEt(Double_t x)   { fLeadTrack3x3HCalEt = x;        }
      void                   SetLeadTrackSignD0Sig(Double_t x)   { fLeadTrackSignD0Sig = x;        }
      void                   SetMaxHCalHitEt(Double_t x)         { fMaxHCalHitEt = x;              }
      void                   SetSignalTracksMass(Double_t x)     { fSignalTracksMass = x;          }
      void                   SetTracksMass(Double_t x)           { fTracksMass = x;                }
      const Track           *SignalTrack(UInt_t i)         const { return fSignalTracks.At(i);     }
      Double_t               SignalTracksMass()            const { return fSignalTracksMass;       }
      const CaloJet         *SourceCaloJet()               const { return fCaloJet.Obj();          }
      const Jet             *SourceJet()                   const { return SourceCaloJet();         }
      Double_t               TracksMass()                  const { return fTracksMass;             }

      // Some structural tools
      void                   Mark(UInt_t  i = 1)           const;

    protected:
      Double_t               GetCharge()                   const;

      Double32_t             fLeadTrackSignD0Sig;   //[0,0,14]signed leading track D0 significance
      Double32_t             fLeadTrack3x3HCalEt;   //[0,0,14]hcal 3x3 calo tower et around lead trk
      Double32_t             fLeadTrack3x3HCalDEta; //[0,0,14]abs(delta eta) between lead track 
                                                    //        and hottest hcal rec hit in 3x3 region
      Double32_t             fSignalTracksMass;     //[0,0,14]invariant mass of signal tracks
      Double32_t             fTracksMass;           //[0,0,14]invariant mass of signal + iso tracks
      Double32_t             fIsoTrackPtSum;        //[0,0,14]sum pt of tracks in isolation annulus
      Double32_t             fIsoECalEtSum;         //[0,0,14]sum et of ecal rechits in iso annulus
      Double32_t             fMaxHCalHitEt;         //[0,0,14]et of largest et hcal rechit
      Ref<Track>             fLeadTrack;            //leading track in signal cone
      Ref<CaloJet>           fCaloJet;              //original reconstructed calo jet
      RefArray<Track>        fSignalTracks;         //tracks in signal cone
      RefArray<Track>        fIsoTracks;            //tracks in isolation annulus
      RefArray<BasicCluster> fNeutralBCs;           //neutral Ecal basic clusters

    ClassDef(CaloTau, 2) // CaloTau class
  };
}

//--------------------------------------------------------------------------------------------------
inline void mithep::CaloTau::Mark(UInt_t ib) const
{
  // mark myself
  mithep::DataObject::Mark(ib);
  // mark my dependencies if they are there
  if (fLeadTrack.IsValid()) 
    fLeadTrack.Obj()->Mark(ib);
  if (fCaloJet.IsValid())
    fCaloJet.Obj()->Mark(ib);
  // arrays
  fSignalTracks.Mark(ib);
  fIsoTracks   .Mark(ib);
  fNeutralBCs  .Mark(ib);
}

//--------------------------------------------------------------------------------------------------
inline Double_t mithep::CaloTau::GetCharge() const
{
  // Get charge from signal tracks.

  Double_t sumq = 0;
  for (UInt_t i=0; i<fSignalTracks.Entries(); ++i) {
    sumq += fSignalTracks.At(i)->Charge();
  }
  return sumq;
}
#endif
