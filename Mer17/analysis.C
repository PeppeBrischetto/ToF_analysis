#include <iostream>
#include <fstream>
#include <cstdlib>
#include "TH1D.h"

void analysis () {

   TFile *cut = new TFile("cuts_file.root");
   
   /*  ************   Ricordati di mettere il taglio giusto per il run giusto ****************  */

   //TCutG *rumore  = (TCutG*)cut->Get("rumore");
   //TCutG *rumore  = (TCutG*)cut->Get("rumore_run4NEW");
   //TCutG *rumore  = (TCutG*)cut->Get("rumore_run5NEW");
   //TCutG *rumore  = (TCutG*)cut->Get("rumore_run6NEW");
   //TCutG *rumore  = (TCutG*)cut->Get("rumore_run7");
   //TCutG *rumore  = (TCutG*)cut->Get("rumore_run10");
   TCutG *rumore  = (TCutG*)cut->Get("rumore_run11");

   //const char* fileName = "TOF_run4NEW.asc";
   //const char* fileName = "TOF_run5NEW.asc";
   //const char* fileName = "TOF_run6NEW.asc";
   //const char* fileName = "TOF_run7.asc";   // questo run ha una risoluzione del tof piuttosto brutta
   //const char* fileName = "TOF_run8.asc"; // questo run sembra fare schifo (nella calibraz tof ci sono tre picchi)
   //const char* fileName = "TOF_run10.asc";
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
	 if ( rumore->IsInside(time, energy) ) 
            h_energy_time->Fill(time, energy);

   }

   gStyle->SetOptFit(1111);  // Questa opzione stampa (in ordine) prob, chi-quadro, i valori dei parametri e i loro errori

   h_energy->GetXaxis()->SetTitle("Channel");
   h_energy->GetYaxis()->SetTitle("Count");
   //h_energy->Draw();      // ************** Ormai ho capito che questo istogramma è inutile

   TCanvas *c2 = new TCanvas("c2", "c2");
   h_energy_time->Draw("colz");

   TH1D *h_ener = h_energy_time->ProjectionX();
   TCanvas *c3 = new TCanvas("c3", "c3");
   h_ener->SetTitle("Spettro energetico");
   h_ener->GetYaxis()->SetTitle("Conteggi");
   h_ener->GetYaxis()->SetTitleOffset(1.15);
   h_ener->SetLineWidth(1);
   h_ener->Draw();

   TH1D *h_time = h_energy_time->ProjectionY();
   TCanvas *c4 = new TCanvas("c4", "c4");
   h_time->SetTitle("ToF");
   h_time->GetYaxis()->SetTitle("Conteggi");
   h_time->GetYaxis()->SetTitleOffset(1.15);
   h_time->Draw();
}
