#include <TSystem.h>
#include "MitCommon/Utils/interface/Utils.h"
#include "MitAna/Catalog/interface/Catalog.h"
#include "MitAna/DataUtil/interface/Debug.h"
#include "MitAna/Catalog/interface/Dataset.h"

ClassImp(mithep::Catalog)

using namespace mithep;

//--------------------------------------------------------------------------------------------------
Catalog::Catalog(const char *location) :
  fLocation(location)
{
  // Constructor
}

//--------------------------------------------------------------------------------------------------
Dataset *Catalog::FindDataset(const char *book, const char *dataset, const char *fileset,
                              int local) const
{
  // Try to find the given dataset in the catalog and return it properly filled.
  // Note that the caller must delete the returned dataset.
  //
  // local =0 - means we do not touch the file names and take them at face value
  //       =1 - translate them into local files, and cache all the beginning of the job
  //       =2 - translate them into local files, no caching wait for Cacher mechanism
  //       =3 - translate them into local files under PWD

  printf(" Catalog: %s, Book: %s, Dataset: %s, Fileset: %s\n",fLocation.Data(),book,dataset,fileset);

  TString slash        = "/";
  TString fullDir      = fLocation +slash+ TString(book) +slash+ TString(dataset);
  TString cmdFilesets  = TString("cat ")+fullDir+slash+TString("Filesets | grep -v ^#");
  TString cmdFiles     = TString("cat ")+fullDir+slash+TString("Files    | grep -v ^#");

  if (!TString(fileset).IsNull()) {
    cmdFilesets += TString(" | grep ^") + TString(fileset);
    cmdFiles    += TString(" | grep ^") + TString(fileset);
  }

  Bool_t  cache = kFALSE;
  char    file[1024], fset[1024], location[1024];
  UInt_t  nAllEvents=0, nEvents=0, nLumiSecs=0;
  UInt_t  nMaxRun=0, nMaxLumiSecMaxRun=0, nMinRun=0, nMinLumiSecMinRun=0;
  FILE   *fHandle=0;

  Dataset *ds = new Dataset(dataset);

  // Determine domainname (on MIT Tier-3 and MIT Tier-2 we can use local files)
  TString domainName = Utils::DomainName();
  Bool_t atMIT = (domainName == TString("mit.edu") || domainName == TString("cmsaf.mit.edu"));

  // Read the locations and parameters of the different filesets
  fHandle = gSystem->OpenPipe(cmdFilesets.Data(),"r");
  while (fscanf(fHandle,"%s %s %u %u %u %u %u %u",fset,location,
                &nAllEvents,&nEvents,//&nLumiSecs,
                &nMaxRun,&nMaxLumiSecMaxRun,&nMinRun,&nMinLumiSecMinRun)
         != EOF) {
    MDB(kGeneral,1)
      printf(" --> %s %s %u %u %u %u %u %u\n",fset,location,
             nAllEvents,nEvents,//nLumiSecs,
             nMaxRun,nMaxLumiSecMaxRun,nMinRun,nMinLumiSecMinRun);
    TString dir = TString(location);
    // careful: 0- no touching, 1- replace, no fileset caching, 2- replace, fileset caching
    if (local > 0) {
      TString tmp(dir);

      if (local < 3 && atMIT)
        dir.ReplaceAll("root://xrootd.cmsaf.mit.edu//", "/mnt/hadoop/cms/");
      else if (local == 3)
        dir.ReplaceAll("root://xrootd.cmsaf.mit.edu//", "./");

      // Test if files are requested to be cached
      if (dir != tmp && local == 2 && atMIT)
        cache = kTRUE;
    }
    FilesetMetaData *fs = new FilesetMetaData(fset,dir.Data());
    ds->AddFileset(fs);
    delete fs;
  }
  gSystem->ClosePipe(fHandle);

  // Read the parameters for each file
  fHandle = gSystem->OpenPipe(cmdFiles.Data(),"r");
  while (fscanf(fHandle,"%s %s %u %u %u %u %u %u",fset,file,
                &nAllEvents,&nEvents,//&nLumiSecs,
                &nMaxRun,&nMaxLumiSecMaxRun,&nMinRun,&nMinLumiSecMinRun)
         != EOF) {
    MDB(kGeneral,1)
      printf(" --> %s %s %u %u %u %u %u %u\n",fset,file,
             nAllEvents,nEvents,//nLumiSecs,
             nMaxRun,nMaxLumiSecMaxRun,nMinRun,nMinLumiSecMinRun);
    BaseMetaData  b(nAllEvents,nEvents,nLumiSecs,
                    nMaxRun,nMaxLumiSecMaxRun,nMinRun,nMinLumiSecMinRun);
    FileMetaData *f = new FileMetaData(file,&b);
    ds->AddFile(fset,f);
    delete f;
  }
  gSystem->ClosePipe(fHandle);

  // If files were at Tier-2: cache them
  if (cache) {
    if (CacheFileset(book, dataset, fileset))
      printf(" Catalog::FindDataset() -- Caching successfully completed.\n");
    else
      printf(" Catalog::FindDataset() - ERROR - Caching failed.\n");
  }

  return ds;
}

//--------------------------------------------------------------------------------------------------
Bool_t Catalog::CacheFileset(const char *book, const char *dataset, const char *fileset) const
{
  // Form system command (script will make necessary adjustment for catalog/book)
  TString space(" ");
  TString cmd = TString(gSystem->Getenv("CMSSW_BASE"))+TString("/src/MitAna/bin/cacheFileset.sh ")+
                fLocation+space+TString(book)+space+TString(dataset)+
                TString(" noskim ")+TString(fileset);
  printf(" Cache: %s\n",cmd.Data());

  // Execute the system command
  int rc = gSystem->Exec(cmd.Data());

  return (rc == 0);
}
