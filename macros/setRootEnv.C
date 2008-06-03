// $Id: setRootEnv.C,v 1.1 2008/05/27 19:59:54 loizides Exp $

#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TEnv.h>
#include <TString.h>
#include <TSystem.h>
#include <TRegexp.h>
#include <TROOT.h>
#include <TFile.h>
#include <TClass.h>
#include <TClassTable.h>
#include <TInterpreter.h>
#include <TRandom3.h>
#include <TError.h>
#endif

void setIncludes();
void loadLibraries();

void setRootEnv()
{
  // automatically restore gDirectory
  TDirectory::TContext context(0);

  // set a better default random generator
  delete gRandom;
  gRandom = new TRandom3;
  gRandom->SetSeed(0);

  // customizing the MIT analysis framework things
  setIncludes();
  loadLibraries();

  if (gClassTable->GetID("mithep::Particle") >= 0) {
    ::Info("setRootEnv", "Welcome to MITROOT!\n");
  }
}

void setIncludes()
{
  gSystem->AddIncludePath("-I$CMSSW_BASE/src/");
  gSystem->AddIncludePath("-I$CMSSW_BASE/src/MitAna/TreeMod/inc");
  gSystem->AddIncludePath("-I$CMSSW_BASE/src/MitAna/macros");
  gInterpreter->AddIncludePath(TString(gSystem->Getenv("CMSSW_BASE"))+"/src/");
  gInterpreter->AddIncludePath(TString(gSystem->Getenv("CMSSW_BASE"))+"/src/MitAna/TreeMod/inc");
  gInterpreter->AddIncludePath(TString(gSystem->Getenv("CMSSW_BASE"))+"/src/MitAna/macros");
  gROOT->SetMacroPath(TString(gROOT->GetMacroPath())
                      +TString(gSystem->Getenv("CMSSW_BASE"))+"/src/MitAna/macros");
}

void loadLibraries()
{
  TString libstr(Form("%s/lib/%s",gSystem->Getenv("CMSSW_BASE"),gSystem->Getenv("SCRAM_ARCH")));

  void *dir = gSystem->OpenDirectory(libstr.Data());
  TRegexp re("libMitAna*.so", kTRUE);
  TRegexp reignore("libMitAnalysis*.so", kTRUE);
  while (const char *direntry=gSystem->GetDirEntry(dir) ) {
    TString sdirentry(direntry);
    if (sdirentry.Index(re) == kNPOS)
      continue;
    if (sdirentry.Index(reignore) != kNPOS)
      continue;
    Int_t len = strlen(direntry)-3;
    if (len<=0)
      continue;
    char *tmpstr = new char[len+1];
    for (Int_t i=0;i<len;i++)
      tmpstr[i]=direntry[i];
    tmpstr[len]='\0';
    if (gInterpreter->IsLoaded(tmpstr)) {
      if (gDebug)
        Warning("setRootEnv","Trying to load \"%s\", but it is already loaded", tmpstr);
    } else {
      if (gSystem->Load(tmpstr)<0) {
        gROOT->Error("setRootEnv", "could not load \"%s\" for use in ACLiC", tmpstr);
      } else {
        if (gDebug)
          Info("setRootEnv","Loaded \"%s\" and its dependencies", tmpstr);
      }
    }
    delete[] tmpstr;
  }
  gSystem->FreeDirectory(dir);
}