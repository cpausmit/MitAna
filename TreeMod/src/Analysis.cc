// $Id: Analysis.cc,v 1.1 2008/05/27 19:50:16 loizides Exp $

#include "MitAna/TreeMod/interface/Analysis.h"
#include <Riostream.h>
#include <TFile.h>
#include <TList.h>
#include <TDSet.h>
#include <TChain.h>
#include <TObjString.h>
#include <TSystem.h>
#include <TProof.h>
#include <TROOT.h>
#include "MitAna/DataUtil/interface/Debug.h"
#include "MitAna/DataTree/interface/Names.h"
#include "MitAna/TAM/interface/TAMVirtualLoader.h"
#include "MitAna/TAM/interface/TAModule.h"
#include "MitAna/TAM/interface/TAMSelector.h"

ClassImp(mithep::Analysis)

using namespace mithep;

//__________________________________________________________________________________________________
Analysis::Analysis(Bool_t up) :
   fUseProof(up), 
   fHierachy(kTRUE), 
   fState(kPristine), 
   fNFriends(0), 
   fList(new TList), 
   fOutput(0), 
   fPackages(new TList), 
   fLoaders(new TList),
   fSuperMod(0), 
   fSelector(0), 
   fChain(0), 
   fSet(0), 
   fDeleteList(new TList),
   fTreeName(Names::mittree),
   fCompLevel(2), 
   fProof(0)
{
   // Default constructor.

   fList->SetOwner();
   fDeleteList->SetOwner();

   /* default packages for PROOF upload */
   fPackages->SetOwner();
   // nothing to be done since we do not use par files (yet?)
}

//__________________________________________________________________________________________________
Analysis::~Analysis()
{
   // Destructor.

   if (fState == kInit || fState == kRun)
      Terminate();

   delete fList;
   delete fDeleteList;
   delete fLoaders;
   delete fSelector;
   delete fPackages;
   fOutput   = 0;   // owned by TAM
   fSuperMod = 0;   // owned by user

   delete fProof;
}

//__________________________________________________________________________________________________
Bool_t  Analysis::AddFile(const char *pname)
{
   // Add file with given name to the list of files to be processed. 
   // Using the token "|", you can specify an arbritray number
   // of paths to tree files that will be concatenated as friend 
   // trees.

   if (fState != kPristine) {
      Error("AddFile", "Analysis already initialized");
      return kFALSE;
   }

   TString pnamestr(pname);
   TString tok("|");
   TObjArray *arr = pnamestr.Tokenize(tok);
   TString msg;

   for(Int_t i=0; i<arr->GetEntries(); i++){
      
      TObjString *dummy = dynamic_cast<TObjString*>(arr->At(i));
      if(!dummy) continue;

      AddFile(dummy->GetName(),i);      
      if(i==0) msg=dummy->GetName();
      else {
         Info("AddFile", "Add file %s as friend to %s", 
              dummy->GetName(), msg.Data());
      }
   }
   delete arr;

   return kTRUE;
}

//________________________________________________________________________
void Analysis::AddFile(const char *pname, Int_t eventlist)
{
   // Add file name to the event list specified by eventlist. The lists
   // are used to hold filenames of different types of events. In case
   // you dont want friend trees, just give no eventlist argument (default 0).

   MitAssert("AddFile", pname != 0);

   TList *l = 0;
   if (eventlist >= 0 && eventlist < fNFriends) {
      
      l = dynamic_cast<TList*>(fList->At(eventlist));
      if (!l) {
         Fatal("AddFile", "Requested list %d not found!", eventlist);
         return;
      }

   } else if (eventlist == fNFriends) {
      
      l = new TList;
      l->SetOwner();
      fList->Add(l);
      fNFriends++;

   } else if (eventlist < 0 || eventlist > fNFriends) {
      Error("AddFile", "Specified list %d not in [0,%d]", eventlist, fNFriends);
      return;
   }
   
   if(!IsValidName(pname)) return;

   l->Add(new TObjString(pname));

   MDB(kAnalysis, 2)
      Info("AddFile", "Added %s to list of files.", pname);
}

//__________________________________________________________________________________________________
void Analysis::AddFile(const TObject *oname, Int_t eventlist)
{
   // Add file name to the event list specified by eventlist. The lists
   // are used to hold filenames of  different types of events. In case
   // you dont want mixing, just give no eventlist argument (default 0).

   MitAssert("AddFile", oname != 0);

   return AddFile(oname->GetName(), eventlist);
}

//__________________________________________________________________________________________________
void Analysis::AddList(TList *list, Int_t eventlist)
{
   // Add file name to the event list specified by eventlist. The lists
   // are used to hold filenames of  different types of events. In case
   // you dont want mixing, just give no eventlist argument (default 0).

   MitAssert("AddList", list != 0);

   TIter next(list);                           
   while (TObject *obj = next())                                
      AddFile(obj->GetName(), eventlist);
}

//__________________________________________________________________________________________________
void Analysis::AddLoader(TAMVirtualLoader *l)      
{ 
   // Add loader to the list of loaders.

   fLoaders->Add(l); 
}

//__________________________________________________________________________________________________
void Analysis::AddPackage(const char* name)
{
   // Add package to the list of uploaded packages.

   MitAssert("AddPackage", name != 0);
   fPackages->Add(new TObjString(name));
}

//__________________________________________________________________________________________________
void Analysis::AddPackages(TList *list)
{
   // Add list of packages to the list of uploaded packages.

   MitAssert("AddPackage", list != 0);

   TIter next(list);
   while (TObject *obj = next()) {
      fPackages->Add(new TObjString(obj->GetName()));
   }
}

//__________________________________________________________________________________________________
Bool_t Analysis::Init()
{
   // Setup the TDSet and TChain to be used for the analysis
   // with or without PROOF. If more than one list of 
   // file names was given, friend trees are supported.

   if (fState == kRun || fState == kInit) {
      Error("Init", "Init in state %d is not possible! Call Terminate() first.",
            Int_t(fState));
      return kFALSE;
   }

   if (fNFriends <= 0) {
      Error("Init", "List of friend lists is empty!");
      return kFALSE;
   }

   if (!fSuperMod) {
      Error("Init", "Top-level TAM module is NULL!");
      return kFALSE;
   }

   if (fUseProof) { // first init our PROOF session
      if (!InitProof()) return kFALSE;
   }

   // we do this here instead in Terminate() so that 
   // we can browse the output even after Terminate()
   delete fSelector;
   fSelector = 0;

   fChain = new TChain(fTreeName); 
   fSet   = new TDSet("TTree",fTreeName);

   for(Int_t i=0; i<fNFriends; i++){

      TList *l = dynamic_cast<TList*>(fList->At(i));
      if (!l) {
         Fatal("Init", "List %d not found!", i);
         return kFALSE;
      }

      if (i == 0) {

         TIter next(l);                           
         while ( TObjString *obj = dynamic_cast<TObjString*>(next()) ) {
            fChain->Add(obj->GetName());
            fSet->Add(obj->GetName());
         }

      } else {

         TChain *chain = new TChain(fTreeName); 
         TDSet *set    = new TDSet("TTree",fTreeName);

         TIter next(l);                           
         while (TObjString *obj = dynamic_cast<TObjString*>(next())) {
            chain->Add(obj->GetName());
            set->Add(obj->GetName());
         }

         TString alias("TAMTREE_"); // aliases currently not used
         alias+=i;

         fChain->AddFriend(chain,alias.Data());
         fSet->AddFriend(set,alias.Data());

         fDeleteList->Add(chain);
         fDeleteList->Add(set);
      }

   }

   // if we had our default TAM plugin we would create it here
   //  TreeLoader *bl = new TreeLoader;
   //  fLoaders->Add(bl);
   //  fDeleteList->Add(bl);

   if (fUseProof) {

      fProof->AddInput(fSuperMod);
      fLoaders->SetName("TAM_LOADERS");
      fProof->AddInput(fLoaders);      

   } else {

      // when not running Proof, we must make a selector
      fSelector = new TAMSelector; 
      fSelector->AddInput(fSuperMod);
      MDB(kAnalysis, 2)
         fSelector->SetVerbosity(1);

      // pass loaders to selector
      TIter next(fLoaders);                           
      while ( TAMVirtualLoader *l = dynamic_cast<TAMVirtualLoader*>(next()) )
         fSelector->AddLoader(l);
   }

   fState = kInit;
   return kTRUE;
}

//__________________________________________________________________________________________________
Bool_t Analysis::InitProof()
{
   // Initialize PROOF connection.

   if(fProof && fProof->IsValid()) 
      return kTRUE;

   delete fProof;

   if (fMaster.Contains("rcf.bnl.gov")) {
      for(Int_t i=0;i<5;i++) {
         Warning("InitProof", "*** DID YOU RUN PROOF_KINIT? %d (5) ***", i);
	    gSystem->Sleep(1000);
      }
   }

   MDB(kAnalysis, 1)
      Info("InitProof", "Starting PROOF on master %s with config %s",
           fMaster.Data(), fConfig.Data());

   fProof = dynamic_cast<TProof*>(TProof::Open(fMaster, fConfig));
   if (!fProof) {
      Error("InitProof", "Could not start PROOF!");
      return kFALSE;
   }

   MDB(kAnalysis, 3)
      gROOT->Print();

   //fProof->AddInput(new TNamed("PROOF_NewPacketizer",""));

   Bool_t ret=kTRUE;
   if (fPackages) {
      // tell Proof what additional libraries we will need on each worker computer
      ret = UploadPackages(fPackages);
   }

   return ret;
}

//__________________________________________________________________________________________________
void Analysis::Run()
{
   // Run the analysis on the created file set.

   if (fState == kPristine || fState == kRun) {
      Error("Run", "Run in state %d is not possible! Call Init() first.",
            Int_t(fState));
   }

   if (fUseProof) {

      MDB(kAnalysis, 1)
         Info("Run", "Start processing with PROOF...");

      fSet->Process("TAMSelector");

   } else {

      MDB(kAnalysis, 1)
         Info("Run", "Start processing (no PROOF)...");

      fChain->Process(fSelector);
   }

   MDB(kAnalysis, 1)
      Info("Run", "Processing complete!");

   fState = kRun;
}

//__________________________________________________________________________________________________
void Analysis::Terminate()
{
   // Terminate current analysis run.

   if (fState == kPristine || fState == kTerminate) {
      Error("Terminate", "Terminate in state %d is not possible! Call Init() first.",
            Int_t(fState));
      return;
   }

   if (fState == kRun) {

      if (fUseProof) {
         // the output list from Proof can (in principal) contain other objects
         // besides the module output hierarchy.
         TList* outputlist = fProof->GetOutputList();
         TIter nextOut(outputlist);
         while (TObject *obj = nextOut()) {
            if (obj->InheritsFrom(TAMOutput::Class())) {
               fOutput = dynamic_cast<TList*>(obj);
               break;
            }
         }

      } else {
         fOutput = fSelector->GetModOutput();
      }

      if (fOutput && !fAnaOutput.IsNull()) {
         TDirectory::TContext context(0); // automatically restore gDirectory

         std::auto_ptr<TFile> outf(TFile::Open(fAnaOutput,"recreate","", fCompLevel));
         if (outf.get() == 0) {
            Error("Terminate", "Could not open file %s for output!", fAnaOutput.Data());
         } else {
            MDB(kAnalysis, 1)
               Info("Terminate", "Saving output to %s!", fAnaOutput.Data());

            if (fHierachy) 
               fOutput->Write(fOutput->GetName(),TObject::kSingleKey);       
            else 
               fOutput->Write();
               
         }
      }
   }

   delete fChain;
   delete fSet;
   fDeleteList->Delete();

   fState = kTerminate;
}

//__________________________________________________________________________________________________
Bool_t Analysis::UploadPackages(TList *packages)
{
   // Upload list of par files to the server.

   MitAssert("UploadPackages", packages != 0);

   for(Int_t i=0; i < packages->GetEntries(); i++) {

      TObject* objstr = packages->At(i); 
      if (!objstr){
         Error("InitProof", "Problem at package number %d!", i);
         return kFALSE;
      }

      TString packname = objstr->GetName();
      Int_t en = 0;
      if (packname.EndsWith("+")) { 
         en=1;
         packname.Resize(packname.Length()-1);
      }

      ifstream ftest(gSystem->ExpandPathName(packname.Data()),ios_base::binary);
      if (!ftest.good()){
         Error("InitProof", "Could not open %s for upload!", packname.Data());
         return kFALSE;
      }

      if(fProof->UploadPackage(packname)<0) {
         Error("UploadPackage", "Upload for %s failed!", packname.Data());
         return kFALSE;
      }

      if (en == 1) {
         Int_t pos=packname.Last('/')+1;
         if (pos) packname.Remove(0,pos);
         if(fProof->EnablePackage(packname)<0) {
            Error("UploadPackage", "Enabling for %s failed!", packname.Data());
            return kFALSE;
         }
      }
   }

   return kTRUE;
}