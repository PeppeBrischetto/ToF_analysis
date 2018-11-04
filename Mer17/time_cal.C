#include <iostream>
#include <fstream>
#include <cstdlib>
#include <stdlib.h>
#include "TGraphErrors.h"
#include "TGraph.h"

void time_cal() {

   const Int_t n = 3;
   const Int_t m = 5;
   
   Double_t bin[m] = {1706., 1487., 1256., 1257., 1707.};
   Double_t scala[m] = {1., 2., 3., 4., 5.};
   Double_t energy[n] = {2243.796, 5419.961, 5462.863};
   //Double_t err_channel[n] = {0.7, 2., 0.3};
   Double_t err_energy[n] = {0., 0., 0.};
   
   TCanvas *c1 = new TCanvas("c1", "c1");
   c1->DrawFrame(0., 1200., 6., 1800.);
   //c1->SetGrid();
 
   TGraph *run456 = new TGraph( n, scala, bin );
   run456->Draw("*");
   run456->SetMarkerStyle(20);
   run456->SetMarkerColor(kBlue);

   TGraph *run10 = new TGraph( 1, &scala[3], &bin[3] );
   run10->Draw("* same");
   run10->SetMarkerStyle(21);
   run10->SetMarkerColor(kGreen);

   TGraph *run11 = new TGraph( 1, &scala[4], &bin[4] );
   run11->Draw("* same");
   run11->SetMarkerStyle(22);
   run11->SetMarkerColor(kRed);

   /* ********************************************** */

   Double_t channel[m] = {3409.87, 2971.97, 2511.79, 2512.83, 3412.62};
   Double_t scala2[m] = {1., 2., 3., 4., 5.};
   Double_t err_channel[m] = {0.174, 0.0211285, 0.022798, 0.0163169, 0.0139570};
   Double_t err_scala[m] = {0., 0., 0., 0., 0.};

   TCanvas *c2 = new TCanvas("c2", "c2");
   c2->DrawFrame(0., 2400., 6., 3500.);
   //c2->SetGrid();
 
   TGraphErrors *run456_fit = new TGraphErrors( n, scala2, channel, err_scala, err_channel );
   run456_fit->Draw("*");
   run456_fit->SetMarkerStyle(20);
   run456_fit->SetMarkerColor(kBlue);

   TGraphErrors *run10_fit = new TGraphErrors( 1, &scala2[3], &channel[3], &err_scala[3], &err_channel[3] );
   run10_fit->Draw("* same");
   run10_fit->SetMarkerStyle(21);
   run10_fit->SetMarkerColor(kGreen);

   TGraphErrors *run11_fit = new TGraphErrors( 1, &scala2[4], &channel[4], &err_scala[4], &err_channel[4] );
   run11_fit->Draw("* same");
   run11_fit->SetMarkerStyle(22);
   run11_fit->SetMarkerColor(kRed);

   TLegend *legend1 = new TLegend( 0.63, 0.3, 0.9, 0.4 );
   legend1->AddEntry( run456_fit, "Merc dopo black out", "pe" );
   legend1->AddEntry( run10_fit, "Merc dopo 1a sostit foglio", "pe" );
   legend1->AddEntry( run11_fit, "Merc dopo 2a sostit foglio", "pe" );
   legend1->SetLineWidth(0);
   legend1->SetTextSize(0.035);
   legend1->Draw();

   /*********** Adesso facciamo il fit sui dati dei soli run 10 e 11 ******************** */

   Double_t canali[2] = {2512.83, 3412.62};
   Double_t tempo[2] = {20., 30.};
   Double_t err_canali[2] = {0.0163169, 0.0139570};
   Double_t err_tempo[2] = {0.011, 0.00882};

   TGraphErrors *run1011_fit = new TGraphErrors( 2, canali, tempo, err_canali, err_tempo );

   TCanvas *c3 = new TCanvas("c3", "c3");
   //c3->DrawFrame(0., 2400., 6., 3500.);
   //c3->SetGrid();
   
   run1011_fit->SetName("Parametri del fit");
   run1011_fit->SetTitle("Calibrazione in Tempo");
   run1011_fit->Draw("A*");
   run1011_fit->SetMarkerStyle(20);
   run1011_fit->GetYaxis()->SetTitle("T [ns]");
   run1011_fit->GetXaxis()->SetTitle("Canali");
   run1011_fit->GetXaxis()->SetTitleSize(0.05);
   run1011_fit->GetYaxis()->SetTitleSize(0.05);
   run1011_fit->GetXaxis()->SetTitleOffset(0.90);
   run1011_fit->GetYaxis()->SetTitleOffset(1.00);
   run1011_fit->Fit("pol1", "M");

   // Questa TF1 era per provare a disegnare una retta sopra un grafico o un istogramma
   //TF1 *retta = new TF1("retta", "0.560817*x-0.", 2000., 6000.);
   //retta->SetLineColor(kBlue);
   //retta->Draw("same");

   TF1 *retta_cal = run1011_fit->GetFunction("pol1");

   retta_cal->SetParName(0, "q");
   retta_cal->SetParName(1, "m");
   
   // *********  Qui di seguito si trovano le rette di best fit più o meno gli errori ********
   // IMPORTANTE: le ho commentate perché praticamente sono sovrapposte alla retta di best fit principale
   /*
   TF1 *retta1 = new TF1("retta1", "pol1", 2000., 6000.);
   retta1->SetLineColor(kGreen);
   retta1->FixParameter(0, (retta_cal->GetParameter(0)) + (retta_cal->GetParError(0)) );
   retta1->FixParameter(1, (retta_cal->GetParameter(1)) + (retta_cal->GetParError(1)) );
   retta1->Draw("same");

   TF1 *retta2 = new TF1("retta2", "pol1", 2000., 6000.);
   retta2->SetLineColor(kOrange);
   retta2->FixParameter(0, (retta_cal->GetParameter(0)) - (retta_cal->GetParError(0)) );
   retta2->FixParameter(1, (retta_cal->GetParameter(1)) - (retta_cal->GetParError(1)) );
   retta2->Draw("same");
   */

   gStyle->SetOptFit(0111); // con queste opzioni stampo i valori dei parametri e i loro errori

   
   TPaveStats *st = (TPaveStats*)run1011_fit->GetListOfFunctions()->FindObject("stats");
   st->SetX1NDC(0.62);
   st->SetX2NDC(0.9);
   st->SetY1NDC(0.38);
   st->SetY2NDC(0.54);
   st->SetTextSize(0.037);
   
   TLegend *legend = new TLegend( 0.62, 0.26, 0.9, 0.36 );
   legend->AddEntry( run1011_fit, "Punti sperimentali", "pe" );
   legend->AddEntry( retta_cal, "Retta di calibrazione", "l" );
   legend->SetTextSize(0.035);
   legend->Draw();
   

   /*********** Adesso facciamo il fit sui dati di tutti i run ******************** */

   Double_t Canali[5] = {3411., 2973., 2511., 2513., 3413.};
   Double_t Tempo[5] = {30., 25., 20., 20., 30.};
   Double_t Err_canali[5] = {4., 2., 2., 4., 2.};
   Double_t Err_tempo[5] = {0.011, 0.00882, 0., 0., 0.};

   TGraphErrors *Run_fit = new TGraphErrors( 5, Tempo, Canali, Err_tempo, Err_canali );

   TCanvas *c4 = new TCanvas("c4", "c4");
   //c4->DrawFrame(0., 2400., 6., 3500.);
   //c4->SetGrid();
   
   Run_fit->SetName("Parametri del fit");
   Run_fit->SetTitle("Calibrazione in Tempo");
   Run_fit->Draw("A*");
   Run_fit->SetMarkerStyle(20);
   Run_fit->GetXaxis()->SetTitle("T [ns]");
   Run_fit->GetYaxis()->SetTitle("Canali");
   Run_fit->GetXaxis()->SetTitleSize(0.05);
   Run_fit->GetYaxis()->SetTitleSize(0.05);
   Run_fit->GetXaxis()->SetTitleOffset(0.90);
   Run_fit->GetYaxis()->SetTitleOffset(1.00);
   Run_fit->Fit("pol1", "M");

   // Questa TF1 era per provare a disegnare una retta sopra un grafico o un istogramma
   //TF1 *retta = new TF1("retta", "0.560817*x-0.", 2000., 6000.);
   //retta->SetLineColor(kBlue);
   //retta->Draw("same");

   TF1 *Retta_cal = Run_fit->GetFunction("pol1");

   Retta_cal->SetParName(0, "q");
   Retta_cal->SetParName(1, "m");
   
   // *********  Qui di seguito si trovano le rette di best fit più o meno gli errori ********
   // IMPORTANTE: le ho commentate perché praticamente sono sovrapposte alla retta di best fit principale
   /*
   TF1 *retta1 = new TF1("retta1", "pol1", 2000., 6000.);
   retta1->SetLineColor(kGreen);
   retta1->FixParameter(0, (retta_cal->GetParameter(0)) + (retta_cal->GetParError(0)) );
   retta1->FixParameter(1, (retta_cal->GetParameter(1)) + (retta_cal->GetParError(1)) );
   retta1->Draw("same");

   TF1 *retta2 = new TF1("retta2", "pol1", 2000., 6000.);
   retta2->SetLineColor(kOrange);
   retta2->FixParameter(0, (retta_cal->GetParameter(0)) - (retta_cal->GetParError(0)) );
   retta2->FixParameter(1, (retta_cal->GetParameter(1)) - (retta_cal->GetParError(1)) );
   retta2->Draw("same");
   */

   gStyle->SetOptFit(0111); // con queste opzioni stampo i valori dei parametri e i loro errori

   
   TPaveStats *st1 = (TPaveStats*)Run_fit->GetListOfFunctions()->FindObject("stats");
   st1->SetX1NDC(0.62);
   st1->SetX2NDC(0.9);
   st1->SetY1NDC(0.38);
   st1->SetY2NDC(0.54);
   st1->SetTextSize(0.037);
   
   TLegend *legend2 = new TLegend( 0.62, 0.26, 0.9, 0.36 );
   legend2->AddEntry( Run_fit, "Punti sperimentali", "pe" );
   legend2->AddEntry( Retta_cal, "Retta di calibrazione", "l" );
   legend2->SetTextSize(0.035);
   legend2->Draw();
}
