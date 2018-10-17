#include <iostream>
#include <fstream>
#include <cstdlib>
#include <stdlib.h>
#include "TTree.h"

void create_tree() {

   //const char* fileName = "TOF2_run1_time.asc";
   //const char* fileName = "TOF2_run2_pul.asc";
   //const char* fileName = "TOF2_run3_puls.asc";
   const char* fileName = "TOF2_tot.asc";

   ifstream inputFile;
   inputFile.open(fileName);
   if ( inputFile.fail() ) {
      //std::cout << "File " << fileName << " does not exist." << std::endl;
      exit(1);
   }
   else std::cout << "File \"" << fileName << "\" correctly opened." << std::endl;

   const char* rootFileName = "TOF2_tot.root";

   TFile *f = new TFile( rootFileName, "RECREATE" );

   TTree *tree =new TTree ( "tree", "Energy and ToF from ASCII" );
  
   Double_t energy, tof;

   tree->Branch("Energy", &energy, "E/D");
   tree->Branch("ToF", &tof, "ToF/D");

   while ( !inputFile.eof() ) {
         inputFile >> tof;
         inputFile >> energy;
         
         tree->Fill();
   }

   inputFile.close();
   
   f->Write(); 
}
