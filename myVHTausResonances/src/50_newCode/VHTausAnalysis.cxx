
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
    , m_jetAK8Puppi( this )
    , m_genJetAK8( this )
    , m_genJetAK4( this )
    , m_eventInfo( this )
    , m_tau( this )
    , m_missingEt( this )
    , m_genParticle( this )
    , m_electron( this )
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
    DeclareProperty( "IsDY",                     m_isDY                     = false );
    DeclareProperty( "IsTTbarUnbinned",          m_isTTbarUnbinned          = false );
    DeclareProperty( "IsQCD",                    m_isQCD                    = false );
    DeclareProperty( "IsRunH",                   m_isRunH                   = false );


    /////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////// SELECTIONS ////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////


    // DeclareProperty( "METCut",                   m_metCut             = 50. );
    DeclareProperty( "METCut",                   m_metCut             = 100. );
   
    DeclareProperty( "jetPtCut",                 m_jetPtCut           = 350. );
    DeclareProperty( "jetEtaCut",                m_jetEtaCut          = 2.4  );

    DeclareProperty( "jetHbbtagMin",             m_jetHbbtagMin       = 0.6  );
    // DeclareProperty( "jetHbbtagMin",             m_jetHbbtagMin       = 0.8  );
    // DeclareProperty( "jetHbbtagMin",             m_jetHbbtagMin       = 0.3  );

    DeclareProperty( "jetMetDeltaPhiMin",        m_jetMetDeltaPhiMin  = TMath::Pi()/2.);
    DeclareProperty( "jetMassMin",               m_jetMassMin         = 105.  );
    DeclareProperty( "jetMassMax",               m_jetMassMax         = 135. );

    DeclareProperty( "tauPtCut",                 m_tauPtCut              = 20. );
    DeclareProperty( "tauEtaCut",                m_tauEtaCut             = 2.4 );
    DeclareProperty( "tautauPtCut",              m_tautauPtCut           = 250.);
    DeclareProperty( "tautauDRMax",              m_tautauDRMax           = 1.  );
    DeclareProperty( "tautauMetDeltaPhiMax",     m_tautauMetDeltaPhiMax  = TMath::Pi()/2.);
    DeclareProperty( "tautauMassMin",            m_tautauMassMin         = 75. );
    DeclareProperty( "tautauMassMax",            m_tautauMassMax         = 160.);

    DeclareProperty( "tautauJetDRMin",           m_tautauJetDRMin        = 1.  );
    DeclareProperty( "tautauJetMassMin",         m_tautauJetMassMin      = 800.);


    DeclareProperty( "ElectronPtCut",            m_electronPtCut         = 10. );
    DeclareProperty( "ElectronEtaCut",           m_electronEtaCut        = 2.5 );
    DeclareProperty( "MuonPtCut",                m_muonPtCut             = 10. );
    DeclareProperty( "MuonEtaCut",               m_muonEtaCut            = 2.4 );
   
    DeclareProperty( "PUPPIJEC", m_PUPPIJEC_  = std::string (std::getenv("SFRAME_DIR")) + "/../VHTausResonances/weightSD/puppiCorr.root" );


    /////////////////////////////////////////////////////////////////////////////////


    DeclareProperty( "JSONName", m_jsonName = std::string (std::getenv("SFRAME_DIR")) + 
                     "/../GoodRunsLists/JSON/Cert_271036-283059_13TeV_PromptReco_Collisions16_JSON_NoL1T.txt" );

    // trig_turnon_eff_HT800 = new TFile("~/HH/8_X_Framework/Analysis/SFrameAnalysis/VHTausResonances/trigger_turnon/HLT_HT800_wMitigation.root");
    // trig_eff_HT800 = (TEfficiency*) trig_turnon_eff_HT800->GetObjectChecked("trigger_eff_vs_HT/eff_HT_thick","TEfficiency");

    // trig_turnon_eff_HT900 = new TFile("~/HH/8_X_Framework/Analysis/SFrameAnalysis/VHTausResonances/trigger_turnon/HLT_HT900_wMitigation.root");
    // trig_eff_HT900 = (TEfficiency*) trig_turnon_eff_HT900->GetObjectChecked("trigger_eff_vs_HT/eff_HT_thick","TEfficiency");

    trig_turnon_eff = new TFile("~/HH/8_X_Framework/Analysis/SFrameAnalysis/VHTausResonances/trigger_turnon/trigger_newDef_HT800or900.root");
    trig_eff = (TEfficiency*) trig_turnon_eff->GetObjectChecked("trigger_eff_vs_HT/eff_HT_thick","TEfficiency");



    //rand.SetSeed();
    

    /*********************/

    do_Hbb_tag_SF_shift = 0;
    do_Hbb_mistag_SF_shift = 0;
    do_JEC_AK4_shift = 0;
    do_JEC_AK8_shift = 0;

    // do_softdrop_shift = 0;

    do_JER_AK4_shift = 0;
    do_JER_AK8_shift = 0;

    do_turnon_shift = 0;
    do_tau_shift = 0;
    do_tauID_shift = 0;
    do_pdf_shift = 0;

    /*********************/


  initPuppiSDWeight();
 
  if ( !m_isData )
      initPuppiSD_ScaleAndRes_Weight();

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
	


    // if (m_isRunH) m_triggerNames.push_back("PFHT900_v") ;
    // else m_triggerNames.push_back("PFHT800_v") ;

    m_triggerNames.push_back("PFHT900_v") ;
    m_triggerNames.push_back("PFHT800_v") ;

    m_triggerNames.push_back("CaloJet500_NoJetID") ;


    m_catNames.clear();
    m_catNames.push_back("tautau_NoWindow");

    m_catNames.push_back("0_SR");
    m_catNames.push_back("1_bveto");
    m_catNames.push_back("2_tauveto");
    m_catNames.push_back("3_allveto");
    m_catNames.push_back("4_check_noTauIso");
    m_catNames.push_back("5_check_noBtag");
    m_catNames.push_back("6_check_noSR");

    m_catNames.push_back("preHbbCut_NoWindow");
    m_catNames.push_back("preHbbCut_SR");
    m_catNames.push_back("preHbbCut_bveto");
    m_catNames.push_back("preHbbCut_tauveto");
    m_catNames.push_back("preHbbCut_allveto");
    m_catNames.push_back("preHbbCut_check_noTauIso");
    m_catNames.push_back("preHbbCut_check_noBtag");
    m_catNames.push_back("preHbbCut_check_noSR");


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
                                      Ntuple::EventInfoBasic|Ntuple::EventInfoTrigger|Ntuple::EventInfoMETFilters|Ntuple::EventInfoTruth
                                      /*Ntuple::EventInfoPDF*/, "" );
    }

    m_tau.ConnectVariables( m_recoTreeName.c_str(), Ntuple::TauBasic|Ntuple::TauID|Ntuple::TauAdvancedID, (m_tauName + "_").c_str() );
    m_missingEt.ConnectVariables( m_recoTreeName.c_str(), Ntuple::MissingEtBasic, (m_missingEtName + "_").c_str() );

    if (m_isData /*|| m_isQCD*/){
        m_jetAK8.ConnectVariables( m_recoTreeName.c_str(), 
                                   /*Ntuple::JetBasic|*/Ntuple::JetAnalysis|Ntuple::JetSubstructure, (m_jetAK8Name + "_").c_str() );
        m_jetAK4.ConnectVariables( m_recoTreeName.c_str(), /*Ntuple::JetBasic|*/Ntuple::JetAnalysis, (m_jetAK4Name + "_").c_str() );
    }

    else{

        m_jetAK8.ConnectVariables( m_recoTreeName.c_str(), 
                                   /*Ntuple::JetBasic|*/Ntuple::JetAnalysis|Ntuple::JetSubstructure|Ntuple::JetTruth, (m_jetAK8Name + "_").c_str() );
        m_jetAK4.ConnectVariables( m_recoTreeName.c_str(), /*Ntuple::JetBasic|*/Ntuple::JetAnalysis|Ntuple::JetTruth, (m_jetAK4Name + "_").c_str() );

        m_genJetAK8.ConnectVariables( m_recoTreeName.c_str(), Ntuple::GenJetSubstructure , (m_genJetAK8Name + "_").c_str() );
        m_genJetAK4.ConnectVariables( m_recoTreeName.c_str(), false, (m_genJetAK4Name + "_").c_str() );

    }


    m_electron.ConnectVariables( m_recoTreeName.c_str(), 
                                 /*Ntuple::ElectronBasic|Ntuple::ElectronID|*/Ntuple::ElectronBoostedID|Ntuple::ElectronBoostedIsolation,
                                 (m_electronName + "_").c_str() );

    m_muon.ConnectVariables( m_recoTreeName.c_str(),
                             /*Ntuple::MuonBasic|*/Ntuple::MuonID/*|Ntuple::MuonIsolation*/| Ntuple::MuonBoostedIsolation, (m_muonName + "_").c_str() );

    m_jetAK8Puppi.ConnectVariables(  m_recoTreeName.c_str(),  Ntuple::PuppiJetBasic|Ntuple::PuppiJetAnalysis/*|Ntuple::PuppiJetSoftdropSubjets*/,"");


    m_logger << INFO << "Connecting input variables completed" << SLogger::endmsg;

    return;

}


void VHTausAnalysis::ExecuteEvent( const SInputData&, Double_t ) throw( SError ) 
{

    using namespace std;

    // cout<<"I'm in the event loop..."<<endl;

    clearBranches();

    event_lheHT = m_eventInfo.lheHT;
  
    b_eventNumber = m_eventInfo.eventNumber;
    b_runNumber = m_eventInfo.runNumber;
    b_lumiBlock = m_eventInfo.lumiBlock;

  
    if (m_isData){

        if (! isGoodEvent(m_eventInfo.runNumber, m_eventInfo.lumiBlock))
            return;

        if (!passTrigger()) 
            return;
    }

    else if (m_isTTbar && m_isTTbarUnbinned) {
        if ( event_lheHT > 600. ) return;
    }

    if (!passMETFilters()) 
        return;

    createTemplates();

    n_AK04_jets = 0 ;

    // compute event HT
    for ( unsigned int i = 0; i < jetAK4_TLV.size(); ++i ) 
    {
        double pt = jetAK4_TLV[i].Pt();
        double eta = jetAK4_TLV[i].Eta();

        if ( (pt < 40.) || (fabs(eta) > 3.) )
            continue;

        event_HT += pt;
        ++ n_AK04_jets ;

    }

    // n_AK04_jets = jetAK4_TLV.size() ;


    // if (event_HT<700) return;
    if (event_HT < 800) return;

    // cout<<endl<<"passed HT"<<endl;




    std::vector<UZH::Electron> goodElectrons;

    for ( int i = 0; i <m_electron.N; ++i ) 
    {

        UZH::Electron myelectron( &m_electron, i );

        bool passCuts = (myelectron.pt() > m_electronPtCut) && (fabs(myelectron.eta()) < m_electronEtaCut);
        bool isID = myelectron.isLooseElectronBoosted() == 1 ;

        if ( !(passCuts && isID) ) continue;
        
        TLorentzVector ele_TLV = * myelectron.getTLV();
        bool overlapWithTau = false;

        for (size_t iTau = 0; iTau < tau_TLV.size(); ++iTau){

            if ( ele_TLV.DeltaR(tau_TLV.at(iTau)) < 0.05 ){
                overlapWithTau = true;
                break;
            } // if overlap
 
        } //loop on taus

        if (overlapWithTau) continue;

        for (size_t iTau = 0; iTau < slimmed_TLV.size(); ++iTau){
            
            if ( ele_TLV.DeltaR(slimmed_TLV.at(iTau)) < 0.05 ){
                overlapWithTau = true;
                break;
            } // if overlap
 
        } //loop on slimmed taus

        if (overlapWithTau) continue;

        goodElectrons.push_back(myelectron);

    }// loop on electrons
    
    std::vector<UZH::Muon> goodMuons;

    for ( int i = 0; i<m_muon.N ; ++i ) 
    {

        UZH::Muon mymuon( &m_muon, i );

        bool passCuts = (mymuon.pt() > m_muonPtCut) && (fabs(mymuon.eta()) < m_muonEtaCut);
        bool isID = mymuon.isLooseMuon() == 1 ;

        if ( !(passCuts && isID) ) continue;
        
        TLorentzVector mu_TLV = * mymuon.getTLV();
        bool overlapWithTau = false;

        for (size_t iTau = 0; iTau < tau_TLV.size(); ++iTau){

            if ( mu_TLV.DeltaR(tau_TLV.at(iTau)) < 0.05 ){
                overlapWithTau = true;
                break;
            } // if overlap
 
        } //loop on taus

        if (overlapWithTau) continue;

        for (size_t iTau = 0; iTau < slimmed_TLV.size(); ++iTau){

            if ( mu_TLV.DeltaR(slimmed_TLV.at(iTau)) < 0.05 ){
                overlapWithTau = true;
                break;
            } // if overlap
 
        } //loop on slimmed taus

        if (overlapWithTau) continue;

        goodMuons.push_back(mymuon);

    } //end loop on muons


    std::vector<UZH::Electron> isoElectrons;

    for ( unsigned int i = 0; i < goodElectrons.size(); ++i ) 
    {
        UZH::Electron myelectron = goodElectrons.at(i);
        TLorentzVector ele_TLV = * myelectron.getTLV();

        double highestTauPt = 0.;
        bool foundTau = false;

        for (size_t iTau = 0; iTau < tau_TLV.size(); ++iTau){

            if ( tau_TLV.at(iTau).DeltaR(ele_TLV) < 0.3 )
                if ( tau_TLV.at(iTau).Pt() > highestTauPt ){
                    highestTauPt = tau_TLV.at(iTau).Pt() ;
                    foundTau = true;
                }
        }

        if ( !foundTau ){

            for (size_t iTau = 0; iTau < slimmed_TLV.size(); ++iTau){

                if ( slimmed_TLV.at(iTau).DeltaR(ele_TLV) < 0.3 )
                    if ( slimmed_TLV.at(iTau).Pt() > highestTauPt ){
                        highestTauPt = slimmed_TLV.at(iTau).Pt() ;
                        foundTau = true;
                    }
            }

        } // if not found boosted tau

        
        if ( !foundTau ) continue;

        double eleIso = (myelectron.SemileptonicPFIso()-highestTauPt) / myelectron.pt() ; 

        if (( fabs(myelectron.eta()) <=  1.479 &&  eleIso < 0.0893) ||
           ( fabs(myelectron.eta()) >   1.479  &&  eleIso < 0.121 ) )

            isoElectrons.push_back(myelectron);

    }


    std::vector<UZH::Muon> isoMuons;

    for ( unsigned int i = 0; i < goodMuons.size(); ++i ) 
    {
        UZH::Muon mymuon = goodMuons.at(i);
        TLorentzVector mu_TLV = * mymuon.getTLV();

        double highestTauPt = 0.;
        bool foundTau = false;

        for (size_t iTau = 0; iTau < tau_TLV.size(); ++iTau){

            if ( tau_TLV.at(iTau).DeltaR(mu_TLV) < 0.4 )
                if ( tau_TLV.at(iTau).Pt() > highestTauPt ){
                    highestTauPt = tau_TLV.at(iTau).Pt() ;
                    foundTau = true;
                }
        }

        if ( !foundTau ){

            for (size_t iTau = 0; iTau < slimmed_TLV.size(); ++iTau){

                if ( slimmed_TLV.at(iTau).DeltaR(mu_TLV) < 0.3 )
                    if ( slimmed_TLV.at(iTau).Pt() > highestTauPt ){
                        highestTauPt = slimmed_TLV.at(iTau).Pt() ;
                        foundTau = true;
                    }
            }

        } // if not found boosted tau

        
        if ( !foundTau ) continue;

        double muIso = (mymuon.SemileptonicPFIso()-highestTauPt) / mymuon.pt() ; 

            if ( muIso < 0.25)
                isoMuons.push_back(mymuon);

    }


    int n_isoMuons = isoMuons.size();
    int n_isoElectrons = isoElectrons.size();

    // cout<<"n. El = "<<n_electrons<<"\t n. Mu = "<<n_muons<<endl;

    if ((n_isoMuons > 0) || (n_isoElectrons > 0) ) 
        return;


    // // cout<<"passed leptons"<<endl;

    // select met
    if (met_et < m_metCut || TMath::IsNaN(met_et))
    // if (TMath::IsNaN(met_et))
        return;

    // cout<<"passed MET"<<endl;



////////////////////////////////////////////////////////////////////////////////////////////



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
            if (!m_isData) goodAK8_jer_sigma_pt.push_back(jetAK8_jer_sigma_pt[i]);


        } // if CHS AK08 passes cuts


    }//end loop on jets


    if (goodAK8_TLV.size() == 0) 
        return;

    n_AK08_jets = goodAK8_TLV.size() ;

    // cout<<"found jet"<<endl;

    // std::vector<UZH::Tau> goodTaus;

    for ( unsigned int i = 0; i<tau_TLV.size(); ++i )
    {

        bool passCuts = (tau_TLV[i].Pt() > m_tauPtCut) && (fabs(tau_TLV[i].Eta()) < m_tauEtaCut);

        if (!passCuts) continue;
        
        goodTau_TLV.push_back(tau_TLV[i]);
        goodTau_VLooseIso.push_back(tau_VLooseIso[i]) ;
        goodTau_LooseIso.push_back(tau_LooseIso[i]) ;
        goodTau_MediumIso.push_back(tau_MediumIso[i]) ;
        goodTau_rawIso.push_back(tau_rawIso[i]) ;


    }//end loop on taus


    if (goodTau_TLV.size() < 2) 
        return;

    n_taus = goodTau_TLV.size() ;
    // cout<<"found taus"<<endl;


    for (unsigned int i = 0; i < goodAK8_TLV.size(); ++i) 
    {

        TVector2 v2_jet (goodAK8_TLV[i].Px(), goodAK8_TLV[i].Py());
        TVector2 v2_met (met_px, met_py);
        double deltaPhi = fabs( v2_jet.DeltaPhi(v2_met) );
        
        // bool isMetAngle = (deltaPhi > m_jetMetDeltaPhiMin);

        if ( (goodAK8_TLV[i].Pt() > goodAK08TLV_sel.Pt()))
        {
            goodAK08TLV_sel = goodAK8_TLV[i];
            jetPrunedMass = goodAK8_pruned_mass[i]; 
            if (!m_isData) jetJerSigmaPt = goodAK8_jer_sigma_pt[i]; 
            jetHbbtag = goodAK8_Hbbtag[i];
            jetMetDeltaPhi = deltaPhi ;
            
        }
    }

    // bool isJetMetAngle = (jetMetDeltaPhi > m_jetMetDeltaPhiMin);
    // if (!isJetMetAngle) return;

    if (goodAK08TLV_sel.Pt() < 10) //i.e. pt = 0
        return;



    UZH::PuppiJet goodPuppiJet; 
    
    float DR_puppijet_ak8 = 0.8 ; 
    bool FoundPuppiJet = false;

    for ( unsigned int i = 0; i < (*m_jetAK8Puppi.jetAK8_puppi_pt).size(); ++i ) {

        UZH::PuppiJet myjetPuppi( &m_jetAK8Puppi, i );
 
        if ( (myjetPuppi.jetAK8_puppi_pt() > m_jetPtCut) && (fabs(myjetPuppi.jetAK8_puppi_eta()) < m_jetEtaCut) ) {
                    
            TLorentzVector puppijet;
            puppijet.SetPtEtaPhiE(myjetPuppi.jetAK8_puppi_pt(),myjetPuppi.jetAK8_puppi_eta(),
                                  myjetPuppi.jetAK8_puppi_phi(),myjetPuppi.jetAK8_puppi_e());
                        
            if (puppijet.DeltaR(goodAK08TLV_sel) <  DR_puppijet_ak8){

                DR_puppijet_ak8 = puppijet.DeltaR( goodAK08TLV_sel ) ;
                goodPuppiJet = myjetPuppi ;
                FoundPuppiJet = true ;

            } // if match
            
        }// if puppi jet passes cuts

    } // loop on puppi AK08


    if ( ! FoundPuppiJet ) return;

    TLorentzVector puppijet_tlv;
    puppijet_tlv.SetPtEtaPhiE(goodPuppiJet.jetAK8_puppi_pt(),goodPuppiJet.jetAK8_puppi_eta(),
                              goodPuppiJet.jetAK8_puppi_phi(),goodPuppiJet.jetAK8_puppi_e());
    
    double puppiCorr = getPUPPIweight( goodPuppiJet.jetAK8_puppi_pt() , goodPuppiJet.jetAK8_puppi_eta() );


    if (!m_isData) jetSoftdropMass = getPuppi_sd_mass_ScaleAndResolution(true, goodPuppiJet.jetAK8_puppi_softdrop_mass() *puppiCorr, 
                                                                         puppijet_tlv,goodAK08TLV_sel,jetJerSigmaPt, 0, 0);

    else jetSoftdropMass = goodPuppiJet.jetAK8_puppi_softdrop_mass() *puppiCorr ;



    bool jetMassWindow = (jetSoftdropMass > m_jetMassMin) && (jetSoftdropMass < m_jetMassMax);
    bool isHbbtag = (jetHbbtag > m_jetHbbtagMin);

    if ( !jetMassWindow ) 
        return;

    // cout<<"passed jet"<<endl;


    if (isHbbtag)
        is_btag_SR = true;


// ////////////////////////////////////////////////////////////////////////////////////////////




    float sumPT_leading = 0;
    int iTau_sel = -1;
    int jTau_sel = -1;

    float tautau_met_deltaPhi_sel = -1. ;
    double deltaR_tautau_sel = -1. ;
    double deltaR_tautaujet_sel = -1. ;


    for (unsigned int iTau=0; iTau<goodTau_TLV.size(); ++iTau)

        for (unsigned int jTau=iTau+1; jTau<goodTau_TLV.size(); ++jTau)
        {

            TLorentzVector sumTLV = goodTau_TLV[iTau] + goodTau_TLV[jTau] ;

            TVector2 v2_taus (sumTLV.Px(), sumTLV.Py());
            TVector2 v2_met (met_px, met_py);

            double sumPT = sumTLV.Pt();
            float deltaPhi = fabs(v2_taus.DeltaPhi(v2_met));
            double deltaR_tautau = goodTau_TLV[iTau].DeltaR(goodTau_TLV[jTau]);
            double deltaR_tautaujet = sumTLV.DeltaR(goodAK08TLV_sel);


            // if ((sumPT >= sumPT_leading) && (deltaR_tautau < m_tautauDRMax)
            //     && (deltaR_tautaujet > m_tautauJetDRMin) /*&& (deltaR_tautaujet < m_tautauJetDRMax)*/ && (deltaPhi < m_tautauMetDeltaPhiMax) )

            if ( (sumPT >= sumPT_leading) && (deltaR_tautau < m_tautauDRMax) 
                && (deltaR_tautaujet > m_tautauJetDRMin)  && (deltaPhi < m_tautauMetDeltaPhiMax) )

            // if ( sumPT >= sumPT_leading )

            {
                sumPT_leading = sumPT;
                iTau_sel = iTau;
                jTau_sel = jTau;

                tautau_met_deltaPhi_sel = deltaPhi ;
                deltaR_tautau_sel = deltaR_tautau ;
                deltaR_tautaujet_sel = deltaR_tautaujet ;

            }


        }// loop on taus


    if ( (iTau_sel < 0) || (jTau_sel<0) )
        return;



    // cout<<"passed taus"<<endl;


    if (goodTau_TLV.at(iTau_sel).Pt() < goodTau_TLV.at(jTau_sel).Pt() )
    {
        int i_temp = iTau_sel;
        iTau_sel = jTau_sel;
        jTau_sel = i_temp;
    }


    tau1_TLV_sel = goodTau_TLV.at(iTau_sel);
    tau2_TLV_sel = goodTau_TLV.at(jTau_sel);
    sumTau_TLV_sel = tau1_TLV_sel + tau2_TLV_sel;

    original_tau_pt_1 = tau1_TLV_sel.Pt();
    original_tau_pt_2 = tau2_TLV_sel.Pt();

    original_tau_TLV_1 = tau1_TLV_sel;
    original_tau_TLV_2 = tau2_TLV_sel;


    double px1 = tau1_TLV_sel.Px();
    double px2 = tau2_TLV_sel.Px();
    double py1 = tau1_TLV_sel.Py();
    double py2 = tau2_TLV_sel.Py();

    if ( ((py2*px1-px2*py1) == 0) || (px1 == 0) )
        return;

    // cout<<"passed taus 2"<<endl;

    b = ( met_py*px1 - met_px*py1 )/( py2*px1 - px2*py1 );
    a = ( met_px -b*px2 )/px1;

    tau1_TLV_sel *= (1+a) ;
    tau2_TLV_sel *= (1+b) ;
    sumTau_TLV_sel = tau1_TLV_sel + tau2_TLV_sel ;

    double tautauMass = sumTau_TLV_sel.M();
    double resonanceMass = ( goodAK08TLV_sel + sumTau_TLV_sel ).M();
    double sumPT = sumTau_TLV_sel.Pt();

    if ( (sumPT < m_tautauPtCut) || (resonanceMass < m_tautauJetMassMin)
         || (tautauMass < m_tautauMassMin) || (tautauMass > m_tautauMassMax) )

        return;

    // cout<<"passed taus 3"<<endl;

    sumAll_TLV_sel = goodAK08TLV_sel + sumTau_TLV_sel ;

    tau1_rawIso = goodTau_rawIso[iTau_sel];
    tau2_rawIso = goodTau_rawIso[jTau_sel];

    bool passIso;

    {

        bool isTauIso_VLoose_1 = goodTau_VLooseIso[iTau_sel];
        bool isTauIso_VLoose_2 = goodTau_VLooseIso[jTau_sel];

        bool isTauIso_Loose_1 = goodTau_LooseIso[iTau_sel];
        bool isTauIso_Loose_2 = goodTau_LooseIso[jTau_sel];

        bool isTauIso_Medium_1 = goodTau_MediumIso[iTau_sel];
        bool isTauIso_Medium_2 = goodTau_MediumIso[jTau_sel];

        passIso = isTauIso_Medium_1 && isTauIso_Medium_2 ;

    }

    if (passIso)
        is_tauiso_SR = true;
            

////////////////////////////////////////////////////////////////////////////////////////////


    b_pass = true;

    if (!m_isData) b_weight = getEventWeight();
    if (!m_isData) applyTurnOn ();
    // if (!m_isData) applyHbbtagSF ( goodAK08TLV_sel );
    if ( (!m_isData) && (!m_isQCD) && (do_tauID_shift != 0) ) applyTauIDerror();

    // if ((!m_isData) && (!m_isSignal)) applyHbbTagcorrection ();
    // if ((!m_isData) && (!m_isQCD) && (do_pdf_shift!=0)) applyPDFreweight();


    int iCat = -1;
    if (is_btag_SR && is_tauiso_SR) iCat = 1;
    else if (!is_btag_SR && is_tauiso_SR) iCat = 2;
    else if (is_btag_SR && !is_tauiso_SR) iCat = 3;
    else iCat = 4;


    // if ( jetHbbtag > -0.8 )
    if ( jetHbbtag > -0.5 )
    {
        fillHistograms(m_catNames[ 0 ]);
        fillHistograms(m_catNames[ iCat ]);
    
        if (!is_tauiso_SR) fillHistograms(m_catNames[ 5 ]);
        if (!is_btag_SR) fillHistograms(m_catNames[ 6 ]);
        if (!(is_btag_SR && is_tauiso_SR)) fillHistograms(m_catNames[ 7 ]);

    }

    int shiftCat = 8 ;

    fillHistograms(m_catNames[ 0 + shiftCat ]);
    fillHistograms(m_catNames[ iCat + shiftCat ]);
    
    if (!is_tauiso_SR) fillHistograms(m_catNames[ 5 + shiftCat ]);
    if (!is_btag_SR) fillHistograms(m_catNames[ 6 + shiftCat ]);
    if (!(is_btag_SR && is_tauiso_SR)) fillHistograms(m_catNames[ 7 + shiftCat ]);


    fillHistograms(m_catNames[0]) ;
                
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

    //b_weightGen = (m_eventInfo.genEventWeight < 0) ? -1 : 1; 
    b_weightGen = m_eventInfo.genEventWeight;
    weight *= b_weightPU*b_weightGen;
  
    return weight;
  
}


void VHTausAnalysis::clearBranches() {

    event_lheHT = -1. ;

    b_weight = 1.;
    b_weightGen = 1.;
    b_weightPU = 1.;
  
    b_runNumber = -99;
    b_eventNumber = -99;
    b_lumiBlock = -99;

    w_tauID = 1.;

    event_HT = 0.;
    b_pass = false;
    is_btag_SR = false;
    is_tauiso_SR = false;


    // TLV templates
    jetAK4_TLV.clear();
    jetAK8_TLV.clear();
    tau_TLV.clear() ;
    slimmed_TLV.clear() ;

    // jet variables (templates)
    jetAK8_Hbbtag.clear() ;
    jetAK8_pruned_mass.clear() ;
    jetAK8_IDTight.clear() ;
    jetAK8_jer_sigma_pt.clear() ;

    goodAK8_jer_sigma_pt.clear() ;

    // tau variables (templates)
    tau_VLooseIso.clear() ;
    tau_LooseIso.clear() ;
    tau_MediumIso.clear() ;
    tau_rawIso.clear() ;

    // preselected TLV
    goodAK8_TLV.clear();
    goodTau_TLV.clear() ;

    // preselected variables
    goodAK8_Hbbtag.clear() ;
    goodAK8_pruned_mass.clear() ;
    goodTau_VLooseIso.clear() ;
    goodTau_LooseIso.clear() ;
    goodTau_MediumIso.clear() ;
    goodTau_rawIso.clear() ;

    // stuff
    a = 0 ;
    b = 0 ;

    n_AK04_jets = 0 ;
    n_AK08_jets = 0 ;
    n_taus = 0 ;


    TLorentzVector null_TLV (0,0,0,0);


    // selected TLV
    goodAK08TLV_sel = null_TLV ;
    tau1_TLV_sel = null_TLV ;
    tau2_TLV_sel = null_TLV ;
    sumTau_TLV_sel = null_TLV ;
    sumAll_TLV_sel = null_TLV ;

    // selected variables
    jetPrunedMass = 0. ;
    jetSoftdropMass = 0. ;
    jetJerSigmaPt = 0. ;
    jetHbbtag = -2.;
    jetMetDeltaPhi = -1.;

    tau1_rawIso = -2;
    tau2_rawIso = -2;
    original_tau_pt_1 = 0.;
    original_tau_pt_2 = 0.;

    original_tau_TLV_1 = null_TLV ;
    original_tau_TLV_2 = null_TLV ;

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


    // event

    Book ( TH1F("HT","event HT;HT [GeV]",200,0,10000), directory);
    Book ( TH1F("lheHT","lheHT;lheHT [GeV]",1000,0,10000), directory);
    Book ( TH1F("n_PV","number of primary vertices;n. of Primary Vertices",101,-.5,100.5), directory);

    Book ( TH1F("n_AK04_jets","number of AK04 jets;n. AK04 jets",36,-.5,35.5), directory);
    Book ( TH1F("n_AK08_jets","number of AK08 jets;n. AK08 jets",11,-.5,10.5), directory);
    Book ( TH1F("n_taus","number of taus;n. taus",16,-.5,15.5), directory);


    // MET

    Book ( TH1F("MET_ET","missing energy ET;missing energy E_{T} [GeV]",40,0,2000), directory);
    Book ( TH1F("MET_px","missing energy px;missing momentum p_{x} [GeV]",20,0,1000), directory);
    Book ( TH1F("MET_py","missing energy py;missing momentum p_{y} [GeV]",20,0,1000), directory);

    // jet

    Book ( TH1F("jet_pT","jet transverse momentum;jet p_{T} [GeV]",30,0,3000), directory);
    Book ( TH1F("jet_eta","jet pseudorapidity ;jet #eta",20,-2.4,2.4), directory);
    Book ( TH1F("jet_phi","jet angle ;jet #phi",10,-1.*TMath::Pi(),TMath::Pi()), directory);
    Book ( TH1F("jet_Hbbtag","double b-tagger output;bb-tagger output",20,-1,1), directory);
    Book ( TH1F("jet_softdrop_mass","jet softdrop mass;jet softdrop mass [GeV]",300,0,1500), directory);
    Book ( TH1F("jet_pruned_mass","jet pruned mass;jet pruned mass [GeV]",300,0,1500), directory);

    // original taus

    Book ( TH1F("original_tau1_pT","leading tau pT;leading tau p_{T} [GeV]",30,0,2000), directory);
    Book ( TH1F("original_tau1_eta","leading tau pseudorapidity ;leading tau #eta",20,-2.4,2.4), directory);
    Book ( TH1F("original_tau1_phi","leading tau angle ;leading tau #phi",10,-1.*TMath::Pi(),TMath::Pi()), directory);

    Book ( TH1F("original_tau2_pT","sub-leading tau pT;sub-leading tau p_{T} [GeV]",30,0,1000), directory);
    Book ( TH1F("original_tau2_eta","sub-leading tau pseudorapidity ;sub-leading tau #eta",20,-2.4,2.4), directory);
    Book ( TH1F("original_tau2_phi","sub-leading tau angle ;leading tau #phi",10,-1.*TMath::Pi(),TMath::Pi()), directory);

    Book ( TH1F("original_tautau_pT","tau pair pT ;tau pair p_{T} [GeV]",30,0,4000), directory);
    Book ( TH1F("original_tautau_eta","tau pair pseudorapidity ; tau pair #eta",20,-2.4,2.4), directory);
    Book ( TH1F("original_tautau_phi","tau pair angle ; tau pair #phi",10,-1.*TMath::Pi(),TMath::Pi()), directory);

    Book ( TH1F("tau1_iso","leading tau isolation;leading tau isolation",22,-1.1,1.1), directory);
    Book ( TH1F("tau2_iso","sub-leading tau isolation;sub-leading tau isolation",22,-1.1,1.1), directory);
    Book ( TH1F("sum_iso","sum of tau isolations;sum of tau isolations",22,-2.2,2.2), directory);


    // corrected taus

    Book ( TH1F("corrected_tau1_pT","leading tau pT;leading tau p_{T} [GeV]",60,0,6000), directory);
    Book ( TH1F("corrected_tau1_eta","leading tau pseudorapidity ;leading tau #eta",20,-2.4,2.4), directory);
    Book ( TH1F("corrected_tau1_phi","leading tau angle ;leading tau #phi",10,-1.*TMath::Pi(),TMath::Pi()), directory);

    Book ( TH1F("corrected_tau2_pT","sub-leading tau pT;sub-leading tau p_{T} [GeV]",60,0,6000), directory);
    Book ( TH1F("corrected_tau2_eta","sub-leading tau pseudorapidity ;sub-leading tau #eta",20,-2.4,2.4), directory);
    Book ( TH1F("corrected_tau2_phi","sub-leading tau angle ;leading tau #phi",10,-1.*TMath::Pi(),TMath::Pi()), directory);

    Book ( TH1F("corrected_tautau_pT","tau pair pT ;tau pair p_{T} [GeV]",30,0,4000), directory);
    Book ( TH1F("corrected_tautau_eta","tau pair pseudorapidity ; tau pair #eta",20,-2.4,2.4), directory);
    Book ( TH1F("corrected_tautau_phi","tau pair angle ; tau pair #phi",10,-1.*TMath::Pi(),TMath::Pi()), directory);

    // angular variables

    Book ( TH1F("jet_met_DP","jet-met angle;#Delta#phi (jet,MET)",20,0,TMath::Pi()), directory);

    Book ( TH1F("original_tautau_jet_DR","tautau-jet distance ;#DeltaR (#tau#tau,jet) ",100,0,20), directory);
    Book ( TH1F("original_tau_tau_DR","tau pair distance;#DeltaR (#tau,#tau)",300,0,20), directory);
    Book ( TH1F("original_tautau_jet_DP","tautau-jet angle; #Delta#phi (#tau#tau,jet)",20,0,TMath::Pi()), directory);
    Book ( TH1F("original_tautau_met_DP","tautau-met angle; #Delta#phi (#tau#tau,MET)",20,0,TMath::Pi()), directory);

    Book ( TH1F("corrected_tautau_jet_DR","tautau-jet distance ;#DeltaR (#tau#tau,jet) ",100,0,20), directory);
    Book ( TH1F("corrected_tau_tau_DR","tau pair distance;#DeltaR (#tau,#tau)",300,0,20), directory);
    Book ( TH1F("corrected_tautau_jet_DP","tautau-jet angle; #Delta#phi (#tau#tau,jet)",20,0,TMath::Pi()), directory);
    Book ( TH1F("corrected_tautau_met_DP","tautau-met angle; #Delta#phi (#tau#tau,MET)",20,0,TMath::Pi()), directory);


    // mass variables

    Book ( TH1F("original_tautau_mass","tau pair invariant mass ; mass (#tau,#tau) [GeV]",300,0,2500), directory);    
    Book ( TH1F("corrected_tautau_mass","tau pair invariant mass ; mass (#tau,#tau) [GeV]",300,0,2500), directory);    
    Book ( TH1F("raw_tautau_jet_mass","tautau-jet invariant mass;mass (#tau#tau,jet) [GeV]",70,0,7000), directory);
    Book ( TH1F("resonance_mass","resonance invariant mass (coll. nu);mass (#tau#tau,jet) [GeV]",100,0,10000), directory);
    Book ( TH1F("resonance_mass_bin","resonance invariant mass (coll. nu);mass (#tau#tau,jet) [GeV]",1000,0,10000), directory);

}



void VHTausAnalysis::fillHistograms( const TString& directory )

{

    if (m_isTTbar) Hist( "lheHT", directory )->Fill( event_lheHT );

    Hist( "HT", directory )->Fill( event_HT , b_weight );
    Hist( "n_PV", directory )->Fill( m_eventInfo.PV_N , b_weight );
    
    Hist( "n_AK04_jets", directory )->Fill( n_AK04_jets , b_weight );
    Hist( "n_AK08_jets", directory )->Fill( n_AK08_jets , b_weight );
    Hist( "n_taus", directory )->Fill( n_taus , b_weight );


    Hist( "MET_ET", directory )->Fill( met_et , b_weight );
    Hist( "MET_px", directory )->Fill( met_px , b_weight );
    Hist( "MET_py", directory )->Fill( met_py , b_weight );

    Hist( "jet_pT", directory )->Fill( goodAK08TLV_sel.Pt() , b_weight );
    Hist( "jet_eta", directory )->Fill( goodAK08TLV_sel.Eta() , b_weight );
    Hist( "jet_phi", directory )->Fill( goodAK08TLV_sel.Phi() , b_weight );
    Hist( "jet_Hbbtag", directory )->Fill( jetHbbtag , b_weight );
    Hist( "jet_softdrop_mass", directory )->Fill( jetSoftdropMass , b_weight );
    Hist( "jet_pruned_mass", directory )->Fill( jetPrunedMass , b_weight );


    Hist( "original_tau1_pT", directory )->Fill( original_tau_TLV_1.Pt() , b_weight );
    Hist( "original_tau1_eta", directory )->Fill( original_tau_TLV_1.Eta() , b_weight );
    Hist( "original_tau1_phi", directory )->Fill( original_tau_TLV_1.Phi() , b_weight );

    Hist( "original_tau2_pT", directory )->Fill( original_tau_TLV_2.Pt() , b_weight );
    Hist( "original_tau2_eta", directory )->Fill( original_tau_TLV_2.Eta() , b_weight );
    Hist( "original_tau2_phi", directory )->Fill( original_tau_TLV_2.Phi() , b_weight );

    TLorentzVector orig_sumTau_TLV = original_tau_TLV_1 + original_tau_TLV_2 ;

    Hist( "original_tautau_pT", directory )->Fill( orig_sumTau_TLV.Pt() , b_weight );
    Hist( "original_tautau_eta", directory )->Fill( orig_sumTau_TLV.Eta() , b_weight );
    Hist( "original_tautau_phi", directory )->Fill( orig_sumTau_TLV.Phi() , b_weight );

    Hist( "tau1_iso", directory )->Fill( tau1_rawIso , b_weight );
    Hist( "tau2_iso", directory )->Fill( tau2_rawIso , b_weight );
    Hist( "sum_iso", directory )->Fill( tau1_rawIso + tau2_rawIso , b_weight );


    Hist( "corrected_tau1_pT", directory )->Fill( tau1_TLV_sel.Pt() , b_weight );
    Hist( "corrected_tau1_eta", directory )->Fill( tau1_TLV_sel.Eta() , b_weight );
    Hist( "corrected_tau1_phi", directory )->Fill( tau1_TLV_sel.Phi() , b_weight );

    Hist( "corrected_tau2_pT", directory )->Fill( tau2_TLV_sel.Pt() , b_weight );
    Hist( "corrected_tau2_eta", directory )->Fill( tau2_TLV_sel.Eta() , b_weight );
    Hist( "corrected_tau2_phi", directory )->Fill( tau2_TLV_sel.Phi() , b_weight );

    Hist( "corrected_tautau_pT", directory )->Fill( sumTau_TLV_sel.Pt() , b_weight );
    Hist( "corrected_tautau_eta", directory )->Fill( sumTau_TLV_sel.Eta() , b_weight );
    Hist( "corrected_tautau_phi", directory )->Fill( sumTau_TLV_sel.Phi() , b_weight );


    TVector2 v2_met (met_px, met_py);
    TVector2 v2_jet (goodAK08TLV_sel.Px(),goodAK08TLV_sel.Py());
    TVector2 v2_original_taus (orig_sumTau_TLV.Px(),orig_sumTau_TLV.Py());
    TVector2 v2_corrected_taus (sumTau_TLV_sel.Px(),sumTau_TLV_sel.Py());

    Hist( "jet_met_DP", directory )->Fill(fabs(v2_met.DeltaPhi(v2_jet)),b_weight );

    Hist( "original_tautau_jet_DR", directory )->Fill( orig_sumTau_TLV.DeltaR( goodAK08TLV_sel ) , b_weight );
    Hist( "original_tau_tau_DR", directory )->Fill( original_tau_TLV_2.DeltaR( original_tau_TLV_1 ) , b_weight );
    Hist( "original_tautau_jet_DP", directory )->Fill( v2_original_taus.DeltaPhi( v2_jet ) , b_weight );
    Hist( "original_tautau_met_DP", directory )->Fill( v2_original_taus.DeltaPhi( v2_met ) , b_weight );

    Hist( "corrected_tautau_jet_DR", directory )->Fill( sumTau_TLV_sel.DeltaR( goodAK08TLV_sel ) , b_weight );
    Hist( "corrected_tau_tau_DR", directory )->Fill( tau2_TLV_sel.DeltaR( tau1_TLV_sel ) , b_weight );
    Hist( "corrected_tautau_jet_DP", directory )->Fill( v2_corrected_taus.DeltaPhi( v2_jet ) , b_weight );
    Hist( "corrected_tautau_met_DP", directory )->Fill( v2_corrected_taus.DeltaPhi( v2_met ) , b_weight );

    Hist( "original_tautau_mass", directory )->Fill( orig_sumTau_TLV.M() , b_weight );
    Hist( "corrected_tautau_mass", directory )->Fill( sumTau_TLV_sel.M() , b_weight );
    Hist( "raw_tautau_jet_mass", directory )->Fill( (orig_sumTau_TLV + goodAK08TLV_sel).M() , b_weight );
    Hist( "resonance_mass", directory )->Fill( sumAll_TLV_sel.M() , b_weight );
    Hist( "resonance_mass_bin", directory )->Fill( sumAll_TLV_sel.M() , b_weight );


}


void VHTausAnalysis::applyTurnOn (){

        
    // double lumi_800 = 27.22;
    // double lumi_900 = 9.;

    double HT_4eff = event_HT;
    if ( HT_4eff > 1500. ) HT_4eff = 1500. ;
        
    // double factor_800 = trig_eff_HT800->GetEfficiency(trig_eff_HT800->FindFixBin(HT_4eff));
    // double factor_900 = trig_eff_HT900->GetEfficiency(trig_eff_HT900->FindFixBin(HT_4eff));
   
    // double trigger_factor = ( factor_800*lumi_800 + factor_900*lumi_900 ) / ( lumi_800+lumi_900 ) ;

    // b_weight *= trigger_factor;

    b_weight *= trig_eff->GetEfficiency(trig_eff->FindFixBin(HT_4eff));
    // std::cout<<trig_eff->GetEfficiency(trig_eff->FindFixBin(HT_4eff))<<std::endl;

}

// void VHTausAnalysis::applyHTcorrection (){


//     if (event_HT<1500.){

//         double HT_factor = 1.;
        
//         // if ( do_turnon_shift == 0 ) HT_factor = trig_eff->GetEfficiency(trig_eff->FindFixBin(event_HT));
//         // else if ( do_turnon_shift == 1 ) HT_factor = trig_eff->GetEfficiency(trig_eff->FindFixBin(event_HT)) + 
//         //                                     trig_eff->GetEfficiencyErrorUp(trig_eff->FindFixBin(event_HT));
//         // else if ( do_turnon_shift == -1 ) HT_factor = trig_eff->GetEfficiency(trig_eff->FindFixBin(event_HT)) - 
//         //                                     trig_eff->GetEfficiencyErrorLow(trig_eff->FindFixBin(event_HT));

//         HT_factor = hist_HT_corr->GetBinContent(hist_HT_corr->FindFixBin(event_HT));
        
//         // std::cout<<HT_factor<<std::endl<<std::endl;

//         b_weight *= HT_factor;


//     }
// }

// void VHTausAnalysis::applyHbbTagcorrection (){


//     double HbbTag_factor = hist_HbbTag_corr->GetBinContent(hist_HbbTag_corr->FindFixBin(jetHbbtag));
//     double err_HbbTag_factor = hist_HbbTag_corr->GetBinError(hist_HbbTag_corr->FindFixBin(jetHbbtag));

//     double rand_HbbTag_factor = Hbb_rand.Gaus( HbbTag_factor, err_HbbTag_factor );

//     b_weight *= rand_HbbTag_factor;

//     return;

// }


void VHTausAnalysis::applyHbbtagSF (const TLorentzVector& selectedJet ){

    // b_weight = getEventWeight();
    double pt = selectedJet.Pt();

                
    if (m_isSignal)
    {

        if (is_btag_SR)
        {

            if (m_jetHbbtagMin == .3){

                if      ( pt>=250. && pt<300. ) b_weight *= SF_btag_loose_250_300 + do_Hbb_tag_SF_shift*err_SF_btag_loose_250_300;
                else if ( pt>=300. && pt<350. ) b_weight *= SF_btag_loose_300_350 + do_Hbb_tag_SF_shift*err_SF_btag_loose_300_350; 
                else if ( pt>=350. && pt<400. ) b_weight *= SF_btag_loose_350_400 + do_Hbb_tag_SF_shift*err_SF_btag_loose_350_400; 
                else if ( pt>=400. && pt<450. ) b_weight *= SF_btag_loose_400_450 + do_Hbb_tag_SF_shift*err_SF_btag_loose_400_450; 
                else if ( pt>=450. && pt<500. ) b_weight *= SF_btag_loose_450_500 + do_Hbb_tag_SF_shift*err_SF_btag_loose_450_500; 
                else if ( pt>=500. && pt<700. ) b_weight *= SF_btag_loose_500_700 + do_Hbb_tag_SF_shift*err_SF_btag_loose_500_700; 
                else                            b_weight *= 1.;
            }

            else if (m_jetHbbtagMin == .6){

                if      ( pt>=250. && pt<300. ) b_weight *= SF_btag_mediumLoose_250_300 + do_Hbb_tag_SF_shift*err_SF_btag_mediumLoose_250_300;
                else if ( pt>=300. && pt<350. ) b_weight *= SF_btag_mediumLoose_300_350 + do_Hbb_tag_SF_shift*err_SF_btag_mediumLoose_300_350; 
                else if ( pt>=350. && pt<400. ) b_weight *= SF_btag_mediumLoose_350_400 + do_Hbb_tag_SF_shift*err_SF_btag_mediumLoose_350_400; 
                else if ( pt>=400. && pt<450. ) b_weight *= SF_btag_mediumLoose_400_450 + do_Hbb_tag_SF_shift*err_SF_btag_mediumLoose_400_450; 
                else if ( pt>=450. && pt<500. ) b_weight *= SF_btag_mediumLoose_450_500 + do_Hbb_tag_SF_shift*err_SF_btag_mediumLoose_450_500; 
                else if ( pt>=500. && pt<700. ) b_weight *= SF_btag_mediumLoose_500_700 + do_Hbb_tag_SF_shift*err_SF_btag_mediumLoose_500_700; 
                else                            b_weight *= 1.;
            }


            else if (m_jetHbbtagMin == .8){

                if      ( pt>=250. && pt<300. ) b_weight *= SF_btag_mediumTight_250_300 + do_Hbb_tag_SF_shift*err_SF_btag_mediumTight_250_300;
                else if ( pt>=300. && pt<350. ) b_weight *= SF_btag_mediumTight_300_350 + do_Hbb_tag_SF_shift*err_SF_btag_mediumTight_300_350; 
                else if ( pt>=350. && pt<400. ) b_weight *= SF_btag_mediumTight_350_400 + do_Hbb_tag_SF_shift*err_SF_btag_mediumTight_350_400; 
                else if ( pt>=400. && pt<450. ) b_weight *= SF_btag_mediumTight_400_450 + do_Hbb_tag_SF_shift*err_SF_btag_mediumTight_400_450; 
                else if ( pt>=450. && pt<500. ) b_weight *= SF_btag_mediumTight_450_500 + do_Hbb_tag_SF_shift*err_SF_btag_mediumTight_450_500; 
                else if ( pt>=500. && pt<700. ) b_weight *= SF_btag_mediumTight_500_700 + do_Hbb_tag_SF_shift*err_SF_btag_mediumTight_500_700; 
                else                            b_weight *= 1.;
            }



        }

        // else 
        // {
        //     if      ( pt>=300. && pt<400. ) b_weight *= 2.- (SF_btag_medium_300_400 + do_Hbb_tag_SF_shift*err_SF_btag_medium_300_400);
        //     else if ( pt>=400. && pt<500. ) b_weight *= 2.- (SF_btag_medium_400_500 + do_Hbb_tag_SF_shift*err_SF_btag_medium_400_500); 
        //     else if ( pt>=500. && pt<600. ) b_weight *= 2.- (SF_btag_medium_500_600 + do_Hbb_tag_SF_shift*err_SF_btag_medium_500_600); 
        //     else if ( pt>=600.            ) b_weight *= 2.- (SF_btag_medium_600_700 + do_Hbb_tag_SF_shift*err_SF_btag_medium_600_700); 
        //     else                            b_weight *= 1.;
        // }

    }//if signal


    // else if (m_isTTbar)
    // {
                    
    //     if (is_btag_SR)
    //     {
    //         if      ( pt>=300. && pt<400. ) b_weight *= mistag_SF_btag_medium_300_400 + do_Hbb_mistag_SF_shift*err_mistag_SF_btag_medium_300_400;
    //         else if ( pt>=400. && pt<550. ) b_weight *= mistag_SF_btag_medium_400_550 + do_Hbb_mistag_SF_shift*err_mistag_SF_btag_medium_400_550; 
    //         else if ( pt>=550.            ) b_weight *= mistag_SF_btag_medium_550_Inf + do_Hbb_mistag_SF_shift*err_mistag_SF_btag_medium_550_Inf; 
    //         else                            b_weight *= 1.;
    //     }

    //     else
    //     {
    //         if      (pt>=300. && pt<400.) b_weight *= 2.- (mistag_SF_btag_medium_300_400 + do_Hbb_mistag_SF_shift*err_mistag_SF_btag_medium_300_400);
    //         else if (pt>=400. && pt<550.) b_weight *= 2.- (mistag_SF_btag_medium_400_550 + do_Hbb_mistag_SF_shift*err_mistag_SF_btag_medium_400_550);
    //         else if (pt>=550.           ) b_weight *= 2.- (mistag_SF_btag_medium_550_Inf + do_Hbb_mistag_SF_shift*err_mistag_SF_btag_medium_550_Inf);
    //         else                            b_weight *= 1.;
    //     }


    // } //if ttbar

    // else b_weight *= 1.;




    return;
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

        if ( recoJet.pt() < 1. ) continue ;
        
        TLorentzVector originalJetTLV = * recoJet.getTLV();
        TLorentzVector recoJetTLV = * recoJet.getTLV();

        if (do_JEC_AK4_shift == 1) recoJetTLV *= recoJet.jecUp() / recoJet.jec();
        else if (do_JEC_AK4_shift == -1) recoJetTLV *= recoJet.jecDown() / recoJet.jec();


        if (!m_isData /*&& !m_isQCD*/)
        {

            double min_dist = -1.;
            double pt_gen = -1.;

            for ( int j = 0; j < (m_genJetAK4.N); ++j )
            {
                UZH::GenJet genJet( &m_genJetAK4, j );

                if (genJet.pt() < 1. ) continue ;
                
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

            if (do_JER_AK4_shift == 1) JER_SF = recoJet.jer_sf_up();
            else if (do_JER_AK4_shift == -1) JER_SF = recoJet.jer_sf_down();

            if ( (min_dist > 0) && (min_dist < 0.4/2.) )
            {
                double new_pt = pt_gen + JER_SF*(recoJetTLV.Pt()-pt_gen);
                if (new_pt < 0) new_pt = 0. ;

                //cout<<"scale\t"<<new_pt / recoJetTLV.Pt()<<endl;
                recoJetTLV *= new_pt / recoJetTLV.Pt();

            } //scaling method

            else
            {

                double new_pt = rand.Gaus( recoJetTLV.Pt(), jer_sigma_pt * pow( JER_SF*JER_SF -1., .5 ) );
                //cout<<"smear\t"<<new_pt / recoJetTLV.Pt()<<endl;
                recoJetTLV *= new_pt / recoJetTLV.Pt();

            }// smearing method

        }// not data and QCD
        
        jetAK4_TLV.push_back(recoJetTLV);

        met_px += originalJetTLV.Px() - recoJetTLV.Px() ;
        met_py += originalJetTLV.Py() - recoJetTLV.Py() ;

    }// reco AK4 loop

    // cout<<"final MET: px = "<<met_px<<"\tpy = "<<met_py<<endl;

    met_et = pow( met_px*met_px + met_py*met_py , .5);

    //jetAK8
    for ( int i = 0; i < (m_jetAK8.N); ++i )     
    {
        UZH::Jet recoJet( &m_jetAK8, i );

        if ( recoJet.pt() < 1. ) continue ;

        TLorentzVector recoJetTLV = * recoJet.getTLV();
        // double softdrop_mass = recoJet.softdrop_massCorr();
        double pruned_mass = recoJet.pruned_massCorr();

        if (do_JEC_AK8_shift == 1) recoJetTLV *= recoJet.jecUp() / recoJet.jec();
        else if (do_JEC_AK8_shift == -1) recoJetTLV *= recoJet.jecDown() / recoJet.jec();

        // if (do_softdrop_shift == 1) softdrop_mass *= recoJet.softdrop_jecUp() / recoJet.softdrop_jec() ;
        // else if (do_softdrop_shift == -1) softdrop_mass *= recoJet.softdrop_jecDown() / recoJet.softdrop_jec() ;


        if (!m_isData /*&& !m_isQCD*/)
        {

            double min_dist = -1.;
            double pt_gen = -1.;

            for ( int j = 0; j < (m_genJetAK8.N); ++j )
            {
                UZH::GenJet genJet( &m_genJetAK8, j );

                if ( genJet.pt() < 1. ) continue ;

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

            if (do_JER_AK8_shift == 1) JER_SF = recoJet.jer_sf_up();
            else if (do_JER_AK8_shift == -1) JER_SF = recoJet.jer_sf_down();

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

            jetAK8_jer_sigma_pt.push_back(recoJet.jer_sigma_pt());

        }// not data and QCD
        

        jetAK8_TLV.push_back(recoJetTLV);

        jetAK8_Hbbtag.push_back(recoJet.Hbbtag());
        jetAK8_IDTight.push_back(recoJet.IDTight());
        // jetAK8_softdrop_mass.push_back(softdrop_mass);
        jetAK8_pruned_mass.push_back(pruned_mass);




    }// reco AK8 loop



    //taus

    for ( int i = 0; i<(m_tau.N); ++i )
    {

        UZH::Tau mytau( &m_tau, i );

        if ( mytau.pt() < 1. ) continue ;

        bool isTauDM = mytau.decayModeFindingNewDMs();
        bool isTauBoosted = mytau.TauType()==2;
        // bool isAgainstElectron = mytau.againstElectronVLooseMVA6();
        // bool isAgainstMuon = mytau.againstMuonLoose3();

        if ( !isTauDM ) continue;
        // if ( (!isAgainstElectron) || (!isAgainstMuon) ) continue;

        TLorentzVector mytau_TLV = * mytau.getTLV();

        mytau_TLV *= 1. + do_tau_shift * .05 ;

        if (isTauBoosted){

            tau_TLV.push_back(mytau_TLV);

            tau_VLooseIso.push_back(mytau.byVLooseIsolationMVArun2v1PWnewDMwLT());
            tau_LooseIso.push_back(mytau.byLooseIsolationMVArun2v1PWnewDMwLT());
            tau_MediumIso.push_back(mytau.byMediumIsolationMVArun2v1PWnewDMwLT());
            tau_rawIso.push_back(mytau.byIsolationMVArun2v1DBnewDMwLTraw());

        }

        else slimmed_TLV.push_back(mytau_TLV);

    }//end loop on taus

    
}//end function


void VHTausAnalysis::applyPDFreweight (){

    // const size_t nmem = set.size()-1;
    // std::vector<double> xAll_1, xAll_2;


    // for (size_t imem = 0; imem <= nmem; imem++){
    //     xAll_1.push_back(pdfs[imem]->xfxQ(m_eventInfo.pdf_id->at(0),m_eventInfo.pdf_x->at(0),(double)m_eventInfo.pdf_scale));
    //     xAll_2.push_back(pdfs[imem]->xfxQ(m_eventInfo.pdf_id->at(1),m_eventInfo.pdf_x->at(1),(double)m_eventInfo.pdf_scale));
    // }

    // const LHAPDF::PDFUncertainty xErr_1 = set.uncertainty(xAll_1, -1);
    // const LHAPDF::PDFUncertainty xErr_2 = set.uncertainty(xAll_2, -1);

    // double w_pdf = 1.;
    // if ( do_pdf_shift == 1 ) w_pdf = (xErr_1.central + xErr_1.errplus) * (xErr_2.central + xErr_2.errplus) / (xErr_1.central * xErr_2.central);
    // else if ( do_pdf_shift == -1 ) w_pdf = (xErr_1.central - xErr_1.errminus) * (xErr_2.central - xErr_2.errminus) / (xErr_1.central * xErr_2.central);

    // b_weight *= w_pdf;

    return;
}

void VHTausAnalysis::applyTauIDerror(){

    w_tauID *= 1. + do_tauID_shift * (.06 + .2E-3 * original_tau_pt_1);
    w_tauID *= 1. + do_tauID_shift * (.06 + .2E-3 * original_tau_pt_2);    

    b_weight *= w_tauID;

    return;
}

void VHTausAnalysis::initPuppiSDWeight(void  ){

 
  
  auto fileSD = TFile::Open( m_PUPPIJEC_.c_str(),"READ");
  puppisd_corrGEN      = (TF1*)fileSD->Get("puppiJECcorr_gen");
  puppisd_corrRECO_cen = (TF1*)fileSD->Get("puppiJECcorr_reco_0eta1v3");
  puppisd_corrRECO_for = (TF1*)fileSD->Get("puppiJECcorr_reco_1v3eta2v5");
  // puppisd_corrRECO_for->SetDirectory(0);
  //puppisd_corrRECO_cen->SetDirectory(0);
 
  fileSD->Close();
   
}
float  VHTausAnalysis::getPUPPIweight(float puppipt, float puppieta ){
  
  float genCorr  = 1.;
  float recoCorr = 1.;
  float totalWeight = 1.;
        
  genCorr =  puppisd_corrGEN->Eval( puppipt );
  
  // std::cout<< "for PUPPI jet pT = 300 GeV : 1./genCorr = " << 1./(puppisd_corrGEN->Eval( 300. )) << " recoCorr = " <<puppisd_corrRECO_cen->Eval( 300 ) << " Total corr = " << (1./(puppisd_corrGEN->Eval( 300. )))*puppisd_corrRECO_cen->Eval( 300 )<<  std::endl;

  if( fabs(puppieta)  <= 1.3 ) recoCorr = puppisd_corrRECO_cen->Eval( puppipt );
  else
    if( fabs(puppieta) > 1.3 ) recoCorr = puppisd_corrRECO_for->Eval( puppipt );
  
  totalWeight = genCorr * recoCorr;
  // std::cout<< "for PUPPI jet pT = " << puppipt << " and eta = " << puppieta << " my weight is: " << std::endl;
  // std::cout<< "Gen corr = " << (1./genCorr) << " recoCorr = " << recoCorr << " Total = " << totalWeight << std::endl;
  return totalWeight;
}

void VHTausAnalysis::initPuppiSD_ScaleAndRes_Weight(void  ){

 
  
  auto  filejmr = TFile::Open(((std::string) (std::getenv("SFRAME_DIR"))+ "/../VHTausResonances/weightSD/puppiSoftdropResol.root").c_str(),"READ");
  puppisd_resolution_cen = (TF1*)filejmr->Get("massResolution_0eta1v3");
  puppisd_resolution_for   = (TF1*)filejmr->Get("massResolution_1v3eta2v5");
  //puppisd_resolution_for->SetDirectory(0);
  //puppisd_resolution_cen->SetDirectory(0);
 
  trandom = new TRandom3();
  filejmr->Close();

   
}

float VHTausAnalysis::getPuppi_sd_mass_ScaleAndResolution(bool isMC, float oldmass, TLorentzVector puppijet_tlv, TLorentzVector AK8jet_tlv , float jerSigmaPt  , float sigmaMassScale, float sigmaMassRes  ) {

    //("oldmass" is the corrected PUPPI softdrop mass (corrected using the jet mass corrections described  in https://twiki.cern.ch/twiki/bin/view/Sandbox/PUPPIJetMassScaleAndResolution).
    // just for MC 
    using namespace std;


    float mass    = 0;   
  
    // Scale correction



    float jms = 0.999;
    float jmsUnc = 0.004;
    mass = oldmass* ( jms + sigmaMassScale*jmsUnc);

    // Resolution correction

    float jmr       = 1.079;
    float jmrUnc = 0.105;
    float massResolution   = 0;
    //Evaluate PUPPI softdropMC  mass resolution as a function of PUPPI jet pt and eta:

 
    if( fabs( puppijet_tlv.Eta()) <= 1.3 ){
        massResolution = puppisd_resolution_cen->Eval( puppijet_tlv.Pt() );
    }
    else{
        massResolution = puppisd_resolution_for->Eval( puppijet_tlv.Pt() );
    }

    //Used the mass already corrected for the Mass scale
  
    bool scaled = false;
  
    // First try scaling:
  
    for ( unsigned int i = 0; i < (*m_genJetAK8.pt).size(); ++i ) {

        UZH::GenJet myjetGen( &m_genJetAK8, i );

        if ( myjetGen.pt() < 1. ) continue ;

        float genMass = myjetGen.softdropmass();

        TLorentzVector genJet;
        genJet.SetPtEtaPhiE(  myjetGen.pt(),  myjetGen.eta(),  myjetGen.phi(),  myjetGen.e() );

        if( AK8jet_tlv.DeltaR(genJet) > 0.4 || ( fabs(AK8jet_tlv.Pt()-genJet.Pt()) > (3 * AK8jet_tlv.Pt() *jerSigmaPt))) 
            continue;

        mass = std::max(float(0.), mass + ((jmr - sigmaMassRes*jmrUnc -1)*(mass-genMass)));

        scaled = true;
        break;
    }


    //Scaling failed, move to smearing:

    if( !scaled ){
        mass = trandom->Gaus( mass, TMath::Sqrt(jmr*jmr-1)*(massResolution-1)*mass);
    }


    return mass;
}
