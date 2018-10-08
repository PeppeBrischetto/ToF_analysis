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

   Double_t energy;

   TH1D *h_energy = new TH1D("h_energy", "Energy", 1024., 0., 4096.);

   while ( !inputFile.eof() ) {
         inputFile >> energy;
         h_energy->Fill(energy);
   }
   
   h_energy->GetXaxis()->SetTitle("Channel");
   h_energy->GetYaxis()->SetTitle("Count");
   h_energy->Draw();
}