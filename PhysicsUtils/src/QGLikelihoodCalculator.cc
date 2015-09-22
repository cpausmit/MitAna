#include "MitAna/PhysicsUtils/interface/QGLikelihoodCalculator.h"
#include "MitAna/PhysicsUtils/interface/Bins.h"

#include "TROOT.h"
#include <cmath>
#include <stdexcept>

QGLikelihoodCalculator::QGLikelihoodCalculator(TString const& dataDir, Bool_t chs/* = false*/)
{
  if (dataDir.Length() == 0)
    throw std::runtime_error("No dataDir passed to QGLikelihoodCalculator");

  TString histoFileName("ReducedHisto_2012.root");
  if (chs)
    histoFileName = "ReducedHisto_2012_CHS.root";

  histoFilePath_ = dataDir + "/" + histoFileName;

  Bins::getBins_int(nPtBins_ + 1, ptBins_, 20., 2000., true);
  ptBins_[nPtBins_ + 1] = 4000.;

  Bins::getBins_int(nRhoBins_ + 1, rhoBins_, 0., 40., false);
}

// ADD map destructor
QGLikelihoodCalculator::~QGLikelihoodCalculator()
{
  for (unsigned iH = 0; iH != 4 * 2; ++iH) {
    for (unsigned iPt = 0; iPt != nPtBins_; ++iPt) {
      for (unsigned iRho = 0; iRho != nRhoBins_; ++iRho)
        delete dataHistoPU_[iH][iPt][iRho];
    }
  }

  for (unsigned iV = 0; iV != 3 * 2; ++iV) {
    for (unsigned iCF = 0; iCF != 2; ++iCF) {
      for (unsigned iPt = 0; iPt != nPtBins_; ++iPt) {
        for (unsigned iRho = 0; iRho != nRhoBins_; ++iRho)
          delete dataHisto2012_[iV][iCF][iPt][iRho];
      }
    }
  }
}

void
QGLikelihoodCalculator::loadInputPU()
{
  auto* inputFile = TFile::Open(histoFilePath_);

  if (!inputFile || inputFile->IsZombie())
    throw std::runtime_error(("Histogram input file for QGLikelihoodCalculator " + histoFilePath_ + " not available or corrupted").Data());

  TString hNames[4 * 2] = {
    "rhoBins_pt%d_%d/nChargedJet0_gluon_pt%d_%d_rho%d",
    "rhoBins_pt%d_%d/nChargedJet0_quark_pt%d_%d_rho%d",
    "rhoBins_pt%d_%d/nNeutralJet0_gluon_pt%d_%d_rho%d",
    "rhoBins_pt%d_%d/nNeutralJet0_quark_pt%d_%d_rho%d",
    "rhoBins_pt%d_%d/ptDJet0_gluon_pt%d_%d_rho%d",
    "rhoBins_pt%d_%d/ptDJet0_quark_pt%d_%d_rho%d",
    "rhoBins_pt%d_%d/rmsCandJet0_gluon_pt%d_%d_rho%d",
    "rhoBins_pt%d_%d/rmsCandJet0_quark_pt%d_%d_rho%d"
  };

  for (unsigned iH = 0; iH != 4 * 2; ++iH) {
    for (unsigned iPt = 0; iPt != nPtBins_; ++iPt) {
      int ptMin = ptBins_[iPt];
      int ptMax = ptBins_[iPt + 1];
      for (unsigned iRho = 0; iRho != nRhoBins_; ++iRho) {
        auto* source = dynamic_cast<TH1*>(inputFile->Get(TString::Format(hNames[iH].Data(), ptMin, ptMax, ptMin, ptMax, iRho)));
        if (!source)
          throw std::runtime_error(("Histogram " + TString::Format(hNames[iH].Data(), ptMin, ptMax, ptMin, ptMax, iRho) + " not found in " + histoFilePath_).Data());

        gROOT->cd();
        auto* clone = static_cast<TH1*>(source->Clone());
        clone->Scale(1. / clone->Integral("width"));

        dataHistoPU_[iH][iPt][iRho] = clone;
      }
    }
  }

  delete inputFile;
}

void
QGLikelihoodCalculator::loadInput2012()
{
  auto* inputFile = TFile::Open(histoFilePath_);

  if (!inputFile || inputFile->IsZombie())
    throw std::runtime_error(("Histogram input file for QGLikelihoodCalculator " + histoFilePath_ + " not available or corrupted").Data());


  TString hNames[2][3 * 2] = {
    {
      "rhoBins_pt%d_%d/nPFCand_QC_ptCutJet0_gluon_pt%d_%d_rho%d",
      "rhoBins_pt%d_%d/nPFCand_QC_ptCutJet0_quark_pt%d_%d_rho%d",
      "rhoBins_pt%d_%d/ptD_QCJet0_gluon_pt%d_%d_rho%d",
      "rhoBins_pt%d_%d/ptD_QCJet0_quark_pt%d_%d_rho%d",
      "rhoBins_pt%d_%d/axis2_QCJet0_gluon_pt%d_%d_rho%d",
      "rhoBins_pt%d_%d/axis2_QCJet0_quark_pt%d_%d_rho%d"
    },
    {
      "rhoBins_pt%d_%d/nPFCand_QC_ptCutJet0_F_gluon_pt%d_%d_rho%d",
      "rhoBins_pt%d_%d/nPFCand_QC_ptCutJet0_F_quark_pt%d_%d_rho%d",
      "rhoBins_pt%d_%d/ptD_QCJet0_F_gluon_pt%d_%d_rho%d",
      "rhoBins_pt%d_%d/ptD_QCJet0_F_quark_pt%d_%d_rho%d",
      "rhoBins_pt%d_%d/axis2_QCJet0_F_gluon_pt%d_%d_rho%d",
      "rhoBins_pt%d_%d/axis2_QCJet0_F_quark_pt%d_%d_rho%d"
    }
  };

  for (unsigned iV = 0; iV != 3 * 2; ++iV) {
    for (unsigned iCF = 0; iCF != 2; ++iCF) {
      for (unsigned iPt = 0; iPt != nPtBins_; ++iPt) {
        int ptMin = ptBins_[iPt];
        int ptMax = ptBins_[iPt + 1];
        for (unsigned iRho = 0; iRho != nRhoBins_; ++iRho) {
          auto* source = dynamic_cast<TH1*>(inputFile->Get(TString::Format(hNames[iCF][iV].Data(), ptMin, ptMax, ptMin, ptMax, iRho)));
          if (!source)
            throw std::runtime_error(("Histogram " + TString::Format(hNames[iCF][iV].Data(), ptMin, ptMax, ptMin, ptMax, iRho) + " not found in " + histoFilePath_).Data());

          gROOT->cd();
          TH1* clone = static_cast<TH1*>(source->Clone());

          // try to make it more stable
          if (clone->GetEntries() < 50.)
            clone->Rebin(5);
          else if (clone->GetEntries() < 500.)
            clone->Rebin(2);

          clone->Scale(1. / clone->Integral("width"));

          dataHisto2012_[iV][iCF][iPt][iRho] = clone;
        }
      }
    }
  }

  delete inputFile;
}

double
QGLikelihoodCalculator::computeQGLikelihoodPU(double pt, double rhoPF, int nCharged, int nNeutral, double ptD, double rmsCand)
{
  double ptMin = 0.;
  double ptMax = 0.;
  int iPt = Bins::getBin(nPtBins_, ptBins_, pt, &ptMin, &ptMax);
  if (iPt < 0)
    return -1;

  if (ptMax == 0.)
    return -1.;

  double rhoMin = 0.;
  double rhoMax = 0;
  int iRho = Bins::getBin(nRhoBins_, rhoBins_, rhoPF, &rhoMin, &rhoMax);
  if (iRho < 0)
    return -1;

  if (!dataHistoPU_[0][0][0])
    loadInputPU();

  TH1* h1_nCharged_gluon = dataHistoPU_[0][iPt][iRho];
  TH1* h1_nCharged_quark = dataHistoPU_[1][iPt][iRho];
  TH1* h1_nNeutral_gluon = dataHistoPU_[2][iPt][iRho];
  TH1* h1_nNeutral_quark = dataHistoPU_[3][iPt][iRho];
  TH1* h1_ptD_gluon = (ptD >= 0.) ? dataHistoPU_[4][iPt][iRho] : 0;
  TH1* h1_ptD_quark = (ptD >= 0.) ? dataHistoPU_[5][iPt][iRho] : 0;
  TH1* h1_rmsCand_gluon = (rmsCand >= 0.) ? dataHistoPU_[6][iPt][iRho] : 0;
  TH1* h1_rmsCand_quark = (rmsCand >= 0.) ? dataHistoPU_[7][iPt][iRho] : 0;

  double gluonP = likelihoodProduct(nCharged, nNeutral, ptD, rmsCand, h1_nCharged_gluon, h1_nNeutral_gluon, h1_ptD_gluon, h1_rmsCand_gluon);
  double quarkP = likelihoodProduct(nCharged, nNeutral, ptD, rmsCand, h1_nCharged_quark, h1_nNeutral_quark, h1_ptD_quark, h1_rmsCand_quark);

  return quarkP / (gluonP + quarkP);
}

//---------------------------------------------------------------------------------------------------
double
QGLikelihoodCalculator::computeQGLikelihood2012(double pt, double eta, double rho,
                                                int nPFCandidates_QC_ptCut, double ptD_QC,
                                                double axis2_QC)
{
  // in forward use inclusive 127-4000 bin
  int iCF = std::abs(eta) <= 2.5 ? 0 : 1;
  if (iCF == 1 && pt > 127.)
    pt = 128.;

  double vars[3] = {
    double(nPFCandidates_QC_ptCut),
    double(ptD_QC),
    -std::log(axis2_QC)
  };

  double ptMin=0.;
  double ptMax=0.;

  int iPt = Bins::getBin(nPtBins_, ptBins_, pt, &ptMin, &ptMax);
  if (iPt < 0)
    return -1;

  if (ptMax == 0.)
    return -1.;

  double rhoMin = 0.;
  double rhoMax = 0;
  int iRho = Bins::getBin(nRhoBins_, rhoBins_, rho, &rhoMin, &rhoMax);
  if (iRho < 0)
    return -1;

  if (!dataHisto2012_[0][0][0][0])
    loadInput2012();

  TH1* dataG[3] = {
    dataHisto2012_[0][iCF][iPt][iRho],
    dataHisto2012_[2][iCF][iPt][iRho],
    dataHisto2012_[4][iCF][iPt][iRho]
  };

  TH1* dataQ[3] = {
    dataHisto2012_[1][iCF][iPt][iRho],
    dataHisto2012_[3][iCF][iPt][iRho],
    dataHisto2012_[5][iCF][iPt][iRho]
  };

  double G = 1.;
  double Q = 1.;

  for(unsigned iV = 0; iV != 3; ++iV){
    double Gi = dataG[iV]->GetBinContent(dataG[iV]->FindBin(vars[iV]));
    double mG = dataG[iV]->GetMean();

    double Qi = dataQ[iV]->GetBinContent(dataQ[iV]->FindBin(vars[iV]));
    double mQ = dataQ[iV]->GetMean();

    double delta = 0.000001;
    if (Qi <= 0. && Gi <= 0.) {
      if (mQ > mG) {
        if(vars[iV] > mQ) {
          Qi = 1. - delta;
          Gi = delta;
        }
        else if (vars[iV] < mG) {
          Qi = delta;
          Gi = 1. - delta;
        }
      }
      else if (mQ < mG) {
        if(vars[iV] < mQ) {
          Qi = 1. - delta;
          Gi = delta;
        }
        else if(vars[iV] > mG) {
          Qi = delta;
          Gi = 1. - delta;
        }
      }
    }

    Q *= Qi;
    G *= Gi;	
  }

  if (Q == 0)
    return 0.;

  return Q / (Q + G);
}

//---------------------------------------------------------------------------------------------------
double
QGLikelihoodCalculator::likelihoodProduct(double nCharged, double nNeutral, double ptD,
                                          double rmsCand,
                                          TH1* h1_nCharged, TH1* h1_nNeutral,
                                          TH1* h1_ptD, TH1* h1_rmsCand)
{
  double likeliProd = h1_nCharged->GetBinContent(h1_nCharged->FindBin(nCharged));
  if (h1_nNeutral)
    likeliProd *= h1_nNeutral->GetBinContent(h1_nNeutral->FindBin(nNeutral));
  if (h1_ptD)
    likeliProd *= h1_ptD->GetBinContent(h1_ptD->FindBin(ptD));
  if (h1_rmsCand)
    likeliProd *= h1_rmsCand->GetBinContent(h1_rmsCand->FindBin(rmsCand));

  return likeliProd;
}

//---------------------------------------------------------------------------------------------------
double
QGLikelihoodCalculator::QGvalue(double input[nInputVariables])
{
  return computeQGLikelihood2012(input[kPt], input[kEta], input[kRhoIso],
				 input[kMult], input[kPtD], input[kAxis2]);
}
