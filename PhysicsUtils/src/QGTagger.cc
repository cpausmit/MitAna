#include "MitAna/PhysicsUtils/interface/QGTagger.h"
#include "MitAna/DataTree/interface/PFJetCol.h"
#include "MitAna/DataTree/interface/VertexCol.h"

#include "TSystem.h"

ClassImp(mithep::QGTagger)

using namespace mithep;

QGTagger::QGTagger(bool useCHS)
{
  qgLikelihood_ = new QGLikelihoodCalculator(gSystem->Getenv("MIT_DATA"), useCHS);
}

QGTagger::~QGTagger()
{
  // Destructor.
}

double
QGTagger::QGValue()
{
  return qgLikelihood_->QGvalue(calculatorInput_);
}

void
QGTagger::CalculateVariables(const PFJet *jet, const VertexCol *vertices)
{
  // quantities we need to fill: pt, eta, rhoIso, mult, ptD, axis2
  calculatorInput_[QGLikelihoodCalculator::kPt] = jet->Pt(); // no correction applied, assume we get already corrected/calibrated jets
  calculatorInput_[QGLikelihoodCalculator::kEta] = jet->Eta();

  // counters
  UInt_t chargedMultiplicity = 0, neutralMultiplicity = 0;
  // global quantities
  double sumWeight = 0., sumDeltaEta = 0., sumDeltaPhi = 0., sumDeltaEta2 = 0., sumDeltaPhi2 = 0., sumDeltaEtaDeltaPhi = 0., sumPt = 0.;

  // loop over constituents
  for (unsigned iCand = 0; iCand < jet->NPFCands(); ++iCand) {
    auto& pfCand(*jet->PFCand(iCand));
    
    // charged candidates
    Track const* track = pfCand.TrackerTrk();
    bool isGoodForAxis = false;
    if (track) {
      // get closest vertex
      double minDZ = -1.;
      int closestVertexIndex = -1;
      for (unsigned iV = 0; iV < vertices->GetEntries(); ++iV){
        double dz = TMath::Abs(track->DzCorrected(*vertices->At(iV)));
        if (minDZ < 0. || dz < minDZ) {
          minDZ = dz;
          closestVertexIndex = iV;
          if (iV != 0)
            break;
        }
      }

      // assume the first vertex is the good one
      if (closestVertexIndex == 0 && track->Quality().Quality(TrackQuality::highPurity)) {
        auto* pv = vertices->At(0);
        double deltaZErr = TMath::Sqrt(track->DszErr() * track->DszErr() + pv->ZErr() * pv->ZErr());
        if (minDZ / deltaZErr < 5.) {
          isGoodForAxis = true;
          double deltaXY = TMath::Abs(track->D0Corrected(*pv));
          double deltaXYErr = TMath::Sqrt(track->DxyErr() * track->DxyErr() + pv->XErr() * pv->XErr() + pv->YErr() * pv->YErr());
          if (deltaXY / deltaXYErr < 5.)
            ++chargedMultiplicity;
        }
      }
    }
    else {
      // neutral candidates
      if (pfCand.Pt() > 1.0)
        ++neutralMultiplicity;

      isGoodForAxis = true;
    }

    // update global quantities
    double deltaEta = pfCand.Eta() - jet->Eta();
    double deltaPhi = 2. * TMath::ATan(TMath::Tan((pfCand.Phi() - jet->Phi()) / 2.));
    double weight = pfCand.Pt() * pfCand.Pt();

    if (isGoodForAxis) {
      sumWeight += weight;
      sumPt += pfCand.Pt();
      sumDeltaEta += deltaEta*weight;
      sumDeltaPhi += deltaPhi*weight;
      sumDeltaEta2 += deltaEta*deltaEta*weight;
      sumDeltaEtaDeltaPhi += deltaEta*deltaPhi*weight;
      sumDeltaPhi2 += deltaPhi*deltaPhi*weight;
    }
  }

  // calculate axis and ptD
  double a = 0., b = 0., c = 0.;
  double averageDeltaEta = 0., averageDeltaPhi = 0., averageDeltaEta2 = 0., averageDeltaPhi2 = 0.;
  if (sumWeight > 0.) {
    calculatorInput_[QGLikelihoodCalculator::kPtD] = TMath::Sqrt(sumWeight)/sumPt;
    averageDeltaEta = sumDeltaEta/sumWeight;
    averageDeltaPhi = sumDeltaPhi/sumWeight;
    averageDeltaEta2 = sumDeltaEta2/sumWeight;
    averageDeltaPhi2 = sumDeltaPhi2/sumWeight;
    a = averageDeltaEta2 - averageDeltaEta*averageDeltaEta;
    b = averageDeltaPhi2 - averageDeltaPhi*averageDeltaPhi;
    c = -(sumDeltaEtaDeltaPhi/sumWeight - averageDeltaEta*averageDeltaPhi);
  }
  else
    calculatorInput_[QGLikelihoodCalculator::kPtD] = 0.;

  double delta = sqrt(fabs((a-b)*(a-b)+4*c*c));
  if (a+b+delta > 0)
    calculatorInput_[QGLikelihoodCalculator::kAxis1] = sqrt(0.5*(a+b+delta));
  else
    calculatorInput_[QGLikelihoodCalculator::kAxis1] = 0.;
  if (a+b-delta > 0)
    calculatorInput_[QGLikelihoodCalculator::kAxis2] = sqrt(0.5*(a+b-delta));
  else
    calculatorInput_[QGLikelihoodCalculator::kAxis2] = 0.;

  calculatorInput_[QGLikelihoodCalculator::kMult] = (chargedMultiplicity + neutralMultiplicity);
}

double QGTagger::GetPtD()
{
  return calculatorInput_[QGLikelihoodCalculator::kPtD];
}

double QGTagger::GetAxis1()
{
  return calculatorInput_[QGLikelihoodCalculator::kAxis1];
}

double QGTagger::GetAxis2() {
  return calculatorInput_[QGLikelihoodCalculator::kAxis2];
}

double QGTagger::GetMult() {
  return calculatorInput_[QGLikelihoodCalculator::kMult];
}
