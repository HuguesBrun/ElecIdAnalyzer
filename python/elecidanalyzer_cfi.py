import FWCore.ParameterSet.Config as cms

savePatInTree=True


process = cms.Process("EX")
process.load("Configuration.StandardSequences.Services_cff")
process.load("Configuration.Geometry.GeometryIdeal_cff")
process.load('Configuration/StandardSequences/MagneticField_38T_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.load("Configuration.StandardSequences.Reconstruction_cff")

# try to add the PAT PF sequences in the analyser...
## import skeleton process

# load the PAT config
if savePatInTree:
    from PhysicsTools.PatAlgos.patTemplate_cfg import *
    process.load("PhysicsTools.PatAlgos.patSequences_cff")
    from PhysicsTools.PatAlgos.tools.pfTools import *
    postfix = "PFlow"
    jetAlgo="AK5"
    usePF2PAT(process,runPF2PAT=True, jetAlgo=jetAlgo, runOnMC=True, postfix=postfix)


process.GlobalTag.globaltag = 'START53_V7A::All'
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(100))







process.MessageLogger.cerr.FwkReport.reportEvery = 10
#
# the MC global Tag : START53_V7A
# the RERECO of 2012 data (Jan22 reRECO) for run ABCD FT_53_V21_AN6
# input
#

process.source = cms.Source(
    "PoolSource",
    fileNames = cms.untracked.vstring(
#    'file:/sps/cms/hbrun/CMSSW_5_3_10_forNewSims/src/files/TTsample/MYCOPY_3_1_1rg.root'),
                                      'file:/sps/cms/hbrun/CMSSW_5_3_10_forNewSims/src/files/runDepMC/MCDY_runDep_1.root',
                                      'file:/sps/cms/hbrun/CMSSW_5_3_10_forNewSims/src/files/runDepMC/MCDY_runDep_2.root',
                                      'file:/sps/cms/hbrun/CMSSW_5_3_10_forNewSims/src/files/runDepMC/MCDY_runDep_3.root',
                                      'file:/sps/cms/hbrun/CMSSW_5_3_10_forNewSims/src/files/runDepMC/MCDY_runDep_4.root',
                                      'file:/sps/cms/hbrun/CMSSW_5_3_10_forNewSims/src/files/runDepMC/MCDY_runDep_5.root'),
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
    isMC                        = cms.bool(True),
	doMuons		                = cms.bool(True),
    doElectrons                 = cms.bool(True),
	doPhotons		            = cms.bool(False),
    doPFPATmatching             = cms.bool(False),
	savePF			            = cms.bool(False),
	saveConversions		        = cms.bool(False),
    doMuMuGammaMC           	= cms.bool(False),
    electronsInputTag       	= cms.InputTag("gsfElectrons"),
    conversionsInputTag     	= cms.InputTag("allConversions"),
    beamSpotInputTag        	= cms.InputTag("offlineBeamSpot"),
    rhoIsoInputTag          	= cms.InputTag("kt6PFJetsForIsolation", "rho"),
    primaryVertexInputTag   	= cms.InputTag("offlinePrimaryVertices"),
	muonProducer 		= cms.VInputTag(cms.InputTag("muons")),
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
    deltaRsavePF            = cms.double(0.3),
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


#keep only events passing the single Electron path
process.load("HLTrigger.HLTfilters.triggerResultsFilter_cfi")
process.triggerResultsFilter.triggerConditions = cms.vstring('*')
process.triggerResultsFilter.l1tResults = ''
process.triggerResultsFilter.throw = False
process.triggerResultsFilter.hltResults = cms.InputTag( "TriggerResults", "", "HLT" )




if savePatInTree:
    # top projections in PF2PAT:
    getattr(process,"pfNoPileUp"+postfix).enable = True
    getattr(process,"pfNoMuon"+postfix).enable = True
    getattr(process,"pfNoElectron"+postfix).enable = True
    
    # tau considered as jet
    getattr(process,"pfNoTau"+postfix).enable = False
    getattr(process,"pfNoJet"+postfix).enable = False

    # verbose flags for the PF2PAT modules
    getattr(process,"pfNoMuon"+postfix).verbose = False

    #ask the analyzer to
    getattr(process,"theEleIdAnalyzer").doPFPATmatching = True

    #all pfMuons considered as isolated
    process.pfIsolatedMuonsPFlow.combinedIsolationCut = cms.double(9999.)
    process.pfIsolatedMuonsPFlow.isolationCut = cms.double(9999.)
    process.pfSelectedMuonsPFlow.cut = cms.string("pt>5")

    #all pfElectrons considered as isolated
    process.pfIsolatedElectronsPFlow.combinedIsolationCut = cms.double(9999.)
    process.pfIsolatedElectronsPFlow.isolationCut = cms.double(9999.)

    process.load('EGamma.EGammaAnalysisTools.electronIdMVAProducer_cfi')
    process.eidMVASequence = cms.Sequence(  process.mvaTrigV0 + process.mvaNonTrigV0 )
    #Electron ID
    process.patElectrons.electronIDSources.mvaTrigV0    = cms.InputTag("mvaTrigV0")
    process.patElectrons.electronIDSources.mvaNonTrigV0 = cms.InputTag("mvaNonTrigV0")
    process.patPF2PATSequence.replace( process.patElectronsPFlow, process.eidMVASequence * process.patElectronsPFlow )

    #Only one isoCone can be saved in patElectrons... Set to DR0.3 since it is used in both SUSY and TOP
    process.patElectronsPFlow.isolationValues.pfNeutralHadrons = cms.InputTag( 'elPFIsoValueNeutral03PFIdPFlow' )
    process.patElectronsPFlow.isolationValues.pfPhotons = cms.InputTag( 'elPFIsoValueGamma03PFIdPFlow' )
    process.patElectronsPFlow.isolationValues.pfChargedHadrons = cms.InputTag( 'elPFIsoValueCharged03PFIdPFlow' )
    process.patElectronsPFlow.isolationValues.pfPUChargedHadrons = cms.InputTag( 'elPFIsoValuePU03PFIdPFlow' )
    process.patElectronsPFlow.isolationValues.pfChargedAll = cms.InputTag("elPFIsoValueChargedAll03PFIdPFlow")


if savePatInTree:
    #sequence with PF
    process.p = cms.Path(process.primaryVertexFilter * process.noscraping * process.kt6PFJetsForIsolation * process.pfiso * getattr(process,"patPF2PATSequence"+postfix)* process.theEleIdAnalyzer)
else:
    #sequence with no PF
    process.p = cms.Path(process.primaryVertexFilter * process.noscraping * process.kt6PFJetsForIsolation * process.pfiso * process.theEleIdAnalyzer)

