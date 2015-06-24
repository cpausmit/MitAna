#ifndef MITANA_TREEMOD_LINKDEF_H
#define MITANA_TREEMOD_LINKDEF_H
#include "MitAna/TreeMod/interface/AnaFwkMod.h"
#include "MitAna/TreeMod/interface/Analysis.h"
#include "MitAna/TreeMod/interface/BaseMod.h"
#include "MitAna/TreeMod/interface/BaseSelMod.h"
#include "MitAna/TreeMod/interface/CatalogingMod.h"
#include "MitAna/TreeMod/interface/HLTFwkMod.h"
#include "MitAna/TreeMod/interface/MCFwkMod.h"
#include "MitAna/TreeMod/interface/HLTMod.h"
#include "MitAna/TreeMod/interface/L1Mod.h"
#include "MitAna/TreeMod/interface/OutputMod.h"
#include "MitAna/TreeMod/interface/Selector.h"
#include "MitAna/TreeMod/interface/TreeBranchLoader.h"
#include "MitAna/TreeMod/interface/TreeLoader.h"
#endif

#ifdef __CLING__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ nestedclass;
#pragma link C++ nestedtypedef;
#pragma link C++ namespace mithep;

#pragma link C++ class mithep::AnaFwkMod+;
#pragma link C++ class mithep::Analysis+;
#pragma link C++ class mithep::BaseMod+;
#pragma link C++ class mithep::BaseSelMod+;
#pragma link C++ class mithep::CatalogingMod+;
#pragma link C++ class mithep::HLTFwkMod+;
#pragma link C++ class mithep::MCFwkMod+;
#pragma link C++ class mithep::HLTMod+;
#pragma link C++ class mithep::L1Mod+;
#pragma link C++ class mithep::OutputMod+;
#pragma link C++ class mithep::Selector+;
#pragma link C++ class mithep::TreeBranchLoader+;
#pragma link C++ class mithep::TreeLoader+;
#endif
