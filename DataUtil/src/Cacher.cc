#include <TSystem.h>
#include <TObjString.h>
#include "MitAna/DataUtil/interface/Debug.h"
#include "MitAna/DataUtil/interface/Cacher.h"

using namespace std;
using namespace mithep;

//--------------------------------------------------------------------------------------------------
Cacher::Cacher(const TList *list, Bool_t fullLocal) :
  fInputList(),
  fFullLocal(fullLocal),
  fCurrentFileIdx(-1),
  fCachedFileIdx(-1),
  fNFilesAhead(2),
  fCumulativeWait(0),
  fCurrentWait(0),
  fWaitLength(10),
  fTimeout(1800)
{
  // Constructor
  fInputList.SetOwner();
  // deep clone the input list
  for (TObject* objStr : *list)
    fInputList.Add(new TObjString(objStr->GetName()));

  // create the synchronized cache status vector
  for (Int_t i=0; i<fInputList.GetEntries(); i++) {
    int status = 0;
    fCacheStatus.push_back(status);
  }
}

//--------------------------------------------------------------------------------------------------
Bool_t Cacher::InitialCaching()
{
  // Caching to get the job going. This needs to be done before Run (maybe in SlaveBegin?)

  Bool_t status = kTRUE;
  for (Int_t i=0; i<fInputList.GetEntries(); i++) {
    Info("Cacher::InitialCaching","cache file: %s",fInputList.At(i)->GetName());
    // here we need to submit the caching request
    status = (status && SubmitCacheRequest(fInputList.At(i)->GetName(), true));
    // keep track of the book keeping
    fCachedFileIdx++;
    fCacheStatus[i] = 1;
    if (i>=fNFilesAhead-1)
      break;
  }

  // Need to wait for download completion of first two files to get going
  fCurrentWait = 0;
  while (kTRUE) {
    Bool_t complete = kTRUE;
    for (Int_t i=0; i<min(fNFilesAhead,fInputList.GetEntries()); i++) {
      if (Exists(fInputList.At(i)->GetName()))
	fCacheStatus[i] = 2;
      else
	complete = kFALSE;
    }
    if (complete) {
      MDB(kTreeIO, 1)
	Info("Cacher::InitialCaching","completed initial caching\n");
      break;
    }
    MDB(kTreeIO, 2)
      Info("Cacher::InitialCaching","waiting for completion (%d sec)", fWaitLength);

    Wait();
    if (fCurrentWait > fTimeout) {
      Warning("Cacher::InitialCaching", "Initial caching timed out after %d seconds.", fTimeout);
      return false;
    }
  }

  return status;
}

//--------------------------------------------------------------------------------------------------
Bool_t Cacher::NextCaching()
{
  // Caching to be triggered after the job already started, checks and waits for completion of the
  // next-to-next file needed and submits the next caching request.

  // remove file that was just completed
  RemoveTemporaryFile(fCurrentFileIdx);
  // keep track of which file is being worked on
  fCurrentFileIdx++;

  // Start with a good completion
  Bool_t status = kTRUE;

  // Submit the next caching request first
  fCachedFileIdx++;
  if (fCachedFileIdx<fInputList.GetEntries()) {
    Info("Cacher::NextCaching","cache file: %s",fInputList.At(fCachedFileIdx)->GetName());
    status = SubmitCacheRequest(fInputList.At(fCachedFileIdx)->GetName(), false);
    fCacheStatus[fCachedFileIdx] = 1;
  }
  else {
    MDB(kTreeIO, 2)
      Info("Cacher::NextCaching","no more files to cache");
  }

  // Next: wait for download completion of last requested file (needs to be available to the job)
  fCurrentWait = 0;
  while (kTRUE) {
    // make sure we are not asking for too much ;-)
    if (fCachedFileIdx-1 >= fInputList.GetEntries())
      break;

    // check download completion
    if (Exists(fInputList.At(fCachedFileIdx-1)->GetName())) {
      fCacheStatus[fCachedFileIdx-1] = 2;
      MDB(kTreeIO, 2)
	Info("Cacher::NextCaching","completed");
      break;
    }

    MDB(kTreeIO, 2)
      Info("Cacher::NextCaching","waiting for completion (%d sec)", fWaitLength);

    Wait();
    if (fCurrentWait > fTimeout) {
      Warning("Cacher::NextCaching", "Cache wait timed out after %d seconds.", fTimeout);
      return false;
    }
  }

  return status;
}

//--------------------------------------------------------------------------------------------------
Bool_t Cacher::WaitForNextFile()
{
  if (fCurrentFileIdx == fInputList.GetEntries() - 1)
    return true;

  char const* fileName = fInputList.At(fCurrentFileIdx + 1)->GetName();
  fCurrentWait = 0;
  while (!Exists(fileName)) {
    Wait();
    if (fCurrentWait > fTimeout) {
      Warning("Cacher::WaitForNextFile", "Next file did not appear after %d seconds.", fTimeout);
      return false;
    }
  }

  return true;
}

//--------------------------------------------------------------------------------------------------
Bool_t Cacher::SubmitCacheRequest(const char* file, Bool_t initial) const
{
  // Submit a Cache request for the specified file

  MDB(kTreeIO, 1)
    Info("Cacher::SubmitCacheRequest","request file: %s",file);

  TString cmd = TString(gSystem->Getenv("CMSSW_BASE"))+TString("/src/MitAna/bin/requestFile.sh ")
               +TString(file);

  if (fFullLocal) {
    // if (initial)
    //   cmd += " -L"; // make a symbolic link under ./store/...
    // else
    //   cmd += " -C"; // copy the file over to ./store/...
    cmd += " -C";
  }

  // Execute the system command
  int rc = gSystem->Exec(cmd.Data());
  
  return (rc == 0);
}

//--------------------------------------------------------------------------------------------------
Bool_t Cacher::Exists(const char* file) const
{
  // Check if the specified file exists

  Long_t *id=0,*size=0,*flags=0,*mt=0;

  MDB(kTreeIO, 2)
    Info("Cacher::Exists","test file: %s (test returns %d)",file,
	 gSystem->GetPathInfo(file,id,size,flags,mt));

  return (gSystem->GetPathInfo(file,id,size,flags,mt) == 0);
}

//--------------------------------------------------------------------------------------------------
void Cacher::RemoveTemporaryFile(int idx) const
{
  // Remove completed file if it was a temporary download
  if (idx > -1 && idx < fInputList.GetEntries()) {
    TString fileName = fInputList.At(idx)->GetName();
    if (fileName.BeginsWith("./")) {
      Info("Cacher::RemoveTemporaryFile","remove: %s",fileName.Data());
      gSystem->Exec((TString("rm -f ")+fileName).Data());
    }
  }
  return;
}

//--------------------------------------------------------------------------------------------------
void Cacher::CleanCache() const
{
  // there is nothing really do delete in terms of objects but the potential local file
  // copies need to be removed
  for (Int_t i=0; i<fInputList.GetEntries(); i++) {
    MDB(kTreeIO, 1)
      Info("Cacher::CleanCache","check whether to remove leftovers %s",fInputList.At(i)->GetName());
    RemoveTemporaryFile(i);
  }
  // Give a summary of the cache waiting time
  Info("Cacher::CleanCache","\n         total waiting time for caching %d sec (%f min)\n\n",
       fCumulativeWait,fCumulativeWait/60.);
}

//--------------------------------------------------------------------------------------------------
void Cacher::Wait()
{
  fCurrentWait += fWaitLength;
  fCumulativeWait += fWaitLength;
  sleep(fWaitLength); // wait 10 seconds
}
