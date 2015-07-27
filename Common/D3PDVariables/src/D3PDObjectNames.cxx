// $Id: D3PDObjectNames.cxx 43055 2015-01-29 16:26:25Z gherbert $

// TODO:
// * provide default values instead of always having to define all names

// Local include(s):
#include "../include/D3PDObjectNames.h"

namespace D3PD {

  D3PDObjectNames::D3PDObjectNames( const TString& ntupleType ) {

    FillMap(ntupleType);

  }

  void D3PDObjectNames::FillMap( TString ntupleType) throw( SError ) {

    //
    // depending on ntupleType fill map
    //
    
    //bw comp.
    if (ntupleType == "D3PDMakerD3PD")          ntupleType = "UZHCMSNtuple";
    else if (ntupleType == "D3PDMakerD3PDOld")  ntupleType = "NTUP_TOP_p1270";

    if (ntupleType == "NTUP_COMMON") {
      //std::cout << "WARNING: Using NTUP_COMMON names" << std::endl;
      Fill_NTUP_COMMON();
    } else if (ntupleType == "NTUP_TOP_p1562") {
      Fill_NTUP_COMMON();
      Fill_NTUP_TOP_p1562();
		} else if (ntupleType == "NTUP_TOP_p1562_clusters") {
      Fill_NTUP_TOP_p1562_clusters();
    } else if (ntupleType == "NTUP_TOP_p1400") {
      Fill_NTUP_COMMON();
      Fill_NTUP_TOP_p1400();
    } else if (ntupleType == "NTUP_TOP_p1270") {
      std::cout << "WARNING: Using NTUP_TOP p1270 names" << std::endl;
      Fill_NTUP_COMMON();
      Fill_NTUP_TOP_OLD();
    } else if (ntupleType == "GroomedJetsD3PD") {
      Fill_NTUP_COMMON();
      FillGroomedJetsD3PD();
    } else if (ntupleType == "ChargedHiggsTRCNtuple") {
      FillChargedHiggsTRCNtuple();
    } else if (ntupleType == "UZHCMSNtuple") {
      FillUZHCMSNtuple();
    } else {
      throw SError( "Ntuple Type '"+ntupleType+"' not defined!", SError::StopExecution );
    }
    //std::cout << "ntuple type: " << ntupleType << std::endl;
    return;

  }


  void D3PDObjectNames::Fill_NTUP_COMMON() {

    // for D3PDs made with official ATLAS D3PD Maker
    // structured similarly to the SUSY D3PD

    // Prefixes
    //	m_prefixes[""] = "el_";
    //	m_prefixes["muons"]     = "mu_";
    //	m_prefixes["trig_dec"]  = "jetmet_"; // trigger decision bits
    // ...



    // Names

    m_names["N"] = "n";
    // general kinematics
    m_names["e"] = "E";
    m_names["pt"] = "pt";
    m_names["m"] = "m";
    m_names["eta"] = "eta";
    m_names["phi"] = "phi";
    m_names["px"] = "px";
    m_names["py"] = "py";
    m_names["pz"] = "pz";
    // lepton related variables
    m_names["charge"] = "charge";
    m_names["etcone20"] = "Etcone20";
    m_names["etcone30"] = "Etcone30";
    m_names["etcone40"] = "Etcone40";
    m_names["etcone20_pt_corrected"] = "Etcone20_pt_corrected";
    m_names["ptcone20"] = "ptcone20";
    m_names["ptcone30"] = "ptcone30";
    m_names["ptcone40"] = "ptcone40";
    m_names["MI10_max30_ptsum"] = "MI10_max30_ptsum"; //mini isolation
    m_names["MI10_max40_ptsum"] = "MI10_max40_ptsum"; //mini isolation
    m_names["author"] = "author";
    m_names["expectHitInBLayer"] = "expectBLayerHit";
    // electron specific variables
    m_names["isEM"] = "isEM";
    m_names["cluster_eta"] = "cl_eta";
    m_names["cluster_etap"] = "etap";
    m_names["cluster_eta_s2"] = "etas2";
    m_names["cluster_phi"] = "cl_phi";
    m_names["cluster_e"] = "cl_E";
    m_names["cluster_pt"] = "cl_pt";
    m_names["isemLoose"] = "loose";   
    m_names["isemMedium"] = "medium"; 
    m_names["isemTight"] = "tight";   
    m_names["isemTightPP"] = "tightPP";
    m_names["isemMediumPP"] = "mediumPP";
    m_names["isemLoosePP"] = "loosePP";
    //m_names["trackParticle_eta"] = "tracketa";
    //m_names["trackParticle_theta"] = "tracktheta";
    //m_names["trackParticle_phi"] = "trackphi";
    m_names["trackd0pvunbiased"]    = "trackd0pvunbiased";  
    m_names["tracksigd0pvunbiased"] = "tracksigd0pvunbiased"; 
    m_names["GSF_trk_index"]  = "GSF_trk_index"; 

    m_names["goodOQ"] = "goodOQ";
    m_names["OQ"] = "OQ";
    m_names["EF_index"] = "EF_index";
    m_names["EF_dr"] = "EF_dr";
    m_names["type"] = "type";
    m_names["origin"] = "origin";
    m_names["typebkg"] = "typebkg";
    m_names["originbkg"] = "originbkg";
    m_names["truth_matched"] = "truth_matched";
    m_names["truth_type"] = "truth_type";
    m_names["truth_mothertype"] = "truth_mothertype";
    m_names["trackz0pvunbiased"] = "trackz0pvunbiased";
    m_names["pt1conv"] = "pt1conv";
    m_names["pt2conv"] = "pt2conv";
    m_names["nSiHits"] = "nSiHits";
    m_names["nSCTOutliers"] = "nSCTOutliers";
    m_names["nPixelOutliers"] = "nPixelOutliers";
    m_names["nBLHits"] = "nBLHits";
    m_names["nBLayerOutliers"] = "nBLayerOutliers";
    m_names["Ethad"] = "Ethad";
    m_names["Ethad1"] = "Ethad1";
    m_names["reta"] = "reta";
    m_names["weta2"] = "weta2";
    m_names["f1"] = "f1";
    m_names["f3"] = "f3";
    m_names["wstot"] = "wstot";
    m_names["emaxs1"] = "emaxs1";
    m_names["Emax2"] = "Emax2";
    m_names["deltaeta1"] = "deltaeta1";
    m_names["deltaphi2"] = "deltaphi2";
    m_names["trackd0_physics"] = "trackd0_physics";
    m_names["TRTHighTOutliersRatio"] = "TRTHighTOutliersRatio";
    
    // muon specific variables
    m_names["isMedium"] = "medium";
    m_names["isTight"] = "tight";
    m_names["Etcone20"] = "etcone20";
    m_names["Etcone30"] = "etcone30";
    m_names["nBLayerHits"] = "nBLHits";
    m_names["nPixelHits"] = "nPixHits";
    m_names["nSCTHits"] = "nSCTHits";
    m_names["nTRTHits"] = "nTRTHits";
    m_names["nTRTOutliers"] = "nTRTOutliers";
    m_names["nPixelDeadSensors"] = "nPixelDeadSensors";
    m_names["nSCTDeadSensors"] = "nSCTDeadSensors";
    m_names["nPixelHoles"] = "nPixHoles";
    m_names["nSCTHoles"] = "nSCTHoles";
    m_names["track_eta"] = "tracketa";
    m_names["track_theta"] = "tracktheta";
    m_names["track_phi"] = "trackphi";
    m_names["track_d0"] = "trackd0";
    m_names["track_qoverp"] = "trackqoverp";
    m_names["me_theta"] = "me_theta";
    m_names["me_phi"] = "me_phi";
    m_names["me_qoverp"] = "me_qoverp";
    m_names["id_theta"] = "id_theta";
    m_names["id_phi"] = "id_phi";
    m_names["id_qoverp"] = "id_qoverp";
    m_names["ms_theta"] = "ms_theta";
    m_names["ms_phi"] = "ms_phi";
    m_names["ms_qoverp"] = "ms_qoverp";
    m_names["ie_theta"] = "ie_theta";
    m_names["ie_phi"] = "ie_phi";
    m_names["ie_qoverp"] = "ie_qoverp";
    m_names["isCombinedMuon"] = "isCombinedMuon";
    m_names["isSegmentTaggedMuon"] = "isSegmentTaggedMuon";
    m_names["isStandAloneMuon"] = "isStandAloneMuon";
    m_names["id_z0_exPV"] = "id_z0_exPV";
    // jet specific variables
    m_names["nTracks"] = "nTrk";
    m_names["assoctrk_index"] = "flavor_assoctrk_index";
    m_names["EMFraction"] = "emfrac";
    m_names["weight"] = "flavor_weight_Comb";
    m_names["IP2D"] = "flavor_weight_IP2D";
    m_names["IP3D"] = "flavor_weight_IP3D";
    m_names["SV0"] = "flavor_weight_SV0";
    m_names["SV0_m"] = "flavor_component_sv0p_mass";
    m_names["SV0_ntrkv"]		=	"flavor_component_sv0p_ntrkv"		;
    m_names["SV0_x"]				=	"flavor_component_sv0p_x"				;
    m_names["SV0_y"]				=	"flavor_component_sv0p_y"				;
    m_names["SV0_efrc"]			=	"flavor_component_sv0p_efrc"		;
    m_names["SV0_n2t"]			=	"flavor_component_sv0p_n2t"			;
    m_names["JFIT_mass"]			=	"flavor_component_jfit_mass"		;
    m_names["JFIT_efrc"]			=	"flavor_component_jfit_efrc"		;
    m_names["JFIT_sig3d"]		=	"flavor_component_jfit_sig3d"		;
    m_names["JFIT_deltaEta"]	=	"flavor_component_jfit_deltaEta";
    m_names["JFIT_deltaPhi"]	=	"flavor_component_jfit_deltaPhi";
    m_names["JFIT_ntrkAtVx"]	=	"flavor_component_jfit_ntrkAtVx";
    m_names["JFIT_nvtx1t"]		=	"flavor_component_jfit_nvtx1t"	;
    
    m_names["flavor_component_jfitcomb_pu"] = "flavor_component_jfitcomb_pu";
    m_names["flavor_component_jfitcomb_pc"] = "flavor_component_jfitcomb_pc";
    m_names["flavor_component_jfitcomb_pb"] = "flavor_component_jfitcomb_pb";
    m_names["flavor_component_svp_ntrkv"]	= "flavor_component_svp_ntrkv";
    m_names["flavor_component_svp_n2t"]	= "flavor_component_svp_n2t";
    m_names["flavor_component_svp_mass"]	= "flavor_component_svp_mass";
    m_names["flavor_component_svp_efrc"]	= "flavor_component_svp_efrc";
    m_names["flavor_component_svp_x"]	= "flavor_component_svp_x";
    m_names["flavor_component_svp_y"]	= "flavor_component_svp_y";
    m_names["flavor_component_svp_z"]	= "flavor_component_svp_z";
    m_names["flavor_component_svp_err_x"]	= "flavor_component_svp_err_x";
    m_names["flavor_component_svp_err_y"]	= "flavor_component_svp_err_y";
    m_names["flavor_component_svp_err_z"]	= "flavor_component_svp_err_z";
    m_names["flavor_component_svp_cov_xy"]	= "flavor_component_svp_cov_xy";
    m_names["flavor_component_svp_cov_yz"]	= "flavor_component_svp_cov_yz";
    m_names["flavor_component_svp_cov_xz"]	= "flavor_component_svp_cov_xz";
    m_names["flavor_component_jfit_pu"]	= "flavor_component_jfit_pu";
    m_names["flavor_component_jfit_pb"]	= "flavor_component_jfit_pb";
    m_names["flavor_component_jfit_pc"]	= "flavor_component_jfit_pc";
    m_names["flavor_component_jfit_nvtx"]	= "flavor_component_jfit_nvtx";
    m_names["flavor_component_sv0p_trk_index"]	= "flavor_component_sv0p_trk_index";
    m_names["flavor_component_jfitvx_theta"]	= "flavor_component_jfitvx_theta";
    m_names["flavor_component_jfitvx_phi"]	= "flavor_component_jfitvx_phi";
    m_names["flavor_component_jfitvx_errtheta"]	= "flavor_component_jfitvx_errtheta";
    m_names["flavor_component_jfitvx_errphi"]	= "flavor_component_jfitvx_errphi";
    m_names["flavor_component_jfitvx_chi2"]	= "flavor_component_jfitvx_chi2";
    m_names["flavor_component_jfitvx_ndof"]	= "flavor_component_jfitvx_ndof";
    m_names["flavor_component_jftwotrackvertex_n"]	= "flavor_component_jftwotrackvertex_n";
    m_names["flavor_component_ipplus_trk_index"]	= "flavor_component_ipplus_trk_index";
    m_names["flavor_component_svp_trk_index"]	= "flavor_component_svp_trk_index";
    
    m_names["SV1"] = "flavor_weight_SV1";
    m_names["MV1"] = "flavor_weight_MV1";
    m_names["JetProb"] = "flavor_weight_JetProb";
    m_names["JetFitterCombNN"] = "flavor_weight_JetFitterCOMBNN";
    m_names["EMScale_E"] = "emscale_E";
    m_names["EMScale_m"] = "emscale_m";
    m_names["EMScale_pt"] = "emscale_pt";
    m_names["EMScale_eta"] = "emscale_eta";
    m_names["EMScale_phi"] = "emscale_phi";
    m_names["isBadLoose"] = "isBadLoose";
    m_names["isBadLooseMinus"] = "isBadLooseMinus";
    m_names["AverageLArQF"] = "AverageLArQF";
    m_names["quality"] = "LArQuality";
    m_names["n90"] = "n90";
    m_names["hecF"] = "hecf";
    m_names["hecQ"] = "HECQuality";
    m_names["NegativeE"] = "NegativeE";
    m_names["timing"] = "Timing";
    m_names["fracSamplingMax"] = "fracSamplingMax";
    m_names["Truth_Label"] = "flavor_truth_label";
    m_names["EMJES_EtaCorr"] = "EMJES_EtaCorr";
    m_names["bch_corr_jet"] = "BCH_CORR_JET";
    m_names["bch_corr_cell"] = "BCH_CORR_CELL";
    m_names["EMJES"] = "EMJES";
    // missing et variables
    m_names["ex"] = "_etx";
    m_names["ey"] = "_ety";
    m_names["et"] = "_et";
    m_names["sumet"] = "_sumet";
    //m_names["ex"] = "etx";
    //m_names["ey"] = "ety";
    //m_names["et"] = "et";
    //m_names["sumet"] = "sumet";
    m_names["RefFinal"] = "_RefFinal_";
    m_names["RefEle"] = "_RefEle_";
    m_names["RefGamma"] = "_RefGamma_";
    m_names["RefJet"] = "_RefJet_";
    m_names["SoftJets"] = "_SoftJets_";
    m_names["CellOut"] = "_CellOut_";
    m_names["Muon_Total_Muid"] = "_Muon_Total_Muid_";
    // met electrons/photons
    m_names["MET_tightpp_wpx"          ] = "MET_tightpp_wpx";
    m_names["MET_tightpp_wpy"          ] = "MET_tightpp_wpy";
    m_names["MET_tightpp_wet"          ] = "MET_tightpp_wet";
    m_names["MET_tightpp_statusWord"   ] = "MET_tightpp_statusWord";
    m_names["MET_mediumpp_wpx"         ] = "MET_mediumpp_wpx";
    m_names["MET_mediumpp_wpy"         ] = "MET_mediumpp_wpy";
    m_names["MET_mediumpp_wet"         ] = "MET_mediumpp_wet";
    m_names["MET_mediumpp_statusWord"  ] = "MET_mediumpp_statusWord";
    m_names["MET_loosepp_wpx"          ] = "MET_loosepp_wpx";
    m_names["MET_loosepp_wpy"          ] = "MET_loosepp_wpy";
    m_names["MET_loosepp_wet"          ] = "MET_loosepp_wet";
    m_names["MET_loosepp_statusWord"   ] = "MET_loosepp_statusWord";
    // met jets
    m_names["tightpp_wpx"          ] = "MET_tightpp_wpx";
    m_names["tightpp_wpy"          ] = "MET_tightpp_wpy";
    m_names["tightpp_wet"          ] = "MET_tightpp_wet";
    m_names["tightpp_statusWord"   ] = "MET_tightpp_statusWord";
    m_names["mediumpp_wpx"         ] = "MET_mediumpp_wpx";
    m_names["mediumpp_wpy"         ] = "MET_mediumpp_wpy";
    m_names["mediumpp_wet"         ] = "MET_mediumpp_wet";
    m_names["mediumpp_statusWord"  ] = "MET_mediumpp_statusWord";
    m_names["loosepp_wpx"          ] = "MET_loosepp_wpx";
    m_names["loosepp_wpy"          ] = "MET_loosepp_wpy";
    m_names["loosepp_wet"          ] = "MET_loosepp_wet";
    m_names["loosepp_statusWord"   ] = "MET_loosepp_statusWord";
    // met muons
    m_names["muid_MET_tightpp_n"           ] = "MET_tightpp_n";
    m_names["muid_MET_tightpp_wpx"         ] = "MET_tightpp_wpx";
    m_names["muid_MET_tightpp_wpy"         ] = "MET_tightpp_wpy";
    m_names["muid_MET_tightpp_wet"         ] = "MET_tightpp_wet";
    m_names["muid_MET_tightpp_statusWord"  ] = "MET_tightpp_statusWord";
    m_names["muid_MET_mediumpp_wpx"        ] = "MET_mediumpp_wpx";
    m_names["muid_MET_mediumpp_wpy"        ] = "MET_mediumpp_wpy";
    m_names["muid_MET_mediumpp_wet"        ] = "MET_mediumpp_wet";
    m_names["muid_MET_mediumpp_statusWord" ] = "MET_mediumpp_statusWord";
    m_names["muid_MET_loosepp_wpx"         ] = "MET_loosepp_wpx";
    m_names["muid_MET_loosepp_wpy"         ] = "MET_loosepp_wpy";
    m_names["muid_MET_loosepp_wet"         ] = "MET_loosepp_wet";
    m_names["muid_MET_loosepp_statusWord"  ] = "MET_loosepp_statusWord";
    // event info variables
    m_names["RunNumber"           ] = "RunNumber";
    m_names["EventNumber"         ] = "EventNumber";
    m_names["LumiBlock"           ] = "lbn";
    m_names["GenEventWeight"      ] = "mcevt_weight";
    m_names["HFORresult"          ] = "top_hfor_type";
    //m_names["isEleMuOverlap"      ] = "top_isEleMuOverlap";
    m_names["averageIntPerXing"   ] = "averageIntPerXing";
    m_names["actualIntPerXing"    ] = "actualIntPerXing";
    m_names["mc_channel_number"   ] = "mc_channel_number";
    m_names["mc_event_weight"     ] = "mc_event_weight";
    m_names["larError"            ] = "larError";
    m_names["tileError"           ] = "tileError";
    m_names["coreFlags"           ] = "coreFlags";
    m_names["Eventshape_rhoKt4LC" ] = "Eventshape_rhoKt4LC";
    m_names["mcVx_n"              ] = "mcVx_n";
    m_names["mcVx_x"              ] = "mcVx_x";
    m_names["mcVx_y"              ] = "mcVx_y";
    m_names["mcVx_z"              ] = "mcVx_z";

    m_names["musp_eta" ] = "musp_eta";
    m_names["musp_phi" ] = "musp_phi";
    m_names["musp_innerSegments" ] = "musp_innerSegments";
    m_names["musp_middleSegments" ] = "musp_middleSegments";
    m_names["musp_outerSegments" ] = "musp_outerSegments";
    
    m_names["pdf1"] = "mcevt_pdf1";
    m_names["pdf2"] = "mcevt_pdf2";
    m_names["pdf_id1"] = "mcevt_pdf_id1";
    m_names["pdf_id2"] = "mcevt_pdf_id2";
    m_names["pdf_scale"] = "mcevt_pdf_scale";
    m_names["pdf_x1"] = "mcevt_pdf_x1";
    m_names["pdf_x2"] = "mcevt_pdf_x2";
    
    // vertex
    m_names["nTracksVertex"       ] = "trk_n";
    m_names["cov_xy"              ] = "cov_xy";
    m_names["cov_xz"              ] = "cov_xz";
    m_names["cov_yz"              ] = "cov_yz";
    m_names["err_x"               ] = "err_x";
    m_names["err_y"               ] = "err_y";
    m_names["err_z"               ] = "err_z";
    m_names["x"                   ] = "x";
    m_names["y"                   ] = "y";
    m_names["z"                   ] = "z";
    m_names["chi2"                ] = "chi2";
    m_names["ndof"                ] = "ndof";
    
    
    //tracks
    m_names["qoverp"              ] = "qoverp_wrtPV"      ;
    m_names["theta0"              ] = "theta_wrtPV"       ;
    m_names["phi0"                ] = "phi_wrtPV"         ;
    m_names["d0"                  ] = "d0_wrtPV"          ;
    m_names["cov_d0"							] =	"cov_d0_wrtPV"			;
		m_names["z0"                  ] = "z0_wrtPV"          ;
    m_names["nBLayerHits"         ] = "nBLHits"           ;
    m_names["expectBLayerHit"     ] = "expectBLayerHit"   ;
    m_names["nPixHits"            ] = "nPixHits"          ;
    m_names["nPixelDeadSensors"   ] = "nPixelDeadSensors" ;
    m_names["nSCTHits"            ] = "nSCTHits"          ;
    m_names["nSCTDeadSensors"     ] = "nSCTDeadSensors"   ;
    m_names["mc_barcode"          ] = "mc_barcode"				;
		m_names["cov_z0_wrtPV"				]	= "cov_z0_wrtPV"			;
		m_names["mc_probability"			]	= "mc_probability"		;
 
    m_names["type"                ] = "type";
    
    //Boosted stuff
    m_names["SPLIT12"       ] = "SPLIT12"               ;                                                                        
    m_names["SPLIT23"       ] = "SPLIT23"               ;                                                                        
    m_names["SPLIT34"       ] = "SPLIT34"               ;                                                                        
    m_names["ZCUT12"        ] = "ZCUT12"                ;                         
    m_names["ZCUT23"        ] = "ZCUT23"                ;                         
    m_names["ZCUT34"        ] = "ZCUT34"                ;                         
    
                            
    m_names["Muon_n"        ] = "Muon_n"                ;
    m_names["Electron_n"    ] = "Electron_n"            ;                                                                        
    m_names["Electron_X"    ] = "Electron_X";                         
    m_names["Electron_Y"    ] = "Electron_Y";                         
    m_names["Electron_Z"    ] = "Electron_Z";                         
    m_names["Electron_MINIISO"] = "Electron_MINIISO";                         
    m_names["Electron_MINIISO10"] = "Electron_MINIISO10";
    m_names["Electron_DR"] = "Electron_DR";                         
    m_names["Electron_X_Prime"] = "Electron_X_Prime";                         
    m_names["Electron_Y_Prime"] = "Electron_Y_Prime";                         
    m_names["Electron_Z_Prime"] = "Electron_Z_Prime";                        
    m_names["Electron_MINIISO_Prime"] = "Electron_MINIISO_Prime";                         
    m_names["Electron_DR_Prime"] = "Electron_DR_Prime";                         
    m_names["Electron_index"] = "Electron_index";                         
    m_names["Electron_contained"] = "Electron_contained";
    m_names["Muon_X"] = "Muon_X";                         
    m_names["Muon_Y"] = "Muon_Y";                         
    m_names["Muon_Z"] = "Muon_Z";                         
    m_names["Muon_MINIISO"] = "Muon_MINIISO";                         
    m_names["Muon_MINIISO10"] = "Muon_MINIISO10";
    m_names["Muon_DR"] = "Muon_DR";                         
    m_names["Muon_index"] = "Muon_index";                         
    m_names["Muon_contained"] = "Muon_contained";
    m_names["Muon_X_Prime"] = "Muon_X_Prime";
    m_names["Muon_Y_Prime"] = "Muon_Y_Prime";
    m_names["Muon_Z_Prime"] = "Muon_Z_Prime";
    m_names["Muon_DR_Prime"] = "Muon_DR_Prime";
    m_names["KtSubjets_index"] = "KtSubjets_index";                         
    m_names["Trimmed_n"] = "Trimmed_n";                         
    m_names["Trimmed_index"] = "Trimmed_index";                         
    m_names["constit_n"] = "constit_n";                         
    m_names["constit_weight"] = "constit_weight";                         
    m_names["constit_index"] = "constit_index";                         
    
    //nomal jets
		m_names["pt"]	=	"pt";
		m_names["eta"] =	"eta";
    m_names["LCJES"] = "LCJES";                         
    m_names["LCJES_EtaCorr"] = "LCJES_EtaCorr";                         
    m_names["LCJES_MassCorr"] = "LCJES_MassCorr";                         
    m_names["KtSubjets_n"] = "KtSubjets_n";                         
    m_names["EtaOrigin"] = "EtaOrigin";                         
    m_names["PhiOrigin"] = "PhiOrigin";                         
    m_names["MOrigin"] = "MOrigin";                         
    m_names["EtaOriginEM"] = "EtaOriginEM";                         
    m_names["PhiOriginEM"] = "PhiOriginEM";                         
    m_names["MOriginEM"] = "MOriginEM";                         
    m_names["emfrac"] = "emfrac";                         
    m_names["emscale_E"] = "emscale_E";                         
    m_names["emscale_pt"] = "emscale_pt";                         
    m_names["emscale_m"] = "emscale_m";                         
    m_names["emscale_eta"] = "emscale_eta";                         
    m_names["emscale_phi"] = "emscale_phi";                         
    m_names["jvtxf"] = "jvtxf";                         
    m_names["jvtx_x"] = "jvtx_x";                         
    m_names["jvtx_y"] = "jvtx_y";                         
    m_names["jvtx_z"] = "jvtx_z";                         
    m_names["nTrk"] = "nTrk";                         
    m_names["sumPtTrk"] = "sumPtTrk_pv0_500MeV";
    m_names["OriginIndex"] = "OriginIndex";                         
    m_names["constscale_E"] = "constscale_E";                        
    m_names["constscale_pt"] = "constscale_pt";                         
    m_names["constscale_m"] = "constscale_m";                         
    m_names["constscale_eta"] = "constscale_eta";                         
    m_names["constscale_phi"] = "constscale_phi";                         
    //for akt4
    m_names["ActiveArea"        ] = "ActiveArea"           ; 
    m_names["ActiveAreaPx"      ] = "ActiveAreaPx"         ;
    m_names["ActiveAreaPy"      ] = "ActiveAreaPy"         ;
    m_names["ActiveAreaPz"      ] = "ActiveAreaPz"         ;
    m_names["ActiveAreaE"       ] = "ActiveAreaE"          ;
    //for akt10 
    m_names["ActiveArea_e"      ] = "ActiveArea_e"         ;
    m_names["ActiveArea_px"     ] = "ActiveArea_px"        ;
    m_names["ActiveArea_py"     ] = "ActiveArea_py"        ;
    m_names["ActiveArea_pz"     ] = "ActiveArea_pz"        ;

    m_names["Dip12"       ] =  "Dip12"       ; 
    m_names["Dip13"       ] =  "Dip13"       ;
    m_names["Dip23"       ] =  "Dip23"       ;
    m_names["DipExcl12"   ] =  "DipExcl12"   ;
    m_names["PlanarFlow"  ] =  "PlanarFlow"  ;
    m_names["Angularity"  ] =  "Angularity"  ;
    m_names["VoronoiArea" ] =  "VoronoiArea" ;
    
    m_names["L1_dr"] = "L1_dr";                         
    m_names["L1_matched"] = "L1_matched";                         
    m_names["L2_dr"] = "L2_dr";                         
    m_names["L2_matched"] = "L2_matched";                         
    m_names["EF_dr"] = "EF_dr";                         
    m_names["EF_matched"] = "EF_matched";                                                        
    m_names["TrackJets_n"] = "TrackJets_n";                            
    m_names["TruthJets_n"] = "TruthJets_n";                            
    m_names["TrackJets_index"] = "TrackJets_index";                            
    m_names["TruthJets_index"] = "TruthJets_index";                            
    
    m_names["TrackAssoc_index"    ] = "TrackAssoc_index"   ;

    m_names["WIDTH"   ] = "WIDTH";                            
    m_names["QW"      ] = "QW";                            
    m_names["PullMag" ] = "PullMag";                            
    m_names["PullPhi" ] = "PullPhi";                            
    m_names["Pull_C00"] = "Pull_C00";                            
    m_names["Pull_C01"] = "Pull_C01";                                                 
    m_names["Pull_C10"] = "Pull_C10";                                                 
    m_names["Pull_C11"] = "Pull_C11";                                                 
    m_names["Tau1"    ] = "Tau1";                                                 
    m_names["Tau2"    ] = "Tau2";                                                 
    m_names["Tau3"    ] = "Tau3";    
    // truth jets
    m_names["flavour"] = "flavor_truth_label";
    // truth particles
    m_names["status"] = "status";
    m_names["barcode"] = "barcode";
    m_names["pdgId"] = "pdgId";
    m_names["parents"] = "parents";
    m_names["children"] = "children";
    m_names["parent_index"] = "parent_index";
    m_names["child_index"] = "child_index";
    
    // cluster
    m_names["E_em"]       = "E_em";
    m_names["E_had"]      = "E_had";
    m_names["centermag"]  = "centermag";

    // trigger stuff
    // EF Electron trigger
    m_names[ "EF_e24vhi_medium1" ] =  "EF_e24vhi_medium1" ;
    m_names[ "EF_e24vh_medium1" ] =  "EF_e24vh_medium1" ;
    m_names[ "EF_e60_medium1"    ] =  "EF_e60_medium1"    ;
    // EF Muon trigger
    m_names[ "EF_mu24i_tight"    ] =  "EF_mu24i_tight"    ;
    m_names[ "EF_mu24_tight"    ] =  "EF_mu24_tight"    ;
    m_names[ "EF_mu36_tight"     ] =  "EF_mu36_tight"     ;
    
    m_names["track_n"] = "track_n";
    m_names["track_CB_pt"] = "track_CB_pt";
    m_names["track_CB_eta"] = "track_CB_eta";
    m_names["track_CB_phi"] = "track_CB_phi";
    //OLD 
    m_names["EF_mu18"] = "EF_mu18";
    m_names["EF_mu18_medium"] = "EF_mu18_medium";
    m_names["EF_e20_medium"] = "EF_e20_medium";
    m_names["EF_e22_medium"] = "EF_e22_medium";
    m_names["EF_e22_medium1"] = "EF_e22_medium1";
    m_names["EF_e45_medium1"] = "EF_e45_medium1";
    m_names["EF_e22vh_medium1"] = "EF_e22vh_medium1";
    m_names["EF_j15_a4tchad"] = "EF_j15_a4tchad"; 
    m_names["EF_j25_a4tchad"] = "EF_j25_a4tchad"; 
    m_names["EF_fj15_a4tchad"] = "EF_fj15_a4tchad"; 
    m_names["EF_fj25_a4tchad"] = "EF_fj25_a4tchad"; 
    m_names["EF_b15_NoCut_j15_a4tchad"] = "EF_b15_NoCut_j15_a4tchad"; 
    m_names["EF_b15_looseEF_j15_a4tchad"] = "EF_b15_looseEF_j15_a4tchad"; 
    m_names["EF_b25_looseEF_j25_a4tchad"] = "EF_b25_looseEF_j25_a4tchad"; 

  }

  void D3PDObjectNames::FillGroomedJetsD3PD() {
    //
    // adds / overwrites TopD3PD branches
    //
    
    //tracks
    /*
    same
    m_names["nBLHits"           ] = "nBLHits"               ;
    m_names["nPixHits"          ] = "nPixHits"              ;
    m_names["nSCTHits"          ] = "nSCTHits"              ;
    m_names["nTRTHits"          ] = "nTRTHits"              ;
    m_names["nPixHoles"         ] = "nPixHoles"             ;
    m_names["nSCTHoles"         ] = "nSCTHoles"             ;
    m_names["nPixelDeadSensors" ] = "nPixelDeadSensors"     ;
    m_names["nSCTDeadSensors"   ] = "nSCTDeadSensors"       ;
    */
    m_names["nTRTHoles"         ] = "nTRTHoles"             ;
    m_names["nBLSharedHits"     ] = "nBLSharedHits"         ;
    m_names["nPixSharedHits"    ] = "nPixSharedHits"        ;
    m_names["nSCTSharedHits"    ] = "nSCTSharedHits"        ;
    m_names["nBLayerSplitHits"  ] = "nBLayerSplitHits"      ;
    m_names["nPixSplitHits"     ] = "nPixSplitHits"         ;

    //fat jets
    m_names["WIDTH"             ] = "WIDTH"                 ; 
    m_names["Dip12"             ] = "Dip12"                 ;
    m_names["Dip13"             ] = "Dip13"                 ;
    m_names["Dip23"             ] = "Dip23"                 ;
    m_names["DipExcl12"         ] = "DipExcl12"             ;
    m_names["PlanarFlow"        ] = "PlanarFlow"            ;
    m_names["Angularity"        ] = "Angularity"            ;
    m_names["ActiveArea"        ] = "ActiveArea"            ; 
    m_names["ActiveAreaPx"      ] = "ActiveArea_px"         ;
    m_names["ActiveAreaPy"      ] = "ActiveArea_py"         ;
    m_names["ActiveAreaPz"      ] = "ActiveArea_pz"         ;
    m_names["ActiveAreaE"       ] = "ActiveArea_e"          ;
    m_names["VoronoiArea"       ] = "VoronoiArea"           ;
    m_names["mcVx_n"            ] = "TruthVertexn"          ;
    m_names["mcVx_x"            ] = "TruthVertexx"          ;
    m_names["mcVx_y"            ] = "TruthVertexy"          ;
    m_names["mcVx_z"            ] = "TruthVertexz"          ;

  }

	void D3PDObjectNames::Fill_NTUP_TOP_p1562() {

    m_names["err_x"               ] = "cov_x";
    m_names["err_y"               ] = "cov_y";
    m_names["err_z"               ] = "cov_z";

		m_names["assoctrk_index"] = "TrackAssoc_index";

  }
	
	void D3PDObjectNames::Fill_NTUP_TOP_p1562_clusters() {

		m_names["centermag"] = "centermag";
		m_names["N"] = "lc_n";
		m_names["pt"] = "lc_pt";
		m_names["eta"] = "lc_eta";
		m_names["phi"] = "lc_phi";

  }


  void D3PDObjectNames::Fill_NTUP_TOP_p1400() {
    m_names["ActiveAreaPx"      ] = "ActiveAreaPx"         ;
    m_names["ActiveAreaPy"      ] = "ActiveAreaPy"         ;
    m_names["ActiveAreaPz"      ] = "ActiveAreaPz"         ;
    m_names["ActiveAreaE"       ] = "ActiveAreaE"          ;
    // met electrons/photons
    m_names["MET_tightpp_wpx"          ] = "MET_AntiKt4LCTopoJets_tightpp_wpx";
    m_names["MET_tightpp_wpy"          ] = "MET_AntiKt4LCTopoJets_tightpp_wpy";
    m_names["MET_tightpp_wet"          ] = "MET_AntiKt4LCTopoJets_tightpp_wet";
    m_names["MET_tightpp_statusWord"   ] = "MET_AntiKt4LCTopoJets_tightpp_statusWord";
    m_names["MET_mediumpp_wpx"         ] = "MET_AntiKt4LCTopoJets_mediumpp_wpx";
    m_names["MET_mediumpp_wpy"         ] = "MET_AntiKt4LCTopoJets_mediumpp_wpy";
    m_names["MET_mediumpp_wet"         ] = "MET_AntiKt4LCTopoJets_mediumpp_wet";
    m_names["MET_mediumpp_statusWord"  ] = "MET_AntiKt4LCTopoJets_mediumpp_statusWord";
    m_names["MET_loosepp_wpx"          ] = "MET_AntiKt4LCTopoJets_loosepp_wpx";
    m_names["MET_loosepp_wpy"          ] = "MET_AntiKt4LCTopoJets_loosepp_wpy";
    m_names["MET_loosepp_wet"          ] = "MET_AntiKt4LCTopoJets_loosepp_wet";
    m_names["MET_loosepp_statusWord"   ] = "MET_AntiKt4LCTopoJets_loosepp_statusWord";
    // met jets
    m_names["tightpp_wpx"          ] = "AntiKt4LCTopoJets_tightpp_wpx";
    m_names["tightpp_wpy"          ] = "AntiKt4LCTopoJets_tightpp_wpy";
    m_names["tightpp_wet"          ] = "AntiKt4LCTopoJets_tightpp_wet";
    m_names["tightpp_statusWord"   ] = "AntiKt4LCTopoJets_tightpp_statusWord";
    m_names["mediumpp_wpx"         ] = "AntiKt4LCTopoJets_mediumpp_wpx";
    m_names["mediumpp_wpy"         ] = "AntiKt4LCTopoJets_mediumpp_wpy";
    m_names["mediumpp_wet"         ] = "AntiKt4LCTopoJets_mediumpp_wet";
    m_names["mediumpp_statusWord"  ] = "AntiKt4LCTopoJets_mediumpp_statusWord";
    m_names["loosepp_wpx"          ] = "AntiKt4LCTopoJets_loosepp_wpx";
    m_names["loosepp_wpy"          ] = "AntiKt4LCTopoJets_loosepp_wpy";
    m_names["loosepp_wet"          ] = "AntiKt4LCTopoJets_loosepp_wet";
    m_names["loosepp_statusWord"   ] = "AntiKt4LCTopoJets_loosepp_statusWord";
    // met muons
    m_names["muid_MET_tightpp_n"           ] = "muid_MET_AntiKt4LCTopoJets_tightpp_n";
    m_names["muid_MET_tightpp_wpx"         ] = "muid_MET_AntiKt4LCTopoJets_tightpp_wpx";
    m_names["muid_MET_tightpp_wpy"         ] = "muid_MET_AntiKt4LCTopoJets_tightpp_wpy";
    m_names["muid_MET_tightpp_wet"         ] = "muid_MET_AntiKt4LCTopoJets_tightpp_wet";
    m_names["muid_MET_tightpp_statusWord"  ] = "muid_MET_AntiKt4LCTopoJets_tightpp_statusWord";
    m_names["muid_MET_mediumpp_wpx"        ] = "muid_MET_AntiKt4LCTopoJets_mediumpp_wpx";
    m_names["muid_MET_mediumpp_wpy"        ] = "muid_MET_AntiKt4LCTopoJets_mediumpp_wpy";
    m_names["muid_MET_mediumpp_wet"        ] = "muid_MET_AntiKt4LCTopoJets_mediumpp_wet";
    m_names["muid_MET_mediumpp_statusWord" ] = "muid_MET_AntiKt4LCTopoJets_mediumpp_statusWord";
    m_names["muid_MET_loosepp_wpx"         ] = "muid_MET_AntiKt4LCTopoJets_loosepp_wpx";
    m_names["muid_MET_loosepp_wpy"         ] = "muid_MET_AntiKt4LCTopoJets_loosepp_wpy";
    m_names["muid_MET_loosepp_wet"         ] = "muid_MET_AntiKt4LCTopoJets_loosepp_wet";
    m_names["muid_MET_loosepp_statusWord"  ] = "muid_MET_AntiKt4LCTopoJets_loosepp_statusWord";

    // jets
    m_names["sumPtTrk"] = "sumPtTrk";

   }

  void D3PDObjectNames::Fill_NTUP_TOP_OLD() {
    // overwrites the met branch names to fit the names pre production tag p1196

    // MET electrons/positron
    m_names["MET_em_tightpp_wpx"] = "MET_em_tightpp_AntiKt4LCTopoJets_wpx"; 
    m_names["MET_em_tightpp_wpy"] = "MET_em_tightpp_AntiKt4LCTopoJets_wpy"; 
    m_names["MET_em_tightpp_wet"] = "MET_em_tightpp_AntiKt4LCTopoJets_wet"; 
    m_names["MET_em_tightpp_statusWord"] = "MET_em_tightpp_AntiKt4LCTopoJets_statusWord"; 
    m_names["MET_em_mediumpp_wpx"] = "MET_em_mediumpp_AntiKt4LCTopoJets_wpx"; 
    m_names["MET_em_mediumpp_wpy"] = "MET_em_mediumpp_AntiKt4LCTopoJets_wpy"; 
    m_names["MET_em_mediumpp_wet"] = "MET_em_mediumpp_AntiKt4LCTopoJets_wet"; 
    m_names["MET_em_mediumpp_statusWord"] = "MET_em_mediumpp_AntiKt4LCTopoJets_statusWord"; 
    m_names["MET_em_loosepp_wpx"] = "MET_em_loosepp_AntiKt4LCTopoJets_wpx"; 
    m_names["MET_em_loosepp_wpy"] = "MET_em_loosepp_AntiKt4LCTopoJets_wpy"; 
    m_names["MET_em_loosepp_wet"] = "MET_em_loosepp_AntiKt4LCTopoJets_wet"; 
    m_names["MET_em_loosepp_statusWord"] = "MET_em_loosepp_AntiKt4LCTopoJets_statusWord"; 
    
    // MET jets
    m_names["em_tightpp_wpx"] = "em_tightpp_AntiKt4LCTopoJets_wpx"; 
    m_names["em_tightpp_wpy"] = "em_tightpp_AntiKt4LCTopoJets_wpy"; 
    m_names["em_tightpp_wet"] = "em_tightpp_AntiKt4LCTopoJets_wet"; 
    m_names["em_tightpp_statusWord"] = "em_tightpp_AntiKt4LCTopoJets_statusWord"; 
    m_names["em_mediumpp_wpx"] = "em_mediumpp_AntiKt4LCTopoJets_wpx"; 
    m_names["em_mediumpp_wpy"] = "em_mediumpp_AntiKt4LCTopoJets_wpy"; 
    m_names["em_mediumpp_wet"] = "em_mediumpp_AntiKt4LCTopoJets_wet"; 
    m_names["em_mediumpp_statusWord"] = "em_mediumpp_AntiKt4LCTopoJets_statusWord"; 
    m_names["em_loosepp_wpx"] = "em_loosepp_AntiKt4LCTopoJets_wpx"; 
    m_names["em_loosepp_wpy"] = "em_loosepp_AntiKt4LCTopoJets_wpy"; 
    m_names["em_loosepp_wet"] = "em_loosepp_AntiKt4LCTopoJets_wet"; 
    m_names["em_loosepp_statusWord"] = "em_loosepp_AntiKt4LCTopoJets_statusWord"; 

    // MET muons
    m_names["muid_MET_em_tightpp_wpx"] = "muid_MET_em_tightpp_AntiKt4LCTopoJets_wpx"; 
    m_names["muid_MET_em_tightpp_wpy"] = "muid_MET_em_tightpp_AntiKt4LCTopoJets_wpy"; 
    m_names["muid_MET_em_tightpp_wet"] = "muid_MET_em_tightpp_AntiKt4LCTopoJets_wet"; 
    m_names["muid_MET_em_tightpp_statusWord"] = "muid_MET_em_tightpp_AntiKt4LCTopoJets_statusWord"; 
    m_names["muid_MET_em_mediumpp_wpx"] = "muid_MET_em_mediumpp_AntiKt4LCTopoJets_wpx"; 
    m_names["muid_MET_em_mediumpp_wpy"] = "muid_MET_em_mediumpp_AntiKt4LCTopoJets_wpy"; 
    m_names["muid_MET_em_mediumpp_wet"] = "muid_MET_em_mediumpp_AntiKt4LCTopoJets_wet"; 
    m_names["muid_MET_em_mediumpp_statusWord"] = "muid_MET_em_mediumpp_AntiKt4LCTopoJets_statusWord"; 
    m_names["muid_MET_em_loosepp_wpx"] = "muid_MET_em_loosepp_AntiKt4LCTopoJets_wpx"; 
    m_names["muid_MET_em_loosepp_wpy"] = "muid_MET_em_loosepp_AntiKt4LCTopoJets_wpy"; 
    m_names["muid_MET_em_loosepp_wet"] = "muid_MET_em_loosepp_AntiKt4LCTopoJets_wet"; 
    m_names["muid_MET_em_loosepp_statusWord"] = "muid_MET_em_loosepp_AntiKt4LCTopoJets_statusWord"; 
  }

  void D3PDObjectNames::FillChargedHiggsTRCNtuple() {
    m_names["nTracksVertex"] = "nTracks";
    m_names["x"] = "x";
    m_names["y"] = "y";
    m_names["z"] = "z";
    m_names["pt"] = "pt";
    m_names["eta"] = "eta";
    m_names["phi"] = "phi";
    m_names["centermag"] = "centermag";
    m_names["constit_n"] = "constit_n";
    m_names["constit_index"] = "constit_index";
    m_names["truth_code"] = "truth_code";
    m_names["N"] = "n";
    m_names["e"] = "E";
    m_names["status"] = "status";
    m_names["barcode"] = "barcode";
    m_names["pdgId"] = "pdgId";
    m_names["charge"] = "charge";
    m_names["parent_index"] = "parent_index";
    m_names["child_index"] = "child_index";
    m_names["runNumber"] = "runNumber";
    m_names["eventNumber"] = "eventNumber";
    m_names["channelNumber"] = "channelNumber";
    m_names["rndRunNumber"] = "rndRunNumber";
    m_names["dataPeriod"] = "dataPeriod";
    m_names["mu"] = "mu";
    m_names["eventWeight"] = "eventWeight";
    m_names["pvxp_n"] = "pvxp_n";
    m_names["hfor"] = "hfor";
    m_names["averageIntPerXing"] = "averageIntPerXing";
    m_names["Eventshape_rhoKt4LC"] = "Eventshape_rhoKt4LC";
    m_names["alljet_n"] = "alljet_n";
    m_names["mc_vxp_z"] = "mc_vxp_z";
    m_names["mcevt_pdf1"] = "mcevt_pdf1";
    m_names["mcevt_pdf2"] = "mcevt_pdf2";
    m_names["mcevt_pdf_id1"] = "mcevt_pdf_id1";
    m_names["mcevt_pdf_id2"] = "mcevt_pdf_id2";
    m_names["mcevt_pdf_scale"] = "mcevt_pdf_scale";
    m_names["mcevt_pdf_x1"] = "mcevt_pdf_x1";
    m_names["mcevt_pdf_x2"] = "mcevt_pdf_x2";
    m_names["scaleFactor_PILEUP"] = "scaleFactor_PILEUP";
    m_names["scaleFactor_ELE"] = "scaleFactor_ELE";
    m_names["scaleFactor_MUON"] = "scaleFactor_MUON";
    m_names["scaleFactor_BTAG"] = "scaleFactor_BTAG";
    m_names["scaleFactor_TRIGGER"] = "scaleFactor_TRIGGER";
    m_names["scaleFactor_WJETSNORM"] = "scaleFactor_WJETSNORM";
    m_names["scaleFactor_WJETSSHAPE"] = "scaleFactor_WJETSSHAPE";
    m_names["scaleFactor_JVFSF"] = "scaleFactor_JVFSF";
    m_names["scaleFactor_ZVERTEX"] = "scaleFactor_ZVERTEX";
    m_names["lep_pt"] = "lep_pt";
    m_names["lep_eta"] = "lep_eta";
    m_names["lep_phi"] = "lep_phi";
    m_names["lep_E"] = "lep_E";
    m_names["lep_trackd0pvunbiased"] = "lep_trackd0pvunbiased";
    m_names["lep_tracksigd0pvunbiased"] = "lep_tracksigd0pvunbiased";
    m_names["lep_charge"] = "lep_charge";
    m_names["lep_trigMatch"] = "lep_trigMatch";
    m_names["el_cl_eta"] = "el_cl_eta";
    m_names["eptcone30"] = "eptcone30";
    m_names["eetcone20"] = "eetcone20";
    m_names["mptcone30"] = "mptcone30";
    m_names["metcone20"] = "metcone20";
    m_names["eminiIso10_4"] = "eminiIso10_4";
    m_names["met_sumet"] = "met_sumet";
    m_names["met_et"] = "met_et";
    m_names["met_phi"] = "met_phi";
    m_names["mwt"] = "mwt";
    m_names["jet_n"] = "jet_n";
    m_names["jet_pt"] = "jet_pt";
    m_names["jet_eta"] = "jet_eta";
    m_names["jet_phi"] = "jet_phi";
    m_names["jet_E"] = "jet_E";
    m_names["jet_charge"] = "jet_charge";
    m_names["jet_jvf"] = "jet_jvf";
    m_names["jet_trueflav"] = "jet_trueflav";
    m_names["jet_truthMatched"] = "jet_truthMatched";
    m_names["jet_SV0"] = "jet_SV0";
    m_names["jet_COMBNN"] = "jet_COMBNN";
    m_names["jet_MV1"] = "jet_MV1";
    m_names["jet_MV1c"] = "jet_MV1c";
    m_names["jet_isSemilep"] = "jet_isSemilep";
    m_names["lep_trackz0pvunbiased"] = "lep_trackz0pvunbiased";
    m_names["lep_tracksigz0pvunbiased"] = "lep_tracksigz0pvunbiased";
    m_names["el_isConv"] = "el_isConv";
    m_names["el_Rconv"] = "el_Rconv";
    m_names["el_zconv"] = "el_zconv";
    m_names["passHFtype"] = "passHFtype";
    m_names["HFtype"] = "HFtype";
    m_names["n_extraLep"] = "n_extraLep";
    m_names["n_extraDilep"] = "n_extraDilep";
    m_names["n_extraMultilep"] = "n_extraMultilep";
    m_names["particle_jet_id"] = "particle_jet_id";
    m_names["particle_jet_trueflav"] = "particle_jet_trueflav";
    m_names["particle_jet_n"] = "particle_jet_n";
    m_names["particle_jet_cut_n"] = "particle_jet_cut_n";
    m_names["particle_jet_hthad"] = "particle_jet_hthad";
    m_names["particle_jet_lead_pt"] = "particle_jet_lead_pt";
    m_names["particle_jet_pt"] = "particle_jet_pt";
    m_names["particle_jet_eta"] = "particle_jet_eta";
    m_names["particle_jet_phi"] = "particle_jet_phi";
    m_names["qq_hthad"] = "qq_hthad";
    m_names["qq_dr"] = "qq_dr";
    m_names["top_pt"] = "top_pt";
    m_names["top_eta"] = "top_eta";
    m_names["ttH_pt"] = "ttH_pt";
    m_names["ttbar_pt"] = "ttbar_pt";
    m_names["ttbar_eta"] = "ttbar_eta";
    m_names["higgs_decay"] = "higgs_decay";
  }

  void D3PDObjectNames::FillUZHCMSNtuple() {
    
    m_names["N"] = "N";
    // general kinematics
    m_names["e"] = "e";
    m_names["pt"] = "pt";
    m_names["m"] = "mass";
    m_names["eta"] = "eta";
    m_names["phi"] = "phi";
    
    m_names["flavour"] = "flavour";

    m_names["IDLoose"] = "IDLoose";
    m_names["jec"] = "jec";
    m_names["muf"] = "muf";
    m_names["phf"] = "phf";
    m_names["emf"] = "emf";
    m_names["nhf"] = "nhf";
    m_names["chf"] = "chf";
    m_names["area"] = "area";
    m_names["cm"] = "cm";
    m_names["nm"] = "nm";
    m_names["che"] = "che";
    m_names["ne"] = "ne";
    m_names["charge"] = "charge";

    m_names["tau1"] = "tau1";
    m_names["tau2"] = "tau2";
    m_names["tau3"] = "tau3";
    m_names["pruned_mass"] = "pruned_mass";
    m_names["pruned_massCorr"] = "pruned_massCorr";
    m_names["softdrop_mass"] = "softdrop_mass";
    m_names["softdrop_massCorr"] = "softdrop_massCorr";
    m_names["pruned_jec"] = "pruned_jec";
    m_names["softdrop_jec"] = "softdrop_jec";
    
  }

  TString D3PDObjectNames::getName( const TString& name ) {

    // check if key exists
    std::map<TString, TString>::iterator found = m_names.find( name ); // Search for key
    if ( found != m_names.end() ) {
      return m_names[name];
    } else {
      //m_logger << WARNING << "D3PDObjectName" << name << "not found in map" << SLogger::endmsg;
      std::cout << "WARNING: D3PDObjectName " << name << " not found in map" << std::endl;
      return name;
    }

  }


  TString D3PDObjectNames::getPrefix( const TString& name ) {

    // check if key exists
    std::map<TString, TString>::iterator found = m_prefixes.find( name ); // Search for key
    if ( found != m_prefixes.end() ) {
      return m_prefixes[name];
    } else {
      throw SError( Form("D3PDObjectName %s not available!", name.Data()), SError::StopExecution );
    }

  }


} // namespace D3PD
