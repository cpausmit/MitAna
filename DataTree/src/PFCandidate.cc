#include "MitAna/DataTree/interface/PFCandidate.h"
#include "MitAna/DataTree/interface/Muon.h"
#include "MitAna/DataTree/interface/Electron.h"
#include "MitAna/DataTree/interface/Photon.h"

ClassImp(mithep::PFCandidate)

// Object reference accessors in src file to avoid circular dependence of header files

mithep::Track const*
mithep::PFCandidate::TrackerTrk() const
{
  if (fTrackerTrack.IsValid()) {
    // reading pre-version 6 object
    return fTrackerTrack.Obj();
  }

  for (UInt_t iR = 0; iR != fRefs.GetEntries(); ++iR) {
    auto* obj = fRefs.At(iR);
    if (obj && obj->ObjType() == kTrack) {
      auto* track = static_cast<Track const*>(obj);
      if (!track->IsGsf())
        return track;
    }
  }

  return 0;
}

mithep::Track const*
mithep::PFCandidate::GsfTrk() const
{
  if (fGsfTrack.IsValid()) {
    // reading pre-version 6 object
    return fGsfTrack.Obj();
  }

  for (UInt_t iR = 0; iR != fRefs.GetEntries(); ++iR) {
    auto* obj = fRefs.At(iR);
    if (obj && obj->ObjType() == kTrack) {
      auto* track = static_cast<Track const*>(obj);
      if (track->IsGsf())
        return track;
    }
  }

  return 0;
}

mithep::SuperCluster const*
mithep::PFCandidate::SCluster() const
{
  if (fSCluster.IsValid()) {
    // reading pre-version 6 object
    return fSCluster.Obj();
  }

  for (UInt_t iR = 0; iR != fRefs.GetEntries(); ++iR) {
    auto* obj = fRefs.At(iR);
    if (obj && obj->ObjType() == kSuperCluster)
      return static_cast<SuperCluster const*>(obj);
  }

  return 0;
}

mithep::Muon const*
mithep::PFCandidate::Mu() const
{
  if (fMuon.IsValid()) {
    // reading pre-version 6 object
    return fMuon.Obj();
  }

  for (UInt_t iR = 0; iR != fRefs.GetEntries(); ++iR) {
    auto* obj = fRefs.At(iR);
    if (obj && obj->ObjType() == kMuon)
      return static_cast<Muon const*>(obj);
  }

  return 0;
}

mithep::Electron const*
mithep::PFCandidate::Ele() const
{
  if (fElectron.IsValid()) {
    // reading pre-version 6 object
    return fElectron.Obj();
  }

  for (UInt_t iR = 0; iR != fRefs.GetEntries(); ++iR) {
    auto* obj = fRefs.At(iR);
    if (obj && obj->ObjType() == kElectron)
      return static_cast<Electron const*>(obj);
  }

  return 0;
}

mithep::Photon const*
mithep::PFCandidate::Pho() const
{
  if (fPhoton.IsValid()) {
    // reading pre-version 6 object
    return fPhoton.Obj();
  }

  for (UInt_t iR = 0; iR != fRefs.GetEntries(); ++iR) {
    auto* obj = fRefs.At(iR);
    if (obj && obj->ObjType() == kPhoton)
      return static_cast<Photon const*>(obj);
  }

  return 0;
}

void
mithep::PFCandidate::Mark(UInt_t ib) const
{
  // mark myself
  mithep::DataObject::Mark(ib);
  // mark my dependencies if they are there
  if (fMother.IsValid())
    fMother.Obj()->Mark(ib);
  for (UInt_t iR = 0; iR != fRefs.GetEntries(); ++iR) {
    auto* obj = fRefs.At(iR);
    if (obj)
      obj->Mark(ib);
  }
}
