
/*

  SELECTIONS - btag > 0.4

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
    , m_eventInfo( this )
    , m_tau( this )
    , m_missingEt( this )
    , m_pileupReweightingTool( this )

{

    m_logger << INFO << "Hello!" << SLogger::endmsg;
    SetLogName( GetName() );
   
    DeclareProperty( "RecoTreeName",             m_recoTreeName             = "physics" );
    DeclareProperty( "OutputTreeName",           m_outputTreeName           = "analysis" );
    DeclareProperty( "JetAK8Name",               m_jetAK8Name               = "jetAK8" );
    DeclareProperty( "TauName",                  m_tauName                  = "tau" );
    DeclareProperty( "MissingEtName",            m_missingEtName            = "MET" );
   
    DeclareProperty( "IsData",                   m_isData                   = false );
    DeclareProperty( "IsSignal",                 m_isSignal                 = true );
    DeclareProperty( "ApplyMETFilters",          m_applyMETFilters          = true );


    /////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////// SELECTIONS ////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////


    DeclareProperty( "METCut",                   m_metCut             = 50. );
   
    DeclareProperty( "jetPtCut",                 m_jetPtCut           = 500. );
    DeclareProperty( "jetEtaCut",                m_jetEtaCut          = 2.4  );
    DeclareProperty( "jetHbbtagMin",             m_jetHbbtagMin       = 0.4  );
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


    /////////////////////////////////////////////////////////////////////////////////

   
    DeclareProperty( "JSONName",                 m_jsonName             = std::string (std::getenv("SFRAME_DIR")) + "/../GoodRunsLists/JSON/Cert_246908-260627_13TeV_PromptReco_Collisions15_25ns_JSON_v2.txt" );
   
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
	

    m_triggerNames.push_back("PFHT800_v") ;
    m_triggerNames.push_back("AK8PFJet360_TrimMass30") ;
    //m_triggerNames.push_back("AK8PFHT700_TrimR0p1PT0p03Mass50") ;

    m_catNames.clear();
    m_catNames.push_back("tautau_NoWindow");

    // m_catNames.push_back("tautau_HiggsWindow");
    // m_catNames.push_back("tautau_LowHiggsSB");
    // m_catNames.push_back("tautau_HighHiggsSB");

    return;

}


void VHTausAnalysis::EndCycle() throw( SError ) {

    return;

}




void VHTausAnalysis::BeginInputData( const SInputData& id ) throw( SError ) {

    //m_logger << INFO << "JSONName:           " <<                 m_jsonName << SLogger::endmsg;
  
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
    // DeclareVariable( b_runNumber,           "b_runNumber"            );
    // DeclareVariable( b_eventNumber,         "b_eventNumber"          );
    // DeclareVariable( b_lumiBlock,           "b_lumiBlock"            );


    b_selection_bits.resize( m_catNames.size() );
    b_selection_lastcut.resize( m_catNames.size() );

    Book( TH1F( "Events", "Events;weight", 10, -.5, 9.5 ) );
    Book( TH1F( "SumEvents", "SumEvents;weight", 10, -.5, 9.5 ) );
    Book( TH1F( "METFilters", "METFilters", 20, 0.5, 20.5 ));


    for (unsigned int s=0;s<m_catNames.size();++s) {
        TString directory = m_catNames[s].c_str();

        Book( TH1F( "cutflow", "cutflow", 20, 0.5, 20.5 ), directory );  
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
    if (m_isData) {

        m_jetAK8.ConnectVariables(       m_recoTreeName.c_str(), Ntuple::JetBasic|Ntuple::JetAnalysis|Ntuple::JetSubstructure/*|Ntuple::JetPrunedSubjets*/, (m_jetAK8Name + "_").c_str() );
        m_eventInfo.ConnectVariables(    m_recoTreeName.c_str(), /*Ntuple::EventInfoBasic|*/Ntuple::EventInfoTrigger|Ntuple::EventInfoMETFilters, "" );
    }
    else {
        m_jetAK8.ConnectVariables(       m_recoTreeName.c_str(), Ntuple::JetBasic|Ntuple::JetAnalysis|Ntuple::JetSubstructure/*|Ntuple::JetTruth|Ntuple::JetPrunedSubjets|Ntuple::JetPrunedSubjetsTruth*/, (m_jetAK8Name + "_").c_str() );
        m_eventInfo.ConnectVariables(    m_recoTreeName.c_str(), Ntuple::EventInfoBasic|Ntuple::EventInfoTrigger|Ntuple::EventInfoMETFilters|Ntuple::EventInfoTruth, "" );
    }
    m_tau.ConnectVariables(         m_recoTreeName.c_str(), Ntuple::TauBasic|Ntuple::TauID|Ntuple::TauAdvancedID, (m_tauName + "_").c_str() );
    m_missingEt.ConnectVariables(    m_recoTreeName.c_str(), Ntuple::MissingEtBasic, (m_missingEtName + "_").c_str() );
    m_logger << INFO << "Connecting input variables completed" << SLogger::endmsg;

    return;

}


void VHTausAnalysis::ExecuteEvent( const SInputData&, Double_t ) throw( SError ) 
{


    m_logger << VERBOSE << "ExecuteEvent" << SLogger::endmsg;
  
    clearBranches();
  
    b_eventNumber = m_eventInfo.eventNumber;
    b_runNumber = m_eventInfo.runNumber;
    b_lumiBlock = m_eventInfo.lumiBlock;
  
    //MD vector of selection bits (a set for each cathegory)

    std::vector<TBits> selectionBits(m_catNames.size(), TBits(kNumCuts));
    for (unsigned int s=0;s<m_catNames.size();++s) 
    {
        selectionBits[s].SetBitNumber( kBeforeCuts );
    }

  
    // Cut 1: check for data if run/lumiblock in JSON
    if (m_isData) 
    {
        if (isGoodEvent(m_eventInfo.runNumber, m_eventInfo.lumiBlock)) {
            for (unsigned int s=0;s<m_catNames.size();++s) {
                selectionBits[s].SetBitNumber( kJSON );
            }
        }
    } // if is data -> JSON

    else 
    {
        for (unsigned int s=0;s<m_catNames.size();++s) {
            selectionBits[s].SetBitNumber( kJSON );
        }
    } //if is not data -> JSON = true
  
    // Cut 2: pass trigger
 




    if (m_isData)
    {
        if (passTrigger()) 
        {
            m_logger << VERBOSE << "Trigger pass" << SLogger::endmsg;
            for (unsigned int s=0;s<m_catNames.size();++s) {
                selectionBits[s].SetBitNumber( kTrigger );
            }
            // m_logger << INFO << "pass: " << selectionBits[0].TestBitNumber( kTrigger ) << SLogger::endmsg;
        } //if pass trigger

        else
        {
            m_logger <<  VERBOSE  << "Trigger fail:" << selectionBits[0].TestBitNumber( kTrigger ) << SLogger::endmsg;

            for (unsigned int s=0;s<m_catNames.size();++s) 
                m_logger << VERBOSE  << selectionBits[s].TestBitNumber( kTrigger ) << SLogger::endmsg;

        } 
    }


    else
    {
        m_logger << VERBOSE << "Trigger pass" << SLogger::endmsg;
        for (unsigned int s=0;s<m_catNames.size();++s) {
            selectionBits[s].SetBitNumber( kTrigger );
        }
        // m_logger << INFO << "pass: " << selectionBits[0].TestBitNumber( kTrigger ) << SLogger::endmsg;
    } //if not data->pass trigger





    // Cut 3: pass MET filters

    if (passMETFilters()) 
    {
        m_logger << VERBOSE << "passMETFilters" << SLogger::endmsg;
        for (unsigned int s=0;s<m_catNames.size();++s) 
            selectionBits[s].SetBitNumber( kMetFilters );

    } //if pass met filters
  




    // Cut : select met
    bool foundMet = false;
    UZH::MissingEt goodMet ( &m_missingEt, 0 );

    if (goodMet.et() > m_metCut) 
        foundMet=true;



    for (unsigned int s=0;s<m_catNames.size();++s) 
        if (m_catNames[s].find("tautau") != std::string::npos) 
            selectionBits[s].SetBitNumber( kMet );

    if (!foundMet) return;


    double met_et = goodMet.et();
    double met_phi = goodMet.phi();
    double met_px = met_et*cos(met_phi);
    double met_py = met_et*sin(met_phi);


////////////////////////////////////////////////////////////////////////////////////////////


    // Cut 4: select two fat jets
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
        bool massWindow = (prunedMass > m_jetMassMin) && (prunedMass < m_jetMassMax);
        bool isHbbtag = (myjet.Hbbtag() > m_jetHbbtagMin);
        bool isMetAngle = (deltaPhi > m_jetMetDeltaPhiMin);

        if (passCuts && isJetID && massWindow && isHbbtag && isMetAngle)
            goodFatJets.push_back(myjet);

    }//end loop on jets

    
    bool foundJet = (goodFatJets.size() > 0);
    if (!foundJet) return;

    for (unsigned int s=0;s<m_catNames.size();++s)
        selectionBits[s].SetBitNumber( kJet );
    
    m_logger << VERBOSE << "kJet" << SLogger::endmsg;

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

        if (passCuts && isTauDM && isTauBoosted && isTauIso && againstElectron && againstMuon)
            goodTaus.push_back(mytau);


    }//end loop on taus

    if (goodTaus.size() < 2) return;

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


                    if ( (sumPT >= sumPT_leading) && (sumPT > m_tautauPtCut) && (deltaR_tautau < m_tautauDRMax) &&
                         (mass > m_tautauMassMin) && (mass < m_tautauMassMax) && (deltaR_tautaujet > m_tautauJetDRMin) && 
                         (resonanceMass > m_tautauJetMassMin ) ) 

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
    if (!foundTauPair) return;

    if (tau1_TLV_sel.Pt() < tau2_TLV_sel.Pt() )
    {
        TLorentzVector temp = tau1_TLV_sel;
        tau1_TLV_sel = tau2_TLV_sel;
        tau2_TLV_sel = temp;
    }

    // std::cout<<" found taus "<<std::endl;

    for (unsigned int s=0;s<m_catNames.size();++s) 
        if (m_catNames[s].find("tautau") != std::string::npos) 
            selectionBits[s].SetBitNumber( kTau );

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
                



    // std::cout<<"selections passed!"<<std::endl<<std::endl;
// selection done
    m_logger << VERBOSE << "selection done" << SLogger::endmsg;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    b_pass = true;
    
    TLorentzVector jet, tau1, tau2, sumTau, sumAll;
    tau1.SetPtEtaPhiE(selectedTau_1.pt(), selectedTau_1.eta(), selectedTau_1.phi(), selectedTau_1.e());
    tau2.SetPtEtaPhiE(selectedTau_2.pt(), selectedTau_2.eta(), selectedTau_2.phi(), selectedTau_2.e());
    jet.SetPtEtaPhiE(selectedJet.pt(),selectedJet.eta(),selectedJet.phi(),selectedJet.e());
    sumTau = tau1 + tau2 ;
    sumAll = sumTau + jet ;

    b_selection_bits.clear();
    b_selection_lastcut.clear();
  





/////////////////////////////////////////////////////////////////////////////////////////////////////////////////




    if (!m_isData) 
    {
        b_weight = getEventWeight();


        Hist( "Events" )->Fill( 0., b_weightGen        ); // event with MC weight
        Hist( "Events" )->Fill( 1,  b_weight           ); // event total weight
        Hist( "Events" )->Fill( 2,  b_weightPU         ); // event pileup weight
        Hist( "Events" )->Fill( 9,  1                  ); // event without MC weight
    
        Hist( "SumEvents" )->Fill( 0., fabs(b_weightGen)       ); // event with MC weight
        Hist( "SumEvents" )->Fill( 1,  fabs(b_weight)          ); // event total weight
        Hist( "SumEvents" )->Fill( 2,  fabs(b_weightPU)       ); // event pileup weight
        Hist( "SumEvents" )->Fill( 9,  1                       ); // event without MC weight

        //Hist( "SumEvents" )->Fill( 6,  fabs(b_weight_jvfScale));  // event JVF SF weight

    } // end if not data fill histos with weight
  

    for (unsigned int s=0;s<m_catNames.size();++s)
    {
        // m_logger << INFO << selectionBits[s].TestBitNumber( kTrigger ) << SLogger::endmsg;
        fillCutflow("cutflow", m_catNames[s], selectionBits[s], b_weight);
    }
  
    bool doHistograms = true;

    // for (unsigned int s=0;s<m_catNames.size();++s) 
    //     if (selectionBits[s].TestBitNumber( kMassWindow )) 
    //         doHistograms = true;

    m_logger << VERBOSE << "before doHistograms" << SLogger::endmsg;
    //std::cout<<"do histos: "<<doHistograms<<std::endl;
    
    if (doHistograms) 
    {
        std::vector<bool> passed_all(m_catNames.size(), true);

        for (unsigned int s=0; s<m_catNames.size(); ++s) 
        {
            for( UInt_t icut = 0; icut < kNumCuts; ++icut )
            {
                if( selectionBits[s].TestBitNumber( icut ) != kTRUE )
                    passed_all[s] = false;
                
                else
                {
                    if (icut) b_selection_bits[s]|=1<<icut; 
                    if (icut-1==(unsigned)b_selection_lastcut[s])
                        b_selection_lastcut[s]++;
                }

            }//cut loop
            //std::cout<<"passed all "<<s<<": "<<passed_all[s]<<std::endl;

        }//category loop



        m_logger << VERBOSE << "category loopfillHistograms" << SLogger::endmsg;

        for (unsigned int s=0;s<m_catNames.size();++s) 
        {
            if (passed_all[s]) 
            {
                m_logger << VERBOSE << m_catNames[s] << SLogger::endmsg;
                fillHistograms(m_catNames[s], selectedJet, selectedTau_1, selectedTau_2, goodMet, sumTauTLV_sel, sumAllTLV_sel);
            }
        }//category loop

    } //if do histograms


    m_logger << VERBOSE << "return" << SLogger::endmsg;
    return;


} //end function


//MD returns a boolean: is run_lumisection a good event
// reads from GRL (if event is data, of course)

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

    b_selection_bits.clear();
    b_selection_lastcut.clear();
  
}

void VHTausAnalysis::fillCutflow( const std::string histName, const std::string dirName, const TBits& cutmap, const Double_t weight ) {

    // sequential cut flow -> stop at first failed cut
    // m_logger << INFO << histName << "\t" << dirName << SLogger::endmsg;

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

    Book ( TH1F("jet_mass","jet pruned mass (with JEC);jet mass (GeV)",50,0,250), directory);
    Book ( TH1F("jet_pT","jet transverse momentum;jet p_{T} (GeV)",30,0,3000), directory);
    Book ( TH1F("jet_eta","jet pseudorapidity ;jet #eta",20,-2.4,2.4), directory);
    Book ( TH1F("jet_tau21","subjettiness ratio tau21; jet #tau_{21}",20,0,1.1), directory);
    Book ( TH1F("jet_Hbbtag","double b-tagger output;bb-tagger",20,-1,1), directory);

    Book ( TH1F("tau1_pT","leading tau pT;leading tau p_{T} (GeV)",30,0,2000), directory);
    Book ( TH1F("tau1_eta","leading tau pseudorapidity ;leading tau #eta",20,-2.4,2.4), directory);

    Book ( TH1F("tau2_pT","trailing tau pT;trailing tau p_{T} (GeV)",30,0,700), directory);
    Book ( TH1F("tau2_eta","trailing tau pseudorapidity ;trailing tau #eta",20,-2.4,2.4), directory);

    Book ( TH1F("tau1_iso","leading tau isolation;leading tau isolation",22,-1.1,1.1), directory);
    Book ( TH1F("tau2_iso","trailing tau isolation;trailing tau isolation",22,-1.1,1.1), directory);
    Book ( TH1F("sum_iso","sum of tau isolations;sum of tau isolations",22,-2.2,2.2), directory);

    Book ( TH1F("MET_ET","missing energy ET;missing energy E_{T} (GeV)",40,0,2000), directory);
    Book ( TH1F("MET_px","missing energy px;missing momentum p_{x} (GeV)",20,0,1000), directory);
    Book ( TH1F("MET_py","missing energy py;missing momentum p_{y} (GeV)",20,0,1000), directory);

    Book ( TH1F("tautaujet_DP","tautau-jet angle;#Delta#phi(#tau#tau,jet)",20,0,TMath::Pi()), directory);
    Book ( TH1F("jetmet_DP","jet-met angle;#Delta#phi(jet,MET)",20,0,TMath::Pi()), directory);
    Book ( TH1F("tautaumet_DP","tautau-met angle (coll. nu);#Delta#phi(#tau#tau,MET)",20,0,TMath::Pi()), directory);
    
    Book ( TH1F("tautau_mass","tau pair invariant mass (coll. nu);m(#tau,#tau) (GeV)",30,0,250), directory);
    Book ( TH1F("tautau_pT","tau pair pT (coll. nu);tau pair p_{T} (GeV)",30,0,3000), directory);
    Book ( TH1F("tautau_eta","tau pair pseudorapidity (coll. nu); tau pair #eta",20,-2.4,2.4), directory);
    Book ( TH1F("tautau_DR","tau pair distance;#DeltaR(#tau,#tau)",30,0,2), directory);
    Book ( TH1F("taujet_DR","tautau-jet distance (coll. nu);#DeltaR(#tau#tau,jet) ",30,0,6), directory);
    Book ( TH1F("resonance_mass","tautau-jet invariant mass (coll. nu);m(#tau#tau,jet) (GeV)",50,0,5000), directory);


}



void VHTausAnalysis::fillHistograms(const TString& directory, const UZH::Jet& Jet, const UZH::Tau Tau1, const UZH::Tau Tau2, const UZH::MissingEt MET,
                                    TLorentzVector sumTau, TLorentzVector sumAll)
{


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


    TVector2 v2_met (MET.et()*cos(MET.phi()),MET.et()*sin(MET.phi()));
    TVector2 v2_taus (sumTau.Px(),sumTau.Py());
    TVector2 v2_jet (jet.Px(),jet.Py());

    Hist( "tautaujet_DP", directory )->Fill(fabs(v2_jet.DeltaPhi(v2_taus)),b_weight);    
    Hist( "jetmet_DP", directory )->Fill(fabs(v2_met.DeltaPhi(v2_jet)),b_weight);
    Hist( "tautaumet_DP", directory )->Fill(fabs(v2_met.DeltaPhi(v2_taus)),b_weight);

  


}



