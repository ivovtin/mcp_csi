//function for fit
TF1* Init_Aprox(Int_t num, const char* name) {
	TF1* f = new TF1(name ,"[0] + [1] * x", 0, 200);
     	f -> SetParName(0, "Const");
     	f -> SetParName(1, "Angle");
     	f -> SetParLimits(1, num, 0);
	f -> SetTitle(name);
	return f;
}

void plot_runs(TString namedir, Float_t K,  Int_t nev, Int_t nev1, Int_t nev2) {
   
    gStyle -> SetPalette(1);
    bool verbose=0;
 
    TString inDir="/home/petruhin/development/test_beam_07_04_2021/";
    TString inDirData = inDir + namedir + "/" + namedir + ".root"; 
    TString outDir="/home/ovtin/development/mcp_csi/";

    TChain *exbeamdata=new TChain("exbeamdata");
    exbeamdata->Add(inDirData);

    TCanvas* CanvAmp = new TCanvas("CanvAmp", "Amplitude distributions", 1000, 600, 600, 600);
    CanvAmp -> Divide(2, 3);
    TCanvas* CanvTimeDistribution = new TCanvas("CanvTimeDistribution", "Time distribution", 500, 600, 600, 600);
    CanvTimeDistribution -> Divide(2, 2);
    
    TH1F* htrg1 = new TH1F("htrg1", "MCP PMT spectra", 4096, 0, 4096);
    TH1F* htrg2 = new TH1F("htrg2", "MCP PMT spectra", 4096, 0, 4096);
    TH1F* ht = new TH1F("ht","#Delta t for MCP", 4001, -20.005, 20.005);
    TH1F* ht0 = new TH1F("ht0","#Zero peak for MCP", 4001, -20.005, 20.005);
    TH1F* TimeDistributionTriggers  =  new TH1F("TimeDistributionTriggers", "Time distribution between triggers MCP", 4001, -20.005, 20.005);
    TH2F* TimeVSAmplitude = new TH2F("TimeVSAmplitude", "Time VS Amplitude", 10000, 0, 5000, 10000, 0, 5000);
    TH1F* amptrgChi2 = new TH1F("amptrgChi2", "Chi2 for trg MCP", 2000, 0, 2000);
    TH1F* ampChi2 = new TH1F("ampChi2", "Chi2 for prototype", 2000, 0, 2000);

    //selection conditions for trgMCP and DAQ
    TString Conditions_1 = "(exbeamdata.chtrg2r.ped - exbeamdata.chtrg2r.min) > 300 && (exbeamdata.chtrg2r.ped - exbeamdata.chtrg2r.min) < 900 && (exbeamdata.chtrg1r.ped - exbeamdata.chtrg1r.min) > 300 && (exbeamdata.chtrg1r.ped - exbeamdata.chtrg1r.min) < 900";
    TString Conditions_all = Conditions_1;

    CanvAmp -> cd(1);
    //Amplitude of min in trigger per event
    exbeamdata -> Draw("exbeamdata.chtrg1r.ped - exbeamdata.chtrg1r.min >> htrg1");
    exbeamdata -> Draw("exbeamdata.chtrg2r.ped - exbeamdata.chtrg2r.min >> htrg2"); 
    htrg1 -> SetLineColor(2);
    htrg2 -> SetLineColor(4);
    htrg1 -> Draw();
    htrg2 -> Draw("same");
    TLine* mcpb1 = new TLine(300, 0, 300, 400);
    TLine* mcpb2 = new TLine(900, 0, 900, 400);
    mcpb1 -> Draw("same");
    mcpb2 -> Draw("same");
    CanvAmp -> Update();

    TProfile* amp = new TProfile("amp", "a(t) for MCP ", 1024, 1. / 1024, 0.2 * 1024);
    TProfile* ampf = new TProfile("ampf", "a(t)f for MCP", 1024, 1. / 1024, 0.2 * 1024);
    TProfile* ampavg = new TProfile("ampavg", "Average a(t) for MCP", 1024, 1. / 1024, 0.2 * 1024);
    TProfile* amptrg1 = new TProfile("amptrg1", "a1(t) for trgmcp1", 1024, 1. / 1024, 0.2 * 1024);
    TProfile* amptrg2 = new TProfile("amptrg2", "a2(t) for trgmcp2", 1024, 1. / 1024, 0.2 * 1024);
    TProfile2D* dtvsxy = new TProfile2D("dtvsxy", "#Delta T vs XY", 140, 0, 140, 40, 0, 40);
    
    //Aproximations for signal   
    TF1* funtrg1 = Init_Aprox(-200000, "funtrg1");      //trgmcp1
    TF1* funtrg2 = Init_Aprox(-200000, "funtrg2");    //trgmcp2

    Int_t nentries = 0;
    if (exbeamdata -> GetEntries() < nev) nev = exbeamdata -> GetEntries();
    if(verbose) cout << nev << endl;
    Int_t NGFirst = 0;
    Int_t NGSecond = 0;
    Int_t i=0;
    
    for (i ; i < nev; i++)
    {
    	if (!(i > nev1 && i < nev2)) 
	{
		exbeamdata -> Draw("(exbeamdata.ch1r.ped - exbeamdata.ch1.amp):exbeamdata.ch1.ti / 5. >> ampf", Conditions_all , "goff", 1, i);
		exbeamdata -> Draw("exbeamdata.ch1.amp:exbeamdata.ch1.ti / 5. >> amp", Conditions_all, "goff", 1, i);
		if(verbose) cout<<i<<endl;
		//Dynamic borders
		Float_t MaxFitBordL;
		Int_t MaxFitBordR;
		Int_t MaxBin;
		Int_t MeanBin;
		Float_t MaxFitBordL1,H=3;
		
		amp -> SetAxisRange(10, 100, "X");
		Float_t mean = amp -> GetMean(2);
		if(verbose) cout<<"mean="<<mean<<endl;
		amp -> SetAxisRange(10, 199, "X");

		Float_t MaximumSignalBin = amp -> GetMaximumBin();
		Float_t MinimumSignal = amp -> GetMinimum();
		Float_t MinimumSignalBin = amp -> GetMinimumBin();
		
		if(verbose) cout<<"minimum="<<MinimumSignal<<endl;
		//Find Maximum
		for(Int_t i = 11; i<=1024; i++)
		{
			if(amp->GetBinContent(i)== MinimumSignal && amp->GetBinContent(i)>0)
			{
				MaxBin = i;
				i=1024;
				if(verbose) cout<<"minimumBin="<<MaxBin<<endl;
			}
			else
			{
				MaxBin = 0;
			}
		}
		//Find center	
		for(Int_t i = 1; i<=1024; i++)
		{
			if(amp->GetBinContent(MaxBin-i)> MinimumSignal+(mean-MinimumSignal)/2)
			{
				MeanBin = MaxBin-i;
				i=1024;
				if(verbose) cout<<"meanPeakBin="<<MeanBin<<"meanPeak="<<amp->GetBinContent(MeanBin)<<endl;
			}
			else
			{
				MeanBin = 0;
			}
		}
		
		MaxFitBordR=MeanBin+3;
		MaxFitBordL=MeanBin-2;

		if(verbose) cout<<"MaxFitBordR="<<MaxFitBordR<<"BordRamp="<<amp->GetBinContent(MaxFitBordR)<<endl;
		if(verbose) cout<<"MaxFitBordL="<<MaxFitBordL<<"BordLamp="<<amp->GetBinContent(MaxFitBordL)<<endl;
			
 		amp -> SetAxisRange(2, 20, "X");
        	Float_t pedmax = amp -> GetMaximum();
        	Float_t pedmin = amp -> GetMinimum();
			
                amp -> SetAxisRange(10, 120, "X");
		
		Float_t MinimumSignal0 = amp -> GetMinimum();

		exbeamdata -> Draw("exbeamdata.chtrg1.amp:exbeamdata.chtrg1.ti / 5. >> amptrg1", Conditions_all, "goff", 1, i);
            	exbeamdata -> Draw("exbeamdata.chtrg2.amp:exbeamdata.chtrg2.ti / 5. >> amptrg2", Conditions_all, "goff", 1, i);

		amptrg1 -> SetAxisRange(30, 45, "X");
            	Float_t pedtrg1 = amptrg1 -> GetMean(2);  //mean along x axis
            	amptrg1 -> SetAxisRange(30, 200, "X");
            	Float_t minbintrg1 = amptrg1 -> GetMinimumBin();
            	Float_t mintrg1 = amptrg1 -> GetMinimum();
     		funtrg1 -> SetParameter(0, pedtrg1);
            	funtrg1 -> SetParameter(1, -2000);
	        if(verbose) cout<<"pedtrg1="<<pedtrg1<<"\t"<<"mintrg1="<<mintrg1<<endl; 

   		amptrg2 -> SetAxisRange(30, 45, "X");
	        Float_t pedtrg2 = amptrg2 -> GetMean(2);
            	amptrg2 -> SetAxisRange(30, 200, "X");
            	Float_t minbintrg2 = amptrg2 -> GetMinimumBin();
		Float_t mintrg2 = amptrg2 -> GetMinimum();
         	funtrg2 -> SetParameter(0, pedtrg2);
            	funtrg2 -> SetParameter(1, -2000);
                if(verbose) cout<<"pedtrg2="<<pedtrg2<<"\t"<<"mintrg2="<<mintrg2<<endl; 

		Int_t Bin2trg1=220.;
                for(Bin2trg1; Bin2trg1<1024.; Bin2trg1++)
                { 
			if(  (amptrg1 -> GetBinContent(Bin2trg1) / pedtrg1 ) <= 0.9806 ) break;
		}
                Int_t FitBin1trg1=Bin2trg1-1.;

		Int_t Bin2trg2=220.;
                for(Bin2trg2; Bin2trg2<1024.; Bin2trg2++)
                { 
			if(  (amptrg2 -> GetBinContent(Bin2trg2) / pedtrg2 ) <= 0.9806 ) break;
		}
                Int_t FitBin1trg2=Bin2trg2-1.;

            	//amptrg1 -> Fit(funtrg1, "W", "S", (minbintrg1 - 5.5)/5., (minbintrg1 - 1.5) / 5.);
            	//amptrg2 -> Fit(funtrg2, "W", "S", (minbintrg2 - 5.5)/5., (minbintrg2 - 1.5) / 5.);
                amptrg1 -> Fit(funtrg1, "W", "S", FitBin1trg1/5., (minbintrg1 - 1.5) / 5.);
            	amptrg2 -> Fit(funtrg2, "W", "S", FitBin1trg2/5., (minbintrg2 - 1.5) / 5.);
	    	if(verbose) cout<<"for amptrg1 chi2="<<funtrg1->GetChisquare()<<endl;	 
	    	if(verbose) cout<<"for amptrg2 chi2="<<funtrg2->GetChisquare()<<endl;	 

                //calculation the time for trgmcp1 and trgmcp2
		Float_t tcrtrg1 = (-K * (pedtrg1 - mintrg1) + pedtrg1 - funtrg1 -> GetParameter(0)) / (funtrg1 -> GetParameter(1));
	    	Float_t tcrtrg2 = (-K * (pedtrg2 - mintrg2) + pedtrg2 - funtrg2 -> GetParameter(0)) / (funtrg2 -> GetParameter(1));
		
		if((tcrtrg1 - tcrtrg2) >= 0) continue;

		TimeDistributionTriggers -> Fill(tcrtrg1 - tcrtrg2);
                amptrgChi2->Fill(funtrg1->GetChisquare()); 
                amptrgChi2->Fill(funtrg2->GetChisquare()); 


         	amp -> SetAxisRange(10, 200, "X");
		if (amp->GetBinContent(MaxFitBordL) - amp->GetBinContent(MaxFitBordR) > 0  && (pedmax- MinimumSignal) > 1 * (pedmax - pedmin) && MaxFitBordR-MaxFitBordL>1 && MaximumSignalBin < 1024 && MaxFitBordL>600 && MaxFitBordR - MaxFitBordL <10 && mean - MinimumSignal>10 && (mean-MinimumSignal0)/(mean-MinimumSignal)<0.5)
		{
			exbeamdata -> Draw("exbeamdata.ch1.amp:exbeamdata.ch1.ti / 5. >> amp", Conditions_all, "goff", 1, i);
           		 
            		amp -> SetAxisRange(2, 20, "X");
            		Float_t ped = amp -> GetMean(2);
			Float_t ped1 = amp -> GetMean(2);
            		amp -> SetAxisRange(2, 200, "X");
            		Float_t min = amp -> GetMinimum();
			Float_t min1 = amp -> GetMinimum();

                        ampavg -> SetAxisRange(2, 20, "X");
            		Float_t pedavg = ampavg -> GetMean(2);
            		ampavg -> SetAxisRange(2, 200, "X");
            		Float_t minavg = ampavg -> GetMinimum();
	   		 
                        TF1 *fun = new TF1("fun","[0] + [1] * x", MaxFitBordL, MaxFitBordR);      
                        fun -> SetParameter(0, ped);
            		fun -> SetParameter(1, -200);

 			Int_t Bin2prot=220.;
                	for(Bin2prot; Bin2prot<1024.; Bin2prot++)
                	{ 
				if(  (amp -> GetBinContent(Bin2prot) / ped ) <= 0.9806 ) break;
			}
                	Int_t FitBin1prot=Bin2prot-1.;

                        //amp -> Fit(fun, "W", "S", MaxFitBordL/5+0.0, MaxFitBordR/5+0.0);
                        amp -> Fit(fun, "W", "S", FitBin1prot/5., MaxFitBordR/5.+0.0);
                        ampChi2->Fill(fun->GetChisquare()); 
      
			CanvAmp -> cd(5);

            		//time of MCP
                           
			Float_t Y1=amp->GetMinimum()+(1-K)*(mean-amp->GetMinimum());
			if(verbose) cout<<"Y1="<<Y1<<endl;
                           
                        Float_t P01=fun -> GetParameter(0);
                        Float_t P11=fun -> GetParameter(1);
			Float_t tcr = ((Y1-P01)/P11);
			if(verbose) cout<<"tcr="<<tcr<<endl;
                           
                        //======================
            		for (int i = 0; i < amp -> GetNbinsX(); i++) {
                		ampavg -> Fill(amp -> GetBinCenter(i), amp -> GetBinContent(i));
            		}
			if(verbose) cout<<"i="<<i<<endl;	
            		 
                        //======================
                        //time between MCP and first Trg MCP
            		ht -> Fill(tcr - tcrtrg2);
			TimeVSAmplitude -> Fill(tcr - tcrtrg2, mean-MinimumSignal);
		}
		else
		{
			ht0 -> Fill(0);
		}
         }
    	 printf("%d\r",i); fflush(stdout);
         gSystem -> ProcessEvents();
    }
    
    CanvTimeDistribution -> cd(1);
    ht -> SetAxisRange(-20, 20, "X");
    ht -> Draw("");
    CanvTimeDistribution -> cd(2);
    
    CanvAmp -> cd(2);
    amptrg1 -> Draw("");
    CanvAmp -> cd(3);
    amptrg2 -> Draw("");
    CanvAmp -> cd(4);
    amp -> SetAxisRange(2, 200, "X");
    amp -> Draw("");
    CanvAmp -> cd(5);
    CanvAmp -> cd(6);
    ampavg -> Draw("");
    
    CanvTimeDistribution -> cd(3);
    TimeDistributionTriggers -> Draw("");
    CanvTimeDistribution -> cd(4);
    TimeVSAmplitude -> SetAxisRange(0, 200, "X");
    TimeVSAmplitude -> Draw("");
     
    TFile f1(outDir + namedir + ".root","RECREATE");
    ht->Write();
    TimeDistributionTriggers->Write();
    amptrgChi2->Write();
    ampChi2->Write();
    TimeVSAmplitude->Write();
    amptrg1->Write();
    amptrg2->Write();
    amp->Write();
    ampavg->Write();    
    f1.Close();
}
