


    // select AK08 jet
    // std::vector<UZH::Jet> goodFatJets;

    // for ( int i = 0; i < (m_jetAK8.N); ++i ) 
    // {
    //     UZH::Jet myjet( &m_jetAK8, i );

    //     TLorentzVector jetTLV;
    //     jetTLV.SetPtEtaPhiE(myjet.pt(), myjet.eta(), myjet.phi(), myjet.e());

    //     TVector2 v2_jet (jetTLV.Px(), jetTLV.Py());
    //     TVector2 v2_met (met_px, met_py);
    //     double deltaPhi = fabs( v2_jet.DeltaPhi(v2_met) );
    //     double prunedMass = myjet.pruned_massCorr();

    //     bool isJetID = (myjet.IDTight()) && (myjet.muf() < 0.8);
    //     bool passCuts =  (myjet.pt() > m_jetPtCut) && (fabs(myjet.eta()) < m_jetEtaCut);
    //     bool massWindow = (prunedMass > m_jetMassMin); //&& (prunedMass < m_jetMassMax);
    //     // bool isHbbtag = (myjet.Hbbtag() > m_jetHbbtagMin);
    //     bool isMetAngle = (deltaPhi > m_jetMetDeltaPhiMin);

    //     if (passCuts && isJetID && massWindow && isMetAngle /*&& isHbbtag*/)
    //         goodFatJets.push_back(myjet);


    // }//end loop on jets


    
    bool foundJet = (goodFatJets.size() > 0);
    if (!foundJet) 
        return;

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
        // bool isTauIso = mytau.byVLooseIsolationMVA3newDMwLT();
        bool isTauBoosted = mytau.TauType()==2;
        bool againstElectron = mytau.againstElectronVLooseMVA5();
        bool againstMuon = mytau.againstMuonLoose3();


        if (passCuts && isTauDM && isTauBoosted && againstElectron && againstMuon /*&& isTauIso*/)
            goodTaus.push_back(mytau);


    }//end loop on taus


    if (goodTaus.size() < 2)
        return;

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

                    bool isTauIso_1 = goodTaus.at(iTau).byVLooseIsolationMVA3newDMwLT();
                    bool isTauIso_2 = goodTaus.at(jTau).byVLooseIsolationMVA3newDMwLT();
                    bool atLeastOneIso = isTauIso_1 || isTauIso_2;

                    //double sumIso = goodTaus.at(iTau).byIsolationMVA3newDMwLTraw() + goodTaus.at(jTau).byIsolationMVA3newDMwLTraw();

                    if ((sumPT >= sumPT_leading) && (sumPT > m_tautauPtCut) && (deltaR_tautau < m_tautauDRMax)
                        && (deltaR_tautaujet > m_tautauJetDRMin) && (resonanceMass > m_tautauJetMassMin)
                        /*&& (mass > m_tautauMassMin) && (mass < m_tautauMassMax) *//*&& ( sumIso > -0.5 )*/ && atLeastOneIso)
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
    if (!foundTauPair) //return;
        return;

    if (tau1_TLV_sel.Pt() < tau2_TLV_sel.Pt() )
    {
        TLorentzVector temp = tau1_TLV_sel;
        tau1_TLV_sel = tau2_TLV_sel;
        tau2_TLV_sel = temp;
    }


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
                


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    b_pass = true;

    TLorentzVector jet, tau1, tau2, sumTau, sumAll;
    tau1.SetPtEtaPhiE(selectedTau_1.pt(), selectedTau_1.eta(), selectedTau_1.phi(), selectedTau_1.e());
    tau2.SetPtEtaPhiE(selectedTau_2.pt(), selectedTau_2.eta(), selectedTau_2.phi(), selectedTau_2.e());
    jet.SetPtEtaPhiE(selectedJet.pt(),selectedJet.eta(),selectedJet.phi(),selectedJet.e());
    sumTau = tau1 + tau2 ;
    sumAll = sumTau + jet ;

    if (!m_isData)
    {
        b_weight = getEventWeight();
        double pt = selectedJet.pt();

        if (event_HT<1000.)
            b_weight *= trig_eff_HT->GetEfficiency(trig_eff_HT->FindFixBin(event_HT));

                // if (m_isSignal)
                // {
                //     double factor = 0.;
                    
                //     if (is_btag_SR)
                //     {

                //         if      ( pt>=300. && pt<400. ) b_weight *= SF_btag_loose_300_400 + factor*err_SF_btag_loose_300_400;
                //         else if ( pt>=400. && pt<500. ) b_weight *= SF_btag_loose_400_500 + factor*err_SF_btag_loose_400_500; 
                //         else if ( pt>=500. && pt<600. ) b_weight *= SF_btag_loose_500_600 + factor*err_SF_btag_loose_500_600; 
                //         else if ( pt>=600.            ) b_weight *= SF_btag_loose_600_700 + factor*err_SF_btag_loose_600_700; 
                //         else                            b_weight *= 1.;
                //     }

                //     else 
                //     {
                //         if      ( pt>=300. && pt<400. ) b_weight *= 2.- (SF_btag_loose_300_400 + factor*err_SF_btag_loose_300_400);
                //         else if ( pt>=400. && pt<500. ) b_weight *= 2.- (SF_btag_loose_400_500 + factor*err_SF_btag_loose_400_500); 
                //         else if ( pt>=500. && pt<600. ) b_weight *= 2.- (SF_btag_loose_500_600 + factor*err_SF_btag_loose_500_600); 
                //         else if ( pt>=600.            ) b_weight *= 2.- (SF_btag_loose_600_700 + factor*err_SF_btag_loose_600_700); 
                //         else                            b_weight *= 1.;
                //     }

                // }//if signal

                // else if (m_isTTbar)
                // {
                //     double factor = 0.;
                    
                //     if (is_btag_SR)
                //     {
                //         if      ( pt>=300. && pt<400. ) b_weight *= mistag_SF_btag_loose_300_400 + factor*err_mistag_SF_btag_loose_300_400;
                //         else if ( pt>=400. && pt<550. ) b_weight *= mistag_SF_btag_loose_400_550 + factor*err_mistag_SF_btag_loose_400_550; 
                //         else if ( pt>=550.            ) b_weight *= mistag_SF_btag_loose_550_Inf + factor*err_mistag_SF_btag_loose_550_Inf; 
                //         else                            b_weight *= 1.;
                //     }

                //     else
                //     {
                //         if      ( pt>=300. && pt<400. ) b_weight *= 2.- (mistag_SF_btag_loose_300_400 + factor*err_mistag_SF_btag_loose_300_400);
                //         else if ( pt>=400. && pt<550. ) b_weight *= 2.- (mistag_SF_btag_loose_400_550 + factor*err_mistag_SF_btag_loose_400_550); 
                //         else if ( pt>=550.            ) b_weight *= 2.- (mistag_SF_btag_loose_550_Inf + factor*err_mistag_SF_btag_loose_550_Inf); 
                //         else                            b_weight *= 1.;
                //     }


                // } //if ttbar

                // else b_weight *= 1.;


    }//if not data


    fillHistograms(m_catNames[0], selectedJet, selectedTau_1, selectedTau_2, goodMet, 
                   sumTauTLV_sel, sumAllTLV_sel, goodAK04_CSV_M );


    if (m_isTTbar){
        fillHistograms(m_catNames[n_lepW+1], selectedJet, selectedTau_1, selectedTau_2, goodMet, 
                       sumTauTLV_sel, sumAllTLV_sel, goodAK04_CSV_M);
    }

    return;



