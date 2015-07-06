#include "MitAna/DataTree/interface/Names.h"
#include "MitAna/TreeFiller/interface/FillerXlFatJets.h"
#include "MitAna/DataTree/interface/PFJetCol.h"

#include "MitAna/DataTree/interface/XlSubJet.h"
#include "MitAna/DataTree/interface/XlFatJet.h"
#include "MitCommon/DataFormats/interface/Vect4M.h"
#include "MitCommon/DataFormats/interface/Vect3.h"
#include "MitCommon/DataFormats/interface/Types.h"
#include "MitCommon/MathTools/interface/MathUtils.h"
#include "MitAna/PhysicsUtils/interface/CMSTopTagger.h"
#include "QjetsPlugin.h"
#include "Qjets.h"

#include "fastjet/contrib/Njettiness.hh"
#include "fastjet/contrib/EnergyCorrelator.hh"
#include "fastjet/contrib/Nsubjettiness.hh"
#include "fastjet/contrib/NjettinessPlugin.hh"
#include "fastjet/contrib/SoftDrop.hh"

using namespace mithep;

ClassImp(mithep::FillerXlFatJets)

//--------------------------------------------------------------------------------------------------
FillerXlFatJets::FillerXlFatJets(const char *name, const char *title) :
  BaseMod (name,title),
  fIsData (kTRUE),
  fFillVSubJets (kTRUE),
  fFillTopSubJets (kFALSE),
  fNSubDeclustering (kFALSE),
  fBTaggingActive (kFALSE),
  fQGTaggingActive (kTRUE),
  fTopTaggingActive (kFALSE),
  fQGTaggerCHS (kFALSE),
  fPublishOutput (kTRUE),
  fProcessNJets (2),
  fJetsName (Names::gkPFJetBrn),
  fJetsFromBranch (kTRUE),
  fJets (0),
  fPfCandidatesName (Names::gkPFCandidatesBrn),
  fPfCandidatesFromBranch(kTRUE),
  fPfCandidates (0),
  fPileUpDenName(Names::gkPileupEnergyDensityBrn),
  fPileUpDenFromBranch(kTRUE),
  fPileUpDen(0),
  //fVertexesName (ModNames::gkGoodVertexesName),
  fVertexesName ("GoodVertexes"), // hack
  fVertexesFromBranch(kFALSE),
  fVertexes(0),
  fSecondaryVertexesName ("InclusiveSecondaryVertexes"),
  fSecondaryVertexesFromBranch(kFALSE),
  fSecondaryVertexes(0),
  fXlFatJetsName ("XlFatJets"),
  fXlSubJetsName ("XlSubJets"),
  fSoftDropZCut (0.1),
  fSoftDropR0 (1.),
  fPruneZCut (0.1),
  fPruneDistCut (0.5),
  fFilterN (3),
  fFilterRad (0.2),
  fTrimRad (0.2),
  fTrimPtFrac (0.05),
  fConeSize (0.6),
  fCounter (0)
{
  // Constructor.
}

FillerXlFatJets::~FillerXlFatJets()
{
  // Destructor
  if (fXlSubJets)
    delete fXlSubJets;
  if (fXlFatJets)
    delete fXlFatJets;

  delete fPruner;
  delete fFilterer;
  delete fTrimmer ;

  delete fCAJetDef;

  delete fActiveArea;
  delete fAreaDefinition;

  delete fQGTagger;
}

//--------------------------------------------------------------------------------------------------
void FillerXlFatJets::Process()
{
  // make sure the out collections are empty before starting
  fXlFatJets->Delete();
  fXlSubJets->Delete();

  // Load the branches we want to work with
  // LoadEventObject(fJetsName,fJets,fJetsFromBranch);
  // if (fQGTaggingActive) {
  //   LoadEventObject(fPileUpDenName,fPileUpDen,fPileUpDenFromBranch);
  //   LoadEventObject(fVertexesName,fVertexes,fVertexesFromBranch);
  // }

  fJets = GetObject<JetOArr>(fJetsName);
  if (fQGTaggingActive){
    fPileUpDen = GetObject<PileupEnergyDensityCol>(fPileUpDenName);
    fVertexes = GetObject<VertexCol>(fVertexesName);
  }

  fSecondaryVertexes = GetObject<VertexCol>(fSecondaryVertexesName);

  // Loop over PFCandidates and unmark them : necessary for skimming
  for (UInt_t i=0; i<fPfCandidates->GetEntries(); ++i)
    fPfCandidates->At(i)->UnmarkMe();

  // Setup pileup density for QG computation
  if (fQGTaggingActive)
    fQGTagger->SetRhoIso(fPileUpDen->At(0)->RhoRandomLowEta());

  // Loop over jets
  for (UInt_t i=0; i<fJets->GetEntries(); ++i) {

    // consider only the first fProcessNJets jets
    if (i >= fProcessNJets)
      break;

    const PFJet *jet = dynamic_cast<const PFJet*>(fJets->At(i));
    if (! jet) {
      printf(" FillerXlFatJets::Process() - ERROR - jets provided are not PFJets.");
      break;
    }

    // mark jet (and consequently its consituents) for further use in skim
    jet->Mark();

    // perform Nsubjettiness analysis and fill the extended XlFatJet object
    // this method will also fill the SubJet collection
    FillXlFatJet(jet);

  }

  return;
}

//--------------------------------------------------------------------------------------------------
void FillerXlFatJets::SlaveBegin()
{
  // Run startup code on the computer (slave) doing the actual analysis.
  ReqEventObject(fJetsName,fJets,fJetsFromBranch);
  ReqEventObject(fPfCandidatesName,fPfCandidates,fPfCandidatesFromBranch);
  ReqEventObject(fPileUpDenName,fPileUpDen,fPileUpDenFromBranch);
  ReqEventObject(fVertexesName,fVertexes,fVertexesFromBranch);

  // Initialize area caculation (done with ghost particles)

  // Create the new output collection
  fXlFatJets = new XlFatJetArr(16,fXlFatJetsName);
  fXlSubJets = new XlSubJetArr(16,fXlSubJetsName);
  // Publish collection for further usage in the analysis
  if (fPublishOutput) {
    PublishObj(fXlFatJets);
    PublishObj(fXlSubJets);
  }

  // Prepare pruner
  fPruner = new fastjet::Pruner(fastjet::cambridge_algorithm,fPruneZCut,fPruneDistCut);
  // Prepare filterer
  fFilterer = new fastjet::Filter(fastjet::JetDefinition(fastjet::cambridge_algorithm,fFilterRad),
                                  fastjet::SelectorNHardest(fFilterN));
  // Prepare trimmer
  fTrimmer = new fastjet::Filter(fastjet::Filter(fastjet::JetDefinition(fastjet::kt_algorithm,fTrimRad),
                                 fastjet::SelectorPtFractionMin(fTrimPtFrac)));

  // CA constructor (fConeSize = 0.6 for antiKt) - reproducing paper 1: http://arxiv.org/abs/1011.2268
  fCAJetDef = new fastjet::JetDefinition(fastjet::cambridge_algorithm, fConeSize);

  // Initialize area caculation (done with ghost particles)
  int activeAreaRepeats = 1;
  double ghostArea = 0.01;
  double ghostEtaMax = 7.0;
  fActiveArea = new fastjet::GhostedAreaSpec(ghostEtaMax,activeAreaRepeats,ghostArea);
  fAreaDefinition = new fastjet::AreaDefinition(fastjet::active_area_explicit_ghosts,*fActiveArea);

  // Initialize QGTagger class
  fQGTagger = new QGTagger(fQGTaggerCHS);

  return;
}

//--------------------------------------------------------------------------------------------------
void FillerXlFatJets::SlaveTerminate()
{
}

//--------------------------------------------------------------------------------------------------
void FillerXlFatJets::FillXlFatJet(const PFJet *pPFJet)
{
  // Prepare and store in an array a new FatJet
  XlFatJet *fatJet = fXlFatJets->Allocate();
  new (fatJet) XlFatJet(*pPFJet);

  // Compute and store weighted charge
  fatJet->SetCharge();

  // Prepare and store QG tagging info
  float qgValue = -1.;
  if (fQGTaggingActive) {
    fQGTagger->CalculateVariables(pPFJet, fVertexes);
    qgValue = fQGTagger->QGValue();
  }
  fatJet->SetQGTag(qgValue);

  std::vector<fastjet::PseudoJet> fjParts;
  // Push all particle flow candidates of the input PFjet into fastjet particle collection
  for (UInt_t j=0; j<pPFJet->NPFCands(); ++j) {
    const PFCandidate *pfCand = pPFJet->PFCand(j);
    fjParts.push_back(fastjet::PseudoJet(pfCand->Px(),pfCand->Py(),pfCand->Pz(),pfCand->E()));
    fjParts.back().set_user_index(j);
  }

  // Setup the cluster for fastjet
  fastjet::ClusterSequenceArea *fjClustering =
    new fastjet::ClusterSequenceArea(fjParts,*fCAJetDef,*fAreaDefinition);
  // ---- Fastjet is ready ----


  // Produce a new set of jets based on the fastjet particle collection and the defined clustering
  // Cut off fat jets with pt < 10 GeV and consider only the hardest jet of the output collection
  std::vector<fastjet::PseudoJet> fjOutJets = sorted_by_pt(fjClustering->inclusive_jets(10.));

  // Check that the output collection size is non-null, otherwise nothing to be done further
  if (fjOutJets.size() < 1) {
    printf(" FillerXlFatJets::FillXlFatJet() - WARNING - input PFJet produces null reclustering output!\n");

    if (fjOutJets.size() > 0)
      fjClustering->delete_self_when_unused();
    delete fjClustering;

    return;
  }
  fastjet::PseudoJet fjJet = fjOutJets[0];

  // Compute the subjettiness
  fastjet::contrib::Njettiness::AxesMode axisMode = fastjet::contrib::Njettiness::onepass_kt_axes;
  double beta = 1.0;
  double RNsub = fConeSize; //FIXME: should revert to RNsub = fConeSize
  double Rcutoff = 10000.;
  fastjet::contrib::Nsubjettiness  nSub1(1,axisMode,beta,RNsub,Rcutoff);
  fastjet::contrib::Nsubjettiness  nSub2(2,axisMode,beta,RNsub,Rcutoff);
  fastjet::contrib::Nsubjettiness  nSub3(3,axisMode,beta,RNsub,Rcutoff);
  double tau1 = nSub1(fjJet);
  double tau2 = nSub2(fjJet);
  double tau3 = nSub3(fjJet);

  // Compute the energy correlation function ratios
  fastjet::contrib::EnergyCorrelatorDoubleRatio ECR2b0  (2,0. ,fastjet::contrib::EnergyCorrelator::pt_R);
  fastjet::contrib::EnergyCorrelatorDoubleRatio ECR2b0p2(2,0.2,fastjet::contrib::EnergyCorrelator::pt_R);
  fastjet::contrib::EnergyCorrelatorDoubleRatio ECR2b0p5(2,0.5,fastjet::contrib::EnergyCorrelator::pt_R);
  fastjet::contrib::EnergyCorrelatorDoubleRatio ECR2b1  (2,1.0,fastjet::contrib::EnergyCorrelator::pt_R);
  fastjet::contrib::EnergyCorrelatorDoubleRatio ECR2b2  (2,2.0,fastjet::contrib::EnergyCorrelator::pt_R);
  double C2b0   = ECR2b0(fjJet);
  double C2b0p2 = ECR2b0p2(fjJet);
  double C2b0p5 = ECR2b0p5(fjJet);
  double C2b1   = ECR2b1(fjJet);
  double C2b2   = ECR2b2(fjJet);

  // Compute Q-jets volatility
  std::vector<fastjet::PseudoJet> constits;
  GetJetConstituents(fjJet, constits, 0.01);
  double QJetVol = GetQjetVolatility(constits, 25, fCounter*25);
  fCounter++;
  constits.clear();

  // Compute groomed masses
  fastjet::contrib::SoftDrop softDropSDb0(0.0, fSoftDropZCut, fSoftDropR0);
  fastjet::contrib::SoftDrop softDropSDb1(1.0, fSoftDropZCut, fSoftDropR0);
  fastjet::contrib::SoftDrop softDropSDb2(2.0, fSoftDropZCut, fSoftDropR0);
  fastjet::contrib::SoftDrop softDropSDbm1(-1.0, fSoftDropZCut, fSoftDropR0);
  softDropSDb0.set_tagging_mode();
  softDropSDb1.set_tagging_mode();
  softDropSDb2.set_tagging_mode();
  softDropSDbm1.set_tagging_mode();
  double MassSDb0 = (softDropSDb0(fjJet)).m();
  double MassSDb1 = (softDropSDb1(fjJet)).m();
  double MassSDb2 = (softDropSDb2(fjJet)).m();
  double MassSDbm1 = (softDropSDbm1(fjJet)).m();

  fastjet::PseudoJet fjJetPruned = (*fPruner)(fjJet);
  double MassPruned = fjJetPruned.m();
  double MassFiltered = ((*fFilterer)(fjJet)).m();
  double MassTrimmed = ((*fTrimmer)(fjJet)).m();

  // do the cms top tagging
  fastjet::PseudoJet iJet;
  fastjet::PseudoJet cmsTopJet;
  fastjet::CMSTopTagger* fCMSTopTagger = new fastjet::CMSTopTagger();
  if (fTopTaggingActive) {
      std::vector<fastjet::PseudoJet> lOutJets = sorted_by_pt(fjClustering->inclusive_jets(0.0));
      iJet = lOutJets[0];
      cmsTopJet = fCMSTopTagger->result(iJet);
  }

  // ---- Fastjet is done ----

  // Store the subjettiness values
  fatJet->SetTau1(tau1);
  fatJet->SetTau2(tau2);
  fatJet->SetTau3(tau3);

  // Store the energy correlation values
  fatJet->SetC2b0(C2b0);
  fatJet->SetC2b0p2(C2b0p2);
  fatJet->SetC2b0p5(C2b0p5);
  fatJet->SetC2b1(C2b1);
  fatJet->SetC2b2(C2b2);

  // Store the Qjets volatility
  fatJet->SetQJetVol(QJetVol);

  // Store the groomed masses, apply JEC
  double thisJEC = fatJet->Pt()/fatJet->RawMom().Pt();
  fatJet->SetMassSDb0(MassSDb0*thisJEC);
  fatJet->SetMassSDb1(MassSDb1*thisJEC);
  fatJet->SetMassSDb2(MassSDb2*thisJEC);
  fatJet->SetMassSDbm1(MassSDbm1*thisJEC);
  fatJet->SetMassPruned(MassPruned*thisJEC);
  fatJet->SetMassFiltered(MassFiltered*thisJEC);
  fatJet->SetMassTrimmed(MassTrimmed*thisJEC);

  // Store the color pull
  fatJet->SetPull(GetPull(fjJet,0.01).Mod());

  // Loop on the subjets and fill the subjet Xl collections - do it according to the user request
  if (fFillVSubJets) {
    std::vector<fastjet::PseudoJet> fjVSubJets;
    if (fNSubDeclustering)
      fjVSubJets = nSub3.currentSubjets();
    else {
      int nSubJPruned = std::min<unsigned int>(fjJetPruned.constituents().size(),3);
      fjVSubJets = fjJetPruned.associated_cluster_sequence()->exclusive_subjets(fjJetPruned,nSubJPruned);
    }
    // Order the subjets according to their pt and discard zero pt subjets
    std::vector<fastjet::PseudoJet> fjSubJetsSorted = Sorted_by_pt_min_pt(fjVSubJets,0.01);
    // Store the color pull angle: either choose 2-prong or 3-prong subclustering!
    fatJet->SetPullAngle(GetPullAngle(fjSubJetsSorted,0.01));
    FillXlSubJets(fjSubJetsSorted,fatJet,XlSubJet::ESubJetType::eV);
  }
  if (fFillTopSubJets) {
    std::vector<fastjet::PseudoJet> fjTopSubJets;
    if (fTopTaggingActive) {
      fjTopSubJets = cmsTopJet.pieces();
    } else if (fNSubDeclustering)
      fjTopSubJets = nSub3.currentSubjets();
    else {
      int nSubJPruned = std::min<unsigned int>(fjJetPruned.constituents().size(),3);
      fjTopSubJets = fjJetPruned.associated_cluster_sequence()->exclusive_subjets(fjJetPruned,nSubJPruned);
    }
    // Order the subjets according to their pt
    std::vector<fastjet::PseudoJet> fjSubJetsSorted = Sorted_by_pt_min_pt(fjTopSubJets,0.01);
    // Store the color pull angle: either choose 2-prong or 3-prong subclustering!
    fatJet->SetPullAngle(GetPullAngle(fjSubJetsSorted,0.01));
    FillXlSubJets(fjSubJetsSorted,fatJet,XlSubJet::ESubJetType::eTop);
  }
  //Inclusive b-tagging
  doBtagging(fatJet);


  // Trim the output collections
  fXlSubJets->Trim();
  fXlFatJets->Trim();


  // Memory cleanup
  if (fjOutJets.size() > 0)
    fjClustering->delete_self_when_unused();
  delete fjClustering;

  return;
}

//--------------------------------------------------------------------------------------------------
void FillerXlFatJets::FillXlSubJets(std::vector<fastjet::PseudoJet> &fjSubJets,
                                 XlFatJet *pFatJet, XlSubJet::ESubJetType subJetType)
{
  for (int iSJet=0; iSJet < (int) fjSubJets.size(); iSJet++) {
    XlSubJet *subJet = fXlSubJets->Allocate();
    // Prepare and store in an array a new SubJet
    new (subJet) XlSubJet();
    subJet->SetRawPtEtaPhiM(fjSubJets[iSJet].pt(),
                            fjSubJets[iSJet].eta(),
                            fjSubJets[iSJet].phi(),
                            fjSubJets[iSJet].m());

    // Store the QG tagging variable
    if (fQGTaggingActive)
      FillSubjetQGTagging(fjSubJets[iSJet], 0.01, subJet, pFatJet);

    // Store the subjet type value
    subJet->SetSubJetType(subJetType);

    // Add the subjet to the relative fatjet
    pFatJet->AddSubJet(subJet);

  }

  return;
}
//--------------------------------------------------------------------------------------------------

void FillerXlFatJets::doBtagging(XlFatJet*fatJet) {

  std::map<double, unsigned int> VTXmass;
  const Vertex * pvx = fVertexes->At(0); // highest HT vertex is assumed to be primary
  double maxSVDeltaRToJet = fConeSize-(0.1+(fConeSize-.8)*(.1/.7));
  UInt_t nVertices = fSecondaryVertexes->GetEntries();
  for (unsigned int idx=0; idx<nVertices; idx++) {
    const Vertex * svx = fSecondaryVertexes->At(idx);
    ThreeVector flightDir = flightDirection(pvx,svx); // maybe save this elsewhere, so only run once an event?
    ThreeVector jetDir(fatJet->Px(),fatJet->Py(),fatJet->Pz());
    Float_t dR2 = MathUtils::DeltaR2(flightDir,jetDir);
    if (dR2 < maxSVDeltaRToJet*maxSVDeltaRToJet)
        VTXmass[svx->Mom4().mass()] = idx;
  }

  // recalc nsubjettiness with IVF
  std::vector<fastjet::PseudoJet> currentAxes;
  float tau1IVF=fatJet->Tau1(), tau2IVF=fatJet->Tau2();
  recalcNsubjettiness(fatJet,tau1IVF,tau2IVF,currentAxes, VTXmass);
  FourVectorM currentAxes0(currentAxes[0].pt(),
                            currentAxes[0].eta(),
                            currentAxes[0].phi(),
                            currentAxes[0].m());
  FourVectorM currentAxes1(currentAxes[1].pt(),
                            currentAxes[1].eta(),
                            currentAxes[1].phi(),
                            currentAxes[1].m());

  FourVectorM sumAllTracks(0,0,0,0);
  unsigned int nPrimaryTracks = pvx->NTracks();
  std::vector<const Track*> jetTracks;
  for (unsigned int i=0; i<nPrimaryTracks; ++i) {
      const Track * trk = pvx->Trk(i);
      if (MathUtils::DeltaR(*fatJet,*trk) < fConeSize) {
          sumAllTracks += FourVectorM(trk->Pt(),trk->Eta(),trk->Phi(),0.13957018);
          jetTracks.push_back(trk);
      }
  }
  float totalEnergy = sumAllTracks.E();

  int cont=0;
  ThreeVector flightDir_0, flightDir_1;
  FourVectorM SV_p4_0 , SV_p4_1;
  for (std::map<double,unsigned int>::reverse_iterator iVtx = VTXmass.rbegin(); iVtx!=VTXmass.rend(); ++iVtx) {
    ++cont;
    const Vertex *svx = fSecondaryVertexes->At(iVtx->second);
    FourVectorM svxWeightedMomentum = svx->WeightedMom4();
    double svxEnergy = svxWeightedMomentum.E();
    FourVectorM svxMom0;
    FourVectorM svxMom1;
    ThreeVector flightDir0;
    ThreeVector flightDir1;
    if (cont==1) {
      svxMom0 = svx->Mom4();
      fatJet->SetSVEnergyRatio0(svxEnergy/totalEnergy);
      fatJet->SetSVMass0(svxMom0.M());
      fatJet->SetSVPt0(svxMom0.Pt());
      flightDir0 = flightDirection(pvx,svx);
      double tau_dot;
      if (MathUtils::DeltaR2(flightDir0,currentAxes1)<MathUtils::DeltaR2(flightDir0,currentAxes0))
            tau_dot = (currentAxes[1].px()*flightDir0.x()+currentAxes[1].py()*flightDir0.y()+currentAxes[1].pz()*flightDir0.z())/(sqrt(currentAxes[1].modp2())*flightDir0.r());
          else
            tau_dot = (currentAxes[0].px()*flightDir0.x()+currentAxes[0].py()*flightDir0.y()+currentAxes[0].pz()*flightDir0.z())/(sqrt(currentAxes[0].modp2())*flightDir0.r());
      fatJet->SetTauDot(tau_dot);
    } else if (cont==2) {
      svxMom1 = svx->Mom4();
      fatJet->SetSVEnergyRatio1(svxEnergy/totalEnergy);
      flightDir1 = flightDirection(pvx,svx);
      double zRatio = MathUtils::DeltaR(flightDir0,flightDir1);
      zRatio *= svxMom0.Pt();
      zRatio /= (svxMom0 + svxMom1).M();
      fatJet->SetZRatio(zRatio);
      break; // only consider leading two
    }
  }
}


ThreeVector FillerXlFatJets::flightDirection(const Vertex * pvx, const Vertex * svx) {
  ThreeVector dir(svx->X()-pvx->X(),
                  svx->Y()-pvx->Y(),
                  svx->Z()-pvx->Z());
  return dir;
}

void FillerXlFatJets::recalcNsubjettiness(XlFatJet *fatJet,
                                          float & tau1,
                                          float & tau2,
                                          std::vector<fastjet::PseudoJet> & currentAxes,
                                          std::map<double, unsigned int> VTXmass)
{
  std::vector<fastjet::PseudoJet> fjParticles;
  std::vector<const Track*> svxTracks;

  for (std::map<double,unsigned int>::iterator iVtx = VTXmass.begin(); iVtx!=VTXmass.end(); ++iVtx) {
    const Vertex *svx = fSecondaryVertexes->At(iVtx->second);
    FourVectorM svxMom = svx->Mom4();
    fjParticles.push_back(fastjet::PseudoJet(svxMom.Px(),
                                              svxMom.Py(),
                                              svxMom.Pz(),
                                              svxMom.E()));
    unsigned int nSvxTracks = svx->NTracks();
    for (unsigned int i=0; i<nSvxTracks; ++i) {
      svxTracks.push_back(svx->Trk(i));
    }
  }

  std::vector<const PFCandidate*> jetChargedPFCands;
  const PFCandidate * pfCand;
  for (unsigned int idx=0; idx<fatJet->NPFCands(); ++idx) {
    pfCand = fatJet->PFCand(idx);
    if (fabs(pfCand->Charge())>0.001)
      jetChargedPFCands.push_back(pfCand);
  }
  double dR2;
  for (std::vector<const Track*>::iterator trk = svxTracks.begin(); trk!=svxTracks.end(); ++trk) {
    double minDeltaR2 = 9999.;
    int minIdx = 0;
    for (unsigned int idx = 0; idx < jetChargedPFCands.size(); ++idx) {
      dR2 = MathUtils::DeltaR2(*(jetChargedPFCands[idx]),**trk);
      if (dR2 < minDeltaR2){
        minDeltaR2 = dR2;
        minIdx = idx;
      }
    }
    jetChargedPFCands.erase(jetChargedPFCands.begin()+minIdx); // remove the charged track closest to the svx track
  }

  // loop over jet constituents that are not daughters of IVF vertices and push them in the vector of FastJet constituents
  for(std::vector<const PFCandidate*>::iterator chargedPFCand = jetChargedPFCands.begin(); chargedPFCand!=jetChargedPFCands.end(); ++chargedPFCand)
  {
    fjParticles.push_back(fastjet::PseudoJet((*chargedPFCand)->Px(),
                                              (*chargedPFCand)->Py(),
                                              (*chargedPFCand)->Pz(),
                                              (*chargedPFCand)->E()));
  }

  // re-calculate N-subjettiness
  fastjet::contrib::Njettiness njettiness_(fastjet::contrib::OnePass_KT_Axes(), fastjet::contrib::NormalizedMeasure(1.0,fConeSize));
  tau1 = njettiness_.getTau(1, fjParticles);
  tau2 = njettiness_.getTau(2, fjParticles);
  currentAxes = njettiness_.currentAxes();
}


//--------------------------------------------------------------------------------------------------
std::vector <fastjet::PseudoJet>   FillerXlFatJets::Sorted_by_pt_min_pt(std::vector <fastjet::PseudoJet> &jets,
                                                                     float jetPtMin)
{
  // First order collection by pt
  std::vector<fastjet::PseudoJet> sortedJets = sorted_by_pt(jets);

  // Loop on the sorted collection and erase jets below jetPtMin
  std::vector<fastjet::PseudoJet>::iterator it = sortedJets.begin();
  for ( ;  it != sortedJets.end(); ) {
    if (it->perp() < jetPtMin)
      it = sortedJets.erase(it);
    else
      it++;
  }

  // Return the reduced and sorted jet collection
  return sortedJets;
}

//--------------------------------------------------------------------------------------------------
void FillerXlFatJets::GetJetConstituents(fastjet::PseudoJet &jet, std::vector <fastjet::PseudoJet> &constits,
                                      float constitsPtMin)
{
  // Loop on input jet constituents vector and discard very soft particles (ghosts)
  for (unsigned int iPart = 0; iPart < jet.constituents().size(); iPart++) {
    if (jet.constituents()[iPart].perp() < constitsPtMin)
      continue;
    constits.push_back(jet.constituents()[iPart]);
  }

  return;
}

//--------------------------------------------------------------------------------------------------
double FillerXlFatJets::GetQjetVolatility(std::vector <fastjet::PseudoJet> &constits, int QJetsN, int seed)
{
  std::vector<float> qjetmasses;

  double zcut(0.1), dcut_fctr(0.5), exp_min(0.), exp_max(0.), rigidity(0.1), truncationFactor(0.01);

  QjetsPlugin qjet_plugin(zcut, dcut_fctr, exp_min, exp_max, rigidity, truncationFactor);
  fastjet::JetDefinition qjet_def(&qjet_plugin);

  for(unsigned int ii = 0 ; ii < (unsigned int) QJetsN ; ii++){
    qjet_plugin.SetRandSeed(seed+ii); // new feature in Qjets to set the random seed
    fastjet::ClusterSequence *qjet_seq =
      new fastjet::ClusterSequence(constits, qjet_def);

    vector<fastjet::PseudoJet> inclusive_jets2 = sorted_by_pt(qjet_seq->inclusive_jets(5.0));
    // skip failed recombinations (with no output jets)
    if (inclusive_jets2.size() < 1)
      continue;
    if (inclusive_jets2.size()>0) { qjetmasses.push_back( inclusive_jets2[0].m() ); }
    // memory cleanup
    if ((qjet_seq->inclusive_jets()).size() > 0)
      qjet_seq->delete_self_when_unused();
    delete qjet_seq;
  }

  // find RMS of a vector
  float qjetsRMS = FindRMS( qjetmasses );
  // find mean of a vector
  float qjetsMean = FindMean( qjetmasses );
  float qjetsVolatility = qjetsRMS/qjetsMean;
  return qjetsVolatility;
}

//--------------------------------------------------------------------------------------------------
double FillerXlFatJets::FindRMS(std::vector<float> qjetmasses)
{
  float total = 0.;
  float ctr = 0.;
  for (unsigned int i = 0; i < qjetmasses.size(); i++){
      total = total + qjetmasses[i];
      ctr++;
  }
  float mean = total/ctr;

  float totalsquared = 0.;
  for (unsigned int i = 0; i < qjetmasses.size(); i++){
    totalsquared += (qjetmasses[i] - mean)*(qjetmasses[i] - mean) ;
  }
  float RMS = sqrt( totalsquared/ctr );
  return RMS;
}

//--------------------------------------------------------------------------------------------------
double FillerXlFatJets::FindMean(std::vector<float> qjetmasses)
{
  float total = 0.;
  float ctr = 0.;
  for (unsigned int i = 0; i < qjetmasses.size(); i++){
      total = total + qjetmasses[i];
      ctr++;
  }
  return total/ctr;
}

//--------------------------------------------------------------------------------------------------
void FillerXlFatJets::FillSubjetQGTagging(fastjet::PseudoJet &jet, float constitsPtMin,
                                       XlSubJet *pSubJet, XlFatJet *pFatJet)
{
  // Prepare a PFJet to compute the QGTagging
  PFJet pfJet;
  pfJet.SetRawPtEtaPhiM(jet.pt(),jet.eta(),jet.phi(),jet.m());

  // Loop on input jet constituents vector and discard very soft particles (ghosts)
  for (unsigned int iPart = 0; iPart < jet.constituents().size(); iPart++) {
    if (jet.constituents()[iPart].perp() < constitsPtMin)
      continue;
    int thisPFCandIndex = jet.constituents()[iPart].user_index();
    // Add the constituent to the PF subjet
    pfJet.AddPFCand(pFatJet->PFCand(thisPFCandIndex));
  }

  // Compute the subjet QGTagging
  if (fQGTaggingActive) {
    fQGTagger->CalculateVariables(&pfJet, fVertexes);
    pSubJet->SetQGTag(fQGTagger->QGValue());
    pSubJet->SetQGPtD(fQGTagger->GetPtD());
    pSubJet->SetQGAxis1(fQGTagger->GetAxis1());
    pSubJet->SetQGAxis2(fQGTagger->GetAxis2());
    pSubJet->SetQGMult(fQGTagger->GetMult());
  }

  return;
}

//--------------------------------------------------------------------------------------------------
TVector2 FillerXlFatJets::GetPull(fastjet::PseudoJet &jet, float constitsPtMin)
{
  double dYSum   = 0;
  double dPhiSum = 0;
  // Loop on input jet constituents vector and discard very soft particles (ghosts)
  for (unsigned int iPart = 0; iPart < jet.constituents().size(); iPart++) {
    if (jet.constituents()[iPart].perp() < constitsPtMin)
      continue;
    double dY     = jet.constituents()[iPart].rapidity()-jet.rapidity();
    double dPhi   = MathUtils::DeltaPhi(jet.constituents()[iPart].phi(),jet.phi());
    double weight = jet.constituents()[iPart].pt()*sqrt(dY*dY + dPhi*dPhi);
    dYSum   += weight*dY;
    dPhiSum += weight*dPhi;
  }
  return TVector2(dYSum/jet.pt(), dPhiSum/jet.pt());
}

//--------------------------------------------------------------------------------------------------
double FillerXlFatJets::GetPullAngle(std::vector<fastjet::PseudoJet> &fjSubJets, float constitsPtMin)
{
  // Subject collection already sorted by pt
  // Consider only the leading and the subleading for the pull angle computation
  // work in dy-dphi space of leading subjet

  // Exclude cases where there is no second subjet (input jet made by one particle)
  if (fjSubJets.size() < 2)
    return -20.;

  TVector2 lPull = GetPull(fjSubJets[0],constitsPtMin);
  TVector2 lJet(fjSubJets[1].rapidity()-fjSubJets[0].rapidity(),
                MathUtils::DeltaPhi(fjSubJets[1].phi(), fjSubJets[0].phi()));
  double lThetaP = lPull.DeltaPhi(lJet);
  return lThetaP;
}
