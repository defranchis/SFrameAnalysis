
/*

  - JER applied (hybrid method: https://twiki.cern.ch/twiki/bin/viewauth/CMS/JetResolution )
  - do_* variables to estimate systematics
  - 0 = central value, 1 = up, -1 = down

 */


#include "../include/VHTausAnalysis.h"
#include "../GoodRunsLists/include/TGoodRunsListReader.h"
#include <TMath.h>
#include "TRandom3.h"



ClassImp( VHTausAnalysis );

// define cut names

const std::string VHTausAnalysis::kCutName[ VHTausAnalysis::kNumCuts ] = {

    "BeforeCuts",            // C0
    "JSON",                  // C1
    "Trigger",               // C2
    "MetFilters",            // C3
    "Met",                   // C4
    "Jet",                   // C5
    "Tau",                   // C6

};


VHTausAnalysis::VHTausAnalysis()
    : SCycleBase()
    , m_jetAK8( this )
    , m_jetAK4( this )
    , m_genJetAK8( this )
    , m_genJetAK4( this )
    , m_eventInfo( this )
    , m_missingEt( this )
    , m_genParticle( this )
    , m_muon( this )
    , m_pileupReweightingTool( this )

{

    m_logger << INFO << "Hello!" << SLogger::endmsg;
    SetLogName( GetName() );
   
    DeclareProperty( "RecoTreeName",             m_recoTreeName             = "physics" );
    DeclareProperty( "OutputTreeName",           m_outputTreeName           = "analysis" );
    DeclareProperty( "JetAK8Name",               m_jetAK8Name               = "jetAK8" );
    DeclareProperty( "JetAK4Name",               m_jetAK4Name               = "jetAK4" );
    DeclareProperty( "GenJetAK8Name",            m_genJetAK8Name            = "genJetAK8" );
    DeclareProperty( "GenJetAK4Name",            m_genJetAK4Name            = "genJetAK4" );
    DeclareProperty( "ElectronName",             m_electronName             = "el" );
    DeclareProperty( "MuonName",                 m_muonName                 = "mu" );
    DeclareProperty( "TauName",                  m_tauName                  = "tau" );
    DeclareProperty( "MissingEtName",            m_missingEtName            = "MET" );
   
    DeclareProperty( "IsData",                   m_isData                   = false );
    DeclareProperty( "IsSignal",                 m_isSignal                 = true );
    DeclareProperty( "IsTTbar",                  m_isTTbar                  = false );
    DeclareProperty( "IsTTbarUnbinned",          m_isTTbarUnbinned          = false );
    DeclareProperty( "IsQCD",                    m_isQCD                    = false );
    DeclareProperty( "IsRunH",                   m_isRunH                   = false );



    /////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////// SELECTIONS ////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////


    // DeclareProperty( "METCut",                   m_metCut             = 50. );
   
    DeclareProperty( "jetPtCut",                 m_jetPtCut           = 350. );
    DeclareProperty( "jetEtaCut",                m_jetEtaCut          = 2.4  );
    DeclareProperty( "jetHbbtagMin",             m_jetHbbtagMin       = 0.6  );


    DeclareProperty( "jetMassMin",               m_jetMassMin         = 50.  );
    DeclareProperty( "jetMassMax",               m_jetMassMax         = 65. );


    DeclareProperty( "mumuMassMin",            m_mumuMassMin         = 80. );
    DeclareProperty( "mumuMassMax",            m_mumuMassMax         = 100.);

    DeclareProperty( "MuonPtCut",                m_muonPtCut             = 10. );
    DeclareProperty( "MuonEtaCut",               m_muonEtaCut            = 2.4 );
   


    /////////////////////////////////////////////////////////////////////////////////

    DeclareProperty( "JSONName", m_jsonName = std::string (std::getenv("SFRAME_DIR")) + 
                     "/../GoodRunsLists/JSON/Cert_271036-283059_13TeV_PromptReco_Collisions16_JSON_NoL1T.txt" );

    trig_turnon_eff = new TFile("~/HH/8_X_Framework/Analysis/SFrameAnalysis/VHTausResonances/trigger_turnon/HLT_HT800or900_wMitigation.root");
    trig_eff = (TEfficiency*) trig_turnon_eff->GetObjectChecked("trigger_eff_vs_HT/eff_HT_thick","TEfficiency");
    trig_eff_coarse = (TEfficiency*) trig_turnon_eff->GetObjectChecked("trigger_eff_vs_HT/eff_HT_insanely_coarse","TEfficiency");



}


VHTausAnalysis::~VHTausAnalysis() {

    m_logger << INFO << "Tschoe!" << SLogger::endmsg;

}



void VHTausAnalysis::BeginCycle() throw( SError ) {

    m_logger << INFO << "Hello to you!" << SLogger::endmsg;
  
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
  
        AddConfigObject( &m_grl );
		
    }
  
    m_triggerNames.clear();
	
    m_triggerNames.push_back("PFHT900_v") ;
    m_triggerNames.push_back("PFHT800_v") ;
    m_triggerNames.push_back("CaloJet500_NoJetID") ;


    m_catNames.clear();

    m_catNames.push_back("postHbbCut_NoWindow");
    m_catNames.push_back("postHbbCut_SR");
    m_catNames.push_back("postHbbCut_bveto");

    m_catNames.push_back("preHbbCut_NoWindow");
    m_catNames.push_back("preHbbCut_SR");
    m_catNames.push_back("preHbbCut_bveto");


    return;

}


void VHTausAnalysis::EndCycle() throw( SError ) {

    return;

}




void VHTausAnalysis::BeginInputData( const SInputData& id ) throw( SError ) {

  
    if (!m_isData) m_pileupReweightingTool.BeginInputData( id );
  
    if (m_isData) {
        TObject* grl;
        if( ! ( grl = GetConfigObject( "MyGoodRunsList" ) ) ) {
            m_logger << FATAL << "Can't access the GRL!" << SLogger::endmsg;
            throw SError( "Can't access the GRL!", SError::SkipCycle );
        }
        m_grl = *( dynamic_cast< Root::TGoodRunsList* >( grl ) );
    }
  

    DeclareVariable( b_pass,"pass" );

    Book( TH1F( "Events", "Events;weight", 10, -.5, 9.5 ) );
    Book( TH1F( "SumEvents", "SumEvents;weight", 10, -.5, 9.5 ) );
    Book( TH1F( "METFilters", "METFilters", 20, 0.5, 20.5 ));

    for (unsigned int s=0;s<m_catNames.size();++s)
    {
        TString directory = m_catNames[s].c_str();
        bookHistograms(directory);
    }


    return;

}


//MD prints out some info at the end of input data

void VHTausAnalysis::EndInputData( const SInputData& ) throw( SError ) {

    return;

}



void VHTausAnalysis::BeginInputFile( const SInputData& ) throw( SError ) {

    m_logger << INFO << "Connecting input variables" << SLogger::endmsg;

    if (m_isData){
        m_eventInfo.ConnectVariables( m_recoTreeName.c_str(), Ntuple::EventInfoTrigger|Ntuple::EventInfoMETFilters, "" );
    }

    else{
        m_eventInfo.ConnectVariables( m_recoTreeName.c_str(), 
                                      Ntuple::EventInfoBasic|Ntuple::EventInfoTrigger|Ntuple::EventInfoMETFilters|Ntuple::EventInfoTruth, "" );
    }
    m_missingEt.ConnectVariables( m_recoTreeName.c_str(), Ntuple::MissingEtBasic, (m_missingEtName + "_").c_str() );

    if (m_isData || m_isQCD){
        m_jetAK8.ConnectVariables( m_recoTreeName.c_str(), 
                                   Ntuple::JetBasic|Ntuple::JetAnalysis|Ntuple::JetSubstructure, (m_jetAK8Name + "_").c_str() );
        m_jetAK4.ConnectVariables( m_recoTreeName.c_str(), Ntuple::JetBasic|Ntuple::JetAnalysis, (m_jetAK4Name + "_").c_str() );
    }

    else{

        m_jetAK8.ConnectVariables( m_recoTreeName.c_str(), 
                                   Ntuple::JetBasic|Ntuple::JetAnalysis|Ntuple::JetSubstructure|Ntuple::JetTruth, (m_jetAK8Name + "_").c_str() );
        m_jetAK4.ConnectVariables( m_recoTreeName.c_str(), Ntuple::JetBasic|Ntuple::JetAnalysis|Ntuple::JetTruth, (m_jetAK4Name + "_").c_str() );

        m_genJetAK8.ConnectVariables( m_recoTreeName.c_str(), false, (m_genJetAK8Name + "_").c_str() );
        m_genJetAK4.ConnectVariables( m_recoTreeName.c_str(), false, (m_genJetAK4Name + "_").c_str() );

    }



    m_muon.ConnectVariables( m_recoTreeName.c_str(),
                             Ntuple::MuonBasic|Ntuple::MuonID|Ntuple::MuonIsolation| Ntuple::MuonBoostedIsolation, (m_muonName + "_").c_str() );

    m_logger << INFO << "Connecting input variables completed" << SLogger::endmsg;

    return;

}


void VHTausAnalysis::ExecuteEvent( const SInputData&, Double_t ) throw( SError ) 
{

    using namespace std;

    clearBranches();
  
    b_eventNumber = m_eventInfo.eventNumber;
    b_runNumber = m_eventInfo.runNumber;
    b_lumiBlock = m_eventInfo.lumiBlock;

  
    if (m_isData){

        if (! isGoodEvent(m_eventInfo.runNumber, m_eventInfo.lumiBlock))
            return;

        if (!passTrigger()) 
            return;
    }


    if (!passMETFilters()) 
        return;

    createTemplates();



    // compute event HT
    for ( unsigned int i = 0; i < jetAK4_TLV.size(); ++i ) 
    {
        double pt = jetAK4_TLV[i].Pt();
        double eta = jetAK4_TLV[i].Eta();

        if ( (pt < 40.) || (fabs(eta) > 3.) )
            continue;

        event_HT += pt;

    }

    if (event_HT < 800) return;

    // select met
    // if (met_et < m_metCut || TMath::IsNaN(met_et))
    //     return;


////////////////////////////////////////////////////////////////////////////////////////////


    // muons
    if (muon_TLV.size() < 2) 
        return;

    // if (muon_TLV.size() != 2) 
    //     return;

    // if (muon_charge.at(0) * muon_charge.at(1) > 0.)
    //     return;

    bool muonMassWindow = false;

    double mumuMass = (muon_TLV.at(0)+muon_TLV.at(1)).M();
    if ( (mumuMass > m_mumuMassMin) && (mumuMass < m_mumuMassMax) )
        muonMassWindow = true;

    if ( !muonMassWindow ) return;

    // select AK08 jet

    for (unsigned int i = 0; i < jetAK8_TLV.size(); ++i ) 
    {
        double pt = jetAK8_TLV[i].Pt();
        double eta = jetAK8_TLV[i].Eta();

        bool isJetID = jetAK8_IDTight[i];
        bool passCuts =  ( pt > m_jetPtCut ) && ( fabs(eta) < m_jetEtaCut );

        if (passCuts && isJetID)
        {
            goodAK8_TLV.push_back(jetAK8_TLV[i]);
            goodAK8_Hbbtag.push_back(jetAK8_Hbbtag[i]);
            goodAK8_pruned_mass.push_back(jetAK8_pruned_mass[i]);
        }

    

    }//end loop on jets


    if (goodAK8_TLV.size() == 0) 
        return;



    for (unsigned int i = 0; i < goodAK8_TLV.size(); ++i) 
    {

        if ( (goodAK8_TLV[i].Pt() > goodAK08TLV_sel.Pt()) )
        {
            goodAK08TLV_sel = goodAK8_TLV[i];
            jetPrunedMass = goodAK8_pruned_mass[i]; 
            jetHbbtag = goodAK8_Hbbtag[i];
        }

    }


    if (goodAK08TLV_sel.Pt() < 10) //i.e. pt = 0
        return;


    bool jetMassWindow = (jetPrunedMass > m_jetMassMin) && (jetPrunedMass < m_jetMassMax);
    bool isHbbtag = (jetHbbtag > m_jetHbbtagMin);

    if ( ! jetMassWindow ) 
        return;

    // cout<<"passed jet"<<endl;

    if (isHbbtag)
        is_btag_SR = true;


////////////////////////////////////////////////////////////////////////////////////////////


    b_pass = true;

    if (!m_isData) b_weight = getEventWeight();
    if (!m_isData) applyTurnOn ();


    //fromhere

    int iCat = -1;
    if ( is_btag_SR ) iCat = 1;
    else iCat = 2;

    
    if ( jetHbbtag > -0.5 )
    {
        fillHistograms(m_catNames[ 0 ]);
        fillHistograms(m_catNames[ iCat ]);

    }

    int shiftCat = 3 ;

    fillHistograms(m_catNames[ 0 + shiftCat ]);
    fillHistograms(m_catNames[ iCat + shiftCat ]);
    
                
    return;


} //end function


bool VHTausAnalysis::isGoodEvent(int runNumber, int lumiSection) {
  
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


bool VHTausAnalysis::passTrigger() {
  
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



bool VHTausAnalysis::passMETFilters() {
  
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

    // if (m_isData){

    if( !(m_eventInfo.passFilter_EEBadSc) ) {
        passMetFilters = false;
        m_logger << VERBOSE << "passFilter_EEBadSc" << SLogger::endmsg;
        Hist( "METFilters" )->Fill(5);
    }

    // }
 
    if( !(m_eventInfo.passFilter_globalTightHalo2016) ){
        passMetFilters = false;
        m_logger << VERBOSE << "passFilter_EEBadSc" << SLogger::endmsg;
        Hist( "METFilters" )->Fill(6);
    }

    if( !(m_eventInfo.passFilter_chargedHadronTrackResolution) ){
        passMetFilters = false;
        m_logger << VERBOSE << "passFilter_EEBadSc" << SLogger::endmsg;
        Hist( "METFilters" )->Fill(7);
    }

    if( !(m_eventInfo.passFilter_muonBadTrack) ) {        
        passMetFilters = false;
        m_logger << VERBOSE << "passFilter_EEBadSc" << SLogger::endmsg;
        Hist( "METFilters" )->Fill(8);
    } 

    return passMetFilters;
  
}


double VHTausAnalysis::getEventWeight() {
  
    double weight = 1.;
    for( unsigned int v = 0; v < (m_eventInfo.actualIntPerXing)->size(); ++v ){
    
        if ( (*m_eventInfo.bunchCrossing)[v] == 0 ) {
            b_weightPU = m_pileupReweightingTool.getPileUpweight( (*m_eventInfo.actualIntPerXing)[v] );
            m_logger << VERBOSE << "Weight: " << b_weightPU << " for true: " << (*m_eventInfo.actualIntPerXing)[v] << SLogger::endmsg;
     
            break;
        }
    }

    b_weightGen = m_eventInfo.genEventWeight;
    weight *= b_weightPU*b_weightGen;
  
    return weight;
  
}


void VHTausAnalysis::clearBranches() {

    b_weight = 1.;
    b_weightGen = 1.;
    b_weightPU = 1.;
  
    b_runNumber = -99;
    b_eventNumber = -99;
    b_lumiBlock = -99;

    event_HT = 0.;
    b_pass = false;
    is_btag_SR = false;

    // TLV templates
    jetAK4_TLV.clear();
    jetAK8_TLV.clear();

    muon_TLV.clear() ;
    muon_rawIso.clear() ;
    muon_charge.clear() ;

    // jet variables (templates)
    jetAK8_Hbbtag.clear() ;
    jetAK8_pruned_mass.clear() ;
    jetAK8_IDTight.clear() ;


    // preselected TLV
    goodAK8_TLV.clear();

    // preselected variables
    goodAK8_Hbbtag.clear() ;
    goodAK8_pruned_mass.clear() ;

    TLorentzVector null_TLV (0,0,0,0);


    // selected TLV
    goodAK08TLV_sel = null_TLV ;


    // selected variables
    jetPrunedMass = 0. ;
    jetHbbtag = -2.;

    // MET
    met_et = 0 ;
    met_px = 0 ;
    met_py = 0 ;


}

void VHTausAnalysis::fillCutflow( const std::string histName, const std::string dirName, const TBits& cutmap, const Double_t weight ) {


    for( UInt_t i = 0; i < cutmap.GetNbits(); ++i ) {
        // m_logger << INFO << i << ":\t" << cutmap.TestBitNumber( i ) << SLogger::endmsg;
        if( cutmap.TestBitNumber( i ) ) {
            Hist( histName.c_str(), dirName.c_str() )->Fill( i+1, weight );
        } else {
            break;
        }
    }
}

//MD creates all the histograms

void VHTausAnalysis::bookHistograms( const TString& directory ) {

    Book ( TH1F("HT","event HT;HT [GeV]",100,0,5000), directory);
    Book ( TH1F("n_PV","number of primary vertices;n. of Primary Vertices",51,-.5,50.5), directory);

    Book ( TH1F("jet_mass","jet pruned mass (with JEC);jet pruned mass [GeV]",50,0,250), directory);
    Book ( TH1F("jet_pT","jet transverse momentum;jet p_{T} [GeV]",30,0,3000), directory);
    Book ( TH1F("jet_eta","jet pseudorapidity ;jet #eta",20,-2.4,2.4), directory);
    Book ( TH1F("jet_Hbbtag","double b-tagger output;bb-tagger output",20,-1,1), directory);

    Book ( TH1F("muon1_pT","leading muon pT;leading muon p_{T} [GeV]",30,0,2000), directory);
    Book ( TH1F("muon1_eta","leading muon pseudorapidity ;leading muon #eta",20,-2.4,2.4), directory);
    Book ( TH1F("muon2_pT","sub-leading muon pT;sub-leading muon p_{T} [GeV]",30,0,700), directory);
    Book ( TH1F("muon2_eta","sub-leading muon pseudorapidity ;sub-leading muon #eta",20,-2.4,2.4), directory);

    Book ( TH1F("muon1_iso","leading muon isolation;leading muon isolation",22,-1.1,1.1), directory);
    Book ( TH1F("muon2_iso","sub-leading muon isolation;sub-leading muon isolation",22,-1.1,1.1), directory);

    Book ( TH1F("mumu_mass","mu pair invariant mass (coll. nu);mass (#mu,#mu) [GeV]",50,0,250), directory);
    Book ( TH1F("sum_iso","sum of muon isolations;sum of muon isolations",22,-2.2,2.2), directory);

    Book ( TH1F("MET_ET","missing energy ET;missing energy E_{T} [GeV]",40,0,2000), directory);
    Book ( TH1F("MET_px","missing energy px;missing momentum p_{x} [GeV]",20,0,1000), directory);
    Book ( TH1F("MET_py","missing energy py;missing momentum p_{y} [GeV]",20,0,1000), directory);

    Book ( TH1F("resonance_mass","tautau-jet invariant mass (coll. nu);mass (#tau#tau,jet) [GeV]",100,0,5000), directory);


}






void VHTausAnalysis::fillHistograms( const TString& directory )

{


    Hist( "HT", directory )->Fill( event_HT , b_weight );
    Hist( "n_PV", directory )->Fill( m_eventInfo.PV_N , b_weight );
    
    Hist( "jet_mass", directory )->Fill( jetPrunedMass , b_weight );
    Hist( "jet_pT", directory )->Fill( goodAK08TLV_sel.Pt() , b_weight );
    Hist( "jet_eta", directory )->Fill( goodAK08TLV_sel.Eta() , b_weight );
    Hist( "jet_Hbbtag", directory )->Fill( jetHbbtag , b_weight );

    Hist( "muon1_pT", directory )->Fill( muon_TLV.at(0).Pt() , b_weight );
    Hist( "muon1_eta", directory )->Fill( muon_TLV.at(0).Eta() , b_weight );
    Hist( "muon1_iso", directory )->Fill( muon_rawIso.at(0) , b_weight );
    
    Hist( "muon2_pT", directory )->Fill( muon_TLV.at(1).Pt() , b_weight );
    Hist( "muon2_eta", directory )->Fill( muon_TLV.at(1).Eta() , b_weight );
    Hist( "muon2_iso", directory )->Fill( muon_rawIso.at(1) , b_weight );

    Hist( "mumu_mass", directory )->Fill( (muon_TLV.at(0) + muon_TLV.at(1)).M() , b_weight );
    Hist( "sum_iso", directory )->Fill( muon_rawIso.at(0) + muon_rawIso.at(1) , b_weight );

    Hist( "MET_ET", directory )->Fill( met_et , b_weight );
    Hist( "MET_px", directory )->Fill( met_px , b_weight );
    Hist( "MET_py", directory )->Fill( met_py , b_weight );

    Hist( "resonance_mass", directory )->Fill( (goodAK08TLV_sel + muon_TLV.at(0) + muon_TLV.at(1)).M() , b_weight );

}


void VHTausAnalysis::applyTurnOn (){

    double trigger_factor = 1. ;
    if ( event_HT > 1500. ) trigger_factor = trig_eff_coarse->GetEfficiency(2);
    else trigger_factor = trig_eff->GetEfficiency(trig_eff->FindFixBin(event_HT));

    b_weight *= trigger_factor;

}



void VHTausAnalysis::createTemplates (){

    using namespace std;

    // met
    UZH::MissingEt goodMet ( &m_missingEt, 0 );
    met_px = goodMet.et() * cos( goodMet.phi() ) ;
    met_py = goodMet.et() * sin( goodMet.phi() ) ;

    // cout<<"original MET: px = "<<met_px<<"\tpy = "<<met_py<<endl;

    //jetAK4
    for ( int i = 0; i < (m_jetAK4.N); ++i )     
    {
        UZH::Jet recoJet( &m_jetAK4, i );

        TLorentzVector originalJetTLV = * recoJet.getTLV();
        TLorentzVector recoJetTLV = * recoJet.getTLV();


        if (!m_isData && !m_isQCD)
        // if ( !m_isData )
        {

            double min_dist = -1.;
            double pt_gen = -1.;

            for ( int j = 0; j < (m_genJetAK4.N); ++j )
            {
                UZH::Jet genJet( &m_genJetAK4, j );
                TLorentzVector genJetTLV = * genJet.getTLV();

                double gen_reco_dist = genJetTLV.DeltaR(recoJetTLV);

                if (j==0) 
                {
                    min_dist = gen_reco_dist;
                    pt_gen = genJetTLV.Pt();
                }

                else 
                {
                    if (gen_reco_dist < min_dist ) 
                    {
                        min_dist = gen_reco_dist;
                        pt_gen = genJetTLV.Pt();
                    }
                }

            }// gen AK4 loop


            double JER_SF = recoJet.jer_sf();
            double jer_sigma_pt = recoJet.jer_sigma_pt() * recoJetTLV.Pt();


            if ( (min_dist > 0) && (min_dist < 0.4/2.) )
            {
                double new_pt = pt_gen + JER_SF*(recoJetTLV.Pt()-pt_gen);
                if (new_pt < 0) new_pt = 0. ;

                recoJetTLV *= new_pt / recoJetTLV.Pt();

            } //scaling method

            else
            {

                double new_pt = rand.Gaus( recoJetTLV.Pt(), jer_sigma_pt * pow( JER_SF*JER_SF -1., .5 ) );
                recoJetTLV *= new_pt / recoJetTLV.Pt();

            }// smearing method

        }// not data
        
        jetAK4_TLV.push_back(recoJetTLV);

        met_px += originalJetTLV.Px() - recoJetTLV.Px() ;
        met_py += originalJetTLV.Py() - recoJetTLV.Py() ;

    }// reco AK4 loop


    met_et = pow( met_px*met_px + met_py*met_py , .5);

    //jetAK8
    for ( int i = 0; i < (m_jetAK8.N); ++i )     
    {
        UZH::Jet recoJet( &m_jetAK8, i );
        TLorentzVector recoJetTLV = * recoJet.getTLV();
        double pruned_mass = recoJet.pruned_massCorr();

        if (!m_isData && !m_isQCD)
        // if ( !m_isData )
        {

            double min_dist = -1.;
            double pt_gen = -1.;

            for ( int j = 0; j < (m_genJetAK8.N); ++j )
            {
                UZH::Jet genJet( &m_genJetAK8, j );
                TLorentzVector genJetTLV = * genJet.getTLV();

                double gen_reco_dist = genJetTLV.DeltaR(recoJetTLV);
                if (j==0) {
                    min_dist = gen_reco_dist;
                    pt_gen = genJetTLV.Pt();
                }

                else {
                    if (gen_reco_dist < min_dist ) {
                        min_dist = gen_reco_dist;
                        pt_gen = genJetTLV.Pt();
                    }
                }

            }// gen AK8 loop

            double JER_SF = recoJet.jer_sf();
            double jer_sigma_pt = recoJet.jer_sigma_pt() * recoJetTLV.Pt();

            if ( (min_dist > 0) && (min_dist < 0.8/2.) )
            {

                double new_pt = pt_gen + JER_SF*(recoJetTLV.Pt()-pt_gen);
                if (new_pt < 0) new_pt = 0. ;

                recoJetTLV *= new_pt / recoJetTLV.Pt();

            } //scaling method

            else
            {

                double new_pt = rand.Gaus( recoJetTLV.Pt(), jer_sigma_pt * pow( JER_SF*JER_SF -1., .5 ) );
                recoJetTLV *= new_pt / recoJetTLV.Pt();

            }// smearing method

        }// not data
        

        jetAK8_TLV.push_back(recoJetTLV);

        jetAK8_Hbbtag.push_back(recoJet.Hbbtag());
        jetAK8_pruned_mass.push_back(pruned_mass);
        jetAK8_IDTight.push_back(recoJet.IDTight());


    }// reco AK8 loop

    
    //muons
    for ( int i = 0; i<(m_muon.N); ++i )
    {

        UZH::Muon mymuon( &m_muon, i );
        
        bool isMuonID = mymuon.isTightMuon();
        // bool isMuonID = mymuon.isLooseMuon();
        bool isMuonIso = ( mymuon.SemileptonicPFIso() / mymuon.pt() < 0.25 ) ;
        bool passCuts = ( mymuon.pt() > m_muonPtCut) && (fabs(mymuon.eta()) < m_muonEtaCut);

        if ( !passCuts  ) continue;
        if ( !isMuonID  ) continue;
        if ( !isMuonIso ) continue;
        
        TLorentzVector mymuon_TLV = * mymuon.getTLV();

        muon_TLV.push_back(mymuon_TLV);
        muon_rawIso.push_back( mymuon.SemileptonicPFIso() / mymuon.pt() );
        muon_charge.push_back(mymuon.charge());        

    }//end loop on muons


}//end function


