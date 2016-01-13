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
      Float_t length{};
      Float_t dist{};
      Float_t dxy{};
      Float_t dz{};
      Float_t IP2D{};
      Float_t IP2Dsig{};
      Float_t IP{};
      Float_t IPsig{};
      Float_t IP2Derr{};
      Float_t IPerr{};
      Float_t prob{};
      Float_t pt{};
      Float_t eta{};
      Float_t phi{};
      Float_t PVWeight{};
      Float_t SVWeight{};
      Int_t PV{};
      Int_t fromSV{};
      Int_t SV{};

      TrackData() {}
      virtual ~TrackData() {}

      ClassDef(TrackData, 1)
    };

    struct SVData { // per SV
      Float_t flight{};
      Float_t flightErr{};
      Float_t deltaRJet{};
      Float_t deltaRSumJet{};
      Float_t deltaRSumDir{};
      Float_t flight2D{};
      Float_t flight2DErr{};
      Float_t totCharge{};
      Float_t vtxDistJetAxis{};
      Int_t nTrk{};
      Float_t mass{};
      Float_t energyRatio{};
      Float_t pt{};
      Float_t eta{};
      Float_t phi{};
      Float_t dirX{};
      Float_t dirY{};
      Float_t dirZ{};

      SVData() {}
      virtual ~SVData() {}

      ClassDef(SVData, 1)
    };

    struct LeptonData {
      Float_t pt{};
      Float_t eta{};
      Float_t phi{};
      Float_t ptRel{};
      Float_t ratio{};
      Float_t ratioRel{};
      Float_t IP{};
      Float_t IP2D{};

      LeptonData() {}
      virtual ~LeptonData() {}

      ClassDef(LeptonData, 1)
    };

    FatJet() {}
    FatJet(PFJet const& p) : PFJet(p) {}
    ~FatJet() {}

    EObjType ObjType() const override { return kFatJet; }

    Float_t Charge() const                { return fCharge; }
    Float_t Tau1IVF() const               { return fTau1IVF; }
    Float_t Tau2IVF() const               { return fTau2IVF; }
    Float_t Tau3IVF() const               { return fTau3IVF; }
    Float_t tauDot() const                { return fTauDot; }
    Float_t zRatio() const                { return fZRatio; }
    Vect3 const& GetTauIVFAxis(UInt_t i) const { return fTauIVFAxes.at(i); }
    Double_t Tau1() const                  { return fTau1; }
    Double_t Tau2() const                  { return fTau2; }
    Double_t Tau3() const                  { return fTau3; }
    Double_t Tau4() const                  { return fTau4; }
    Double_t MassSoftDrop() const          { return fMassSD; }

    Jet* MakeCopy() const override { return new FatJet(*this); }

    std::vector<TrackData> const&  GetTrackData() const    { return fTrackData; }
    std::vector<SVData> const&     GetSVData() const       { return fSVData; }
    std::vector<LeptonData> const& GetMuonData() const     { return fMuonData; }
    std::vector<LeptonData> const& GetElectronData() const { return fElectronData; }
    std::vector<Float_t> const& GetSubJetBtags() const     { return fSubjetBtags; }

    void SetCharge()                    { fCharge  = this->GetCharge(); }
    void SetTau1IVF(Float_t t)         { fTau1IVF        = t; }
    void SetTau2IVF(Float_t t)         { fTau2IVF        = t; }
    void SetTau3IVF(Float_t t)         { fTau3IVF        = t; }
    void AddTauIVFAxis(Vect3 p)         { fTauIVFAxes.push_back(p); }
    void SetTau1(float t)            { fTau1        = t; }
    void SetTau2(float t)            { fTau2        = t; }
    void SetTau3(float t)            { fTau3        = t; }
    void SetTau4(float t)            { fTau4        = t; }
    void SetMassSoftDrop(float t)    { fMassSD      = t; }
    void SetTauDot(Float_t t)          { fTauDot = t; }
    void SetZRatio(Float_t t)          { fZRatio = t; }
    void AddTrackData(TrackData *t)     { fTrackData.push_back(*t); }
    void AddSVData(SVData *s)           { fSVData.push_back(*s); }
    void AddMuonData(LeptonData *s)     { fMuonData.push_back(*s); }
    void AddElectronData(LeptonData *s) { fElectronData.push_back(*s); }
    void AddSubJetBtag(Float_t btag) { fSubjetBtags.push_back(btag); }


  protected:
    Double_t GetCharge() const override;

    Float_t         fCharge{0.};       //Pt-weighted jet charge
    Float_t         fTau1IVF{-1.};         //1-subjettiness
    Float_t         fTau2IVF{-1.};         //2-subjettiness
    Float_t         fTau3IVF{-1.};         //3-subjettiness
    Double32_t         fTau1{-1.};         //1-subjettiness
    Double32_t         fTau2{-1.};         //2-subjettiness
    Double32_t         fTau3{-1.};         //3-subjettiness
    Double32_t         fTau4{-1.};         //4-subjettiness
    Double32_t         fMassSD{-1.};       // soft drop mass
    std::vector<Vect3> fTauIVFAxes{};

    std::vector<Float_t> fSubjetBtags; // ordered by decreasing subjet pT


    // IVF variables
    Float_t         fTauDot{-1.};
    Float_t         fZRatio{-1.};
    std::vector<TrackData>  fTrackData{};
    std::vector<SVData>     fSVData{};
    std::vector<LeptonData> fMuonData{};
    std::vector<LeptonData> fElectronData{};

    ClassDef(FatJet, 3) // FatJet class
  };

}

//--------------------------------------------------------------------------------------------------
inline
Double_t
mithep::FatJet::GetCharge() const
{
  // Return the sum of constituents PFCandidates weighted by their pt

  Float_t charge = 0;
  for (UInt_t i = 0; i< NPFCands(); ++i)
    charge += PFCand(i)->Charge() * PFCand(i)->Pt();

  return charge / Pt();
}

#endif
