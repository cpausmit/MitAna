//--------------------------------------------------------------------------------------------------
// Cacher
//
// Class implementing a utility to keep track of the files needed by the analysis and caching them
// in advance and in parallel using SmartCache infrastructure or the AAA system.
//
// WARNING - bambu supports reading several files in parallel which we do not support here. We will
//           pretend that there is only one filelist. In principle this can be added.
//
// Authors: C.Paus
//--------------------------------------------------------------------------------------------------
#ifndef MITANA_DATAUTIL_CACHER_H
#define MITANA_DATAUTIL_CACHER_H

#include <vector>
#include <TList.h>

namespace mithep 
{
  class Cacher
  {
  public:
    Cacher(const TList *list, Bool_t fullLocal);
    virtual ~Cacher();
    
    Bool_t               InitialCaching();
    Bool_t               NextCaching();
    Bool_t               Exists(const char* file) const;
    Bool_t               NextFileReady() const;
    void                 CleanCache() const;
    void                 Wait();
    void                 SetNFilesAhead(Int_t n) { fNFilesAhead = n; }
    
  protected:
    
  private:
    Bool_t               SubmitCacheRequest(const char* file, Bool_t initial);
    void                 RemoveTemporaryFile(int idx);

    TList               *fInputList;          //in bambu several input files can be handled in
                                              //parallel we do not (yet) implement this here
    Bool_t               fFullLocal;          //when true, call cache request command with special options
    Int_t                fCurrentFileIdx;     //index of currently processing file
    Int_t                fCachedFileIdx;      //index of last cached file
    Int_t                fNFilesAhead;        //number of files to cache ahead of running job
    Int_t                fNSecWait;           //keep track how many seconds we were waiting
    std::vector<int>     fCacheStatus;        //0-nothing, 1-submitted, 2-complete
  };
}
#endif
