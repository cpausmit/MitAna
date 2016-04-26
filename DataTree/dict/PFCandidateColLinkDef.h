#ifndef MITANA_DATATREE_PFCANDIDATECOLLINKDEF_H
#define MITANA_DATATREE_PFCANDIDATECOLLINKDEF_H

#include "MitAna/DataCont/interface/RefArray.h"
#include "MitAna/DataCont/interface/Ref.h"
#include "MitAna/DataTree/interface/PFCandidateCol.h"
#endif

#ifndef __CLING__
#   define _R__UNIQUEIDENTIFIER_ PFCandidateCol
#   define _R__JOIN3_(F,X,Y) _NAME3_(F,X,Y)
#   undef _R__UNIQUE_
#   define _R__UNIQUE_(X) _R__JOIN3_( _R__UNIQUEIDENTIFIER_,X,__LINE__)
#endif

#ifdef __CLING__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ nestedclass;
#pragma link C++ nestedtypedef;
#pragma link C++ namespace mithep;

/* #pragma read \ */
/*     sourceClass="mithep::PFCandidate" \ */
/*     version="[-5]" \ */
/*     include="MitAna/DataTree/interface/Muon.h,MitAna/DataTree/interface/Electron.h,MitAna/DataTree/interface/Photon.h,MitAna/DataTree/interface/SuperCluster.h" \ */
/*     source="mithep::Ref<mithep::Track> fTrackerTrack; \ */
/*             mithep::Ref<mithep::Track> fGsfTrack; \ */
/*             mithep::Ref<mithep::Muon> fMuon; \ */
/*             mithep::Ref<mithep::SuperCluster> fSCluster; \ */
/*             mithep::Ref<mithep::Electron> fElectron; \ */
/*             mithep::Ref<mithep::Photon> fPhoton; \ */
/*             mithep::BitMask32 fPFFlags;" \ */
/*     targetClass="mithep::PFCandidate" \ */
/*     target="fRefs, fPFFlagBits" \ */
/*     code="{ if (onfile.fTrackerTrack.IsValid()) fRefs.Add(onfile.fTrackerTrack.Obj()); \ */
/*       if (onfile.fGsfTrack.IsValid()) fRefs.Add(onfile.fGsfTrack.Obj()); \ */
/*       if (onfile.fMuon.IsValid()) fRefs.Add(onfile.fMuon.Obj()); \ */
/*       if (onfile.fSCluster.IsValid()) fRefs.Add(onfile.fSCluster.Obj()); \ */
/*       if (onfile.fElectron.IsValid()) fRefs.Add(onfile.fElectron.Obj()); \ */
/*       if (onfile.fPhoton.IsValid()) fRefs.Add(onfile.fPhoton.Obj()); \ */
/*       fPFFlagBits.SetBits(onfile.fPFFlags.Bits()); }" \ */

#pragma read \
    sourceClass="mithep::PFCandidate" \
    version="[-5]" \
    source="mithep::BitMask32 fPFFlags;" \
    targetClass="mithep::PFCandidate" \
    target="fPFFlagBits" \
    code="{ fPFFlagBits.SetBits(onfile.fPFFlags.Bits()); }" \

#pragma link C++ class mithep::Ref<mithep::PFCandidate>+;
#pragma link C++ class mithep::PFCandidate+;
#pragma link C++ class mithep::Collection<mithep::PFCandidate>+;
#pragma link C++ class mithep::Array<mithep::PFCandidate>+;
#pragma link C++ class mithep::ObjArray<mithep::PFCandidate>+;
#pragma link C++ class mithep::RefArray<mithep::PFCandidate>+;
#pragma link C++ typedef mithep::PFCandidateCol;
#pragma link C++ typedef mithep::PFCandidateArr;
#pragma link C++ typedef mithep::PFCandidateOArr;
#endif
