//--------------------------------------------------------------------------------------------------
// XlSubJet
//
// This class holds information about reconstructed sub-jets
//
// Authors: L.DiMatteo, S.Narayanan
//--------------------------------------------------------------------------------------------------

#ifndef MITANA_DATATREE_XLSUBJET_H
#define MITANA_DATATREE_XLSUBJET_H

#include "MitAna/DataTree/interface/Jet.h"
#include "MitAna/DataTree/interface/Types.h"
#include "MitCommon/DataFormats/interface/Vect3.h"

namespace mithep {
  class XlSubJet : public Jet {
  public:
    enum ESubJetType {
      kSoftDrop,
      kPruned,
      kTrimmed,
      kCMSTT,
      kHEPTT,
      kNjettiness,
      nSubJetTypes
    };

    XlSubJet() : Jet() {}
    XlSubJet(Jet const& j) : Jet(j) {}

    EObjType ObjType() const override { return kXlSubJet; }

    Double_t    BTag() const       { return fBTag; }
    Double_t    QGTag() const      { return fQGTag; }
    Double_t    QGPtD() const      { return fQGPtD; }
    Double_t    QGAxis1() const    { return fQGAxis1; }
    Double_t    QGAxis2() const    { return fQGAxis2; }
    Double_t    QGMult() const     { return fQGMult; }
    ESubJetType SubJetType() const { return fSubJetType; }

    Jet* MakeCopy() const override { return new XlSubJet(*this); }

    void SetBTag(Double_t d)          { fBTag = d; }
    void SetQGTag(Double_t d)         { fQGTag = d; }
    void SetQGPtD(Double_t d)         { fQGPtD = d; }
    void SetQGAxis1(Double_t d)       { fQGAxis1 = d; }
    void SetQGAxis2(Double_t d)       { fQGAxis2 = d; }
    void SetQGMult(Double_t d)        { fQGMult = d; }
    void SetSubJetType(ESubJetType t) { fSubJetType = t; }

  protected:
    Double32_t fBTag{0.};       //sub-jet b-tag value
    Double32_t fQGTag{0.};      //sub-jet quark-gluon tag value
    Double32_t fQGPtD{0.};      //QG tagging PtD
    Double32_t fQGAxis1{0.};    //QG tagging Axis1
    Double32_t fQGAxis2{0.};    //QG tagging Axis2
    Double32_t fQGMult{0.};     //QG tagging Multiplicity

    ESubJetType fSubJetType{nSubJetTypes}; //subjet type

    ClassDef(XlSubJet, 2) // XlFatJet class
  };
}

#endif
