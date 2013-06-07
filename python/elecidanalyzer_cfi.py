import FWCore.ParameterSet.Config as cms

process = cms.Process("EX")
process.load("Configuration.StandardSequences.Services_cff")
process.load("Configuration.Geometry.GeometryIdeal_cff")
process.load('Configuration/StandardSequences/MagneticField_38T_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.GlobalTag.globaltag = 'START53_V7A::All'
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(200))

process.MessageLogger.cerr.FwkReport.reportEvery = 1000
#
# the MC global Tag : START53_V7A
# the RERECO of 2012 data (Jan22 reRECO) for run ABC FT_53_V21_AN3
# input
#

process.source = cms.Source(
    "PoolSource",
    fileNames = cms.untracked.vstring(
	#'file:/sps/cms/hbrun/CMSSW_5_3_6_recup/src/recupRunD/fichier_dataD.root',
	#'file:/sps/cms/hbrun/CMSSW_5_3_6_recup/src/recupRunD/fichier_dataD2.root'
	#'file:/sps/cms/hbrun/CMSSW_5_3_6_recup/src/recupTrigger/diMu/theFileTrigger.root'
#	'file:/sps/cms/hbrun/CMSSW_5_3_7_myCode/src/dataFile_runA/theFile.root'
#	'file:/sps/cms/hbrun/CMSSW_5_3_2_myCode/src/files/DYJetsToLLM50.root'
	'file:/sps/cms/hbrun/CMSSW_5_3_2_myCode/src/files/mumuFiles/mumugamma_1.root'
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
    isMC                                    = cms.bool(False),
	doMuons					= cms.bool(False),

	doPhotons				= cms.bool(False),
	savePF					= cms.bool(True),
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







process.p = cms.Path(process.primaryVertexFilter * process.noscraping * process.kt6PFJetsForIsolation * process.pfiso * process.theEleIdAnalyzer)
