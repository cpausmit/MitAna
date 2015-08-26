//--------------------------------------------------------------------------------------------------
// AnaFwkMod
//
// This module collects interesting global information, like the overall number of processed
// events and puts this information into the OutputList.
//
// Authors: C.Loizides, C.Paus
//--------------------------------------------------------------------------------------------------
#ifndef MITANA_TREEEMOD_ANAFWKMOD_H
#define MITANA_TREEEMOD_ANAFWKMOD_H

#include "MitAna/TreeMod/interface/BaseMod.h" 
#include "MitAna/DataTree/interface/EventHeaderCol.h"
#include "MitAna/DataTree/interface/MCEventInfo.h"
#include "MitAna/DataTree/interface/PileupInfoFwd.h"

class TTree;
class TH1D;
class TH3D;
class TStopwatch;

namespace mithep 
{
  class AnaFwkMod : public BaseMod
  {
    public:
      AnaFwkMod(const char *name="AnaFwkMod", 
                const char *title="Analysis framework module");

      UInt_t           GetCheckMC()               const { return fCheckMC;     }
      Long64_t         GetPrintScale()            const { return fPrintScale;  }
      Long64_t         GetSkipNEvents()           const { return fSkipNEvents; }
      void             SetCheckMC(UInt_t n)             { fCheckMC = n;        }
      void             SetPrintScale(UInt_t n)          { fPrintScale  = n;    }
      void             SetSkipNEvents(Long64_t n)       { fSkipNEvents = n;    }

    protected:
      void             BeginRun();
      Bool_t           Notify();
      void             CopyAllEventHeaders();
      void             Process();
      void             SlaveBegin();
      void             SlaveTerminate();

      UInt_t           fCheckMC;           //=0 no data/MC check, =1 abort if not MC, =2 warn if not MC
      TString          fAllHeadTreeName;   //all events tree name
      TString          fAllHeadBrName;     //all event headers branch name
      Long64_t         fSkipNEvents;       //number of events to skip from beginning (def=0)
      UInt_t           fPrintScale;        //scale of when to print event number/timings (def=100)
      TStopwatch      *fSWtotal;           //!stop watch for overall timing
      TStopwatch      *fSWevent;           //!stop watch per n events
      EventHeaderArr   fAllHeaders;        //!pointer to array of all event headers
      TTree           *fAllHeadTree;       //!all event headers tree in current file
      EventHeader     *fAllEventHeader;    //!all event header
      Bool_t           fReload;            //!reload tree after file changed
      Int_t            fCurEnt;            //!curent entry in AllEvents tree
      Int_t            fNEventsSkimmed;    //!number of skimmed events
      Int_t            fNEventsSkipped;    //!number of skippeded events
      const PileupInfoCol *fPileupInfo;    //!      
      TString          fPileupInfoName;    //Name of PileupInfo Branch for pileup truth distributions
      Bool_t           fDoPUInfo;
      TH1D             *hNPU;              //in-time pileup distribution
      TH3D             *hNPU50ns;          //three-dimensional pileup distribution (bx=0,-2,+2) suitable for 50ns spacing
      TH1D             *hNPUTrue;          //true pileup distribution
      const MCEventInfo *fMCEventInfo;     //!MC Event Info pointer
      TString          fMCEventInfoName;   //name of MC Event info branch
      TH1D             *hDTotalMCWeight;   //total monte carlo weight from mc event header

    ClassDef(AnaFwkMod, 1) // Analysis framework module
  };
}
#endif
