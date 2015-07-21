//--------------------------------------------------------------------------------------------------
// $Id: PFJet.h,v 1.7 2012/03/28 12:15:34 paus Exp $
//
// XlFatJet
//
// This class holds information about reconstructed jets and their substructure
//
// Authors: L.DiMatteo
//--------------------------------------------------------------------------------------------------

#ifndef MITANA_DATATREE_XLFATJET_H
#define MITANA_DATATREE_XLFATJET_H

#include "MitAna/DataTree/interface/FatJet.h"
#include "MitAna/DataCont/interface/RefArray.h"
#include "MitAna/DataTree/interface/Types.h"
#include "MitCommon/DataFormats/interface/Vect4M.h"
#include "MitAna/DataTree/interface/XlSubJet.h"

namespace mithep
{
  class XlFatJet : public FatJet
  {
    public:

      XlFatJet() :
                fQGTag(0),
                fC2b0(0), fC2b0p2(0), fC2b0p5(0), fC2b1(0), fC2b2(0),
                fMassSDb0(0), fMassSDb1(0), fMassSDb2(0), fMassSDbm1(0),
                fMassPruned(0), fMassFiltered(0), fMassTrimmed(0),
                fPull(0), fPullAngle(0) {}
      XlFatJet(const FatJet & f) :
                FatJet(f),
                fQGTag(0),
                fC2b0(0), fC2b0p2(0), fC2b0p5(0), fC2b1(0), fC2b2(0),
                fMassSDb0(0), fMassSDb1(0), fMassSDb2(0), fMassSDbm1(0),
                fMassPruned(0), fMassFiltered(0), fMassTrimmed(0),
                fPull(0), fPullAngle(0) {}

      ~XlFatJet();
      Jet                  *MakeCopy()                      const { return new XlFatJet(*this);    }
      Double_t              QGTag()                         const { return fQGTag;                 }
      Double_t              C2b0()                          const { return fC2b0;                  }
      Double_t              C2b0p2()                        const { return fC2b0p2;                }
      Double_t              C2b0p5()                        const { return fC2b0p5;                }
      Double_t              C2b1()                          const { return fC2b1;                  }
      Double_t              C2b2()                          const { return fC2b2;                  }
      Double_t              MassSDb0()                      const { return fMassSDb0;              }
      Double_t              MassSDb1()                      const { return fMassSDb1;              }
      Double_t              MassSDb2()                      const { return fMassSDb2;              }
      Double_t              MassSDbm1()                     const { return fMassSDbm1;             }
      Double_t              MassPruned()                    const { return fMassPruned;            }
      Double_t              MassFiltered()                  const { return fMassFiltered;          }
      Double_t              MassTrimmed()                   const { return fMassTrimmed;           }
      Double_t              Pull()                          const { return fPull;                  }
      Double_t              PullAngle()                     const { return fPullAngle;             }
      Double_t              chi()                           const { return fChi;                   }
      Int_t                 nMicrojets()                    const { return fNMicrojets;            }

      void                  SetQGTag(Double_t t)                  { fQGTag       = t;              }
      void                  SetC2b0(Double_t t)                   { fC2b0        = t;              }
      void                  SetC2b0p2(Double_t t)                 { fC2b0p2      = t;              }
      void                  SetC2b0p5(Double_t t)                 { fC2b0p5      = t;              }
      void                  SetC2b1(Double_t t)                   { fC2b1        = t;              }
      void                  SetC2b2(Double_t t)                   { fC2b2        = t;              }
      void                  SetMassSDb0(Double_t t)               { fMassSDb0    = t;              }
      void                  SetMassSDb1(Double_t t)               { fMassSDb1    = t;              }
      void                  SetMassSDb2(Double_t t)               { fMassSDb2    = t;              }
      void                  SetMassSDbm1(Double_t t)              { fMassSDbm1   = t;              }
      void                  SetMassPruned(Double_t t)             { fMassPruned  = t;              }
      void                  SetMassFiltered(Double_t t)           { fMassFiltered = t;             }
      void                  SetMassTrimmed(Double_t t)            { fMassTrimmed = t;              }
      void                  SetPull(Double_t t)                   { fPull = t;                     }
      void                  SetPullAngle(Double_t t)              { fPullAngle = t;                }
      void                  SetChi(Double_t t)                    { fChi = t;                      }
      void                  SetNMicrojets(Int_t t)                { fNMicrojets = t;               }


    protected:

      Double32_t            fQGTag;        //QG tagging
      Double32_t            fC2b0;         //ECF ratio order 2, beta 0
      Double32_t            fC2b0p2;       //ECF ratio order 2, beta 0.2
      Double32_t            fC2b0p5;       //ECF ratio order 2, beta 0.2
      Double32_t            fC2b1;         //ECF ratio order 2, beta 1
      Double32_t            fC2b2;         //ECF ratio order 2, beta 2
      Double32_t            fMassSDb0;     //Groomed mass (soft drop b 0)
      Double32_t            fMassSDb1;     //Groomed mass (soft drop b 1)
      Double32_t            fMassSDb2;     //Groomed mass (soft drop b 2)
      Double32_t            fMassSDbm1;    //Groomed mass (soft drop b-1)
      Double32_t            fMassPruned;   //Groomed mass (pruning)
      Double32_t            fMassFiltered; //Groomed mass (filtering)
      Double32_t            fMassTrimmed;  //Groomed mass (trimming)
      Double32_t            fPull;         //Color pull
      Double32_t            fPullAngle;    //Angle between pulls of lead/subleading subjets:
                                           //either choose 2-prong or 3-prong subclustering!
      Double32_t            fChi=-999;          // shower deconstruction probability
      Int_t                 fNMicrojets;
      RefArray<XlSubJet>    fSubJets;      //sub jets in the jet

    ClassDef(XlFatJet, 5) // XlFatJet class
  };
}
#endif
