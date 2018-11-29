#include <iostream>
#include <fstream>
#include <cstdlib>
#include <stdlib.h>
#include "TGraphErrors.h"

void calibration() {

   const Int_t n = 3;
   
   /*  ********  Questi erano i valori PRIMA di aver parlato con Politi *********  */
   /*
   Double_t channel[n] = {1201.6, 2989., 3009.8}; // va bene per il chi-quadro
   //Double_t channel[n] = {1200.1, 2983., 3008.5}; // va bene per il log likelihood
   //Double_t channel[n] = {1201.7, 2991., 3010.3}; // valori per 4096 bin -> meglio 2048 bin
   Double_t energy[n] = {2243.796, 5419.961, 5462.863};
   Double_t err_channel[n] = {0.7, 2., 0.3};
   Double_t err_energy[n] = {0., 0., 0.};
   */

   /*  ********  Questi sono i valori DOPO di aver parlato con Politi *********  */
   const Int_t m = 2;
   //Double_t channel[m] = {1203., 3009.};  // Stimato con il crosshair al centro del bin
   //Double_t energy[m] = {2243.796, 5462.863}; // valori senza la media pesata sulle energie delle alpha 
   //Double_t err_channel[m] = {76., 28.};  // valori senza la media pesata sulle energie delle alpha
   Double_t channel[m] = {1203., 3012.};  // Stimato con il fit sul lato destro del picco
   Double_t energy[m] = {2243.796, 5462.863}; // valori senza la media pesata sulle energie delle alpha 
   Double_t err_channel[m] = {76., 28.};  // valori senza la media pesata sulle energie delle alpha
   Double_t err_energy[m] = {0., 0.};

   TCanvas *c1 = new TCanvas("c1", "c1");
   //c1->SetGrid();
 
   TGraphErrors *ch_ener = new TGraphErrors( m, energy, channel, err_energy, err_channel );
   ch_ener->SetName("Parametri del fit");
   ch_ener->SetTitle("Calibrazione in Energia");
   ch_ener->Draw("A*");
   ch_ener->SetMarkerStyle(20);
   ch_ener->GetXaxis()->SetTitle("E [keV]");
   ch_ener->GetYaxis()->SetTitle("Canali");
   ch_ener->GetXaxis()->SetTitleSize(0.05);
   ch_ener->GetYaxis()->SetTitleSize(0.05);
   ch_ener->GetXaxis()->SetTitleOffset(0.90);
   ch_ener->GetYaxis()->SetTitleOffset(1.00);
   ch_ener->Fit("pol1", "M");

   // Questa TF1 era per provare a disegnare una retta sopra un grafico o un istogramma
   //TF1 *retta = new TF1("retta", "0.560817*x-0.", 2000., 6000.);
   //retta->SetLineColor(kBlue);
   //retta->Draw("same");

   TF1 *retta_cal = ch_ener->GetFunction("pol1");

   retta_cal->SetParName(0, "b");
   retta_cal->SetParName(1, "a");
   
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

   //retta_cal->FixParameter(0, 0.);
   ch_ener->Fit(retta_cal, "M");

   gStyle->SetOptFit(0111); // con queste opzioni stampo i valori dei parametri e i loro errori

   TPaveStats *st = (TPaveStats*)ch_ener->GetListOfFunctions()->FindObject("stats");
   st->SetX1NDC(0.63);
   st->SetX2NDC(0.9);
   st->SetY1NDC(0.42);
   st->SetY2NDC(0.58);
   st->SetTextSize(0.037);

   TLegend *legend = new TLegend( 0.63, 0.3, 0.9, 0.4 );
   legend->AddEntry( retta_cal, "Retta di calibrazione", "l" );
   legend->SetTextSize(0.035);
   legend->Draw();
}
