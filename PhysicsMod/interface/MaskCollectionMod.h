#ifndef MITPHYSICS_MODS_MASKCOLLECTIONMOD_H
#define MITPHYSICS_MODS_MASKCOLLECTIONMOD_H

#include "MitAna/TreeMod/interface/BaseMod.h"
#include "MitAna/DataTree/interface/ParticleFwd.h"

#include "TString.h"

namespace mithep {

  class MaskCollectionMod : public BaseMod {
  public:
    MaskCollectionMod(char const* name = "MaskCollectionMod", char const* title = "Mask collections") : BaseMod(name, title) {}
    ~MaskCollectionMod() {}

    char const* GetInputName() const { return fInputName; }
    char const* GetMaskName() const { return fMaskName; }
    char const* GetOutputName() const { return fOutput.GetName(); }

    void SetInputName(char const* name) { fInputName = name; }
    void SetMaskName(char const* name) { fMaskName = name; }
    void SetOutputName(char const* name) { fOutput.SetName(name); }

  private:
    void Process() override;
    void SlaveBegin() override;
    void SlaveTerminate() override;

    TString fInputName{};
    TString fMaskName{};
    ParticleOArr fOutput{};

    ClassDef(MaskCollectionMod, 0)
  };

}

#endif
