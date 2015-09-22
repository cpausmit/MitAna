//--------------------------------------------------------------------------------------------------
// RunLumiListMod
//
// This module is used to extract the run & lumi numbers the module runs over. The output is in the
// form of a tree, which should be merged later by a script.
//
// Authors: Y.Iiyama
//--------------------------------------------------------------------------------------------------

#ifndef MITANA_PHYSICSMOD_RUNLUMILISTMOD_H
#define MITANA_PHYSICSMOD_RUNLUMILISTMOD_H

#include "MitAna/TreeMod/interface/BaseMod.h"

namespace mithep {

  class RunLumiListMod : public BaseMod {
  public:
    RunLumiListMod(char const* name = "RunLumiListMod", char const* title = "RunLumiListMod") : BaseMod(name, title) {}
    ~RunLumiListMod() {}

  private:
    void Process() override;
    void SlaveBegin() override;
    void SlaveTerminate() override;

    TTree* fListTree{};
    UInt_t fRunNumber{};
    UInt_t fLumiNumber{};
    UInt_t fNEvents{};

    ClassDef(RunLumiListMod, 0)
  };

}

#endif
