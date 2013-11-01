#include "../interface/ElecIdAnalyzer.h"

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//


ElecIdAnalyzer::ElecIdAnalyzer(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed
    // is DATA/MC 
    isMC_                   = iConfig.getParameter<bool>("isMC");
    doMuons_				= iConfig.getParameter<bool>("doMuons");
    doElectrons_		    = iConfig.getParameter<bool>("doElectrons");
    doPhotons_              = iConfig.getParameter<bool>("doPhotons");
    savePF_                 = iConfig.getParameter<bool>("savePF");
    saveConversions_        = iConfig.getParameter<bool>("saveConversions");
    doMuMuGammaMCtruth_     = iConfig.getParameter<bool>("doMuMuGammaMC");
    doPFPATmatching_           = iConfig.getParameter<bool>("doPFPATmatching");
    
    // get input parameters
    electronsInputTag_      = iConfig.getParameter<edm::InputTag>("electronsInputTag");
    conversionsInputTag_    = iConfig.getParameter<edm::InputTag>("conversionsInputTag");
    beamSpotInputTag_       = iConfig.getParameter<edm::InputTag>("beamSpotInputTag");
    rhoIsoInputTag          = iConfig.getParameter<edm::InputTag>("rhoIsoInputTag");
    primaryVertexInputTag_  = iConfig.getParameter<edm::InputTag>("primaryVertexInputTag");
    triggerResultsLabel_    = iConfig.getParameter<edm::InputTag>("TriggerResults");
    triggerSummaryLabel_    = iConfig.getParameter<edm::InputTag>("HLTTriggerSummaryAOD");
	muonProducers_			= iConfig.getParameter<vtag>("muonProducer");
    photonCollection_       = iConfig.getParameter<std::string>("photonCollection");
    isoValInputTags_        = iConfig.getParameter<std::vector<edm::InputTag> >("isoValInputTags");
    
    //parameters
    deltaRpf_               = iConfig.getParameter<double>("deltaRsavePF");
    
    // debug
    printDebug_             = iConfig.getParameter<bool>("printDebug");
    
    outputFile_   = iConfig.getParameter<std::string>("outputFile");
    rootFile_ = TFile::Open(outputFile_.c_str(),"RECREATE");
    
    HLT_name.push_back("HLT_Ele27_WP80_v");//0
    HLT_name.push_back("HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v");//1
    HLT_name.push_back("HLT_Ele17_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT_Ele8_Mass50_v");//2
    HLT_name.push_back("HLT_Ele20_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT_SC4_Mass50_v");//3
    HLT_name.push_back("HLT_Ele22_CaloIdL_CaloIsoVL_v");//4
    HLT_name.push_back("HLT_Ele27_CaloIdL_CaloIsoVL_TrkIdVL_TrkIsoVL_v");//5
    HLT_name.push_back("HLT_Ele30_CaloIdVT_TrkIdT_v");//6
    HLT_name.push_back("HLT_Ele27_WP80_PFMET_MT50_v");//7
    HLT_name.push_back("HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralPFNoPUJet30_v");//8
    HLT_name.push_back("HLT_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v");//9
    HLT_name.push_back("HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v");//10
    HLT_name.push_back("HLT_Mu17_TkMu8_v");//11
    HLT_name.push_back("HLT_Mu17_Mu8_v");//12
    HLT_name.push_back("HLT_Mu17_v");//13
    HLT_name.push_back("HLT_Mu8_v");//14
    HLT_name.push_back("HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v");//15
    HLT_name.push_back("HLT_Mu17_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v");//16
    HLT_name.push_back("HLT_IsoMu24_v");//17
    HLT_name.push_back("HLT_IsoMu24_eta2p1_v");//18
    
    
    //electron part :
    HLT_triggerObjects.push_back("hltEle27WP80TrackIsoFilter");//0
    HLT_triggerObjects.push_back("hltEle17TightIdLooseIsoEle8TightIdLooseIsoTrackIsoDoubleFilter");//1
    HLT_triggerObjects.push_back("hltEle17TightIdLooseIsoEle8TightIdLooseIsoTrackIsoFilter");//2
    HLT_triggerObjects.push_back("hltEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVTEle8PMMassFilter");//3
    HLT_triggerObjects.push_back("hltEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVTEle8TrackIsoFilter");//4
    HLT_triggerObjects.push_back("hltEle20CaloIdVTCaloIsoVTTrkIdTTrkIsoVTSC4PMMassFilter");//5
    HLT_triggerObjects.push_back("hltEle20CaloIdVTCaloIsoVTTrkIdTTrkIsoVTSC4TrackIsoFilter");//6
    HLT_triggerObjects.push_back("hltEle17CaloIdTCaloIsoVLTrkIdVLTrkIsoVLTrackIsoFilter");//7
    HLT_triggerObjects.push_back("hltEle8TightIdLooseIsoTrackIsoFilter");//8
    
    //muon part :
    HLT_triggerObjects.push_back("hltL3fL1sMu10MuOpenOR3p5L1f0L2f10L3Filtered17");//9
    HLT_triggerObjects.push_back("hltDiMuonGlbFiltered17TrkFiltered8");//10
    HLT_triggerObjects.push_back("hltL3pfL1DoubleMu10MuOpenOR3p5L1f0L2pf0L3PreFiltered8");//11
    HLT_triggerObjects.push_back("hltL3fL1DoubleMu10MuOpenOR3p5L1f0L2f10L3Filtered17");//12
    HLT_triggerObjects.push_back("hltL3fL1sMu12L3Filtered17");//13
    HLT_triggerObjects.push_back("hltL3fL1sMu3L3Filtered8");//14
    HLT_triggerObjects.push_back("hltMu8Ele17CaloIdTCaloIsoVLTrkIdVLTrkIsoVLTrackIsoFilter");//15
    HLT_triggerObjects.push_back("hltL1sL1Mu3p5EG12ORL1MuOpenEG12L3Filtered8");//16
    HLT_triggerObjects.push_back("hltMu17Ele8CaloIdTCaloIsoVLTrkIdVLTrkIsoVLTrackIsoFilter");//17
    HLT_triggerObjects.push_back("hltL1Mu12EG7L3MuFiltered17");//18
    HLT_triggerObjects.push_back("hltL3crIsoL1sMu16L1f0L2f16QL3f24QL3crIsoRhoFiltered0p15");//19
    HLT_triggerObjects.push_back("hltL3crIsoL1sMu16Eta2p1L1f0L2f16QL3f24QL3crIsoRhoFiltered0p15");//20
    //muon begin of the year
    HLT_triggerObjects.push_back("hltL3fL1DoubleMu10MuOpenL1f0L2f10L3Filtered17");//21
    HLT_triggerObjects.push_back("hltL3pfL1DoubleMu10MuOpenL1f0L2pf0L3PreFiltered8");//22
    HLT_triggerObjects.push_back("hltL3fL1sMu10MuOpenL1f0L2f10L3Filtered17");//23
    HLT_triggerObjects.push_back("hltDiMuonGlbFiltered17TrkFiltered8");//24

    
    
    fElectronIsoMVA = new EGammaMvaEleEstimator();
    vector<string> eleiso_weightfiles;
    eleiso_weightfiles.push_back("EGamma/EGammaAnalysisTools/data/ElectronIso_BDTG_V0_BarrelPt5To10.weights.xml");
    eleiso_weightfiles.push_back("EGamma/EGammaAnalysisTools/data/ElectronIso_BDTG_V0_EndcapPt5To10.weights.xml");
    eleiso_weightfiles.push_back("EGamma/EGammaAnalysisTools/data/ElectronIso_BDTG_V0_BarrelPt10ToInf.weights.xml");
    eleiso_weightfiles.push_back("EGamma/EGammaAnalysisTools/data/ElectronIso_BDTG_V0_EndcapPt10ToInf.weights.xml");
    
    vector<string> eleiso_weightfilesReal;
    string the_path;
    for (unsigned i  = 0 ; i < eleiso_weightfiles.size() ; i++){
        the_path = edm::FileInPath ( eleiso_weightfiles[i] ).fullPath();
        eleiso_weightfilesReal.push_back(the_path);
    }
    
    
    fElectronIsoMVA->initialize("EleIso_BDTG_IsoRings",
                                EGammaMvaEleEstimator::kIsoRings,
                                kTRUE,
                                eleiso_weightfilesReal);
    fElectronIsoMVA->SetPrintMVADebug(kFALSE);
    
    myMVANonTrig = new EGammaMvaEleEstimator();
    
    // NOTE: it is better if you copy the MVA weight files locally
    std::vector<std::string> myManualCatWeigths;
    myManualCatWeigths.push_back("EGamma/EGammaAnalysisTools/data/Electrons_BDTG_NonTrigV0_Cat1.weights.xml");
    myManualCatWeigths.push_back("EGamma/EGammaAnalysisTools/data/Electrons_BDTG_NonTrigV0_Cat2.weights.xml");
    myManualCatWeigths.push_back("EGamma/EGammaAnalysisTools/data/Electrons_BDTG_NonTrigV0_Cat3.weights.xml");
    myManualCatWeigths.push_back("EGamma/EGammaAnalysisTools/data/Electrons_BDTG_NonTrigV0_Cat4.weights.xml");
    myManualCatWeigths.push_back("EGamma/EGammaAnalysisTools/data/Electrons_BDTG_NonTrigV0_Cat5.weights.xml");
    myManualCatWeigths.push_back("EGamma/EGammaAnalysisTools/data/Electrons_BDTG_NonTrigV0_Cat6.weights.xml");
    
    Bool_t manualCat = true;
    
    vector<string> myManualCatWeigthsReal;
    for (unsigned i  = 0 ; i < myManualCatWeigths.size() ; i++){
        the_path = edm::FileInPath ( myManualCatWeigths[i] ).fullPath();
        myManualCatWeigthsReal.push_back(the_path);
    }
    
    myMVANonTrig->initialize("BDT",
                             EGammaMvaEleEstimator::kNonTrig,
                             manualCat, 
                             myManualCatWeigthsReal);
    
    // NOTE: it is better if you copy the MVA weight files locally
    
    std::vector<std::string> myManualCatWeigthsTrig;
    myManualCatWeigthsTrig.push_back("EGamma/EGammaAnalysisTools/data/Electrons_BDTG_TrigV0_Cat1.weights.xml");
    myManualCatWeigthsTrig.push_back("EGamma/EGammaAnalysisTools/data/Electrons_BDTG_TrigV0_Cat2.weights.xml");
    myManualCatWeigthsTrig.push_back("EGamma/EGammaAnalysisTools/data/Electrons_BDTG_TrigV0_Cat3.weights.xml");
    myManualCatWeigthsTrig.push_back("EGamma/EGammaAnalysisTools/data/Electrons_BDTG_TrigV0_Cat4.weights.xml");
    myManualCatWeigthsTrig.push_back("EGamma/EGammaAnalysisTools/data/Electrons_BDTG_TrigV0_Cat5.weights.xml");
    myManualCatWeigthsTrig.push_back("EGamma/EGammaAnalysisTools/data/Electrons_BDTG_TrigV0_Cat6.weights.xml");
    
    vector<string> myManualCatWeigthsTrigReal;
    for (unsigned i  = 0 ; i < myManualCatWeigthsTrig.size() ; i++){
        the_path = edm::FileInPath ( myManualCatWeigthsTrig[i] ).fullPath();
        myManualCatWeigthsTrigReal.push_back(the_path);
    }
    
    myMVATrig = new EGammaMvaEleEstimator();
    myMVATrig->initialize("BDT",
                          EGammaMvaEleEstimator::kTrig,
                          manualCat,
                          myManualCatWeigthsTrigReal);
    
    // setup all the MVA tools !
    std::string baseFolder("Muon/MuonAnalysisTools/data");
    std::vector<string> muonIsoRings;
    muonIsoRings.push_back(baseFolder+"/MuonIsoMVA_sixie-BarrelPt5To10_V0_BDTG.weights.xml");
    muonIsoRings.push_back(baseFolder+"/MuonIsoMVA_sixie-EndcapPt5To10_V0_BDTG.weights.xml");
    muonIsoRings.push_back(baseFolder+"/MuonIsoMVA_sixie-BarrelPt10ToInf_V0_BDTG.weights.xml");
    muonIsoRings.push_back(baseFolder+"/MuonIsoMVA_sixie-EndcapPt10ToInf_V0_BDTG.weights.xml");
    muonIsoRings.push_back(baseFolder+"/MuonIsoMVA_sixie-Tracker_V0_BDTG.weights.xml");
    muonIsoRings.push_back(baseFolder+"/MuonIsoMVA_sixie-Global_V0_BDTG.weights.xml");
    
    vector<string> myMuonIsoRings;
    for (unsigned i  = 0 ; i < muonIsoRings.size() ; i++){
        the_path = edm::FileInPath ( muonIsoRings[i] ).fullPath();
        myMuonIsoRings.push_back(the_path);
    }
    
    muMVANonTrig  = new MuonMVAEstimator();
    muMVANonTrig->initialize("MuonIso_BDTG_IsoRings",MuonMVAEstimator::kIsoRings,true,myMuonIsoRings);
    muMVANonTrig->SetPrintMVADebug(kFALSE);

}


ElecIdAnalyzer::~ElecIdAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
    delete rootFile_;
    

}


//
// member functions
//

// ------------ method called for each event  ------------
void
ElecIdAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

    using namespace std;
    using namespace edm;
    using namespace reco;
    
    beginEvent();
     bool debugMVAclass = false;
    // electrons
    edm::Handle<reco::GsfElectronCollection> els_h;
    iEvent.getByLabel(electronsInputTag_, els_h);
    const GsfElectronCollection inElectrons = *(els_h.product());

    
    //recup of the isolation vals
    // iso deposits
    IsoDepositVals isoVals(isoValInputTags_.size());
    for (size_t j = 0; j < isoValInputTags_.size(); ++j) {
        iEvent.getByLabel(isoValInputTags_[j], isoVals[j]);
    }
    
    // conversions
    edm::Handle<reco::ConversionCollection> conversions_h;
    iEvent.getByLabel(conversionsInputTag_, conversions_h);
    
    // beam spot
    edm::Handle<reco::BeamSpot> beamspot_h;
    iEvent.getByLabel(beamSpotInputTag_, beamspot_h);
    const reco::BeamSpot &beamSpot = *(beamspot_h.product());
    
    // the muons collection
    Handle<reco::MuonCollection> hMuonProduct;
    iEvent.getByLabel("muons", hMuonProduct);  
    const reco::MuonCollection inMuons = *(hMuonProduct.product()); 
    
    // pf Collection
    Handle<reco::PFCandidateCollection> hPfCandProduct;
	iEvent.getByLabel("particleFlow", hPfCandProduct);
    const reco::PFCandidateCollection &inPfCands = *(hPfCandProduct.product());
    
    //mET stuff
    edm::Handle<reco::PFMETCollection> metPF;
    iEvent.getByLabel("pfMet",metPF);
    const PFMET * metsPF= &((metPF.product())->front());
	
	
	//muon collection :
	
	edm::Handle < std::vector <reco::Muon> > recoMuons;
    edm::InputTag muonProducer = muonProducers_.at(0);
	iEvent.getByLabel(muonProducer, recoMuons);
	
  /*  edm::Handle<reco::PFMETCollection> metPFTypeI;
    iEvent.getByLabel("pfType1CorrectedMet",metPFTypeI);
    const PFMET * metsPFTypeI= &((metPFTypeI.product())->front());*/
    
    // Jet 
    edm::Handle < std::vector <reco::PFJet> > recoPFJets;
    iEvent.getByLabel("ak5PFJets", recoPFJets);
    int nJets = recoPFJets->size();
    
    // vertices
    edm::Handle<reco::VertexCollection> vtx_h;
    iEvent.getByLabel(primaryVertexInputTag_, vtx_h);
    const reco::VertexCollection& vtxs = *(vtx_h.product());
    
    //trans tracks builder
    edm::ESHandle<TransientTrackBuilder> builder;
    iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder", builder);
    TransientTrackBuilder thebuilder = *(builder.product());
    
    //ecal recHit for cluster shape 
    InputTag  reducedEBRecHitCollection(string("reducedEcalRecHitsEB"));
    InputTag  reducedEERecHitCollection(string("reducedEcalRecHitsEE"));
    
    EcalClusterLazyTools lazyTools(iEvent, iSetup, reducedEBRecHitCollection, reducedEERecHitCollection);
    
    //collection of Gen Particle for MC case
    edm::Handle <reco::GenParticleCollection> genParticles;
//    std::vector<reco::GenParticle> theGenParts;
    
    //read the trigger results 
    edm::Handle<edm::TriggerResults> triggerResults;
    iEvent.getByLabel(triggerResultsLabel_, triggerResults);
    
    edm::Handle<trigger::TriggerEvent> triggerSummary;
    iEvent.getByLabel(triggerSummaryLabel_, triggerSummary);
    
    
    // read the photon collections
    Handle<reco::PhotonCollection> pPhotons;
    iEvent.getByLabel(photonCollection_, pPhotons);
    const reco::PhotonCollection* photons = pPhotons.product();
    
    
    //read the PAT PF electrons
    edm::Handle<pat::ElectronCollection > electrons;
    iEvent.getByLabel( "selectedPatElectronsPFlow", electrons );
    


    //map for MC matching
   // Handle<CandMatchMap> match;
   // iEvent.getByLabel( "electronsMCmatch", match );
    
    
    
    reco::Vertex dummy;
    const reco::Vertex *pv = &dummy;
    if (vtx_h->size() != 0) {
        pv = &*vtx_h->begin();
    } else { // create a dummy PV
        Vertex::Error e;
        e(0, 0) = 0.0015 * 0.0015;
        e(1, 1) = 0.0015 * 0.0015;
        e(2, 2) = 15. * 15.;
        Vertex::Point p(0, 0, 0);
        dummy = Vertex(p, e, 0, 0, 0);
    }
  
    // rho for isolation
    edm::Handle<double> rhoIso_h;
    iEvent.getByLabel(rhoIsoInputTag, rhoIso_h);
    double rhoIso = *(rhoIso_h.product());
    
    // rho 
    Handle<double> hRho;
    edm::InputTag tag("kt6PFJets","rho");
    iEvent.getByLabel(tag,hRho);
    double Rho = *hRho;


    T_Event_Rho = Rho;
    T_Event_RhoIso = rhoIso;

    /// fill the electron and the muon collection for the MVA iso 
    
    reco::MuonCollection IdentifiedMuons;
    reco::GsfElectronCollection IdentifiedElectrons;
    reco::GsfElectronCollection FOelectrons;
    
    
    for (reco::GsfElectronCollection::const_iterator iE = inElectrons.begin(); 
         iE != inElectrons.end(); ++iE) {
        
        double electronTrackZ = 0;
        if (iE->gsfTrack().isNonnull()) {
            electronTrackZ = iE->gsfTrack()->dz(vtx_h->at(0).position());
        } else if (iE->closestCtfTrackRef().isNonnull()) {
            electronTrackZ = iE->closestCtfTrackRef()->dz(vtx_h->at(0).position());
        }    
        if(fabs(electronTrackZ) > 0.2)  continue;
        
        
        if(fabs(iE->superCluster()->eta())<1.479) {     
            if(iE->pt() > 20) {
                if(iE->sigmaIetaIeta()       > 0.01)  continue;
                if(fabs(iE->deltaEtaSuperClusterTrackAtVtx()) > 0.007) continue;
                if(fabs(iE->deltaPhiSuperClusterTrackAtVtx()) > 0.8)  continue;
                if(iE->hadronicOverEm()       > 0.15)  continue;    
            } else {
                if(iE->sigmaIetaIeta()       > 0.012)  continue;
                if(fabs(iE->deltaEtaSuperClusterTrackAtVtx()) > 0.007) continue;
                if(fabs(iE->deltaPhiSuperClusterTrackAtVtx()) > 0.8)  continue;
                if(iE->hadronicOverEm()       > 0.15) continue;    
            } 
        } else {     
            if(iE->pt() > 20) {
                if(iE->sigmaIetaIeta()       > 0.03)  continue;
                if(fabs(iE->deltaEtaSuperClusterTrackAtVtx()) > 0.010) continue;
                if(fabs(iE->deltaPhiSuperClusterTrackAtVtx()) > 0.8)  continue;
            } else {
                if(iE->sigmaIetaIeta()       > 0.032)  continue;
                if(fabs(iE->deltaEtaSuperClusterTrackAtVtx()) > 0.010) continue;
                if(fabs(iE->deltaPhiSuperClusterTrackAtVtx()) > 0.8)  continue;
            }
        }
        IdentifiedElectrons.push_back(*iE);
        bool vtxFitConversion = ConversionTools::hasMatchedConversion(*iE, conversions_h, beamSpot.position());
        bool isAnFOelectron = passFOcuts((*iE), vtx_h->at(0), vtxFitConversion);
        if (isAnFOelectron) FOelectrons.push_back(*iE);
        
    }

    for (reco::MuonCollection::const_iterator iM = inMuons.begin(); 
         iM != inMuons.end(); ++iM) {
        
        if(!(iM->innerTrack().isNonnull())) {
            continue;
        } 
        
        if(!(iM->isGlobalMuon() || iM->isTrackerMuon())) continue;
        if(iM->innerTrack()->numberOfValidHits() < 11 ) continue;
        
        IdentifiedMuons.push_back(*iM);
        
    }
    /// prepare the HLT matching 


   bool changedConfig = false;
    if (!hltConfig.init(iEvent.getRun(), iSetup, triggerResultsLabel_.process(), changedConfig)) {
        edm::LogError("HLTMatchingFilter") << "Initialization of HLTConfigProvider failed!!"; 
        return;
    }
    if (changedConfig){
	std::cout << "the present menu is " << hltConfig.tableName() << std::endl;
        moduleLabels.clear();
        for (size_t m = 0 ; m < HLT_name.size() ; m++){
            bool foundThePathInMenu = false;
            for (size_t j = 0; j < hltConfig.triggerNames().size(); j++) {
                if (TString(hltConfig.triggerNames()[j]).Contains(HLT_name[m])){
                    cout << j << " = " << hltConfig.triggerNames()[j] << endl;
                    theBitCorr.push_back(j);
                    foundThePathInMenu = true;
                }
            }
            if (!(foundThePathInMenu)) theBitCorr.push_back(-1);
        }
        for (unsigned int j=0; j<HLT_triggerObjects.size(); j++)
            moduleLabels.push_back(edm::InputTag(HLT_triggerObjects[j], "", triggerResultsLabel_.process()));
    }
    // nom fill the trigger bits : 

    T_Event_HLT_Ele27_WP80 = (theBitCorr[0]==-1) ? -1 : triggerResults->accept(theBitCorr[0]);
    T_Event_HLT_Ele17_Ele8 = (theBitCorr[1]==-1) ? -1 : triggerResults->accept(theBitCorr[1]);
    T_Event_HLT_Ele17_Ele8_M50_TnP = (theBitCorr[2]==-1) ? -1 : triggerResults->accept(theBitCorr[2]);
    T_Event_HLT_Ele20_SC4_M50_TnP = (theBitCorr[3]==-1) ? -1 : triggerResults->accept(theBitCorr[3]);
    T_Event_HLT_Ele22_CaloIdL_CaloIsoVL = (theBitCorr[4]==-1) ? -1 : triggerResults->accept(theBitCorr[4]);
    T_Event_HLT_Ele27_CaloIdL_CaloIsoVL_TrkIdVL_TrkIsoVL = (theBitCorr[5]==-1) ? -1 : triggerResults->accept(theBitCorr[5]);
    T_Event_HLT_Ele30_CaloIdVT_TrkIdT = (theBitCorr[6]==-1) ? -1 : triggerResults->accept(theBitCorr[6]);
    T_Event_HLT_Ele27_WP80_PFMET_MT50 = (theBitCorr[7]==-1) ? -1 : triggerResults->accept(theBitCorr[7]);
    T_Event_HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralPFNoPUJet30 = (theBitCorr[8]==-1) ? -1 : triggerResults->accept(theBitCorr[8]);
    T_Event_HLT_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL     = (theBitCorr[9]==-1) ? -1 : triggerResults->accept(theBitCorr[9]);
    T_Event_HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL = (theBitCorr[10]==-1) ? -1 : triggerResults->accept(theBitCorr[10]);
    T_Event_HLT_Mu17_Mu8 = (theBitCorr[11]==-1) ? -1 : triggerResults->accept(theBitCorr[11]);
    T_Event_HLT_Mu17_TkMu8 = (theBitCorr[12]==-1) ? -1 : triggerResults->accept(theBitCorr[12]);
    T_Event_HLT_Mu17 = (theBitCorr[13]==-1) ? -1 : triggerResults->accept(theBitCorr[13]);
    T_Event_HLT_Mu8 = (theBitCorr[14]==-1) ? -1 : triggerResults->accept(theBitCorr[14]);
    T_Event_HLT_Mu8_Ele17 = (theBitCorr[15]==-1) ? -1 : triggerResults->accept(theBitCorr[15]);
    T_Event_HLT_Ele8_Mu17 = (theBitCorr[16]==-1) ? -1 : triggerResults->accept(theBitCorr[16]);
    T_Event_HLT_IsoMu24 = (theBitCorr[17]==-1) ? -1 : triggerResults->accept(theBitCorr[17]);
    T_Event_HLT_IsoMu24_2p1 = (theBitCorr[18]==-1) ? -1 : triggerResults->accept(theBitCorr[18]);
    
    /// fill the in selected Objet the HLT filter we will use for the matching
    trigger::TriggerObjectCollection allTriggerObjects = triggerSummary->getObjects();
    trigger::TriggerObjectCollection selectedObjects;
    vector<int> theHLTcorr;
    for (size_t t=0; t<moduleLabels.size(); t++) {
        
        size_t filterIndex = (*triggerSummary).filterIndex(moduleLabels[t]);
        if (filterIndex < (*triggerSummary).sizeFilters()) {
            const trigger::Keys &keys = (*triggerSummary).filterKeys(filterIndex);
            
            for (size_t j = 0; j < keys.size(); j++) {
                trigger::TriggerObject foundObject = (allTriggerObjects)[keys[j]];
                selectedObjects.push_back(foundObject);
                theHLTcorr.push_back(t);
            }
        }
    }
    


    

    
    
    //  now fill event content
    
    T_Event_RunNumber = iEvent.id().run();
	//cout << "coucou on est dans le run " << T_Event_RunNumber << endl;
    T_Event_EventNumber = iEvent.id().event();
    T_Event_LuminosityBlock = iEvent.id().luminosityBlock(); 
    
    T_Event_nPU =-1;
    T_Event_nPUp=-1;
    T_Event_nPUm=-1;
    T_Event_AveNTruePU=-1.;
    T_Event_ptHat = -1.;
    T_Event_processID = -1.;
    
    float truePu=0.;
    
    if(isMC_){
           edm::Handle<GenEventInfoProduct> genEvent;
           iEvent.getByLabel("generator", genEvent);
        
           iEvent.getByLabel( "genParticles", genParticles );
    //    theGenParts = genParticles;


        T_Event_processID = genEvent->signalProcessID();
        if ( genEvent->binningValues().size()>0 ) T_Event_ptHat = genEvent->binningValues()[0];
        Handle<std::vector< PileupSummaryInfo > > puInfo;
        try {
            iEvent.getByLabel("addPileupInfo",puInfo);
            std::vector<PileupSummaryInfo>::const_iterator PVI;
            //The in-time crossing is getBunchCrossing = 0; negative ones are early, positive ones are late.
            for(PVI = puInfo->begin(); PVI != puInfo->end(); ++PVI) {
                
                //    std::cout << " Pileup Information: bunchXing, nvtx: " << PVI->getBunchCrossing() << " " << PVI->getPU_NumInteractions() << std::endl;
                if(PVI->getBunchCrossing()==0){
                    T_Event_nPU =PVI->getPU_NumInteractions();
                    T_Event_nTruePU=PVI->getTrueNumInteractions();
                    
                }
                
                else if(PVI->getBunchCrossing()==-1){
                    T_Event_nPUm=PVI->getPU_NumInteractions();
                }
                else if(PVI->getBunchCrossing()==1){
                    T_Event_nPUp=PVI->getPU_NumInteractions();
                }
                truePu += PVI->getTrueNumInteractions();
            }
        } catch (...) {}
        if (doMuMuGammaMCtruth_){
       /*     const reco::GenParticle candidatePhoton;
            const reco::GenParticle candidateMuonNear;
            const reco::GenParticle candidateMuonFar;
            
            int theNbOfGen = genParticles->size();
            for (int j=0 ; j < theNbOfGen; j++){
                const reco::GenParticle & p = (*genParticles)[j];
                //
               if ((fabs(p.pdgId())==13)){
                    cout << "find muon " ;
                    const reco::Candidate * mom = p.mother();
                    cout << "muon status=" << p.status();
                    cout << " phi=" << p.phi() << " eta=" << p.eta();
                    cout << " muon mother type="<< mom->pdgId();
                    cout << " status=" << mom->status();
                    cout << " phi=" <<  mom->phi() << " eta=" <<  mom->eta() << endl;
                    //if ((fabs(mom->pdgId())==13)&&(mom->status()==3)){
                    //    cout <
                    //}
                }
                if ((fabs(p.pdgId())==22)&&(p.status()==1)){
                    const reco::Candidate * mom = p.mother();
                    if (fabs(mom->pdgId())==13){
                        cout << "find photon " ;
                        cout << "photon status=" << p.status();
                        cout << " phi=" << p.phi() << " eta=" << p.eta();
                        cout << " photon  mother type="<< mom->pdgId();
                        cout << " status=" << mom->status();
                        cout << " phi=" <<  mom->phi() << " eta=" <<  mom->eta() << endl;
                        const reco::Candidate *theCand = &p;
                        while (theCand->numberOfMothers()>0){
                            const reco::Candidate *proviCand = theCand->mother();
                            *theCand = *proviCand;
                        }
                        
                    }
                    //if ((fabs(mom->pdgId())==13)&&(mom->status()==3)){
                    //    cout <
                    //}
                }*/
/*                int nbOfCandPhotons = photonCandidate.size();
                for (int k=0 ; k<nbOfCandPhotons ; k++){
                    for (int j=0 ; j < theNbOfGen; j++){
                        const reco::GenParticle & p = (*genParticles)[j];
                        const reco::Candidate * mom = photonCandidate[j].mother();
                        cout << "the part it " << p.pdgId() << endl;
                        cout << "the mother it " << mom->pdgId() << endl;
                    }
                }*/
         //   }
        }
    }
    T_Event_AveNTruePU=truePu/3.;
    
    // loop on the vertices 
    if (vtxs.size() != 0){
        for (size_t i=0; i < vtxs.size(); i++){
            T_Vertex_z->push_back(vtxs[i].z());
            T_Vertex_y->push_back(vtxs[i].y());
            T_Vertex_x->push_back(vtxs[i].x());
            T_Vertex_Chi2Prob->push_back(ChiSquaredProbability(vtxs[i].chi2(),vtxs[i].ndof()));
            T_Vertex_rho->push_back( vtxs[i].position().Rho());
            T_Vertex_ndof->push_back(vtxs[i].ndof());
            T_Vertex_isFake->push_back(vtxs[i].isFake());
            T_Vertex_tracksSize->push_back(vtxs[i].tracksSize());      
        }
    } 
    
    
    
    if (doElectrons_){
        // loop on electrons
        unsigned int n = els_h->size();
    //    cout << "nb of electrons = " << n << endl;
        for(unsigned int i = 0; i < n; ++i) {
            
            // get reference to electron
            reco::GsfElectronRef ele(els_h, i);
            
            if (isMC_) doMCtruth(ele, genParticles, 0.3);
            
          //  cout << " le electron, eta=" << ele->eta() << " phi=" << ele->phi() << endl;
            
            T_Elec_Eta->push_back(ele->eta());
            T_Elec_Pt->push_back(ele->pt());
            T_Elec_Px->push_back(ele->px());
            T_Elec_Py->push_back(ele->py());
            T_Elec_Pz->push_back(ele->pz());
            T_Elec_Energy->push_back(ele->energy());
            T_Elec_Charge->push_back(ele->charge());

            T_Elec_nBrems->push_back(ele->numberOfBrems());
            T_Elec_fBrem->push_back(ele->fbrem());
            T_Elec_eSuperClusterOverP->push_back(ele->eSuperClusterOverP());
            T_Elec_vz->push_back(ele->vz());
            T_Elec_vy->push_back(ele->vy());
            T_Elec_vx->push_back(ele->vx());	

            T_Elec_SC_Et->push_back( ele->superCluster()->energy()/TMath::CosH(ele->superCluster()->eta()));
            T_Elec_SC_Eta->push_back( ele->superCluster()->eta());
        float etaSC = ele->superCluster()->eta();

            T_Elec_sigmaIetaIeta ->push_back( ele->sigmaIetaIeta());
            T_Elec_deltaPhiIn->push_back( ele->deltaPhiSuperClusterTrackAtVtx());
            T_Elec_deltaEtaIn->push_back( ele->deltaEtaSuperClusterTrackAtVtx());
            T_Elec_isEcalDriven -> push_back(ele->ecalDrivenSeed());
            T_Elec_HtoE ->push_back(ele->hadronicOverEm());

        T_Elec_dr03TkSumPt->push_back(ele->dr03TkSumPt());
        T_Elec_dr03EcalSumEt->push_back(ele->dr03EcalRecHitSumEt());
        T_Elec_dr03HcalSumEt->push_back(ele->dr03HcalTowerSumEt());

            T_Elec_isEB->push_back(ele->isEB());
            T_Elec_isEE->push_back(ele->isEE());
            
            // conversion rejection variables
            bool vtxFitConversion = ConversionTools::hasMatchedConversion(*ele, conversions_h, beamSpot.position());
            float mHits = ele->gsfTrack()->trackerExpectedHitsInner().numberOfHits(); 
            float missingHist = ele->gsfTrack()->trackerExpectedHitsInner().numberOfLostHits();
            T_Elec_passConversionVeto->push_back(vtxFitConversion);
            T_Elec_nHits->push_back(mHits);
            T_Elec_nLost->push_back(missingHist);
            
            double iso_ch =  (*(isoVals)[0])[ele];
            double iso_em = (*(isoVals)[1])[ele];
            double iso_nh = (*(isoVals)[2])[ele];
            double iso_chAll = (*(isoVals)[3])[ele];
            double iso_chPU = (*(isoVals)[4])[ele];
            double iso_ch04 =  (*(isoVals)[5])[ele];
            double iso_em04 = (*(isoVals)[6])[ele];
            double iso_nh04 = (*(isoVals)[7])[ele];
            double iso_chAll04 = (*(isoVals)[8])[ele];
            double iso_chPU04 = (*(isoVals)[9])[ele];
            T_Elec_photonIso->push_back(iso_em);
            T_Elec_neutralHadronIso->push_back(iso_nh);
            T_Elec_chargedHadronIso->push_back(iso_ch);
            T_Elec_allChargedHadronIso->push_back(iso_chAll);
            T_Elec_puChargedHadronIso->push_back(iso_chPU);
            
            
            T_Elec_photonIso04->push_back(iso_em04);
            T_Elec_neutralHadronIso04->push_back(iso_nh04);
            T_Elec_chargedHadronIso04->push_back(iso_ch04);
            T_Elec_allChargedHadronIso04->push_back(iso_chAll04);
            T_Elec_puChargedHadronIso04->push_back(iso_chPU04);
            
            float rhoPlus = std::max(0.0, Rho);
            // effective area for isolation
            float AEff = ElectronEffectiveArea::GetElectronEffectiveArea(ElectronEffectiveArea::kEleGammaAndNeutralHadronIso04, etaSC, ElectronEffectiveArea::kEleEAData2011);
            
            float isoSum04 = iso_ch04 + std::max(iso_em04 + iso_nh04 - rhoPlus * AEff, 0.0); 
        
            T_Elec_CombIsoHWW->push_back(isoSum04);  
            
            // working points
            bool veto       = EgammaCutBasedEleId::PassWP(EgammaCutBasedEleId::VETO, ele, conversions_h, beamSpot, vtx_h, iso_ch, iso_em, iso_nh, rhoIso);
            bool loose      = EgammaCutBasedEleId::PassWP(EgammaCutBasedEleId::LOOSE, ele, conversions_h, beamSpot, vtx_h, iso_ch, iso_em, iso_nh, rhoIso);
            bool medium     = EgammaCutBasedEleId::PassWP(EgammaCutBasedEleId::MEDIUM, ele, conversions_h, beamSpot, vtx_h, iso_ch, iso_em, iso_nh, rhoIso);
            bool tight      = EgammaCutBasedEleId::PassWP(EgammaCutBasedEleId::TIGHT, ele, conversions_h, beamSpot, vtx_h, iso_ch, iso_em, iso_nh, rhoIso);
            
            // eop/fbrem cuts for extra tight ID
            bool fbremeopin = EgammaCutBasedEleId::PassEoverPCuts(ele);
            
            // cuts to match tight trigger requirements
            bool trigtight = EgammaCutBasedEleId::PassTriggerCuts(EgammaCutBasedEleId::TRIGGERTIGHT, ele);
            
            // for 2011 WP70 trigger
            bool trigwp70 = EgammaCutBasedEleId::PassTriggerCuts(EgammaCutBasedEleId::TRIGGERWP70, ele);
            
            T_Elec_passVeto->push_back(veto);
            T_Elec_passLoose->push_back(loose);
            T_Elec_passMedium->push_back(medium);
            T_Elec_passTight->push_back(tight);
            T_Elec_passFbremopin->push_back(fbremeopin);
            T_Elec_passtrigTight->push_back(trigtight);
            T_Elec_passtrigwp70->push_back(trigwp70);
            
            bool isTriggering = trainTrigPresel(*ele);
            //T_Elec_isTrig->push_back();
             double myMVANonTrigMethod = myMVANonTrig->mvaValue(*ele,*pv,thebuilder,lazyTools,debugMVAclass);
            double myMVATrigMethod= myMVATrig->mvaValue(*ele,*pv,thebuilder,lazyTools,debugMVAclass);
            
            double isomva = fElectronIsoMVA->mvaValue( *ele, vtx_h->at(0), 
                                                      inPfCands, Rho, 
                                                      ElectronEffectiveArea::kEleEAData2011,
                                                      IdentifiedElectrons, IdentifiedMuons);
            T_Elec_isTrig->push_back(isTriggering);
            T_Elec_MVAid_trig->push_back(myMVATrigMethod);
            T_Elec_MVAid_Nontrig->push_back(myMVANonTrigMethod);
            T_Elec_Mvaiso->push_back(isomva);
            
            if (doPFPATmatching_){
            /// look if the electron is a PF one
                T_Elec_isPF->push_back(0);
                for( size_t iElectron = 0; iElectron < electrons->size(); ++iElectron ) {
                    float PFdeltaR = deltaR(ele->phi(), electrons->at( iElectron ).phi(), ele->eta(), electrons->at( iElectron ).eta());
                    if (PFdeltaR>0.1) continue;
                    T_Elec_isPF->push_back(1);
                    T_Elec_PFenergy->push_back(electrons->at( iElectron ).energy());
                    T_Elec_PFeta->push_back(electrons->at( iElectron ).eta());
                    T_Elec_PFphi->push_back(electrons->at( iElectron ).phi());
                    T_Elec_PFpt->push_back(electrons->at( iElectron ).pt());
                    T_Elec_PFpx->push_back(electrons->at( iElectron ).px());
                    T_Elec_PFpy->push_back(electrons->at( iElectron ).py());
                    T_Elec_PFpz->push_back(electrons->at( iElectron ).pz());
                    T_Elec_PFmva->push_back(electrons->at( iElectron ).userFloat("mvaTrigV0"));
                }
            }
            
            double theRadIso = GetRadialIsoValue(*ele, inPfCands);
            T_Elec_RadialIso->push_back(theRadIso);
            
            double theRadIsoVeto = GetRadialIsoValueVeto(*ele, inPfCands);
            T_Elec_RadialIsoVeto->push_back(theRadIsoVeto);
            
            double theRadIsoVetoMore = GetRadialIsoValueVetoMore(*ele, inPfCands, IdentifiedElectrons, IdentifiedMuons);
            T_Elec_RadialIsoVetoMore->push_back(theRadIsoVetoMore);
            
            fillIsoRings(*ele, vtx_h->at(0), 
            inPfCands, Rho, 
            ElectronEffectiveArea::kEleEAData2011,
            IdentifiedElectrons, IdentifiedMuons);
            
            int pass_Elec_HLT_Elec27_WP80 = 0;
            int pass_Elec_HLT_Ele17TightID_Ele8_Ele8Leg = 0;
            int pass_Elec_HLT_Ele17TightID_Ele8_Ele17Leg = 0;
            int pass_Elec_HLT_Ele17_Ele8_Ele8Leg = 0;
            int pass_Elec_HLT_Ele17_Ele8_Ele17Leg = 0;
            int pass_Elec_HLT_Ele17_Ele8_TnP_Ele8Leg = 0;
            int pass_Elec_HLT_Ele17_Ele8_TnP_Ele17Leg = 0;
            int pass_Elec_HLT_Ele20_SC4_TnP_SC4Leg = 0;
            int pass_Elec_HLT_Ele20_SC4_TnP_Ele20Leg = 0;
            int pass_Elec_HLT_Mu8_Ele17_Ele17Leg = 0;
            int pass_Elec_HLT_Ele8_Mu17_Ele8Leg = 0;
            
            for (size_t t = 0 ; t < selectedObjects.size() ; t++){
          //    cout << "eta = " << selectedObjects[t].eta() << " phi = " << selectedObjects[t].phi() << "filter = " << HLT_triggerObjects[theHLTcorr[t]] << endl;
                float HLTdeltaR = deltaR(ele->phi(), selectedObjects[t].phi(), ele->eta(), selectedObjects[t].eta());
          //  cout << "delta R =" << HLTdeltaR << endl;
                if (HLTdeltaR < 0.3){
          //     cout << "coucou on passe = " << theHLTcorr[t] << endl;
                    if (theHLTcorr[t] == 0) pass_Elec_HLT_Elec27_WP80 = 1;
                    if (theHLTcorr[t] == 1) pass_Elec_HLT_Ele17TightID_Ele8_Ele8Leg = 1; 
                    if (theHLTcorr[t] == 2) pass_Elec_HLT_Ele17TightID_Ele8_Ele17Leg = 1; 
                    if (theHLTcorr[t] == 3) pass_Elec_HLT_Ele17_Ele8_TnP_Ele8Leg = 1; 
                    if (theHLTcorr[t] == 4) pass_Elec_HLT_Ele17_Ele8_TnP_Ele17Leg = 1; 
                    if (theHLTcorr[t] == 5) pass_Elec_HLT_Ele20_SC4_TnP_SC4Leg = 1; 
                    if (theHLTcorr[t] == 6) pass_Elec_HLT_Ele20_SC4_TnP_Ele20Leg = 1;
                    if (theHLTcorr[t] == 7) pass_Elec_HLT_Ele17_Ele8_Ele8Leg = 1;
                    if (theHLTcorr[t] == 8) pass_Elec_HLT_Ele17_Ele8_Ele17Leg = 1;
                    if (theHLTcorr[t] == 15) pass_Elec_HLT_Mu8_Ele17_Ele17Leg = 1;
                    if (theHLTcorr[t] == 17) pass_Elec_HLT_Ele8_Mu17_Ele8Leg = 1;
               }
            }
            T_Elec_HLT_Elec27_WP80->push_back(pass_Elec_HLT_Elec27_WP80);
            T_Elec_HLT_Ele17TightID_Ele8_Ele8Leg->push_back(pass_Elec_HLT_Ele17TightID_Ele8_Ele8Leg);
            T_Elec_HLT_Ele17TightID_Ele8_Ele17Leg->push_back(pass_Elec_HLT_Ele17TightID_Ele8_Ele17Leg);
            T_Elec_HLT_Ele17_Ele8_Ele8Leg->push_back(pass_Elec_HLT_Ele17_Ele8_Ele8Leg);
            T_Elec_HLT_Ele17_Ele8_Ele17Leg->push_back(pass_Elec_HLT_Ele17_Ele8_Ele17Leg);
            T_Elec_HLT_Ele17_Ele8_TnP_Ele17Leg->push_back(pass_Elec_HLT_Ele17_Ele8_TnP_Ele17Leg);
            T_Elec_HLT_Ele17_Ele8_TnP_Ele8Leg->push_back(pass_Elec_HLT_Ele17_Ele8_TnP_Ele8Leg);
            T_Elec_HLT_Ele20_SC4_TnP_Ele20Leg->push_back(pass_Elec_HLT_Ele20_SC4_TnP_Ele20Leg);
            T_Elec_HLT_Ele20_SC4_TnP_SC4Leg->push_back(pass_Elec_HLT_Ele20_SC4_TnP_SC4Leg);
            T_Elec_HLT_Mu8_Ele17_Ele17Leg->push_back(pass_Elec_HLT_Mu8_Ele17_Ele17Leg);
            T_Elec_HLT_Ele8_Mu17_Ele8Leg->push_back(pass_Elec_HLT_Ele8_Mu17_Ele8Leg);
            
            bool validKF= false; 
            reco::TrackRef myTrackRef = ele->closestCtfTrackRef();
            validKF = (myTrackRef.isAvailable());
            validKF = (myTrackRef.isNonnull());  
            
            T_Elec_kfchi2->push_back((validKF) ? myTrackRef->normalizedChi2() : 0 );
            T_Elec_kfhits->push_back((validKF) ? myTrackRef->hitPattern().trackerLayersWithMeasurement() : -1.);
            T_Elec_gsfchi2->push_back(ele->gsfTrack()->normalizedChi2());
            T_Elec_detacalo->push_back(ele->deltaEtaSeedClusterTrackAtCalo());
            T_Elec_see->push_back(ele->sigmaIetaIeta());
            std::vector<float> vCov = lazyTools.localCovariances(*(ele->superCluster()->seed())) ;
            if (!isnan(vCov[2])) T_Elec_spp->push_back(sqrt(vCov[2]));   //EleSigmaIPhiIPhi
            else T_Elec_spp->push_back(0.);
            T_Elec_etawidth->push_back(ele->superCluster()->etaWidth());
            T_Elec_phiwidth->push_back(ele->superCluster()->phiWidth());
            T_Elec_e1x5e5x5->push_back((ele->e5x5()) !=0. ? 1.-(ele->e1x5()/ele->e5x5()) : -1.);
            T_Elec_R9->push_back(lazyTools.e3x3(*(ele->superCluster()->seed())) / ele->superCluster()->rawEnergy());
            T_Elec_EoP->push_back(ele->eSuperClusterOverP());
            T_Elec_ESeedoP->push_back(ele->eSeedClusterOverP());
            T_Elec_ESeedoPout->push_back(ele->eSeedClusterOverPout());
            T_Elec_IoEmIoP->push_back((1.0/ele->ecalEnergy()) - (1.0 / ele->p()));
            T_Elec_eleEoPout->push_back(ele->eEleClusterOverPout());
            T_Elec_PreShowerOverRaw->push_back(ele->superCluster()->preshowerEnergy() / ele->superCluster()->rawEnergy());
            T_Elec_EcalEnergy->push_back(ele->ecalEnergy());
            T_Elec_TrackPatVtx->push_back(ele->trackMomentumAtVtx().R());
            
            bool isPassingMVA = passMVAcuts((*ele), myMVATrigMethod);
            T_Elec_passMVA->push_back(isPassingMVA);
            
            bool isPassingFO = passFOcuts((*ele), vtx_h->at(0), vtxFitConversion);
            T_Elec_isFO->push_back(isPassingFO);
            
            float fMVAVar_d0;
            if (ele->gsfTrack().isNonnull()) {
                fMVAVar_d0 = (-1.0)*ele->gsfTrack()->dxy(vtx_h->at(0).position()); 
            } else if (ele->closestCtfTrackRef().isNonnull()) {
                fMVAVar_d0 = (-1.0)*ele->closestCtfTrackRef()->dxy(vtx_h->at(0).position()); 
            } else {
                fMVAVar_d0 = -9999.0;
            }
            T_Elec_d0->push_back(fMVAVar_d0);
            
            float fMVAVar_ip3d = -999.0; 
            // fMVAVar_ip3dSig = 0.0;
            if (ele->gsfTrack().isNonnull()) {
                const double gsfsign   = ( (-ele->gsfTrack()->dxy(vtx_h->at(0).position()))   >=0 ) ? 1. : -1.;
                
               const reco::TransientTrack &tt = thebuilder.build(ele->gsfTrack()); 
               const std::pair<bool,Measurement1D> &ip3dpv =  IPTools::absoluteImpactParameter3D(tt,vtx_h->at(0));
                if (ip3dpv.first) {
                    double ip3d = gsfsign*ip3dpv.second.value();
                    //double ip3derr = ip3dpv.second.error();  
                    fMVAVar_ip3d = ip3d; 
                    // fMVAVar_ip3dSig = ip3d/ip3derr;
                }
            }
            T_Elec_IP3D->push_back(fMVAVar_ip3d);
            float dzvtx = 0;
            if (vtx_h->size() > 0) {
                reco::VertexRef vtx(vtx_h, 0);    
                dzvtx = ele->gsfTrack()->dz(vtx->position());
            } else {
                dzvtx = ele->gsfTrack()->dz();
            }
            T_Elec_dZ->push_back(dzvtx);
        
        }
    }

    T_METPF_ET = metsPF[0].pt();
    T_METPF_Phi = metsPF[0].phi();
    T_METPF_Sig = metsPF[0].significance();
   /* T_METPFTypeI_ET = metsPFTypeI[0].pt();
    T_METPFTypeI_Phi = metsPFTypeI[0].phi();*/
    
    
    ///now fill the jets collections 
  /*  JetCorrectorParameters *ResJetPar,*L3JetPar,*L2JetPar,*L1JetPar;
	std::vector<JetCorrectorParameters> vPar;
	if (!(isMC_)){
        ResJetPar = new JetCorrectorParameters("../data/data_L2L3Residual_AK5PF.txt"); 
        L3JetPar  = new JetCorrectorParameters("../data/data_L3Absolute_AK5PF.txt");
        L2JetPar  = new JetCorrectorParameters("../data/data_L2Relative_AK5PF.txt");
        L1JetPar  = new JetCorrectorParameters("../data/data_L1FastJet_AK5PF.txt");
        //  Load the JetCorrectorParameter objects into a vector, IMPORTANT: THE ORDER MATTERS HERE !!!! 
        vPar.push_back(*L1JetPar);
        vPar.push_back(*L2JetPar);
        vPar.push_back(*L3JetPar);
        vPar.push_back(*ResJetPar);
	}
	else{
        L3JetPar  = new JetCorrectorParameters("../data/MC_L3Absolute_AK5PF.txt");
        L2JetPar  = new JetCorrectorParameters("../data/MC_L2Relative_AK5PF.txt");
        L1JetPar  = new JetCorrectorParameters("../data/MC_L1FastJet_AK5PF.txt");
        //  Load the JetCorrectorParameter objects into a vector, IMPORTANT: THE ORDER MATTERS HERE !!!!
        vPar.push_back(*L1JetPar);
        vPar.push_back(*L2JetPar);
        vPar.push_back(*L3JetPar);
	}
    
    FactorizedJetCorrector *JetCorrector = new FactorizedJetCorrector(vPar);*/
    
    for (int k = 0 ; k < nJets ; k++){
        const reco::Jet* jet = (const reco::Jet*) ( & ((*recoPFJets)[k]) );
        double correction = 1.0;
        
      /*  JetCorrector->setJetEta(jet->eta());
        JetCorrector->setJetPt(jet->pt());
        JetCorrector->setJetA(jet->jetArea());
        JetCorrector->setRho(Rho);
        correction = JetCorrector->getCorrection();*/
     /*   T_Jet_Px->push_back(jet->px()*correction);
        T_Jet_Py->push_back(jet->py()*correction);
        T_Jet_Pz->push_back(jet->pz()*correction);
        T_Jet_Eta->push_back(jet->eta());
        T_Jet_Et->push_back(jet->et()*correction);
        T_Jet_Energy->push_back(jet->energy()*correction);
        T_Jet_Phi->push_back(jet->phi());
        T_Jet_Corr->push_back(correction);*/
        T_Jet_Px->push_back(jet->px());
        T_Jet_Py->push_back(jet->py());
        T_Jet_Pz->push_back(jet->pz());
        T_Jet_Eta->push_back(jet->eta());
        T_Jet_Et->push_back(jet->et());
        T_Jet_Energy->push_back(jet->energy());
        T_Jet_Phi->push_back(jet->phi());
        T_Jet_Corr->push_back(correction);
    }
	
	if (doMuons_){
		int nbMuons = recoMuons->size();
		//cout << "il y a " << nbMuons << " muons " << endl;
		//loop on the muons in the event 
		for (int k = 0 ; k < nbMuons ; k++){
	
          const reco::Muon* muon = &((*recoMuons)[k]);
          //  cout << "le muon : eta=" << muon->eta() << " phi=" << muon->phi() << endl;
            if (isMC_) doMCtruthMuons(muon, genParticles, 0.3);

           T_Muon_Eta->push_back(muon->eta());
            T_Muon_Phi->push_back(muon->phi());
            T_Muon_IsGlobalMuon->push_back(muon->isGlobalMuon());
            T_Muon_IsPFMuon->push_back(muon->isPFMuon());
            T_Muon_IsTrackerMuon->push_back(muon->isTrackerMuon());
            T_Muon_IsCaloMuon->push_back(muon->isCaloMuon());
            T_Muon_IsStandAloneMuon->push_back(muon->isStandAloneMuon());
            T_Muon_IsMuon->push_back(muon->isMuon());
            T_Muon_Energy->push_back(muon->energy());
            T_Muon_Et->push_back(muon->et());
            T_Muon_Pt->push_back(muon->pt());
            T_Muon_Px->push_back(muon->px());
            T_Muon_Py->push_back(muon->py());
            T_Muon_Pz->push_back(muon->pz());
            T_Muon_Mass->push_back(muon->mass());
            T_Muon_charge->push_back(muon->charge());
            
            T_Muon_numberOfChambers->push_back(muon->numberOfChambers());
            T_Muon_numberOfChambersRPC->push_back(muon->numberOfChambersNoRPC());
            T_Muon_numberOfMatches->push_back(muon->numberOfMatches());
            T_Muon_numberOfMatchedStations->push_back(muon->numberOfMatchedStations());
            bool isMatchTheStation = muon::isGoodMuon(*muon, muon::TMOneStationTight);
            bool isGlobalMuonPT = muon::isGoodMuon(*muon, muon::GlobalMuonPromptTight);
            bool isGlobalMuonArbitrated = muon::isGoodMuon(*muon, muon::TrackerMuonArbitrated);
            T_Muon_TMLastStationTight->push_back(isMatchTheStation);
            T_Muon_IsGlobalMuon_PromptTight->push_back(isGlobalMuonPT);
            T_Muon_IsTrackerMuonArbitrated->push_back(isGlobalMuonArbitrated);
            
            if (muon->globalTrack().isNull()) T_Muon_globalTrackChi2->push_back(-1); else T_Muon_globalTrackChi2->push_back(muon->globalTrack()->normalizedChi2());
            if (muon->globalTrack().isNull()) T_Muon_validMuonHits->push_back(-1); else T_Muon_validMuonHits->push_back(muon->globalTrack()->hitPattern().numberOfValidMuonHits());
            T_Muon_trkKink->push_back(muon->combinedQuality().trkKink);
            if (muon->muonBestTrack().isNull()) {
                T_Muon_trkNbOfTrackerLayers->push_back(-1);
                T_Muon_trkError->push_back(-1);
                T_Muon_dB->push_back(-1);
                T_Muon_dBstop->push_back(-1);
                T_Muon_dzPV->push_back(-1);
                T_Muon_dzstop->push_back(-1);
                T_Muon_trkValidPixelHits->push_back(-1);
                T_Muon_trkNbOfValidTrackeHits->push_back(-1);
            }
            else {
                T_Muon_trkNbOfTrackerLayers->push_back(muon->muonBestTrack()->hitPattern().trackerLayersWithMeasurement());
                T_Muon_trkError->push_back(muon->muonBestTrack()->ptError());
                T_Muon_trkValidPixelHits->push_back(muon->muonBestTrack()->hitPattern().numberOfValidPixelHits());
                T_Muon_dB->push_back(fabs(muon->muonBestTrack()->dxy(pv->position())));
                T_Muon_dBstop->push_back(fabs(muon->muonBestTrack()->dxy(pv->position())));
                T_Muon_dzPV->push_back(fabs(muon->muonBestTrack()->dz(pv->position())));
                T_Muon_dzstop->push_back(fabs(muon->muonBestTrack()->dz(pv->position())));
                T_Muon_trkNbOfValidTrackeHits->push_back(muon->muonBestTrack()->hitPattern().numberOfValidTrackerHits());
            }
            T_Muon_isoR03_emEt->push_back(muon->isolationR03().emEt);
            T_Muon_isoR03_hadEt->push_back(muon->isolationR03().hadEt);
            T_Muon_isoR03_hoEt->push_back(muon->isolationR03().hoEt);
            T_Muon_isoR03_sumPt->push_back(muon->isolationR03().sumPt);
            T_Muon_isoR03_nTracks->push_back(muon->isolationR03().nTracks);
            T_Muon_isoR03_nJets->push_back(muon->isolationR03().nJets);
            T_Muon_chargedHadronIsoR04->push_back(muon->pfIsolationR04().sumChargedHadronPt);
            T_Muon_neutralHadronIsoR04->push_back(muon->pfIsolationR04().sumNeutralHadronEt);
            T_Muon_photonIsoR04->push_back(muon->pfIsolationR04().sumPhotonEt);
            T_Muon_chargedHadronIsoPUR04->push_back(muon->pfIsolationR04().sumPUPt);
            T_Muon_chargedHadronIsoR03->push_back(muon->pfIsolationR03().sumChargedHadronPt);
            T_Muon_neutralHadronIsoR03->push_back(muon->pfIsolationR03().sumNeutralHadronEt);
            T_Muon_photonIsoR03->push_back(muon->pfIsolationR03().sumPhotonEt);
            T_Muon_chargedHadronIsoPUR03->push_back(muon->pfIsolationR03().sumPUPt);
            
            
            float theMVAvalue = PFisolationMVA(muon,  inPfCands, pv, T_Event_Rho);
            T_Muon_isoRingsMVA->push_back(theMVAvalue);
            
            /// now do HLT matching for muons
            int pass_HLT_Mu17_TkMu8_Mu17Leg = 0;
            int pass_HLT_Mu17_TkMu8_Mu8Leg = 0;
            int pass_HLT_Mu17_Mu8_Mu17Leg = 0;
            int pass_HLT_Mu17_Mu8_Mu8Leg = 0;
            int pass_HLT_Mu17_Mu17_obj = 0;
            int pass_HLT_Mu17_Mu8_obj = 0;
            int pass_HLT_Mu8_Ele17_Mu8Leg= 0;
            int pass_HLT_Ele8_Mu17_Mu17Leg = 0;
            int pass_HLT_IsoMu24 = 0;
            int pass_HLT_IsoMu24_2p1 = 0;

            
            for (size_t t = 0 ; t < selectedObjects.size() ; t++){
               // cout << "eta = " << selectedObjects[t].eta() << " phi = " << selectedObjects[t].phi() << "muon Pt =" << muon->pt()<< "filter = " << HLT_triggerObjects[theHLTcorr[t]] << endl;
               // cout << "pt trigger" << selectedObjects[t].pt() << endl;
                float HLTdeltaR = deltaR(muon->phi(), selectedObjects[t].phi(), muon->eta(), selectedObjects[t].eta());
		float relatDeltaPt = fabs(selectedObjects[t].pt()-muon->pt())/muon->pt();
               // cout << "delta R =" << HLTdeltaR << endl;
                //if (HLTdeltaR < 0.3){
                if ((HLTdeltaR < 0.3)&&(relatDeltaPt<0.5)){
               //     	cout << "coucou on passe = " << theHLTcorr[t] << endl;
                    if ((theHLTcorr[t] == 9)||(theHLTcorr[t] == 23))  pass_HLT_Mu17_TkMu8_Mu17Leg = 1;
                    if ((theHLTcorr[t] == 10)||(theHLTcorr[t] == 24)) pass_HLT_Mu17_TkMu8_Mu8Leg = 1;
                    if ((theHLTcorr[t] == 12)||(theHLTcorr[t] == 21)) pass_HLT_Mu17_Mu8_Mu17Leg = 1;
                    if ((theHLTcorr[t] == 11)||(theHLTcorr[t] == 22)) pass_HLT_Mu17_Mu8_Mu8Leg = 1;
                    if (theHLTcorr[t] == 13) pass_HLT_Mu17_Mu17_obj = 1;
                    if (theHLTcorr[t] == 14) pass_HLT_Mu17_Mu8_obj = 1;
                    if (theHLTcorr[t] == 16) pass_HLT_Mu8_Ele17_Mu8Leg = 1;
                    if (theHLTcorr[t] == 18) pass_HLT_Ele8_Mu17_Mu17Leg = 1;
                    if (theHLTcorr[t] == 19) pass_HLT_IsoMu24 = 1;
                    if (theHLTcorr[t] == 20) pass_HLT_IsoMu24_2p1 = 1;

                }
            }
            T_Muon_HLT_Mu17_TkMu8_Mu17Leg->push_back(pass_HLT_Mu17_TkMu8_Mu17Leg);
            T_Muon_HLT_Mu17_TkMu8_Mu8Leg->push_back(pass_HLT_Mu17_TkMu8_Mu8Leg);
            T_Muon_HLT_Mu17_Mu8_Mu17Leg->push_back(pass_HLT_Mu17_Mu8_Mu17Leg);
            T_Muon_HLT_Mu17_Mu8_Mu8Leg->push_back(pass_HLT_Mu17_Mu8_Mu8Leg);
            T_Muon_HLT_Mu17_obj->push_back(pass_HLT_Mu17_Mu17_obj);
            T_Muon_HLT_Mu8_obj->push_back(pass_HLT_Mu17_Mu8_obj);
            T_Muon_HLT_Mu8_Ele17_Mu8Leg->push_back(pass_HLT_Mu8_Ele17_Mu8Leg);
            T_Muon_HLT_Ele8_Mu17_Mu17Leg->push_back(pass_HLT_Ele8_Mu17_Mu17Leg);
            T_Muon_HLT_IsoMu24->push_back(pass_HLT_IsoMu24);
            T_Muon_HLT_IsoMu24_2p1->push_back(pass_HLT_IsoMu24_2p1);

           // if ((pass_HLT_Mu17_TkMu8_Mu17Leg&&muon->pt()<17)) cout << "/////////////////////////////////////////////////////////////////////////////////////// on a matché pass_HLT_Mu17_Mu8_Mu17Leg, le Pt du muon " << muon->pt() << endl;
		}
   /*     bool findApair = false;
        for (int i = 0 ; i<nbMuons ; i++){
            for (int j=i+1 ; j<nbMuons ; j++){
                if ((T_Muon_HLT_Mu17_Mu8_Mu17Leg->at(i)==1&&T_Muon_HLT_Mu17_Mu8_Mu8Leg->at(j)==1)||(T_Muon_HLT_Mu17_Mu8_Mu8Leg->at(i)==1&&T_Muon_HLT_Mu17_Mu8_Mu17Leg->at(j)==1)) findApair = true;
            }
        }
        if (T_Event_HLT_Mu17_Mu8==1&&(!findApair)) cout << "coucou on a trouve un pb de matching ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////" << endl;
*/
	}

    
    if (savePF_){
        //cout << "coucou, on ets dans savePF" << inPfCands.size() << endl;
        for (reco::PFCandidateCollection::const_iterator iP = inPfCands.begin(); iP != inPfCands.end(); ++iP) {
            bool shareAtrack = false;
            bool gammaWithElecMissHit = false;
            bool inAcone = false;
            reco::SuperClusterRef superClusterRefofPF;
            if (iP->pdgId()==22) superClusterRefofPF = iP->superClusterRef();
            for (reco::GsfElectronCollection::const_iterator iE = FOelectrons.begin();
                 iE != FOelectrons.end(); ++iE) {
                 double tmpDR = sqrt(pow(iP->eta() - iE->eta(),2) + pow(acos(cos(iP->phi() - iE->phi())),2));
                if (tmpDR<0.6){
                    inAcone = true;
                    if(iP->gsfTrackRef().isNonnull() && iE->gsfTrack().isNonnull() &&
                       refToPtr(iP->gsfTrackRef()) == refToPtr(iE->gsfTrack())) shareAtrack=true;
                    reco::SuperClusterRef superClusterRefofElectron = iE->superCluster();
                    if ((superClusterRefofPF.isNonnull())&&(superClusterRefofElectron.isNonnull())){
                        if (superClusterRefofPF==superClusterRefofElectron){
                            if (iE->gsfTrack().isNonnull()) {
                                if (iE->gsfTrack()->trackerExpectedHitsInner().numberOfHits() > 0 ) {gammaWithElecMissHit=true; cout << "bad PF photon" << endl;}
                            }
                        }
                    }
                }
            }
            for (reco::MuonCollection::const_iterator iM = IdentifiedMuons.begin();
                 iM != IdentifiedMuons.end(); ++iM) {
                double tmpDR = sqrt(pow(iP->eta() - iM->eta(),2) + pow(acos(cos(iP->phi() - iM->phi())),2));
                if (tmpDR<0.6){
                    inAcone = true;
                    if(iP->trackRef().isNonnull() && iM->innerTrack().isNonnull() &&
                       refToPtr(iP->trackRef()) == refToPtr(iM->innerTrack())) shareAtrack = true;
                }
            }
	    //inAcone=true;
            if (inAcone){
                T_PF_Et->push_back(iP->eta());
                T_PF_Pt->push_back(iP->pt());
                T_PF_Px->push_back(iP->px());
                T_PF_Py->push_back(iP->py());
                T_PF_Pz->push_back(iP->pz());
                T_PF_Eta->push_back(iP->eta());
                T_PF_Phi->push_back(iP->phi());
                T_PF_pdgID->push_back(iP->pdgId());
                T_PF_particleID->push_back(iP->particleId());
                if (shareAtrack) T_PF_hasTrack->push_back(1);
                else T_PF_hasTrack->push_back(0);
                int iVertex= -1; 
                if (iP->particleId()==reco::PFCandidate::h) iVertex = PFisCommingFromVertex((*iP), vtxs);
                if ((iVertex==-1)||(iVertex==0)) T_PF_isPU->push_back(0); else  T_PF_isPU->push_back(1);
                if (gammaWithElecMissHit) T_PF_hasTrackWithMissingHits->push_back(1); else T_PF_hasTrackWithMissingHits->push_back(0);
            }
        }
        
    }
    
    if (doPhotons_){
        int nbphoton = 0;
        for(reco::PhotonCollection::const_iterator aPho = photons->begin(); aPho != photons->end(); aPho++){
            T_Pho_Et->push_back(aPho->et());
            T_Pho_Energy->push_back(aPho->energy());
            T_Pho_Pt->push_back(aPho->pt());
            T_Pho_Px->push_back(aPho->px());
            T_Pho_Py->push_back(aPho->py());
            T_Pho_Pz->push_back(aPho->pz());
            
            T_Pho_Eta->push_back(aPho->eta());
            T_Pho_Phi->push_back(aPho->phi());
            
            T_Pho_r9->push_back(aPho->r9());
            T_Pho_sigmaIetaIeta->push_back(aPho->sigmaIetaIeta());
            
            reco::SuperClusterRef superCluster = aPho->superCluster();
            if ( superCluster.isNonnull() ) {
                T_Pho_SCEt->push_back(superCluster->energy()*sin(superCluster->position().theta()));
                T_Pho_SCEnergy->push_back(superCluster->energy());
                T_Pho_SCEta->push_back(superCluster->position().eta());
                T_Pho_SCPhi->push_back(superCluster->position().phi());
                T_Pho_EtaWidth->push_back(superCluster->etaWidth());
                T_Pho_PhiWidth->push_back(superCluster->phiWidth());
            }
            else {
                T_Pho_SCEt->push_back(-1);
                T_Pho_SCEnergy->push_back(-1);
                T_Pho_SCEta->push_back(-1);
                T_Pho_SCPhi->push_back(-1);
                T_Pho_EtaWidth->push_back(-1);
                T_Pho_PhiWidth->push_back(-1);
            }
            if (isMC_){
                int nbOfGen = genParticles->size();
                float minDiff= 100;
                int iteDiff = -1000;
                for (int j = 0 ; j < nbOfGen ; j++){
                    const reco::GenParticle & p = (*genParticles)[j];
                    float theDeltaR = deltaR(p.phi(), aPho->phi(), p.eta(), aPho->eta());
                    if (theDeltaR < minDiff){
                        minDiff = theDeltaR;
                        iteDiff = j;
                    }
                }
                if (iteDiff>=0){
                    T_Pho_isMatchedWithMC->push_back(1);
                    const reco::GenParticle & theCand = (*genParticles)[iteDiff];
                    const reco::Candidate * mom = theCand.mother();
                    T_Pho_Gen_PDGid->push_back(theCand.pdgId());
                    T_Pho_Gen_Status->push_back(theCand.status());
                    if (theCand.numberOfMothers()>0) T_Pho_Gen_MotherID->push_back(mom->pdgId());
                    else T_Pho_Gen_MotherID->push_back(-1);
                    
                }
                else {
                    T_Pho_isMatchedWithMC->push_back(0);
                    T_Pho_Gen_PDGid->push_back(-1);
                    T_Pho_Gen_Status->push_back(-1);
                    T_Pho_Gen_MotherID->push_back(-1);
                }
            }
            int itElectron = -1;
            bool SCfound  = false;
            for(reco::GsfElectronCollection::const_iterator gsfEle = els_h->begin(); gsfEle!=els_h->end(); ++gsfEle) {
                itElectron++;
               reco::SuperClusterRef SuperClusterElec = gsfEle->superCluster();
             //   cout << "electron " << itElectron << endl;
                if (SuperClusterElec == superCluster ) {  SCfound = true; break;}
            }
            if (!SCfound) itElectron = -1;
            T_Pho_indOfTheElec->push_back(itElectron);
        }
        
    }
    
    if (saveConversions_){
        int localIte = -1;
        for (reco::ConversionCollection::const_iterator conv = conversions_h->begin(); conv!= conversions_h->end(); ++conv) {
            localIte++;
            reco::Vertex vtx = conv->conversionVertex();
            reco::ConversionRef refConv(conversions_h,localIte);
            if (vtx.isValid()) {
                int iel=-1;
                bool foundAnElec = false;
                for(reco::GsfElectronCollection::const_iterator gsfEle = els_h->begin(); gsfEle!=els_h->end(); ++gsfEle) {
                    iel++;
                    if (ConversionTools::matchesConversion(*gsfEle, *conv)) {
                        foundAnElec = true;
                        break;
                    }
                }
                if (!foundAnElec) iel=-1;
                int ipho=-1;
                bool foundAPhoton = false;
                for(reco::PhotonCollection::const_iterator aPho = photons->begin(); aPho != photons->end(); aPho++){
                    ipho++;
                    reco::SuperCluster theSC = *(aPho->superCluster());
                    reco::ConversionRef theConv = ConversionTools::matchedConversion(theSC,conversions_h,beamSpot.position());
                    if (refConv == theConv) { foundAPhoton=true; break;}
                }
                if (!foundAPhoton) ipho=-1;
                if ((ipho==-1)&&(iel==-1)) continue;
                T_Conv_EleInd->push_back(iel);
                T_Conv_PhoInd->push_back(ipho);
                T_Conv_vtxProb->push_back(TMath::Prob( vtx.chi2(), vtx.ndof()));
                math::XYZVector mom(conv->refittedPairMomentum());
                double dbsx = vtx.x() - beamSpot.position().x();
                double dbsy = vtx.y() - beamSpot.position().y();
                T_Conv_lxy->push_back((mom.x()*dbsx + mom.y()*dbsy)/mom.rho());
                int nbHitMax = 0;
                for (std::vector<uint8_t>::const_iterator it = conv->nHitsBeforeVtx().begin(); it!=conv->nHitsBeforeVtx().end(); ++it) {
                    if ((*it)>nbHitMax) nbHitMax = (*it);
                }
                T_Conv_nHitsMax->push_back(nbHitMax);
            }
        
        }
    }
    mytree_->Fill();
    endEvent();
    //cout << "on a fini cet event ;) " << endl;
}



// ------------ method called once each job just before starting event loop  ------------
void 
ElecIdAnalyzer::beginJob()
{
    mytree_ = new TTree("eventsTree","");
    
   mytree_->Branch("T_Event_Rho", &T_Event_Rho, "T_Event_Rho/F");
   mytree_->Branch("T_Event_RhoIso", &T_Event_RhoIso, "T_Event_RhoIso/F");
   mytree_->Branch("T_Event_RhoNoPu", &T_Event_RhoNoPu, "T_Event_RhoNoPu/F");
   mytree_->Branch("T_Event_RhoIsoNoPu", &T_Event_RhoIsoNoPu, "T_Event_RhoIsoNoPu/F");
   mytree_->Branch("T_Event_RunNumber", &T_Event_RunNumber, "T_Event_RunNumber/I");
   mytree_->Branch("T_Event_EventNumber", &T_Event_EventNumber, "T_Event_EventNumber/I");
   mytree_->Branch("T_Event_LuminosityBlock", &T_Event_LuminosityBlock, "T_Event_LuminosityBlock/I");
   mytree_->Branch("T_Event_processID", &T_Event_processID, "T_Event_processID/I");    
   mytree_->Branch("T_Event_ptHat", &T_Event_ptHat, "T_Event_ptHat/I");    
   mytree_->Branch("T_Event_nPU", &T_Event_nPU, "T_Event_nPU/I");
   mytree_->Branch("T_Event_nTruePU", &T_Event_nTruePU, "T_Event_nTruePU/F");
   mytree_->Branch("T_Event_nPUm", &T_Event_nPUm, "T_Event_nPUm/I");
   mytree_->Branch("T_Event_nPUp", &T_Event_nPUp, "T_Event_nPUp/I");
   mytree_->Branch("T_Event_AveNTruePU", &T_Event_AveNTruePU, "T_Event_AveNTruePU/F"); 
   mytree_->Branch("T_Event_HLT_Ele27_WP80",&T_Event_HLT_Ele27_WP80,"T_Event_HLT_Ele27_WP80/I");
   mytree_->Branch("T_Event_HLT_Ele17_Ele8",&T_Event_HLT_Ele17_Ele8,"T_Event_HLT_Ele17_Ele8/I");
   mytree_->Branch("T_Event_HLT_Ele17_Ele8_M50_TnP",&T_Event_HLT_Ele17_Ele8_M50_TnP,"T_Event_HLT_Ele17_Ele8_M50_TnP/I");
   mytree_->Branch("T_Event_HLT_Ele20_SC4_M50_TnP",&T_Event_HLT_Ele20_SC4_M50_TnP,"T_Event_HLT_Ele20_SC4_M50_TnP/I");
   mytree_->Branch("T_Event_HLT_Ele22_CaloIdL_CaloIsoVL",&T_Event_HLT_Ele22_CaloIdL_CaloIsoVL,"T_Event_HLT_Ele22_CaloIdL_CaloIsoVL/I");
   mytree_->Branch("T_Event_HLT_Ele27_CaloIdL_CaloIsoVL_TrkIdVL_TrkIsoVL",&T_Event_HLT_Ele27_CaloIdL_CaloIsoVL_TrkIdVL_TrkIsoVL,"T_Event_HLT_Ele27_CaloIdL_CaloIsoVL_TrkIdVL_TrkIsoVL/I");
   mytree_->Branch("T_Event_HLT_Ele30_CaloIdVT_TrkIdT",&T_Event_HLT_Ele30_CaloIdVT_TrkIdT,"T_Event_HLT_Ele30_CaloIdVT_TrkIdT/I");
   mytree_->Branch("T_Event_HLT_Ele27_WP80_PFMET_MT50",&T_Event_HLT_Ele27_WP80_PFMET_MT50,"T_Event_HLT_Ele27_WP80_PFMET_MT50/I");
   mytree_->Branch("T_Event_HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralPFNoPUJet30",&T_Event_HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralPFNoPUJet30,"T_Event_HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralPFNoPUJet30/I");
    mytree_->Branch("T_Event_HLT_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL",&T_Event_HLT_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL,"T_Event_HLT_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL/I");
    mytree_->Branch("T_Event_HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL",&T_Event_HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL,"T_Event_HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL/I");
    mytree_->Branch("T_Event_HLT_Mu17_Mu8",&T_Event_HLT_Mu17_Mu8,"T_Event_HLT_Mu17_Mu8/I");
    mytree_->Branch("T_Event_HLT_Mu17_TkMu8",&T_Event_HLT_Mu17_TkMu8,"T_Event_HLT_Mu17_TkMu8/I");
    mytree_->Branch("T_Event_HLT_Mu17",&T_Event_HLT_Mu17,"T_Event_HLT_Mu17/I");
    mytree_->Branch("T_Event_HLT_Mu8",&T_Event_HLT_Mu8,"T_Event_HLT_Mu8/I");
    mytree_->Branch("T_Event_HLT_Mu8_Ele17",&T_Event_HLT_Mu8_Ele17,"T_Event_HLT_Mu8_Ele17/I");
    mytree_->Branch("T_Event_HLT_Ele8_Mu17",&T_Event_HLT_Ele8_Mu17,"T_Event_HLT_Ele8_Mu17/I");
    mytree_->Branch("T_Event_HLT_IsoMu24",&T_Event_HLT_IsoMu24,"T_Event_HLT_IsoMu24/I");
    mytree_->Branch("T_Event_HLT_IsoMu24_2p1",&T_Event_HLT_IsoMu24_2p1,"T_Event_HLT_IsoMu24_2p1/I");

    
    
   mytree_->Branch("T_Gen_Elec_Px", "std::vector<float>", &T_Gen_Elec_Px);
   mytree_->Branch("T_Gen_Elec_Py", "std::vector<float>", &T_Gen_Elec_Py);
   mytree_->Branch("T_Gen_Elec_Pz", "std::vector<float>", &T_Gen_Elec_Pz);
   mytree_->Branch("T_Gen_Elec_Energy", "std::vector<float>", &T_Gen_Elec_Energy);
   mytree_->Branch("T_Gen_Elec_deltaR","std::vector<float>",&T_Gen_Elec_deltaR);
   mytree_->Branch("T_Gen_Elec_MCpart", "std::vector<int>", &T_Gen_Elec_MCpart);
   mytree_->Branch("T_Gen_Elec_PDGid", "std::vector<int>", &T_Gen_Elec_PDGid);
   mytree_->Branch("T_Gen_Elec_status", "std::vector<int>", &T_Gen_Elec_status);
   mytree_->Branch("T_Gen_Elec_MotherID", "std::vector<int>", &T_Gen_Elec_MotherID);
   
    
    //Vertex
   mytree_->Branch("T_Vertex_z", "std::vector<float>", &T_Vertex_z);
   mytree_->Branch("T_Vertex_y", "std::vector<float>", &T_Vertex_y);
   mytree_->Branch("T_Vertex_x", "std::vector<float>", &T_Vertex_x);
   mytree_->Branch("T_Vertex_Chi2Prob", "std::vector<float>", &T_Vertex_Chi2Prob);
   mytree_->Branch("T_Vertex_rho","std::vector<float>", &T_Vertex_rho);
   mytree_->Branch("T_Vertex_ndof","std::vector<float>", &T_Vertex_ndof);
   mytree_->Branch("T_Vertex_isFake","std::vector<bool>", &T_Vertex_isFake);
   mytree_->Branch("T_Vertex_tracksSize","std::vector<int>", &T_Vertex_tracksSize);
    
    //Electrons  
   mytree_->Branch("T_Elec_Eta", "std::vector<float>", &T_Elec_Eta);
   mytree_->Branch("T_Elec_IPwrtAveBS", "std::vector<float>", &T_Elec_IPwrtAveBS);
   mytree_->Branch("T_Elec_IPwrtPV", "std::vector<float>", &T_Elec_IPwrtPV);
   mytree_->Branch("T_Elec_Px", "std::vector<float>", &T_Elec_Px);
   mytree_->Branch("T_Elec_Py", "std::vector<float>", &T_Elec_Py);
   mytree_->Branch("T_Elec_Pz", "std::vector<float>", &T_Elec_Pz);
   mytree_->Branch("T_Elec_Pt", "std::vector<float>", &T_Elec_Pt);
   mytree_->Branch("T_Elec_Energy", "std::vector<float>", &T_Elec_Energy);
   mytree_->Branch("T_Elec_Charge", "std::vector<int>", &T_Elec_Charge);    
   mytree_->Branch("T_Elec_nBrems", "std::vector<int>", &T_Elec_nBrems); 
   mytree_->Branch("T_Elec_fBrem", "std::vector<float>", &T_Elec_fBrem);
   mytree_->Branch("T_Elec_eSuperClusterOverP", "std::vector<float>", &T_Elec_eSuperClusterOverP);
   mytree_->Branch("T_Elec_isEB", "std::vector<bool>", &T_Elec_isEB);
   mytree_->Branch("T_Elec_isEE", "std::vector<bool>", &T_Elec_isEE);
   mytree_->Branch("T_Elec_MVA", "std::vector<float>", &T_Elec_MVA);
   mytree_->Branch("T_Elec_simpleEleId95", "std::vector<float>", &T_Elec_simpleEleId95);
   mytree_->Branch("T_Elec_simpleEleId90", "std::vector<float>", &T_Elec_simpleEleId90);
   mytree_->Branch("T_Elec_simpleEleId85", "std::vector<float>", &T_Elec_simpleEleId85);
   mytree_->Branch("T_Elec_simpleEleId80", "std::vector<float>", &T_Elec_simpleEleId80);
   mytree_->Branch("T_Elec_simpleEleId70", "std::vector<float>", &T_Elec_simpleEleId70);
   mytree_->Branch("T_Elec_simpleEleId60", "std::vector<float>", &T_Elec_simpleEleId60);
   mytree_->Branch("T_Elec_chargedHadronIso", "std::vector<float>", &T_Elec_chargedHadronIso);
   mytree_->Branch("T_Elec_neutralHadronIso", "std::vector<float>", &T_Elec_neutralHadronIso);
   mytree_->Branch("T_Elec_photonIso", "std::vector<float>", &T_Elec_photonIso);
   mytree_->Branch("T_Elec_chargedHadronIso04", "std::vector<float>", &T_Elec_chargedHadronIso04);
   mytree_->Branch("T_Elec_neutralHadronIso04", "std::vector<float>", &T_Elec_neutralHadronIso04);
   mytree_->Branch("T_Elec_photonIso04", "std::vector<float>", &T_Elec_photonIso04);
   mytree_->Branch("T_Elec_puChargedHadronIso04", "std::vector<float>", &T_Elec_puChargedHadronIso04);
   mytree_->Branch("T_Elec_allChargedHadronIso04", "std::vector<float>", &T_Elec_allChargedHadronIso04);
   mytree_->Branch("T_Elec_puChargedHadronIso", "std::vector<float>", &T_Elec_puChargedHadronIso);
   mytree_->Branch("T_Elec_allChargedHadronIso", "std::vector<float>", &T_Elec_allChargedHadronIso);

   mytree_->Branch("T_Elec_passConversionVeto","std::vector<bool>",&T_Elec_passConversionVeto);
    
   mytree_->Branch("T_Elec_sigmaIetaIeta", "std::vector<float>", &T_Elec_sigmaIetaIeta);
   mytree_->Branch("T_Elec_deltaPhiIn", "std::vector<float>", &T_Elec_deltaPhiIn);
   mytree_->Branch("T_Elec_deltaEtaIn", "std::vector<float>", &T_Elec_deltaEtaIn);
   mytree_->Branch("T_Elec_isEcalDriven", "std::vector<bool>", &T_Elec_isEcalDriven);
   mytree_->Branch("T_Elec_HtoE", "std::vector<float>", &T_Elec_HtoE);
   mytree_->Branch("T_Elec_vz", "std::vector<float>", &T_Elec_vz);
   mytree_->Branch("T_Elec_vy", "std::vector<float>", &T_Elec_vy);
   mytree_->Branch("T_Elec_vx", "std::vector<float>", &T_Elec_vx);
   mytree_->Branch("T_Elec_nLost", "std::vector<int>", &T_Elec_nLost);
   mytree_->Branch("T_Elec_nHits", "std::vector<int>", &T_Elec_nHits);
   mytree_->Branch("T_Elec_SC_Et","std::vector<float>", &T_Elec_SC_Et);
   mytree_->Branch("T_Elec_SC_Eta","std::vector<float>", &T_Elec_SC_Eta); 

    mytree_->Branch("T_Elec_passVeto","std::vector<bool>", &T_Elec_passVeto); 
    mytree_->Branch("T_Elec_passLoose","std::vector<bool>", &T_Elec_passLoose); 
    mytree_->Branch("T_Elec_passMedium","std::vector<bool>", &T_Elec_passMedium); 
    mytree_->Branch("T_Elec_passTight","std::vector<bool>", &T_Elec_passTight); 
    mytree_->Branch("T_Elec_passFbremopin","std::vector<bool>", &T_Elec_passFbremopin); 
    mytree_->Branch("T_Elec_passtrigTight","std::vector<bool>", &T_Elec_passtrigTight); 
    mytree_->Branch("T_Elec_passtrigwp70","std::vector<bool>", &T_Elec_passtrigwp70);
    mytree_->Branch("T_Elec_isTrig","std::vector<bool>", &T_Elec_isTrig);
    
    mytree_->Branch("T_Elec_MVAid_trig","std::vector<double>", &T_Elec_MVAid_trig); 
    mytree_->Branch("T_Elec_MVAid_Nontrig","std::vector<double>", &T_Elec_MVAid_Nontrig); 
    mytree_->Branch("T_Elec_Mvaiso","std::vector<double>", &T_Elec_Mvaiso); 
    mytree_->Branch("T_Elec_RadialIso","std::vector<double>", &T_Elec_RadialIso); 
    mytree_->Branch("T_Elec_RadialIsoVeto","std::vector<double>", &T_Elec_RadialIsoVeto); 
    mytree_->Branch("T_Elec_RadialIsoVetoMore","std::vector<double>", &T_Elec_RadialIsoVetoMore); 
    
    mytree_->Branch("T_Elec_ChargedIso_DR0p1To0p1_DR0p0To0p1","std::vector<double>", &T_Elec_ChargedIso_DR0p0To0p1); 
    mytree_->Branch("T_Elec_ChargedIso_DR0p1To0p2","std::vector<double>", &T_Elec_ChargedIso_DR0p1To0p2); 
    mytree_->Branch("T_Elec_ChargedIso_DR0p2To0p3","std::vector<double>", &T_Elec_ChargedIso_DR0p2To0p3); 
    mytree_->Branch("T_Elec_ChargedIso_DR0p3To0p4","std::vector<double>", &T_Elec_ChargedIso_DR0p3To0p4); 
    mytree_->Branch("T_Elec_ChargedIso_DR0p4To0p5","std::vector<double>", &T_Elec_ChargedIso_DR0p4To0p5); 
    
    mytree_->Branch("T_Elec_GammaIso_DR0p0To0p1","std::vector<double>", &T_Elec_GammaIso_DR0p0To0p1); 
    mytree_->Branch("T_Elec_GammaIso_DR0p1To0p2","std::vector<double>", &T_Elec_GammaIso_DR0p1To0p2); 
    mytree_->Branch("T_Elec_GammaIso_DR0p2To0p3","std::vector<double>", &T_Elec_GammaIso_DR0p2To0p3); 
    mytree_->Branch("T_Elec_GammaIso_DR0p3To0p4","std::vector<double>", &T_Elec_GammaIso_DR0p3To0p4); 
    mytree_->Branch("T_Elec_GammaIso_DR0p4To0p5","std::vector<double>", &T_Elec_GammaIso_DR0p4To0p5); 
    
    mytree_->Branch("T_Elec_NeutralHadronIso_DR0p0To0p1","std::vector<double>", &T_Elec_NeutralHadronIso_DR0p0To0p1); 
    mytree_->Branch("T_Elec_NeutralHadronIso_DR0p1To0p2","std::vector<double>", &T_Elec_NeutralHadronIso_DR0p1To0p2); 
    mytree_->Branch("T_Elec_NeutralHadronIso_DR0p2To0p3","std::vector<double>", &T_Elec_NeutralHadronIso_DR0p2To0p3); 
    mytree_->Branch("T_Elec_NeutralHadronIso_DR0p3To0p4","std::vector<double>", &T_Elec_NeutralHadronIso_DR0p3To0p4); 
    mytree_->Branch("T_Elec_NeutralHadronIso_DR0p4To0p5","std::vector<double>", &T_Elec_NeutralHadronIso_DR0p4To0p5); 

    mytree_->Branch("T_Elec_HLT_Elec27_WP80","std::vector<int>", &T_Elec_HLT_Elec27_WP80);
    mytree_->Branch("T_Elec_HLT_Ele17TightID_Ele8_Ele8Leg","std::vector<int>", &T_Elec_HLT_Ele17TightID_Ele8_Ele8Leg);
    mytree_->Branch("T_Elec_HLT_Ele17TightID_Ele8_Ele17Leg","std::vector<int>", &T_Elec_HLT_Ele17TightID_Ele8_Ele17Leg);
    mytree_->Branch("T_Elec_HLT_Ele17_Ele8_Ele8Leg","std::vector<int>", &T_Elec_HLT_Ele17_Ele8_Ele8Leg);
    mytree_->Branch("T_Elec_HLT_Ele17_Ele8_Ele17Leg","std::vector<int>", &T_Elec_HLT_Ele17_Ele8_Ele17Leg);
    mytree_->Branch("T_Elec_HLT_Ele17_Ele8_TnP_Ele8Leg","std::vector<int>", &T_Elec_HLT_Ele17_Ele8_TnP_Ele8Leg);
    mytree_->Branch("T_Elec_HLT_Ele17_Ele8_TnP_Ele17Leg","std::vector<int>", &T_Elec_HLT_Ele17_Ele8_TnP_Ele17Leg); 
    mytree_->Branch("T_Elec_HLT_Ele20_SC4_TnP_SC4Leg","std::vector<int>", &T_Elec_HLT_Ele20_SC4_TnP_SC4Leg); 
    mytree_->Branch("T_Elec_HLT_Ele20_SC4_TnP_Ele20Leg","std::vector<int>", &T_Elec_HLT_Ele20_SC4_TnP_Ele20Leg);
    mytree_->Branch("T_Elec_HLT_Mu8_Ele17_Ele17Leg","std::vector<int>", &T_Elec_HLT_Mu8_Ele17_Ele17Leg);
    mytree_->Branch("T_Elec_HLT_Ele8_Mu17_Ele8Leg","std::vector<int>", &T_Elec_HLT_Ele8_Mu17_Ele8Leg);

    mytree_->Branch("T_Elec_passMVA","std::vector<bool>", &T_Elec_passMVA); 
    mytree_->Branch("T_Elec_kfchi2","std::vector<float>", &T_Elec_kfchi2); 
    mytree_->Branch("T_Elec_kfhits","std::vector<float>", &T_Elec_kfhits); 
    mytree_->Branch("T_Elec_gsfchi2","std::vector<float>", &T_Elec_gsfchi2); 
    mytree_->Branch("T_Elec_detacalo","std::vector<float>", &T_Elec_detacalo); 
    mytree_->Branch("T_Elec_see","std::vector<float>", &T_Elec_see); 
    mytree_->Branch("T_Elec_spp","std::vector<float>", &T_Elec_spp); 
    mytree_->Branch("T_Elec_etawidth","std::vector<float>", &T_Elec_etawidth); 
    mytree_->Branch("T_Elec_phiwidth","std::vector<float>", &T_Elec_phiwidth); 
    mytree_->Branch("T_Elec_e1x5e5x5","std::vector<float>", &T_Elec_e1x5e5x5); 
    mytree_->Branch("T_Elec_R9","std::vector<float>", &T_Elec_R9); 
    mytree_->Branch("T_Elec_EoP","std::vector<float>", &T_Elec_EoP);
    mytree_->Branch("T_Elec_ESeedoP","std::vector<float>", &T_Elec_ESeedoP);
    mytree_->Branch("T_Elec_ESeedoPout","std::vector<float>", &T_Elec_ESeedoPout);
    mytree_->Branch("T_Elec_IoEmIoP","std::vector<float>", &T_Elec_IoEmIoP);
    mytree_->Branch("T_Elec_eleEoPout","std::vector<float>", &T_Elec_eleEoPout);
    mytree_->Branch("T_Elec_EcalEnergy","std::vector<float>", &T_Elec_EcalEnergy);
    mytree_->Branch("T_Elec_TrackPatVtx","std::vector<float>", &T_Elec_TrackPatVtx);
    mytree_->Branch("T_Elec_PreShowerOverRaw","std::vector<float>", &T_Elec_PreShowerOverRaw);
    mytree_->Branch("T_Elec_d0","std::vector<float>", &T_Elec_d0); 
    mytree_->Branch("T_Elec_IP3D","std::vector<float>", &T_Elec_IP3D); 
    mytree_->Branch("T_Elec_dZ","std::vector<float>", &T_Elec_dZ);
    mytree_->Branch("T_Elec_isFO","std::vector<bool>", &T_Elec_isFO); 
    mytree_->Branch("T_Elec_CombIsoHWW","std::vector<float>", &T_Elec_CombIsoHWW);
    mytree_->Branch("T_Elec_dr03TkSumPt","std::vector<float>", &T_Elec_dr03TkSumPt);
    mytree_->Branch("T_Elec_dr03EcalSumEt","std::vector<float>", &T_Elec_dr03EcalSumEt);
    mytree_->Branch("T_Elec_dr03HcalSumEt","std::vector<float>", &T_Elec_dr03HcalSumEt);
    
    if (doPFPATmatching_){
        mytree_->Branch("T_Elec_isPF","std::vector<int>", &T_Elec_isPF);
        mytree_->Branch("T_Elec_PFenergy","std::vector<float>", &T_Elec_PFenergy);
        mytree_->Branch("T_Elec_PFeta","std::vector<float>", &T_Elec_PFeta);
        mytree_->Branch("T_Elec_PFphi","std::vector<float>", &T_Elec_PFphi);
        mytree_->Branch("T_Elec_PFpt","std::vector<float>", &T_Elec_PFpt);
        mytree_->Branch("T_Elec_PFpx","std::vector<float>", &T_Elec_PFpx);
        mytree_->Branch("T_Elec_PFpy","std::vector<float>", &T_Elec_PFpy);
        mytree_->Branch("T_Elec_PFpz","std::vector<float>", &T_Elec_PFpz);
        mytree_->Branch("T_Elec_PFmva","std::vector<float>", &T_Elec_PFmva);
    }
    
    
    mytree_->Branch("T_METPF_ET", &T_METPF_ET, "T_METPF_ET/F");
    mytree_->Branch("T_METPF_Phi", &T_METPF_Phi, "T_METPF_Phi/F");	
    mytree_->Branch("T_METPF_Sig", &T_METPF_Sig, "T_METPF_Sig/F");
    
    mytree_->Branch("T_METPFTypeI_ET", &T_METPFTypeI_ET, "T_METPFTypeI_ET/F");
    mytree_->Branch("T_METPFTypeI_Phi", &T_METPFTypeI_Phi, "T_METPFTypeI_Phi/F");
	
	//Muons
	if (doMuons_){
        mytree_->Branch("T_Muon_Eta", "std::vector<float>", &T_Muon_Eta);
        mytree_->Branch("T_Muon_Phi", "std::vector<float>", &T_Muon_Phi);
        mytree_->Branch("T_Muon_Energy", "std::vector<float>", &T_Muon_Energy);
        mytree_->Branch("T_Muon_Et", "std::vector<float>", &T_Muon_Et);
        mytree_->Branch("T_Muon_Pt", "std::vector<float>", &T_Muon_Pt);
        mytree_->Branch("T_Muon_Px", "std::vector<float>", &T_Muon_Px);
        mytree_->Branch("T_Muon_Py", "std::vector<float>", &T_Muon_Py);
        mytree_->Branch("T_Muon_Pz", "std::vector<float>", &T_Muon_Pz);
        mytree_->Branch("T_Muon_Mass", "std::vector<float>", &T_Muon_Mass);
        mytree_->Branch("T_Muon_IsGlobalMuon", "std::vector<bool>", &T_Muon_IsGlobalMuon);
        mytree_->Branch("T_Muon_IsTrackerMuon", "std::vector<bool>", &T_Muon_IsTrackerMuon);
        mytree_->Branch("T_Muon_IsPFMuon", "std::vector<bool>", &T_Muon_IsPFMuon);
        mytree_->Branch("T_Muon_IsCaloMuon", "std::vector<bool>", &T_Muon_IsCaloMuon);
        mytree_->Branch("T_Muon_IsStandAloneMuon", "std::vector<bool>", &T_Muon_IsStandAloneMuon);
        mytree_->Branch("T_Muon_IsMuon", "std::vector<bool>", &T_Muon_IsMuon);
        mytree_->Branch("T_Muon_IsGlobalMuon_PromptTight", "std::vector<bool>", &T_Muon_IsGlobalMuon_PromptTight);
        mytree_->Branch("T_Muon_IsTrackerMuonArbitrated", "std::vector<bool>", &T_Muon_IsTrackerMuonArbitrated);
        mytree_->Branch("T_Muon_numberOfChambers", "std::vector<int>", &T_Muon_numberOfChambers);
        mytree_->Branch("T_Muon_numberOfChambersRPC", "std::vector<int>", &T_Muon_numberOfChambersRPC);
        mytree_->Branch("T_Muon_numberOfMatches", "std::vector<int>", &T_Muon_numberOfMatches);
        mytree_->Branch("T_Muon_numberOfMatchedStations", "std::vector<int>", &T_Muon_numberOfMatchedStations);
        mytree_->Branch("T_Muon_charge", "std::vector<int>", &T_Muon_charge);
        mytree_->Branch("T_Muon_TMLastStationTight", "std::vector<bool>", &T_Muon_TMLastStationTight);
        mytree_->Branch("T_Muon_globalTrackChi2", "std::vector<float>", &T_Muon_globalTrackChi2);
        mytree_->Branch("T_Muon_validMuonHits", "std::vector<int>", &T_Muon_validMuonHits);
        mytree_->Branch("T_Muon_trkKink", "std::vector<float>", &T_Muon_trkKink);
        mytree_->Branch("T_Muon_trkNbOfTrackerLayers", "std::vector<int>", &T_Muon_trkNbOfTrackerLayers);
        mytree_->Branch("T_Muon_trkNbOfValidTrackeHits", "std::vector<int>", &T_Muon_trkNbOfValidTrackeHits);
        mytree_->Branch("T_Muon_trkValidPixelHits", "std::vector<int>", &T_Muon_trkValidPixelHits);
        mytree_->Branch("T_Muon_trkError", "std::vector<float>", &T_Muon_trkError);
        mytree_->Branch("T_Muon_dB", "std::vector<float>", &T_Muon_dB);
        mytree_->Branch("T_Muon_dzPV", "std::vector<float>", &T_Muon_dzPV);
        mytree_->Branch("T_Muon_dBstop", "std::vector<float>", &T_Muon_dBstop);
        mytree_->Branch("T_Muon_dzstop", "std::vector<float>", &T_Muon_dzstop);        
        mytree_->Branch("T_Muon_isoR03_emEt", "std::vector<float>", &T_Muon_isoR03_emEt);
        mytree_->Branch("T_Muon_isoR03_hadEt", "std::vector<float>", &T_Muon_isoR03_hadEt);
        mytree_->Branch("T_Muon_isoR03_hoEt", "std::vector<float>", &T_Muon_isoR03_hoEt);
        mytree_->Branch("T_Muon_isoR03_sumPt", "std::vector<float>", &T_Muon_isoR03_sumPt);
        mytree_->Branch("T_Muon_isoR03_nTracks", "std::vector<int>", &T_Muon_isoR03_nTracks);
        mytree_->Branch("T_Muon_isoR03_nJets", "std::vector<int>", &T_Muon_isoR03_nJets);
        mytree_->Branch("T_Muon_chargedHadronIsoR04", "std::vector<float>", &T_Muon_chargedHadronIsoR04);
        mytree_->Branch("T_Muon_neutralHadronIsoR04", "std::vector<float>", &T_Muon_neutralHadronIsoR04);
        mytree_->Branch("T_Muon_photonIsoR04", "std::vector<float>", &T_Muon_photonIsoR04);
        mytree_->Branch("T_Muon_chargedHadronIsoPUR04", "std::vector<float>", &T_Muon_chargedHadronIsoPUR04);
        mytree_->Branch("T_Muon_chargedHadronIsoR03", "std::vector<float>", &T_Muon_chargedHadronIsoR03);
        mytree_->Branch("T_Muon_neutralHadronIsoR03", "std::vector<float>", &T_Muon_neutralHadronIsoR03);
        mytree_->Branch("T_Muon_photonIsoR03", "std::vector<float>", &T_Muon_photonIsoR03);
        mytree_->Branch("T_Muon_chargedHadronIsoPUR03", "std::vector<float>", &T_Muon_chargedHadronIsoPUR03);
        mytree_->Branch("T_Muon_isoRingsMVA", "std::vector<float>", &T_Muon_isoRingsMVA);
        mytree_->Branch("T_Muon_HLT_Mu17_TkMu8_Mu17Leg", "std::vector<int>", &T_Muon_HLT_Mu17_TkMu8_Mu17Leg);
        mytree_->Branch("T_Muon_HLT_Mu17_TkMu8_Mu8Leg", "std::vector<int>", &T_Muon_HLT_Mu17_TkMu8_Mu8Leg);
        mytree_->Branch("T_Muon_HLT_Mu17_Mu8_Mu17Leg", "std::vector<int>", &T_Muon_HLT_Mu17_Mu8_Mu17Leg);
        mytree_->Branch("T_Muon_HLT_Mu17_Mu8_Mu8Leg", "std::vector<int>", &T_Muon_HLT_Mu17_Mu8_Mu8Leg);
        mytree_->Branch("T_Muon_HLT_Mu17_obj", "std::vector<int>", &T_Muon_HLT_Mu17_obj);
        mytree_->Branch("T_Muon_HLT_Mu8_obj", "std::vector<int>", &T_Muon_HLT_Mu8_obj);
        mytree_->Branch("T_Muon_HLT_Mu8_Ele17_Mu8Leg", "std::vector<int>", &T_Muon_HLT_Mu8_Ele17_Mu8Leg);
        mytree_->Branch("T_Muon_HLT_Ele8_Mu17_Mu17Leg", "std::vector<int>", &T_Muon_HLT_Ele8_Mu17_Mu17Leg);
        mytree_->Branch("T_Muon_HLT_IsoMu24", "std::vector<int>", &T_Muon_HLT_IsoMu24);
        mytree_->Branch("T_Muon_HLT_IsoMu24_2p1", "std::vector<int>", &T_Muon_HLT_IsoMu24_2p1);
  
        mytree_->Branch("T_Gen_Muon_Px", "std::vector<float>", &T_Gen_Muon_Px);
        mytree_->Branch("T_Gen_Muon_Py", "std::vector<float>", &T_Gen_Muon_Py);
        mytree_->Branch("T_Gen_Muon_Pz", "std::vector<float>", &T_Gen_Muon_Pz);
        mytree_->Branch("T_Gen_Muon_Energy", "std::vector<float>", &T_Gen_Muon_Energy);
        mytree_->Branch("T_Gen_Muon_deltaR", "std::vector<float>", &T_Gen_Muon_deltaR);
        mytree_->Branch("T_Gen_Muon_MCpart", "std::vector<int>", &T_Gen_Muon_MCpart);
        mytree_->Branch("T_Gen_Muon_PDGid", "std::vector<int>", &T_Gen_Muon_PDGid);
        mytree_->Branch("T_Gen_Muon_status", "std::vector<int>", &T_Gen_Muon_status);
        mytree_->Branch("T_Gen_Muon_MotherID", "std::vector<int>", &T_Gen_Muon_MotherID);

        
        
        
    }
    
    if (savePF_){
        mytree_->Branch("T_PF_Et", "std::vector<float>", &T_PF_Et);
        mytree_->Branch("T_PF_Pt", "std::vector<float>", &T_PF_Pt);
        mytree_->Branch("T_PF_Px", "std::vector<float>", &T_PF_Px);
        mytree_->Branch("T_PF_Py", "std::vector<float>", &T_PF_Py);
        mytree_->Branch("T_PF_Pz", "std::vector<float>", &T_PF_Pz);
        mytree_->Branch("T_PF_Eta", "std::vector<float>", &T_PF_Eta);
        mytree_->Branch("T_PF_Phi", "std::vector<float>", &T_PF_Phi);
        mytree_->Branch("T_PF_pdgID", "std::vector<int>", &T_PF_pdgID);
        mytree_->Branch("T_PF_particleID", "std::vector<int>", &T_PF_particleID);
        mytree_->Branch("T_PF_hasTrack", "std::vector<int>", &T_PF_hasTrack);
        mytree_->Branch("T_PF_hasTrackWithMissingHits", "std::vector<int>", &T_PF_hasTrackWithMissingHits);
        mytree_->Branch("T_PF_isPU", "std::vector<int>", &T_PF_isPU);
    }
    
    if (doPhotons_){
        mytree_->Branch("T_Pho_Et", "std::vector<float>", &T_Pho_Et);
        mytree_->Branch("T_Pho_Energy", "std::vector<float>", &T_Pho_Energy);
        mytree_->Branch("T_Pho_Pt", "std::vector<float>", &T_Pho_Pt);
        mytree_->Branch("T_Pho_Px", "std::vector<float>", &T_Pho_Px);
        mytree_->Branch("T_Pho_Py", "std::vector<float>", &T_Pho_Py);
        mytree_->Branch("T_Pho_Pz", "std::vector<float>", &T_Pho_Pz);
        mytree_->Branch("T_Pho_Eta", "std::vector<float>", &T_Pho_Eta);
        mytree_->Branch("T_Pho_Phi", "std::vector<float>", &T_Pho_Phi);
        mytree_->Branch("T_Pho_r9", "std::vector<float>", &T_Pho_r9);
        mytree_->Branch("T_Pho_EtaWidth", "std::vector<float>", &T_Pho_EtaWidth);
        mytree_->Branch("T_Pho_PhiWidth", "std::vector<float>", &T_Pho_PhiWidth);
        mytree_->Branch("T_Pho_sigmaIetaIeta", "std::vector<float>", &T_Pho_sigmaIetaIeta);
        mytree_->Branch("T_Pho_indOfTheElec", "std::vector<int>", &T_Pho_indOfTheElec);
        mytree_->Branch("T_Pho_SCEt", "std::vector<float>", &T_Pho_SCEt);
        mytree_->Branch("T_Pho_SCEnergy", "std::vector<float>", &T_Pho_SCEnergy);
        mytree_->Branch("T_Pho_SCEta", "std::vector<float>", &T_Pho_SCEta);
        mytree_->Branch("T_Pho_SCPhi", "std::vector<float>", &T_Pho_SCPhi);
        mytree_->Branch("T_Pho_isMatchedWithMC", "std::vector<int>", &T_Pho_isMatchedWithMC);
        mytree_->Branch("T_Pho_Gen_PDGid", "std::vector<int>", &T_Pho_Gen_PDGid);
        mytree_->Branch("T_Pho_Gen_Status", "std::vector<int>", &T_Pho_Gen_Status);
        mytree_->Branch("T_Pho_Gen_MotherID", "std::vector<int>", &T_Pho_Gen_MotherID);
    }
    if (saveConversions_){
        mytree_->Branch("T_Conv_EleInd", "std::vector<int>", &T_Conv_EleInd);
        mytree_->Branch("T_Conv_PhoInd", "std::vector<int>", &T_Conv_PhoInd);
        mytree_->Branch("T_Conv_vtxProb", "std::vector<float>", &T_Conv_vtxProb);
        mytree_->Branch("T_Conv_lxy", "std::vector<float>", &T_Conv_lxy);
        mytree_->Branch("T_Conv_nHitsMax", "std::vector<int>", &T_Conv_nHitsMax);
    }
    mytree_->Branch("T_Jet_Px" , "std::vector<float>", &T_Jet_Px);
    mytree_->Branch("T_Jet_Py", "std::vector<float>", &T_Jet_Py);
    mytree_->Branch("T_Jet_Pz", "std::vector<float>", &T_Jet_Pz);
    mytree_->Branch("T_Jet_Et", "std::vector<float>", &T_Jet_Et);
    mytree_->Branch("T_Jet_Eta", "std::vector<float>", &T_Jet_Eta);
    mytree_->Branch("T_Jet_Energy", "std::vector<float>", &T_Jet_Energy);
    mytree_->Branch("T_Jet_Corr", "std::vector<float>", &T_Jet_Corr);
    mytree_->Branch("T_Jet_Phi", "std::vector<float>", &T_Jet_Phi); 


    
}

// ------------ method called once each job just after ending the event loop  ------------
void 
ElecIdAnalyzer::endJob() 
{
    rootFile_->Write();
    rootFile_->Close();

}

// ------------ method called when starting to processes a run  ------------
void 
ElecIdAnalyzer::beginRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
ElecIdAnalyzer::endRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
ElecIdAnalyzer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
ElecIdAnalyzer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

void 
ElecIdAnalyzer::beginEvent()
{
    T_Gen_Elec_Px = new std::vector<float>;
    T_Gen_Elec_Py = new std::vector<float>;
    T_Gen_Elec_Pz = new std::vector<float>;
    T_Gen_Elec_Energy = new std::vector<float>;
    T_Gen_Elec_deltaR = new std::vector<float>;
    T_Gen_Elec_MCpart = new std::vector<int>;    
    T_Gen_Elec_PDGid = new std::vector<int>;    
    T_Gen_Elec_status = new std::vector<int>;
    T_Gen_Elec_MotherID = new std::vector<int>;
    
    T_Vertex_z = new std::vector<float>;   
    T_Vertex_y = new std::vector<float>;
    T_Vertex_x = new std::vector<float>; 
    T_Vertex_Chi2Prob= new std::vector<float>;
    T_Vertex_rho = new std::vector<float>;
    T_Vertex_ndof = new std::vector<float>;
    T_Vertex_isFake = new std::vector<bool>;
    T_Vertex_tracksSize = new std::vector<int>; 
    
    T_Elec_Eta  = new std::vector<float>;
    T_Elec_IPwrtAveBS = new std::vector<float>;
    T_Elec_IPwrtPV = new std::vector<float>;
    T_Elec_Px = new std::vector<float>;
    T_Elec_Py = new std::vector<float>;
    T_Elec_Pz = new std::vector<float>;
    T_Elec_Pt = new std::vector<float>;
    T_Elec_Energy = new std::vector<float>;
    T_Elec_Charge = new std::vector<int>;
    
    T_Elec_vz = new std::vector<float>;
    T_Elec_vy = new std::vector<float>;  
    T_Elec_vx = new std::vector<float>;
    T_Elec_nLost =  new std::vector<int>; 
    T_Elec_nHits  =  new std::vector<int>;
    T_Elec_SC_Et = new std::vector<float>;
    T_Elec_SC_Eta = new std::vector<float>; 
    T_Elec_nBrems  = new std::vector<int>;
    T_Elec_fBrem = new std::vector<float>;
    T_Elec_eSuperClusterOverP = new std::vector<float>;
    
    T_Elec_passConversionVeto=new std::vector<bool>;
    T_Elec_isEB = new std::vector<bool>;
    T_Elec_isEE = new std::vector<bool>;
    T_Elec_MVA = new std::vector<float>;
    T_Elec_simpleEleId95 = new std::vector<float>;
    T_Elec_simpleEleId90 = new std::vector<float>;
    T_Elec_simpleEleId85 = new std::vector<float>;
    T_Elec_simpleEleId80 = new std::vector<float>;
    T_Elec_simpleEleId70 = new std::vector<float>;
    T_Elec_simpleEleId60 = new std::vector<float>;
    
    T_Elec_sigmaIetaIeta = new std::vector<float>;
    T_Elec_deltaPhiIn = new std::vector<float>;
    T_Elec_deltaEtaIn =new  std::vector<float>;
    T_Elec_isEcalDriven = new std::vector<bool>;
    T_Elec_HtoE = new std::vector<float>;
    T_Elec_chargedHadronIso= new std::vector<float>;
    T_Elec_neutralHadronIso= new std::vector<float>;
    T_Elec_puChargedHadronIso = new std::vector<float>;
    T_Elec_allChargedHadronIso = new std::vector<float>;
    T_Elec_allChargedHadronIso = new std::vector<float>;
    T_Elec_photonIso= new std::vector<float>;
    T_Elec_chargedHadronIso04= new std::vector<float>;
    T_Elec_neutralHadronIso04= new std::vector<float>;
    T_Elec_photonIso04= new std::vector<float>;
    T_Elec_puChargedHadronIso04 = new std::vector<float>;
    T_Elec_allChargedHadronIso04 = new std::vector<float>;
    T_Elec_allChargedHadronIso04 = new std::vector<float>;
    T_Elec_photonIso= new std::vector<float>;

    T_Elec_passVeto= new std::vector<bool>;
    T_Elec_passLoose= new std::vector<bool>;
    T_Elec_passMedium= new std::vector<bool>;
    T_Elec_passTight= new std::vector<bool>;
    T_Elec_passFbremopin= new std::vector<bool>;
    T_Elec_passtrigTight= new std::vector<bool>;
    T_Elec_passtrigwp70= new std::vector<bool>;
    
    T_Elec_isTrig = new std::vector<bool>;
    
    T_Elec_MVAid_trig = new std::vector<double>;
    T_Elec_MVAid_Nontrig= new std::vector<double>;
    T_Elec_Mvaiso= new std::vector<double>;
    T_Elec_RadialIso = new std::vector<double>;
    T_Elec_RadialIsoVeto = new std::vector<double>;
    T_Elec_RadialIsoVetoMore = new std::vector<double>;
    
    T_Elec_ChargedIso_DR0p0To0p1 = new std::vector<double>;
    T_Elec_ChargedIso_DR0p1To0p2 = new std::vector<double>;
    T_Elec_ChargedIso_DR0p2To0p3 = new std::vector<double>;
    T_Elec_ChargedIso_DR0p3To0p4 = new std::vector<double>;
    T_Elec_ChargedIso_DR0p4To0p5 = new std::vector<double>;
    T_Elec_GammaIso_DR0p0To0p1 = new std::vector<double>;
    T_Elec_GammaIso_DR0p1To0p2 = new std::vector<double>;
    T_Elec_GammaIso_DR0p2To0p3 = new std::vector<double>;
    T_Elec_GammaIso_DR0p3To0p4 = new std::vector<double>;
    T_Elec_GammaIso_DR0p4To0p5 = new std::vector<double>;
    T_Elec_NeutralHadronIso_DR0p0To0p1 = new std::vector<double>;
    T_Elec_NeutralHadronIso_DR0p1To0p2 = new std::vector<double>;
    T_Elec_NeutralHadronIso_DR0p2To0p3 = new std::vector<double>;
    T_Elec_NeutralHadronIso_DR0p3To0p4 = new std::vector<double>;
    T_Elec_NeutralHadronIso_DR0p4To0p5 = new std::vector<double>;

    T_Elec_HLT_Elec27_WP80 = new std::vector<int>;
    T_Elec_HLT_Ele17TightID_Ele8_Ele8Leg = new std::vector<int>;
    T_Elec_HLT_Ele17TightID_Ele8_Ele17Leg = new std::vector<int>;
    T_Elec_HLT_Ele17_Ele8_Ele8Leg = new std::vector<int>;
    T_Elec_HLT_Ele17_Ele8_Ele17Leg = new std::vector<int>;
    T_Elec_HLT_Ele17_Ele8_TnP_Ele8Leg = new std::vector<int>;
    T_Elec_HLT_Ele17_Ele8_TnP_Ele17Leg = new std::vector<int>;
    T_Elec_HLT_Ele20_SC4_TnP_SC4Leg = new std::vector<int>;
    T_Elec_HLT_Ele20_SC4_TnP_Ele20Leg = new std::vector<int>;
    T_Elec_HLT_Mu8_Ele17_Ele17Leg = new std::vector<int>;
    T_Elec_HLT_Ele8_Mu17_Ele8Leg = new std::vector<int>;
    
    T_Elec_passMVA = new std::vector<bool>;
    T_Elec_kfchi2 = new std::vector<float>;
    T_Elec_kfhits = new std::vector<float>;
    T_Elec_gsfchi2 = new std::vector<float>;
    T_Elec_detacalo = new std::vector<float>;
    T_Elec_see = new std::vector<float>;
    T_Elec_spp = new std::vector<float>;
    T_Elec_etawidth = new std::vector<float>;
    T_Elec_phiwidth = new std::vector<float>;
    T_Elec_e1x5e5x5 = new std::vector<float>;
    T_Elec_R9 = new std::vector<float>;
    T_Elec_EoP = new std::vector<float>;
    T_Elec_ESeedoP = new std::vector<float>;
    T_Elec_ESeedoPout = new std::vector<float>;
    T_Elec_IoEmIoP = new std::vector<float>;
    T_Elec_eleEoPout = new std::vector<float>;
    T_Elec_EcalEnergy = new std::vector<float>;
    T_Elec_TrackPatVtx = new std::vector<float>;
    T_Elec_PreShowerOverRaw = new std::vector<float>;
    T_Elec_d0 = new std::vector<float>;
    T_Elec_IP3D = new std::vector<float>;
    T_Elec_dZ = new std::vector<float>;
    T_Elec_isFO = new std::vector<bool>;
    T_Elec_CombIsoHWW = new std::vector<float>;
    T_Elec_dr03TkSumPt = new std::vector<float>;
    T_Elec_dr03EcalSumEt = new std::vector<float>;
    T_Elec_dr03HcalSumEt = new std::vector<float>;
    
    T_Elec_isPF = new std::vector<int>;
    T_Elec_PFenergy = new std::vector<float>;
    T_Elec_PFeta = new std::vector<float>;
    T_Elec_PFphi = new std::vector<float>;
    T_Elec_PFpt = new std::vector<float>;
    T_Elec_PFpx = new std::vector<float>;
    T_Elec_PFpy = new std::vector<float>;
    T_Elec_PFpz = new std::vector<float>;
    T_Elec_PFmva = new std::vector<float>;

    

	
    T_Gen_Elec_Px = new std::vector<float>;
    T_Gen_Elec_Py = new std::vector<float>;
    T_Gen_Elec_Pz = new std::vector<float>;
    T_Gen_Elec_Energy = new std::vector<float>;
    
	T_Muon_Eta = new std::vector<float>;
	T_Muon_Phi = new std::vector<float>;
	T_Muon_Energy = new std::vector<float>;
	T_Muon_Et = new std::vector<float>;
	T_Muon_Pt = new std::vector<float>;
	T_Muon_Px = new std::vector<float>;
	T_Muon_Py = new std::vector<float>;
	T_Muon_Pz = new std::vector<float>;
	T_Muon_Mass = new std::vector<float>;
	T_Muon_IsGlobalMuon = new std::vector<bool>;
	T_Muon_IsTrackerMuon = new std::vector<bool>;
	T_Muon_IsPFMuon = new std::vector<bool>;
	T_Muon_IsCaloMuon = new std::vector<bool>;
	T_Muon_IsStandAloneMuon = new std::vector<bool>;
	T_Muon_IsMuon = new std::vector<bool>;
	T_Muon_IsGlobalMuon_PromptTight = new std::vector<bool>;
	T_Muon_IsTrackerMuonArbitrated = new std::vector<bool>;
	T_Muon_numberOfChambers = new std::vector<int>;
	T_Muon_numberOfChambersRPC = new std::vector<int>;
	T_Muon_numberOfMatches = new std::vector<int>;
	T_Muon_numberOfMatchedStations = new std::vector<int>;
	T_Muon_charge = new std::vector<int>;
	T_Muon_TMLastStationTight = new std::vector<bool>;
	T_Muon_globalTrackChi2 = new std::vector<float>;
	T_Muon_validMuonHits = new std::vector<int>;
	T_Muon_trkKink = new std::vector<float>;
	T_Muon_trkNbOfValidTrackeHits = new std::vector<int>;
	T_Muon_trkNbOfTrackerLayers = new std::vector<int>;
	T_Muon_trkValidPixelHits = new std::vector<int>;
	T_Muon_trkError = new std::vector<float>;
	T_Muon_dB = new std::vector<float>;
	T_Muon_dzPV = new std::vector<float>;
    T_Muon_dBstop = new std::vector<float>;
	T_Muon_dzstop = new std::vector<float>;
	T_Muon_isoR03_emEt = new std::vector<float>;
	T_Muon_isoR03_hadEt = new std::vector<float>;
	T_Muon_isoR03_hoEt = new std::vector<float>;
	T_Muon_isoR03_sumPt = new std::vector<float>;
	T_Muon_isoR03_nTracks = new std::vector<int>;
	T_Muon_isoR03_nJets = new std::vector<int>;
	T_Muon_isoRingsMVA = new std::vector<float>;
    T_Muon_chargedHadronIsoR04 = new std::vector<float>;
    T_Muon_neutralHadronIsoR04 = new std::vector<float>;
    T_Muon_photonIsoR04 = new std::vector<float>;
    T_Muon_chargedHadronIsoPUR04 = new std::vector<float>;
    T_Muon_chargedHadronIsoR03 = new std::vector<float>;
    T_Muon_neutralHadronIsoR03 = new std::vector<float>;
    T_Muon_photonIsoR03 = new std::vector<float>;
    T_Muon_chargedHadronIsoPUR03 = new std::vector<float>;
	T_Muon_HLT_Mu17_TkMu8_Mu17Leg = new std::vector<int>;
	T_Muon_HLT_Mu17_TkMu8_Mu8Leg = new std::vector<int>;
	T_Muon_HLT_Mu17_Mu8_Mu17Leg = new std::vector<int>;
	T_Muon_HLT_Mu17_Mu8_Mu8Leg = new std::vector<int>;
	T_Muon_HLT_Mu17_obj = new std::vector<int>;
	T_Muon_HLT_Mu8_obj = new std::vector<int>;
	T_Muon_HLT_Mu8_Ele17_Mu8Leg = new std::vector<int>;
	T_Muon_HLT_Ele8_Mu17_Mu17Leg = new std::vector<int>;
	T_Muon_HLT_IsoMu24 = new std::vector<int>;
	T_Muon_HLT_IsoMu24_2p1 = new std::vector<int>;
    
    T_Gen_Muon_Px = new std::vector<float>;
    T_Gen_Muon_Py = new std::vector<float>;
    T_Gen_Muon_Pz = new std::vector<float>;
    T_Gen_Muon_Energy = new std::vector<float>;
    T_Gen_Muon_deltaR = new std::vector<float>;
    T_Gen_Muon_MCpart = new std::vector<int>;
    T_Gen_Muon_PDGid = new std::vector<int>;
    T_Gen_Muon_status = new std::vector<int>;
    T_Gen_Muon_MotherID = new std::vector<int>;

    
    
    T_PF_Et = new std::vector<float>;
    T_PF_Pt = new std::vector<float>;
    T_PF_Px = new std::vector<float>;
    T_PF_Py = new std::vector<float>;
    T_PF_Pz = new std::vector<float>;
    T_PF_Eta = new std::vector<float>;
    T_PF_Phi = new std::vector<float>;
    T_PF_pdgID = new std::vector<int>;
    T_PF_particleID = new std::vector<int>;
    T_PF_hasTrack = new std::vector<int>;
    T_PF_hasTrackWithMissingHits = new std::vector<int>;
    T_PF_isPU  = new std::vector<int>;
    
    T_Pho_Et = new std::vector<float>;
	T_Pho_Energy = new std::vector<float>;
	T_Pho_Pt = new std::vector<float>;
	T_Pho_Px = new std::vector<float>;
	T_Pho_Py = new std::vector<float>;
	T_Pho_Pz = new std::vector<float>;
	T_Pho_Eta = new std::vector<float>;
	T_Pho_Phi = new std::vector<float>;
	T_Pho_r9 = new std::vector<float>;
	T_Pho_EtaWidth = new std::vector<float>;
	T_Pho_PhiWidth = new std::vector<float>;
	T_Pho_sigmaIetaIeta = new std::vector<float>;
    T_Pho_indOfTheElec = new std::vector<int>;
	T_Pho_SCEt = new std::vector<float>;
	T_Pho_SCEnergy = new std::vector<float>;
	T_Pho_SCEta = new std::vector<float>;
	T_Pho_SCPhi = new std::vector<float>;
    
    T_Pho_isMatchedWithMC = new std::vector<int>;
	T_Pho_Gen_PDGid = new std::vector<int>;
	T_Pho_Gen_Status = new std::vector<int>;
	T_Pho_Gen_MotherID = new std::vector<int>;

	
    T_Conv_EleInd = new std::vector<int>;
	T_Conv_PhoInd = new std::vector<int>;
	T_Conv_vtxProb = new std::vector<float>;
	T_Conv_lxy = new std::vector<float>;
	T_Conv_nHitsMax = new std::vector<int>;
    
    
    
    T_Jet_Px = new std::vector<float>;
    T_Jet_Py = new std::vector<float>;
    T_Jet_Pz = new std::vector<float>;
    T_Jet_Et = new std::vector<float>;
    T_Jet_Eta = new std::vector<float>;
    T_Jet_Energy = new std::vector<float>;
    T_Jet_Phi = new std::vector<float>;
    T_Jet_Corr = new std::vector<float>;
    
    
}

void ElecIdAnalyzer::endEvent(){
    //Vertex 
    delete T_Vertex_z;
    delete T_Vertex_y;
    delete T_Vertex_x;
    delete T_Vertex_Chi2Prob;
    delete T_Vertex_rho;
    delete T_Vertex_ndof;
    delete T_Vertex_isFake; 
    delete T_Vertex_tracksSize;
    
    //Electrons
    delete T_Elec_Eta;
    delete T_Elec_IPwrtAveBS;
    delete T_Elec_IPwrtPV;
    delete T_Elec_Px;
    delete T_Elec_Py;
    delete T_Elec_Pz;
    delete T_Elec_Pt;
    delete T_Elec_Energy;
    delete T_Elec_Charge;
    delete T_Elec_vz;
    delete T_Elec_vy;
    delete T_Elec_vx;
    delete T_Elec_nLost;
    delete T_Elec_nHits;
    delete T_Elec_SC_Et;
    delete T_Elec_SC_Eta;
    delete T_Elec_nBrems;
    delete T_Elec_fBrem;
    delete T_Elec_eSuperClusterOverP;
    delete T_Elec_isEB;
    delete T_Elec_isEE;
    delete T_Elec_MVA;
    delete T_Elec_simpleEleId95;
    delete T_Elec_simpleEleId90;
    delete T_Elec_simpleEleId85;
    delete T_Elec_simpleEleId80;
    delete T_Elec_simpleEleId70;
    delete T_Elec_simpleEleId60;
    delete T_Elec_sigmaIetaIeta;
    delete T_Elec_deltaPhiIn;
    delete T_Elec_deltaEtaIn;
    delete T_Elec_isEcalDriven;
    delete T_Elec_HtoE;
    delete T_Elec_chargedHadronIso;
    delete T_Elec_puChargedHadronIso04;
    delete T_Elec_allChargedHadronIso04;
    delete T_Elec_neutralHadronIso;
    delete T_Elec_photonIso;
    delete T_Elec_chargedHadronIso04;
    delete T_Elec_neutralHadronIso04;
    delete T_Elec_photonIso04;
    delete T_Elec_puChargedHadronIso;
    delete T_Elec_allChargedHadronIso;
    delete T_Elec_passConversionVeto;
    delete T_Elec_passVeto;
    delete T_Elec_passLoose;
    delete T_Elec_passMedium;
    delete T_Elec_passTight;
    delete T_Elec_passFbremopin;
    delete T_Elec_passtrigTight;
    delete T_Elec_passtrigwp70;
    delete T_Elec_isTrig;
    delete T_Elec_MVAid_trig;
    delete T_Elec_MVAid_Nontrig;
    delete T_Elec_Mvaiso;
    delete T_Elec_RadialIso;
    delete T_Elec_RadialIsoVeto;
    delete T_Elec_RadialIsoVetoMore;
    
    delete T_Elec_ChargedIso_DR0p0To0p1;
    delete T_Elec_ChargedIso_DR0p1To0p2;
    delete T_Elec_ChargedIso_DR0p2To0p3;
    delete T_Elec_ChargedIso_DR0p3To0p4;
    delete T_Elec_ChargedIso_DR0p4To0p5;
    delete T_Elec_GammaIso_DR0p0To0p1;
    delete T_Elec_GammaIso_DR0p1To0p2;
    delete T_Elec_GammaIso_DR0p2To0p3;
    delete T_Elec_GammaIso_DR0p3To0p4;
    delete T_Elec_GammaIso_DR0p4To0p5;
    delete T_Elec_NeutralHadronIso_DR0p0To0p1;
    delete T_Elec_NeutralHadronIso_DR0p1To0p2;
    delete T_Elec_NeutralHadronIso_DR0p2To0p3;
    delete T_Elec_NeutralHadronIso_DR0p3To0p4;
    delete T_Elec_NeutralHadronIso_DR0p4To0p5;
    
    delete T_Elec_HLT_Elec27_WP80;
    delete T_Elec_HLT_Ele17TightID_Ele8_Ele8Leg;
    delete T_Elec_HLT_Ele17TightID_Ele8_Ele17Leg;
    delete T_Elec_HLT_Ele17_Ele8_Ele8Leg;
    delete T_Elec_HLT_Ele17_Ele8_Ele17Leg;
    delete T_Elec_HLT_Ele17_Ele8_TnP_Ele8Leg;
    delete T_Elec_HLT_Ele17_Ele8_TnP_Ele17Leg;
    delete T_Elec_HLT_Ele20_SC4_TnP_SC4Leg;
    delete T_Elec_HLT_Ele20_SC4_TnP_Ele20Leg;
    delete T_Elec_HLT_Mu8_Ele17_Ele17Leg;
    delete T_Elec_HLT_Ele8_Mu17_Ele8Leg;
    
    delete T_Elec_passMVA;
    delete T_Elec_kfchi2;
    delete T_Elec_kfhits;
    delete T_Elec_gsfchi2;
    delete T_Elec_detacalo;
    delete T_Elec_see;
    delete T_Elec_spp;
    delete T_Elec_etawidth;
    delete T_Elec_phiwidth;
    delete T_Elec_e1x5e5x5;
    delete T_Elec_R9;
    delete T_Elec_EoP;
    delete T_Elec_ESeedoP;
    delete T_Elec_ESeedoPout;
    delete T_Elec_IoEmIoP;
    delete T_Elec_eleEoPout;
    delete T_Elec_EcalEnergy;
    delete T_Elec_TrackPatVtx;
    delete T_Elec_PreShowerOverRaw;
    delete T_Elec_d0;
    delete T_Elec_IP3D;
    delete T_Elec_dZ;
    delete T_Elec_isFO;
    delete T_Elec_CombIsoHWW;
    delete T_Elec_dr03TkSumPt;
    delete T_Elec_dr03EcalSumEt;
    delete T_Elec_dr03HcalSumEt;
    
    delete T_Elec_isPF;
    delete T_Elec_PFenergy;
    delete T_Elec_PFeta;
    delete T_Elec_PFphi;
    delete T_Elec_PFpt;
    delete T_Elec_PFpx;
    delete T_Elec_PFpy;
    delete T_Elec_PFpz;
    delete T_Elec_PFmva;

  
    
    delete T_Gen_Elec_Px;
    delete T_Gen_Elec_Py;
    delete T_Gen_Elec_Pz;
    delete T_Gen_Elec_Energy;
    delete T_Gen_Elec_MCpart;    
    delete T_Gen_Elec_PDGid;    
    delete T_Gen_Elec_status;
    delete T_Gen_Elec_MotherID;
    delete T_Gen_Elec_deltaR;
	
	//Muons
	delete T_Muon_Eta;
	delete T_Muon_Phi;
	delete T_Muon_Energy;
	delete T_Muon_Et;
	delete T_Muon_Pt;
	delete T_Muon_Px;
	delete T_Muon_Py;
	delete T_Muon_Pz;
	delete T_Muon_Mass;
	delete T_Muon_IsGlobalMuon;
	delete T_Muon_IsTrackerMuon;
	delete T_Muon_IsPFMuon;
	delete T_Muon_IsCaloMuon;
	delete T_Muon_IsStandAloneMuon;
	delete T_Muon_IsMuon;
	delete T_Muon_IsTrackerMuonArbitrated;
	delete T_Muon_IsGlobalMuon_PromptTight;
	delete T_Muon_numberOfChambers;
	delete T_Muon_numberOfChambersRPC;
	delete T_Muon_numberOfMatches;
	delete T_Muon_numberOfMatchedStations;
	delete T_Muon_charge;
	delete T_Muon_TMLastStationTight;
	delete T_Muon_globalTrackChi2;
	delete T_Muon_validMuonHits;
	delete T_Muon_trkKink;
	delete T_Muon_trkNbOfTrackerLayers;
    delete T_Muon_trkNbOfValidTrackeHits;
	delete T_Muon_trkValidPixelHits;
	delete T_Muon_trkError;
	delete T_Muon_dB;
	delete T_Muon_dzPV;
    delete T_Muon_dBstop;
	delete T_Muon_dzstop;
	delete T_Muon_isoR03_emEt;
	delete T_Muon_isoR03_hadEt;
	delete T_Muon_isoR03_hoEt;
	delete T_Muon_isoR03_sumPt;
	delete T_Muon_isoR03_nTracks;
	delete T_Muon_isoR03_nJets;
	delete T_Muon_isoRingsMVA;
	delete T_Muon_chargedHadronIsoR04;
	delete T_Muon_neutralHadronIsoR04;
	delete T_Muon_photonIsoR04;
	delete T_Muon_chargedHadronIsoPUR04;
    delete T_Muon_chargedHadronIsoR03;
	delete T_Muon_neutralHadronIsoR03;
	delete T_Muon_photonIsoR03;
	delete T_Muon_chargedHadronIsoPUR03;
	delete T_Muon_HLT_Mu17_TkMu8_Mu17Leg;
	delete T_Muon_HLT_Mu17_TkMu8_Mu8Leg;
	delete T_Muon_HLT_Mu17_Mu8_Mu17Leg;
	delete T_Muon_HLT_Mu17_Mu8_Mu8Leg;
	delete T_Muon_HLT_Mu17_obj;
	delete T_Muon_HLT_Mu8_obj;
	delete T_Muon_HLT_Mu8_Ele17_Mu8Leg;
	delete T_Muon_HLT_Ele8_Mu17_Mu17Leg;
	delete T_Muon_HLT_IsoMu24;
	delete T_Muon_HLT_IsoMu24_2p1;
    
    delete T_Gen_Muon_Px;
    delete T_Gen_Muon_Py;
    delete T_Gen_Muon_Pz;
    delete T_Gen_Muon_Energy;
    delete T_Gen_Muon_deltaR;
    delete T_Gen_Muon_MCpart;
    delete T_Gen_Muon_PDGid;
    delete T_Gen_Muon_status;
    delete T_Gen_Muon_MotherID;
    
    
    delete T_PF_Et;
    delete T_PF_Pt;
    delete T_PF_Px;
    delete T_PF_Py;
    delete T_PF_Pz;
    delete T_PF_Eta; 
    delete T_PF_Phi;
    delete T_PF_pdgID;
    delete T_PF_particleID;
    delete T_PF_hasTrackWithMissingHits;
    delete T_PF_hasTrack;
    delete T_PF_isPU;
    
    delete T_Pho_Et;
	delete T_Pho_Energy;
	delete T_Pho_Pt;
	delete T_Pho_Px;
	delete T_Pho_Py;
	delete T_Pho_Pz;
	delete T_Pho_Eta;
	delete T_Pho_Phi;
	delete T_Pho_r9;
	delete T_Pho_EtaWidth;
	delete T_Pho_PhiWidth;
	delete T_Pho_sigmaIetaIeta;
    delete T_Pho_indOfTheElec;
	delete T_Pho_SCEt;
	delete T_Pho_SCEnergy;
	delete T_Pho_SCEta;
	delete T_Pho_SCPhi;
    delete T_Pho_isMatchedWithMC;
	delete T_Pho_Gen_PDGid;
	delete T_Pho_Gen_Status;
	delete T_Pho_Gen_MotherID;
    
	delete T_Conv_EleInd;
	delete T_Conv_PhoInd;
	delete T_Conv_vtxProb;
	delete T_Conv_lxy;
	delete T_Conv_nHitsMax;
   
    delete T_Jet_Px;
    delete T_Jet_Py;
    delete T_Jet_Pz;
    delete T_Jet_Et;
    delete T_Jet_Eta;
    delete T_Jet_Energy;
    delete T_Jet_Phi;
    delete T_Jet_Corr;
     
}

bool ElecIdAnalyzer::trainTrigPresel(const reco::GsfElectron& ele) {
    
    bool myTrigPresel = false;
    if(fabs(ele.superCluster()->eta()) < 1.479) {
        if(ele.sigmaIetaIeta() < 0.014 &&
           ele.hadronicOverEm() < 0.15 &&
           ele.dr03TkSumPt()/ele.pt() < 0.2 &&
           ele.dr03EcalRecHitSumEt()/ele.pt() < 0.2 &&
           ele.dr03HcalTowerSumEt()/ele.pt() < 0.2 &&
           ele.gsfTrack()->trackerExpectedHitsInner().numberOfLostHits() == 0)
            myTrigPresel = true;
    }
    else {
        if(ele.sigmaIetaIeta() < 0.035 &&
           ele.hadronicOverEm() < 0.10 &&
           ele.dr03TkSumPt()/ele.pt() < 0.2 &&
           ele.dr03EcalRecHitSumEt()/ele.pt() < 0.2 &&
           ele.dr03HcalTowerSumEt()/ele.pt() < 0.2 &&
           ele.gsfTrack()->trackerExpectedHitsInner().numberOfLostHits() == 0)
            myTrigPresel = true;
    }
    return myTrigPresel;
}
double ElecIdAnalyzer::GetRadialIsoValue(const reco::GsfElectron& electron, 
                                         const reco::PFCandidateCollection &PFCandidates){
    
    double RadialIso = 0;
   // GsfTrackRef elecTrack = electron.gsfTrack();
    

    if (electron.gsfTrack().isNull()) {
        //if muon is not standalone either, then return -9999
        return -9999;
    }
    
 for (reco::PFCandidateCollection::const_iterator iP = PFCandidates.begin(); iP != PFCandidates.end(); ++iP) {
        //exclude the electron itself
        if(iP->trackRef().isNonnull() && electron.gsfTrack().isNonnull() &&
           refToPtr(iP->trackRef()) == refToPtr(electron.closestTrack())) continue;
        
        //************************************************************
        // New Isolation Calculations
        //************************************************************
      double dr = sqrt(pow(iP->eta() - electron.eta(),2) + pow(acos(cos(iP->phi() - electron.phi())),2));
        
        if (dr > 0.3)  continue;
        if (dr < 0.01) continue; 
        
        //Charged
        if(iP->trackRef().isNonnull()) {	  	   
            // Veto any PFmuon, or PFEle
            if (iP->particleId() == reco::PFCandidate::e || iP->particleId() == reco::PFCandidate::mu) continue;
            RadialIso += iP->pt() * (1 - 3*dr) / electron.pt();
        }
        else if (iP->pt() > 1.0) {
            RadialIso += iP->pt() * (1 - 3*dr) / electron.pt();
        } 
    } //loop over PF candidate
    
    return RadialIso;
}

double ElecIdAnalyzer::GetRadialIsoValueVeto(const reco::GsfElectron& electron, 
                                         const reco::PFCandidateCollection &PFCandidates){
    
    double RadialIso = 0;
    // GsfTrackRef elecTrack = electron.gsfTrack();
    
    
    if (electron.gsfTrack().isNull()) {
        //if muon is not standalone either, then return -9999
        return -9999;
    }
    
    for (reco::PFCandidateCollection::const_iterator iP = PFCandidates.begin(); iP != PFCandidates.end(); ++iP) {
        //exclude the electron itself
        if(iP->gsfTrackRef().isNonnull() && electron.gsfTrack().isNonnull() &&
           refToPtr(iP->gsfTrackRef()) == refToPtr(electron.gsfTrack())) continue;
        if(iP->trackRef().isNonnull() && electron.closestCtfTrackRef().isNonnull() &&
           refToPtr(iP->trackRef()) == refToPtr(electron.closestCtfTrackRef())) continue;   

        //if pf candidate lies in veto regions of the electron, then veto it
        double tmpDR = sqrt(pow(iP->eta() - electron.eta(),2) + pow(acos(cos(iP->phi() - electron.phi())),2));
        if(iP->trackRef().isNonnull() && fabs(electron.superCluster()->eta()) >= 1.479 
           && tmpDR < 0.015) continue;
        if(iP->particleId() == reco::PFCandidate::gamma && fabs(electron.superCluster()->eta()) >= 1.479 
           && tmpDR < 0.08) continue;
        
        //************************************************************
        // New Isolation Calculations
        //************************************************************
        double dr = sqrt(pow(iP->eta() - electron.eta(),2) + pow(acos(cos(iP->phi() - electron.phi())),2));
        
        if (dr > 0.3)  continue;
        if (dr < 0.01) continue; 
        
        //Charged
        if(iP->trackRef().isNonnull()) {	  	   
            // Veto any PFmuon, or PFEle
            if (iP->particleId() == reco::PFCandidate::e || iP->particleId() == reco::PFCandidate::mu) continue;
            RadialIso += iP->pt() * (1 - 3*dr) / electron.pt();
        }
        else if (iP->pt() > 1.0) {
            RadialIso += iP->pt() * (1 - 3*dr) / electron.pt();
        } 
    } //loop over PF candidate
    
    return RadialIso;
}

double ElecIdAnalyzer::GetRadialIsoValueVetoMore(const reco::GsfElectron& electron, 
                                                 const reco::PFCandidateCollection &PFCandidates,
                                                 const reco::GsfElectronCollection &IdentifiedElectrons,
                                                 const reco::MuonCollection &IdentifiedMuons){
    
    double RadialIso = 0;
    // GsfTrackRef elecTrack = electron.gsfTrack();
    
    
    if (electron.gsfTrack().isNull()) {
        //if muon is not standalone either, then return -9999
        return -9999;
    }
    
    for (reco::PFCandidateCollection::const_iterator iP = PFCandidates.begin(); iP != PFCandidates.end(); ++iP) {
        //exclude the electron itself
        if(iP->gsfTrackRef().isNonnull() && electron.gsfTrack().isNonnull() &&
           refToPtr(iP->gsfTrackRef()) == refToPtr(electron.gsfTrack())) continue;
        if(iP->trackRef().isNonnull() && electron.closestCtfTrackRef().isNonnull() &&
           refToPtr(iP->trackRef()) == refToPtr(electron.closestCtfTrackRef())) continue;   
        
        //if pf candidate lies in veto regions of the electron, then veto it
        double tmpDR = sqrt(pow(iP->eta() - electron.eta(),2) + pow(acos(cos(iP->phi() - electron.phi())),2));
        if(iP->trackRef().isNonnull() && fabs(electron.superCluster()->eta()) >= 1.479 
           && tmpDR < 0.015) continue;
        if(iP->particleId() == reco::PFCandidate::gamma && fabs(electron.superCluster()->eta()) >= 1.479 
           && tmpDR < 0.08) continue;
        Bool_t IsLeptonFootprint = kFALSE;
        //************************************************************
        // Lepton Footprint Removal
        //************************************************************   
        for (reco::GsfElectronCollection::const_iterator iE = IdentifiedElectrons.begin(); 
             iE != IdentifiedElectrons.end(); ++iE) {
            //if pf candidate matches an electron passing ID cuts, then veto it
            if(iP->gsfTrackRef().isNonnull() && iE->gsfTrack().isNonnull() &&
               refToPtr(iP->gsfTrackRef()) == refToPtr(iE->gsfTrack())) IsLeptonFootprint = kTRUE;
            if(iP->trackRef().isNonnull() && iE->closestCtfTrackRef().isNonnull() &&
               refToPtr(iP->trackRef()) == refToPtr(iE->closestCtfTrackRef())) IsLeptonFootprint = kTRUE;
            
            //if pf candidate lies in veto regions of electron passing ID cuts, then veto it
            double tmpDR = sqrt(pow(iP->eta() - iE->eta(),2) + pow(acos(cos(iP->phi() - iE->phi())),2));
            if(iP->trackRef().isNonnull() && fabs(iE->superCluster()->eta()) >= 1.479 
               && tmpDR < 0.015) IsLeptonFootprint = kTRUE;
            if(iP->particleId() == reco::PFCandidate::gamma && fabs(iE->superCluster()->eta()) >= 1.479 
               && tmpDR < 0.08) IsLeptonFootprint = kTRUE;
        }
        for (reco::MuonCollection::const_iterator iM = IdentifiedMuons.begin(); 
             iM != IdentifiedMuons.end(); ++iM) {
            //if pf candidate matches an muon passing ID cuts, then veto it
            if(iP->trackRef().isNonnull() && iM->innerTrack().isNonnull() &&
               refToPtr(iP->trackRef()) == refToPtr(iM->innerTrack())) IsLeptonFootprint = kTRUE;
            
            //if pf candidate lies in veto regions of muon passing ID cuts, then veto it
            double tmpDR = sqrt(pow(iP->eta() - iM->eta(),2) + pow(acos(cos(iP->phi() - iM->phi())),2));
            if(iP->trackRef().isNonnull() && tmpDR < 0.01) IsLeptonFootprint = kTRUE;
        }
        if (IsLeptonFootprint) continue;
        //************************************************************
        // New Isolation Calculations
        //************************************************************
        double dr = sqrt(pow(iP->eta() - electron.eta(),2) + pow(acos(cos(iP->phi() - electron.phi())),2));
        
        if (dr > 0.3)  continue;
        if (dr < 0.01) continue; 
        
        //Charged
        if(iP->trackRef().isNonnull()) {	  	   
            // Veto any PFmuon, or PFEle
            if (iP->particleId() == reco::PFCandidate::e || iP->particleId() == reco::PFCandidate::mu) continue;
            RadialIso += iP->pt() * (1 - 3*dr) / electron.pt();
        }
        else if (iP->pt() > 1.0) {
            RadialIso += iP->pt() * (1 - 3*dr) / electron.pt();
        } 
    } //loop over PF candidate
    
    return RadialIso;
}


/*void 
ElecIdAnalyzer::doMCtruth(reco::GsfElectronRef theElec, edm::Handle <reco::GenParticleCollection> genParts, double dR)
{
    int nbOfGen = genParts->size();
   // cout << "electron pt=" << theElec->pt() << " eta=" << theElec->eta() << " phi=" << theElec->phi() << endl;
    float minDiff= 100; 
    int iteDiff = -1000; 
    //reco::GenParticle & theCand = (*genParts)[0];
    for (int j = 0 ; j < nbOfGen ; j++){
        const reco::GenParticle & p = (*genParts)[j];
        float theDeltaR = deltaR(p.phi(), theElec->phi(), p.eta(), theElec->eta());
        if (theDeltaR > dR) continue;
        if (!(p.status()==1)) continue;
        float theDiff = fabs(p.pt()-theElec->pt())/p.pt();
        if (theDiff < minDiff){
            minDiff = theDiff;
            iteDiff = j;            
        }
    }
    T_Gen_Elec_Px->push_back(-1);
    T_Gen_Elec_Py->push_back(-1);
    T_Gen_Elec_Pz->push_back(-1);
    T_Gen_Elec_Energy->push_back(-1);
    T_Gen_Elec_MCpart->push_back(-1);
    T_Gen_Elec_PDGid->push_back(-1);
    T_Gen_Elec_status->push_back(-1);
    T_Gen_Elec_MotherID->push_back(-1);
    
    if (iteDiff>=0) {
        const reco::GenParticle & theCand = (*genParts)[iteDiff];
        const reco::Candidate * mom = theCand.mother();
   //     cout << "ID =" << theCand.pdgId() << " pt = " << theCand.pt() << " status=" << theCand.status() << endl;
     //   cout << "nb of mother " << theCand.numberOfMothers() << endl;
        if (theCand.numberOfMothers()>0) mom = theCand.mother();
     //   cout << "mother id " << mom->pdgId() << endl; 
        T_Gen_Elec_Px->push_back(theCand.px());
        T_Gen_Elec_Py->push_back(theCand.py());
        T_Gen_Elec_Pz->push_back(theCand.pz());
        T_Gen_Elec_Energy->push_back(theCand.energy());
        T_Gen_Elec_MCpart->push_back(1);
        T_Gen_Elec_PDGid->push_back(theCand.pdgId());
        T_Gen_Elec_status->push_back(theCand.status());
        if (theCand.numberOfMothers()>0) T_Gen_Elec_MotherID->push_back(mom->pdgId());  
        else T_Gen_Elec_MotherID->push_back(-1); 
    }
    else{
        T_Gen_Elec_Px->push_back(-1);
        T_Gen_Elec_Py->push_back(-1);
        T_Gen_Elec_Pz->push_back(-1);
        T_Gen_Elec_Energy->push_back(-1);
        T_Gen_Elec_MCpart->push_back(0);
        T_Gen_Elec_PDGid->push_back(-1);
        T_Gen_Elec_status->push_back(-1);
        T_Gen_Elec_MotherID->push_back(-1);      
    }
    
}*/


void 
ElecIdAnalyzer::doMCtruth(reco::GsfElectronRef theElec, edm::Handle <reco::GenParticleCollection> genParts, double dR)
{
    int nbOfGen = genParts->size();
    // cout << "electron pt=" << theElec->pt() << " eta=" << theElec->eta() << " phi=" << theElec->phi() << endl;
    float minDiff= 100; 
    int iteDiff = -1000;
    int motherID = 0;
    //reco::GenParticle & theCand = (*genParts)[0];
    for (int j = 0 ; j < nbOfGen ; j++){
        const reco::GenParticle & p = (*genParts)[j];
        float theDeltaR = deltaR(p.phi(), theElec->phi(), p.eta(), theElec->eta());
        if (theDeltaR > 0.2) continue;
        if (!(p.status()==1)) continue;
        if (fabs(p.pdgId())!=11) continue;
        // find the mother of the particle !
        const reco::Candidate * theLocalCandidate = &p;
        bool hasMother = (theLocalCandidate->numberOfMothers()>0);
        const reco::Candidate * theMother;
        while (hasMother) {
            theMother = theLocalCandidate->mother();
        //    cout << "mum PDGid = " << theMother->pdgId() << endl;
            theLocalCandidate = theMother;
            hasMother = (theLocalCandidate->numberOfMothers()>0);
            motherID = theMother->pdgId();
            if ((theMother->pdgId()==23)||(theMother->pdgId()==22)) break;
        }
      //  cout << "fin " << endl;
   //     float theDiff = fabs(p.pt()-theElec->pt())/p.pt();
   //     if (theDeltaR < minDiff){
            minDiff = theDeltaR;
            iteDiff = j;            
      //  }
    }

    
    if (iteDiff>=0) {
        const reco::GenParticle & theCand = (*genParts)[iteDiff];
        const reco::Candidate * mom = theCand.mother();
        //     cout << "ID =" << theCand.pdgId() << " pt = " << theCand.pt() << " status=" << theCand.status() << endl;
        //   cout << "nb of mother " << theCand.numberOfMothers() << endl;
        if (theCand.numberOfMothers()>0) mom = theCand.mother();
        //   cout << "mother id " << mom->pdgId() << endl; 
        T_Gen_Elec_Px->push_back(theCand.px());
        T_Gen_Elec_Py->push_back(theCand.py());
        T_Gen_Elec_Pz->push_back(theCand.pz());
        T_Gen_Elec_Energy->push_back(theCand.energy());
        T_Gen_Elec_MCpart->push_back(1);
        T_Gen_Elec_PDGid->push_back(theCand.pdgId());
        T_Gen_Elec_status->push_back(theCand.status());
        //if (theCand.numberOfMothers()>0) T_Gen_Elec_MotherID->push_back(mom->pdgId());
        if (theCand.numberOfMothers()>0) T_Gen_Elec_MotherID->push_back(motherID);
        else T_Gen_Elec_MotherID->push_back(-1);
        T_Gen_Elec_deltaR->push_back(minDiff);
    }
    else{
        T_Gen_Elec_Px->push_back(-1);
        T_Gen_Elec_Py->push_back(-1);
        T_Gen_Elec_Pz->push_back(-1);
        T_Gen_Elec_Energy->push_back(-1);
        T_Gen_Elec_MCpart->push_back(0);
        T_Gen_Elec_PDGid->push_back(-1);
        T_Gen_Elec_status->push_back(-1);
        T_Gen_Elec_MotherID->push_back(-1);  
        T_Gen_Elec_deltaR->push_back(-1);
        
    }
    
}

void
ElecIdAnalyzer::doMCtruthMuons(const reco::Muon* theMuon, edm::Handle <reco::GenParticleCollection> genParts, double dR)
{
    int nbOfGen = genParts->size();
    float minDiff= 100;
    int iteDiff = -1000;
    int motherID = 0;
    //reco::GenParticle & theCand = (*genParts)[0];
    for (int j = 0 ; j < nbOfGen ; j++){
        const reco::GenParticle & p = (*genParts)[j];
        float theDeltaR = deltaR(p.phi(), theMuon->phi(), p.eta(), theMuon->eta());
        if (theDeltaR > 0.2) continue;
        if (!(p.status()==1)) continue;
        if (fabs(p.pdgId())!=13) continue;
        // find the mother of the particle !
        const reco::Candidate * theLocalCandidate = &p;
        bool hasMother = (theLocalCandidate->numberOfMothers()>0);
        const reco::Candidate * theMother;
        while (hasMother) {
            theMother = theLocalCandidate->mother();
            //cout << "mum PDGid = " << theMother->pdgId() << endl;
            theLocalCandidate = theMother;
            hasMother = (theLocalCandidate->numberOfMothers()>0);
            motherID = theMother->pdgId();
            if ((theMother->pdgId()==23)||(theMother->pdgId()==22)) break;
        }
        //  cout << "fin " << endl;
        //     float theDiff = fabs(p.pt()-theElec->pt())/p.pt();
        //     if (theDeltaR < minDiff){
        minDiff = theDeltaR;
        iteDiff = j;
        //  }
    }
    
    
    if (iteDiff>=0) {
        const reco::GenParticle & theCand = (*genParts)[iteDiff];
        const reco::Candidate * mom = theCand.mother();
        //     cout << "ID =" << theCand.pdgId() << " pt = " << theCand.pt() << " status=" << theCand.status() << endl;
       //    cout << "nb of mother " << theCand.numberOfMothers() << endl;
        if (theCand.numberOfMothers()>0) mom = theCand.mother();
        //   cout << "mother id " << mom->pdgId() << endl;
        T_Gen_Muon_Px->push_back(theCand.px());
        T_Gen_Muon_Py->push_back(theCand.py());
        T_Gen_Muon_Pz->push_back(theCand.pz());
        T_Gen_Muon_Energy->push_back(theCand.energy());
        T_Gen_Muon_MCpart->push_back(1);
        T_Gen_Muon_PDGid->push_back(theCand.pdgId());
        T_Gen_Muon_status->push_back(theCand.status());
        //if (theCand.numberOfMothers()>0) T_Gen_Muon_MotherID->push_back(mom->pdgId());
        if (theCand.numberOfMothers()>0) T_Gen_Muon_MotherID->push_back(motherID);
        else T_Gen_Muon_MotherID->push_back(-1);
        T_Gen_Muon_deltaR->push_back(minDiff);
    }
    else{
        T_Gen_Muon_Px->push_back(-1);
        T_Gen_Muon_Py->push_back(-1);
        T_Gen_Muon_Pz->push_back(-1);
        T_Gen_Muon_Energy->push_back(-1);
        T_Gen_Muon_MCpart->push_back(0);
        T_Gen_Muon_PDGid->push_back(-1);
        T_Gen_Muon_status->push_back(-1);
        T_Gen_Muon_MotherID->push_back(-1);
        T_Gen_Muon_deltaR->push_back(-1);
        
    }
    
}

float ElecIdAnalyzer::deltaR(float phi1, float phi2, float eta1, float eta2)
{
    float dphi=deltaPhi(phi1,phi2);
    float deta=fabs(eta1-eta2);
    float dr = sqrt(dphi*dphi+ deta*deta);
    return dr;
}


float ElecIdAnalyzer::deltaPhi(float phi1, float phi2)
{
    float dphi;
    if(phi1<0) phi1+=2*TMath::Pi();
    if(phi2<0) phi2+=2*TMath::Pi();
    dphi=fabs(phi1-phi2);
    if(dphi>2*TMath::Pi()) dphi-=2*TMath::Pi();
    if(dphi>TMath::Pi()) dphi=2*TMath::Pi()-dphi;
    return dphi;
}

void 
ElecIdAnalyzer::fillIsoRings(const reco::GsfElectron& ele, 
                             const reco::Vertex& vertex, 
                             const reco::PFCandidateCollection &PFCandidates,
                             double Rho,
                             ElectronEffectiveArea::ElectronEffectiveAreaTarget EATarget,
                             const reco::GsfElectronCollection &IdentifiedElectrons,
                             const reco::MuonCollection &IdentifiedMuons)
{
    
    float fMVAVar_eta = ele.superCluster()->eta(); 
    //**********************************************************
    //Isolation variables
    //**********************************************************
    Double_t tmpChargedIso_DR0p0To0p1  = 0;
    Double_t tmpChargedIso_DR0p1To0p2  = 0;
    Double_t tmpChargedIso_DR0p2To0p3  = 0;
    Double_t tmpChargedIso_DR0p3To0p4  = 0;
    Double_t tmpChargedIso_DR0p4To0p5  = 0;
    Double_t tmpGammaIso_DR0p0To0p1  = 0;
    Double_t tmpGammaIso_DR0p1To0p2  = 0;
    Double_t tmpGammaIso_DR0p2To0p3  = 0;
    Double_t tmpGammaIso_DR0p3To0p4  = 0;
    Double_t tmpGammaIso_DR0p4To0p5  = 0;
    Double_t tmpNeutralHadronIso_DR0p0To0p1  = 0;
    Double_t tmpNeutralHadronIso_DR0p1To0p2  = 0;
    Double_t tmpNeutralHadronIso_DR0p2To0p3  = 0;
    Double_t tmpNeutralHadronIso_DR0p3To0p4  = 0;
    Double_t tmpNeutralHadronIso_DR0p4To0p5  = 0;
    
    double electronTrackZ = 0;
    if (ele.gsfTrack().isNonnull()) {
        electronTrackZ = ele.gsfTrack()->dz(vertex.position());
    } else if (ele.closestCtfTrackRef().isNonnull()) {
        electronTrackZ = ele.closestCtfTrackRef()->dz(vertex.position());
    }
    
    for (reco::PFCandidateCollection::const_iterator iP = PFCandidates.begin(); 
         iP != PFCandidates.end(); ++iP) {
        
        //exclude the electron itself
        if(iP->gsfTrackRef().isNonnull() && ele.gsfTrack().isNonnull() &&
           refToPtr(iP->gsfTrackRef()) == refToPtr(ele.gsfTrack())) continue;
        if(iP->trackRef().isNonnull() && ele.closestCtfTrackRef().isNonnull() &&
           refToPtr(iP->trackRef()) == refToPtr(ele.closestCtfTrackRef())) continue;      
        
        //************************************************************
        // New Isolation Calculations
        //************************************************************
        double dr = sqrt(pow(iP->eta() - ele.eta(),2) + pow(acos(cos(iP->phi() - ele.phi())),2));
        //Double_t deta = (iP->eta() - ele.eta());
        
        if (dr < 1.0) {
            Bool_t IsLeptonFootprint = kFALSE;
            //************************************************************
            // Lepton Footprint Removal
            //************************************************************   
            for (reco::GsfElectronCollection::const_iterator iE = IdentifiedElectrons.begin(); 
                 iE != IdentifiedElectrons.end(); ++iE) {
                //if pf candidate matches an electron passing ID cuts, then veto it
                if(iP->gsfTrackRef().isNonnull() && iE->gsfTrack().isNonnull() &&
                   refToPtr(iP->gsfTrackRef()) == refToPtr(iE->gsfTrack())) IsLeptonFootprint = kTRUE;
                if(iP->trackRef().isNonnull() && iE->closestCtfTrackRef().isNonnull() &&
                   refToPtr(iP->trackRef()) == refToPtr(iE->closestCtfTrackRef())) IsLeptonFootprint = kTRUE;
                
                //if pf candidate lies in veto regions of electron passing ID cuts, then veto it
                double tmpDR = sqrt(pow(iP->eta() - iE->eta(),2) + pow(acos(cos(iP->phi() - iE->phi())),2));
                if(iP->trackRef().isNonnull() && fabs(iE->superCluster()->eta()) >= 1.479 
                   && tmpDR < 0.015) IsLeptonFootprint = kTRUE;
                if(iP->particleId() == reco::PFCandidate::gamma && fabs(iE->superCluster()->eta()) >= 1.479 
                   && tmpDR < 0.08) IsLeptonFootprint = kTRUE;
            }
            for (reco::MuonCollection::const_iterator iM = IdentifiedMuons.begin(); 
                 iM != IdentifiedMuons.end(); ++iM) {
                //if pf candidate matches an muon passing ID cuts, then veto it
                if(iP->trackRef().isNonnull() && iM->innerTrack().isNonnull() &&
                   refToPtr(iP->trackRef()) == refToPtr(iM->innerTrack())) IsLeptonFootprint = kTRUE;
                
                //if pf candidate lies in veto regions of muon passing ID cuts, then veto it
                double tmpDR = sqrt(pow(iP->eta() - iM->eta(),2) + pow(acos(cos(iP->phi() - iM->phi())),2));
                if(iP->trackRef().isNonnull() && tmpDR < 0.01) IsLeptonFootprint = kTRUE;
            }
            
            if (!IsLeptonFootprint) {
                Bool_t passVeto = kTRUE;
                //Charged
                if(iP->trackRef().isNonnull()) {	  	   
                    if (!(fabs(iP->trackRef()->dz(vertex.position()) - electronTrackZ) < 0.2)) passVeto = kFALSE;
                    //************************************************************
                    // Veto any PFmuon, or PFEle
                    if (iP->particleId() == reco::PFCandidate::e || iP->particleId() == reco::PFCandidate::mu) passVeto = kFALSE;
                    //************************************************************
                    //************************************************************
                    // Footprint Veto
                    if (fabs(fMVAVar_eta) > 1.479 && dr < 0.015) passVeto = kFALSE;
                    //************************************************************
                    if (passVeto) {
                        if (dr < 0.1) tmpChargedIso_DR0p0To0p1 += iP->pt();
                        if (dr >= 0.1 && dr < 0.2) tmpChargedIso_DR0p1To0p2 += iP->pt();
                        if (dr >= 0.2 && dr < 0.3) tmpChargedIso_DR0p2To0p3 += iP->pt();
                        if (dr >= 0.3 && dr < 0.4) tmpChargedIso_DR0p3To0p4 += iP->pt();
                        if (dr >= 0.4 && dr < 0.5) tmpChargedIso_DR0p4To0p5 += iP->pt();
                    } //pass veto	   
                }
                //Gamma
                else if (iP->particleId() == reco::PFCandidate::gamma) {
                    //************************************************************
                    // Footprint Veto
                    if (fabs(fMVAVar_eta) > 1.479 && dr < 0.08) passVeto = kFALSE;
                    //************************************************************	
                    if (passVeto) {
                        if (dr < 0.1) tmpGammaIso_DR0p0To0p1 += iP->pt();
                        if (dr >= 0.1 && dr < 0.2) tmpGammaIso_DR0p1To0p2 += iP->pt();
                        if (dr >= 0.2 && dr < 0.3) tmpGammaIso_DR0p2To0p3 += iP->pt();
                        if (dr >= 0.3 && dr < 0.4) tmpGammaIso_DR0p3To0p4 += iP->pt();
                        if (dr >= 0.4 && dr < 0.5) tmpGammaIso_DR0p4To0p5 += iP->pt();
                    }
                }
                //NeutralHadron
                else {
                    if (dr < 0.1) tmpNeutralHadronIso_DR0p0To0p1 += iP->pt();
                    if (dr >= 0.1 && dr < 0.2) tmpNeutralHadronIso_DR0p1To0p2 += iP->pt();
                    if (dr >= 0.2 && dr < 0.3) tmpNeutralHadronIso_DR0p2To0p3 += iP->pt();
                    if (dr >= 0.3 && dr < 0.4) tmpNeutralHadronIso_DR0p3To0p4 += iP->pt();
                    if (dr >= 0.4 && dr < 0.5) tmpNeutralHadronIso_DR0p4To0p5 += iP->pt();
                }
            } //not lepton footprint
        } //in 1.0 dr cone
    } //loop over PF candidates
    T_Elec_ChargedIso_DR0p0To0p1->push_back(TMath::Min((tmpChargedIso_DR0p0To0p1)/ele.pt(), 2.5));
    T_Elec_ChargedIso_DR0p1To0p2->push_back(TMath::Min((tmpChargedIso_DR0p1To0p2)/ele.pt(), 2.5));
    T_Elec_ChargedIso_DR0p2To0p3->push_back(TMath::Min((tmpChargedIso_DR0p2To0p3)/ele.pt(), 2.5));
    T_Elec_ChargedIso_DR0p3To0p4->push_back(TMath::Min((tmpChargedIso_DR0p3To0p4)/ele.pt(), 2.5));
    T_Elec_ChargedIso_DR0p4To0p5->push_back(TMath::Min((tmpChargedIso_DR0p4To0p5)/ele.pt(), 2.5));
    
    T_Elec_GammaIso_DR0p0To0p1->push_back(TMath::Min((tmpGammaIso_DR0p0To0p1)/ele.pt(), 2.5));
    T_Elec_GammaIso_DR0p1To0p2->push_back(TMath::Min((tmpGammaIso_DR0p1To0p2)/ele.pt(), 2.5));
    T_Elec_GammaIso_DR0p2To0p3->push_back(TMath::Min((tmpGammaIso_DR0p2To0p3)/ele.pt(), 2.5));
    T_Elec_GammaIso_DR0p3To0p4->push_back(TMath::Min((tmpGammaIso_DR0p3To0p4)/ele.pt(), 2.5));
    T_Elec_GammaIso_DR0p4To0p5->push_back(TMath::Min((tmpGammaIso_DR0p4To0p5)/ele.pt(), 2.5));
    
    T_Elec_NeutralHadronIso_DR0p0To0p1->push_back(TMath::Min((tmpNeutralHadronIso_DR0p0To0p1)/ele.pt(), 2.5));
    T_Elec_NeutralHadronIso_DR0p1To0p2->push_back(TMath::Min((tmpNeutralHadronIso_DR0p1To0p2)/ele.pt(), 2.5));
    T_Elec_NeutralHadronIso_DR0p2To0p3->push_back(TMath::Min((tmpNeutralHadronIso_DR0p2To0p3)/ele.pt(), 2.5));
    T_Elec_NeutralHadronIso_DR0p3To0p4->push_back(TMath::Min((tmpNeutralHadronIso_DR0p3To0p4)/ele.pt(), 2.5));
    T_Elec_NeutralHadronIso_DR0p4To0p5->push_back(TMath::Min((tmpNeutralHadronIso_DR0p4To0p5)/ele.pt(), 2.5));

}

bool
ElecIdAnalyzer::passMVAcuts(const reco::GsfElectron&  electron, double theMVAvalue)
{
    bool theResult = false;
    float pt = electron.pt();
    float abseta = fabs(electron.eta());
    
    if (pt <= 20){
        if ((abseta>=0)&&(abseta<0.8)) theResult = (theMVAvalue > 0.0);
        else if ((abseta>=0.8)&&(abseta<1.479)) theResult = (theMVAvalue > 0.1);
        else if ((abseta>=1.479)&&(abseta<2.5)) theResult = (theMVAvalue > 0.62);
    }
    else {
        if ((abseta>=0)&&(abseta<0.8)) theResult = (theMVAvalue > 0.94);
        else if ((abseta>=0.8)&&(abseta<1.479)) theResult = (theMVAvalue > 0.85);
        else if ((abseta>=1.479)&&(abseta<2.5)) theResult = (theMVAvalue > 0.92); 

    }
    return theResult;
}

bool 
ElecIdAnalyzer::passFOcuts(const reco::GsfElectron &electron,const reco::Vertex& vertex, bool conv)
{
    float pt = electron.pt();
    float d0 = fabs(electron.gsfTrack()->dxy(vertex.position()));
    float dz = fabs(electron.gsfTrack()->dz(vertex.position()));
    if (electron.dr03TkSumPt()/pt          > 0.2)  return false;
    if (electron.dr03HcalTowerSumEt()/pt   > 0.2)  return false;
    if (d0 > 0.02)                                  return false;
    if (dz > 0.1)                                   return false;
    
    unsigned int mhits = electron.gsfTrack()->trackerExpectedHitsInner().numberOfHits();
    if (mhits > 0)      return false;
    if (conv)           return false;
    
    if (electron.isEB()) {
        if (electron.sigmaIetaIeta()                               > 0.01)  return false;
        if (fabs(electron.deltaEtaSuperClusterTrackAtVtx())        > 0.007) return false;
        if (fabs(electron.deltaPhiSuperClusterTrackAtVtx())        > 0.15)  return false;
        if (electron.hadronicOverEm()                              > 0.12)  return false;
        if ((electron.dr03EcalRecHitSumEt() - 1.0)/pt > 0.2)   return false;
    } else {
        if (electron.sigmaIetaIeta()                               > 0.03)  return false;
        if (fabs(electron.deltaEtaSuperClusterTrackAtVtx())        > 0.009) return false;
        if (fabs(electron.deltaPhiSuperClusterTrackAtVtx())        > 0.10)  return false;
        if (electron.hadronicOverEm()                              > 0.10)  return false;
        if (electron.dr03EcalRecHitSumEt()/pt                      > 0.2)   return false;
    }
    
    return true;
    
}

int
ElecIdAnalyzer::PFisCommingFromVertex(const reco::PFCandidate &thePF, const reco::VertexCollection& vertices){
    reco::TrackBaseRef trackBaseRef( thePF.trackRef() );
    
    size_t  iVertex = 0;
    unsigned index=0;
    unsigned nFoundVertex = 0;
    typedef reco::VertexCollection::const_iterator IV;
    typedef reco::Vertex::trackRef_iterator IT;
    float bestweight=0;
    for(IV iv=vertices.begin(); iv!=vertices.end(); ++iv, ++index) {
        
        const reco::Vertex& vtx = *iv;
        
        // loop on tracks in vertices
        for(IT iTrack=vtx.tracks_begin();
            iTrack!=vtx.tracks_end(); ++iTrack) {
            
            const reco::TrackBaseRef& baseRef = *iTrack;
            
            // one of the tracks in the vertex is the same as
            // the track considered in the function
            if(baseRef == trackBaseRef ) {
                float w = vtx.trackWeight(baseRef);
                //select the vertex for which the track has the highest weight
                if (w > bestweight){
                    bestweight=w;
                    iVertex=index;
                    nFoundVertex++;
                }
            }
        }
    }
    
    if (nFoundVertex>0){
        if (nFoundVertex!=1)
            cout << "TrackOnTwoVertex" <<"a track is shared by at least two verteces. Used to be an assert" << endl;
        return iVertex;
    }
    // no vertex found with this track.
    double dzmin = 10000;
    double ztrack = thePF.vertex().z();
    bool foundVertex = false;
    index = 0;
    for(IV iv=vertices.begin(); iv!=vertices.end(); ++iv, ++index) {
        
        double dz = fabs(ztrack - iv->z());
        if(dz<dzmin) {
            dzmin = dz;
            iVertex = index;
            foundVertex = true;
        }
    }
    
    if( foundVertex ) 
        return iVertex;
    return -1;
}


double
ElecIdAnalyzer::PFisolationMVA(const reco::Muon* muon,  const reco::PFCandidateCollection &inPfCands, const reco::Vertex *pv, float theRho){
    Double_t tmpChargedIso_DR0p0To0p1  = 0;
    Double_t tmpChargedIso_DR0p1To0p2  = 0;
    Double_t tmpChargedIso_DR0p2To0p3  = 0;
    Double_t tmpChargedIso_DR0p3To0p4  = 0;
    Double_t tmpChargedIso_DR0p4To0p5  = 0;
    Double_t tmpGammaIso_DR0p0To0p1  = 0;
    Double_t tmpGammaIso_DR0p1To0p2  = 0;
    Double_t tmpGammaIso_DR0p2To0p3  = 0;
    Double_t tmpGammaIso_DR0p3To0p4  = 0;
    Double_t tmpGammaIso_DR0p4To0p5  = 0;
    Double_t tmpNeutralHadronIso_DR0p0To0p1  = 0;
    Double_t tmpNeutralHadronIso_DR0p1To0p2  = 0;
    Double_t tmpNeutralHadronIso_DR0p2To0p3  = 0;
    Double_t tmpNeutralHadronIso_DR0p3To0p4  = 0;
    Double_t tmpNeutralHadronIso_DR0p4To0p5  = 0;
    
    float etaMuon = muon->eta();
    float ptMuon = muon->pt();
    float isGlobalMuon = muon->isGlobalMuon();
    float isTrackerMuon = muon->isTrackerMuon();
    
    
    double muonTrackZ = 0;
    if (muon->muonBestTrack().isNonnull()) {
        muonTrackZ = muon->muonBestTrack()->dz(pv->position());
    }
   // cout << "le Z de la trackMuon " << muonTrackZ << endl;
   // cout << "on va tourner sur les PF particles" << endl;
    for (reco::PFCandidateCollection::const_iterator iP = inPfCands.begin();
         iP != inPfCands.end(); ++iP) {
        double dr = sqrt(pow(iP->eta() - muon->eta(),2) + pow(acos(cos(iP->phi() - muon->phi())),2));
        Bool_t passVeto = kTRUE;
        //Charged
        if(iP->trackRef().isNonnull()) {
            if (!(fabs(iP->trackRef()->dz(pv->position()) - muonTrackZ) < 0.2)) passVeto = kFALSE;
            //************************************************************
            // Veto any PFmuon, or PFEle
            if (iP->particleId() == reco::PFCandidate::e || iP->particleId() == reco::PFCandidate::mu) passVeto = kFALSE;
            //************************************************************
            //************************************************************
            // Footprint Veto
            if (fabs(muon->muonBestTrack()->eta()) > 1.479 && dr < 0.01) passVeto = kFALSE;
            //************************************************************
            if (passVeto) {
                if (dr < 0.1) tmpChargedIso_DR0p0To0p1 += iP->pt();
                if (dr >= 0.1 && dr < 0.2) tmpChargedIso_DR0p1To0p2 += iP->pt();
                if (dr >= 0.2 && dr < 0.3) tmpChargedIso_DR0p2To0p3 += iP->pt();
                if (dr >= 0.3 && dr < 0.4) tmpChargedIso_DR0p3To0p4 += iP->pt();
                if (dr >= 0.4 && dr < 0.5) tmpChargedIso_DR0p4To0p5 += iP->pt();
            } //pass veto
        }
        //Gamma
        else if (iP->particleId() == reco::PFCandidate::gamma) {
            if (dr < 0.1) tmpGammaIso_DR0p0To0p1 += iP->pt();
            if (dr >= 0.1 && dr < 0.2) tmpGammaIso_DR0p1To0p2 += iP->pt();
            if (dr >= 0.2 && dr < 0.3) tmpGammaIso_DR0p2To0p3 += iP->pt();
            if (dr >= 0.3 && dr < 0.4) tmpGammaIso_DR0p3To0p4 += iP->pt();
            if (dr >= 0.4 && dr < 0.5) tmpGammaIso_DR0p4To0p5 += iP->pt();
        }
        //NeutralHadron
        else {
            if (dr < 0.1) tmpNeutralHadronIso_DR0p0To0p1 += iP->pt();
            if (dr >= 0.1 && dr < 0.2) tmpNeutralHadronIso_DR0p1To0p2 += iP->pt();
            if (dr >= 0.2 && dr < 0.3) tmpNeutralHadronIso_DR0p2To0p3 += iP->pt();
            if (dr >= 0.3 && dr < 0.4) tmpNeutralHadronIso_DR0p3To0p4 += iP->pt();
            if (dr >= 0.4 && dr < 0.5) tmpNeutralHadronIso_DR0p4To0p5 += iP->pt();
        }
        
        
    }
   float mvaIso = muMVANonTrig->mvaValue_Iso(ptMuon, etaMuon, isGlobalMuon, isTrackerMuon, theRho, effAreaTarget,
                                              tmpChargedIso_DR0p0To0p1,
                                              tmpChargedIso_DR0p1To0p2,
                                              tmpChargedIso_DR0p2To0p3,
                                              tmpChargedIso_DR0p3To0p4,
                                              tmpChargedIso_DR0p4To0p5,
                                              tmpGammaIso_DR0p0To0p1,
                                              tmpGammaIso_DR0p1To0p2,
                                              tmpGammaIso_DR0p2To0p3,
                                              tmpGammaIso_DR0p3To0p4,
                                              tmpGammaIso_DR0p4To0p5,
                                              tmpNeutralHadronIso_DR0p0To0p1,
                                              tmpNeutralHadronIso_DR0p1To0p2,
                                              tmpNeutralHadronIso_DR0p2To0p3,
                                              tmpNeutralHadronIso_DR0p3To0p4,
                                              tmpNeutralHadronIso_DR0p4To0p5,
                                              false);
    //cout << "the value of MVAiso=" << mvaIso << endl;
    
    return mvaIso;
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
ElecIdAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}



//define this as a plug-in
DEFINE_FWK_MODULE(ElecIdAnalyzer);
