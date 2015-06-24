//--------------------------------------------------------------------------------------------------
// ObjTypes
//
// Here we define an enum for each data class.
//
// Authors: C.Loizides
//--------------------------------------------------------------------------------------------------

#ifndef MITANA_DATATREE_OBJTYPES_H
#define MITANA_DATATREE_OBJTYPES_H
 
namespace mithep
{
  enum EObjType {
    kUnspecified = 0,
    kBaseVertex,
    kBasicCluster,
    kBeamSpot,
    kBranchName,
    kCaloJet,
    kCaloMet,
    kCaloTau,
    kCaloTower,
    kChargedParticle,
    kCompositeParticle,
    kCompoundParticle,
    kConversion,
    kDataBase,
    kDataObject,
    kDaughterData,
    kDCASig,
    kDecayData,
    kDecayParticle,
    kEmbedWeight,
    kElectron,
    kEventHeader,
    kEvtSelData,
    kGenJet,
    kGenMet,
    kGenericParticle,
    kJet,
    kJPTJet,
    kLAHeader,
    kMCRunInfo,
    kMCEventInfo,
    kMCParticle,
    kMet,
    kMuon,
    kL1TriggerMask,
    kPair,
    kPFCandidate,
    kPFJet,
    kPFMet,
    kPFTau,
    kParticle,
    kSParticle,
    kPhoton,
    kPixelHit,
    kPsCluster,
    kRunInfo,
    kStableData,
    kStableDaughter,
    kStableParticle,
    kStripHit,
    kSuperCluster,
    kTau,
    kTrack,
    kTrackingParticle,
    kTrackJet,
    kTriggerMask,
    kTriggerName,
    kTriggerObject,
    kTriggerObjectBase,
    kTriggerObjectRel,
    kVertex,
    kUnknown
  };
}
#endif
