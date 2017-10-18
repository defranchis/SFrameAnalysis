
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
    DeclareProperty( "IsQCD",                    m_isQCD                  = false );

    // DeclareProperty( "ApplyMETFilters",          m_applyMETFilters          = true );
    // DeclareProperty( "IsBtagSR",                 is_btag_SR                 = true );
    // DeclareProperty( "IsTauIsoSR",               is_tauiso_SR               = true );


    /////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////// SELECTIONS ////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////


    DeclareProperty( "METCut",                   m_metCut             = 50. );
   
    DeclareProperty( "jetPtCut",                 m_jetPtCut           = 350. );
    DeclareProperty( "jetEtaCut",                m_jetEtaCut          = 2.4  );
    DeclareProperty( "jetHbbtagMin",             m_jetHbbtagMin       = 0.6  );
    DeclareProperty( "jetMetDeltaPhiMin",        m_jetMetDeltaPhiMin  = TMath::Pi()/2.);
    DeclareProperty( "jetMassMin",               m_jetMassMin         = 105.  );
    DeclareProperty( "jetMassMax",               m_jetMassMax         = 135. );

    DeclareProperty( "tauPtCut",                 m_tauPtCut              = 20. );
    DeclareProperty( "tauEtaCut",                m_tauEtaCut             = 2.4 );
    DeclareProperty( "tautauPtCut",              m_tautauPtCut           = 250.);
    DeclareProperty( "tautauDRMax",              m_tautauDRMax           = 1.  );
    DeclareProperty( "tautauMetDeltaPhiMax",     m_tautauMetDeltaPhiMax  = TMath::Pi()/2.);
    DeclareProperty( "tautauMassMin",            m_tautauMassMin         = 60. );
    DeclareProperty( "tautauMassMax",            m_tautauMassMax         = 160.);

    DeclareProperty( "tautauJetDRMin",           m_tautauJetDRMin        = 1.  );
    DeclareProperty( "tautauJetMassMin",         m_tautauJetMassMin      = 800.);


    DeclareProperty( "ElectronPtCut",            m_electronPtCut         = 10. );
    DeclareProperty( "ElectronEtaCut",           m_electronEtaCut        = 2.5 );
    DeclareProperty( "MuonPtCut",                m_muonPtCut             = 10. );
    DeclareProperty( "MuonEtaCut",               m_muonEtaCut            = 2.4 );
   


    /////////////////////////////////////////////////////////////////////////////////

   
    DeclareProperty( "JSONName",                 m_jsonName             = std::string (std::getenv("SFRAME_DIR")) + "/../GoodRunsLists/JSON/Cert_246908-260627_13TeV_PromptReco_Collisions15_25ns_JSON_v2.txt" );
   
    trig_turnon_eff_HT = new TFile("~/HH/Framework/Analysis/SFrameAnalysis/VHTausResonances/trigger_turnon/trigger_HT.root");
    trig_eff_HT = (TEfficiency*) trig_turnon_eff_HT->GetObjectChecked("trigger_eff_vs_HT/eff_HT_thick","TEfficiency");

    //rand.SetSeed();
    

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
	
    //m_triggerNames.push_back("AK8PFJet360_TrimMass30") ;
    m_triggerNames.push_back("PFHT800_v") ;

    m_catNames.clear();
    m_catNames.push_back("tautau_NoWindow");
    m_catNames.push_back("0_SR");
    m_catNames.push_back("1_bveto");
    m_catNames.push_back("2_tauveto");
    m_catNames.push_back("3_allveto");

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
    m_tau.ConnectVariables( m_recoTreeName.c_str(), Ntuple::TauBasic|Ntuple::TauID|Ntuple::TauAdvancedID, (m_tauName + "_").c_str() );
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


    m_electron.ConnectVariables( m_recoTreeName.c_str(), 
                                 Ntuple::ElectronBasic|Ntuple::ElectronID|Ntuple::ElectronBoostedID|Ntuple::ElectronBoostedIsolation,
                                 (m_electronName + "_").c_str() );

    m_muon.ConnectVariables( m_recoTreeName.c_str(),
                             Ntuple::MuonBasic|Ntuple::MuonID|Ntuple::MuonIsolation| Ntuple::MuonBoostedIsolation, (m_muonName + "_").c_str() );


    m_logger << INFO << "Connecting input variables completed" << SLogger::endmsg;

    return;

}


void VHTausAnalysis::ExecuteEvent( const SInputData&, Double_t ) throw( SError ) 
{

    using namespace std;

    /*********************/

    do_Hbb_tag_SF_shift = 0;      // done
    do_Hbb_mistag_SF_shift = 0;   // done

    do_JEC_AK4_shift = 0;
    do_JEC_AK8_shift = 0;

    do_pruned_shift = 0;   // done

    do_JER_AK4_shift = 0;  // done
    do_JER_AK8_shift = 0;  // done

    do_turnon_shift = 0;   // done

    /*********************/

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


    if (event_HT<700) return;

    // cout<<endl<<"passed HT"<<endl;

    std::vector<UZH::Electron> goodElectrons;

    for ( int i = 0; i <m_electron.N; ++i ) 
    {

        UZH::Electron myelectron( &m_electron, i );

        bool passCuts = (myelectron.pt() > m_electronPtCut) && (fabs(myelectron.eta()) < m_electronEtaCut);
        bool isID = myelectron.isMediumElectronBoosted() == 1 ;

        if (passCuts && isID)

            if(( fabs(myelectron.eta()) <=  1.479 &&  (myelectron.SemileptonicCorrPFIso()/myelectron.pt()) < 0.0766) ||
               ( fabs(myelectron.eta()) >   1.479  &&  (myelectron.SemileptonicCorrPFIso()/myelectron.pt()) < 0.0678 ) )

                goodElectrons.push_back(myelectron);

    }
    
    std::vector<UZH::Muon> goodMuons;

    for ( int i = 0; i<m_muon.N ; ++i ) 
    {

        UZH::Muon mymuon( &m_muon, i );

        bool passCuts = (mymuon.pt() > m_muonPtCut) && (fabs(mymuon.eta()) < m_muonEtaCut);
        bool isID = mymuon.isLooseMuon() == 1 ;

        if (passCuts && isID)
            if (mymuon.SemileptonicCorrPFIso() /mymuon.pt() <0.25)
                goodMuons.push_back(mymuon);

    }


    int n_muons = goodMuons.size();
    int n_electrons = goodElectrons.size();

    if ((n_muons > 0) || (n_electrons > 0) ) 
        return;

    // cout<<"passed leptons"<<endl;

    // select met
    if (met_et < m_metCut || TMath::IsNaN(met_et))
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
            // goodAK8_pt.push_back(jetAK8_pt[i]);
            // goodAK8_eta.push_back(jetAK8_eta[i]);
            // goodAK8_phi.push_back(jetAK8_phi[i]);
            // goodAK8_e.push_back(jetAK8_e[i]);
            goodAK8_TLV.push_back(jetAK8_TLV[i]);
            goodAK8_Hbbtag.push_back(jetAK8_Hbbtag[i]);
            goodAK8_pruned_mass.push_back(jetAK8_pruned_mass[i]);
        }

    

    }//end loop on jets


    if (goodAK8_TLV.size() == 0) 
        return;

    // cout<<"found jet"<<endl;

    std::vector<UZH::Tau> goodTaus;

    for ( int i = 0; i<(m_tau.N); ++i )
    {

        UZH::Tau mytau( &m_tau, i );

        bool passCuts = (mytau.pt() > m_tauPtCut) && (fabs(mytau.eta()) < m_tauEtaCut);
        bool isTauDM = mytau.decayModeFindingNewDMs();
        bool isTauBoosted = mytau.TauType()==2;

        if (passCuts && isTauDM && isTauBoosted)
            goodTaus.push_back(mytau);

    }//end loop on taus


    if (goodTaus.size() < 2) 
        return;

    // cout<<"found taus"<<endl;


    TLorentzVector goodAK08TLV_sel (0,0,0,0);
    double jetPrunedMass = 0. ;
    double jetHbbtag = -2.;

    for (unsigned int i = 0; i < goodAK8_TLV.size(); ++i) 
    {

        TVector2 v2_jet (goodAK8_TLV[i].Px(), goodAK8_TLV[i].Py());
        TVector2 v2_met (met_px, met_py);
        double deltaPhi = fabs( v2_jet.DeltaPhi(v2_met) );
        
        bool isMetAngle = (deltaPhi > m_jetMetDeltaPhiMin);

        if ( (goodAK8_TLV[i].Pt() > goodAK08TLV_sel.Pt()) && isMetAngle)
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

    if (!jetMassWindow) return;

    // cout<<"passed jet"<<endl;

    if (isHbbtag)
        is_btag_SR = true;


// ////////////////////////////////////////////////////////////////////////////////////////////




    float sumPT_leading = 0;
    int iTau_sel = -1;
    int jTau_sel = -1;


    for (unsigned int iTau=0; iTau<goodTaus.size(); ++iTau)

        for (unsigned int jTau=iTau+1; jTau<goodTaus.size(); ++jTau)
        {
            TLorentzVector tau1_TLV, tau2_TLV, sumTLV;
            
            tau1_TLV.SetPtEtaPhiE( goodTaus.at(iTau).pt(), goodTaus.at(iTau).eta(), goodTaus.at(iTau).phi() ,goodTaus.at(iTau).e() );
            tau2_TLV.SetPtEtaPhiE( goodTaus.at(jTau).pt(), goodTaus.at(jTau).eta(), goodTaus.at(jTau).phi() ,goodTaus.at(jTau).e() );
            sumTLV = tau1_TLV + tau2_TLV ;

            TVector2 v2_taus (sumTLV.Px(), sumTLV.Py());
            TVector2 v2_met (met_px, met_py);

            double sumPT = sumTLV.Pt();
            float deltaPhi = fabs(v2_taus.DeltaPhi(v2_met));
            double deltaR_tautau = tau1_TLV.DeltaR(tau2_TLV);
            double deltaR_tautaujet = sumTLV.DeltaR(goodAK08TLV_sel);


            if ((sumPT >= sumPT_leading) && (deltaR_tautau < m_tautauDRMax)
                && (deltaR_tautaujet > m_tautauJetDRMin) && (deltaPhi < m_tautauMetDeltaPhiMax) )
            {
                sumPT_leading = sumPT;
                iTau_sel = iTau;
                jTau_sel = jTau;
            }


        }// loop on taus


    if ( (iTau_sel < 0) || (jTau_sel<0) )
        return;

    if (goodTaus.at(iTau_sel).pt() < goodTaus.at(jTau_sel).pt() )
    {
        int i_temp = iTau_sel;
        iTau_sel = jTau_sel;
        jTau_sel = i_temp;
    }


    UZH::Tau selectedTau_1 = goodTaus.at(iTau_sel);
    UZH::Tau selectedTau_2 = goodTaus.at(jTau_sel);

    TLorentzVector tau1_TLV_sel, tau2_TLV_sel, sumTau_TLV_sel;
    tau1_TLV_sel.SetPtEtaPhiE(goodTaus.at(iTau_sel).pt(),goodTaus.at(iTau_sel).eta(),goodTaus.at(iTau_sel).phi(),goodTaus.at(iTau_sel).e());
    tau2_TLV_sel.SetPtEtaPhiE(goodTaus.at(jTau_sel).pt(),goodTaus.at(jTau_sel).eta(),goodTaus.at(jTau_sel).phi(),goodTaus.at(jTau_sel).e());
    sumTau_TLV_sel = tau1_TLV_sel + tau2_TLV_sel ;

    double px1 = tau1_TLV_sel.Px();
    double px2 = tau2_TLV_sel.Px();
    double py1 = tau1_TLV_sel.Py();
    double py2 = tau2_TLV_sel.Py();

    if ( ((py2*px1-px2*py1) == 0) || (px1 == 0) )
        return;

    // cout<<"passed taus 2"<<endl;

    double b = ( met_py*px1 - met_px*py1 )/( py2*px1 - px2*py1 );
    double a = ( met_px -b*px2 )/px1;

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

    TLorentzVector sumAll_TLV_sel = goodAK08TLV_sel + sumTau_TLV_sel ;

    bool passIso;

    {

        bool isTauIso_VLoose_1 = selectedTau_1.byVLooseIsolationMVA3newDMwLT();
        bool isTauIso_VLoose_2 = selectedTau_2.byVLooseIsolationMVA3newDMwLT();

        bool isTauIso_Loose_1 = selectedTau_1.byLooseIsolationMVA3newDMwLT();
        bool isTauIso_Loose_2 = selectedTau_2.byLooseIsolationMVA3newDMwLT();

        bool passLoose = isTauIso_Loose_1 || isTauIso_Loose_2 ;
        bool passVLoose = isTauIso_VLoose_1 && isTauIso_VLoose_2 ;

        passIso = passLoose && passVLoose ;

    }

    if (passIso)
        is_tauiso_SR = true;
            

////////////////////////////////////////////////////////////////////////////////////////////


    b_pass = true;

    if (!m_isData) applyHbbtagSF( goodAK08TLV_sel );



    int iCat = -1;
    if (is_btag_SR && is_tauiso_SR) iCat = 1;
    else if (!is_btag_SR && is_tauiso_SR) iCat = 2;
    else if (is_btag_SR && !is_tauiso_SR) iCat = 3;
    else iCat = 4;

    fillHistograms(m_catNames[0], goodAK08TLV_sel,jetHbbtag, jetPrunedMass, selectedTau_1, selectedTau_2, sumTau_TLV_sel, sumAll_TLV_sel);
    fillHistograms(m_catNames[iCat], goodAK08TLV_sel,jetHbbtag, jetPrunedMass, selectedTau_1, selectedTau_2, sumTau_TLV_sel, sumAll_TLV_sel);
                
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
    if( !(m_eventInfo.passFilter_EEBadSc) ) {
        passMetFilters = false;
        m_logger << VERBOSE << "passFilter_EEBadSc" << SLogger::endmsg;
        Hist( "METFilters" )->Fill(5);
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
  
    b_weight = 1.;
    b_weightGen = 1.;
    b_weightPU = 1.;
  
    b_runNumber = -99;
    b_eventNumber = -99;
    b_lumiBlock = -99;


    event_HT = 0.;
    b_pass = false;
    is_btag_SR = false;
    is_tauiso_SR = false;

    // jetAK4_pt.clear() ;
    // jetAK4_eta.clear() ;
    // jetAK4_phi.clear() ;
    // jetAK4_e.clear() ;
    
    jetAK4_TLV.clear();
    jetAK8_TLV.clear();
    goodAK8_TLV.clear();

    // jetAK8_pt.clear() ;
    // jetAK8_eta.clear() ;
    // jetAK8_phi.clear() ;
    // jetAK8_e.clear() ;
    jetAK8_Hbbtag.clear() ;
    jetAK8_pruned_mass.clear() ;
    jetAK8_IDTight.clear() ;

    // goodAK8_pt.clear() ;
    // goodAK8_eta.clear() ;
    // goodAK8_phi.clear() ;
    // goodAK8_e.clear() ;


    goodAK8_Hbbtag.clear() ;
    goodAK8_pruned_mass.clear() ;
  
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

    Book ( TH1F("jet_mass","jet pruned mass (with JEC);jet pruned mass [GeV]",50,0,250), directory);
    Book ( TH1F("jet_pT","jet transverse momentum;jet p_{T} [GeV]",30,0,3000), directory);
    Book ( TH1F("jet_eta","jet pseudorapidity ;jet #eta",20,-2.4,2.4), directory);
    // Book ( TH1F("jet_tau21","subjettiness ratio tau21; jet #tau_{21}",20,0,1.1), directory);
    Book ( TH1F("jet_Hbbtag","double b-tagger output;bb-tagger output",20,-1,1), directory);

    Book ( TH1F("tau1_pT","leading tau pT;leading tau p_{T} [GeV]",30,0,2000), directory);
    Book ( TH1F("tau1_eta","leading tau pseudorapidity ;leading tau #eta",20,-2.4,2.4), directory);

    Book ( TH1F("tau2_pT","sub-leading tau pT;sub-leading tau p_{T} [GeV]",30,0,700), directory);
    Book ( TH1F("tau2_eta","sub-leading tau pseudorapidity ;sub-leading tau #eta",20,-2.4,2.4), directory);

    Book ( TH1F("tau1_iso","leading tau isolation;leading tau isolation",22,-1.1,1.1), directory);
    Book ( TH1F("tau2_iso","sub-leading tau isolation;sub-leading tau isolation",22,-1.1,1.1), directory);
    Book ( TH1F("sum_iso","sum of tau isolations;sum of tau isolations",22,-2.2,2.2), directory);

    Book ( TH1F("MET_ET","missing energy ET;missing energy E_{T} [GeV]",40,0,2000), directory);
    Book ( TH1F("MET_px","missing energy px;missing momentum p_{x} [GeV]",20,0,1000), directory);
    Book ( TH1F("MET_py","missing energy py;missing momentum p_{y} [GeV]",20,0,1000), directory);

    Book ( TH1F("tautaujet_DP","tautau-jet angle;#Delta#phi (#tau#tau,jet)",20,0,TMath::Pi()), directory);
    Book ( TH1F("jetmet_DP","jet-met angle;#Delta#phi (jet,MET)",20,0,TMath::Pi()), directory);
    Book ( TH1F("tautaumet_DP","tautau-met angle (coll. nu);#Delta#phi (#tau#tau,MET)",20,0,TMath::Pi()), directory);
    
    Book ( TH1F("tautau_mass","tau pair invariant mass (coll. nu);mass (#tau,#tau) [GeV]",30,0,250), directory);
    Book ( TH1F("tautau_pT","tau pair pT (coll. nu);tau pair p_{T} [GeV]",30,0,3000), directory);
    Book ( TH1F("tautau_eta","tau pair pseudorapidity (coll. nu); tau pair #eta",20,-2.4,2.4), directory);
    Book ( TH1F("tautau_DR","tau pair distance;#DeltaR (#tau,#tau)",30,0,2), directory);
    Book ( TH1F("taujet_DR","tautau-jet distance (coll. nu);#DeltaR (#tau#tau,jet) ",30,0,6), directory);
    Book ( TH1F("resonance_mass","tautau-jet invariant mass (coll. nu);mass (#tau#tau,jet) [GeV]",70,0,7000), directory);
    Book ( TH1F("resonance_mass_bin","tautau-jet invariant mass (coll. nu);mass (#tau#tau,jet) [GeV]",700,0,7000), directory);



}


// void VHTausAnalysis::fillHistograms( const TString& directory, const UZH::Jet& HbbJet , const UZH::Tau &Tau1, const UZH::Tau &Tau2,
//                                      const UZH::MissingEt &MET, TLorentzVector &sumTau, TLorentzVector &sumAll, 
//                                      const std::vector<UZH::Jet> & goodAK04_CSV_M )
// {
//     return;
// }




void VHTausAnalysis::fillHistograms( const TString& directory, const TLorentzVector& jet , const double& Hbbtag, const double& pruned_mass, 
                                     const UZH::Tau& Tau1, const UZH::Tau& Tau2, TLorentzVector& sumTau, TLorentzVector& sumAll)

{

    Hist( "HT", directory )->Fill( event_HT , b_weight);
    

    Hist( "jet_mass", directory )->Fill( pruned_mass , b_weight);
    Hist( "jet_pT", directory )->Fill( jet.Pt() , b_weight);
    Hist( "jet_eta", directory )->Fill( jet.Eta() , b_weight);
    Hist( "jet_Hbbtag", directory )->Fill( Hbbtag , b_weight);

    Hist( "tau1_pT", directory )->Fill( Tau1.pt() , b_weight);
    Hist( "tau1_eta", directory )->Fill( Tau1.eta() , b_weight);
    Hist( "tau1_iso", directory )->Fill( Tau1.byIsolationMVA3newDMwLTraw() , b_weight);
    Hist( "tau2_pT", directory )->Fill( Tau2.pt() , b_weight);
    Hist( "tau2_eta", directory )->Fill( Tau2.eta() , b_weight);
    Hist( "tau2_iso", directory )->Fill( Tau2.byIsolationMVA3newDMwLTraw() , b_weight);
    Hist( "sum_iso", directory )->Fill( Tau1.byIsolationMVA3newDMwLTraw() + Tau2.byIsolationMVA3newDMwLTraw() , b_weight);

    Hist( "MET_ET", directory )->Fill( met_et , b_weight);
    Hist( "MET_px", directory )->Fill( met_px , b_weight);
    Hist( "MET_py", directory )->Fill( met_py , b_weight);


    TLorentzVector tau1, tau2;
    tau1.SetPtEtaPhiE(Tau1.pt(), Tau1.eta(), Tau1.phi(), Tau1.e());
    tau2.SetPtEtaPhiE(Tau2.pt(), Tau2.eta(), Tau2.phi(), Tau2.e());

    float tautauMass = sumTau.M();
    float tautauPT = sumTau.Pt();
    float tautauEta = sumTau.Eta();
    float tautauDR = tau1.DeltaR(tau2);
    float tauJetDR = sumTau.DeltaR(jet);
    float tauJetDEta = fabs(sumTau.Eta()-jet.Eta());
    float tauJetMass = sumAll.M();

    Hist( "tautau_mass", directory )->Fill(tautauMass, b_weight);
    Hist( "tautau_pT", directory )->Fill(tautauPT, b_weight);
    Hist( "tautau_eta", directory )->Fill(tautauEta, b_weight);
    Hist( "tautau_DR", directory )->Fill(tautauDR, b_weight);
    Hist( "taujet_DR", directory )->Fill(tauJetDR, b_weight);
    Hist( "resonance_mass", directory )->Fill(tauJetMass, b_weight);
    Hist( "resonance_mass_bin", directory )->Fill(tauJetMass, b_weight);

    TVector2 v2_met (met_px, met_py);
    TVector2 v2_taus (sumTau.Px(),sumTau.Py());
    TVector2 v2_jet (jet.Px(),jet.Py());

    Hist( "tautaujet_DP", directory )->Fill(fabs(v2_jet.DeltaPhi(v2_taus)),b_weight);    
    Hist( "jetmet_DP", directory )->Fill(fabs(v2_met.DeltaPhi(v2_jet)),b_weight);
    Hist( "tautaumet_DP", directory )->Fill(fabs(v2_met.DeltaPhi(v2_taus)),b_weight);


}


void VHTausAnalysis::applyHbbtagSF (const TLorentzVector& selectedJet ){

    b_weight = getEventWeight();
    double pt = selectedJet.Pt();


    if (event_HT<1000.){
        
        if ( do_turnon_shift == 0 ) b_weight *= trig_eff_HT->GetEfficiency(trig_eff_HT->FindFixBin(event_HT));

        else if ( do_turnon_shift == 1 ) b_weight *= trig_eff_HT->GetEfficiency(trig_eff_HT->FindFixBin(event_HT)) + 
                                            trig_eff_HT->GetEfficiencyErrorUp(trig_eff_HT->FindFixBin(event_HT));

        else if ( do_turnon_shift == -1 ) b_weight *= trig_eff_HT->GetEfficiency(trig_eff_HT->FindFixBin(event_HT)) - 
                                            trig_eff_HT->GetEfficiencyErrorLow(trig_eff_HT->FindFixBin(event_HT));

    }

    //fillHistograms(m_catNames[0], goodAK08TLV_sel,jetHbbtag, jetPrunedMass, selectedTau_1, selectedTau_2, sumTau_TLV_sel, sumAll_TLV_sel);
                
                
    if (m_isSignal)
    {

        if (is_btag_SR)
        {

            if      ( pt>=300. && pt<400. ) b_weight *= SF_btag_medium_300_400 + do_Hbb_tag_SF_shift*err_SF_btag_medium_300_400;
            else if ( pt>=400. && pt<500. ) b_weight *= SF_btag_medium_400_500 + do_Hbb_tag_SF_shift*err_SF_btag_medium_400_500; 
            else if ( pt>=500. && pt<600. ) b_weight *= SF_btag_medium_500_600 + do_Hbb_tag_SF_shift*err_SF_btag_medium_500_600; 
            else if ( pt>=600.            ) b_weight *= SF_btag_medium_600_700 + do_Hbb_tag_SF_shift*err_SF_btag_medium_600_700; 
            else                            b_weight *= 1.;
        }

        else 
        {
            if      ( pt>=300. && pt<400. ) b_weight *= 2.- (SF_btag_medium_300_400 + do_Hbb_tag_SF_shift*err_SF_btag_medium_300_400);
            else if ( pt>=400. && pt<500. ) b_weight *= 2.- (SF_btag_medium_400_500 + do_Hbb_tag_SF_shift*err_SF_btag_medium_400_500); 
            else if ( pt>=500. && pt<600. ) b_weight *= 2.- (SF_btag_medium_500_600 + do_Hbb_tag_SF_shift*err_SF_btag_medium_500_600); 
            else if ( pt>=600.            ) b_weight *= 2.- (SF_btag_medium_600_700 + do_Hbb_tag_SF_shift*err_SF_btag_medium_600_700); 
            else                            b_weight *= 1.;
        }

    }//if signal


    else if (m_isTTbar)
    {
                    
        if (is_btag_SR)
        {
            if      ( pt>=300. && pt<400. ) b_weight *= mistag_SF_btag_medium_300_400 + do_Hbb_mistag_SF_shift*err_mistag_SF_btag_medium_300_400;
            else if ( pt>=400. && pt<550. ) b_weight *= mistag_SF_btag_medium_400_550 + do_Hbb_mistag_SF_shift*err_mistag_SF_btag_medium_400_550; 
            else if ( pt>=550.            ) b_weight *= mistag_SF_btag_medium_550_Inf + do_Hbb_mistag_SF_shift*err_mistag_SF_btag_medium_550_Inf; 
            else                            b_weight *= 1.;
        }

        else
        {
            if      (pt>=300. && pt<400.) b_weight *= 2.- (mistag_SF_btag_medium_300_400 + do_Hbb_mistag_SF_shift*err_mistag_SF_btag_medium_300_400);
            else if (pt>=400. && pt<550.) b_weight *= 2.- (mistag_SF_btag_medium_400_550 + do_Hbb_mistag_SF_shift*err_mistag_SF_btag_medium_400_550);
            else if (pt>=550.           ) b_weight *= 2.- (mistag_SF_btag_medium_550_Inf + do_Hbb_mistag_SF_shift*err_mistag_SF_btag_medium_550_Inf);
            else                            b_weight *= 1.;
        }


    } //if ttbar

    else b_weight *= 1.;




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

        TLorentzVector originalJetTLV = * recoJet.getTLV();
        TLorentzVector recoJetTLV = * recoJet.getTLV();

        if (do_JEC_AK4_shift == 1) recoJetTLV *= recoJet.jecUp() / recoJet.jec();
        else if (do_JEC_AK4_shift == -1) recoJetTLV *= recoJet.jecDown() / recoJet.jec();


        if (!m_isData && !m_isQCD)
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


            double JER_SF = recoJet.jer_SF();
            double sigma_MC_pT = recoJet.sigma_MC_pT() * recoJetTLV.Pt();

            if (do_JER_AK4_shift == 1) JER_SF = recoJet.jer_SF_up();
            else if (do_JER_AK4_shift == -1) JER_SF = recoJet.jer_SF_down();

            if ( (min_dist > 0) && (min_dist < 0.4/2.) )
            {
                double new_pt = pt_gen + JER_SF*(recoJetTLV.Pt()-pt_gen);
                if (new_pt < 0) new_pt = 0. ;

                //cout<<"scale\t"<<new_pt / recoJetTLV.Pt()<<endl;
                recoJetTLV *= new_pt / recoJetTLV.Pt();

            } //scaling method

            else
            {

                double new_pt = rand.Gaus( recoJetTLV.Pt(), sigma_MC_pT * pow( JER_SF*JER_SF -1., .5 ) );
                //cout<<"smear\t"<<new_pt / recoJetTLV.Pt()<<endl;
                recoJetTLV *= new_pt / recoJetTLV.Pt();

            }// smearing method

        }// not data and QCD
        
        // jetAK4_pt.push_back(recoJetTLV.Pt());
        // jetAK4_eta.push_back(recoJetTLV.Eta());
        // jetAK4_phi.push_back(recoJetTLV.Phi());
        // jetAK4_e.push_back(recoJetTLV.E());

        jetAK4_TLV.push_back(recoJetTLV);

        // cout<<"original jet pt = "<<originalJetTLV.Pt()<<endl;
        // cout<<"adding px: "<<originalJetTLV.Px() - recoJetTLV.Px()<<endl;
        // cout<<"adding py: "<<originalJetTLV.Py() - recoJetTLV.Py()<<endl;

        
        met_px += originalJetTLV.Px() - recoJetTLV.Px() ;
        met_py += originalJetTLV.Py() - recoJetTLV.Py() ;

    }// reco AK4 loop

    // cout<<"final MET: px = "<<met_px<<"\tpy = "<<met_py<<endl;

    met_et = pow( met_px*met_px + met_py*met_py , .5);

    // TVector2 old_met (goodMet.et()*cos(goodMet.phi()),goodMet.et()*sin(goodMet.phi()));
    // TVector2 new_met (met_px, met_py);

    // cout<<met_et/goodMet.et()<<"\t"<<old_met.DeltaPhi(new_met)<<endl;
    // cout<<endl;

    //jetAK8
    for ( int i = 0; i < (m_jetAK8.N); ++i )     
    {
        UZH::Jet recoJet( &m_jetAK8, i );
        TLorentzVector recoJetTLV = * recoJet.getTLV();
        double pruned_mass = recoJet.pruned_massCorr();

        if (do_JEC_AK8_shift == 1) recoJetTLV *= recoJet.jecUp() / recoJet.jec();
        else if (do_JEC_AK8_shift == -1) recoJetTLV *= recoJet.jecDown() / recoJet.jec();

        if (do_pruned_shift == 1) pruned_mass *= recoJet.pruned_jecUp() / recoJet.pruned_jec() ;
        else if (do_pruned_shift == -1) pruned_mass *= recoJet.pruned_jecDown() / recoJet.pruned_jec() ;


        if (!m_isData && !m_isQCD)
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

            double JER_SF = recoJet.jer_SF();
            double sigma_MC_pT = recoJet.sigma_MC_pT() * recoJetTLV.Pt();

            if (do_JER_AK8_shift == 1) JER_SF = recoJet.jer_SF_up();
            else if (do_JER_AK8_shift == -1) JER_SF = recoJet.jer_SF_down();

            if ( (min_dist > 0) && (min_dist < 0.8/2.) )
            {

                double new_pt = pt_gen + JER_SF*(recoJetTLV.Pt()-pt_gen);
                if (new_pt < 0) new_pt = 0. ;

                recoJetTLV *= new_pt / recoJetTLV.Pt();

            } //scaling method

            else
            {

                double new_pt = rand.Gaus( recoJetTLV.Pt(), sigma_MC_pT * pow( JER_SF*JER_SF -1., .5 ) );
                recoJetTLV *= new_pt / recoJetTLV.Pt();

            }// smearing method

        }// not data and QCD
        
        // jetAK8_pt.push_back(recoJetTLV.Pt());
        // jetAK8_eta.push_back(recoJetTLV.Eta());
        // jetAK8_phi.push_back(recoJetTLV.Phi());
        // jetAK8_e.push_back(recoJetTLV.E());

        jetAK8_TLV.push_back(recoJetTLV);

        jetAK8_Hbbtag.push_back(recoJet.Hbbtag());
        jetAK8_pruned_mass.push_back(pruned_mass);
        jetAK8_IDTight.push_back(recoJet.IDTight());


    }// reco AK8 loop

    
}//end function


