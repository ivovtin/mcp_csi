void DrawResult() 
{
   gStyle->SetOptFit(1111);

   TString indir = "/home/ovtin/development/mcp_csi/"; 

   TString file1 = "2021-04-08_01-45-33";    //Umcp=2200V; Upc=2100V;
   TString file2 = "2021-04-08_09-00-26";  //Umcp=2600V; Upc=3200V;
   //TString file2 = "2021-04-08_08-42-18";  //Umcp=2600V; Upc=2900V;
   //TString file2 = "2021-04-08_03-59-57";    //Umcp=2400V; Upc=3000V;
  
   TString file1TitleCan = "U_{mcp}=2200V U_{phc}=2100V";
   TString file2TitleCan = "U_{mcp}=2600V  U_{phc}=3200V";
   //TString file2TitleCan = "U_{mcp}=2600V  U_{phc}=2900V";
   //TString file2TitleCan = "U_{mcp}=2400V  U_{phc}=3000V";

   TString indir1 = indir + file1 + ".root";      
   TString indir2 = indir + file2 + ".root"; 

   TString outfile1 = "out_pic_" + file1 + "_" + file2 + ".pdf";   
   TString outfile2 = "out_pic_" + file1 + "_" + file2 + ".eps";   

   double xmin = 10.2;
   double xmax = 11.5;

   TCanvas* CanvTimeDistribution = new TCanvas("CanvTimeDistribution", "Time distribution", 500, 600, 600, 600);
   CanvTimeDistribution->Divide(2, 2);
   TFile* f1 = TFile::Open(indir1);
   TH1F * htCLOSE = (TH1F*)f1->Get("ht");
   CanvTimeDistribution->cd(1);
   htCLOSE->GetXaxis()->SetRangeUser(8, 13);
   htCLOSE->GetXaxis()->SetTitle("Time resolution, ns");
   htCLOSE->SetTitle(file1TitleCan);
   htCLOSE->Draw();
   TAxis *axis = htCLOSE->GetXaxis();
   int bmin1 = axis->FindBin(xmin); 
   int bmax1 = axis->FindBin(xmax); 
   double integral1 = htCLOSE->Integral(bmin1,bmax1);
   cout<<"For close photocatode case - Nevents="<<integral1<<endl;

   CanvTimeDistribution->cd(2);
   TFile* f2 = TFile::Open(indir2);
   TH1F * htOPEN = (TH1F*)f2->Get("ht");
   htOPEN->SetLineColor(kMagenta);
   htOPEN->GetXaxis()->SetRangeUser(8, 13);
   htOPEN->GetXaxis()->SetTitle("Time resolution, ns");
   htOPEN->SetTitle(file2TitleCan);
   htOPEN->Draw();
   htOPEN->Fit("gaus","","",10.41,10.58);
   //htOPEN->Fit("gaus","","",10.55,10.75);
   //htOPEN->Fit("gaus","","",10.37,10.63);
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
   htCLOSEnorm->GetYaxis()->SetRangeUser(0., 0.047);
   htCLOSEnorm->GetXaxis()->SetRangeUser(8., 13.);
   htCLOSEnorm->GetXaxis()->SetTitle("Time resolution, ns");
   htCLOSEnorm->SetTitle(" ");
   htCLOSEnorm->Draw("hist");


   TH1F* htOPENnorm = (TH1F*) htOPEN->Clone();
   Double_t b=htOPENnorm->GetEntries();
   htOPENnorm->Scale(1/b);
   htOPENnorm->GetYaxis()->SetRangeUser(0., 0.047);
   htOPENnorm->GetXaxis()->SetRangeUser(3., 13.);
   htOPENnorm->SetTitle(" ");
   htOPENnorm->Draw("hist, same");

   TFile* f3 = TFile::Open(indir2);
   TH1F * hmcptrig = (TH1F*)f3->Get("TimeDistributionTriggers");
   CanvTimeDistribution->cd(4);
   hmcptrig->GetXaxis()->SetRangeUser(-1.1, -0.6);
   hmcptrig->GetXaxis()->SetTitle("Time resolution, ns");
   hmcptrig->SetTitle("Time distribution between triggers MCP");
   hmcptrig->Draw();
   Double_t NevMCPtrig = hmcptrig->GetEntries();
   cout<<"NevMCPtrig="<<NevMCPtrig<<endl;

   cout<<"Efficiency="<<integral2/NevMCPtrig<<"\t"<<"for MCP Efficiency="<<integral3/NevMCPtrig<<endl;

   CanvTimeDistribution->Print(outfile1);
   CanvTimeDistribution->Print(outfile2);
}
