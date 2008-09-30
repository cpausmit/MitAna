// $Id: runSimpleExample.C,v 1.5 2008/06/09 00:06:36 bendavid Exp $

#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TROOT.h>
#include "MitAna/DataUtil/interface/Debug.h"
#include "MitAna/TreeMod/interface/Analysis.h"
#include "MitAna/TreeMod/interface/HLTMod.h"
#include "MitAna/TreeMod/interface/HLTExampleMod.h"
#endif

//--------------------------------------------------------------------------------------------------
void runHLTExample(const char *trigger="HLT_Jet80", const char *files = "mit-full_000.root")
{
  using namespace mithep;
  gDebugMask  = Debug::kAnalysis;
  gDebugLevel = 1;

  HLTMod *hltmod = new HLTMod;
  hltmod->AddTrigger(trigger);
  hltmod->SetTrigObjsName("myhltobjs");

  HLTExampleMod *hltexmod = new HLTExampleMod;
  hltexmod->SetTrigObjsName("myhltobjs");
  hltmod->Add(hltexmod);
  
  // set up analysis
  Analysis *ana = new Analysis;
  ana->SetSuperModule(hltmod);
  ana->AddFile(files);
  if (gROOT->IsBatch())
    ana->SetOutputName("mit-hlt-hist.root");

  // run the analysis after successful initialisation
  ana->Run(!gROOT->IsBatch());

  cout << "HLTMod: Accepted " << hltmod->GetNAccepted() 
       << " of " << hltmod->GetNEvents() << " events" << endl;
}