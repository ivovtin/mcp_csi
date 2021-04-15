void DrawResult() 
{
   TString indir = "/home/petruhin/development/test_beam_07_04_2021/"; 

   TString indir1 = indir + "2021-04-08_01-45-33/2021-04-08_01-45-33_N=74125Ped=0.5_Data.root";
   //TString indir2 = indir + "2021-04-08_02-19-18/2021-04-08_02-19-18_N=78695Ped=0.5_Data.root"; 	
   TString indir2 = indir + "2021-04-08_08-42-18/2021-04-08_08-42-18_N=125818Ped=0.5_Data.root"; 	
   double xmin = 10.2;
   double xmax = 11.5;

   TCanvas* CanvTimeDistribution = new TCanvas("CanvTimeDistribution", "Time distribution", 500, 600, 600, 600);
   CanvTimeDistribution->Divide(2, 2);
   TFile* f1 = TFile::Open(indir1);
   TH1F * htCLOSE = (TH1F*)f1->Get("ht");
   CanvTimeDistribution->cd(1);
   htCLOSE->GetXaxis()->SetRangeUser(8, 13);
   htCLOSE->GetXaxis()->SetTitle("Time resolution, ns");
   htCLOSE->Draw();
   TAxis *axis = htCLOSE->GetXaxis();
   int bmin1 = axis->FindBin(xmin); 
   int bmax1 = axis->FindBin(xmax); 
   double integral1 = htCLOSE->Integral(bmin1,bmax1);
   cout<<"For close photocatode case - Nevents="<<integral1<<endl;

   CanvTimeDistribution->cd(2);
   TFile* f2 = TFile::Open(indir2);
   TH1F * htOPEN = (TH1F*)f2->Get("ht");
   htOPEN->SetLineColor(kRed);
   htOPEN->GetXaxis()->SetRangeUser(8, 13);
   htOPEN->GetXaxis()->SetTitle("Time resolution, ns");
   htOPEN->Draw();
   TAxis *axis2 = htOPEN->GetXaxis();
   int bmin2 = axis->FindBin(xmin); 
   int bmax2 = axis->FindBin(xmax); 
   double integral2 = htOPEN->Integral(bmin2,bmax2);
   cout<<"For open photocatode case - Nevents="<<integral2<<endl;
   TAxis *axis3 = htOPEN->GetXaxis();
   xmin = 8.5;
   xmax = 10.2;
   int bmin3 = axis->FindBin(xmin); 
   int bmax3 = axis->FindBin(xmax); 
   double integral3 = htOPEN->Integral(bmin3,bmax3);
   cout<<"From MCP, for open photocatode case - Nevents="<<integral3<<endl;

   CanvTimeDistribution->cd(3);
   TH1F* htCLOSEnorm = (TH1F*) htCLOSE->Clone();
   Double_t a=htCLOSEnorm->GetEntries();
   htCLOSEnorm->Scale(1/a); 
   htCLOSEnorm->GetYaxis()->SetRangeUser(0., 0.035);
   htCLOSEnorm->GetXaxis()->SetRangeUser(8., 13.);

   htCLOSEnorm->GetXaxis()->SetTitle("Time resolution, ns");
   htCLOSEnorm->Draw("hist");


   TH1F* htOPENnorm = (TH1F*) htOPEN->Clone();
   Double_t b=htOPENnorm->GetEntries();
   htOPENnorm->Scale(1/b);
   htOPENnorm->GetYaxis()->SetRangeUser(0., 0.035);
   htOPENnorm->GetXaxis()->SetRangeUser(3., 13.);
   htOPENnorm->Draw("hist, same");

   TFile* f3 = TFile::Open(indir1);
   TH1F * hmcptrig = (TH1F*)f3->Get("TimeDistributionTriggers");
   CanvTimeDistribution->cd(4);
   hmcptrig->GetXaxis()->SetRangeUser(-1.1, -0.6);
   hmcptrig->GetXaxis()->SetTitle("Time resolution, ns");
   hmcptrig->Draw();
   Double_t NevMCPtrig = hmcptrig->GetEntries();
   cout<<"NevMCPtrig="<<NevMCPtrig<<endl;

   cout<<"Efficiency="<<integral2/NevMCPtrig<<"\t"<<"for MCP Efficiency="<<integral3/NevMCPtrig<<endl;

   CanvTimeDistribution->Print("out_pic.pdf");
   CanvTimeDistribution->Print("out_pic.eps");
}
