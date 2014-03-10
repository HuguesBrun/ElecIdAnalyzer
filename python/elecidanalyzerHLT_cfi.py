
import FWCore.ParameterSet.Config as cms


saveRECOoutput = False

process = cms.Process('myHLT')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.Geometry.GeometryExtended2015Reco_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_PostLS1_cff')
process.load('Configuration.StandardSequences.Digi_cff')
process.load('Configuration.StandardSequences.SimL1Emulator_cff')
process.load('Configuration.StandardSequences.DigiToRaw_cff')
process.load('HLTrigger.Configuration.HLT_GRun_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')



# to compute FastJet rho to correct isolation (note: EtaMax restricted to 2.5)
# all the analyses
from RecoJets.JetProducers.kt4PFJets_cfi import *
process.kt6PFJetsForIsolation = kt4PFJets.clone( rParam = 0.6, doRhoFastjet = True )
process.kt6PFJetsForIsolation.Rho_EtaMax = cms.double(2.5)


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
                                          TriggerResults          = cms.InputTag("TriggerResults", "", "myHLT"),
                                          HLTTriggerSummaryAOD    = cms.InputTag("hltTriggerSummaryAOD", "", "myHLT"),
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

process.MessageLogger.cerr.FwkReport.reportEvery = 1

process.maxEvents = cms.untracked.PSet(
                                       input = cms.untracked.int32(20)
                                       )



process.source = cms.Source( "PoolSource",
                            fileNames = cms.untracked.vstring(
                                                              'file:/sps/cms/hbrun/CMSSW_6_2_5_triggerStudies/src/file/fileReco1.root',
                                                              ),
                            secondaryFileNames = cms.untracked.vstring(
                                                                       'file:/sps/cms/hbrun/CMSSW_6_2_5_triggerStudies/src/file/fileRAW1.root',
                                                                       'file:/sps/cms/hbrun/CMSSW_6_2_5_triggerStudies/src/file/fileRAW2.root',
                                                                       
                                                                       ),
                            inputCommands = cms.untracked.vstring(
                                                                  'keep *'
                                                                  )
                            )



process.configurationMetadata = cms.untracked.PSet(
                                                   version = cms.untracked.string('$Revision: 1.20 $'),
                                                   annotation = cms.untracked.string('step2 nevts:10'),
                                                   name = cms.untracked.string('Applications')
                                                   )

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'POSTLS162_V2::All', '')

process.RECOSIMoutput = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = cms.untracked.vstring(
                                                                      'drop *',
                                                                      'keep *_standAloneMuons_*_RECO',
                                                                      'keep *_dt4DSegments_*_RECO',
                                                                      'keep *_cscSegments_*_RECO',
                                                                      'keep *_*_*_myHLT',	    
                                                                      'keep *_*_*_ALZ'),	    
    fileName = cms.untracked.string('file:/tmp/hbrun/MUO-Fall13dr-00013.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('GEN-SIM-RECO')
    )
)

process.p = cms.Path(process.primaryVertexFilter + process.noscraping + process.kt6PFJetsForIsolation)
process.pAnalyzer = cms.EndPath( process.theEleIdAnalyzer)
process.pInSchedule = cms.Schedule([process.p,process.pAnalyzer])

process.schedule = cms.Schedule(process.HLTSchedule)
process.schedule.extend(process.pInSchedule)

if saveRECOoutput:
    process.endjob_step = cms.EndPath(process.endOfProcess)
    process.RECOSIMoutput_step = cms.EndPath(process.RECOSIMoutput)
    process.schedule.extend([process.endjob_step,process.RECOSIMoutput_step])

# customisation of the process.

# Automatic addition of the customisation function from HLTrigger.Configuration.customizeHLTforMC
from HLTrigger.Configuration.customizeHLTforMC import customizeHLTforMC

#call to customisation function customizeHLTforMC imported from HLTrigger.Configuration.customizeHLTforMC
process = customizeHLTforMC(process)

# Automatic addition of the customisation function from SLHCUpgradeSimulations.Configuration.postLS1Customs
from SLHCUpgradeSimulations.Configuration.postLS1Customs import customisePostLS1

#call to customisation function customisePostLS1 imported from SLHCUpgradeSimulations.Configuration.postLS1Customs
process = customisePostLS1(process)







