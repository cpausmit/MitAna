// $Id: MitAnaDataTreeLinkDef.h,v 1.1 2008/05/27 19:24:49 loizides Exp $

#ifndef DATATREE_LINKDEF_H
#define DATATREE_LINKDEF_H

#include <vector>
#include "MitAna/DataTree/interface/Types.h"
#include "MitAna/DataTree/interface/Names.h"
#include "MitAna/DataTree/interface/Collections.h"
#include "MitAna/DataTree/interface/Vector.h"
#include "MitAna/DataTree/interface/Particle.h"
#endif

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ nestedclass;
#pragma link C++ nestedtypedef;
#pragma link C++ namespace mithep;

#pragma link C++ class mithep::Names+;

#pragma link C++ class mithep::Particle+;
#pragma link C++ class std::vector<mithep::Particle>+;
#pragma link C++ class mithep::Collection<mithep::Particle>+;
#pragma link C++ class mithep::Vector<mithep::Particle>+;
#pragma link C++ typedef mithep::ParticleCollection;

#endif