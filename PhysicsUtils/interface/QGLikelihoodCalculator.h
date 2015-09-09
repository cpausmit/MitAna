// ------------------------------------------------------------
//  
//    QGLikelihoodCalculator - Class
//    for the computation of the QG likelihood.
//    Needs files provided by having run the
//    Ntp1Finalizer_QG on QCD samples.
//
// ------------------------------------------------------------

#ifndef QGLikelihoodCalculator_h
#define QGLikelihoodCalculator_h

#include "TFile.h"
#include "TH1.h"
#include "TString.h"

class QGLikelihoodCalculator {
 public:
  enum InputVariable {
    kPt,
    kEta,
    kRhoIso,
    kMult,
    kPtD,
    kAxis1,
    kAxis2,
    nInputVariables
  };

  QGLikelihoodCalculator(TString const& dataDir, Bool_t chs = false);
   ~QGLikelihoodCalculator();

  double computeQGLikelihoodPU(double pt, double rhoPF, int nCharged, int nNeutral, double ptD, double rmsCand=-1.);
  double computeQGLikelihood2012(double pt, double eta, double rho, int nPFCandidates_QC, double ptD_QC, double axis2_QC);
  double QGvalue(double input[nInputVariables]);

  double likelihoodProduct(double nCharged, double nNeutral, double ptD, double rmsCand, TH1* h1_nCharged, TH1* h1_nNeutral, TH1* h1_ptD, TH1* h1_rmsCand);

 private:
  void loadInputPU();
  void loadInput2012();

  static unsigned const nPtBins_{20};
  static unsigned const nRhoBins_{40};
  double ptBins_[22];
  double rhoBins_[41];

  TString histoFilePath_{""};
  TH1* dataHistoPU_[4 * 2][nPtBins_][nRhoBins_] = {}; // 4 jet types * (g, q)
  TH1* dataHisto2012_[3 * 2][2][nPtBins_][nRhoBins_] = {}; // 3 var types * (g, q)
};

#endif
