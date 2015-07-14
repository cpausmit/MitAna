#ifndef ISRMODEL_H
#define ISRMODEL_H

#include "ShowerDeconstruction/SDAlgorithm/interface/Model.h"
#include <fastjet/PseudoJet.hh>
#include <vector>

#include "ShowerDeconstruction/SDAlgorithm/interface/Parameters.h"

#include "ShowerDeconstruction/SDAlgorithm/interface/StoredJet.h"

namespace Deconstruction {

  class ISRModel : public Model {
    public:
      ISRModel(Parameters &param);
      virtual ~ISRModel();

      double weight(const StoredJet &jets, fastjet::PseudoJet &sum);
      double weight(const std::vector<fastjet::PseudoJet> &jets, fastjet::PseudoJet &sum);
      double hamiltonian(double pTsum = 0);
  };

}

#endif

