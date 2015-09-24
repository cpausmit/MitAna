#include "MitAna/PhysicsUtils/interface/Bins.h"

#include "TMath.h"

int
Bins::getBins(double* bins, int nBins, double MinBin, double MaxBin, bool log)
{
  if (log) {
    double incr = TMath::Power(MaxBin / MinBin, 1. / nBins);
    bins[0] = MinBin;
    bins[nBins] = MaxBin;
    for(int i = 1; i < nBins; i++)
      bins[i] = bins[i - 1] * incr;
  }
  else {
    double incr = (MaxBin - MinBin) / nBins;
    bins[0] = MinBin;
    bins[nBins + 1] = MaxBin;
    for(int i = 1; i < nBins + 1; i++)
      bins[i] = bins[i - 1] + incr;
  }

  return 0;
}


int
Bins::getBin(int nBins, double bins[], double value, double* x0, double* x1)
{
  if (value < bins[0])
    return -1;

  if (value > bins[nBins]) {
    if (x0)
      *x0 = bins[nBins - 1];
    if (x1)
      *x1 = bins[nBins];
    return nBins - 1;
  }

  int R = 0;
  for (; R < nBins; ++R) {
    if (bins[R] > value)
      break;	
  }
  --R;
  if (x0)
    *x0 = bins[R];
  if (x1)
    *x1 = bins[R + 1];

  return R;	
}

void
Bins::getBins_int(int nBins_total, double* Lower, double xmin, double xmax, bool plotLog)
{
  int nBins = nBins_total - 1;
  double dx = plotLog ? TMath::Power(xmax / xmin, 1. / nBins) : (xmax - xmin) / nBins;

  double lowerExact = xmin;
  Lower[0] = TMath::Ceil(lowerExact);
  for (int i = 1; i != nBins; ++i) {
    if (plotLog) {
      lowerExact *= dx;
      Lower[i] = TMath::Ceil(lowerExact);
    }
    else
      Lower[i] = TMath::Ceil(Lower[i - 1] + dx);
  }

  Lower[nBins] = xmax;
}
