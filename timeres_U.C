void timeres_U()
{
    auto c1 = new TCanvas("c1","c1",800,500);
    TGraphErrors *g = new TGraphErrors("res_timeres_u.dat", "%lg %lg %lg %lg");
    g->SetTitle(" ");
    g->GetXaxis()->SetTitle("U_{gap}, V");
    g->GetYaxis()->SetTitle("Time resolution, ns");
    g->SetLineColor(kBlue);
    g->SetMarkerStyle(21);
    g->SetMarkerColor(kBlue);
    g->GetYaxis()->SetRangeUser(0.058, 0.080);
    g->Draw("ALP");
  
   c1->Print("time_u_res.pdf");
   c1->Print("time_u_res.eps");
}
