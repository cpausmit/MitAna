// $Id: TreeWriter.h,v 1.2 2008/05/27 19:59:54 loizides Exp $

#ifndef DATATREE_TREEWRITER_H
#define DATATREE_TREEWRITER_H

#include <TNamed.h>
#include <TString.h>
#include <TFile.h>
#include <TTree.h>
#include <TObjArray.h>

//--------------------------------------------------------------------------------------------------
//
// TreeWriter
//
// Class implementing a tree writing infra structure. The class creates and maintaines
// a standard tree with name "tname" as given in the constructor. The tree is stored in 
// a file or in multiple files if needed, where the file path name and maximum size can be
// specified. Using TreeWriter::AddBranch allows to add various branches to the tree.
// The user has to make sure that (s)he calls TreeWriter::BeginEvent and 
// TreeWriter::EndEvent for every event before and after filling of the branch 
// structures. Note that in addition to the standard tree one can create new
// tree(s) in the same file using the TreeWriter::AddBranchToTree member functions.
//
// Authors: C.Loizides
//
//--------------------------------------------------------------------------------------------------

namespace mithep 
{
  class MyTree : public TTree 
  {
    public:
      MyTree(const char* name, const char* title, Int_t splitlevel = 99) :
         TTree(name,title, splitlevel), fAutoFill(1) {}
      Bool_t GetAutoFill() const   { return fAutoFill; }
      void   SetAutoFill(Bool_t b) { fAutoFill = b; }
    protected:
      Bool_t fAutoFill; //!=true then fill automatically in TreeWriter (def=1)
  };

  class TreeWriter : public TNamed 
  {
    public:
      TreeWriter(const char *tname="MitTree", Bool_t doreset = kFALSE);
      ~TreeWriter();

      void                 AddBranch(const char *name, const char *cname, 
                                     void *obj,  Int_t bsize, Int_t level);
      void                 AddBranch(const char *name, const char *cname, 
                                     void *obj, Int_t bsize);
      void                 AddBranch(const char *name, const char *cname, 
                                     void *obj);
      void                 AddBranchToTree(const char *tname, const char *name, const char *cname, 
                                           void *obj,  Int_t bsize, Int_t level);
      void                 AddBranchToTree(const char *tname, const char *name, const char *cname, 
                                           void *obj, Int_t bsize);
      void                 AddBranchToTree(const char *tname, const char *name, const char *cname, 
                                           void *obj);
      Bool_t               BeginEvent(Bool_t doreset=kFALSE);
      Bool_t               EndEvent(Bool_t doreset=kFALSE);
      const char          *GetBaseURL()                 const { return fBaseURL.IsNull() ? "." : fBaseURL; }  
      Int_t                GetCompressLevel()           const { return fCompressLevel; }
      Long64_t             GetEntries(const char *tn=0) const;
      Long64_t             GetFileSize()                const { return fFile != 0 ? fFile->GetEND() : 0; }
      const TFile         *GetFile()                    const { return fFile; }
      const char          *GetFileName()                const { return Form("%s_%03d.root",
                                                                            GetPrefix(), GetFileNumber()); }
      UShort_t             GetFileNumber()              const { return fFileNumber; }
      const char          *GetFullName()                const { return Form("%s/%s",
                                                                            GetBaseURL(), GetFileName()); }
      const char          *GetPrefix()                  const { return fPrefix; }  
      const TTree         *GetTree(const char *tn)      const;
      TTree               *GetTree(const char *tn);
      void                 Print(Option_t *option="")   const;
      void                 SetAutoFill(const char *tn, Bool_t b);
      void                 SetBaseURL(const char *b)          { fBaseURL = b; }
      void                 SetCompressLevel(Int_t l)          { fCompressLevel = l; }
      void                 SetDefaultBrSize(Int_t s)          { fDefBrSize=s; }
      void                 SetDefaultSL(Int_t s)              { fDefSL=s;}
      void                 SetMaxSize(Long64_t s)             { fMaxSize=s; }
      void                 SetPrefix(const char *p)           { fPrefix = p; }
      void                 StoreObject(const TObject *obj);

    protected:
      TString              fBaseURL;            //base url for tree storage
      TString              fPrefix;             //prefix of file name 
      UShort_t             fFileNumber;         //current sequence number
      Int_t                fCompressLevel;      //compression level used for TFile
      Int_t                fDefBrSize;          //default buffer size for branches
      Int_t                fDefSL;              //default split level for branches
      Long64_t             fMaxSize;            //maximum file size for a file [Bytes]
      const Long64_t       fkMinFreeSpace;      //minimum free space required for closing file
      const Long64_t       fkMinAvgSize;        //minimum average entry size 
      Long64_t             fEvtObjNum;          //event object number offset (for TRef)
      Bool_t               fIsInit;             //true if OpenFile() was called
      Bool_t               fDoObjNumReset;      //true if obj. number resets automatically (def=0)
      TFile               *fFile;               //file being written
      TObjArray            fTrees;              //array of tree(s) being filled

      mithep::MyTree      *AddOrGetMyTree(const char *tn);
      mithep::MyTree      *GetMyTree(const char *tn);
      Bool_t               IsInit()                     const { return fIsInit; }
      Bool_t               IsFull()                     const;
      void                 OpenFile();
      void                 CloseFile();

      ClassDef(TreeWriter,0) // Tree writer class
  };

} /*namespace mithep*/

#endif /*DATATREE_TREEWRITER_H*/