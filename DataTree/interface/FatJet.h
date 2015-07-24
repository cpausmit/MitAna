//--------------------------------------------------------------------------------------------------
// FatJet
//
// This class holds information about reconstructed jets and their substructure
//
// Authors: L.DiMatteo, S.Narayanan
//--------------------------------------------------------------------------------------------------

#ifndef MITANA_DATATREE_FATJET_H
#define MITANA_DATATREE_FATJET_H

#include "MitAna/DataTree/interface/PFJet.h"
#include "MitAna/DataCont/interface/RefArray.h"
#include "MitAna/DataTree/interface/Types.h"
#include "MitCommon/DataFormats/interface/Vect4M.h"
#include "MitAna/DataTree/interface/XlSubJet.h"

namespace mithep {

  class FatJet : public PFJet {
  public:
    struct TrackData { // per track
      double length;
      double dist;
      double dxy;
      double dz;
      double IP2D;
      double IP2Dsig;
      double IP;
      double IPsig;
      double IP2Derr;
      double IPerr;
      double prob;
      double pt;
      double eta;
      double phi;
      double PVWeight;
      double SVWeight;
      int PV;
      int fromSV;
      int SV;
    };

    struct SVData { // per SV
      double flight;
      double flightErr;
      double deltaRJet;
      double deltaRSumJet;
      double deltaRSumDir;
      double flight2D;
      double flight2DErr;
      double totCharge;
      double vtxDistJetAxis;
      int nTrk;
      double mass;
      double energyRatio;
      double pt;
      double eta;
      double phi;
      double dirX;
      double dirY;
      double dirZ;
    };

    struct LeptonData {
      double pt;
      double eta;
      double phi;
      double ptRel;
      double ratio;
      double ratioRel;
      double IP;
      double IP2D;
    };

    FatJet() : PFJet() {}
    FatJet(PFJet const& p) : PFJet(p) {}
    ~FatJet() {}

    EObjType ObjType() const override { return kFatJet; }

    Double_t Charge() const                { return fCharge; }
    Double_t Tau1IVF() const               { return fTau1IVF; }
    Double_t Tau2IVF() const               { return fTau2IVF; }
    Double_t Tau3IVF() const               { return fTau3IVF; }
    Vect3    GetTauIVFAxis(UInt_t i) const { return fTauIVFAxes.at(i); }
    Double_t Tau1() const                  { return fTau1; }
    Double_t Tau2() const                  { return fTau2; }
    Double_t Tau3() const                  { return fTau3; }
    Double_t Tau4() const                  { return fTau4; }
    Double_t QJetVol() const               { return fQJetVol; }
    Double_t tauDot() const                { return fTauDot; }
    Double_t zRatio() const                { return fZRatio; }

    Bool_t   HasSubJet(XlSubJet const* s, XlSubJet::ESubJetType t) const
    { return fSubJets[t].HasObject(s); }

    Jet* MakeCopy() const override { return new FatJet(*this); }

    std::vector<TrackData> const&  GetTrackData() const    { return fTrackData; }
    std::vector<SVData> const&     GetSVData() const       { return fSVData; }
    std::vector<LeptonData> const& GetMuonData() const     { return fMuonData; }
    std::vector<LeptonData> const& GetElectronData() const { return fElectronData; }

    RefArray<XlSubJet> const& GetSubJets(XlSubJet::ESubJetType t) const { return fSubJets[t]; }

    void SetCharge()                    { fCharge  = this->GetCharge(); }
    void SetTau1IVF(Double_t t)         { fTau1IVF        = t; }
    void SetTau2IVF(Double_t t)         { fTau2IVF        = t; }
    void SetTau3IVF(Double_t t)         { fTau3IVF        = t; }
    void AddTauIVFAxis(Vect3 p)         { fTauIVFAxes.push_back(p); }
    void SetTau1(Double_t t)            { fTau1        = t; }
    void SetTau2(Double_t t)            { fTau2        = t; }
    void SetTau3(Double_t t)            { fTau3        = t; }
    void SetTau4(Double_t t)            { fTau4        = t; }
    void SetQJetVol(Double_t t)         { fQJetVol     = t; }
    void SetTauDot(Double_t t)          { fTauDot = t; }
    void SetZRatio(Double_t t)          { fZRatio = t; }
    void AddTrackData(TrackData *t)     { fTrackData.push_back(*t); }
    void AddSVData(SVData *s)           { fSVData.push_back(*s); }
    void AddMuonData(LeptonData *s)     { fMuonData.push_back(*s); }
    void AddElectronData(LeptonData *s) { fElectronData.push_back(*s); }
    void SetPrunedP(Vect4M p)           { fPrunedP = p; }
    void SetTrimmedP(Vect4M p)          { fTrimmedP = p; }
    void AddSubJet(XlSubJet const*, XlSubJet::ESubJetType = XlSubJet::nSubJetTypes);

    void Mark(UInt_t i=1) const override;

  protected:
    Double_t GetCharge() const override;

    Double32_t         fCharge{0.};       //Pt-weighted jet charge
    Double32_t         fTau1IVF{-1.};         //1-subjettiness
    Double32_t         fTau2IVF{-1.};         //2-subjettiness
    Double32_t         fTau3IVF{-1.};         //3-subjettiness
    std::vector<Vect3> fTauIVFAxes{};
    Double32_t         fTau1{-1.};         //1-subjettiness
    Double32_t         fTau2{-1.};         //2-subjettiness
    Double32_t         fTau3{-1.};         //3-subjettiness
    Double32_t         fTau4{-1.};         //4-subjettiness
    Double32_t         fQJetVol{0.};      //QJets volatility
    Double32_t         fCh{-999.};          // shower deconstruction probability
    Vect4M             fPrunedP{};
    Vect4M             fTrimmedP{};

    RefArray<XlSubJet> fSubJets[XlSubJet::nSubJetTypes];      //sub jets in the jet

    // IVF variables
    Double32_t         fTauDot{-1.};
    Double32_t         fZRatio{-1.};
    std::vector<TrackData>  fTrackData{};
    std::vector<SVData>     fSVData{};
    std::vector<LeptonData> fMuonData{};
    std::vector<LeptonData> fElectronData{};

    ClassDef(FatJet, 1) // FatJet class
  };

}

//--------------------------------------------------------------------------------------------------
inline
void
mithep::FatJet::Mark(UInt_t ib) const
{
  // mark myself
  mithep::DataObject::Mark(ib);
  // mark my dependencies if they are there
  for (auto& subjet : fSubJets)
    subjet.Mark(ib);
}

//--------------------------------------------------------------------------------------------------
inline
Double_t
mithep::FatJet::GetCharge() const
{
  // Return the sum of constituents PFCandidates weighted by their pt

  Double_t charge = 0;
  for (UInt_t i = 0; i< NPFCands(); ++i)
    charge += PFCand(i)->Charge() * PFCand(i)->Pt();

  return charge / Pt();
}

inline
void
mithep::FatJet::AddSubJet(XlSubJet const* subjet, XlSubJet::ESubJetType type/* = XlSubJet::nSubJetTypes*/)
{
  if (type == XlSubJet::nSubJetTypes)
    fSubJets[subjet->SubJetType()].Add(subjet);
  else
    fSubJets[type].Add(subjet);
}

#endif
