#ifndef MITANA_DATATREE_PILEUPENERGYDENSITYCOLLINKDEF_H
#define MITANA_DATATREE_PILEUPENERGYDENSITYCOLLINKDEF_H

#include "MitAna/DataCont/interface/Ref.h"
#include "MitAna/DataTree/interface/PileupEnergyDensityCol.h"
#endif

#ifndef __CLING__
#   define _R__UNIQUEIDENTIFIER_ PileupEnergyDensityCol
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

// below onfile.LowEta is filled to kHighEta - this is not a typo!
#pragma read \
    sourceClass="mithep::PileupEnergyDensity" \
    version="[-4]" \
    source="Double32_t fRho; \
            Double32_t fRhoHighEta; \
            Double32_t fRhoRandom; \
            Double32_t fRhoRandomLowEta; \
            Double32_t fRhoFixedGridAll; \
            Double32_t fRhoFixedGridFastjetAll; \
            Double32_t fRhoKt6CaloJets; \
            Double32_t fRhoKt6CaloJetsCentral; \
            Double32_t fRhoKt6PFJets; \
            Double32_t fRhoKt6PFJetsCentralChargedPileUp; \
            Double32_t fRhoKt6PFJetsCentralNeutral; \
            Double32_t fRhoKt6PFJetsCentralNeutralTight;" \
    targetClass="mithep::PileupEnergyDensity" \
    target="fRho, fRhoLegacy" \
    code="{ if (&fRhoLegacy); \
      static const TString bname(Form(\"fRhoLegacy[%d]\", mithep::PileupEnergyDensity::nLegacyAlgos)); \
      static Long_t actualOffset = cls->GetDataMemberOffset(bname); \
      fRhoLegacy_t& fActualRhoLegacy = *(fRhoLegacy_t*)(target + actualOffset); \
      auto setRho([&fRho](UInt_t idx, Double_t value) { fRho[idx] = value; }); \
      auto setLegacy([&fActualRhoLegacy](UInt_t idx, Double_t value) { fActualRhoLegacy[idx - mithep::PileupEnergyDensity::nAlgos] = value; }); \
      setRho(mithep::PileupEnergyDensity::kHighEta, onfile.fRho); \
      setRho(mithep::PileupEnergyDensity::kLowEta, onfile.fRhoHighEta); \
      setRho(mithep::PileupEnergyDensity::kRandom, onfile.fRhoRandom); \
      setRho(mithep::PileupEnergyDensity::kRandomLowEta, onfile.fRhoRandomLowEta); \
      setRho(mithep::PileupEnergyDensity::kFixedGridAll, onfile.fRhoFixedGridAll); \
      setRho(mithep::PileupEnergyDensity::kFixedGridFastjetAll, onfile.fRhoFixedGridFastjetAll); \
      setLegacy(mithep::PileupEnergyDensity::kKt6CaloJets, onfile.fRhoKt6CaloJets); \
      setLegacy(mithep::PileupEnergyDensity::kKt6CaloJetsCentral, onfile.fRhoKt6CaloJetsCentral); \
      setLegacy(mithep::PileupEnergyDensity::kKt6PFJets, onfile.fRhoKt6PFJets); \
      setLegacy(mithep::PileupEnergyDensity::kKt6PFJetsCentralChargedPileUp, onfile.fRhoKt6PFJetsCentralChargedPileUp); \
      setLegacy(mithep::PileupEnergyDensity::kKt6PFJetsCentralNeutral, onfile.fRhoKt6PFJetsCentralNeutral); \
      setLegacy(mithep::PileupEnergyDensity::kKt6PFJetsCentralNeutralTight, onfile.fRhoKt6PFJetsCentralNeutralTight); }" \

#pragma read \
    sourceClass="mithep::PileupEnergyDensity" \
    version="[5]" \
    source="Double32_t fRho[10];" \
    targetClass="mithep::PileupEnergyDensity" \
    target="fRho, fRhoLegacy" \
    code="{ if (&fRhoLegacy); \
      static const TString bname(Form(\"fRhoLegacy[%d]\", mithep::PileupEnergyDensity::nLegacyAlgos)); \
      static Long_t actualOffset = cls->GetDataMemberOffset(bname); \
      fRhoLegacy_t& fActualRhoLegacy = *(fRhoLegacy_t*)(target + actualOffset); \
      enum OldAlgo { \
        lHighEta, \
        lLowEta, \
        lRandom, \
        lRandomLowEta, \
        lFixedGridAll, \
        lFixedGridFastjetAll, \
        lFixedGridFastjetAllCalo, \
        lFixedGridFastjetCentralCalo, \
        lFixedGridFastjetCentralChargedPileUp, \
        lFixedGridFastjetCentralNeutral \
      }; \
      auto setRho([&fRho, &onfile](UInt_t newIdx, UInt_t oldIdx) { fRho[newIdx] = onfile.fRho[oldIdx]; }); \
      auto setLegacy([&fActualRhoLegacy, &onfile](UInt_t newIdx, UInt_t oldIdx) { fActualRhoLegacy[newIdx - mithep::PileupEnergyDensity::nAlgos] = onfile.fRho[oldIdx]; }); \
      setRho(mithep::PileupEnergyDensity::kFixedGridAll, lFixedGridAll); \
      setRho(mithep::PileupEnergyDensity::kFixedGridFastjetAll, lFixedGridFastjetAll); \
      setRho(mithep::PileupEnergyDensity::kFixedGridFastjetAllCalo, lFixedGridFastjetAllCalo); \
      setRho(mithep::PileupEnergyDensity::kFixedGridFastjetCentralCalo, lFixedGridFastjetCentralCalo); \
      setRho(mithep::PileupEnergyDensity::kFixedGridFastjetCentralChargedPileUp, lFixedGridFastjetCentralChargedPileUp); \
      setRho(mithep::PileupEnergyDensity::kFixedGridFastjetCentralNeutral, lFixedGridFastjetCentralNeutral); \
      setLegacy(mithep::PileupEnergyDensity::kHighEta, lHighEta); \
      setLegacy(mithep::PileupEnergyDensity::kLowEta, lLowEta); \
      setLegacy(mithep::PileupEnergyDensity::kRandom, lRandom); \
      setLegacy(mithep::PileupEnergyDensity::kRandomLowEta, lRandomLowEta); }" \

#pragma link C++ class mithep::PileupEnergyDensity+;
#pragma link C++ enum mithep::PileupEnergyDensity::Algo;
#pragma link C++ class mithep::Collection<mithep::PileupEnergyDensity>+;
#pragma link C++ class mithep::Array<mithep::PileupEnergyDensity>+;
#pragma link C++ class mithep::ObjArray<mithep::PileupEnergyDensity>+;
#pragma link C++ class mithep::Ref<mithep::PileupEnergyDensity>+;
#pragma link C++ typedef mithep::PileupEnergyDensityCol;
#pragma link C++ typedef mithep::PileupEnergyDensityArr;
#endif
