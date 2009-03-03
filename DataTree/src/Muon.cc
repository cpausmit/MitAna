// $Id: Muon.cc,v 1.2 2008/12/01 17:32:34 loizides Exp $

#include "MitAna/DataTree/interface/Muon.h"

ClassImp(mithep::Muon)

using namespace mithep;

//--------------------------------------------------------------------------------------------------
Muon::Muon() : 
  fIsoR03SumPt(0), fIsoR03EmEt(0), fIsoR03HadEt(0), fIsoR03HoEt(0),
  fIsoR03NTracks(0), fIsoR03NJets(0), fIsoR05SumPt(0), fIsoR05EmEt(0),
  fIsoR05HadEt(0), fIsoR05HoEt(0), fIsoR05NTracks(0), fIsoR05NJets(0),
  fEmEnergy(0), fHadEnergy(0), fHoEnergy(0), fEmS9Energy(0), 
  fHadS9Energy(0), fHoS9Energy(0), fNTraversedChambers(0), fIsGlobalMuon(kFALSE),
  fIsTrackerMuon(kFALSE), fIsStandaloneMuon(kFALSE), fIsCaloMuon(kFALSE)
{ 
  // Constructor.

  memset(fDX,0,sizeof(Double32_t)*8); 
  memset(fDY,0,sizeof(Double32_t)*8); 
  memset(fPullX,0,sizeof(Double32_t)*8); 
  memset(fPullY,0,sizeof(Double32_t)*8); 
  memset(fTrackDist,0,sizeof(Double32_t)*8); 
  memset(fTrackDistErr,0,sizeof(Double32_t)*8); 
  memset(fNSegments,0,sizeof(Int_t)*8); 
}
