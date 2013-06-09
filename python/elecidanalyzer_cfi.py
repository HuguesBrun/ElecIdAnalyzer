import FWCore.ParameterSet.Config as cms

process = cms.Process("EX")
process.load("Configuration.StandardSequences.Services_cff")
process.load("Configuration.StandardSequences.Geometry_cff")
process.load('Configuration/StandardSequences/MagneticField_38T_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.GlobalTag.globaltag = 'START42_V14B::All'
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(1000))

process.MessageLogger.cerr.FwkReport.reportEvery = 100
#
# the MC global Tag : START42_V14B
# 2011 data global tag : 

process.source = cms.Source(
    "PoolSource",
    fileNames = cms.untracked.vstring(
	#'file:/sps/cms/hbrun/CMSSW_5_3_6_recup/src/recupRunD/fichier_dataD.root',
	#'file:/sps/cms/hbrun/CMSSW_5_3_6_recup/src/recupRunD/fichier_dataD2.root'
	#'file:/sps/cms/hbrun/CMSSW_5_3_6_recup/src/recupTrigger/diMu/theFileTrigger.root'
#	'file:/sps/cms/hbrun/CMSSW_5_3_7_myCode/src/dataFile_runA/theFile.root'
#	'file:/sps/cms/hbrun/CMSSW_5_3_2_myCode/src/files/DYJetsToLLM50.root'
#        'file:/sps/cms/hbrun/CMSSW_5_3_7_myCode/src/files/MC_DY_1.root'
        'file:/sps/cms/hbrun/CMSSW_5_3_7_myCode/src/files/data_DoublePhoton.root'
    ),
    secondaryFileNames = cms.untracked.vstring(),
    noEventSort = cms.untracked.bool(True),
    duplicateCheckMode = cms.untracked.string('noDuplicateCheck')
)

# to compute FastJet rho to correct isolation (note: EtaMax restricted to 2.5)
# all the analyses
from RecoJets.JetProducers.kt4PFJets_cfi import *
process.kt6PFJetsForIsolation = kt4PFJets.clone( rParam = 0.6, doRhoFastjet = True )
process.kt6PFJetsForIsolation.Rho_EtaMax = cms.double(2.5)

#compute the pf iso
from CommonTools.ParticleFlow.Tools.pfIsolation import setupPFElectronIso, setupPFMuonIso
process.eleIsoSequence = setupPFElectronIso(process, 'gsfElectrons')
process.pfiso = cms.Sequence(process.pfParticleSelectionSequence + process.eleIsoSequence)

process.theDiElecFilter = cms.EDFilter('DiElecFilter',
     electronsInputTag       = cms.InputTag("gsfElectrons")                                       
)
                                    

process.theEleIdAnalyzer = cms.EDAnalyzer('ElecIdAnalyzer',
        isMC                    = cms.bool(False),
	doMuons					= cms.bool(False),
        doElectrons             = cms.bool(True),
	doPhotons				= cms.bool(True),
	savePF					= cms.bool(False),
	saveConversions			= cms.bool(False),
    doMuMuGammaMC           = cms.bool(False),
    electronsInputTag       = cms.InputTag("gsfElectrons"),
    conversionsInputTag     = cms.InputTag("allConversions"),
    beamSpotInputTag        = cms.InputTag("offlineBeamSpot"),
    rhoIsoInputTag          = cms.InputTag("kt6PFJetsForIsolation", "rho"),
    primaryVertexInputTag   = cms.InputTag("offlinePrimaryVertices"),
	muonProducer = cms.VInputTag(cms.InputTag("muons")),
	isoValInputTags         = cms.VInputTag(cms.InputTag('elPFIsoValueCharged03PFIdPFIso'),
											cms.InputTag('elPFIsoValueGamma03PFIdPFIso'),
											cms.InputTag('elPFIsoValueNeutral03PFIdPFIso'),
											cms.InputTag('elPFIsoValueChargedAll03PFIdPFIso'),
											cms.InputTag('elPFIsoValuePU03PFIdPFIso'),
											cms.InputTag('elPFIsoValueCharged04PFIdPFIso'),
											cms.InputTag('elPFIsoValueGamma04PFIdPFIso'),
											cms.InputTag('elPFIsoValueNeutral04PFIdPFIso'),	
											cms.InputTag('elPFIsoValueChargedAll04PFIdPFIso'),
											cms.InputTag('elPFIsoValuePU04PFIdPFIso')),
    TriggerResults          = cms.InputTag("TriggerResults", "", "HLT"),
    HLTTriggerSummaryAOD    = cms.InputTag("hltTriggerSummaryAOD", "", "HLT"),
	photonCollection		= cms.string("photons"),
    outputFile		        = cms.string("elecIDtree.root"),
    deltaRsavePF            = cms.double(0.6),
    printDebug              = cms.bool(True)
)

process.primaryVertexFilter = cms.EDFilter("GoodVertexFilter",
                                           vertexCollection = cms.InputTag('offlinePrimaryVertices'),
                                           minimumNDOF = cms.uint32(4) ,
                                           maxAbsZ = cms.double(24),
                                           maxd0 = cms.double(2)	
                                           )


process.noscraping = cms.EDFilter("FilterOutScraping",
                                  applyfilter = cms.untracked.bool(True),
                                  debugOn = cms.untracked.bool(False),
                                  numtrack = cms.untracked.uint32(10),
                                  thresh = cms.untracked.double(0.25)
                                  )


process.load("HLTrigger.HLTfilters.triggerResultsFilter_cfi")
process.triggerResultsFilter.triggerConditions = cms.vstring( 'HLT_Ele17_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT_SC8_Mass30_v*')
process.triggerResultsFilter.l1tResults = ''
process.triggerResultsFilter.throw = False
process.triggerResultsFilter.hltResults = cms.InputTag( "TriggerResults", "", "HLT" )




process.p = cms.Path(process.triggerResultsFilter* process.primaryVertexFilter * process.noscraping * process.kt6PFJetsForIsolation * process.pfiso * process.theEleIdAnalyzer)
