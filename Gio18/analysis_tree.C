#include <iostream>
#include <fstream>
#include <cstdlib>
#include <stdlib.h>
#include "TTree.h"
#include "TH1D.h"
#include "TH2D.h"

void analysis_tree() {

   const char* fileName = "TOF2_tot.root";
   const char* treeName = "tree";

   TFile *f = new TFile ( fileName );

   if (!f) {
     // std::cout << "File " << fileName << " does not exist." << std::endl;
     return;
   }
   else std::cout << "File \"" << fileName << "\" correctly opened." << std::endl;

   TTree *t = (TTree *)f->Get( treeName );

   Double_t energy, tof;

   t->SetBranchAddress("Energy", &energy);
   t->SetBranchAddress("ToF", &tof);

   Long64_t nEntries = t->GetEntries();
   std::cout << "Il numero di entries e' " << nEntries << std::endl;

   TH2D *h_energy_tof = new TH2D("h_energy_tof", "Energy vs ToF", 2048., 0., 4096., 2048., 0., 4096.);

   for (Long64_t i=0; i<nEntries; i++) {
    
       t->GetEntry(i);

       h_energy_tof->Fill( tof, energy );
       
   }

   t->Draw( "E:ToF", "E>500 && E<3500 && ToF>500 && ToF<3500", "" );

   TCanvas *c2 = new TCanvas( "c2", "c2" );
   h_energy_tof->Draw("colz");
}
