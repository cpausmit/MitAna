#include "MitAna/DataTree/interface/PFCandidate.h"
#include "MitAna/DataTree/interface/Muon.h"
#include "MitAna/DataTree/interface/Electron.h"
#include "MitAna/DataTree/interface/Photon.h"
#include "MitAna/DataTree/interface/Track.h"
#include "MitAna/DataTree/interface/SuperCluster.h"

ClassImp(mithep::PFCandidate)

// Object reference accessors in src file to avoid circular dependence of header files

Bool_t
mithep::PFCandidate::HasTrackerTrk() const
{
  return fTrackerTrack.IsValid();
}

Bool_t
mithep::PFCandidate::HasGsfTrk() const
{
  return fGsfTrack.IsValid();
}

Bool_t
mithep::PFCandidate::HasSCluster() const
{
  return fSCluster.IsValid();

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
mithep::PFCandidate::SetTrackerTrk(Track const* t)
{
  fTrackerTrack = t;
}

void
mithep::PFCandidate::SetGsfTrk(Track const* t)
{
  fGsfTrack = t;
}

void
mithep::PFCandidate::SetSCluster(SuperCluster const* s)
{
  fSCluster = s;
}

mithep::Track const*
mithep::PFCandidate::TrackerTrk() const
{
  return fTrackerTrack.Obj();
}

mithep::Track const*
mithep::PFCandidate::GsfTrk() const
{
  return fGsfTrack.Obj();
}

mithep::SuperCluster const*
mithep::PFCandidate::SCluster() const
{
  return fSCluster.Obj();
}

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
mithep::PFCandidate::Mark(UInt_t ib) const
{
  // mark myself
  mithep::DataObject::Mark(ib);

  // mark my dependencies if they are there
  if (fTrackerTrack.IsValid())
    fTrackerTrack.Obj()->Mark(ib);
  if (fGsfTrack.IsValid())
    fGsfTrack.Obj()->Mark(ib);
  if (fMuon.IsValid())
    fMuon.Obj()->Mark(ib);
  if (fSCluster.IsValid())
    fSCluster.Obj()->Mark(ib);
}
