#ifndef MITPHYSICS_UTILS_QGTAGGER_H
#define MITPHYSICS_UTILS_QGTAGGER_H

#include "MitAna/DataTree/interface/VertexFwd.h"
#include "MitAna/PhysicsUtils/interface/QGLikelihoodCalculator.h"
#include <map>

namespace mithep {
  class PFJet;

  class QGTagger {
  public:
    QGTagger(bool useCHS);
    virtual ~QGTagger();

    // setters
    void SetRhoIso(double rhoIso) { calculatorInput_[QGLikelihoodCalculator::kRhoIso] = rhoIso; }

    // compute the inputs for gluon-quark discrimination
    void CalculateVariables(const PFJet *jet, const VertexCol *vertices);

    // compute the discriminant
    double QGValue();

    // getters
    double GetPtD();
    double GetAxis1();
    double GetAxis2();
    double GetMult();

  private:
    QGLikelihoodCalculator* qgLikelihood_{0};
    double calculatorInput_[QGLikelihoodCalculator::nInputVariables] = {};

    ClassDef(QGTagger, 1)
  };

}

#endif
