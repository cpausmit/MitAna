// $Id: EvtSelData.cc,v 1.1 2009/12/08 10:49:54 loizides Exp $

#include "MitAna/DataTree/interface/EvtSelData.h"

ClassImp(mithep::EvtSelData)

using namespace mithep;

//--------------------------------------------------------------------------------------------------
EvtSelData::EvtSelData(Double_t HcalNeg, Double_t HcalPos,
                       Double_t HfNeg, Double_t HfPos, Double_t HfNegTime, Double_t HfPosTime,
                       Double_t CaNeg, Double_t CaPos, Double_t CaNegTime, Double_t CaPosTime,
                       Double_t ZdcNeg, Double_t ZdcPos, Double_t ZdcNegTime, Double_t ZdcPosTime,
                       Int_t PxbHits, Int_t PxHits, Double_t ClusVtxQual, Double_t ClusVtxDiff)
  :   fHcalNeg(HcalNeg), fHcalPos(HcalPos),
      fHfNeg(HfNeg), fHfPos(HfPos), fHfNegTime(HfNegTime), fHfPosTime(HfPosTime),
      fCaNeg(CaNeg), fCaPos(CaPos), fCaNegTime(CaNegTime), fCaPosTime(CaPosTime),
      fZdcNeg(ZdcNeg), fZdcPos(ZdcPos), fZdcNegTime(ZdcNegTime), fZdcPosTime(ZdcPosTime),
      fPxbHits(PxbHits), fPxHits(PxHits), fClusVtxQual(ClusVtxQual), fClusVtxDiff(ClusVtxDiff)
{
  // Default constructor.
}

//--------------------------------------------------------------------------------------------------
void EvtSelData::Set (Double_t HcalNeg, Double_t HcalPos,
                      Double_t HfNeg, Double_t HfPos, Double_t HfNegTime, Double_t HfPosTime,
                      Double_t CaNeg, Double_t CaPos, Double_t CaNegTime, Double_t CaPosTime,
                      Double_t ZdcNeg, Double_t ZdcPos, Double_t ZdcNegTime, Double_t ZdcPosTime,
		      Int_t PxbHits, Int_t PxHits, Double_t ClusVtxQual, Double_t ClusVtxDiff)

{
  // Set values.

  fHcalNeg     = HcalNeg; 
  fHcalPos     = HcalPos;
  fHfNeg       = HfNeg; 
  fHfPos       = HfPos; 
  fHfNegTime   = HfNegTime; 
  fHfPosTime   = HfPosTime;
  fCaNeg       = CaNeg; 
  fCaPos       = CaPos; 
  fCaNegTime   = CaNegTime; 
  fCaPosTime   = CaPosTime;
  fZdcNeg      = ZdcNeg; 
  fZdcPos      = ZdcPos;
  fZdcNegTime  = ZdcNegTime; 
  fZdcPosTime  = ZdcPosTime;
  fPxbHits     = PxbHits;
  fPxHits      = PxHits;
  fClusVtxQual = ClusVtxQual;
  fClusVtxDiff = ClusVtxDiff;
}