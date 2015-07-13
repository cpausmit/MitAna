#include <iostream>
#include <TROOT.h>
#include <TSystem.h>
#include <TFile.h>
#include <TTree.h>
#include <TKey.h>
#include <TH1D.h>

const TString slash      = "/";
const TString dCacheDoor = "dcap://t2srv0012.cmsaf.mit.edu/";
//const TString dCacheDoor = "dcap://t2srv0005.cmsaf.mit.edu/";
const TString hadoopDoor = "root://xrootd.cmsaf.mit.edu/";

void   catalogFile(const char *dir, const char *file);
void   reset();
TFile *openFile(TString fileName); 

using namespace std;

//--------------------------------------------------------------------------------------------------
void runSimpleFileCataloger(const char *dir  = "/mnt/hadoop/cms/store/user/paus/fabstoec/Summer11Private/TTH_HToGG_M-120_TuneZ2_7TeV-pythia6/Summer11-PU32_8CM_START42_V13C-v4/GEN-SIM-RECO",
			    const char *file = "")
{
  // -----------------------------------------------------------------------------------------------
  // This script runs a full cataloging action on the given directory
  // -----------------------------------------------------------------------------------------------
  reset();
  catalogFile(dir,file);
  return;
}

//--------------------------------------------------------------------------------------------------
void catalogFile(const char *dir, const char *file)
{
  TString fileName = TString(dir) + slash +  + TString(file);

  // Try open this file
  TFile *f = openFile(fileName);
  if (! f) {
    cout << " ERROR -- file could not be opened: " << fileName << endl;
    return;
  }    

  // Deal with Delphes files first
  TTree* tree = (TTree*) f->FindObjectAny("Delphes");
  if (tree) {
    cout << "0000 " << fileName
	 << " " << tree->GetEntries()  << " " << tree->GetEntries() << endl;
    return;
  }

  // Unskimmed files
  Bool_t done = kFALSE;
  tree    = (TTree*) f->FindObjectAny("Events");
  if (tree) {
    cout << "XX-CATALOG-XX " << fileName << " "
	 << tree->GetEntries() << endl;
    done = kTRUE;
  }

  // Skimmed files have the AllEvents tree
  TTree* allTree = (TTree*) f->FindObjectAny("AllEvents");
  if (tree && allTree) {
    cout << "XX-CATALOG-XX " << fileName << " "
	 << tree->GetEntries() << " " << allTree->GetEntries() << endl;
    done = kTRUE;
  }
  
  // Finally if nothing works try to find out histogram
  if (!done) {
    TString hname="hDEvents";
    f->cd("AnaFwkMod");
    TH1D *h = (TH1D*) f->FindObjectAny(hname);
    if (h)
      cout << "XX-CATALOG-XX " << fileName
	   << " " << h->GetEntries() << " " << h->GetEntries() << endl;
  }

}

//--------------------------------------------------------------------------------------------------
void reset()
{
}

//--------------------------------------------------------------------------------------------------
TFile *openFile(TString fileName)
{
  // First try simply to open the given file name
  TFile* f = TFile::Open(fileName.Data());
  if (f)
    return f;

  // Looks like this is not a local file
  if (fileName.Index("castor/cern.ch") != -1)
    fileName = TString("castor:") + fileName;
  if (fileName.Index("pnfs/cmsaf.mit.edu") != -1)
    fileName = dCacheDoor + fileName;
  if (fileName.Index("/mnt/hadoop/cms/store") == 0) {
    fileName.Remove(0,15);
    fileName = hadoopDoor + fileName;
  }

  f = TFile::Open(fileName.Data());

  return f;
}
