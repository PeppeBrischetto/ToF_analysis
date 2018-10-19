#include <iostream>
#include <fstream>
#include <cstdlib>
#include <stdlib.h>
#include "TGraphErrors.h"

void calibration() {

   const Int_t n = 3;
   
   Double_t channel[n] = {1199., 2989.2, 3009.8};
   Double_t energy[n] = {2233.8, 5419.961, 5462.863};
   Double_t err_channel[n] = {0.3, 1.6, 0.3};
   Double_t err_energy[n] = {0., 0., 0.};
   
   TGraphErrors *ch_ener = new TGraphErrors( n, energy, channel, err_energy, err_channel );
   ch_ener->SetTitle("Calibration Plot"); 
   ch_ener->Draw("A*");
   ch_ener->Fit("pol1");

   // Questa TF1 era per provare a disegnare una retta sopra un grafico o un istogramma
   //TF1 *retta = new TF1("retta", "0.560817*x-0.", 2000., 6000.);
   //retta->SetLineColor(kBlue);
   //retta->Draw("same");

   TF1 *retta_cal = (TF1*)ch_ener->FindObject("pol1");

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

   gStyle->SetOptFit(0011);

   TLegend *legend = new TLegend( 0.6, 0.65, 0.35, 0.75 );
   legend->AddEntry( retta_cal, "Retta di calibrazione", "l" );
   legend->Draw();
}
