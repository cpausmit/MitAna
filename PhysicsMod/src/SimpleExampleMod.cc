#include "MitAna/PhysicsMod/interface/SimpleExampleMod.h"
#include "MitAna/DataTree/interface/Names.h"
#include "MitAna/DataTree/interface/MCParticleCol.h"
#include <TH1D.h>

using namespace mithep;

ClassImp(mithep::SimpleExampleMod)

//--------------------------------------------------------------------------------------------------
SimpleExampleMod::SimpleExampleMod(const char *name, const char *title) : 
  BaseMod(name,title),
  fPartName(Names::gkMCPartBrn),
  fPtHist(0),
  fEtaHist(0)
{
  // Constructor.
}

//--------------------------------------------------------------------------------------------------
void SimpleExampleMod::Begin()
{
  // Run startup code on the client machine. For this module, we dont
  // do anything here.
}

//--------------------------------------------------------------------------------------------------
void SimpleExampleMod::BeginRun()
{
  // Run startup code on the client machine. For this module, we dont
  // do anything here.
}

//--------------------------------------------------------------------------------------------------
void SimpleExampleMod::EndRun()
{
  // Run startup code on the client machine. For this module, we dont
  // do anything here.
}

//--------------------------------------------------------------------------------------------------
void SimpleExampleMod::Process()
{
  // Process entries of the tree. For this module, we just load
  // the branch and fill the histograms.

  auto* particles = GetObject<mithep::MCParticleCol>(GetPartName());

  Int_t ents=particles->GetEntries();
  for(Int_t i=0;i<ents;++i) {
     const MCParticle *p = particles->At(i);
     fPtHist->Fill(p->Pt());
     fEtaHist->Fill(p->Eta());
  }

}

//--------------------------------------------------------------------------------------------------
void SimpleExampleMod::SlaveBegin()
{
  // Run startup code on the computer (slave) doing the actual
  // analysis. Here, we typically initialize histograms and 
  // other analysis objects and request branches. For this module,
  // we request a branch of the MitTree.

  AddTH1(fPtHist,"hPtHist",";p_{t} [GeV];#",100,0.,25.);
  AddTH1(fEtaHist,"hEtaHist",";#eta;#",160,-8.,8.);
}

//--------------------------------------------------------------------------------------------------
void SimpleExampleMod::SlaveTerminate()
{
  // Run finishing code on the computer (slave) that did the
  // analysis. For this module, we dont do anything here.
}

//--------------------------------------------------------------------------------------------------
void SimpleExampleMod::Terminate()
{
  // Run finishing code on the client computer. 
  // For this module, we dont do anything here.
}
