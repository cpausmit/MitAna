#ifndef MITMONOJET_DATATREE_LINKDEF_H
#define MITMONOJET_DATATREE_LINKDEF_H

#include "MitMonoJet/DataTree/interface/XlEvtSelData.h"
#endif

#ifndef __CINT__
# define _R__UNIQUEIDENTIFIER_ MitMonoJetDataTree
# define _R__JOIN3_(F,X,Y) _NAME3_(F,X,Y)
# undef _R__UNIQUE_
# define _R__UNIQUE_(X) _R__JOIN3_( _R__UNIQUEIDENTIFIER_,X,__LINE__)
#endif

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ nestedclass;
#pragma link C++ nestedtypedef;
#pragma link C++ namespace mithep;

#pragma link C++ class mithep::XlEvtSelData+;
#endif
