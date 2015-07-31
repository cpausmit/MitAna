//--------------------------------------------------------------------------------------------------
// FastJetMod 
//
// This module process a collection of input PFCandidates, cluster them
// using fastjet and then spits out Bambu PFJets collection(s)
//
// Authors: L.DiMatteo
//--------------------------------------------------------------------------------------------------

#ifndef MITANA_MODS_FastJetMod_H
#define MITANA_MODS_FastJetMod_H

#include "fastjet/PseudoJet.hh"
#include "fastjet/JetDefinition.hh"
#include "fastjet/GhostedAreaSpec.hh"
#include "fastjet/AreaDefinition.hh"
#include "fastjet/ClusterSequenceArea.hh"

#include "MitAna/TreeMod/interface/BaseMod.h"
#include "MitAna/DataTree/interface/JetCol.h"
#include "MitAna/DataTree/interface/PFJetCol.h"
#include "MitAna/DataTree/interface/PFCandidateCol.h"

namespace mithep
{
  class FastJetMod : public BaseMod
  {
    public:
      FastJetMod(const char *name = "FastJetMod",
                   const char *title = "FastJet module");
      ~FastJetMod();

      const char *GetOutputJetsName() const { return fOutputJetsName;     }

      UInt_t GetJetAlgorithm() const        { return fJetAlgorithm;       }
      void SetJetAlgorithm(UInt_t n)        { fJetAlgorithm = n;          }
      
      void GetMatchBtag(Bool_t b)               { fGetMatchBtag = b;          }
      void UseBambuJets(Bool_t b)               { fUseBambuJets = b;          }

      void SetBtaggedJetsName(const char *n)    { fBtaggedJetsName = n;       }
      void SetJetsName(const char *n)           { fJetsName = n;              }
      void SetPfCandidatesName(const char *n)   { fPfCandidatesName = n;      }

      void SetOutputJetsName(const char *n)     { fOutputJetsName = n;        }
                                                                    
      void SetConeSize(double d)                { fJetConeSize = d;           }

      void SetParticleMinPt(double d)           { fParticleMinPt = d;         }
      void SetJetMinPt(double d)                { fJetMinPt = d;              }
 
    protected:
      void Process();
      void SlaveBegin();
      void SlaveTerminate();
      
      // PFJet filler helper
      void FillPFJet (PFJet *pPFJet, fastjet::PseudoJet &fjJet);    

      enum JetAlgorithms {
        kCA,
        kKT,
        kAK
      };
        
    private:

      UInt_t fJetAlgorithm = kAK;
      Bool_t fGetMatchBtag;                //=true if b-tag obtained by match with standard jets (AK5)
      Bool_t fUseBambuJets;                //=true if input small jets already present in bambu
      Bool_t fUseBambuFatJets;             //=true if input large jets already present in bambu

      TString fBtaggedJetsName;            //(i) name of input btagged jets
      const PFJetCol *fBtaggedJets;        //input btagged jets
      
      TString fJetsName;                   //(i) name of input jets
      const PFJetCol *fJets;               //input jets

      TString fPfCandidatesName;           //(i) name of PF candidates coll
      const PFCandidateCol *fPfCandidates; //particle flow candidates coll handle
 
      TString fOutputJetsName;             //name of output jets collection
      JetOArr *fOutputJets;                //output jets collection
      
      // Objects from fastjet we want to use
      double fJetConeSize;                 //fastjet clustering radius
      double fFatJetConeSize;              //fastjet fat clustering radius
      fastjet::JetDefinition *fJetDef  ;   //fastjet clustering definition
      fastjet::GhostedAreaSpec *fActiveArea;
      fastjet::AreaDefinition *fAreaDefinition;

      double fParticleMinPt;               //low cutoff to avoid including unphysical PFCands
      double fJetMinPt;                    //low cutoff to carrying on soft jets
      
      ClassDef(FastJetMod, 2)              //FastJet bambu producer      
  };
}
#endif
