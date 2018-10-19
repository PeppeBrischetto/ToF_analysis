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
  
}
