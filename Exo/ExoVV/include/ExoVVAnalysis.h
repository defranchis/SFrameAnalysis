// Dear emacs, this is -*- c++ -*-
// $Id: CycleCreators.py 344 2012-12-13 13:10:53Z krasznaa $
#ifndef ExoVVAnalysis_H
#define ExoVVAnalysis_H

// SFrame include(s):
#include "core/include/SCycleBase.h"

// External include(s):
#include "../../Common/D3PDVariables/include/JetD3PDObject.h"
#include "../../Common/D3PDVariables/include/Jet.h"

class TH1D;
class TH2D;
class TRandom3;
namespace DESY {
  class Jet;
}

/**
 *   @short Put short description of class here
 *
 *          Put a longer description over here...
 *
 *  @author Put your name here
 * @version $Revision: 344 $
 */
class ExoVVAnalysis : public SCycleBase {

public:
   /// Default constructor
   ExoVVAnalysis();
   /// Default destructor
   ~ExoVVAnalysis();

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

private:
   //
   // Put all your private variables here
   //
  
  //
  // Input variable objects:
  //
  D3PD::JetD3PDObject       m_jetAK4;            ///< jet container
  D3PD::JetD3PDObject       m_jetAK8;            ///< jet container
  
  //
  // XML settings for ColourFlowSelection
  //
  std::string m_recoTreeName;       ///< name of tree with reconstructed objects in D3PD
  std::string m_jetAK4Name;            ///< name of jet collection in tree with reconstructed objects
  std::string m_jetAK8Name;       ///< name of truth jet collection in tree with reconstructed objects
  double      m_jetPtLowCut;             ///< cut on jet pT
  double      m_jetPtHighCut;             ///< cut on jet pT
  double      m_jetEtaCut;             ///< cut on jet eta

   // Macro adding the functions for dictionary generation
   ClassDef( ExoVVAnalysis, 0 );

}; // class ExoVVAnalysis

#endif // ExoVVAnalysis_H

