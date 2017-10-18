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
    // class Tau;
    class MissingEt;
    // class Electron;
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
    virtual void applyTurnOn ();

private:
  

    // input variable objects:


    Ntuple::JetNtupleObject         m_jetAK8;            //< jet container
    Ntuple::JetNtupleObject         m_jetAK4;            //< jet container

    Ntuple::JetNtupleObject         m_genJetAK8;            //< jet container
    Ntuple::JetNtupleObject         m_genJetAK4;            //< jet container

    Ntuple::EventInfoNtupleObject   m_eventInfo; //< event info container
    // Ntuple::TauNtupleObject         m_tau;            //< tau container
    Ntuple::MissingEtNtupleObject   m_missingEt;            //< missing E_T container
    Ntuple::GenParticleNtupleObject m_genParticle;            ///< gen particle container
    // Ntuple::ElectronNtupleObject    m_electron;            ///< electron container
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
    bool        m_isTTbarUnbinned;

    bool        m_isRunH;

    // bool        m_applyMETFilters;
    
    //selections
    double m_metCut;
    double m_jetPtCut;
    double m_jetEtaCut;
    double m_jetHbbtagMin;
    double m_jetMetDeltaPhiMin;
    double m_jetMassMin;
    double m_jetMassMax;

    double m_muonPtCut;
    double m_muonEtaCut;
    double m_mumuMassMin;
    double m_mumuMassMax;



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

    // event variables
    int b_runNumber;
    int b_eventNumber;
    int b_lumiBlock;

    // dummy variable
    bool b_pass;
    bool is_btag_SR;
    double event_HT;



    // TLV templates
    std::vector<TLorentzVector> jetAK4_TLV;
    std::vector<TLorentzVector> jetAK8_TLV;

    // muon 
    std::vector<TLorentzVector> muon_TLV;
    std::vector<double> muon_rawIso ;
    std::vector<double> muon_charge ;

    // jet variables (templates)
    std::vector<double> jetAK8_Hbbtag ;
    std::vector<double> jetAK8_pruned_mass ;
    std::vector<bool>   jetAK8_IDTight ;


    // preselected TLV
    std::vector<TLorentzVector> goodAK8_TLV;

    // preselected variables
    std::vector<double> goodAK8_Hbbtag ;
    std::vector<double> goodAK8_pruned_mass ;

    // selected TLV
    TLorentzVector goodAK08TLV_sel;
    // TLorentzVector sumAll_TLV_sel;

    // selected variables
    double jetPrunedMass;
    double jetHbbtag;

    // MET
    double met_et;
    double met_px;
    double met_py;


    TFile * trig_turnon_eff;
    TEfficiency * trig_eff;
    TEfficiency * trig_eff_coarse;




    TRandom3 rand;


    // Macro adding the functions for dictionary generation
    ClassDef( VHTausAnalysis, 0 );

}; // class VHAnalysis

#endif // VHAnalysis_H

