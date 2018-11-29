#include <iostream>
#include <fstream>
#include <cstdlib>
#include <stdlib.h>
#include "TH1D.h"
#include "TH2D.h"
#include "TPaveStats.h"
#include "TStyle.h"

void analysis () {

   TFile *cut = new TFile("cuts_file.root");
   
   //TCutG *prova  = (TCutG*)cut->Get("prova");
   //TCutG *rumore  = (TCutG*)cut->Get("rumore");  // Questo taglio dice Politi che non conviene
   TCutG *picco_alta_energia  = (TCutG*)cut->Get("picco_alta_energia");
   TCutG *picco_bassa_energia  = (TCutG*)cut->Get("picco_bassa_energia");
   TCutG *rumore_new  = (TCutG*)cut->Get("rumore_new"); // Questo è il taglio definito dopo aver parlato con Politi
   TCutG *rumore_cal  = (TCutG*)cut->Get("rumore_cal"); // Questo è il taglio sullo spettro in energia calibrato
   TCutG *pulser_picco_alto  = (TCutG*)cut->Get("pulser_picco_alto1"); // Questo taglio è inutile

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

   std::cout << "Cominciamo questa analisi" << std::endl;

   Double_t energy, time;

   TH1D *h_energy = new TH1D("h_energy", "Energy", 2087., 0., 4090.);
   TH2D *h_energy_time = new TH2D("h_energy_time", "Energy vs Time", 2048., 0., 4096., 2048., 0., 4096.);

   //TH2D *h_energy_time_cal = new TH2D("h_energy_time_cal", "Energy vs Time", 2048., 0., 4096., 2038., 99.5, 7400.); // questi sono per i valori stimati con il crosshair
   TH2D *h_energy_time_cal = new TH2D("h_energy_time_cal", "Energy vs Time", 2048., 0., 4096., 2022., 100., 7396.); // questi sono per i valori stimati con il fit sul lato destro dei picchi -> NON FUNZIONA BENE
   //TH2D *h_energy_time_cal = new TH2D("h_energy_time_cal", "Energy vs Time", 2048., 0., 4096., 2082., 99.5, 7400.); // questi per Simone

   TH2D *h_energy_time_cal2 = new TH2D("h_energy_time_cal2", "Energy vs Time", 2048., 0., 4096., 2046., 0., 7440.);

   while ( !inputFile.eof() ) {
         inputFile >> energy;
         h_energy->Fill(energy);

   }

   inputFile.clear();
   inputFile.seekg(0);

   while ( !inputFile.eof() ) {
         inputFile >> time;
         inputFile >> energy;
         //h_energy_time->Fill(time, energy);
	 if ( rumore_new->IsInside(time, energy) )
	 //if ( rumore_new->IsInside(time, energy) && picco_alta_energia->IsInside(time, energy) ) 
            h_energy_time->Fill(time, energy);

	 if ( rumore_new->IsInside(time, energy) )
            //h_energy_time_cal->Fill(time, (energy + 55.8416)/0.561032 ); // valori stimati con il crosshair sul picco con più conteggi
            h_energy_time_cal->Fill(time, (energy + 56.5387)/0.561343 ); // valori stimati con il fit sul lato destro dei picchi -> NON FUNZIONA BENE
            //h_energy_time_cal->Fill(time, energy*1.780 + 106 );  // valori trovati da Simone

	 if ( rumore_new->IsInside(time, energy) )
            h_energy_time_cal2->Fill(time, energy/0.550482 );

   }
   gStyle->SetOptFit(1111);  // Questa opzione stampa (in ordine) prob, chi-quadro, i valori dei parametri e i loro errori
   gStyle->SetOptStat("n");
   gStyle->SetStatY(0.9);
   gStyle->SetStatX(0.9);
   gStyle->SetStatW(0.05);
   gStyle->SetStatH(0.05);
 
   h_energy->GetXaxis()->SetTitle("Channel");
   h_energy->GetYaxis()->SetTitle("Count");
   //h_energy->Draw();      // ************** Ormai ho capito che questo istogramma è inutile

   TCanvas *c2 = new TCanvas("c2", "c2");
   h_energy_time->GetXaxis()->SetTitle("T [ch]");
   h_energy_time->GetXaxis()->SetTitleSize(0.05);
   h_energy_time->GetXaxis()->SetTitleOffset(0.92);
   h_energy_time->GetYaxis()->SetTitle("E [ch]");
   h_energy_time->GetYaxis()->SetTitleSize(0.05);
   h_energy_time->GetYaxis()->SetTitleOffset(0.93);
   h_energy_time->Draw("colz");

   TH1D *h_ener = h_energy_time->ProjectionY();

   TCanvas *c3 = new TCanvas("c3", "c3");
   //c3->SetGrid();
   h_ener->SetTitle("Spettro energetico");
   h_ener->GetXaxis()->SetTitleSize(0.05);
   h_ener->GetXaxis()->SetTitleOffset(0.85);
   h_ener->GetYaxis()->SetTitle("Conteggi");
   h_ener->GetYaxis()->SetTitleSize(0.05);
   h_ener->GetYaxis()->SetTitleOffset(0.85);
   h_ener->SetLineWidth(1);
   //h_ener->SetLineColor(kAzure - 8);
   //h_ener->SetFillColor(kAzure - 8);
   //h_ener->SetFillStyle(3002);
   h_ener->Draw();
 
   TH1D *h_ener_clone1 = (TH1D*)( h_ener->Clone() );
   TH1D *h_ener_clone2 = (TH1D*)( h_ener->Clone() );
   
   h_ener_clone1->SetName("Fit picco alta E");
   h_ener_clone2->SetName("Parametri funzione a due gaussiane");

   /*  *****  Questa parte serve per fittare il picco con una funzione a due gaussiane  ******   */
   
   TF1 *gaus1 = new TF1("gaus1", "gaus", 2900., 2999.);
   gaus1->SetParLimits(1, 2960., 2994.);
   TF1 *gaus2 = new TF1("gaus2", "gaus", 3006., 3040.);

   TF1 *total = new TF1("total", "gaus(0) + gaus(3)", 2900., 3060.);
   total->SetParName(0, "Const1");
   total->SetParName(1, "#mu1");
   total->SetParName(2, "#sigma1");
   total->SetParName(3, "Const2");
   total->SetParName(4, "#mu2");
   total->SetParName(5, "#sigma2");

   gaus1->SetLineColor(kBlue + 2);
   gaus2->SetLineColor(kOrange);
   Double_t par[6];

   h_ener->Fit(gaus1,"RM");
   h_ener_clone1->Fit(gaus2,"RM","sames");

   gaus1->GetParameters(&par[0]);
   gaus2->GetParameters(&par[3]);
 
   total->SetParameters(par);

   h_ener_clone2->Fit(total,"RM","sames");

   TF1 *gaus_draw1 = new TF1("gaus_draw1", "gaus", 2900., 3100.);
   gaus_draw1->FixParameter( 0, total->GetParameter(0) );
   gaus_draw1->FixParameter( 1, total->GetParameter(1) );
   gaus_draw1->FixParameter( 2, total->GetParameter(2) );
   gaus_draw1->SetLineColor(kOrange + 1);
   gaus_draw1->SetLineStyle(2);
   gaus_draw1->Draw("same");
  
   TF1 *gaus_draw2 = new TF1("gaus_draw2", "gaus", 2900., 3100.);
   gaus_draw2->FixParameter( 0, total->GetParameter(3) );
   gaus_draw2->FixParameter( 1, total->GetParameter(4) );
   gaus_draw2->FixParameter( 2, total->GetParameter(5) );
   gaus_draw2->SetLineColor(kGreen-3);
   gaus_draw2->SetLineStyle(9);
   gaus_draw2->Draw("same");

   gPad->Update();


   //TPaveStats *stat = (TPaveStats*) h_ener->GetListOfFunctions()->FindObject("stats");
   //TPaveStats *stat1 = (TPaveStats*) h_ener_clone1->GetListOfFunctions()->FindObject("stats");
   TPaveStats *stat2 = (TPaveStats*) h_ener_clone2->GetListOfFunctions()->FindObject("stats");
   //if ( stat && stat1 && stat2 ) {
   if ( stat2 ) {
   //stat->SetTextColor(kBlue + 2);
   //stat1->SetTextColor(kGreen);
   stat2->SetTextColor(kBlack);
   stat2->SetX1NDC(0.62);
   stat2->SetX2NDC(0.9);
   stat2->SetY1NDC(0.58);
   stat2->SetY2NDC(0.9);
   //Float_t height1 = stat1->GetY2NDC() - stat1->GetY1NDC();
   //stat1->SetY1NDC(stat->GetY1NDC() - height1);
   //stat1->SetY2NDC(stat->GetY1NDC() );
   //stat1->Draw();
   }

   TLegend *legend = new TLegend( 0.615, 0.405, 0.85, 0.555 );
   legend->AddEntry( h_ener, "Dati sperimentali", "fl" );
   //legend->AddEntry( gaus_draw1, "Gaussiana picco a 5420 keV", "l" );
   //legend->AddEntry( gaus_draw2, "Gaussiana picco a 5463 keV", "l" );
   //legend->AddEntry( total, "Fit con funzione a due gaussiane", "l" );
   legend->SetTextSize(0.034);
   legend->SetLineWidth(1.);
   legend->Draw();  

   TH1D *h_time = h_energy_time->ProjectionX();
   TCanvas *c4 = new TCanvas("c4", "c4");
   h_time->SetTitle("ToF");
   h_time->GetYaxis()->SetTitle("Conteggi");
   h_time->GetXaxis()->SetTitleSize(0.05);
   h_time->GetXaxis()->SetTitleOffset(0.85);
   h_time->GetYaxis()->SetTitle("Conteggi");
   h_time->GetYaxis()->SetTitleSize(0.05);
   h_time->GetYaxis()->SetTitleOffset(0.85);
   h_time->Draw();

   TLegend *legend1 = new TLegend( 0.615, 0.405, 0.85, 0.555 );
   legend1->AddEntry( h_ener, "Dati sperimentali", "fl" );
   legend1->SetTextSize(0.034);
   legend1->SetLineWidth(1.);
   legend1->Draw(); 

   /* ***** Qui comincia la parte con la calibrazione in energia con b!=0 *******  */
   
   TCanvas *c5 = new TCanvas("c5", "c5");
   h_energy_time_cal->GetXaxis()->SetTitle("T [ch]");
   h_energy_time_cal->GetXaxis()->SetTitleSize(0.05);
   h_energy_time_cal->GetXaxis()->SetTitleOffset(0.92);
   h_energy_time_cal->GetYaxis()->SetTitle("E [keV]");
   h_energy_time_cal->GetYaxis()->SetTitleSize(0.05);
   h_energy_time_cal->GetYaxis()->SetTitleOffset(0.93);
   h_energy_time_cal->Draw("colz");
  
   TH1D *h_ener_cal = h_energy_time_cal->ProjectionY();

   TCanvas *c6 = new TCanvas("c6", "c6");
   //c3->SetGrid();
   h_ener_cal->SetTitle("Spettro energetico");
   h_ener_cal->GetYaxis()->SetTitle("Conteggi");
   h_ener_cal->GetYaxis()->SetTitleOffset(1.15);
   h_ener_cal->SetLineWidth(1);
   //h_ener_cal->SetLineColor(kAzure - 8);
   //h_ener_cal->SetFillColor(kAzure - 8);
   //h_ener_cal->SetFillStyle(3002);
   h_ener_cal->Draw();

   TH1D *h_ener_cal_clone1 = (TH1D*)( h_ener_cal->Clone() );
   TH1D *h_ener_cal_clone2 = (TH1D*)( h_ener_cal->Clone() );
   
   h_ener_cal_clone1->SetName("Fit picco alta E");
   h_ener_cal_clone2->SetName("Parametri funzione a due gaussiane");

   TF1 *gaus5 = new TF1("gaus5", "gaus", 5320., 5440.); // valori per la calibrazione stimata con il crosshair
   gaus5->SetParLimits(1, 5410., 5425.); // valori per la calibrazione stimata con il crosshair
   TF1 *gaus6 = new TF1("gaus6", "gaus", 5444., 5600.); // valori per la calibrazione stimata con il crosshair

   TF1 *total2 = new TF1("total2", "gaus(0) + gaus(3)", 5300., 5600.);
   total2->SetParName(0, "Const1");
   total2->SetParName(1, "#mu1");
   total2->SetParName(2, "#sigma1");
   total2->SetParName(3, "Const2");
   total2->SetParName(4, "#mu2");
   total2->SetParName(5, "#sigma2");

   gaus5->SetLineColor(kBlue + 2);
   gaus6->SetLineColor(kOrange);
   Double_t par3[6];

   //h_ener_cal->Fit(gaus5,"RM");
   //h_ener_cal_clone1->Fit(gaus6,"RM","sames");
   h_ener_cal->Fit(gaus6,"RM");

   Double_t sigma1 = gaus6->GetParameter(2);
   //std::cout << "La sigma di gaus4 e' " << sigma1 << std::endl;

   gaus5->SetParLimits(2, sigma1 - 0.0001*sigma1, sigma1 + 0.0001*sigma1);
   //gaus5->FixParameter(2, sigma1);
   h_ener_cal_clone1->Fit(gaus5,"RM","sames");

   gaus5->GetParameters(&par3[0]);
   gaus6->GetParameters(&par3[3]);
 
   total2->SetParameters(par3);
   //total2->SetParLimits(1, 5418., 5422.);
   //total2->SetParLimits(2, sigma1 - 0.1*sigma1, sigma1 - 0.004*sigma1);
   total2->SetParLimits(2, sigma1 - 0.0001*sigma1, sigma1 + 0.0001*sigma1);
   total2->SetParLimits(5, sigma1 - 0.0001*sigma1, sigma1 + 0.0001*sigma1);

   h_ener_cal_clone2->Fit(total2,"RM","sames");

   TF1 *gaus_draw5 = new TF1("gaus_draw5", "gaus", 5350., 5500.);
   gaus_draw5->FixParameter( 0, total2->GetParameter(0) );
   gaus_draw5->FixParameter( 1, total2->GetParameter(1) );
   gaus_draw5->FixParameter( 2, total2->GetParameter(2) );
   gaus_draw5->SetLineColor(kOrange + 1);
   gaus_draw5->SetLineStyle(2);
   gaus_draw5->Draw("same");
  
   TF1 *gaus_draw6 = new TF1("gaus_draw6", "gaus", 5400., 5600.);
   gaus_draw6->FixParameter( 0, total2->GetParameter(3) );
   gaus_draw6->FixParameter( 1, total2->GetParameter(4) );
   gaus_draw6->FixParameter( 2, total2->GetParameter(5) );
   gaus_draw6->SetLineColor(kGreen-3);
   gaus_draw6->SetLineStyle(9);
   gaus_draw6->Draw("same");

   gPad->Update();

   

   /* ***** Qui comincia la parte con la calibrazione in energia con b=0 *******  */
   /*
   TCanvas *c7 = new TCanvas("c7", "c7");
   h_energy_time_cal2->GetXaxis()->SetTitle("T [ch]");
   h_energy_time_cal2->GetXaxis()->SetTitleSize(0.05);
   h_energy_time_cal2->GetXaxis()->SetTitleOffset(0.92);
   h_energy_time_cal2->GetYaxis()->SetTitle("E [keV]");
   h_energy_time_cal2->GetYaxis()->SetTitleSize(0.05);
   h_energy_time_cal2->GetYaxis()->SetTitleOffset(0.93);
   h_energy_time_cal2->Draw("colz");
  
   TH1D *h_ener_cal2 = h_energy_time_cal2->ProjectionY();

   TCanvas *c8 = new TCanvas("c8", "c8");
   //c3->SetGrid();
   h_ener_cal2->SetTitle("Spettro energetico");
   h_ener_cal2->GetYaxis()->SetTitle("Conteggi");
   h_ener_cal2->GetYaxis()->SetTitleOffset(1.15);
   h_ener_cal2->SetLineWidth(1);
   //h_ener_cal->SetLineColor(kAzure - 8);
   //h_ener_cal->SetFillColor(kAzure - 8);
   //h_ener_cal->SetFillStyle(3002);
   h_ener_cal2->Draw();

   TH1D *h_ener_cal2_clone1 = (TH1D*)( h_ener_cal2->Clone() );
   TH1D *h_ener_cal2_clone2 = (TH1D*)( h_ener_cal2->Clone() );
   
   h_ener_cal2_clone1->SetName("Fit picco alta E");
   h_ener_cal2_clone2->SetName("Parametri funzione a due gaussiane");

   TF1 *gaus3 = new TF1("gaus3", "gaus", 5320., 5440.);
   gaus3->SetParLimits(1, 5400., 5430.);
   //gaus3->SetParLimits(2, 18.53, 18.54);
   TF1 *gaus4 = new TF1("gaus4", "gaus", 5448., 5600.);
   //gaus4->SetParLimits(1, 5460., 5466.);

   TF1 *total1 = new TF1("total1", "gaus(0) + gaus(3)", 5300., 5600.);
   total1->SetParName(0, "Const1");
   total1->SetParName(1, "#mu1");
   total1->SetParName(2, "#sigma1");
   total1->SetParName(3, "Const2");
   total1->SetParName(4, "#mu2");
   total1->SetParName(5, "#sigma2");

   gaus3->SetLineColor(kBlue + 2);
   gaus4->SetLineColor(kOrange);
   Double_t par2[6];

   //h_ener_cal2->Fit(gaus3,"RM");
   h_ener_cal2_clone1->Fit(gaus4,"RM","sames");

   Double_t sigma = gaus4->GetParameter(2);
   //std::cout << "La sigma di gaus4 e' " << sigma << std::endl;

   //gaus3->SetParLimits(2, sigma - 0.001*sigma, sigma + 0.001*sigma);
   //gaus3->FixParameter(2, sigma);
   h_ener_cal2->Fit(gaus3,"RM");

   gaus3->GetParameters(&par2[0]);
   gaus4->GetParameters(&par2[3]);
 
   total1->SetParameters(par2);
   //total1->SetParLimits(1, 5410., 5422.);
   total1->SetParLimits(2, sigma - 0.001*sigma, sigma + 0.001*sigma);
   //total1->FixParameter(2, 20.11);
   //total1->SetParLimits(4, 5462., 5463.);
   total1->SetParLimits(5, sigma - 0.001*sigma, sigma + 0.001*sigma);
   //total1->FixParameter(5, sigma);

   h_ener_cal2_clone2->Fit(total1,"RM","sames");

   TF1 *gaus_draw3 = new TF1("gaus_draw3", "gaus", 5400., 5600.);
   gaus_draw3->FixParameter( 0, total1->GetParameter(0) );
   gaus_draw3->FixParameter( 1, total1->GetParameter(1) );
   gaus_draw3->FixParameter( 2, total1->GetParameter(2) );
   gaus_draw3->SetLineColor(kOrange + 1);
   gaus_draw3->SetLineStyle(2);
   gaus_draw3->Draw("same");
  
   TF1 *gaus_draw4 = new TF1("gaus_draw4", "gaus", 5400., 5600.);
   gaus_draw4->FixParameter( 0, total1->GetParameter(3) );
   gaus_draw4->FixParameter( 1, total1->GetParameter(4) );
   gaus_draw4->FixParameter( 2, total1->GetParameter(5) );
   gaus_draw4->SetLineColor(kGreen-3);
   gaus_draw4->SetLineStyle(9);
   gaus_draw4->Draw("same");

   gPad->Update();
   */
}
