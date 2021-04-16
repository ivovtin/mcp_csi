void eff_U()
{
    auto c1 = new TCanvas("c1","c1",800,500);
    TGraph *g = new TGraph("res_eff_u_csi.dat", "%lg %lg");
    g->SetTitle(" ");
    g->GetXaxis()->SetTitle("U_{gap}, V");
    g->GetYaxis()->SetTitle("Efficiency");
    g->SetLineColor(kBlue);
    g->SetMarkerStyle(21);
    g->SetMarkerColor(kBlue);
    g->GetYaxis()->SetRangeUser(0.02, 0.42);
    g->Draw("ALP");

    TGraph *gmcp = new TGraph("res_eff_u_mcp.dat", "%lg %lg");
    gmcp->SetTitle(" ");
    gmcp->SetLineColor(kMagenta);
    gmcp->SetMarkerStyle(21);
    gmcp->SetMarkerColor(kMagenta);
    gmcp->Draw("same");

   auto legend = new TLegend(0.1,0.80,0.40,0.90);
   legend->AddEntry(g,"From Photocathode","l");
   legend->AddEntry(gmcp,"From MCP","l");
   legend->Draw("same");

   c1->Print("eff_u_res.pdf");
   c1->Print("eff_u_res.eps");
}
