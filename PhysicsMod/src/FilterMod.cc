#include "MitAna/PhysicsMod/interface/FilterMod.h"
#include "MitAna/DataTree/interface/PFCandidate.h"
#include "MitAna/DataTree/interface/MCParticle.h"
#include <algorithm>

using namespace mithep;

templateClassImp(mithep::FilterMod)

template<>
Bool_t
mithep::FilterMod<mithep::PFCandidate, mithep::PFCandidate>::IsAllowedParticleId(mithep::PFCandidate const& part) const
{
  if (fParticleIds.size() == 0)
    return true;

  int id = part.PFType();
  if (part.Charge() < 0.)
    id *= -1;

  bool inlist = std::binary_search(fParticleIds.begin(), fParticleIds.end(), id);
  return fVetoParticleId ? !inlist : inlist;
}

template<>
Bool_t
mithep::FilterMod<mithep::MCParticle, mithep::MCParticle>::IsAllowedParticleId(mithep::MCParticle const& part) const
{
  if (fParticleIds.size() == 0)
    return true;

  bool inlist = std::binary_search(fParticleIds.begin(), fParticleIds.end(), part.PdgId());
  return fVetoParticleId ? !inlist : inlist;
}
