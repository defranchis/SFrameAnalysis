// Dear emacs, this is -*- c++ -*-
// $Id: CycleCreators.py 344 2012-12-13 13:10:53Z krasznaa $
#ifndef WHAnalysis_H
#define WHAnalysis_H

// SFrame include(s):
#include "core/include/SCycleBase.h"

// External include(s):
#include "../../Common/D3PDVariables/include/JetD3PDObject.h"
#include "../../Common/D3PDVariables/include/Jet.h"
#include "../../Common/D3PDVariables/include/EventInfoD3PDObject.h"
#include "../../Common/D3PDVariables/include/ElectronD3PDObject.h"
#include "../../Common/D3PDVariables/include/Electron.h"
#include "../../Common/D3PDVariables/include/MuonD3PDObject.h"
#include "../../Common/D3PDVariables/include/Muon.h"
#include "../../Common/D3PDVariables/include/MissingEtD3PDObject.h"
#include "../../Common/D3PDVariables/include/MissingEt.h"
#include "../../Common/D3PDVariables/include/GenParticleD3PDObject.h"
#include "../../Common/D3PDVariables/include/GenParticle.h"
#include "../../GoodRunsLists/include/TGoodRunsList.h"
#include "../../Common/PileupReweightingTool/include/PileupReweightingTool.h"

class TH1D;
class TH2D;
class TRandom3;
namespace DESY {
  class Jet;
  class Electron;
  class Muon;
  class MissingEt;
  class GenParticle;
}

/**
 *   @short Put short description of class here
 *
 *          Put a longer description over here...
 *
 *  @author Put your name here
 * @version $Revision: 344 $
 */
class WHAnalysis : public SCycleBase {

public:
   /// Default constructor
   WHAnalysis();
   /// Default destructor
   ~WHAnalysis();

   /// Function called at the beginning of the cycle
   virtual void BeginCycle() throw( SError );
   /// Function called at the end of the cycle
   virtual void EndCycle() throw( SError );

   /// Function called at the beginning of a new input data
   virtual void BeginInputData( const SInputData& ) throw( SError );
   /// Function called after finishing to process an input data
   virtual void EndInputData  ( const SInputData& ) throw( SError );

   /// Function called after opening each new input file
   virtual void BeginInputFile( const SInputData& ) throw( SError );

   /// Function called for every event
   virtual void ExecuteEvent( const SInputData&, Double_t ) throw( SError );
   
   /// Function to check good lumi section
   virtual bool isGoodEvent(int runNumber, int lumiSection);
   
   /// Function to check for trigger pass
   virtual bool passTrigger();
   
   /// Function to clear/reset all output branches
   virtual void clearBranches();

private:
   //
   // Put all your private variables here
   //
  
  //
  // Input variable objects:
  //
  D3PD::JetD3PDObject         m_jetAK4;            ///< jet container
  D3PD::JetD3PDObject         m_jetAK8;            ///< jet container
  D3PD::EventInfoD3PDObject   m_eventInfo; ///< event info container
  D3PD::ElectronD3PDObject    m_electron;            ///< electron container
  D3PD::MuonD3PDObject        m_muon;            ///< muon container
  D3PD::MissingEtD3PDObject   m_missingEt;            ///< missing E_T container
  D3PD::GenParticleD3PDObject m_genParticle;            ///< gen particle container
  
  
  //
  // Further objects
  //
  Root::TGoodRunsList m_grl;
  PileupReweightingTool m_pileupReweightingTool;
  
  //
  // XML settings for WHAnalysis
  //
  // naming
  std::string m_recoTreeName;       ///< name of tree with reconstructed objects in ntuple
  std::string m_outputTreeName;    ///< name of output tree
  std::string m_jetAK4Name;            ///< name of AK4 jet collection in tree with reconstructed objects
  std::string m_jetAK8Name;       ///< name of AK8 jet collection in tree with reconstructed objects
  std::string m_electronName;       ///< name of electron collection in tree with reconstructed objects
  std::string m_muonName;       ///< name of muon collection in tree with reconstructed objects
  std::string m_missingEtName;       ///< name of missing E_T collection in tree with reconstructed objects
  std::string m_genParticleName;       ///< name of gen particle collection in tree with reconstructed objects
  
  // flags
  bool        m_isData;
  bool        m_isSignal;
  bool        m_applyMETFilters;
  
  // cuts
  // jets
  double      m_jetPtCut;         ///< cut on jet pT
  double      m_jetEtaCut;             ///< cut on jet eta
  double      m_mjjCut;
  double      m_jetDeltaEtaCut;
  // substructure cuts
  double      m_tau21HPCut;
  double      m_tau21LPCut;
  double      m_mWLowerCut;
  double      m_mWUpperCut;
  double      m_mZLowerCut;
  double      m_mZUpperCut;
  double      m_mHLowerCut;
  double      m_mHUpperCut;
  // b-tagging
  double      m_csvMin;
  
  
  // electrons
  double         m_electronPtCut;
  double         m_electronEtaCut;

  // muons
  double         m_muonPtCut;
  double         m_muonEtaCut;
  
  // MET
  double         m_metCut;
   
  // further settings
  std::string m_pileupProfileData;
  std::string m_jsonName;
  
  // other variables needed
  std::vector<std::string> m_triggerNames;
  
  ///
  /// branches
  ///
  double b_weight;

   // Macro adding the functions for dictionary generation
   ClassDef( WHAnalysis, 0 );

}; // class WHAnalysis

#endif // WHAnalysis_H

