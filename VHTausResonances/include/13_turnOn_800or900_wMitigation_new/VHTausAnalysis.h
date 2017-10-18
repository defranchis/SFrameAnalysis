// Dear emacs, this is -*- c++ -*-
// $Id: CycleCreators.py 344 2012-12-13 13:10:53Z krasznaa $
#ifndef VHTausAnalysis_H
#define VHTausAnalysis_H

// SFrame include(s):
#include "core/include/SCycleBase.h"

// ROOT include(s):
#include <TBits.h>
#include <TH2.h>
#include <TH2F.h>
#include "TEfficiency.h"
#include "TFile.h"
#include "TRandom3.h"
#include "LHAPDF/LHAPDF.h"

// External include(s):
#include "../NtupleVariables/include/JetNtupleObject.h"
#include "../NtupleVariables/include/Jet.h"
#include "../NtupleVariables/include/EventInfoNtupleObject.h"
#include "../NtupleVariables/include/TauNtupleObject.h"
#include "../NtupleVariables/include/Tau.h"
#include "../NtupleVariables/include/MissingEtNtupleObject.h"
#include "../NtupleVariables/include/MissingEt.h"
#include "../GoodRunsLists/include/TGoodRunsList.h"
#include "../PileupReweightingTool/include/PileupReweightingTool.h"

#include "../NtupleVariables/include/ElectronNtupleObject.h"
#include "../NtupleVariables/include/Electron.h"
#include "../NtupleVariables/include/MuonNtupleObject.h"
#include "../NtupleVariables/include/Muon.h"
#include "../NtupleVariables/include/GenParticleNtupleObject.h"
#include "../NtupleVariables/include/GenParticle.h"
//#include "../BTaggingTools/include/BTaggingScaleTool.h"


// class TH2D;
// class TRandom3;


class TH1D;
class TBits;
namespace UZH {

    class Jet;
    class TruthJet;
    class Tau;
    class MissingEt;
    class Electron;
    class Muon;
    class GenParticle;

}


class VHTausAnalysis : public SCycleBase {

public:
  
    // Enumeration of all cut flags
    typedef enum {

        kBeforeCuts,            // C0
        kJSON,                  // C1
        kTrigger,               // C2
        kMetFilters,            // C3
        kMet,                   // C4
        kJet,                   // C5
        kTau,                   // C6
        kNumCuts                // last!

    } SelectionCuts;
  
    // static array of all cut names
    static const std::string kCutName[ kNumCuts ];
  
    // Default constructor
    VHTausAnalysis();
    // Default destructor
    ~VHTausAnalysis();


    // Function called at the beginning of the cycle
    virtual void BeginCycle() throw( SError );
    // Function called at the end of the cycle
    virtual void EndCycle() throw( SError );

    // Function called at the beginning of a new input data
    virtual void BeginInputData( const SInputData& ) throw( SError );
    // Function called after finishing to process an input data
    virtual void EndInputData  ( const SInputData& ) throw( SError );

    // Function called after opening each new input file
    virtual void BeginInputFile( const SInputData& ) throw( SError );

    // Function called for every event
    virtual void ExecuteEvent( const SInputData&, Double_t ) throw( SError );
   
    // Function to check good lumi section
    virtual bool isGoodEvent(int runNumber, int lumiSection);
   
    // Function to check for trigger pass
    virtual bool passTrigger();
   
    // Function to check for MET filters pass
    virtual bool passMETFilters();
   
    // Function to obtain event weights for MC
    virtual double getEventWeight();
   
    // Function to clear/reset all output branches
    virtual void clearBranches();
   
    // Function to fill cut flow
    virtual void fillCutflow( const std::string histName, const std::string dirName, const TBits& cutmap, const Double_t weight );
   
    // Function to book histograms
    virtual void bookHistograms( const TString& directory );
   

    // virtual void fillHistograms( const TString& directory, const TLorentzVector& jet , const double& Hbbtag, const double& pruned_mass, 
    //                              const UZH::Tau& Tau1, const UZH::Tau& Tau2, TLorentzVector& sumTau, TLorentzVector& sumAll);

    virtual void fillHistograms( const TString& directory );


    virtual void createTemplates ();

    virtual void applyHbbtagSF (const TLorentzVector& selectedJet );
    virtual void applyPDFreweight ();
    virtual void applyTauIDerror();

private:
  

    // input variable objects:


    Ntuple::JetNtupleObject         m_jetAK8;            //< jet container
    Ntuple::JetNtupleObject         m_jetAK4;            //< jet container

    Ntuple::JetNtupleObject         m_genJetAK8;            //< jet container
    Ntuple::JetNtupleObject         m_genJetAK4;            //< jet container

    Ntuple::EventInfoNtupleObject   m_eventInfo; //< event info container
    Ntuple::TauNtupleObject         m_tau;            //< tau container
    Ntuple::MissingEtNtupleObject   m_missingEt;            //< missing E_T container
    Ntuple::GenParticleNtupleObject m_genParticle;            ///< gen particle container
    Ntuple::ElectronNtupleObject    m_electron;            ///< electron container
    Ntuple::MuonNtupleObject        m_muon;            ///< muon container

    // further objects
    Root::TGoodRunsList m_grl;
    PileupReweightingTool m_pileupReweightingTool;
  

    // XML settings for VHAnalysis
    // naming
    std::string m_recoTreeName;       //< name of tree with reconstructed objects in ntuple
    std::string m_outputTreeName;    //< name of output tree
    //int m_ntupleLevel;               //< cut at which branches for ntuple are written out
    std::string m_jetAK4Name;            //< name of AK4 jet collection in tree with reconstructed objects
    std::string m_jetAK8Name;       //< name of AK8 jet collection in tree with reconstructed objects
    std::string m_genJetAK4Name;            //< name of AK4 genJet collection in tree with reconstructed objects
    std::string m_genJetAK8Name;       //< name of AK8 genJet collection in tree with reconstructed objects
    std::string m_electronName;       //< name of electron collection in tree with reconstructed objects
    std::string m_muonName;       //< name of muon collection in tree with reconstructed objects
    std::string m_tauName;       //< name of tau collection in tree with reconstructed objects
    std::string m_missingEtName;       //< name of missing E_T collection in tree with reconstructed objects
    std::string m_genParticleName;       //< name of gen particle collection in tree with reconstructed objects
  
    // flags
    bool        m_isData;
    bool        m_isSignal;
    bool        m_isTTbar;
    bool        m_isQCD;

    // bool        m_applyMETFilters;
    
    //selections
    double m_metCut;
    double m_jetPtCut;
    double m_jetEtaCut;
    double m_jetHbbtagMin;
    double m_jetMetDeltaPhiMin;
    double m_jetMassMin;
    double m_jetMassMax;
    double m_tauPtCut;
    double m_tauEtaCut;
    double m_tautauPtCut;
    double m_tautauDRMax;
    double m_tautauDRMin;
    double m_tautauMetDeltaPhiMax;
    double m_tautauMassMin;
    double m_tautauMassMax;
    double m_tautauJetDRMin;
    double m_tautauJetDRMax;
    double m_tautauJetMassMin;

    double m_electronPtCut;
    double m_electronEtaCut;
    double m_muonPtCut;
    double m_muonEtaCut;



    // int n_lepW;
    // int n_lepTaus;
    // int n_electrons;
    // int n_muons;
    // int n_CSV_tags;
    // double Mtt;

    // further settings
    std::string m_jsonName;
  
    // other variables needed
    std::vector<std::string> m_triggerNames;
    std::vector<std::string> m_catNames;
  
    // branches
    double b_weight;
    double b_weightGen;
    double b_weightPU;
    double b_weightBtag;

    double w_tauID;
  
    // event variables
    int b_runNumber;
    int b_eventNumber;
    int b_lumiBlock;

    // dummy variable
    bool b_pass;

    bool is_btag_SR;
    bool is_tauiso_SR;

    double event_HT;

    // loose double-b tagger SF

    double SF_btag_loose_300_400 = .951;
    double SF_btag_loose_400_500 = .982;
    double SF_btag_loose_500_600 = .900;
    double SF_btag_loose_600_700 = .958;

    double err_SF_btag_loose_300_400 = .073;
    double err_SF_btag_loose_400_500 = .119;
    double err_SF_btag_loose_500_600 = .178;
    double err_SF_btag_loose_600_700 = .250;

    double mistag_SF_btag_loose_300_400 = 1.24;
    double mistag_SF_btag_loose_400_550 = 1.12;
    double mistag_SF_btag_loose_550_Inf = 1.40;

    double err_mistag_SF_btag_loose_300_400 = .13;
    double err_mistag_SF_btag_loose_400_550 = .13;
    double err_mistag_SF_btag_loose_550_Inf = .32;

    // medium double-b tagger SF

    double SF_btag_medium_300_400 = .929;
    double SF_btag_medium_400_500 = .999;
    double SF_btag_medium_500_600 = .933;
    double SF_btag_medium_600_700 = 1.048;

    double err_SF_btag_medium_300_400 = .078;
    double err_SF_btag_medium_400_500 = .126;
    double err_SF_btag_medium_500_600 = .195;
    double err_SF_btag_medium_600_700 = .215;

    double mistag_SF_btag_medium_300_400 = 1.14;
    double mistag_SF_btag_medium_400_550 = 1.01;
    double mistag_SF_btag_medium_550_Inf = 1.13;

    double err_mistag_SF_btag_medium_300_400 = .16;
    double err_mistag_SF_btag_medium_400_550 = .17;
    double err_mistag_SF_btag_medium_550_Inf = .39;


    // JER scale factors
    double JER_SF_00_08 = 1.061;
    double JER_SF_08_13 = 1.088;
    double JER_SF_13_19 = 1.106;
    double JER_SF_19_25 = 1.126;
    double JER_SF_25_30 = 1.343;

    double err_JER_SF_00_08 = .023;
    double err_JER_SF_08_13 = .029;
    double err_JER_SF_13_19 = .030;
    double err_JER_SF_19_25 = .094;
    double err_JER_SF_25_30 = .123;

    int do_JEC_AK4_shift ;
    int do_JEC_AK8_shift ;
    int do_JER_AK4_shift ;
    int do_JER_AK8_shift ;
    int do_pruned_shift ;
    int do_Hbb_tag_SF_shift ;
    int do_Hbb_mistag_SF_shift ;
    int do_turnon_shift;
    int do_tau_shift;
    int do_tauID_shift;
    int do_pdf_shift;
    

    // TLV templates
    std::vector<TLorentzVector> jetAK4_TLV;
    std::vector<TLorentzVector> jetAK8_TLV;
    std::vector<TLorentzVector> tau_TLV;

    // jet variables (templates)
    std::vector<double> jetAK8_Hbbtag ;
    std::vector<double> jetAK8_pruned_mass ;
    std::vector<bool>   jetAK8_IDTight ;

    // tau variables (templates)
    std::vector<bool>   tau_VLooseIso ;
    std::vector<bool>   tau_LooseIso ;
    std::vector<bool>   tau_MediumIso ;
    std::vector<double> tau_rawIso ;

    // preselected TLV
    std::vector<TLorentzVector> goodAK8_TLV;
    std::vector<TLorentzVector> goodTau_TLV;

    // preselected variables
    std::vector<double> goodAK8_Hbbtag ;
    std::vector<double> goodAK8_pruned_mass ;
    std::vector<bool>   goodTau_VLooseIso ;
    std::vector<bool>   goodTau_LooseIso ;
    std::vector<bool>   goodTau_MediumIso ;
    std::vector<double> goodTau_rawIso ;

    // selected TLV
    TLorentzVector goodAK08TLV_sel;
    TLorentzVector tau1_TLV_sel;
    TLorentzVector tau2_TLV_sel;
    TLorentzVector sumTau_TLV_sel;
    TLorentzVector sumAll_TLV_sel;

    // selected variables
    double jetPrunedMass;
    double jetHbbtag;
    double tau1_rawIso;
    double tau2_rawIso;
    double original_tau_pt_1;
    double original_tau_pt_2;

    // MET
    double met_et;
    double met_px;
    double met_py;

    // stuff
    double a, b;

    // utility
    // TFile * trig_turnon_eff_HT;
    TEfficiency * trig_eff_HT;
    TRandom3 rand;
    LHAPDF::PDFSet set = LHAPDF::getPDFSet("PDF4LHC15_nlo_mc");
    const std::vector<LHAPDF::PDF*> pdfs = set.mkPDFs();

    bool pass_trigger;


    // std::vector<Int_t>      b_selection_bits;
    // std::vector<Int_t>      b_selection_lastcut;

    // Macro adding the functions for dictionary generation
    ClassDef( VHTausAnalysis, 0 );

}; // class VHAnalysis

#endif // VHAnalysis_H

