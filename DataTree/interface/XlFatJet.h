//--------------------------------------------------------------------------------------------------
// XlFatJet
//
// This class holds information about reconstructed jets and their substructure
//
// Authors: L.DiMatteo, S.Narayanan
//--------------------------------------------------------------------------------------------------

#ifndef MITANA_DATATREE_XLFATJET_H
#define MITANA_DATATREE_XLFATJET_H

#include "MitAna/DataTree/interface/FatJet.h"

namespace mithep {

  class XlFatJet : public FatJet {
  public:

    XlFatJet() : FatJet() {}
    XlFatJet(FatJet const& f) : FatJet(f) {}
    ~XlFatJet() {}

    EObjType ObjType() const override { return kXlFatJet; }

    Double_t QGTag() const        { return fQGTag; }
    Double_t C2b0() const         { return fC2b0; }
    Double_t C2b0p2() const       { return fC2b0p2; }
    Double_t C2b0p5() const       { return fC2b0p5; }
    Double_t C2b1() const         { return fC2b1; }
    Double_t C2b2() const         { return fC2b2; }
    Double_t MassSDb0() const     { return fMassSDb0; }
    Double_t MassSDb1() const     { return fMassSDb1; }
    Double_t MassSDb2() const     { return fMassSDb2; }
    Double_t MassSDbm1() const    { return fMassSDbm1; }
    Double_t MassPruned() const   { return fMassPruned; }
    Double_t MassFiltered() const { return fMassFiltered; }
    Double_t MassTrimmed() const  { return fMassTrimmed; }
    Double_t Pull() const         { return fPull; }
    Double_t PullAngle() const    { return fPullAngle; }
    Double_t chi() const          { return fChi; }
    Int_t    nMicrojets() const   { return fNMicrojets; }
    Double_t Tau1() const                  { return fTau1; }
    Double_t Tau2() const                  { return fTau2; }
    Double_t Tau3() const                  { return fTau3; }
    Double_t Tau4() const                  { return fTau4; }
    Double_t QJetVol() const               { return fQJetVol; }
    Vect4M  const& PrunedP() const      { return fPrunedP; }
    Vect4M  const& TrimmedP() const      { return fTrimmedP; }
    Vect4M  const& SoftDropP() const      { return fSoftDropP; }

    Bool_t   HasSubJet(XlSubJet const* s, XlSubJet::ESubJetType t) const
    { return fSubJets[t].HasObject(s); }

    RefArray<XlSubJet> const& GetSubJets(XlSubJet::ESubJetType t) const { return fSubJets[t]; }

    Jet* MakeCopy() const override { return new XlFatJet(*this); }
    void Mark(UInt_t i=1) const override;

    void SetQGTag(Double_t t)        { fQGTag = t; }
    void SetC2b0(Double_t t)         { fC2b0 = t; }
    void SetC2b0p2(Double_t t)       { fC2b0p2 = t; }
    void SetC2b0p5(Double_t t)       { fC2b0p5 = t; }
    void SetC2b1(Double_t t)         { fC2b1 = t; }
    void SetC2b2(Double_t t)         { fC2b2 = t; }
    void SetMassSDb0(Double_t t)     { fMassSDb0 = t; }
    void SetMassSDb1(Double_t t)     { fMassSDb1 = t; }
    void SetMassSDb2(Double_t t)     { fMassSDb2 = t; }
    void SetMassSDbm1(Double_t t)    { fMassSDbm1 = t; }
    void SetMassPruned(Double_t t)   { fMassPruned = t; }
    void SetMassFiltered(Double_t t) { fMassFiltered = t; }
    void SetMassTrimmed(Double_t t)  { fMassTrimmed = t; }
    void SetPull(Double_t t)         { fPull = t; }
    void SetPullAngle(Double_t t)    { fPullAngle = t; }
    void SetChi(Double_t t)          { fChi = t; }
    void SetNMicrojets(Int_t t)      { fNMicrojets = t; }
    void SetTau1(float t)            { fTau1        = t; }
    void SetTau2(float t)            { fTau2        = t; }
    void SetTau3(float t)            { fTau3        = t; }
    void SetTau4(float t)            { fTau4        = t; }
    void SetQJetVol(float t)         { fQJetVol     = t; }
    void SetPrunedP(Vect4M p)           { fPrunedP = p; }
    void SetTrimmedP(Vect4M p)          { fTrimmedP = p; }
    void SetTrimmedP(Vect4M p)          { fSoftDropP = p; }
    void AddSubJet(XlSubJet const*, XlSubJet::ESubJetType);
    void AddSubJet(XlSubJet const*);

  protected:
    Double32_t fQGTag{0.};        //QG tagging
    Double32_t fC2b0{0.};         //ECF ratio order 2, beta 0
    Double32_t fC2b0p2{0.};       //ECF ratio order 2, beta 0.2
    Double32_t fC2b0p5{0.};       //ECF ratio order 2, beta 0.2
    Double32_t fC2b1{0.};         //ECF ratio order 2, beta 1
    Double32_t fC2b2{0.};         //ECF ratio order 2, beta 2
    Double32_t fMassSDb0{0.};     //Groomed mass (soft drop b 0)
    Double32_t fMassSDb1{0.};     //Groomed mass (soft drop b 1)
    Double32_t fMassSDb2{0.};     //Groomed mass (soft drop b 2)
    Double32_t fMassSDbm1{0.};    //Groomed mass (soft drop b-1)
    Double32_t fMassPruned{0.};   //Groomed mass (pruning)
    Double32_t fMassFiltered{0.}; //Groomed mass (filtering)
    Double32_t fMassTrimmed{0.};  //Groomed mass (trimming)
    Double32_t fPull{0.};         //Color pull
    Double32_t fPullAngle{0.};    //Angle between pulls of lead/subleading subjets:
    //either choose 2-prong or 3-prong subclustering!
    Double32_t fChi{-999.};          // shower deconstruction probability
    Int_t      fNMicrojets{0};
    Double32_t         fTau1{-1.};         //1-subjettiness
    Double32_t         fTau2{-1.};         //2-subjettiness
    Double32_t         fTau3{-1.};         //3-subjettiness
    Double32_t         fTau4{-1.};         //4-subjettiness
    Double32_t         fQJetVol{0.};      //QJets volatility
    Vect4M             fPrunedP{};
    Vect4M             fTrimmedP{};
    Vect4M             fSoftDropP{};

    RefArray<XlSubJet> fSubJets[XlSubJet::nSubJetTypes];      //sub jets in the jet

    ClassDef(XlFatJet, 6) // XlFatJet class
  };

}

inline
void
mithep::XlFatJet::AddSubJet(XlSubJet const* subjet, XlSubJet::ESubJetType type)
{
  if (type == XlSubJet::nSubJetTypes)
    fSubJets[subjet->SubJetType()].Add(subjet);
  else
    fSubJets[type].Add(subjet);
}

inline
void
mithep::XlFatJet::AddSubJet(XlSubJet const* subjet)
{
  AddSubJet(subjet,subjet->SubJetType);
}

//--------------------------------------------------------------------------------------------------
inline
void
mithep::XlFatJet::Mark(UInt_t ib) const
{
  // mark myself
  mithep::DataObject::Mark(ib);
  // mark my dependencies if they are there
  for (auto& subjet : fSubJets)
    subjet.Mark(ib);
}

#endif
