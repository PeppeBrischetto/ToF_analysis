#include <iostream>
#include <fstream>
#include <cstdlib>
#include "TH1D.h"

void analysis () {

   const char* fileName = "TOF_run11.asc";
   ifstream inputFile;
   inputFile.open(fileName);
   if ( inputFile.fail() ) {
      //std::cout << "File " << fileName << " does not exist." << std::endl;
      exit(1);
   }
   else std::cout << "File \"" << fileName << "\" correctly opened." << std::endl;

   std::cout << "Cominciamo questa analisi" << std::endl;

   Double_t energy, time;

   TH1D *h_energy = new TH1D("h_energy", "Energy", 2087., 0., 4090.);
   TH2D *h_energy_time = new TH2D("h_energy_time", "Energy vs Time", 2048., 0., 4096., 2048., 0., 4096.);

   while ( !inputFile.eof() ) {
         inputFile >> energy;
         h_energy->Fill(energy);

   }

   inputFile.clear();
   inputFile.seekg(0);

   while ( !inputFile.eof() ) {
         inputFile >> energy;
         inputFile >> time;
         //h_energy_time->Fill(energy, time);
	 //if ( prova_rumore->IsInside(time, energy) ) 
            h_energy_time->Fill(time, energy);

   }


   h_energy->GetXaxis()->SetTitle("Channel");
   h_energy->GetYaxis()->SetTitle("Count");
   h_energy->Draw();

   TCanvas *c2 = new TCanvas("c2", "c2");
   h_energy_time->Draw("colz");

   TH1D *h_ener = h_energy_time->ProjectionX();
   TCanvas *c3 = new TCanvas("c3", "c3");
   h_ener->SetTitle("Energy");
   h_ener->Draw();

   TH1D *h_time = h_energy_time->ProjectionY();
   TCanvas *c4 = new TCanvas("c4", "c4");
   h_time->SetTitle("ToF");
   h_time->Draw();
}
