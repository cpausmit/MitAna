#ifndef MITANA_DATATREE_MCPARTICLECOLLINKDEF_H
#define MITANA_DATATREE_MCPARTICLECOLLINKDEF_H

#include "MitAna/DataCont/interface/Ref.h"
#include "MitAna/DataCont/interface/RefArray.h"
#include "MitAna/DataTree/interface/MCParticleCol.h"
#endif

#ifndef __CLING__
#   define _R__UNIQUEIDENTIFIER_ MCParticleCol
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

#pragma read \
    sourceClass="mithep::MCParticle" \
    version="[-4]" \
    source="Bool_t fIsSimulated; \
            mithep::BitMask<2> fStatusFlags;" \
    targetClass="mithep::MCParticle" \
    target="fStatusFlags" \
    code="{ fStatusFlags.SetBits(onfile.fStatusFlags.Bits()); \
      fStatusFlags.SetBit(mithep::MCParticle::kIsSimulated, onfile.fIsSimulated); }" \

#pragma link C++ class mithep::Ref<mithep::MCParticle>+;
#pragma link C++ class mithep::MCParticle+;
#pragma link C++ class mithep::Collection<mithep::MCParticle>+;
#pragma link C++ class mithep::Array<mithep::MCParticle>+;
#pragma link C++ class mithep::ObjArray<mithep::MCParticle>+;
#pragma link C++ class mithep::RefArray<mithep::MCParticle>+;
#pragma link C++ typedef mithep::MCParticleCol;
#pragma link C++ typedef mithep::MCParticleArr;
#pragma link C++ typedef mithep::MCParticleOArr;
#pragma link C++ enum mithep::MCParticle::EPartType;
#endif
