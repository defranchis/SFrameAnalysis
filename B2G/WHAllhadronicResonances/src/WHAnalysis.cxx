// $Id: CycleCreators.py 344 2012-12-13 13:10:53Z krasznaa $

// Local include(s):
#include "../include/WHAnalysis.h"

// External include(s):
#include "../../GoodRunsLists/include/TGoodRunsListReader.h"

#include <TMath.h>

ClassImp( WHAnalysis );

// define cut names

const std::string WHAnalysis::kCutName[ WHAnalysis::kNumCuts ] = {
  "BeforeCuts",            // C0
  "JSON",                  // C1
  "Trigger",               // C2
  "MetFilters",            // C3
  "TwoFatJets",            // C4
  "FatJetsDeltaEta",       // C5
  "DijetMass",             // C6
  "VWindow",               // C7
  "HiggsWindow",           // C8
  "Tau21",                 // C9
  "SubjetSingleTag",       // C10
  "SubjetDoubleTag"        // C11
};

WHAnalysis::WHAnalysis()
   : SCycleBase()
   , m_jetAK4( this )
   , m_jetAK8( this )
   , m_eventInfo( this )
   , m_electron( this )
   , m_muon( this )
   , m_missingEt( this )
   , m_genParticle( this )
   , m_pileupReweightingTool( this )
{

   m_logger << INFO << "Hello!" << SLogger::endmsg;
   SetLogName( GetName() );
   
   // read configuration details from XML file
   // (defaults agree with July 2010 acceptance challenge)
   DeclareProperty( "RecoTreeName",             m_recoTreeName             = "physics" );
   DeclareProperty( "OutputTreeName",           m_outputTreeName           = "analysis" );
   DeclareProperty( "NtupleLevel",              m_ntupleLevel              = kTwoFatJets );
   DeclareProperty( "JetAK4Name",               m_jetAK4Name               = "jetAK4" );
   DeclareProperty( "JetAK8Name",               m_jetAK8Name               = "jetAK8" );
   DeclareProperty( "ElectronName",             m_electronName             = "el" );
   DeclareProperty( "MuonName",                 m_muonName                 = "mu" );
   DeclareProperty( "MissingEtName",            m_missingEtName            = "MET" );
   DeclareProperty( "GenParticleName",          m_genParticleName          = "genParticle" );
   
   DeclareProperty( "IsData",                   m_isData                   = false );
   DeclareProperty( "IsSignal",                 m_isSignal                 = true );
   DeclareProperty( "ApplyMETFilters",          m_applyMETFilters          = true );
   
   DeclareProperty( "JetPtCut",                 m_jetPtCut           = 200. );
   DeclareProperty( "JetEtaCut",                m_jetEtaCut          = 2.4  );
   DeclareProperty( "MjjCut",                   m_mjjCut             = 1000 );
   DeclareProperty( "JetDeltaEtaCut",           m_jetDeltaEtaCut     = 1.3  );

   DeclareProperty( "Tau21HPCut",                m_tau21HPCut             = 0.45 );
   DeclareProperty( "Tau21LPCut",                m_tau21LPCut             = 0.75 );
   DeclareProperty( "MVLowSidebandCut",          m_mVLowSidebandCut       = 40 );
   DeclareProperty( "MWLowerCut",                m_mWLowerCut             = 65 );
   DeclareProperty( "MWUpperCut",                m_mWUpperCut             = 85 );
   DeclareProperty( "MZLowerCut",                m_mZLowerCut             = 85 );
   DeclareProperty( "MZUpperCut",                m_mZUpperCut             = 105 );
   DeclareProperty( "MHLowerCut",                m_mHLowerCut             = 105 );
   DeclareProperty( "MHUpperCut",                m_mHUpperCut             = 135 );
   
   DeclareProperty( "CSVMin",                m_csvMin             = 0.605 );
   
   DeclareProperty( "ElectronPtCut",                 m_electronPtCut              = 200. );
   DeclareProperty( "ElectronEtaCut",                m_electronEtaCut             = 2.4 );
   
   DeclareProperty( "MuonPtCut",                 m_muonPtCut              = 200. );
   DeclareProperty( "MuonEtaCut",                m_muonEtaCut             = 2.4 );
   
   DeclareProperty( "METCut",                m_metCut             = 2.4 );
   
   DeclareProperty( "JSONName",                 m_jsonName             = std::string (std::getenv("SFRAME_DIR")) + "/../GoodRunsLists/JSON/Cert_246908-260627_13TeV_PromptReco_Collisions15_25ns_JSON_Silver_v2.txt" );
   
   
}

WHAnalysis::~WHAnalysis() {

  m_logger << INFO << "Tschoe!" << SLogger::endmsg;

}

void WHAnalysis::BeginCycle() throw( SError ) {

  m_logger << INFO << "Hello to you!" << SLogger::endmsg;
  
  //
  // Load the GRL:
  //
  if (m_isData) {
    m_logger << INFO << "Loading GoodRunsList from file: " << m_jsonName << SLogger::endmsg;
    Root::TGoodRunsListReader reader( TString( m_jsonName.c_str() ) );
    if( ! reader.Interpret() ) {
      m_logger << FATAL << "Couldn't read in the GRL!" << SLogger::endmsg;
      throw SError( ( "Couldn't read in file: " + m_jsonName ).c_str(), SError::SkipCycle );
    }
    m_grl = reader.GetMergedGoodRunsList();
    m_grl.Summary();
    m_grl.SetName( "MyGoodRunsList" );
  
    //
    // Add it as a configuration object, so that the worker nodes will receive it:
    //
    AddConfigObject( &m_grl );
		
    // m_logger << INFO << "Loading RunEventFilter from file: " << m_runEventFilterName << SLogger::endmsg;
    // m_runEventFilterReader.SetTextFile( TString( m_runEventFilterName.c_str() ) );
    // if( ! m_runEventFilterReader.Interpret() ) {
    //   m_logger << FATAL << "Couldn't read in the RunEventFilter file!" << SLogger::endmsg;
    //   throw SError( ( "Couldn't read in file: " + m_runEventFilterName ).c_str(), SError::SkipCycle );
    // }
		
  }
  
  m_triggerNames.clear();
	
  //Dijet triggers
  m_triggerNames.push_back("AK8PFJet360_TrimMass30") ;
  m_triggerNames.push_back("AK8PFHT700_TrimR0p1PT0p03Mass50") ;
  // trignames.push_back("AK8DiPFJet280_200_TrimMass30_BTagCSV0p45") ;
  m_triggerNames.push_back("PFHT650_WideJetMJJ950DEtaJJ1p5") ;
  m_triggerNames.push_back("PFHT650_WideJetMJJ900DEtaJJ1p5") ;
  m_triggerNames.push_back("PFHT800_v") ;
  
  // set names for various selections
  m_catNames.clear();
  m_catNames.push_back("VWindow_NoTau21_SubjetPreTag");
  m_catNames.push_back("VWindow_NoTau21_SubjetNoTag");
  m_catNames.push_back("VWindow_NoTau21_SubjetSingleTagIncl");
  m_catNames.push_back("VWindow_NoTau21_SubjetSingleTagExcl");
  m_catNames.push_back("VWindow_NoTau21_SubjetDoubleTag");
  
  m_catNames.push_back("VWindow_Tau21LP_SubjetPreTag");
  m_catNames.push_back("VWindow_Tau21LP_SubjetNoTag");
  m_catNames.push_back("VWindow_Tau21LP_SubjetSingleTagIncl");
  m_catNames.push_back("VWindow_Tau21LP_SubjetSingleTagExcl");
  m_catNames.push_back("VWindow_Tau21LP_SubjetDoubleTag");
  m_catNames.push_back("WWindow_Tau21LP_SubjetPreTag");
  m_catNames.push_back("WWindow_Tau21LP_SubjetNoTag");
  m_catNames.push_back("WWindow_Tau21LP_SubjetSingleTagIncl");
  m_catNames.push_back("WWindow_Tau21LP_SubjetSingleTagExcl");
  m_catNames.push_back("WWindow_Tau21LP_SubjetDoubleTag");
  m_catNames.push_back("ZWindow_Tau21LP_SubjetPreTag");
  m_catNames.push_back("ZWindow_Tau21LP_SubjetNoTag");
  m_catNames.push_back("ZWindow_Tau21LP_SubjetSingleTagIncl");
  m_catNames.push_back("ZWindow_Tau21LP_SubjetSingleTagExcl");
  m_catNames.push_back("ZWindow_Tau21LP_SubjetDoubleTag");
  
  m_catNames.push_back("VWindow_Tau21HP_SubjetPreTag");
  m_catNames.push_back("VWindow_Tau21HP_SubjetNoTag");
  m_catNames.push_back("VWindow_Tau21HP_SubjetSingleTagIncl");
  m_catNames.push_back("VWindow_Tau21HP_SubjetSingleTagExcl");
  m_catNames.push_back("VWindow_Tau21HP_SubjetDoubleTag");
  m_catNames.push_back("WWindow_Tau21HP_SubjetPreTag");
  m_catNames.push_back("WWindow_Tau21HP_SubjetNoTag");
  m_catNames.push_back("WWindow_Tau21HP_SubjetSingleTagIncl");
  m_catNames.push_back("WWindow_Tau21HP_SubjetSingleTagExcl");
  m_catNames.push_back("WWindow_Tau21HP_SubjetDoubleTag");
  m_catNames.push_back("ZWindow_Tau21HP_SubjetPreTag");
  m_catNames.push_back("ZWindow_Tau21HP_SubjetNoTag");
  m_catNames.push_back("ZWindow_Tau21HP_SubjetSingleTagIncl");
  m_catNames.push_back("ZWindow_Tau21HP_SubjetSingleTagExcl");
  m_catNames.push_back("ZWindow_Tau21HP_SubjetDoubleTag");
  
  // low sideband for vector boson
  m_catNames.push_back("VLowSB_NoTau21_SubjetPreTag");
  m_catNames.push_back("VLowSB_NoTau21_SubjetNoTag");
  m_catNames.push_back("VLowSB_NoTau21_SubjetSingleTagIncl");
  m_catNames.push_back("VLowSB_NoTau21_SubjetSingleTagExcl");
  m_catNames.push_back("VLowSB_NoTau21_SubjetDoubleTag");
  
  m_catNames.push_back("VLowSB_Tau21LP_SubjetPreTag");
  m_catNames.push_back("VLowSB_Tau21LP_SubjetNoTag");
  m_catNames.push_back("VLowSB_Tau21LP_SubjetSingleTagIncl");
  m_catNames.push_back("VLowSB_Tau21LP_SubjetSingleTagExcl");
  m_catNames.push_back("VLowSB_Tau21LP_SubjetDoubleTag");
  
  m_catNames.push_back("VLowSB_Tau21HP_SubjetPreTag");
  m_catNames.push_back("VLowSB_Tau21HP_SubjetNoTag");
  m_catNames.push_back("VLowSB_Tau21HP_SubjetSingleTagIncl");
  m_catNames.push_back("VLowSB_Tau21HP_SubjetSingleTagExcl");
  m_catNames.push_back("VLowSB_Tau21HP_SubjetDoubleTag");
  
  // low sideband for Higgs boson
  m_catNames.push_back("VWindow_HLowSB_NoTau21_SubjetPreTag");
  m_catNames.push_back("VWindow_HLowSB_NoTau21_SubjetNoTag");
  m_catNames.push_back("VWindow_HLowSB_NoTau21_SubjetSingleTagIncl");
  m_catNames.push_back("VWindow_HLowSB_NoTau21_SubjetSingleTagExcl");
  m_catNames.push_back("VWindow_HLowSB_NoTau21_SubjetDoubleTag");
  
  m_catNames.push_back("WWindow_HLowSB_Tau21LP_SubjetPreTag");
  m_catNames.push_back("WWindow_HLowSB_Tau21LP_SubjetNoTag");
  m_catNames.push_back("WWindow_HLowSB_Tau21LP_SubjetSingleTagIncl");
  m_catNames.push_back("WWindow_HLowSB_Tau21LP_SubjetSingleTagExcl");
  m_catNames.push_back("WWindow_HLowSB_Tau21LP_SubjetDoubleTag");
  
  m_catNames.push_back("ZWindow_HLowSB_Tau21HP_SubjetPreTag");
  m_catNames.push_back("ZWindow_HLowSB_Tau21HP_SubjetNoTag");
  m_catNames.push_back("ZWindow_HLowSB_Tau21HP_SubjetSingleTagIncl");
  m_catNames.push_back("ZWindow_HLowSB_Tau21HP_SubjetSingleTagExcl");
  m_catNames.push_back("ZWindow_HLowSB_Tau21HP_SubjetDoubleTag");
  
   return;

}

void WHAnalysis::EndCycle() throw( SError ) {

   return;

}

void WHAnalysis::BeginInputData( const SInputData& id ) throw( SError ) {

  m_logger << INFO << "RecoTreeName:         " <<             m_recoTreeName << SLogger::endmsg;
  m_logger << INFO << "OutputTreeName:       " <<             m_outputTreeName << SLogger::endmsg;
  m_logger << INFO << "NtupleLevel:          " <<             m_ntupleLevel << SLogger::endmsg;
  m_logger << INFO << "JetAK4Name:           " <<             m_jetAK4Name << SLogger::endmsg;
  m_logger << INFO << "JetAK8Name:           " <<             m_jetAK8Name << SLogger::endmsg;
  m_logger << INFO << "ElectronName:         " <<             m_electronName << SLogger::endmsg;
  m_logger << INFO << "MuonName:             " <<             m_muonName << SLogger::endmsg;
  m_logger << INFO << "GenParticleName:      " <<             m_genParticleName << SLogger::endmsg;
  
  m_logger << INFO << "IsData:           " <<                   (m_isData ? "TRUE" : "FALSE") << SLogger::endmsg;
  m_logger << INFO << "IsSignal:           " <<                 (m_isSignal ? "TRUE" : "FALSE") << SLogger::endmsg;
  m_logger << INFO << "ApplyMETFilters:           " <<          (m_applyMETFilters ? "TRUE" : "FALSE") << SLogger::endmsg;
  
  m_logger << INFO << "JetPtCut:           " <<                 m_jetPtCut << SLogger::endmsg;
  m_logger << INFO << "JetEtaCut:           " <<                m_jetEtaCut << SLogger::endmsg;
  m_logger << INFO << "MjjCut:           " <<                m_mjjCut << SLogger::endmsg;
  m_logger << INFO << "JetDeltaEtaCut:           " <<                m_jetDeltaEtaCut << SLogger::endmsg;

  m_logger << INFO << "Tau21HPCut:           " <<                m_tau21HPCut << SLogger::endmsg;
  m_logger << INFO << "Tau21LPCut:           " <<                m_tau21LPCut << SLogger::endmsg;
  
  m_logger << INFO << "MVLowSidebandCut:     " <<                m_mVLowSidebandCut << SLogger::endmsg;
  m_logger << INFO << "MWLowerCut:           " <<                m_mWLowerCut << SLogger::endmsg;
  m_logger << INFO << "MWUpperCut:           " <<                m_mWUpperCut << SLogger::endmsg;
  m_logger << INFO << "MZLowerCut:           " <<                m_mZLowerCut << SLogger::endmsg;
  m_logger << INFO << "MZUpperCut:           " <<                m_mZUpperCut << SLogger::endmsg;
  m_logger << INFO << "MHLowerCut:           " <<                m_mHLowerCut << SLogger::endmsg;
  m_logger << INFO << "MHUpperCut:           " <<                m_mHUpperCut << SLogger::endmsg;
  
  m_logger << INFO << "CSVMin:           " <<                m_csvMin << SLogger::endmsg;
  
  m_logger << INFO << "ElectronPtCut:           " <<                 m_electronPtCut << SLogger::endmsg;
  m_logger << INFO << "ElectronEtaCut:           " <<                m_electronEtaCut << SLogger::endmsg;
  
  m_logger << INFO << "MuonPtCut:           " <<                 m_muonPtCut << SLogger::endmsg;
  m_logger << INFO << "MuonEtaCut:           " <<                m_muonEtaCut << SLogger::endmsg;
  
  m_logger << INFO << "METCut:           " <<                m_metCut << SLogger::endmsg;
  
  m_logger << INFO << "JSONName:           " <<                 m_jsonName << SLogger::endmsg;
  
  if (!m_isData) m_pileupReweightingTool.BeginInputData( id );
  
  if (m_isData) {
    TObject* grl;
    if( ! ( grl = GetConfigObject( "MyGoodRunsList" ) ) ) {
      m_logger << FATAL << "Can't access the GRL!" << SLogger::endmsg;
      throw SError( "Can't access the GRL!", SError::SkipCycle );
    }
    m_grl = *( dynamic_cast< Root::TGoodRunsList* >( grl ) );
  }
  
  //
  // output branches
  //
  DeclareVariable(b_weight              , "weight"                 );
  DeclareVariable(b_weightGen           , "weightGen"              );
  DeclareVariable(b_weightPU            , "weightPU"               );
  DeclareVariable(b_weightBtag          , "weightBtag"             );
  
  DeclareVariable( b_runNumber,           "b_runNumber"            );
  DeclareVariable( b_eventNumber,         "b_eventNumber"          );
  DeclareVariable( b_lumiBlock,           "b_lumiBlock"            );
  
  DeclareVariable( b_ak8jet0_pt,           "b_ak8jet0_pt"          );
  DeclareVariable( b_ak8jet0_phi,          "b_ak8jet0_phi"         );
  DeclareVariable( b_ak8jet0_eta,          "b_ak8jet0_eta"         );
  DeclareVariable( b_ak8jet0_e,            "b_ak8jet0_e"           );
  DeclareVariable( b_ak8jet0_tau21,        "b_ak8jet0_tau21"       );
  DeclareVariable( b_ak8jet0_m,            "b_ak8jet0_m"           );
  DeclareVariable( b_ak8jet0_mpruned,      "b_ak8jet0_mpruned"     );
  DeclareVariable( b_ak8jet0_csv,          "b_ak8jet0_csv"         );
  DeclareVariable( b_ak8jet1_pt,           "b_ak8jet1_pt"          );
  DeclareVariable( b_ak8jet1_phi,          "b_ak8jet1_phi"         );
  DeclareVariable( b_ak8jet1_eta,          "b_ak8jet1_eta"         );
  DeclareVariable( b_ak8jet1_e,            "b_ak8jet1_e"           );
  DeclareVariable( b_ak8jet1_tau21,        "b_ak8jet1_tau21"       );
  DeclareVariable( b_ak8jet1_m,            "b_ak8jet1_m"           );
  DeclareVariable( b_ak8jet1_mpruned,      "b_ak8jet1_mpruned"     );
  DeclareVariable( b_ak8jet1_csv,          "b_ak8jet1_csv"         );
  DeclareVariable( b_ak8jet0_subjet01_dr,   "b_ak8jet0_subjet01_dr"  );
  DeclareVariable( b_ak8jet0_subjet01_deta, "b_ak8jet0_subjet01_deta");
  DeclareVariable( b_ak8jet0_subjet01_dphi, "b_ak8jet0_subjet01_dphi");
  DeclareVariable( b_ak8jet0_subjet0_pt,   "b_ak8jet0_subjet0_pt"  );
  DeclareVariable( b_ak8jet0_subjet1_pt,   "b_ak8jet0_subjet1_pt"  );
  DeclareVariable( b_ak8jet0_subjet0_csv,  "b_ak8jet0_subjet0_csv" );
  DeclareVariable( b_ak8jet0_subjet1_csv,  "b_ak8jet0_subjet1_csv" );
  DeclareVariable( b_ak8jet1_subjet01_dr,   "b_ak8jet1_subjet01_dr"  );
  DeclareVariable( b_ak8jet1_subjet01_deta, "b_ak8jet1_subjet01_deta");
  DeclareVariable( b_ak8jet1_subjet01_dphi, "b_ak8jet1_subjet01_dphi");
  DeclareVariable( b_ak8jet1_subjet0_pt,   "b_ak8jet1_subjet0_pt"  );
  DeclareVariable( b_ak8jet1_subjet1_pt,   "b_ak8jet1_subjet1_pt"  );
  DeclareVariable( b_ak8jet1_subjet0_csv,  "b_ak8jet1_subjet0_csv" );
  DeclareVariable( b_ak8jet1_subjet1_csv,  "b_ak8jet1_subjet1_csv" );
  
  b_selection_bits.resize( m_catNames.size() );
  b_selection_lastcut.resize( m_catNames.size() );
  for (unsigned int s=0;s<m_catNames.size();++s) {
    DeclareVariable(b_selection_bits[s]    , Form("selection_bits_%s", m_catNames[s].c_str())    );
    DeclareVariable(b_selection_lastcut[s] , Form("selection_lastcut_%s", m_catNames[s].c_str()) );
  }
  
  //
  // Declare the output histograms:
  //
  Book( TH1F( "Events", "Events;weight", 10, -.5, 9.5 ) );
  Book( TH1F( "SumEvents", "SumEvents;weight", 10, -.5, 9.5 ) );
  Book( TH1F( "METFilters", "METFilters", 20, 0.5, 20.5 ));

  for (unsigned int s=0;s<m_catNames.size();++s) {
    TString directory = m_catNames[s].c_str();
    // cutflow
    Book( TH1F( "cutflow", "cutflow", 20, 0.5, 20.5 ), directory );  
    bookHistograms(directory);
  }
  
   return;

}

void WHAnalysis::EndInputData( const SInputData& ) throw( SError ) {

  //
  // Final analysis of cut flow
  //
  
  TString defaultCutflow = m_catNames[m_catNames.size()-1]; //"VWindow_Tau21HP_SubjetDoubleTag";
  m_logger << INFO << "cut flow for " << defaultCutflow << SLogger::endmsg;
  m_logger << INFO << Form( "Cut\t%25.25s\tEvents\tRelEff\tAbsEff", "Name" ) << SLogger::endmsg;
  
  Double_t ntot = Hist( "cutflow", defaultCutflow )->GetBinContent( 1 );
  m_logger << INFO << Form( "\t%25.25s\t%6.0f", "start", ntot ) << SLogger::endmsg;
  for( Int_t ibin = 2; ibin <= kNumCuts; ++ibin ) {
    Int_t    icut    = ibin - 1;
    Double_t nevents = Hist( "cutflow", defaultCutflow )->GetBinContent( ibin );
    Double_t releff  = 100. * nevents / Hist( "cutflow", defaultCutflow )->GetBinContent( ibin-1 );
    Double_t abseff  = 100. * nevents / ntot;
    m_logger << INFO  << Form( "C%d\t%25.25s\t%6.0f\t%6.2f\t%6.2f", icut-1, kCutName[icut].c_str(), nevents, releff, abseff ) << SLogger::endmsg;
  }
   
   return;

}

void WHAnalysis::BeginInputFile( const SInputData& ) throw( SError ) {

  m_logger << INFO << "Connecting input variables" << SLogger::endmsg;
  if (m_isData) {
    // m_jetAK4.ConnectVariables(       m_recoTreeName.c_str(), D3PD::JetBasic|D3PD::JetAnalysis, (m_jetAK4Name + "_").c_str() );
    m_jetAK8.ConnectVariables(       m_recoTreeName.c_str(), D3PD::JetBasic|D3PD::JetAnalysis|D3PD::JetSubstructure|D3PD::JetPrunedSubjets, (m_jetAK8Name + "_").c_str() );
    m_eventInfo.ConnectVariables(    m_recoTreeName.c_str(), D3PD::EventInfoBasic|D3PD::EventInfoTrigger|D3PD::EventInfoMETFilters, "" );
  }
  else {
    // m_jetAK4.ConnectVariables(       m_recoTreeName.c_str(), D3PD::JetBasic|D3PD::JetAnalysis|D3PD::JetTruth, (m_jetAK4Name + "_").c_str() );
    m_jetAK8.ConnectVariables(       m_recoTreeName.c_str(), D3PD::JetBasic|D3PD::JetAnalysis|D3PD::JetSubstructure|D3PD::JetTruth|D3PD::JetPrunedSubjets|D3PD::JetPrunedSubjetsTruth, (m_jetAK8Name + "_").c_str() );
    m_eventInfo.ConnectVariables(    m_recoTreeName.c_str(), D3PD::EventInfoBasic|D3PD::EventInfoTrigger|D3PD::EventInfoMETFilters|D3PD::EventInfoTruth, "" );
    m_genParticle.ConnectVariables(  m_recoTreeName.c_str(), D3PD::GenParticleBasic, (m_genParticleName + "_").c_str() );
  }
  // m_electron.ConnectVariables(     m_recoTreeName.c_str(), D3PD::ElectronBasic|D3PD::ElectronID, (m_electronName + "_").c_str() );
  // m_muon.ConnectVariables(         m_recoTreeName.c_str(), D3PD::MuonBasic|D3PD::MuonID|D3PD::MuonIsolation, (m_muonName + "_").c_str() );
  // m_missingEt.ConnectVariables(    m_recoTreeName.c_str(), D3PD::MissingEtBasic, (m_missingEtName + "_").c_str() );
  
  m_logger << INFO << "Connecting input variables completed" << SLogger::endmsg;

   return;

}

void WHAnalysis::ExecuteEvent( const SInputData&, Double_t ) throw( SError ) {

  m_logger << VERBOSE << "ExecuteEvent" << SLogger::endmsg;
  
  clearBranches();
  
  b_eventNumber = m_eventInfo.eventNumber;
  b_runNumber = m_eventInfo.runNumber;
  b_lumiBlock = m_eventInfo.lumiBlock;
  
  std::vector<TBits> selectionBits(m_catNames.size(), TBits(kNumCuts));
  for (unsigned int s=0;s<m_catNames.size();++s) {
    selectionBits[s].SetBitNumber( kBeforeCuts );
  }
  bool moveOn = true;
  
  // Cut 1: check for data if run/lumiblock in JSON
  if (m_isData) {
    if (isGoodEvent(m_eventInfo.runNumber, m_eventInfo.lumiBlock)) {
      for (unsigned int s=0;s<m_catNames.size();++s) {
        selectionBits[s].SetBitNumber( kJSON );
      }
    }
  }
  else {
    for (unsigned int s=0;s<m_catNames.size();++s) {
      selectionBits[s].SetBitNumber( kJSON );
    }
  }
  
  // Cut 2: pass trigger
  if (passTrigger()) {
    m_logger << VERBOSE << "Trigger pass" << SLogger::endmsg;
    for (unsigned int s=0;s<m_catNames.size();++s) {
      selectionBits[s].SetBitNumber( kTrigger );
    }
    // m_logger << INFO << "pass: " << selectionBits[0].TestBitNumber( kTrigger ) << SLogger::endmsg;
  }
  // else {
  //   m_logger << WARNING << "Trigger fail:" << selectionBits[0].TestBitNumber( kTrigger ) << SLogger::endmsg;
  //   for (unsigned int s=0;s<m_catNames.size();++s) {
  //     m_logger << WARNING << selectionBits[s].TestBitNumber( kTrigger ) << SLogger::endmsg;
  //   }
  // }
  
  // Cut 3: pass MET filters
  if (passMETFilters()) {
    m_logger << VERBOSE << "passMETFilters" << SLogger::endmsg;
    for (unsigned int s=0;s<m_catNames.size();++s) {
      selectionBits[s].SetBitNumber( kMetFilters );
    }
  }
  
  // Cut 4: select two fat jets
  std::vector<DESY::Jet> goodFatJets;
  for ( int i = 0; i < (m_jetAK8.N); ++i ) {
    DESY::Jet myjet( &m_jetAK8, i );
    if (myjet.pt() > m_jetPtCut) {
      if (fabs(myjet.eta()) < m_jetEtaCut) {
        if (myjet.IDTight()) {
          goodFatJets.push_back(myjet);
          
          // fill output variables
          if (goodFatJets.size() == 1) {
            b_ak8jet0_pt = myjet.pt();
            b_ak8jet0_phi = myjet.phi();
            b_ak8jet0_eta = myjet.eta();
            b_ak8jet0_e = myjet.e();
            double tau21 = -1;
            if (myjet.tau1() != 0) {
              tau21 = myjet.tau2() / myjet.tau1();
            }
            b_ak8jet0_tau21 = tau21;
            b_ak8jet0_m = myjet.m();
            b_ak8jet0_mpruned = myjet.pruned_massCorr();
            b_ak8jet0_csv = myjet.csv();
            if (myjet.subjet_pruned_N() >= 2) {
              double deta = fabs(myjet.subjet_pruned_eta()[0] - myjet.subjet_pruned_eta()[1]);
              double dphi = fabs(myjet.subjet_pruned_phi()[0] - myjet.subjet_pruned_phi()[1]);
              double dr = sqrt(deta*deta + dphi*dphi);
              b_ak8jet0_subjet01_dr = dr;
              b_ak8jet0_subjet01_deta = deta;
              b_ak8jet0_subjet01_dphi = dphi;
              b_ak8jet0_subjet0_pt = myjet.subjet_pruned_pt()[0];
              b_ak8jet0_subjet1_pt = myjet.subjet_pruned_pt()[1];
              b_ak8jet0_subjet0_csv = myjet.subjet_pruned_csv()[0];
              b_ak8jet0_subjet1_csv = myjet.subjet_pruned_csv()[1];
            }
          }
          else if (goodFatJets.size() == 2) {
            b_ak8jet1_pt = myjet.pt();
            b_ak8jet1_phi = myjet.phi();
            b_ak8jet1_eta = myjet.eta();
            b_ak8jet1_e = myjet.e();
            double tau21 = -1;
            if (myjet.tau1() != 0) {
              tau21 = myjet.tau2() / myjet.tau1();
            }
            b_ak8jet1_tau21 = tau21;
            b_ak8jet1_m = myjet.m();
            b_ak8jet1_mpruned = myjet.pruned_massCorr();
            b_ak8jet1_csv = myjet.csv();
            if (myjet.subjet_pruned_N() >= 2) {
              double deta = fabs(myjet.subjet_pruned_eta()[0] - myjet.subjet_pruned_eta()[1]);
              double dphi = fabs(myjet.subjet_pruned_phi()[0] - myjet.subjet_pruned_phi()[1]);
              double dr = sqrt(deta*deta + dphi*dphi);
              b_ak8jet1_subjet01_dr = dr;
              b_ak8jet1_subjet01_deta = deta;
              b_ak8jet1_subjet01_dphi = dphi;
              b_ak8jet1_subjet0_pt = myjet.subjet_pruned_pt()[0];
              b_ak8jet1_subjet1_pt = myjet.subjet_pruned_pt()[1];
              b_ak8jet1_subjet0_csv = myjet.subjet_pruned_csv()[0];
              b_ak8jet1_subjet1_csv = myjet.subjet_pruned_csv()[1];
            }
          }
        }
      }
    }
  }
  if (goodFatJets.size() >= 2) {
    for (unsigned int s=0;s<m_catNames.size();++s) {
      selectionBits[s].SetBitNumber( kTwoFatJets );
    }
  }
  else {
    // can only continue with at least two selected fat jets
    moveOn = false;
  }
  
  m_logger << VERBOSE << "kTwoFatJets" << SLogger::endmsg;
  
  int goodFatJet1Index = -1;
  int goodFatJet2Index = -1;
  
  if (moveOn) {
    // Cut 5: find two selected jets that are close in DeltaEta
  
    for (unsigned int i = 0; i < goodFatJets.size()-1; ++i) {
      for (unsigned int j = i+1; j < goodFatJets.size(); ++j) {
        if (fabs(goodFatJets[i].eta() - goodFatJets[j].eta()) < m_jetDeltaEtaCut) {
          for (unsigned int s=0;s<m_catNames.size();++s) {
            selectionBits[s].SetBitNumber( kFatJetsDeltaEta );
          }
          goodFatJet1Index = i;
          goodFatJet2Index = j;
        }
        if (goodFatJet1Index != -1) break;
      }
      if (goodFatJet1Index != -1) break;
    }
    if (!(selectionBits[0].TestBitNumber( kFatJetsDeltaEta ))) {
      // need to have selected the two candidate jets
      moveOn = false;
    }
  }
  
  DESY::Jet vectorJet;
  DESY::Jet higgsJet;
  
  m_logger << VERBOSE << "kFatJetsDeltaEta" << SLogger::endmsg;
  
  if (moveOn) { // need to have selected the two candidate jets
    
    moveOn = false;
  
    // Cut 6: require dijet system to pass mass threshold
    TLorentzVector dijet = goodFatJets[goodFatJet1Index].tlv() + goodFatJets[goodFatJet2Index].tlv();
    if (dijet.M() > m_mjjCut) {
      for (unsigned int s=0;s<m_catNames.size();++s) {
        selectionBits[s].SetBitNumber( kDijetMass );
      }
    }
    
    m_logger << VERBOSE << "kDijetMass" << SLogger::endmsg;

    // Cut 7: require one of the jets to be in the V-boson mass window
    // make other selected jet the Higgs jet
    if ((goodFatJets[goodFatJet1Index].pruned_massCorr() > m_mWLowerCut) && (goodFatJets[goodFatJet1Index].pruned_massCorr() <= m_mZUpperCut)) {
      vectorJet = goodFatJets[goodFatJet1Index];
      higgsJet = goodFatJets[goodFatJet2Index];
      moveOn = true;
    }
    else if ((goodFatJets[goodFatJet2Index].pruned_massCorr() > m_mWLowerCut) && (goodFatJets[goodFatJet2Index].pruned_massCorr() <= m_mZUpperCut)) {
      vectorJet = goodFatJets[goodFatJet2Index];
      higgsJet = goodFatJets[goodFatJet1Index];
      moveOn = true;
    }
    if (moveOn) {
      for (unsigned int s=0;s<m_catNames.size();++s) {
        if (m_catNames[s].find("VWindow") != std::string::npos) {
          selectionBits[s].SetBitNumber( kVWindow );
        }
        if ((m_catNames[s].find("WWindow") != std::string::npos) && (vectorJet.pruned_massCorr() <= m_mWUpperCut)) {
          selectionBits[s].SetBitNumber( kVWindow );
        }
        if ((m_catNames[s].find("ZWindow") != std::string::npos) && (vectorJet.pruned_massCorr() > m_mZLowerCut)) {
          selectionBits[s].SetBitNumber( kVWindow );
        }
      }
    }
    else { // this means we're not in the signal region of the pruned mass
      if ((goodFatJets[goodFatJet1Index].pruned_massCorr() > m_mVLowSidebandCut) && (goodFatJets[goodFatJet1Index].pruned_massCorr() <= m_mWLowerCut)) {
        vectorJet = goodFatJets[goodFatJet1Index];
        higgsJet = goodFatJets[goodFatJet2Index];
        moveOn = true;
      }
      else if ((goodFatJets[goodFatJet2Index].pruned_massCorr() > m_mVLowSidebandCut) && (goodFatJets[goodFatJet2Index].pruned_massCorr() <= m_mWLowerCut)) {
        vectorJet = goodFatJets[goodFatJet2Index];
        higgsJet = goodFatJets[goodFatJet1Index];
        moveOn = true;
      }
      if (moveOn) {
        for (unsigned int s=0;s<m_catNames.size();++s) {
          if (m_catNames[s].find("VLowSB") != std::string::npos) {
            selectionBits[s].SetBitNumber( kVWindow );
          }
        }      
      }
    }
  } // moveOn after having selected two candidate jets
  
  if (moveOn) { // move on only if V- and H-jets identified
    
    m_logger << VERBOSE << "kVWindow" << SLogger::endmsg;
    
    m_logger << VERBOSE << higgsJet << SLogger::endmsg;    

    // Cut 8: check if Higgs candidate jet is in Higgs mass window
    if ((higgsJet.pruned_massCorr() > m_mHLowerCut) && (higgsJet.pruned_massCorr() <= m_mHUpperCut)) {
      for (unsigned int s=0;s<m_catNames.size();++s) {
        if (m_catNames[s].find("HLowSB") == std::string::npos) { // not HLowSB category
          selectionBits[s].SetBitNumber( kHiggsWindow );
        }
      }
    }
    else if ((higgsJet.pruned_massCorr() > m_mVLowSidebandCut) && (higgsJet.pruned_massCorr() <= m_mWLowerCut)) {
      for (unsigned int s=0;s<m_catNames.size();++s) {
        if (m_catNames[s].find("HLowSB") != std::string::npos) { // HLowSB category
          selectionBits[s].SetBitNumber( kHiggsWindow );
        }
      }
    }
    
    m_logger << VERBOSE << "kHiggsWindow" << SLogger::endmsg;

    m_logger << VERBOSE << vectorJet << SLogger::endmsg;    

    // Cut 9: pass tau21 for V-candidate jet
    if (vectorJet.tau1() != 0) {
      double tau21 = vectorJet.tau2() / vectorJet.tau1();
      m_logger << VERBOSE << tau21 << SLogger::endmsg;
      if (tau21 < m_tau21HPCut) {
        for (unsigned int s=0;s<m_catNames.size();++s) {
          if ((m_catNames[s].find("Tau21HP") != std::string::npos)) {
            m_logger << VERBOSE << m_catNames[s] << SLogger::endmsg;
            selectionBits[s].SetBitNumber( kTau21 );
          }
        }
      }
      else if ((tau21 >= m_tau21HPCut) && (tau21 < m_tau21LPCut)) {
        for (unsigned int s=0;s<m_catNames.size();++s) {
          if ((m_catNames[s].find("Tau21LP") != std::string::npos)) {
            m_logger << VERBOSE << m_catNames[s] << SLogger::endmsg;
            selectionBits[s].SetBitNumber( kTau21 );
          }
        }
      }
    }
    for (unsigned int s=0;s<m_catNames.size();++s) {
      if (m_catNames[s].find("NoTau21") != std::string::npos) {
        selectionBits[s].SetBitNumber( kTau21 );
      }
    }
    
    m_logger << VERBOSE << "kTau21" << SLogger::endmsg;
    
    // Cut 10: require at least one of the subjets from the Higgs jet to be b-tagged
    // Cut 11: require two subjets from the Higgs jet to be b-tagged
    // count number of b-tagged subjets
    int nTaggedSubjets = 0;
    for (int i = 0; i < higgsJet.subjet_pruned_N(); ++i) {
      if (higgsJet.subjet_pruned_csv()[i] > m_csvMin) {
        ++nTaggedSubjets;
      }
    }
    for (unsigned int s=0;s<m_catNames.size();++s) {
      if (m_catNames[s].find("NoTag") != std::string::npos) {
        if (nTaggedSubjets == 0) {
          selectionBits[s].SetBitNumber( kSubjetSingleTag );
          selectionBits[s].SetBitNumber( kSubjetDoubleTag );
        }
      }
      else if (m_catNames[s].find("PreTag") != std::string::npos) {
        selectionBits[s].SetBitNumber( kSubjetSingleTag );
        selectionBits[s].SetBitNumber( kSubjetDoubleTag );
      }
      else if (m_catNames[s].find("SingleTagExcl") != std::string::npos) {
        if (nTaggedSubjets == 1) {
          selectionBits[s].SetBitNumber( kSubjetSingleTag );
          selectionBits[s].SetBitNumber( kSubjetDoubleTag );
        }
      }
      else if (m_catNames[s].find("SingleTagIncl") != std::string::npos) {
        if (nTaggedSubjets >= 1) {
          selectionBits[s].SetBitNumber( kSubjetSingleTag );
          selectionBits[s].SetBitNumber( kSubjetDoubleTag );
        }
      }
      else if (nTaggedSubjets >= 1) {
        // these will all be DoubleTag
        selectionBits[s].SetBitNumber( kSubjetSingleTag );
        if (nTaggedSubjets >= 2) {
          selectionBits[s].SetBitNumber( kSubjetDoubleTag );
        }
      }
    }
    
  
    // selection done
    m_logger << VERBOSE << "selection done" << SLogger::endmsg;
    if (!m_isData) {
      b_weight = getEventWeight();
    }
  
    // std::vector<DESY::Jet> goodJetsAK4;
    // for ( int i = 0; i < (m_jetAK4.N); ++i ) {
    //   DESY::Jet myjet( &m_jetAK4, i );
    //   if (fabs(myjet.eta()) < m_jetEtaCut) {
    //     if (myjet.pt() > m_jetPtCut) {
    //       goodJetsAK4.push_back(myjet);
    //       // m_logger << INFO << myjet.pt() << SLogger::endmsg;
    //       Book( TH1F( "Jet_pt", "Jet p_{T} [GeV]", 100, 0.0, 1000 ), "AK4_low" )->Fill( myjet.pt() );
    //       Book( TH1F( "Jet_eta", "Jet #eta", 50, -2.5, 2.5 ), "AK4_low" )->Fill( myjet.eta() );
    //       Book( TH1F( "Jet_m", "Jet m [GeV]", 50, 0, 250 ), "AK4_low" )->Fill( myjet.m() );
    //       Book( TH1F( "Jet_phi", "Jet #phi", 50, -TMath::Pi(), TMath::Pi() ), "AK4_low" )->Fill( myjet.phi() );
    //       Book( TH1F( "Jet_e", "Jet e [GeV]", 100, 0.0, 1000 ), "AK4_low" )->Fill( myjet.e() );
    //
    //       Book( TH1F( "Jet_muf", "Jet muf", 50, 0.0, 1.0 ), "AK4_low" )->Fill( myjet.muf() );
    //       Book( TH1F( "Jet_phf", "Jet phf", 50, 0.0, 1.0 ), "AK4_low" )->Fill( myjet.phf() );
    //       Book( TH1F( "Jet_emf", "Jet emf", 50, 0.0, 1.0 ), "AK4_low" )->Fill( myjet.emf() );
    //       Book( TH1F( "Jet_nhf", "Jet nhf", 50, 0.0, 1.0 ), "AK4_low" )->Fill( myjet.nhf() );
    //       Book( TH1F( "Jet_chf", "Jet chf", 50, 0.0, 1.0 ), "AK4_low" )->Fill( myjet.chf() );
    //       Book( TH1F( "Jet_area", "Jet area", 40, 0.0, 4.0 ), "AK4_low" )->Fill( myjet.area() );
    //       Book( TH1F( "Jet_cm", "Jet cm", 50, 0.0, 50.0 ), "AK4_low" )->Fill( myjet.cm() );
    //       Book( TH1F( "Jet_nm", "Jet nm", 50, 0.0, 50.0 ), "AK4_low" )->Fill( myjet.nm() );
    //     }
    //   }
    // }
    //
    //
    // double mW_min = 60;
    // double mW_max = 100;
    //
    // std::vector<DESY::Jet> goodJetsAK8;
    // for ( int i = 0; i < (m_jetAK8.N); ++i ) {
    //   DESY::Jet myjet( &m_jetAK8, i );
    //   if (fabs(myjet.eta()) < m_jetEtaCut) {
    //     if (myjet.pt() > m_jetPtCut) {
    //       goodJetsAK8.push_back(myjet);
    //       // m_logger << INFO << myjet.pt() << SLogger::endmsg;
    //       Book( TH1F( "Jet_pt", "Jet p_{T} [GeV]", 100, 0.0, 1000 ), "AK8_low" )->Fill( myjet.pt() );
    //       Book( TH1F( "Jet_eta", "Jet #eta", 50, -2.5, 2.5 ), "AK8_low" )->Fill( myjet.eta() );
    //       Book( TH1F( "Jet_m", "Jet m [GeV]", 50, 0, 250 ), "AK8_low" )->Fill( myjet.m() );
    //       Book( TH1F( "Jet_phi", "Jet #phi", 50, -TMath::Pi(), TMath::Pi() ), "AK8_low" )->Fill( myjet.phi() );
    //       Book( TH1F( "Jet_e", "Jet e [GeV]", 100, 0.0, 1000 ), "AK8_low" )->Fill( myjet.e() );
    //
    //       Book( TH1F( "Jet_muf", "Jet muf", 50, 0.0, 1.0 ), "AK8_low" )->Fill( myjet.muf() );
    //       Book( TH1F( "Jet_phf", "Jet phf", 50, 0.0, 1.0 ), "AK8_low" )->Fill( myjet.phf() );
    //       Book( TH1F( "Jet_emf", "Jet emf", 50, 0.0, 1.0 ), "AK8_low" )->Fill( myjet.emf() );
    //       Book( TH1F( "Jet_nhf", "Jet nhf", 50, 0.0, 1.0 ), "AK8_low" )->Fill( myjet.nhf() );
    //       Book( TH1F( "Jet_chf", "Jet chf", 50, 0.0, 1.0 ), "AK8_low" )->Fill( myjet.chf() );
    //       Book( TH1F( "Jet_area", "Jet area", 40, 0.0, 4.0 ), "AK8_low" )->Fill( myjet.area() );
    //       Book( TH1F( "Jet_cm", "Jet cm", 50, 0.0, 50.0 ), "AK8_low" )->Fill( myjet.cm() );
    //       Book( TH1F( "Jet_nm", "Jet nm", 50, 0.0, 50.0 ), "AK8_low" )->Fill( myjet.nm() );
    //
    //       Book( TH1F( "Jet_tau1", "Jet tau1", 50, 0, 1.0 ), "AK8_low" )->Fill( myjet.tau1() );
    //       Book( TH1F( "Jet_tau2", "Jet tau2", 50, 0, 1.0 ), "AK8_low" )->Fill( myjet.tau2() );
    //       Book( TH1F( "Jet_tau3", "Jet tau3", 50, 0, 1.0 ), "AK8_low" )->Fill( myjet.tau3() );
    //
    //       Book( TH1F( "Jet_tau21", "Jet tau21", 50, 0, 1.0 ), "AK8_low" )->Fill( myjet.tau2()/myjet.tau1() );
    //       Book( TH1F( "Jet_tau32", "Jet tau32", 50, 0, 1.0 ), "AK8_low" )->Fill( myjet.tau3()/myjet.tau2() );
    //       Book( TH1F( "Jet_tau31", "Jet tau31", 50, 0, 1.0 ), "AK8_low" )->Fill( myjet.tau3()/myjet.tau1() );
    //
    //       Book( TH1F( "Jet_pruned_m", "Jet pruned m [GeV]", 50, 0, 250 ), "AK8_low" )->Fill( myjet.pruned_mass() );
    //       Book( TH1F( "Jet_softdrop_m", "Jet softdrop m [GeV]", 50, 0, 250 ), "AK8_low" )->Fill( myjet.softdrop_mass() );
    //
    //       if ((myjet.pruned_mass() > mW_min) && (myjet.pruned_mass() < mW_max))
    //         Book( TH1F( "Jet_pruned_Wwindow_m", "Jet pruned W window m [GeV]", 50, mW_min, mW_max ), "AK8_low" )->Fill( myjet.pruned_mass() );
    //       if ((myjet.softdrop_mass() > mW_min) && (myjet.softdrop_mass() < mW_max))
    //         Book( TH1F( "Jet_softdrop_Wwindow_m", "Jet softdrop W window m [GeV]", 50, mW_min, mW_max ), "AK8_low" )->Fill( myjet.softdrop_mass() );
    //
    //     }
    //   }
    // }
  
  } // two fat jets
  
  // book-keeping
  Hist( "Events" )->Fill( 0., b_weightGen        ); // event with MC weight
  Hist( "Events" )->Fill( 1,  b_weight           ); // event total weight
  Hist( "Events" )->Fill( 2,  b_weightPU         ); // event pileup weight
  // Hist( "Events" )->Fill( 3,  b_weight_elScale   ); // event electron SF weight
  // Hist( "Events" )->Fill( 4,  b_weight_muScale   ); // event muon SF weight
  // Hist( "Events" )->Fill( 5,  b_weight_btagScale ); // event b-tag SF weight
  Hist( "Events" )->Fill( 9,  1                  ); // event without MC weight
  Hist( "SumEvents" )->Fill( 0., fabs(b_weightGen)       ); // event with MC weight
  Hist( "SumEvents" )->Fill( 1,  fabs(b_weight)          ); // event total weight
  Hist( "SumEvents" )->Fill( 2,  fabs(b_weightPU)       ); // event pileup weight
  // Hist( "SumEvents" )->Fill( 3,  fabs(b_weight_elScale)  ); // event electron SF weight
  // Hist( "SumEvents" )->Fill( 4,  fabs(b_weight_muScale)  ); // event muon SF weight
  // Hist( "SumEvents" )->Fill( 5,  fabs(b_weight_btagScale)); // event b-tag SF weight
  // Hist( "SumEvents" )->Fill( 6,  fabs(b_weight_jvfScale));  // event JVF SF weight
  Hist( "SumEvents" )->Fill( 9,  1                       ); // event without MC weight
  
  // fill cut info for ntuple
  std::vector<bool> passed_all(m_catNames.size(), true);
  for (unsigned int s=0;s<m_catNames.size();++s) {
    for( UInt_t icut = 0; icut < kNumCuts; ++icut ) {
      if( selectionBits[s].TestBitNumber( icut ) != kTRUE ){
        passed_all[s] = false;
      }
      else{
        if (icut) b_selection_bits[s]|=1<<icut; 
        if (icut-1==(unsigned)b_selection_lastcut[s])
          b_selection_lastcut[s]++;
      }
    }//cut loop
  }//category loop
  
  for (unsigned int s=0;s<m_catNames.size();++s) {
    // m_logger << INFO << selectionBits[s].TestBitNumber( kTrigger ) << SLogger::endmsg;
    fillCutflow("cutflow", m_catNames[s], selectionBits[s], b_weight);
  }
  
  bool doHistograms = false;
  // need vectorJet and higgsJet to be defined
  for (unsigned int s=0;s<m_catNames.size();++s) {
    if (selectionBits[s].TestBitNumber( kVWindow )) {
      doHistograms = true;
    }
  }
  
  m_logger << VERBOSE << "before doHistograms" << SLogger::endmsg;
  
  if (doHistograms) {
    // calculate a few variables before filling histograms
    double vJet_tau21 = -1;
    double vJet_tau31 = -1;
    double vJet_tau32 = -1;
    if (vectorJet.tau1() != 0) {
      vJet_tau21 = vectorJet.tau2()/vectorJet.tau1();
      vJet_tau31 = vectorJet.tau3()/vectorJet.tau1();
    }
    if (vectorJet.tau2() != 0) {
      vJet_tau32 = vectorJet.tau3()/vectorJet.tau2();
    }
    int vJet_nTaggedSubjets = 0;
    double vJet_subjet0_csv = -99;
    double vJet_subjet1_csv = -99;
    for (int i = 0; i < vectorJet.subjet_pruned_N(); ++i) {
      switch(i) {
        case 0:
          vJet_subjet0_csv = vectorJet.subjet_pruned_csv()[i];
          break;
        case 1:
          vJet_subjet1_csv = vectorJet.subjet_pruned_csv()[i];
          break;
      }
      if (vectorJet.subjet_pruned_csv()[i] > m_csvMin) {
        ++vJet_nTaggedSubjets;
      }
    }

    double hJet_tau21 = -1;
    double hJet_tau31 = -1;
    double hJet_tau32 = -1;
    if (higgsJet.tau1() != 0) {
      hJet_tau21 = higgsJet.tau2()/higgsJet.tau1();
      hJet_tau31 = higgsJet.tau3()/higgsJet.tau1();
    }
    if (higgsJet.tau2() != 0) {
      hJet_tau32 = higgsJet.tau3()/higgsJet.tau2();
    }
    int hJet_nTaggedSubjets = 0;
    double hJet_subjet0_csv = -99;
    double hJet_subjet1_csv = -99;
    for (int i = 0; i < higgsJet.subjet_pruned_N(); ++i) {
      switch(i) {
        case 0:
          hJet_subjet0_csv = higgsJet.subjet_pruned_csv()[i];
          break;
        case 1:
          hJet_subjet1_csv = higgsJet.subjet_pruned_csv()[i];
          break;
      }
      if (higgsJet.subjet_pruned_csv()[i] > m_csvMin) {
        ++hJet_nTaggedSubjets;
      }
    }

    double deta = fabs(vectorJet.eta() - higgsJet.eta());
    double dphi = fabs(vectorJet.phi() - higgsJet.phi());
    double dr = sqrt(deta*deta + dphi*dphi);
    TLorentzVector diJet = vectorJet.tlv() + higgsJet.tlv();
    
    m_logger << VERBOSE << "category loopfillHistograms" << SLogger::endmsg;

    for (unsigned int s=0;s<m_catNames.size();++s) {
      if (passed_all[s]) {
        m_logger << VERBOSE << m_catNames[s] << SLogger::endmsg;
        fillHistograms(m_catNames[s], vectorJet, higgsJet, diJet, vJet_tau21, vJet_tau31, vJet_tau32, vJet_nTaggedSubjets, vJet_subjet0_csv, vJet_subjet1_csv, hJet_tau21, hJet_tau31, hJet_tau32, hJet_nTaggedSubjets, hJet_subjet0_csv, hJet_subjet1_csv, deta, dphi, dr );
      }
    }
  }
  m_logger << VERBOSE << "return" << SLogger::endmsg;
  
   return;

}

bool WHAnalysis::isGoodEvent(int runNumber, int lumiSection) {
  
  bool isGood = true;
  if (m_isData) {
    isGood = m_grl.HasRunLumiBlock( runNumber, lumiSection );
    if( !isGood ) {
      m_logger << WARNING << "Bad event! Run: " << runNumber <<  " - Lumi Section: " << lumiSection << SLogger::endmsg;
      // throw SError( SError::SkipEvent );
    }
    else m_logger << VERBOSE << "Good event! Run: " << runNumber <<  " - Lumi Section: " << lumiSection << SLogger::endmsg;
  }
  return isGood;
  
}


bool WHAnalysis::passTrigger() {
  
  bool passTrigger = false;
  
  for (std::map<std::string,bool>::iterator it = (m_eventInfo.trigDecision)->begin(); it != (m_eventInfo.trigDecision)->end(); ++it){
    for (unsigned int t = 0; t < m_triggerNames.size(); ++t ){
      if ((it->first).find(m_triggerNames[t]) != std::string::npos) {
        if (it->second == true) {
          m_logger << VERBOSE << "Trigger pass: " << (it->first) << SLogger::endmsg;
          passTrigger = true;
          return passTrigger;
        }
      }
    }
  }
  
  return passTrigger;
  
}


bool WHAnalysis::passMETFilters() {
  
  bool passMetFilters = true;
  
  // using only what's recommended in https://twiki.cern.ch/twiki/bin/viewauth/CMS/MissingETOptionalFiltersRun2
    
  if( !(m_eventInfo.PV_filter) ) {
    passMetFilters = false;
    m_logger << VERBOSE << "PV_filter" << SLogger::endmsg;
    Hist( "METFilters" )->Fill(1);
  }
  if( !(m_eventInfo.passFilter_CSCHalo) ) {
    passMetFilters = false;
    m_logger << VERBOSE << "passFilter_CSCHalo" << SLogger::endmsg;
    Hist( "METFilters" )->Fill(2);
  }
  if( !(m_eventInfo.passFilter_HBHELoose) ) {
    passMetFilters = false;
    m_logger << VERBOSE << "passFilter_HBHELoose" << SLogger::endmsg;
    Hist( "METFilters" )->Fill(3);
  }
  if( !(m_eventInfo.passFilter_HBHEIso) ) {
    passMetFilters = false;
    m_logger << VERBOSE << "passFilter_HBHEIso" << SLogger::endmsg;
    Hist( "METFilters" )->Fill(4);
  }
  if( !(m_eventInfo.passFilter_EEBadSc) ) {
    passMetFilters = false;
    m_logger << VERBOSE << "passFilter_EEBadSc" << SLogger::endmsg;
    Hist( "METFilters" )->Fill(5);
  }
  
  return passMetFilters;
  
}


double WHAnalysis::getEventWeight() {
  
  double weight = 1.;
  for( unsigned int v = 0; v < (m_eventInfo.actualIntPerXing)->size(); ++v ){
    if ( (*m_eventInfo.bunchCrossing)[v] == 0 ) {
      b_weightPU = m_pileupReweightingTool.getPileUpweight( (*m_eventInfo.actualIntPerXing)[v] );
      m_logger << VERBOSE << "Weight: " << b_weightPU << " for true: " << (*m_eventInfo.actualIntPerXing)[v] << SLogger::endmsg;
      break;
    }
  }
  b_weightGen = (m_eventInfo.genEventWeight < 0) ? -1 : 1; 
  weight *= b_weightPU*b_weightGen;
  
  return weight;
  
}

void WHAnalysis::clearBranches() {
  
  b_weight = 1.;
  b_weightGen = 1.;
  b_weightPU = 1.;
  b_weightBtag = 1.;
  
  b_runNumber = -99;;
  b_eventNumber = -99;
  b_lumiBlock = -99;
  
  b_ak8jet0_pt = -99;
  b_ak8jet0_phi = -99;
  b_ak8jet0_eta = -99;
  b_ak8jet0_e = -99;
  b_ak8jet0_tau21 = -99;
  b_ak8jet0_m = -99;
  b_ak8jet0_mpruned = -99;
  b_ak8jet0_csv = -99;
  b_ak8jet1_pt = -99;
  b_ak8jet1_phi = -99;
  b_ak8jet1_eta = -99;
  b_ak8jet1_e = -99;
  b_ak8jet1_tau21 = -99;
  b_ak8jet1_m = -99;
  b_ak8jet1_mpruned = -99;
  b_ak8jet1_csv = -99;
  b_ak8jet0_subjet01_dr = -99;
  b_ak8jet0_subjet01_deta = -99;
  b_ak8jet0_subjet01_dphi = -99;
  b_ak8jet0_subjet0_pt = -99;
  b_ak8jet0_subjet1_pt = -99;
  b_ak8jet0_subjet0_csv = -99;
  b_ak8jet0_subjet1_csv = -99;
  b_ak8jet1_subjet01_dr = -99;
  b_ak8jet1_subjet01_deta = -99;
  b_ak8jet1_subjet01_dphi = -99;
  b_ak8jet1_subjet0_pt = -99;
  b_ak8jet1_subjet1_pt = -99;
  b_ak8jet1_subjet0_csv = -99;
  b_ak8jet1_subjet1_csv = -99;
  
  b_selection_bits.clear();
  b_selection_lastcut.clear();
  
}

void WHAnalysis::fillCutflow( const std::string histName, const std::string dirName, const TBits& cutmap, const Double_t weight ) {

  // bool writeNtuple = false;
  // sequential cut flow -> stop at first failed cut
  // m_logger << INFO << histName << "\t" << dirName << SLogger::endmsg;
  for( UInt_t i = 0; i < cutmap.GetNbits(); ++i ) {
    // m_logger << INFO << i << ":\t" << cutmap.TestBitNumber( i ) << SLogger::endmsg;
    if( cutmap.TestBitNumber( i ) ) {
      Hist( histName.c_str(), dirName.c_str() )->Fill( i+1, weight );
      // if (i == (unsigned int) m_ntupleLevel) {
      //   writeNtuple = true;
      // }
    } else {
      break;
    }
  }
  // if (!writeNtuple) {
  // // this does something really bad...
  //   throw SError( SError::SkipEvent );
  // }
}

void WHAnalysis::bookHistograms( const TString& directory ) {
  
  // kinematics histograms
  Book( TH1F( "vjet_pt", "Vjet p_{T};Vjet p_{T} [GeV]", 200, 0, 2000 ), directory ); 
  Book( TH1F( "vjet_eta", "Vjet #eta;Vjet #eta", 50, -2.5, 2.5 ), directory ); 
  Book( TH1F( "vjet_phi", "Vjet #phi;Vjet #phi", 50, -3.15, 3.15 ), directory ); 
  Book( TH1F( "vjet_m", "Vjet m;Vjet m [GeV]", 40, 0, 200 ), directory ); 
  Book( TH1F( "vjet_mpruned", "Vjet pruned m;Vjet pruned m [GeV]", 40, 0, 200 ), directory ); 
  Book( TH1F( "vjet_tau1", "Vjet #tau_{1};Vjet #tau_{1}", 50, 0, 1 ), directory ); 
  Book( TH1F( "vjet_tau2", "Vjet #tau_{2};Vjet #tau_{2}", 50, 0, 1 ), directory ); 
  Book( TH1F( "vjet_tau3", "Vjet #tau_{3};Vjet #tau_{3}", 50, 0, 1 ), directory ); 
  Book( TH1F( "vjet_tau21", "Vjet #tau_{21};Vjet #tau_{21}", 50, 0, 1 ), directory ); 
  Book( TH1F( "vjet_tau31", "Vjet #tau_{31};Vjet #tau_{31}", 50, 0, 1 ), directory ); 
  Book( TH1F( "vjet_tau32", "Vjet #tau_{32};Vjet #tau_{32}", 50, 0, 1 ), directory ); 
  Book( TH1F( "vjet_nSubjets", "Vjet N subjets;Vjet N subjets", 10, -.5, 9.5 ), directory ); 
  Book( TH1F( "vjet_nTaggedSubjets", "Vjet N tagged subjets;Vjet N tagged subjets", 10, -.5, 9.5 ), directory ); 
  Book( TH1F( "vjet_subjet0_csv", "Vjet subjet 0 CSV;Vjet subjet0 CSV", 50, 0, 1 ), directory ); 
  Book( TH1F( "vjet_subjet1_csv", "Vjet subjet 1 CSV;Vjet subjet1 CSV", 50, 0, 1 ), directory ); 

  Book( TH1F( "hjet_pt", "Hjet p_{T};Hjet p_{T} [GeV]", 200, 0, 2000 ), directory ); 
  Book( TH1F( "hjet_eta", "Hjet #eta;Hjet #eta", 50, -2.5, 2.5 ), directory ); 
  Book( TH1F( "hjet_phi", "Hjet #phi;Hjet #phi", 50, -3.15, 3.15 ), directory ); 
  Book( TH1F( "hjet_m", "Hjet m;Hjet m [GeV]", 40, 0, 200 ), directory ); 
  Book( TH1F( "hjet_mpruned", "Hjet pruned m;Hjet pruned m [GeV]", 40, 0, 200 ), directory ); 
  Book( TH1F( "hjet_tau1", "Hjet #tau_{1};Hjet #tau_{1}", 50, 0, 1 ), directory ); 
  Book( TH1F( "hjet_tau2", "Hjet #tau_{2};Hjet #tau_{2}", 50, 0, 1 ), directory ); 
  Book( TH1F( "hjet_tau3", "Hjet #tau_{3};Hjet #tau_{3}", 50, 0, 1 ), directory ); 
  Book( TH1F( "hjet_tau21", "Hjet #tau_{21};Hjet #tau_{21}", 50, 0, 1 ), directory ); 
  Book( TH1F( "hjet_tau31", "Hjet #tau_{31};Hjet #tau_{31}", 50, 0, 1 ), directory ); 
  Book( TH1F( "hjet_tau32", "Hjet #tau_{32};Hjet #tau_{32}", 50, 0, 1 ), directory ); 
  Book( TH1F( "hjet_nSubjets", "Hjet N subjets;Hjet N subjets", 10, -.5, 9.5 ), directory ); 
  Book( TH1F( "hjet_nTaggedSubjets", "Hjet N tagged subjets;Hjet N tagged subjets", 10, -.5, 9.5 ), directory ); 
  Book( TH1F( "hjet_subjet0_csv", "Hjet subjet 0 CSV;Hjet subjet0 CSV", 100, -1, 1 ), directory ); 
  Book( TH1F( "hjet_subjet1_csv", "Hjet subjet 1 CSV;Hjet subjet1 CSV", 100, -1, 1 ), directory ); 

  Book( TH1F( "jets_deta", "jets #Delta #eta;jets #Delta #eta", 50, 0, 5 ), directory ); 
  Book( TH1F( "jets_dphi", "jets #Delta #phi;jets #Delta #phi", 50, 0, 6.3 ), directory ); 
  Book( TH1F( "jets_dr", "jets #Delta R;jets #Delta R", 50, 0, 5 ), directory ); 

  Book( TH1F( "dijet_pt", "dijet p_{T};dijet p_{T} [GeV]", 100, 0, 1000 ), directory ); 
  Book( TH1F( "dijet_eta", "dijet #eta;dijet #eta", 50, -2.5, 2.5 ), directory ); 
  Book( TH1F( "dijet_phi", "dijet #phi;Vdijet #phi", 50, -3.15, 3.15 ), directory ); 
  Book( TH1F( "dijet_m", "dijet m;dijet m [GeV]", 400, 0, 4000 ), directory );
  Book( TH1F( "dijet_template_m", "dijet m;dijet m [GeV]", 7000, 0, 7000 ), directory ); 
  
}



void WHAnalysis::fillHistograms( const TString& directory, const DESY::Jet& vectorJet, const DESY::Jet& higgsJet, const TLorentzVector& diJet, const double& vJet_tau21, const double& vJet_tau31, const double& vJet_tau32, const int& vJet_nTaggedSubjets, const double& vJet_subjet0_csv, const double& vJet_subjet1_csv, const double& hJet_tau21, const double& hJet_tau31, const double& hJet_tau32, const int& hJet_nTaggedSubjets, const double& hJet_subjet0_csv, const double& hJet_subjet1_csv, const double& deta, const double& dphi, const double& dr ) {
  
  // fill all histograms
  Hist( "vjet_pt", directory )->Fill( vectorJet.pt() , b_weight);
  Hist( "vjet_eta", directory )->Fill( vectorJet.eta() , b_weight);
  Hist( "vjet_phi", directory )->Fill( vectorJet.phi() , b_weight);
  Hist( "vjet_m", directory )->Fill( vectorJet.m() , b_weight);
  Hist( "vjet_mpruned", directory )->Fill( vectorJet.pruned_massCorr() , b_weight);
  Hist( "vjet_tau1", directory )->Fill( vectorJet.tau1() , b_weight);
  Hist( "vjet_tau2", directory )->Fill( vectorJet.tau2() , b_weight);
  Hist( "vjet_tau3", directory )->Fill( vectorJet.tau3() , b_weight);
  Hist( "vjet_tau21", directory )->Fill( vJet_tau21 , b_weight);
  Hist( "vjet_tau31", directory )->Fill( vJet_tau31 , b_weight);
  Hist( "vjet_tau32", directory )->Fill( vJet_tau32 , b_weight);
  Hist( "vjet_nSubjets", directory )->Fill( vectorJet.subjet_pruned_N() , b_weight);
  Hist( "vjet_nTaggedSubjets", directory )->Fill( vJet_nTaggedSubjets , b_weight);
  Hist( "vjet_subjet0_csv", directory )->Fill( vJet_subjet0_csv , b_weight);
  Hist( "vjet_subjet1_csv", directory )->Fill( vJet_subjet1_csv , b_weight);

  Hist( "hjet_pt", directory )->Fill( higgsJet.pt() , b_weight);
  Hist( "hjet_eta", directory )->Fill( higgsJet.eta() , b_weight);
  Hist( "hjet_phi", directory )->Fill( higgsJet.phi() , b_weight);
  Hist( "hjet_m", directory )->Fill( higgsJet.m() , b_weight);
  Hist( "hjet_mpruned", directory )->Fill( higgsJet.pruned_massCorr() , b_weight);
  Hist( "hjet_tau1", directory )->Fill( higgsJet.tau1() , b_weight);
  Hist( "hjet_tau2", directory )->Fill( higgsJet.tau2() , b_weight);
  Hist( "hjet_tau3", directory )->Fill( higgsJet.tau3() , b_weight);
  Hist( "hjet_tau21", directory )->Fill( hJet_tau21 , b_weight);
  Hist( "hjet_tau31", directory )->Fill( hJet_tau31 , b_weight);
  Hist( "hjet_tau32", directory )->Fill( hJet_tau32 , b_weight);
  Hist( "hjet_nSubjets", directory )->Fill( higgsJet.subjet_pruned_N() , b_weight);
  Hist( "hjet_nTaggedSubjets", directory )->Fill( hJet_nTaggedSubjets , b_weight);
  Hist( "hjet_subjet0_csv", directory )->Fill( hJet_subjet0_csv , b_weight);
  Hist( "hjet_subjet1_csv", directory )->Fill( hJet_subjet1_csv , b_weight);

  Hist( "jets_deta", directory )->Fill( deta , b_weight);
  Hist( "jets_dphi", directory )->Fill( dphi , b_weight);
  Hist( "jets_dr", directory )->Fill( dr , b_weight);

  Hist( "dijet_pt", directory )->Fill( diJet.Pt() , b_weight);
  Hist( "dijet_eta", directory )->Fill( diJet.Eta() , b_weight);
  Hist( "dijet_phi", directory )->Fill( diJet.Phi() , b_weight);
  Hist( "dijet_m", directory )->Fill( diJet.M() , b_weight);
  Hist( "dijet_template_m", directory )->Fill( diJet.M() , b_weight);
  
}


