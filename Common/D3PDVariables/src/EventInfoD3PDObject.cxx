// $Id: EventInfoD3PDObject.cxx 43055 2015-01-29 16:26:25Z clange $

// Local include(s):
#include "../include/EventInfoD3PDObject.h"

namespace D3PD {

  EventInfoD3PDObject::EventInfoD3PDObject( SCycleBaseNTuple* parent ) : SInputVariables< SCycleBaseNTuple >( parent ) {
  }


  void EventInfoD3PDObject::ConnectVariables( const TString& treeName,
                                              UInt_t detail_level,
                                              const TString& prefix,
                                              const TString& ntupleType ) throw( SError ) {

    // get instance of D3PDObjectNames
    D3PDObjectNames m_objectNames(ntupleType);
    //
    // variables to be connected:
    //
    ConnectVariable( treeName, m_objectNames.getName("runNumber"), runNumber );
    ConnectVariable( treeName, m_objectNames.getName("eventNumber"), eventNumber );
    ConnectVariable( treeName, m_objectNames.getName("lumiBlock"  ), lumiBlock            );

    if( ( (detail_level & D3PD::EventInfoBasic) == D3PD::EventInfoBasic) ) {
      ConnectVariable( treeName, m_objectNames.getName("averageIntPerXing"    ), averageIntPerXing    );
      ConnectVariable( treeName, m_objectNames.getName("actualIntPerXing"     ), actualIntPerXing     );
      ConnectVariable( treeName, m_objectNames.getName("bunchCrossing"        ), bunchCrossing        );
    }
    if( (detail_level & D3PD::EventInfoTrigger) == D3PD::EventInfoTrigger ) {
      ConnectVariable( treeName, m_objectNames.getName("trigDecision"), trigDecision  );
    }
    if( (detail_level & D3PD::EventInfoTruth) == D3PD::EventInfoTruth ) {
      ConnectVariable( treeName, m_objectNames.getName("genEventWeight"   ), genEventWeight     );
    }
    if( (detail_level & D3PD::EventInfoPDF) == D3PD::EventInfoPDF  ) {
      ConnectVariable( treeName, m_objectNames.getName("pdf_id"), pdf_id );
      ConnectVariable( treeName, m_objectNames.getName("pdf_x"), pdf_x );
      ConnectVariable( treeName, m_objectNames.getName("pdf_xPDF"), pdf_xPDF );
      ConnectVariable( treeName, m_objectNames.getName("pdf_scale"), pdf_scale );
    }
    if( (detail_level & D3PD::EventInfoMETFilters) == D3PD::EventInfoMETFilters  ) {
      ConnectVariable( treeName, m_objectNames.getName("passFilter_HBHE"), passFilter_HBHE );
      ConnectVariable( treeName, m_objectNames.getName("passFilter_HBHEIso"), passFilter_HBHEIso );
      ConnectVariable( treeName, m_objectNames.getName("passFilter_HBHELoose"), passFilter_HBHELoose );
      ConnectVariable( treeName, m_objectNames.getName("passFilter_CSCHalo"), passFilter_CSCHalo );
      ConnectVariable( treeName, m_objectNames.getName("passFilter_HCALlaser"), passFilter_HCALlaser );
      ConnectVariable( treeName, m_objectNames.getName("passFilter_ECALDeadCell"), passFilter_ECALDeadCell );
      ConnectVariable( treeName, m_objectNames.getName("passFilter_GoodVtx"), passFilter_GoodVtx );
      ConnectVariable( treeName, m_objectNames.getName("passFilter_TrkFailure"), passFilter_TrkFailure );
      ConnectVariable( treeName, m_objectNames.getName("passFilter_EEBadSc"), passFilter_EEBadSc );
      ConnectVariable( treeName, m_objectNames.getName("passFilter_ECALlaser"), passFilter_ECALlaser );
      ConnectVariable( treeName, m_objectNames.getName("passFilter_TrkPOG"), passFilter_TrkPOG );
      ConnectVariable( treeName, m_objectNames.getName("passFilter_TrkPOG_manystrip"), passFilter_TrkPOG_manystrip );
      ConnectVariable( treeName, m_objectNames.getName("passFilter_TrkPOG_toomanystrip"), passFilter_TrkPOG_toomanystrip );
      ConnectVariable( treeName, m_objectNames.getName("passFilter_TrkPOG_logError"), passFilter_TrkPOG_logError );
      ConnectVariable( treeName, m_objectNames.getName("passFilter_METFilters"), passFilter_METFilters );
      ConnectVariable( treeName, m_objectNames.getName("PV_filter"), PV_filter );
    }
    
    return;
    
  }

} // namespace D3PD
