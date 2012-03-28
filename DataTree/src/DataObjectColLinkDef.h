// $Id: DataObjectColLinkDef.h,v 1.2 2011/03/11 04:03:00 bendavid Exp $

#ifndef MITANA_DATATREE_DATAOBJECTCOLLINKDEF_H
#define MITANA_DATATREE_DATAOBJECTCOLLINKDEF_H

#include "MitAna/DataCont/interface/Ref.h"
#include "MitAna/DataTree/interface/DataObjectCol.h"
#endif

#ifndef __CINT__
#   define _R__UNIQUEIDENTIFIER_ DataObjectCol
#   define _R__JOIN3_(F,X,Y) _NAME3_(F,X,Y)
#   undef _R__UNIQUE_
#   define _R__UNIQUE_(X) _R__JOIN3_( _R__UNIQUEIDENTIFIER_,X,__LINE__)
#endif

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ nestedclass;
#pragma link C++ nestedtypedef;
#pragma link C++ namespace mithep;
#pragma read                                               \
     sourceClass="mithep::DataObject"                      \
     version="[-2]"                                        \
     source="dummysource"                                  \
     targetClass="mithep::DataObject"                      \
     target="fMarker"                                      \
  code="{ fMarker = 0; }"   \

#pragma link C++ typedef mithep::DataObjectCol;
#pragma link C++ typedef mithep::DataObjectArr;
#pragma link C++ typedef mithep::DataObjectOArr;
#pragma link C++ class mithep::DataObject+;
#pragma link C++ class mithep::Collection<mithep::DataObject>+;
#pragma link C++ class mithep::Array<mithep::DataObject>+;
#pragma link C++ class mithep::ObjArray<mithep::DataObject>+;
#pragma link C++ class mithep::Ref<mithep::DataObject>+;
#pragma link C++ typedef mithep::DataObjectCol;
#pragma link C++ typedef mithep::DataObjectArr;
#pragma link C++ typedef mithep::DataObjectOArr;
#endif
