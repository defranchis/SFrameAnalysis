// Dear emacs, this is -*- c++ -*-
// $Id: D3PDObjectNames.h 42092 2014-03-19 15:37:47Z lrehnisc $

#ifndef SFRAME_D3PDVARIABLES_D3PDObjectNames_H
#define SFRAME_D3PDVARIABLES_D3PDObjectNames_H

// STL include(s):
#include <map>
#include <iostream>

// ROOT include(s):
#include <TString.h>

// SFrame include(s):
#include "core/include/SError.h"

namespace D3PD {

  /**
  *  @short Class that returns the right branch names for ntuple type
  *
  *         This class can be used to return the right branch names 
  *         ntuple type, for instance for ntuples created by the SingleTopDPDMaker
  *         code or SM D3PDs.
  *
  * @author Clemens Lange <Clemens.Lange@desy.de>
  *
  * $Rev: 42092 $
  * $Date: 2014-03-19 16:37:47 +0100 (Wed, 19 Mar 2014) $
  */
  // pseudo-flexibility for different ntuples, also change in Particle.h
  typedef float floatingnumber;
  class D3PDObjectNames {

    public:
      /// Constructor
      D3PDObjectNames( const TString& ntupleType = "NTUP_TOP_p1400" );
      
      /// Return names saved in map 
      TString getName( const TString& name );
      TString getPrefix( const TString& name );


    private:
    
      /// Fill the map according to ntupleType
      void FillMap ( TString ntupleType ) throw( SError );
      
      /// functions filling the name maps
      //void FillD3PDMakerD3PD();
      void Fill_NTUP_COMMON();
      void Fill_NTUP_TOP_p1400();
      void Fill_NTUP_TOP_p1562();
      void Fill_NTUP_TOP_p1562_clusters();
			void Fill_NTUP_TOP_OLD();
      void FillGroomedJetsD3PD();
      void FillChargedHiggsTRCNtuple();
      void FillOld();
      void FillUZHCMSNtuple();

      //
      // THE name map
      //
      std::map<TString, TString> m_names;
      
      // and THE prefix map
      std::map<TString, TString> m_prefixes;

  }; // class D3PDObjectNames
  
} // namespace D3PD

#endif // SFRAME_D3PDVARIABLES_D3PDObjectNames_H
