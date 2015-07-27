// $Id: CycleCreators.py 344 2012-12-13 13:10:53Z krasznaa $

// Local include(s):
#include "../include/ExoVVAnalysis.h"

#include <TMath.h>

ClassImp( ExoVVAnalysis );

ExoVVAnalysis::ExoVVAnalysis()
   : SCycleBase()
   , m_jetAK4( this )
   , m_jetAK8( this )
{

   m_logger << INFO << "Hello!" << SLogger::endmsg;
   SetLogName( GetName() );
   
   // read configuration details from XML file
   // (defaults agree with July 2010 acceptance challenge)
   DeclareProperty( "RecoTreeName",             m_recoTreeName             = "physics" );
   DeclareProperty( "JetAK4Name",               m_jetAK4Name               = "jetAK4" );
   DeclareProperty( "JetAK8Name",               m_jetAK8Name               = "jetAK8" );
   DeclareProperty( "JetPtLowCut",              m_jetPtLowCut              = 200. );
   DeclareProperty( "JetPtHighCut",             m_jetPtHighCut             = 500. );
   DeclareProperty( "JetEtaCut",                m_jetEtaCut                = 2.4 );
   
}

ExoVVAnalysis::~ExoVVAnalysis() {

  m_logger << INFO << "Tschoe!" << SLogger::endmsg;

}

void ExoVVAnalysis::BeginCycle() throw( SError ) {

  m_logger << INFO << "Hello to you!" << SLogger::endmsg;
   return;

}

void ExoVVAnalysis::EndCycle() throw( SError ) {

   return;

}

void ExoVVAnalysis::BeginInputData( const SInputData& ) throw( SError ) {

  m_logger << INFO << "RecoTreeName:            " << m_recoTreeName << SLogger::endmsg;
  m_logger << INFO << "JetAK4Name:              " << m_jetAK4Name << SLogger::endmsg;
  m_logger << INFO << "JetAK8Name:              " << m_jetAK8Name << SLogger::endmsg;
  m_logger << INFO << "JetPtLowCut:             " << m_jetPtLowCut << SLogger::endmsg;
  m_logger << INFO << "JetPtHighCut:            " << m_jetPtHighCut << SLogger::endmsg;
  m_logger << INFO << "JetEtaCut:               " << m_jetEtaCut << SLogger::endmsg;
   return;

}

void ExoVVAnalysis::EndInputData( const SInputData& ) throw( SError ) {

   return;

}

void ExoVVAnalysis::BeginInputFile( const SInputData& ) throw( SError ) {

  m_logger << INFO << "Connecting input variables" << SLogger::endmsg;
  m_jetAK4.ConnectVariables(          m_recoTreeName.c_str(), D3PD::JetBasic|D3PD::JetAnalysis, (m_jetAK4Name + "_").c_str() );
  m_jetAK8.ConnectVariables(          m_recoTreeName.c_str(), D3PD::JetBasic|D3PD::JetAnalysis|D3PD::JetSubstructure, (m_jetAK8Name + "_").c_str() );
  m_logger << INFO << "Connecting input variables completed" << SLogger::endmsg;
   return;

}

void ExoVVAnalysis::ExecuteEvent( const SInputData&, Double_t ) throw( SError ) {

  std::vector<DESY::Jet> goodJetsAK4;
  for ( int i = 0; i < (m_jetAK4.N); ++i ) {
    DESY::Jet myjet( &m_jetAK4, i );
    if (fabs(myjet.eta()) < m_jetEtaCut) {
      if (myjet.pt() > m_jetPtLowCut) {
        goodJetsAK4.push_back(myjet);
        // m_logger << INFO << myjet.pt() << SLogger::endmsg;
        Book( TH1F( "Jet_pt", "Jet p_{T} [GeV]", 100, 0.0, 1000 ), "AK4_low" )->Fill( myjet.pt() );
        Book( TH1F( "Jet_eta", "Jet #eta", 50, -2.5, 2.5 ), "AK4_low" )->Fill( myjet.eta() );
        Book( TH1F( "Jet_m", "Jet m [GeV]", 50, 0, 250 ), "AK4_low" )->Fill( myjet.m() );
        Book( TH1F( "Jet_phi", "Jet #phi", 50, -TMath::Pi(), TMath::Pi() ), "AK4_low" )->Fill( myjet.phi() );
        Book( TH1F( "Jet_e", "Jet e [GeV]", 100, 0.0, 1000 ), "AK4_low" )->Fill( myjet.e() );
        
        Book( TH1F( "Jet_muf", "Jet muf", 50, 0.0, 1.0 ), "AK4_low" )->Fill( myjet.muf() );
        Book( TH1F( "Jet_phf", "Jet phf", 50, 0.0, 1.0 ), "AK4_low" )->Fill( myjet.phf() );
        Book( TH1F( "Jet_emf", "Jet emf", 50, 0.0, 1.0 ), "AK4_low" )->Fill( myjet.emf() );
        Book( TH1F( "Jet_nhf", "Jet nhf", 50, 0.0, 1.0 ), "AK4_low" )->Fill( myjet.nhf() );
        Book( TH1F( "Jet_chf", "Jet chf", 50, 0.0, 1.0 ), "AK4_low" )->Fill( myjet.chf() );
        Book( TH1F( "Jet_area", "Jet area", 40, 0.0, 4.0 ), "AK4_low" )->Fill( myjet.area() );
        Book( TH1F( "Jet_cm", "Jet cm", 50, 0.0, 50.0 ), "AK4_low" )->Fill( myjet.cm() );
        Book( TH1F( "Jet_nm", "Jet nm", 50, 0.0, 50.0 ), "AK4_low" )->Fill( myjet.nm() );
      }
      if (myjet.pt() > m_jetPtHighCut) {
        goodJetsAK4.push_back(myjet);
        // m_logger << INFO << myjet.pt() << SLogger::endmsg;
        Book( TH1F( "Jet_pt", "Jet p_{T} [GeV]", 100, 0.0, 1000 ), "AK4_high" )->Fill( myjet.pt() );
        Book( TH1F( "Jet_eta", "Jet #eta", 50, -2.5, 2.5 ), "AK4_high" )->Fill( myjet.eta() );
        Book( TH1F( "Jet_m", "Jet m [GeV]", 50, 0, 250 ), "AK4_high" )->Fill( myjet.m() );
        Book( TH1F( "Jet_phi", "Jet #phi", 50, -TMath::Pi(), TMath::Pi() ), "AK4_high" )->Fill( myjet.phi() );
        Book( TH1F( "Jet_e", "Jet e [GeV]", 100, 0.0, 1000 ), "AK4_high" )->Fill( myjet.e() );
        
        Book( TH1F( "Jet_muf", "Jet muf", 50, 0.0, 1.0 ), "AK4_high" )->Fill( myjet.muf() );
        Book( TH1F( "Jet_phf", "Jet phf", 50, 0.0, 1.0 ), "AK4_high" )->Fill( myjet.phf() );
        Book( TH1F( "Jet_emf", "Jet emf", 50, 0.0, 1.0 ), "AK4_high" )->Fill( myjet.emf() );
        Book( TH1F( "Jet_nhf", "Jet nhf", 50, 0.0, 1.0 ), "AK4_high" )->Fill( myjet.nhf() );
        Book( TH1F( "Jet_chf", "Jet chf", 50, 0.0, 1.0 ), "AK4_high" )->Fill( myjet.chf() );
        Book( TH1F( "Jet_area", "Jet area", 40, 0.0, 4.0 ), "AK4_high" )->Fill( myjet.area() );
        Book( TH1F( "Jet_cm", "Jet cm", 50, 0.0, 50.0 ), "AK4_high" )->Fill( myjet.cm() );
        Book( TH1F( "Jet_nm", "Jet nm", 50, 0.0, 50.0 ), "AK4_high" )->Fill( myjet.nm() );
      }
    }
  }
  
  
  double mW_min = 60;
  double mW_max = 100;
  
  std::vector<DESY::Jet> goodJetsAK8;
  for ( int i = 0; i < (m_jetAK8.N); ++i ) {
    DESY::Jet myjet( &m_jetAK8, i );
    if (fabs(myjet.eta()) < m_jetEtaCut) {
      if (myjet.pt() > m_jetPtLowCut) {
        goodJetsAK8.push_back(myjet);
        // m_logger << INFO << myjet.pt() << SLogger::endmsg;
        Book( TH1F( "Jet_pt", "Jet p_{T} [GeV]", 100, 0.0, 1000 ), "AK8_low" )->Fill( myjet.pt() );
        Book( TH1F( "Jet_eta", "Jet #eta", 50, -2.5, 2.5 ), "AK8_low" )->Fill( myjet.eta() );
        Book( TH1F( "Jet_m", "Jet m [GeV]", 50, 0, 250 ), "AK8_low" )->Fill( myjet.m() );
        Book( TH1F( "Jet_phi", "Jet #phi", 50, -TMath::Pi(), TMath::Pi() ), "AK8_low" )->Fill( myjet.phi() );
        Book( TH1F( "Jet_e", "Jet e [GeV]", 100, 0.0, 1000 ), "AK8_low" )->Fill( myjet.e() );
        
        Book( TH1F( "Jet_muf", "Jet muf", 50, 0.0, 1.0 ), "AK8_low" )->Fill( myjet.muf() );
        Book( TH1F( "Jet_phf", "Jet phf", 50, 0.0, 1.0 ), "AK8_low" )->Fill( myjet.phf() );
        Book( TH1F( "Jet_emf", "Jet emf", 50, 0.0, 1.0 ), "AK8_low" )->Fill( myjet.emf() );
        Book( TH1F( "Jet_nhf", "Jet nhf", 50, 0.0, 1.0 ), "AK8_low" )->Fill( myjet.nhf() );
        Book( TH1F( "Jet_chf", "Jet chf", 50, 0.0, 1.0 ), "AK8_low" )->Fill( myjet.chf() );
        Book( TH1F( "Jet_area", "Jet area", 40, 0.0, 4.0 ), "AK8_low" )->Fill( myjet.area() );
        Book( TH1F( "Jet_cm", "Jet cm", 50, 0.0, 50.0 ), "AK8_low" )->Fill( myjet.cm() );
        Book( TH1F( "Jet_nm", "Jet nm", 50, 0.0, 50.0 ), "AK8_low" )->Fill( myjet.nm() );
        
        Book( TH1F( "Jet_tau1", "Jet tau1", 50, 0, 1.0 ), "AK8_low" )->Fill( myjet.tau1() );
        Book( TH1F( "Jet_tau2", "Jet tau2", 50, 0, 1.0 ), "AK8_low" )->Fill( myjet.tau2() );
        Book( TH1F( "Jet_tau3", "Jet tau3", 50, 0, 1.0 ), "AK8_low" )->Fill( myjet.tau3() );
        
        Book( TH1F( "Jet_tau21", "Jet tau21", 50, 0, 1.0 ), "AK8_low" )->Fill( myjet.tau2()/myjet.tau1() );
        Book( TH1F( "Jet_tau32", "Jet tau32", 50, 0, 1.0 ), "AK8_low" )->Fill( myjet.tau3()/myjet.tau2() );
        Book( TH1F( "Jet_tau31", "Jet tau31", 50, 0, 1.0 ), "AK8_low" )->Fill( myjet.tau3()/myjet.tau1() );
        
        Book( TH1F( "Jet_pruned_m", "Jet pruned m [GeV]", 50, 0, 250 ), "AK8_low" )->Fill( myjet.pruned_mass() );
        Book( TH1F( "Jet_softdrop_m", "Jet softdrop m [GeV]", 50, 0, 250 ), "AK8_low" )->Fill( myjet.softdrop_mass() );
        
        if ((myjet.pruned_mass() > mW_min) && (myjet.pruned_mass() < mW_max))
          Book( TH1F( "Jet_pruned_Wwindow_m", "Jet pruned W window m [GeV]", 50, mW_min, mW_max ), "AK8_low" )->Fill( myjet.pruned_mass() );
        if ((myjet.softdrop_mass() > mW_min) && (myjet.softdrop_mass() < mW_max))
          Book( TH1F( "Jet_softdrop_Wwindow_m", "Jet softdrop W window m [GeV]", 50, mW_min, mW_max ), "AK8_low" )->Fill( myjet.softdrop_mass() );
        
      }
      if (myjet.pt() > m_jetPtHighCut) {
        goodJetsAK8.push_back(myjet);
        // m_logger << INFO << myjet.pt() << SLogger::endmsg;
        Book( TH1F( "Jet_pt", "Jet p_{T} [GeV]", 100, 0.0, 1000 ), "AK8_high" )->Fill( myjet.pt() );
        Book( TH1F( "Jet_eta", "Jet #eta", 50, -2.5, 2.5 ), "AK8_high" )->Fill( myjet.eta() );
        Book( TH1F( "Jet_m", "Jet m [GeV]", 50, 0, 250 ), "AK8_high" )->Fill( myjet.m() );
        Book( TH1F( "Jet_phi", "Jet #phi", 50, -TMath::Pi(), TMath::Pi() ), "AK8_high" )->Fill( myjet.phi() );
        Book( TH1F( "Jet_e", "Jet e [GeV]", 100, 0.0, 1000 ), "AK8_high" )->Fill( myjet.e() );
        
        Book( TH1F( "Jet_muf", "Jet muf", 50, 0.0, 1.0 ), "AK8_high" )->Fill( myjet.muf() );
        Book( TH1F( "Jet_phf", "Jet phf", 50, 0.0, 1.0 ), "AK8_high" )->Fill( myjet.phf() );
        Book( TH1F( "Jet_emf", "Jet emf", 50, 0.0, 1.0 ), "AK8_high" )->Fill( myjet.emf() );
        Book( TH1F( "Jet_nhf", "Jet nhf", 50, 0.0, 1.0 ), "AK8_high" )->Fill( myjet.nhf() );
        Book( TH1F( "Jet_chf", "Jet chf", 50, 0.0, 1.0 ), "AK8_high" )->Fill( myjet.chf() );
        Book( TH1F( "Jet_area", "Jet area", 40, 0.0, 4.0 ), "AK8_high" )->Fill( myjet.area() );
        Book( TH1F( "Jet_cm", "Jet cm", 50, 0.0, 50.0 ), "AK8_high" )->Fill( myjet.cm() );
        Book( TH1F( "Jet_nm", "Jet nm", 50, 0.0, 50.0 ), "AK8_high" )->Fill( myjet.nm() );
        
        Book( TH1F( "Jet_tau1", "Jet tau1", 50, 0, 1.0 ), "AK8_high" )->Fill( myjet.tau1() );
        Book( TH1F( "Jet_tau2", "Jet tau2", 50, 0, 1.0 ), "AK8_high" )->Fill( myjet.tau2() );
        Book( TH1F( "Jet_tau3", "Jet tau3", 50, 0, 1.0 ), "AK8_high" )->Fill( myjet.tau3() );
        
        Book( TH1F( "Jet_tau21", "Jet tau21", 50, 0, 1.0 ), "AK8_high" )->Fill( myjet.tau2()/myjet.tau1() );
        Book( TH1F( "Jet_tau32", "Jet tau32", 50, 0, 1.0 ), "AK8_high" )->Fill( myjet.tau3()/myjet.tau2() );
        Book( TH1F( "Jet_tau31", "Jet tau31", 50, 0, 1.0 ), "AK8_high" )->Fill( myjet.tau3()/myjet.tau1() );
        
        Book( TH1F( "Jet_pruned_m", "Jet pruned m [GeV]", 50, 0, 250 ), "AK8_high" )->Fill( myjet.pruned_mass() );
        Book( TH1F( "Jet_softdrop_m", "Jet softdrop m [GeV]", 50, 0, 250 ), "AK8_high" )->Fill( myjet.softdrop_mass() );
        
        if ((myjet.pruned_mass() > mW_min) && (myjet.pruned_mass() < mW_max))
          Book( TH1F( "Jet_pruned_Wwindow_m", "Jet pruned W window m [GeV]", 50, mW_min, mW_max ), "AK8_high" )->Fill( myjet.pruned_mass() );
        if ((myjet.softdrop_mass() > mW_min) && (myjet.softdrop_mass() < mW_max))
          Book( TH1F( "Jet_softdrop_Wwindow_m", "Jet softdrop W window m [GeV]", 50, mW_min, mW_max ), "AK8_high" )->Fill( myjet.softdrop_mass() );
      }
    }
  }
  
   return;

}

