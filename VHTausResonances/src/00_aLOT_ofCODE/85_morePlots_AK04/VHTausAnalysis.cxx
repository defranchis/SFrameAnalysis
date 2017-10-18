
/*
  
  SELECTIONS
  LOWER PT THRESHOLD
  TRIGGER EFFICIENCY APPLIED ON MC
  BBTAG SF APPLIED ON SIGNAL AND TTBAR
  BOTH SIGNAL AND SIDEBAND REGIONS

  ESTIMATE SYST UNCERT FROM BBTAG SF

*/


#include "../include/VHTausAnalysis.h"
#include "../GoodRunsLists/include/TGoodRunsListReader.h"
#include <TMath.h>

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
    DeclareProperty( "ElectronName",             m_electronName             = "el" );
    DeclareProperty( "MuonName",                 m_muonName                 = "mu" );
    DeclareProperty( "TauName",                  m_tauName                  = "tau" );
    DeclareProperty( "MissingEtName",            m_missingEtName            = "MET" );
    DeclareProperty( "GenParticleName",          m_genParticleName          = "genParticle" );

   
    DeclareProperty( "IsData",                   m_isData                   = false );
    DeclareProperty( "IsSignal",                 m_isSignal                 = true );
    DeclareProperty( "IsTTbar",                  m_isTTbar                  = false );
    DeclareProperty( "ApplyMETFilters",          m_applyMETFilters          = true );

    DeclareProperty( "IsBtagSR",                 is_btag_SR                 = true );
    // DeclareProperty( "IsTauIsoSR",               is_tauiso_SR               = true );



    /////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////// SELECTIONS ////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////


    DeclareProperty( "METCut",                   m_metCut             = 50. );
   
    DeclareProperty( "jetPtCut",                 m_jetPtCut           = 350. );
    DeclareProperty( "jetEtaCut",                m_jetEtaCut          = 2.4  );
    DeclareProperty( "jetHbbtagMin",             m_jetHbbtagMin       = 0.3  );
    DeclareProperty( "jetMetDeltaPhiMin",        m_jetMetDeltaPhiMin  = TMath::Pi()/2.);
    //DeclareProperty( "jetMassMin",               m_jetMassMin         = 105.  );
    DeclareProperty( "jetMassMin",               m_jetMassMin         = 50.  );
    //DeclareProperty( "jetMassMax",               m_jetMassMax         = 135. );

    DeclareProperty( "tauPtCut",                 m_tauPtCut              = 20. );
    DeclareProperty( "tauEtaCut",                m_tauEtaCut             = 2.4 );
    DeclareProperty( "tautauPtCut",              m_tautauPtCut           = 250.);
    DeclareProperty( "tautauDRMax",              m_tautauDRMax           = 1.  );
    DeclareProperty( "tautauMetDeltaPhiMax",     m_tautauMetDeltaPhiMax  = TMath::Pi()/2.);
    DeclareProperty( "tautauMassMin",            m_tautauMassMin         = 60. );
    DeclareProperty( "tautauMassMax",            m_tautauMassMax         = 160.);

    DeclareProperty( "tautauJetDRMin",           m_tautauJetDRMin        = 1.  );
    DeclareProperty( "tautauJetMassMin",         m_tautauJetMassMin      = 800.);


    /////////////////////////////////////////////////////////////////////////////////

   
    DeclareProperty( "JSONName",                 m_jsonName             = std::string (std::getenv("SFRAME_DIR")) + "/../GoodRunsLists/JSON/Cert_246908-260627_13TeV_PromptReco_Collisions15_25ns_JSON_v2.txt" );
   
    trig_turnon_eff_HT = new TFile("~/HH/Framework/Analysis/SFrameAnalysis/VHTausResonances/trigger_turnon/trigger_HT.root");
    trig_eff_HT = (TEfficiency*) trig_turnon_eff_HT->GetObjectChecked("trigger_eff_vs_HT/eff_HT_thick","TEfficiency");

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
    //m_catNames.push_back("tautau_NoWindow");
    // m_catNames.push_back("0_SR");
    // m_catNames.push_back("1_bveto");
    // m_catNames.push_back("2_tauveto");
    // m_catNames.push_back("3_allveto");

    m_catNames.push_back("tautau_NoWindow");

    if (m_isTTbar)
    {
        m_catNames.push_back("0_lep");
        m_catNames.push_back("1_lep");
        m_catNames.push_back("2_lep");
    }



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
    DeclareVariable( n_lepW,"n_lepW" );

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

    if (m_isData)
        m_eventInfo.ConnectVariables( m_recoTreeName.c_str(), Ntuple::EventInfoTrigger|Ntuple::EventInfoMETFilters, "" );

    else
        m_eventInfo.ConnectVariables( m_recoTreeName.c_str(), Ntuple::EventInfoBasic|Ntuple::EventInfoTrigger|Ntuple::EventInfoMETFilters|Ntuple::EventInfoTruth, "" );

    m_tau.ConnectVariables(         m_recoTreeName.c_str(), Ntuple::TauBasic|Ntuple::TauID|Ntuple::TauAdvancedID, (m_tauName + "_").c_str() );
    m_missingEt.ConnectVariables(    m_recoTreeName.c_str(), Ntuple::MissingEtBasic, (m_missingEtName + "_").c_str() );
    m_jetAK8.ConnectVariables( m_recoTreeName.c_str(), Ntuple::JetBasic|Ntuple::JetAnalysis|Ntuple::JetSubstructure, (m_jetAK8Name + "_").c_str() );
    m_jetAK4.ConnectVariables( m_recoTreeName.c_str(), Ntuple::JetBasic, (m_jetAK4Name + "_").c_str() );
    m_logger << INFO << "Connecting input variables completed" << SLogger::endmsg;

    if (m_isTTbar)
        m_genParticle.ConnectVariables(  m_recoTreeName.c_str(), Ntuple::GenParticleBasic, (m_genParticleName + "_").c_str() );


    m_electron.ConnectVariables(     m_recoTreeName.c_str(), Ntuple::ElectronBasic|Ntuple::ElectronID|Ntuple::ElectronBoostedID|Ntuple::ElectronBoostedIsolation, (m_electronName + "_").c_str() );

    m_muon.ConnectVariables(         m_recoTreeName.c_str(), Ntuple::MuonBasic|Ntuple::MuonID|Ntuple::MuonIsolation| Ntuple::MuonBoostedIsolation, (m_muonName + "_").c_str() );



    return;

}


void VHTausAnalysis::ExecuteEvent( const SInputData&, Double_t ) throw( SError ) 
{

    using namespace std;
    clearBranches();

    if (m_isTTbar)
    {
        n_lepW = 0;

        for (int iGenPart = 0; iGenPart <  m_genParticle.N; ++iGenPart)
        {

            UZH::GenParticle myGenPart ( &m_genParticle, iGenPart );

            int pdgId = myGenPart.pdgId();
            int n_Dau = myGenPart.nDau();
        
            if ( abs(pdgId) == 24 && n_Dau == 2 )
            {
                int dau_1 = myGenPart.dau().at(0);

                if ( abs(dau_1) >= 11 && abs(dau_1) <= 16  )
                    ++ n_lepW;
            }

        }// loop gen particles
    }


  
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


    // compute event HT

    std::vector<UZH::Jet> goodAK04_CSV_M;

    for ( int i = 0; i < (m_jetAK4.N); ++i ) 
    {
        UZH::Jet myjet( &m_jetAK4, i );

        double pt = myjet.pt();
        double eta = myjet.eta();
        
        {
            bool passCuts =  (pt > 40.) && (fabs(eta) < 3.);
            if (passCuts) event_HT += pt;
        }

        // medium CSV
        bool isID = myjet.IDTight();
        bool passCuts = (pt > 50.) && (fabs(eta) < 2.4);
        bool isMediumCSV = ( myjet.csv() > 0.89 );

        if ( isID && passCuts && isMediumCSV )
            goodAK04_CSV_M.push_back(myjet);
        

    }


    if (event_HT<700) return;



/////////////////////////////////////////////////

    std::vector<UZH::Muon> goodMuons;

    for ( int i = 0; i<(m_muon.N); ++i )
    {
        UZH::Muon mymuon( &m_muon, i );

        bool passCuts = (mymuon.pt() > 30.) && (fabs(mymuon.eta()) < 2.4);
        bool isMuonID = mymuon.isTightMuon();

        if (passCuts && isMuonID)
            goodMuons.push_back(mymuon);

    }//end loop on muons

    n_muons = goodMuons.size();

    std::vector<UZH::Electron> goodElectrons;

    for ( int i = 0; i<(m_electron.N); ++i )
    {
        UZH::Electron myelectron( &m_electron, i );

        bool passCuts = (myelectron.pt() > 30.) && (fabs(myelectron.eta()) < 2.4);
        bool isElectronID = myelectron.isMediumElectron();

        if (passCuts && isElectronID)
            goodElectrons.push_back(myelectron);

    }//end loop on electrons

    n_electrons = goodElectrons.size();

    // if ((n_muons == 0) && (n_electrons == 0) ) 
    //     return;

    UZH::Muon selectedMuon;
    UZH::Electron selectedElectron;

    if (n_muons > 0)
    {
        double max_pt = 0.;
        
        for (int iMu = 0; iMu<n_muons; ++iMu )
        {
            double pt = goodMuons.at(iMu).pt();
            if (pt > max_pt)
            {
                max_pt = pt;
                selectedMuon = goodMuons.at(iMu);
            }
        }

    }

    if (n_electrons > 0)
    {
        double max_pt = 0.;
        
        for (int iEle = 0; iEle<n_electrons; ++iEle )
        {
            double pt = goodElectrons.at(iEle).pt();
            if (pt > max_pt)
            {
                max_pt = pt;
                selectedElectron = goodElectrons.at(iEle);
            }
        }

    }





    // select met
    bool foundMet = false;
    UZH::MissingEt goodMet ( &m_missingEt, 0 );

    if (goodMet.et() > m_metCut) 
        foundMet=true;

    if (!foundMet) return;

    double met_et = goodMet.et();
    double met_phi = goodMet.phi();
    double met_px = met_et*cos(met_phi);
    double met_py = met_et*sin(met_phi);


////////////////////////////////////////////////////////////////////////////////////////////


    // select AK08 jet
    std::vector<UZH::Jet> goodFatJets;

    for ( int i = 0; i < (m_jetAK8.N); ++i ) 
    {
        UZH::Jet myjet( &m_jetAK8, i );

        TLorentzVector jetTLV;
        jetTLV.SetPtEtaPhiE(myjet.pt(), myjet.eta(), myjet.phi(), myjet.e());

        TVector2 v2_jet (jetTLV.Px(), jetTLV.Py());
        TVector2 v2_met (met_px, met_py);
        double deltaPhi = fabs( v2_jet.DeltaPhi(v2_met) );
        double prunedMass = myjet.pruned_massCorr();

        bool isJetID = (myjet.IDTight()) && (myjet.muf() < 0.8);
        bool passCuts =  (myjet.pt() > m_jetPtCut) && (fabs(myjet.eta()) < m_jetEtaCut);
        bool massWindow = (prunedMass > m_jetMassMin); //&& (prunedMass < m_jetMassMax);
        //bool isHbbtag = (myjet.Hbbtag() > m_jetHbbtagMin);
        bool isMetAngle = (deltaPhi > m_jetMetDeltaPhiMin);

        if (passCuts && isJetID && massWindow && isMetAngle /*&& isHbbtag*/)
            goodFatJets.push_back(myjet);


    }//end loop on jets

    
    bool foundJet = (goodFatJets.size() > 0);
    if (!foundJet) 
        return;

    int leadingFatJetIndex = -1;
    float leadingPT = 0.;

    for (unsigned int i = 0; i < goodFatJets.size(); ++i) 
        if (goodFatJets.at(i).pt()>leadingPT)
        {
            leadingPT = goodFatJets.at(i).pt();
            leadingFatJetIndex = i;
        }


    UZH::Jet selectedJet = goodFatJets.at(leadingFatJetIndex);
    double jetPrunedMass_sel = selectedJet.pruned_massCorr();
    TLorentzVector jetAK08TLV_sel;
    jetAK08TLV_sel.SetPtEtaPhiE(selectedJet.pt(), selectedJet.eta(), selectedJet.phi(), selectedJet.e());


////////////////////////////////////////////////////////////////////////////////////////////


    std::vector<UZH::Tau> goodTaus;

    for ( int i = 0; i<(m_tau.N); ++i )
    {

        UZH::Tau mytau( &m_tau, i );

        bool passCuts = mytau.pt()>m_tauPtCut && fabs(mytau.eta())<m_tauEtaCut;
        bool isTauDM = mytau.decayModeFindingNewDMs();
        bool isTauIso = mytau.byVLooseIsolationMVA3newDMwLT();
        bool isTauBoosted = mytau.TauType()==2;
        bool againstElectron = mytau.againstElectronVLooseMVA5();
        bool againstMuon = mytau.againstMuonLoose3();


        if (passCuts && isTauDM && isTauBoosted && againstElectron && againstMuon && isTauIso)
            goodTaus.push_back(mytau);


    }//end loop on taus


    if (goodTaus.size() < 2)
        return;

    bool foundTauPair = false;
    float sumPT_leading = 0;
    int iTau_sel = -1;
    int jTau_sel = -1;
    TLorentzVector tau1_TLV_sel, tau2_TLV_sel, sumTauTLV_sel;

    for (unsigned int iTau=0; iTau<goodTaus.size(); ++iTau)

        for (unsigned int jTau=iTau+1; jTau<goodTaus.size(); ++jTau)
        {
            TLorentzVector tau1_TLV, tau2_TLV, sumTLV;
            
            tau1_TLV.SetPtEtaPhiE( goodTaus.at(iTau).pt(), goodTaus.at(iTau).eta(), goodTaus.at(iTau).phi() ,goodTaus.at(iTau).e() );
            tau2_TLV.SetPtEtaPhiE( goodTaus.at(jTau).pt(), goodTaus.at(jTau).eta(), goodTaus.at(jTau).phi() ,goodTaus.at(jTau).e() );
            sumTLV = tau1_TLV + tau2_TLV ;

            TVector2 v2_taus (sumTLV.Px(), sumTLV.Py());
            TVector2 v2_met (met_px, met_py);
            float deltaPhi = fabs(v2_taus.DeltaPhi(v2_met));

            if ( deltaPhi < m_tautauMetDeltaPhiMax )
            {

                double px1 = tau1_TLV.Px();
                double px2 = tau2_TLV.Px();
                double py1 = tau1_TLV.Py();
                double py2 = tau2_TLV.Py();

                if ( (py2*px1 - px2*py1) != 0 && (px1 != 0) )
                {

                    double b = ( met_py*px1 - met_px*py1 )/( py2*px1 - px2*py1 );
                    double a = ( met_px -b*px2 )/px1;

                    tau1_TLV *= (1+a) ;
                    tau2_TLV *= (1+b) ;
                    sumTLV = tau1_TLV + tau2_TLV ;

                    double sumPT = sumTLV.Pt();
                    double deltaR_tautau = tau1_TLV.DeltaR(tau2_TLV);
                    double mass = sumTLV.M();
                    double deltaR_tautaujet = sumTLV.DeltaR(jetAK08TLV_sel);
                    double resonanceMass = ( jetAK08TLV_sel + sumTLV ).M();

                    //double sumIso = goodTaus.at(iTau).byIsolationMVA3newDMwLTraw() + goodTaus.at(jTau).byIsolationMVA3newDMwLTraw();

                    if ((sumPT >= sumPT_leading) && (sumPT > m_tautauPtCut) && (deltaR_tautau < m_tautauDRMax)
                        && (deltaR_tautaujet > m_tautauJetDRMin) && (resonanceMass > m_tautauJetMassMin)
                        /*&& (mass > m_tautauMassMin) && (mass < m_tautauMassMax) *//*&& ( sumIso > -0.5 )*/)
                    {
                        sumPT_leading = sumPT;
                        iTau_sel = iTau;
                        jTau_sel = jTau;
                        tau1_TLV_sel = tau1_TLV;
                        tau2_TLV_sel = tau2_TLV;
                        sumTauTLV_sel = sumTLV;
                    }


                }// a, b parameters
            }// deltaphi
        }// loop on taus


    if (iTau_sel>=0 && jTau_sel>=0) foundTauPair = true;
    if (!foundTauPair) //return;
        return;

    if (tau1_TLV_sel.Pt() < tau2_TLV_sel.Pt() )
    {
        TLorentzVector temp = tau1_TLV_sel;
        tau1_TLV_sel = tau2_TLV_sel;
        tau2_TLV_sel = temp;
    }


    TLorentzVector sumAllTLV_sel = jetAK08TLV_sel + sumTauTLV_sel ;
    UZH::Tau selectedTau_1, selectedTau_2;
    TLorentzVector sumTau_TLV;
    
    if (goodTaus.at(iTau_sel).pt()>goodTaus.at(jTau_sel).pt())
    {
        selectedTau_1 = goodTaus.at(iTau_sel);
        selectedTau_2 = goodTaus.at(jTau_sel);
    }

    else
    {
        selectedTau_1 = goodTaus.at(jTau_sel);
        selectedTau_2 = goodTaus.at(iTau_sel);
    }
                


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    b_pass = true;

    TLorentzVector jet, tau1, tau2, sumTau, sumAll;
    tau1.SetPtEtaPhiE(selectedTau_1.pt(), selectedTau_1.eta(), selectedTau_1.phi(), selectedTau_1.e());
    tau2.SetPtEtaPhiE(selectedTau_2.pt(), selectedTau_2.eta(), selectedTau_2.phi(), selectedTau_2.e());
    jet.SetPtEtaPhiE(selectedJet.pt(),selectedJet.eta(),selectedJet.phi(),selectedJet.e());
    sumTau = tau1 + tau2 ;
    sumAll = sumTau + jet ;

    if (!m_isData)
    {
        b_weight = getEventWeight();
        double pt = selectedJet.pt();

        if (event_HT<1000.)
            b_weight *= trig_eff_HT->GetEfficiency(trig_eff_HT->FindFixBin(event_HT));

                // if (m_isSignal)
                // {
                //     double factor = 0.;
                    
                //     if (is_btag_SR)
                //     {

                //         if      ( pt>=300. && pt<400. ) b_weight *= SF_btag_loose_300_400 + factor*err_SF_btag_loose_300_400;
                //         else if ( pt>=400. && pt<500. ) b_weight *= SF_btag_loose_400_500 + factor*err_SF_btag_loose_400_500; 
                //         else if ( pt>=500. && pt<600. ) b_weight *= SF_btag_loose_500_600 + factor*err_SF_btag_loose_500_600; 
                //         else if ( pt>=600.            ) b_weight *= SF_btag_loose_600_700 + factor*err_SF_btag_loose_600_700; 
                //         else                            b_weight *= 1.;
                //     }

                //     else 
                //     {
                //         if      ( pt>=300. && pt<400. ) b_weight *= 2.- (SF_btag_loose_300_400 + factor*err_SF_btag_loose_300_400);
                //         else if ( pt>=400. && pt<500. ) b_weight *= 2.- (SF_btag_loose_400_500 + factor*err_SF_btag_loose_400_500); 
                //         else if ( pt>=500. && pt<600. ) b_weight *= 2.- (SF_btag_loose_500_600 + factor*err_SF_btag_loose_500_600); 
                //         else if ( pt>=600.            ) b_weight *= 2.- (SF_btag_loose_600_700 + factor*err_SF_btag_loose_600_700); 
                //         else                            b_weight *= 1.;
                //     }

                // }//if signal

                // else if (m_isTTbar)
                // {
                //     double factor = 0.;
                    
                //     if (is_btag_SR)
                //     {
                //         if      ( pt>=300. && pt<400. ) b_weight *= mistag_SF_btag_loose_300_400 + factor*err_mistag_SF_btag_loose_300_400;
                //         else if ( pt>=400. && pt<550. ) b_weight *= mistag_SF_btag_loose_400_550 + factor*err_mistag_SF_btag_loose_400_550; 
                //         else if ( pt>=550.            ) b_weight *= mistag_SF_btag_loose_550_Inf + factor*err_mistag_SF_btag_loose_550_Inf; 
                //         else                            b_weight *= 1.;
                //     }

                //     else
                //     {
                //         if      ( pt>=300. && pt<400. ) b_weight *= 2.- (mistag_SF_btag_loose_300_400 + factor*err_mistag_SF_btag_loose_300_400);
                //         else if ( pt>=400. && pt<550. ) b_weight *= 2.- (mistag_SF_btag_loose_400_550 + factor*err_mistag_SF_btag_loose_400_550); 
                //         else if ( pt>=550.            ) b_weight *= 2.- (mistag_SF_btag_loose_550_Inf + factor*err_mistag_SF_btag_loose_550_Inf); 
                //         else                            b_weight *= 1.;
                //     }


                // } //if ttbar

                // else b_weight *= 1.;


    }//if not data


    fillHistograms(m_catNames[0], selectedJet, selectedTau_1, selectedTau_2, goodMet, 
                   sumTauTLV_sel, sumAllTLV_sel, goodAK04_CSV_M );


    if (m_isTTbar){
        fillHistograms(m_catNames[n_lepW+1], selectedJet, selectedTau_1, selectedTau_2, goodMet, 
                       sumTauTLV_sel, sumAllTLV_sel, goodAK04_CSV_M);
    }

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

    b_pass = false;
    event_HT = 0.;

    b_selection_bits.clear();
    b_selection_lastcut.clear();

    n_lepW = -1;
    n_electrons = -1;
    n_muons = -1;

  
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

    Book ( TH1F("HT","event HT;HT [GeV]",50,0,5000), directory);

    Book ( TH1F("jet_mass","jet pruned mass (with JEC);jet pruned mass [GeV]",50,0,250), directory);
    Book ( TH1F("jet_pT","jet transverse momentum;jet p_{T} [GeV]",30,0,3000), directory);
    Book ( TH1F("jet_eta","jet pseudorapidity ;jet #eta",20,-2.4,2.4), directory);
    Book ( TH1F("jet_tau21","subjettiness ratio tau21; jet #tau_{21}",20,0,1.1), directory);
    Book ( TH1F("jet_Hbbtag","double b-tagger output;bb-tagger output",20,-1,1), directory);

    Book ( TH1F("tau1_pT","leading tau pT;leading tau p_{T} [GeV]",30,0,2000), directory);
    Book ( TH1F("tau1_eta","leading tau pseudorapidity ;leading tau #eta",20,-2.4,2.4), directory);

    Book ( TH1F("tau2_pT","sub-leading tau pT;sub-leading tau p_{T} [GeV]",30,0,700), directory);
    Book ( TH1F("tau2_eta","sub-leading tau pseudorapidity ;sub-leading tau #eta",20,-2.4,2.4), directory);

    Book ( TH1F("tau1_iso","leading tau isolation;leading tau isolation",22,-1.1,1.1), directory);
    Book ( TH1F("tau2_iso","sub-leading tau isolation;sub-leading tau isolation",22,-1.1,1.1), directory);
    Book ( TH1F("sum_iso","sum of tau isolations;sum of tau isolations",22,-2.2,2.2), directory);
    Book ( TH1F("sum_iso_thick","sum of tau isolations;sum of tau isolations",110,-2.2,2.2), directory);

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

    Book ( TH1F("n_lepW_TTbar","number of leptonic W in ttbar ; n. lept. W",3,-.5,2.5), directory);
    Book ( TH1F("n_mediumCSV_AK04","number of medium b-tagged (CSV) ak04 ; n. medium CSV b-tagged AK04",6,-.5,5.5), directory);

    Book ( TH1F("jetAK04_pT","ak04 jet transverse momentum;jet ak04 p_{T} [GeV]",200,0,2000), directory);
    Book ( TH1F("jetAK04_csv","ak04 jet CSV; jet ak04 CSV",100,0,1), directory);
    //Book ( TH1F("jetAK04_mass","ak04 jet mass; jet ak04 pruned mass [GeV]",300,0,300), directory);


}


void VHTausAnalysis::fillHistograms(const TString& directory, const UZH::Jet& Jet, const UZH::Tau Tau1, const UZH::Tau Tau2, const UZH::MissingEt MET,
                                    TLorentzVector sumTau, TLorentzVector sumAll)
{

    Hist( "HT", directory )->Fill( event_HT , b_weight);

    Hist( "jet_mass", directory )->Fill( Jet.pruned_massCorr() , b_weight);
    Hist( "jet_pT", directory )->Fill( Jet.pt() , b_weight);
    Hist( "jet_eta", directory )->Fill( Jet.eta() , b_weight);
    Hist( "jet_tau21", directory )->Fill( Jet.tau2()/Jet.tau1() , b_weight);
    Hist( "jet_Hbbtag", directory )->Fill( Jet.Hbbtag() , b_weight);

    Hist( "tau1_pT", directory )->Fill( Tau1.pt() , b_weight);
    Hist( "tau1_eta", directory )->Fill( Tau1.eta() , b_weight);
    Hist( "tau1_iso", directory )->Fill( Tau1.byIsolationMVA3newDMwLTraw() , b_weight);
    Hist( "tau2_pT", directory )->Fill( Tau2.pt() , b_weight);
    Hist( "tau2_eta", directory )->Fill( Tau2.eta() , b_weight);
    Hist( "tau2_iso", directory )->Fill( Tau2.byIsolationMVA3newDMwLTraw() , b_weight);
    Hist( "sum_iso", directory )->Fill( Tau1.byIsolationMVA3newDMwLTraw() + Tau2.byIsolationMVA3newDMwLTraw() , b_weight);

    Hist( "MET_ET", directory )->Fill( MET.et() , b_weight);
    Hist( "MET_px", directory )->Fill( MET.et()*cos(MET.phi()) , b_weight);
    Hist( "MET_py", directory )->Fill( MET.et()*sin(MET.phi()) , b_weight);

    TLorentzVector tau1, tau2, jet;
    tau1.SetPtEtaPhiE(Tau1.pt(), Tau1.eta(), Tau1.phi(), Tau1.e());
    tau2.SetPtEtaPhiE(Tau2.pt(), Tau2.eta(), Tau2.phi(), Tau2.e());
    jet.SetPtEtaPhiE(Jet.pt(),Jet.eta(),Jet.phi(),Jet.e());

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

    TVector2 v2_met (MET.et()*cos(MET.phi()),MET.et()*sin(MET.phi()));
    TVector2 v2_taus (sumTau.Px(),sumTau.Py());
    TVector2 v2_jet (jet.Px(),jet.Py());

    Hist( "tautaujet_DP", directory )->Fill(fabs(v2_jet.DeltaPhi(v2_taus)),b_weight);    
    Hist( "jetmet_DP", directory )->Fill(fabs(v2_met.DeltaPhi(v2_jet)),b_weight);
    Hist( "tautaumet_DP", directory )->Fill(fabs(v2_met.DeltaPhi(v2_taus)),b_weight);
    
    if (m_isTTbar)
        Hist( "n_lepW_TTbar", directory )->Fill(n_lepW,b_weight);

    Hist ("h_electrons", directory)->Fill(n_electrons, b_weight);
    Hist ("h_muons", directory)->Fill(n_muons, b_weight);
}




void VHTausAnalysis::fillHistograms( const TString& directory, const UZH::Jet& Jet , const UZH::Tau &Tau1, const UZH::Tau &Tau2,
                                     const UZH::MissingEt &MET, TLorentzVector &sumTau, TLorentzVector &sumAll, const UZH::Muon &mu,
                                     const UZH::Electron &ele )
{

    Hist( "HT", directory )->Fill( event_HT , b_weight);

    Hist( "jet_mass", directory )->Fill( Jet.pruned_massCorr() , b_weight);
    Hist( "jet_pT", directory )->Fill( Jet.pt() , b_weight);
    Hist( "jet_eta", directory )->Fill( Jet.eta() , b_weight);
    Hist( "jet_tau21", directory )->Fill( Jet.tau2()/Jet.tau1() , b_weight);
    Hist( "jet_Hbbtag", directory )->Fill( Jet.Hbbtag() , b_weight);

    Hist( "tau1_pT", directory )->Fill( Tau1.pt() , b_weight);
    Hist( "tau1_eta", directory )->Fill( Tau1.eta() , b_weight);
    Hist( "tau1_iso", directory )->Fill( Tau1.byIsolationMVA3newDMwLTraw() , b_weight);
    Hist( "tau2_pT", directory )->Fill( Tau2.pt() , b_weight);
    Hist( "tau2_eta", directory )->Fill( Tau2.eta() , b_weight);
    Hist( "tau2_iso", directory )->Fill( Tau2.byIsolationMVA3newDMwLTraw() , b_weight);
    Hist( "sum_iso", directory )->Fill( Tau1.byIsolationMVA3newDMwLTraw() + Tau2.byIsolationMVA3newDMwLTraw() , b_weight);
    Hist( "sum_iso_thick", directory )->Fill( Tau1.byIsolationMVA3newDMwLTraw() + Tau2.byIsolationMVA3newDMwLTraw() , b_weight);

    Hist( "MET_ET", directory )->Fill( MET.et() , b_weight);
    Hist( "MET_px", directory )->Fill( MET.et()*cos(MET.phi()) , b_weight);
    Hist( "MET_py", directory )->Fill( MET.et()*sin(MET.phi()) , b_weight);

    TLorentzVector tau1, tau2, jet;
    tau1.SetPtEtaPhiE(Tau1.pt(), Tau1.eta(), Tau1.phi(), Tau1.e());
    tau2.SetPtEtaPhiE(Tau2.pt(), Tau2.eta(), Tau2.phi(), Tau2.e());
    jet.SetPtEtaPhiE(Jet.pt(),Jet.eta(),Jet.phi(),Jet.e());

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

    TVector2 v2_met (MET.et()*cos(MET.phi()),MET.et()*sin(MET.phi()));
    TVector2 v2_taus (sumTau.Px(),sumTau.Py());
    TVector2 v2_jet (jet.Px(),jet.Py());

    Hist( "tautaujet_DP", directory )->Fill(fabs(v2_jet.DeltaPhi(v2_taus)),b_weight);    
    Hist( "jetmet_DP", directory )->Fill(fabs(v2_met.DeltaPhi(v2_jet)),b_weight);
    Hist( "tautaumet_DP", directory )->Fill(fabs(v2_met.DeltaPhi(v2_taus)),b_weight);
    
    if (m_isTTbar)
        Hist( "n_lepW_TTbar", directory )->Fill(n_lepW,b_weight);

    Hist ("h_electrons", directory)->Fill(n_electrons, b_weight);
    Hist ("h_muons", directory)->Fill(n_muons, b_weight);

    if (n_muons > 0){
        Hist ("muon_pT", directory)->Fill(mu.pt(), b_weight);
        // Hist ("muon_Iso", directory)->Fill(mu.pfRhoCorrRelIso03()/mu.pt(), b_weight);
        // Hist ("muon_Iso_thick", directory)->Fill(mu.pfRhoCorrRelIso03()/mu.pt(), b_weight);
    }

    if (n_electrons > 0)
        Hist ("electron_pT", directory)->Fill(ele.pt(), b_weight);

    return;

}

void VHTausAnalysis::fillHistograms( const TString& directory, const UZH::Jet& Jet , const UZH::Tau &Tau1, const UZH::Tau &Tau2,
                                     const UZH::MissingEt &MET, TLorentzVector &sumTau, TLorentzVector &sumAll, 
                                     const std::vector<UZH::Jet> & goodAK04_CSV_M)
{

    Hist( "HT", directory )->Fill( event_HT , b_weight);

    Hist( "jet_mass", directory )->Fill( Jet.pruned_massCorr() , b_weight);
    Hist( "jet_pT", directory )->Fill( Jet.pt() , b_weight);
    Hist( "jet_eta", directory )->Fill( Jet.eta() , b_weight);
    Hist( "jet_tau21", directory )->Fill( Jet.tau2()/Jet.tau1() , b_weight);
    Hist( "jet_Hbbtag", directory )->Fill( Jet.Hbbtag() , b_weight);

    Hist( "tau1_pT", directory )->Fill( Tau1.pt() , b_weight);
    Hist( "tau1_eta", directory )->Fill( Tau1.eta() , b_weight);
    Hist( "tau1_iso", directory )->Fill( Tau1.byIsolationMVA3newDMwLTraw() , b_weight);
    Hist( "tau2_pT", directory )->Fill( Tau2.pt() , b_weight);
    Hist( "tau2_eta", directory )->Fill( Tau2.eta() , b_weight);
    Hist( "tau2_iso", directory )->Fill( Tau2.byIsolationMVA3newDMwLTraw() , b_weight);
    Hist( "sum_iso", directory )->Fill( Tau1.byIsolationMVA3newDMwLTraw() + Tau2.byIsolationMVA3newDMwLTraw() , b_weight);
    Hist( "sum_iso_thick", directory )->Fill( Tau1.byIsolationMVA3newDMwLTraw() + Tau2.byIsolationMVA3newDMwLTraw() , b_weight);

    Hist( "MET_ET", directory )->Fill( MET.et() , b_weight);
    Hist( "MET_px", directory )->Fill( MET.et()*cos(MET.phi()) , b_weight);
    Hist( "MET_py", directory )->Fill( MET.et()*sin(MET.phi()) , b_weight);

    TLorentzVector tau1, tau2, jet;
    tau1.SetPtEtaPhiE(Tau1.pt(), Tau1.eta(), Tau1.phi(), Tau1.e());
    tau2.SetPtEtaPhiE(Tau2.pt(), Tau2.eta(), Tau2.phi(), Tau2.e());
    jet.SetPtEtaPhiE(Jet.pt(),Jet.eta(),Jet.phi(),Jet.e());

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

    TVector2 v2_met (MET.et()*cos(MET.phi()),MET.et()*sin(MET.phi()));
    TVector2 v2_taus (sumTau.Px(),sumTau.Py());
    TVector2 v2_jet (jet.Px(),jet.Py());

    Hist( "tautaujet_DP", directory )->Fill(fabs(v2_jet.DeltaPhi(v2_taus)),b_weight);    
    Hist( "jetmet_DP", directory )->Fill(fabs(v2_met.DeltaPhi(v2_jet)),b_weight);
    Hist( "tautaumet_DP", directory )->Fill(fabs(v2_met.DeltaPhi(v2_taus)),b_weight);

    Hist( "resonance_mass", directory )->Fill(tauJetMass, b_weight);
    Hist( "resonance_mass_bin", directory )->Fill(tauJetMass, b_weight);

    // Book ( TH1F("n_mediumCSV_AK04","number of medium b-tagged (CSV) ak04 ; n. medium CSV b-tagged AK04",6,-.5,5.5), directory);
    // Book ( TH1F("jetAK04_pT","ak04 jet transverse momentum;jet ak04 p_{T} [GeV]",30,0,3000), directory);
    // Book ( TH1F("jetAK04_csv","ak04 jet CSV; jet ak04 CSV",100,0,1), directory);
    // Book ( TH1F("jetAK04_mass","ak04 jet mass; jet ak04 pruned mass [GeV]",100,0,300), directory);
    // goodAK04_CSV_M

    if (m_isTTbar)
        Hist( "n_lepW_TTbar", directory )->Fill(n_lepW,b_weight);

    Hist("n_mediumCSV_AK04", directory )->Fill(goodAK04_CSV_M.size(), b_weight);

    for (unsigned int iAK4 = 0; iAK4 < goodAK04_CSV_M.size(); ++iAK4)
    {
        Hist("jetAK04_pT", directory )->Fill( goodAK04_CSV_M.at(iAK4).pt(), b_weight);
        Hist("jetAK04_csv", directory )->Fill( goodAK04_CSV_M.at(iAK4).csv(), b_weight);
        //Hist("jetAK04_mass", directory )->Fill( goodAK04_CSV_M.at(iAK4).pruned_massCorr(), b_weight);
    }


    return;
}
