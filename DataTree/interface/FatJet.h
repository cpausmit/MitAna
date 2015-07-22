//--------------------------------------------------------------------------------------------------
// $Id: PFJet.h,v 1.7 2012/03/28 12:15:34 paus Exp $
//
// FatJet
//
// This class holds information about reconstructed jets and their substructure
//
// Authors: L.DiMatteo, S.Narayanan
//--------------------------------------------------------------------------------------------------

#ifndef MITANA_DATATREE_FATJET_H
#define MITANA_DATATREE_FATJET_H

#include "MitAna/DataTree/interface/PFJet.h"
#include "MitAna/DataCont/interface/RefArray.h"
#include "MitAna/DataTree/interface/Types.h"
#include "MitCommon/DataFormats/interface/Vect4M.h"
#include "MitAna/DataTree/interface/XlSubJet.h"

using namespace std;

namespace mithep
{
  class FatJet : public PFJet
  {
    public:
      struct TrackData {
        double length;             // per track
        double dist;
        double dxy;
        double dz;
        double IP2D ;
        double IP2Dsig ;
        double IP ;
        double IPsig ;
        double IP2Derr ;
        double IPerr ;
        double prob ;
        double pt ;
        double eta ;
        double phi ;
        double PVWeight ;
        double SVWeight ;
        int PV ;
        int fromSV ;
        int SV ;
      };

      struct SVData {
        double flight;                //per SV
        double flightErr;
        double deltaRJet;
        double deltaRSumJet;
        double deltaRSumDir;
        double flight2D;
        double flight2DErr;
        double totCharge;
        double vtxDistJetAxis;
        int nTrk;
        double mass;
        double energyRatio;
        double pt;
        double eta;
        double phi;
        double dirX;
        double dirY;
        double dirZ;
      };

      struct LeptonData {
        double pt;
        double eta;
        double phi;
        double ptRel;
        double ratio;
        double ratioRel;
        double IP;
        double IP2D;
      };

      FatJet() :
                fCharge (0),
                fTau1IVF(-1), fTau2IVF(-1), fTau3IVF(),
                fTau1(-1), fTau2(-1), fTau3(-1), fTau4(-1),
                fQJetVol(0),
                fTauDot(-1), fZRatio(-1) {}
      FatJet(const PFJet & p) :
                PFJet(p),
                fCharge (0),
                fTau1IVF(-1), fTau2IVF(-1), fTau3IVF(),
                fTau1(-1), fTau2(-1), fTau3(-1), fTau4(-1),
                fQJetVol(0),
                fTauDot(-1), fZRatio(-1) {}

      ~FatJet();
      // const XlSubJet       *SubJet(UInt_t i)                const { return fSubJets.At(i);         }
      // const XlSubJet       *SubJet(UInt_t i, XlSubJet::ESubJetType t) const;
      Bool_t                HasSubJet(const XlSubJet *p, XlSubJet::ESubJetType t)    const;
      Jet                  *MakeCopy()                      const { return new FatJet(*this);    }
      Double_t              Charge()                        const { return fCharge;                }
      Double_t              Tau1IVF()                          const { return fTau1IVF;                  }
      Double_t              Tau2IVF()                          const { return fTau2IVF;                  }
      Double_t              Tau3IVF()                          const { return fTau3IVF;                  }
      Vect3                 GetTauIVFAxis(int i)               const { return fTauIVFAxes.at(i); }
      Double_t              Tau1()                          const { return fTau1;                  }
      Double_t              Tau2()                          const { return fTau2;                  }
      Double_t              Tau3()                          const { return fTau3;                  }
      Double_t              Tau4()                          const { return fTau4;                  }
      Double_t              QJetVol()                       const { return fQJetVol;               }
      Double_t              tauDot()                        const { return fTauDot;                }
      Double_t              zRatio()                        const { return fZRatio;                }
      vector<TrackData>    GetTrackData()                     const { return fTrackData;                }
      vector<SVData>       GetSVData()                        const { return fSVData;                   }
      vector<LeptonData>       GetMuonData()                        const { return fMuonData;                   }
      vector<LeptonData>       GetElectronData()                        const { return fElectronData;                   }
      const RefArray<XlSubJet>    GetSubJets(XlSubJet::ESubJetType t) const;

      // void                  AddSubJet(const XlSubJet *p)          { fSubJets.Add(p);               }
      void                  SetCharge()                           { fCharge  = this->GetCharge();  }
      void                  SetTau1IVF(Double_t t)                   { fTau1IVF        = t;              }
      void                  SetTau2IVF(Double_t t)                   { fTau2IVF        = t;              }
      void                  SetTau3IVF(Double_t t)                   { fTau3IVF        = t;              }
      void                  AddTauIVFAxis(Vect3 p)                { fTauIVFAxes.push_back(p);      }
      void                  SetTau1(Double_t t)                   { fTau1        = t;              }
      void                  SetTau2(Double_t t)                   { fTau2        = t;              }
      void                  SetTau3(Double_t t)                   { fTau3        = t;              }
      void                  SetTau4(Double_t t)                   { fTau4        = t;              }
      void                  SetQJetVol(Double_t t)                { fQJetVol     = t;              }
      void                  SetTauDot(Double_t t)                 { fTauDot = t;                   }
      void                  SetZRatio(Double_t t)                 { fZRatio = t;                   }
      void                  SetChi(Double_t t)                    { fChi = t;                      }
      void                  SetNMicrojets(Int_t t)                { fNMicrojets = t;               }
      void                  AddTrackData(TrackData *t)            { fTrackData.push_back(*t);       }
      void                  AddSVData(SVData *s)                  { fSVData.push_back(*s);          }
      void                  AddMuonData(LeptonData *s)            { fMuonData.push_back(*s);          }
      void                  AddElectronData(LeptonData *s)        { fElectronData.push_back(*s);          }
      void                  AddSubJet(const XlSubJet * sj);
      void                  AddSubJet(const XlSubJet * sj, XlSubJet::ESubJetType t);
      void                  SetPrunedP(Vect4M p)                 { fPrunedP = p;                  }
      void                  SetTrimmedP(Vect4M p)                { fTrimmedP = p;                  }
      // Some structural tools
      void                  Mark(UInt_t i=1)                const;

    protected:
      Double32_t            GetCharge()                     const;

      Double32_t            fCharge;       //Pt-weighted jet charge
      Double32_t            fTau1IVF;         //1-subjettiness
      Double32_t            fTau2IVF;         //2-subjettiness
      Double32_t            fTau3IVF;         //3-subjettiness
      std::vector<Vect3>    fTauIVFAxes;
      Double32_t            fTau1;         //1-subjettiness
      Double32_t            fTau2;         //2-subjettiness
      Double32_t            fTau3;         //3-subjettiness
      Double32_t            fTau4;         //4-subjettiness
      Double32_t            fQJetVol;      //QJets volatility
      Double32_t            fChi=-999;          // shower deconstruction probability
      Int_t                 fNMicrojets;
      Vect4M            fPrunedP;
      Vect4M            fTrimmedP;

      RefArray<XlSubJet>    fSubJets[XlSubJet::nSubJetTypes];      //sub jets in the jet

      // IVF variables
      Double32_t            fTauDot;
      Double32_t            fZRatio;
      vector<TrackData> fTrackData;
      vector<SVData> fSVData;
      vector<LeptonData> fMuonData;
      vector<LeptonData> fElectronData;

    ClassDef(FatJet, 0) // FatJet class
  };
}

//--------------------------------------------------------------------------------------------------
inline void mithep::FatJet::Mark(UInt_t ib) const
{
  // mark myself
  mithep::DataObject::Mark(ib);
  // mark my dependencies if they are there
  for (unsigned int i = 0; i!=XlSubJet::nSubJetTypes; ++i) {
    (fSubJets[i])->Mark(ib);
  }
}

//--------------------------------------------------------------------------------------------------
inline Double32_t mithep::FatJet::GetCharge() const
{
  // Return the sum of constituents PFCandidates weighted by their pt

  Double_t charge = 0;
  for (UInt_t i=0; i<NPFCands(); ++i)
    charge += PFCand(i)->Charge()*PFCand(i)->Pt();

  return charge/this->Pt();
}

#endif
