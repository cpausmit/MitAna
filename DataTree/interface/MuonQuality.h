//--------------------------------------------------------------------------------------------------
// MuonQuality
//
// Wrapper for BitMask storing the Muon Quality flags. We save information on exactly which 
// of the official muon ID cuts are passed.
//
// Authors: S.Xie
//--------------------------------------------------------------------------------------------------

#ifndef MITANA_DATATREE_MUONQUALITY_H
#define MITANA_DATATREE_MUONQUALITY_H
 
#include "MitAna/DataCont/interface/BitMask.h"
#include "MitAna/DataCont/interface/Types.h"

namespace mithep 
{
  class MuonQuality
  {
    public:
      enum MuonSelectionType { //taken from DataFormats/MuonReco/interface/MuonSelectors.h
        All,                      //dummy options - always true
        AllGlobalMuons,           //checks isGlobalMuon flag
        AllStandAloneMuons,       //checks isStandAloneMuon flag
        AllTrackerMuons,          //checks isTrackerMuon flag
        TrackerMuonArbitrated,    //resolve ambiguity of sharing segments
        AllArbitrated,            //all muons with the tracker muon arbitrated
        GlobalMuonPromptTight,    //global muons with tighter fit requirements
        TMLastStationLoose,       //penetration depth loose selector
        TMLastStationTight,       //penetration depth tight selector
        TM2DCompatibilityLoose,   //likelihood based loose selector
        TM2DCompatibilityTight,  //likelihood based tight selector
        TMOneStationLoose,       //require one well matched segment
        TMOneStationTight,       //require one well matched segment
        TMLastStationOptimizedLowPtLoose, //combination of TMLastStation and TMOneStation
        TMLastStationOptimizedLowPtTight, //combination of TMLastStation and TMOneStation
        GMTkChiCompatibility,    //require tk stub have good chi2 relative to glb track
        GMStaChiCompatibility,   //require sta stub have good chi2 compatibility 
                                      //relative to glb track
        GMTkKinkTight,           //require a small kink value in the tracker stub
        TMLastStationAngLoose,   //TMLastStationLoose with additional angular cuts
        TMLastStationAngTight,   //TMLastStationTight with additional angular cuts
        TMOneStationAngLoose,    //TMOneStationLoose with additional angular cuts
        TMOneStationAngTight,    //TMOneStationTight with additional angular cuts
        //The two algorithms that follow are identical to what were known as
        //TMLastStationOptimizedLowPt* (sans the Barrel) as late as revision
        //1.7 of this file. The names were changed because indeed the low pt
        //optimization applies only to the barrel region, whereas the sel-
        //ectors above are more efficient at low pt in the endcaps, which is
        //what we feel is more suggestive of the algorithm name. This will be
        //less confusing for future generations of CMS members, I hope...
        TMLastStationOptimizedBarrelLowPtLoose, //combination of TMLastStation and TMOneStation 
                                                //but with low pT optimization in barrel only
        TMLastStationOptimizedBarrelLowPtTight  //combination of TMLastStation and TMOneStation 
                                                //but with low pT optimization in barrel only
      };

      MuonQuality() {}
      virtual ~MuonQuality() {}

      void                 ClearQuality(MuonSelectionType q)  { fQualityMask.ClearBit(q);       } 
      Bool_t               Quality(MuonSelectionType q) const { return fQualityMask.TestBit(q); }
      const BitMask32     &QualityMask()                const { return fQualityMask;            }
      UInt_t               NQuality()                   const { return fQualityMask.NBitsSet(); }
      void                 SetQuality(MuonSelectionType q)    { fQualityMask.SetBit(q);         }
      void                 SetQualityMask(const BitMask32 &q) { fQualityMask = q;               }

    protected:
      BitMask32            fQualityMask;         //track quality bitmask
	      
    ClassDef(MuonQuality, 1) 
  };
}
#endif
