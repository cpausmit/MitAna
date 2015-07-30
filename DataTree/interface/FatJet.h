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
      float length;
      float dist;
      float dxy;
      float dz;
      float IP2D;
      float IP2Dsig;
      float IP;
      float IPsig;
      float IP2Derr;
      float IPerr;
      float prob;
      float pt;
      float eta;
      float phi;
      float PVWeight;
      float SVWeight;
      int PV;
      int fromSV;
      int SV;
    };

    struct SVData { // per SV
      float flight;
      float flightErr;
      float deltaRJet;
      float deltaRSumJet;
      float deltaRSumDir;
      float flight2D;
      float flight2DErr;
      float totCharge;
      float vtxDistJetAxis;
      int nTrk;
      float mass;
      float energyRatio;
      float pt;
      float eta;
      float phi;
      float dirX;
      float dirY;
      float dirZ;
    };

    struct LeptonData {
      float pt;
      float eta;
      float phi;
      float ptRel;
      float ratio;
      float ratioRel;
      float IP;
      float IP2D;
    };

    FatJet() : PFJet() {}
    FatJet(PFJet const& p) : PFJet(p) {}
    ~FatJet() {}

    EObjType ObjType() const override { return kFatJet; }

    float Charge() const                { return fCharge; }
    float Tau1IVF() const               { return fTau1IVF; }
    float Tau2IVF() const               { return fTau2IVF; }
    float Tau3IVF() const               { return fTau3IVF; }
    Vect3    GetTauIVFAxis(UInt_t i) const { return fTauIVFAxes.at(i); }
    float tauDot() const                { return fTauDot; }
    float zRatio() const                { return fZRatio; }


    Jet* MakeCopy() const override { return new FatJet(*this); }

    std::vector<TrackData> const&  GetTrackData() const    { return fTrackData; }
    std::vector<SVData> const&     GetSVData() const       { return fSVData; }
    std::vector<LeptonData> const& GetMuonData() const     { return fMuonData; }
    std::vector<LeptonData> const& GetElectronData() const { return fElectronData; }
    std::vector<float> const& GetSubJetBtags() const       { return fSubjetBtags; }


    void SetCharge()                    { fCharge  = this->GetCharge(); }
    void SetTau1IVF(float t)         { fTau1IVF        = t; }
    void SetTau2IVF(float t)         { fTau2IVF        = t; }
    void SetTau3IVF(float t)         { fTau3IVF        = t; }
    void AddTauIVFAxis(Vect3 p)         { fTauIVFAxes.push_back(p); }

    void SetTauDot(float t)          { fTauDot = t; }
    void SetZRatio(float t)          { fZRatio = t; }
    void AddTrackData(TrackData *t)     { fTrackData.push_back(*t); }
    void AddSVData(SVData *s)           { fSVData.push_back(*s); }
    void AddMuonData(LeptonData *s)     { fMuonData.push_back(*s); }
    void AddElectronData(LeptonData *s) { fElectronData.push_back(*s); }
    void AddSubJetBtag(float btag) { fSubjetBtags.push_back(btag); }


  protected:
    Double_t GetCharge() const override;

    float         fCharge{0.};       //Pt-weighted jet charge
    float         fTau1IVF{-1.};         //1-subjettiness
    float         fTau2IVF{-1.};         //2-subjettiness
    float         fTau3IVF{-1.};         //3-subjettiness
    std::vector<Vect3> fTauIVFAxes{};

    std::vector<float> fSubjetBtags;


    // IVF variables
    float         fTauDot{-1.};
    float         fZRatio{-1.};
    std::vector<TrackData>  fTrackData{};
    std::vector<SVData>     fSVData{};
    std::vector<LeptonData> fMuonData{};
    std::vector<LeptonData> fElectronData{};

    ClassDef(FatJet, 2) // FatJet class
  };

}



//--------------------------------------------------------------------------------------------------
inline
Double_t
mithep::FatJet::GetCharge() const
{
  // Return the sum of constituents PFCandidates weighted by their pt

  float charge = 0;
  for (UInt_t i = 0; i< NPFCands(); ++i)
    charge += PFCand(i)->Charge() * PFCand(i)->Pt();

  return charge / Pt();
}



#endif
