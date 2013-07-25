

// -*- C++ -*-
//
// Package:    ElecIdAnalyzer
// Class:      ElecIdAnalyzer
// 
/**\class ElecIdAnalyzer ElecIdAnalyzer.cc hugues/ElecIdAnalyzer/src/ElecIdAnalyzer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Hugues Brun
//         Created:  Mon Jul  2 10:05:53 CEST 2012
// $Id: ElecIdAnalyzer.h,v 1.4.2.3 2013/03/15 14:35:57 hbrun Exp $
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"

#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/EgammaCandidates/interface/Conversion.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/RecoCandidate/interface/IsoDeposit.h" 
#include "DataFormats/RecoCandidate/interface/RecoCandidate.h"
#include "EGamma/EGammaAnalysisTools/interface/EGammaCutBasedEleId.h"

#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/MuonReco/interface/MuonSelectors.h"

#include "RecoEgamma/EgammaTools/interface/ConversionTools.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "EGamma/EGammaAnalysisTools/interface/EGammaMvaEleEstimator.h"

#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"


#include "RecoEcal/EgammaCoreTools/interface/EcalClusterLazyTools.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/MuonReco/interface/Muon.h" 

#include "DataFormats/Common/interface/RefToPtr.h"

#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"

#include "CommonTools/Statistics/interface/ChiSquaredProbability.h"

#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/METReco/interface/PFMET.h"
#include "DataFormats/METReco/interface/PFMETFwd.h"
#include "DataFormats/METReco/interface/GenMET.h"
#include "DataFormats/METReco/interface/GenMETFwd.h"
#include "DataFormats/METReco/interface/CaloMETFwd.h"


#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "CondFormats/JetMETObjects/interface/FactorizedJetCorrector.h"

#include "DataFormats/PatCandidates/interface/Jet.h"

#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"
#include "DataFormats/HLTReco/interface/TriggerEventWithRefs.h"
#include "DataFormats/Common/interface/TriggerResults.h"

#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/HLTReco/interface/TriggerObject.h"

#include "DataFormats/EgammaCandidates/interface/Photon.h"
#include "DataFormats/EgammaCandidates/interface/PhotonFwd.h"

#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/IPTools/interface/IPTools.h"

#include "DataFormats/Candidate/interface/CandMatchMap.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"

#include "Muon/MuonAnalysisTools/interface/MuonEffectiveArea.h"
#include "Muon/MuonAnalysisTools/interface/MuonMVAEstimator.h"
#include "Muon/MuonAnalysisTools/src/MuonMVAEstimator.cc"




// root stuff !
#include "TH1D.h"
#include <map>
#include "TFile.h"
#include <math.h> 
#include <sstream>
#include <string>
#include <stdlib.h>
#include <string.h>
#include "TLorentzVector.h"
#include "TVector3.h"
#include "TString.h"
#include "TTree.h"




//

// class declaration
//
typedef std::vector<edm::InputTag> vtag;
/*namespace reco {
    typedef edm::AssociationMap<edm::OneToOne<reco::CandidateCollection, reco::CandidateCollection>> CandMatchMap;
}*/


class ElecIdAnalyzer : public edm::EDAnalyzer {
   public:
      explicit ElecIdAnalyzer(const edm::ParameterSet&);
      ~ElecIdAnalyzer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      virtual void beginRun(edm::Run const&, edm::EventSetup const&);
      virtual void endRun(edm::Run const&, edm::EventSetup const&);
      virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
      virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);

      virtual void beginEvent();
      virtual void endEvent();
      virtual bool trainTrigPresel(const reco::GsfElectron&);
      virtual double GetRadialIsoValue(const reco::GsfElectron&, 
                                       const reco::PFCandidateCollection &);
     virtual double GetRadialIsoValueVeto(const reco::GsfElectron&, 
                                     const reco::PFCandidateCollection &);
     virtual double GetRadialIsoValueVetoMore(const reco::GsfElectron&, 
                                                     const reco::PFCandidateCollection &,
                                                     const reco::GsfElectronCollection &,
                                              const reco::MuonCollection &);
      virtual void doMCtruth(reco::GsfElectronRef, edm::Handle <reco::GenParticleCollection>, double);
    virtual void doMCtruthMuons(const reco::Muon*, edm::Handle <reco::GenParticleCollection>, double);

      virtual float deltaR(float , float , float , float );
      virtual float deltaPhi(float , float);
      virtual void fillIsoRings(const reco::GsfElectron& , 
                              const reco::Vertex& , 
                              const reco::PFCandidateCollection &,
                              double ,
                              ElectronEffectiveArea::ElectronEffectiveAreaTarget ,
                              const reco::GsfElectronCollection &,
                              const reco::MuonCollection &);
      virtual bool passMVAcuts(const reco::GsfElectron&, double );
      virtual bool passFOcuts(const reco::GsfElectron&, const reco::Vertex&, bool);
      virtual int PFisCommingFromVertex(const reco::PFCandidate&, const reco::VertexCollection&);
     virtual double PFisolationMVA(const reco::Muon*, const reco::PFCandidateCollection &, const reco::Vertex*, float);

    
      // ----------member data ---------------------------
    // is DATA / MC 
    bool isMC_;
	bool doMuons_;
	bool doElectrons_;
    bool doPhotons_;
    bool doMuMuGammaMCtruth_;
    bool savePF_;
    bool saveConversions_;
	
	vtag muonProducers_;
    // input tags
    edm::InputTag               electronsInputTag_;
    edm::InputTag               conversionsInputTag_;
    edm::InputTag               beamSpotInputTag_;
    edm::InputTag               rhoIsoInputTag;
    edm::InputTag               primaryVertexInputTag_;
    edm::InputTag               triggerResultsLabel_;
    edm::InputTag               triggerSummaryLabel_;
    std::string                 photonCollection_;
    std::vector<edm::InputTag>  isoValInputTags_;
    
    //parameters
    double deltaRpf_;
    
    // debug
    bool printDebug_;
    
    vector<TString> HLT_name;
    vector<int> theBitCorr;
    vector<std::string> HLT_triggerObjects;

    std::vector<edm::InputTag> moduleLabels;

    
    std::string outputFile_; // output file
    
    EGammaMvaEleEstimator* myMVANonTrig;
    EGammaMvaEleEstimator* myMVATrig;
	EGammaMvaEleEstimator *fElectronIsoMVA;
    
    MuonMVAEstimator *muMVANonTrig;
    MuonEffectiveArea::MuonEffectiveAreaTarget effAreaTarget;


    HLTConfigProvider hltConfig;
    

    
    
    //Events
    float T_Event_Rho;
    float T_Event_RhoIso;
    float T_Event_RhoNoPu;
    float T_Event_RhoIsoNoPu;
    int T_Event_RunNumber;
    int T_Event_EventNumber;
    int T_Event_LuminosityBlock;
    int T_Event_processID;
    int T_Event_ptHat;
    int T_Event_nPU;
    float T_Event_nTruePU;
    int T_Event_nPUm;
    int T_Event_nPUp;
    float T_Event_AveNTruePU;
    int T_Event_HLT_Ele27_WP80;
    int T_Event_HLT_Ele17_Ele8;
    int T_Event_HLT_Ele17_Ele8_M50_TnP;
    int T_Event_HLT_Ele20_SC4_M50_TnP;
    int T_Event_HLT_Ele22_CaloIdL_CaloIsoVL;
    int T_Event_HLT_Ele27_CaloIdL_CaloIsoVL_TrkIdVL_TrkIsoVL;
    int T_Event_HLT_Ele30_CaloIdVT_TrkIdT;
    int T_Event_HLT_Ele27_WP80_PFMET_MT50;
    int T_Event_HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralPFNoPUJet30;
    int T_Event_HLT_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL;
    int T_Event_HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL;
    int T_Event_HLT_Mu17_Mu8;
    int T_Event_HLT_Mu17_TkMu8;
    int T_Event_HLT_Mu17;
    int T_Event_HLT_Mu8;
    int T_Event_HLT_Mu8_Ele17;
    int T_Event_HLT_Ele8_Mu17;
    int T_Event_HLT_IsoMu24;
    int T_Event_HLT_IsoMu24_2p1;

    // gen info on the electron
    std::vector<float> *T_Gen_Elec_Px;
    std::vector<float> *T_Gen_Elec_Py;
    std::vector<float> *T_Gen_Elec_Pz;
    std::vector<float> *T_Gen_Elec_Energy;
    std::vector<float> *T_Gen_Elec_deltaR;
    std::vector<int> *T_Gen_Elec_MCpart;    
    std::vector<int> *T_Gen_Elec_PDGid;    
    std::vector<int> *T_Gen_Elec_status;
    std::vector<int> *T_Gen_Elec_MotherID;

    //Vertex
    std::vector<float> *T_Vertex_z;
    std::vector<float> *T_Vertex_y;
    std::vector<float> *T_Vertex_x;
    std::vector<float> *T_Vertex_Chi2Prob;
    std::vector<float> *T_Vertex_ndof;
    std::vector<float> *T_Vertex_rho;
    std::vector<bool> *T_Vertex_isFake;
    std::vector<int>  *T_Vertex_tracksSize;
    
    
    //Electrons
    std::vector<float> *T_Elec_Eta;
    std::vector<float> *T_Elec_IPwrtAveBS;
    std::vector<float> *T_Elec_IPwrtPV;
    std::vector<float> *T_Elec_Px;
    std::vector<float> *T_Elec_Py;
    std::vector<float> *T_Elec_Pz;
    std::vector<float> *T_Elec_Pt;
    std::vector<float> *T_Elec_Energy;
    std::vector<int> *T_Elec_Charge;
    std::vector<float> *T_Elec_puChargedHadronIso;
    std::vector<float> *T_Elec_allChargedHadronIso;
    std::vector<float> *T_Elec_chargedHadronIso;
    std::vector<float> *T_Elec_neutralHadronIso;
    std::vector<float> *T_Elec_photonIso;
    std::vector<float> *T_Elec_puChargedHadronIso04;
    std::vector<float> *T_Elec_allChargedHadronIso04;
    std::vector<float> *T_Elec_chargedHadronIso04;
    std::vector<float> *T_Elec_neutralHadronIso04;
    std::vector<float> *T_Elec_photonIso04;
    std::vector<bool> *T_Elec_passConversionVeto;
    std::vector<float> *T_Elec_vz;
    std::vector<float> *T_Elec_vy;
    std::vector<float> *T_Elec_vx;
    std::vector<int> *T_Elec_nLost; 
    std::vector<int> *T_Elec_nHits;
    std::vector<float> *T_Elec_SC_Et;
    std::vector<float> *T_Elec_SC_Eta;
    std::vector<int> *T_Elec_nBrems;
    std::vector<float> *T_Elec_fBrem;
    std::vector<float> *T_Elec_eSuperClusterOverP;
    std::vector<float> *T_Elec_dr03TkSumPt;
    std::vector<float> *T_Elec_dr03EcalSumEt;
    std::vector<float> *T_Elec_dr03HcalSumEt;
    std::vector<float> *T_Elec_ConvInfoDist;
    std::vector<float> *T_Elec_ConvInfoDCot;
    std::vector<bool> *T_Elec_isEB;
    std::vector<bool> *T_Elec_isEE;
    std::vector<float> *T_Elec_MVA;
    std::vector<float> *T_Elec_simpleEleId95;
    std::vector<float> *T_Elec_simpleEleId90;
    std::vector<float> *T_Elec_simpleEleId85;
    std::vector<float> *T_Elec_simpleEleId80;
    std::vector<float> *T_Elec_simpleEleId70;
    std::vector<float> *T_Elec_simpleEleId60;
    std::vector<float> *T_Elec_deltaPhiIn;
    std::vector<float> *T_Elec_deltaEtaIn;
    std::vector<float> *T_Elec_sigmaIetaIeta;
    std::vector<bool>   *T_Elec_isEcalDriven; 
    std::vector<float> *T_Elec_HtoE;
    std::vector<bool>  *T_Elec_passVeto;
    std::vector<bool>  *T_Elec_passLoose;
    std::vector<bool>  *T_Elec_passMedium;
    std::vector<bool>  *T_Elec_passTight;
    std::vector<bool>  *T_Elec_passFbremopin;
    std::vector<bool>  *T_Elec_passtrigTight;
    std::vector<bool>  *T_Elec_passtrigwp70;
    std::vector<bool>  *T_Elec_isTrig;
    std::vector<double> *T_Elec_MVAid_trig;
    std::vector<double> *T_Elec_MVAid_Nontrig;
    std::vector<double> *T_Elec_Mvaiso;
    std::vector<double> *T_Elec_RadialIso;
    std::vector<double> *T_Elec_RadialIsoVeto;
    std::vector<double> *T_Elec_RadialIsoVetoMore;
    std::vector<double> *T_Elec_ChargedIso_DR0p0To0p1;
    std::vector<double> *T_Elec_ChargedIso_DR0p1To0p2;
    std::vector<double> *T_Elec_ChargedIso_DR0p2To0p3;
    std::vector<double> *T_Elec_ChargedIso_DR0p3To0p4;
    std::vector<double> *T_Elec_ChargedIso_DR0p4To0p5;
    std::vector<double> *T_Elec_GammaIso_DR0p0To0p1;
    std::vector<double> *T_Elec_GammaIso_DR0p1To0p2;
    std::vector<double> *T_Elec_GammaIso_DR0p2To0p3;
    std::vector<double> *T_Elec_GammaIso_DR0p3To0p4;
    std::vector<double> *T_Elec_GammaIso_DR0p4To0p5;
    std::vector<double> *T_Elec_NeutralHadronIso_DR0p0To0p1;
    std::vector<double> *T_Elec_NeutralHadronIso_DR0p1To0p2;
    std::vector<double> *T_Elec_NeutralHadronIso_DR0p2To0p3;
    std::vector<double> *T_Elec_NeutralHadronIso_DR0p3To0p4;
    std::vector<double> *T_Elec_NeutralHadronIso_DR0p4To0p5;
    std::vector<int>    *T_Elec_HLT_Elec27_WP80;
    std::vector<int>    *T_Elec_HLT_Ele17TightID_Ele8_Ele8Leg;
    std::vector<int>    *T_Elec_HLT_Ele17TightID_Ele8_Ele17Leg;
    std::vector<int>    *T_Elec_HLT_Ele17_Ele8_Ele8Leg;
    std::vector<int>    *T_Elec_HLT_Ele17_Ele8_Ele17Leg;
    std::vector<int>    *T_Elec_HLT_Ele17_Ele8_TnP_Ele8Leg;
    std::vector<int>    *T_Elec_HLT_Ele17_Ele8_TnP_Ele17Leg;
    std::vector<int>    *T_Elec_HLT_Ele20_SC4_TnP_SC4Leg;
    std::vector<int>    *T_Elec_HLT_Ele20_SC4_TnP_Ele20Leg;
    std::vector<int>    *T_Elec_HLT_Mu8_Ele17_Ele17Leg;
    std::vector<int>    *T_Elec_HLT_Ele8_Mu17_Ele8Leg;
    std::vector<bool>* T_Elec_passMVA;
    std::vector<float>* T_Elec_kfchi2;
    std::vector<float>* T_Elec_kfhits;
    std::vector<float>* T_Elec_gsfchi2;
    std::vector<float>* T_Elec_detacalo;
    std::vector<float>* T_Elec_see;
    std::vector<float>* T_Elec_spp;
    std::vector<float>* T_Elec_etawidth;
    std::vector<float>* T_Elec_phiwidth;
    std::vector<float>* T_Elec_e1x5e5x5;
    std::vector<float>* T_Elec_R9;
    std::vector<float>* T_Elec_EoP;
    std::vector<float>* T_Elec_ESeedoP;
    std::vector<float>* T_Elec_ESeedoPout;
    std::vector<float>* T_Elec_IoEmIoP;
    std::vector<float>* T_Elec_eleEoPout;
    std::vector<float>* T_Elec_PreShowerOverRaw;
    std::vector<float>* T_Elec_EcalEnergy;
    std::vector<float>* T_Elec_TrackPatVtx;
    std::vector<float>* T_Elec_d0;
    std::vector<float>* T_Elec_IP3D;
    std::vector<float>* T_Elec_dZ;
    std::vector<bool>* T_Elec_isFO;
    std::vector<float>* T_Elec_CombIsoHWW;
	
	
	//now the muons ! 
	std::vector<float>*T_Muon_Eta;
 	std::vector<float>*T_Muon_Phi;
	std::vector<float>*T_Muon_Energy;
	std::vector<float>*T_Muon_Et;
	std::vector<float>*T_Muon_Pt;
	std::vector<float>*T_Muon_Px;
	std::vector<float>*T_Muon_Py;
	std::vector<float>*T_Muon_Pz;
	std::vector<float>*T_Muon_Mass;

    
	std::vector<bool> *T_Muon_IsGlobalMuon;
    std::vector<bool> *T_Muon_IsTrackerMuon;
    std::vector<bool> *T_Muon_IsPFMuon;
    std::vector<bool> *T_Muon_IsCaloMuon;
    std::vector<bool> *T_Muon_IsStandAloneMuon;
    std::vector<bool> *T_Muon_IsMuon;
    std::vector<bool> *T_Muon_IsGlobalMuon_PromptTight;
    std::vector<bool> *T_Muon_IsTrackerMuonArbitrated;
    std::vector<int>  *T_Muon_numberOfChambers;
    std::vector<int>  *T_Muon_numberOfChambersRPC;
    std::vector<int>  *T_Muon_numberOfMatches;
    std::vector<int>  *T_Muon_numberOfMatchedStations;
    std::vector<int>  *T_Muon_charge;
    
    
    std::vector<bool> *T_Muon_TMLastStationTight;
    std::vector<float> *T_Muon_globalTrackChi2;
    std::vector<int>  *T_Muon_validMuonHits;
    std::vector<float> *T_Muon_trkKink;
    std::vector<int>  *T_Muon_trkNbOfTrackerLayers;
    std::vector<int>  *T_Muon_trkNbOfValidTrackeHits;
    std::vector<int>  *T_Muon_trkValidPixelHits;
    std::vector<float> *T_Muon_trkError;
    std::vector<float> *T_Muon_dB;
    std::vector<float> *T_Muon_dzPV;
    std::vector<float> *T_Muon_dBstop;
    std::vector<float> *T_Muon_dzstop;
    
    // PF isolation
    std::vector<float> *T_Muon_chargedHadronIsoR04;
    std::vector<float> *T_Muon_neutralHadronIsoR04;
    std::vector<float> *T_Muon_photonIsoR04;
    std::vector<float> *T_Muon_chargedHadronIsoPUR04;
    
    std::vector<float> *T_Muon_chargedHadronIsoR03;
    std::vector<float> *T_Muon_neutralHadronIsoR03;
    std::vector<float> *T_Muon_photonIsoR03;
    std::vector<float> *T_Muon_chargedHadronIsoPUR03;
    
    std::vector<float> *T_Muon_isoR03_emEt;
    std::vector<float> *T_Muon_isoR03_hadEt;
    std::vector<float> *T_Muon_isoR03_hoEt;
    std::vector<float> *T_Muon_isoR03_sumPt;
    std::vector<int> *T_Muon_isoR03_nTracks;
    std::vector<int> *T_Muon_isoR03_nJets;
    std::vector<float> *T_Muon_isoRingsMVA;
    
    //trigger matching 
    std::vector<int> *T_Muon_HLT_Mu17_TkMu8_Mu17Leg;
    std::vector<int> *T_Muon_HLT_Mu17_TkMu8_Mu8Leg;
    std::vector<int> *T_Muon_HLT_Mu17_Mu8_Mu17Leg;
    std::vector<int> *T_Muon_HLT_Mu17_Mu8_Mu8Leg;
    std::vector<int> *T_Muon_HLT_Mu17_obj;
    std::vector<int> *T_Muon_HLT_Mu8_obj;
    std::vector<int> *T_Muon_HLT_Mu8_Ele17_Mu8Leg;
    std::vector<int> *T_Muon_HLT_Ele8_Mu17_Mu17Leg;
    std::vector<int> *T_Muon_HLT_IsoMu24;
    std::vector<int> *T_Muon_HLT_IsoMu24_2p1;
    
    //the muon GEN infos !
    std::vector<float> *T_Gen_Muon_Px;
    std::vector<float> *T_Gen_Muon_Py;
    std::vector<float> *T_Gen_Muon_Pz;
    std::vector<float> *T_Gen_Muon_Energy;
    std::vector<float> *T_Gen_Muon_deltaR;
    std::vector<int> *T_Gen_Muon_MCpart;
    std::vector<int> *T_Gen_Muon_PDGid;
    std::vector<int> *T_Gen_Muon_status;
    std::vector<int> *T_Gen_Muon_MotherID;
    
    
    //PF particles
    std::vector<float> *T_PF_Et;
    std::vector<float> *T_PF_Pt;
    std::vector<float> *T_PF_Px;
    std::vector<float> *T_PF_Py;
    std::vector<float> *T_PF_Pz;
    std::vector<float> *T_PF_Eta;
    std::vector<float> *T_PF_Phi;
    std::vector<int>   *T_PF_pdgID;
    std::vector<int>   *T_PF_particleID;    
    std::vector<int>   *T_PF_hasTrack;
    std::vector<int>   *T_PF_hasTrackWithMissingHits;
    std::vector<int>   *T_PF_isPU;
    
    
    
    //Jets vector
    std::vector<float> *T_Jet_Px;
    std::vector<float> *T_Jet_Py;
    std::vector<float> *T_Jet_Pz;
    std::vector<float> *T_Jet_Et;
    std::vector<float> *T_Jet_Eta;
    std::vector<float> *T_Jet_Energy;
    std::vector<float> *T_Jet_Phi;
    std::vector<float> *T_Jet_Corr;
    
    
    // gamma
    
    std::vector<float> *T_Pho_Et;
    std::vector<float> *T_Pho_Energy;
    std::vector<float> *T_Pho_Pt;
    std::vector<float> *T_Pho_Px;
    std::vector<float> *T_Pho_Py;
    std::vector<float> *T_Pho_Pz;
    
    std::vector<float> *T_Pho_Eta;
    std::vector<float> *T_Pho_Phi;
    
    std::vector<float> *T_Pho_r9;
    std::vector<float> *T_Pho_EtaWidth;
    std::vector<float> *T_Pho_PhiWidth;
    std::vector<float> *T_Pho_sigmaIetaIeta;
    
    std::vector<int>   *T_Pho_indOfTheElec;
    
    std::vector<float> *T_Pho_SCEt;
    std::vector<float> *T_Pho_SCEnergy;
    std::vector<float> *T_Pho_SCEta;
    std::vector<float> *T_Pho_SCPhi;
    
    std::vector<int> *T_Pho_isMatchedWithMC;
    std::vector<int> *T_Pho_Gen_PDGid;
    std::vector<int> *T_Pho_Gen_Status;
    std::vector<int> *T_Pho_Gen_MotherID;

    
    
    //conversions
    std::vector<int> *T_Conv_EleInd;
    std::vector<int> *T_Conv_PhoInd;
    std::vector<float> *T_Conv_vtxProb;
    std::vector<float> *T_Conv_lxy;
    std::vector<int  > *T_Conv_nHitsMax;
    



  
    
    //met of the event
    float T_METPF_ET;
    float T_METPF_Phi;
    float T_METPF_Sig;
    float T_METPFTypeI_ET;
    float T_METPFTypeI_Phi;
    
    
    // root file to store histograms
    TFile*  rootFile_;
    
    //Tree
    TTree* mytree_;
    
    


};
typedef std::vector< edm::Handle< edm::ValueMap<reco::IsoDeposit> > >   IsoDepositMaps;
typedef std::vector< edm::Handle< edm::ValueMap<double> > >             IsoDepositVals;

//}



