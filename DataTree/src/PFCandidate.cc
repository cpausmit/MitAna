#include "MitAna/DataTree/interface/PFCandidate.h"
#include "MitAna/DataTree/interface/Muon.h"
#include "MitAna/DataTree/interface/Electron.h"
#include "MitAna/DataTree/interface/Photon.h"

ClassImp(mithep::PFCandidate)

// Object reference accessors ins src file to avoid circular dependence of header files

mithep::Muon const*
mithep::PFCandidate::Mu() const
{
  return fMuon.Obj();
}

mithep::Electron const*
mithep::PFCandidate::Ele() const
{
  return fElectron.Obj();
}

mithep::Photon const*
mithep::PFCandidate::Pho() const
{
  return fPhoton.Obj();
}

void
mithep::PFCandidate::SetMuon(Muon const* m)
{
  fMuon = m;
}

void
mithep::PFCandidate::SetElectron(Electron const* e)
{
  fElectron = e;
}
void
mithep::PFCandidate::SetPhoton(Photon const* p)
{
  fPhoton = p;
}

void
mithep::PFCandidate::Mark(UInt_t ib) const
{
  // mark myself
  mithep::DataObject::Mark(ib);
  // mark my dependencies if they are there
  if (fMother.IsValid())
    fMother.Obj()->Mark(ib);
  if (fTrackerTrack.IsValid())
    fTrackerTrack.Obj()->Mark(ib);
  if (fGsfTrack.IsValid())
    fGsfTrack.Obj()->Mark(ib);
  if (fMuon.IsValid())
    fMuon.Obj()->Mark(ib);
  if (fSCluster.IsValid())
    fSCluster.Obj()->Mark(ib);
}
