
/*

  B-tag efficiency

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


    DeclareProperty( "jetPtCut",                 m_jetPtCut           = 500. );
    DeclareProperty( "jetEtaCut",                m_jetEtaCut          = 2.4  );
    DeclareProperty( "jetHbbtagMin",             m_jetHbbtagMin       = 0.4  );
    DeclareProperty( "jetMassMin",               m_jetMassMin         = 50.  );


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

    m_catNames.clear();
    m_catNames.push_back("tautau_NoWindow");

    return;

}


void VHTausAnalysis::EndCycle() throw( SError ) {

    return;

}




void VHTausAnalysis::BeginInputData( const SInputData& id ) throw( SError ) {

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
  

    DeclareVariable( b_pass,"pass" );
    DeclareVariable( b_sel,"sel" );

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
  

    for (unsigned int s=0;s<m_catNames.size();++s) 
        if (m_catNames[s].find("tautau") != std::string::npos) 
            selectionBits[s].SetBitNumber( kMet );




////////////////////////////////////////////////////////////////////////////////////////////


    // Cut 4: select two fat jets
    std::vector<UZH::Jet> goodFatJets;

    for ( int i = 0; i < (m_jetAK8.N); ++i ) 
    {
        UZH::Jet myjet( &m_jetAK8, i );

        double prunedMass = myjet.pruned_massCorr();
        bool passCuts =  (myjet.pt() > m_jetPtCut) && (fabs(myjet.eta()) < m_jetEtaCut);
        bool massWindow = (prunedMass > m_jetMassMin) ;



        if (passCuts && massWindow)
            goodFatJets.push_back(myjet);

    }//end loop on jets

    
    bool foundJet = (goodFatJets.size() > 0);
    if (!foundJet) return;

    for (unsigned int s=0;s<m_catNames.size();++s)
        selectionBits[s].SetBitNumber( kJet );

    for (unsigned int s=0;s<m_catNames.size();++s) 
        if (m_catNames[s].find("tautau") != std::string::npos) 
            selectionBits[s].SetBitNumber( kTau );


    int leadingFatJetIndex = -1;
    float leadingPT = 0.;

    for (unsigned int i = 0; i < goodFatJets.size(); ++i) 
        if (goodFatJets.at(i).pt()>leadingPT)
        {
            leadingPT = goodFatJets.at(i).pt();
            leadingFatJetIndex = i;
        }


    UZH::Jet selectedJet = goodFatJets.at(leadingFatJetIndex);
    bool isJetID = (selectedJet.IDTight()) && (selectedJet.muf() < 0.8);
    bool isHbbtag = (selectedJet.Hbbtag() > m_jetHbbtagMin);

    if (isJetID && isHbbtag) b_pass = true;



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
                //fillHistograms(m_catNames[s], selectedJet, selectedTau_1, selectedTau_2, goodMet, sumTauTLV_sel, sumAllTLV_sel);
                fill(m_catNames[s]);
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

    Book ( TH1F("pass","pass",2,0,2), directory);

}



void VHTausAnalysis::fillHistograms(const TString& directory, const UZH::Jet& Jet, const UZH::Tau Tau1, const UZH::Tau Tau2, const UZH::MissingEt MET,
                                    TLorentzVector sumTau, TLorentzVector sumAll)
{

}



void VHTausAnalysis::fill(const TString& directory)
{

    Hist( "pass", directory )->Fill( b_pass , b_weight);

}
