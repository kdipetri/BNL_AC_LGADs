#ifndef plothelper_h
#define plothelper_h

#include <map>
#include <iostream>
#include <string>
#include <cmath>
#include <TROOT.h>
#include "TCanvas.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TProfile.h"
#include "TColor.h"
#include "TList.h"
#include "TF1.h"
#include "TMath.h"
#include "TLegend.h"
#include "TLegendEntry.h"
#include "TGraph.h"
#include "TStyle.h"
#include "TPaveStats.h"

using namespace std;
std::string plotdir = "plots";

class PlotHelper {
 public:
  PlotHelper(const std::string formatString="eps") {
    setPlotStyle();
    doSumW2 = true;
    doPng = (formatString.find("png")!=std::string::npos);
    doEps = (formatString.find("eps")!=std::string::npos);
    doC = (formatString.find("C")!=std::string::npos);
  };

  static void setPlotStyle(const int fontOptions=0) {
    if (fontOptions==1) {
      std::cout << "Using Karris Plot Style" << endl;
      TStyle* gStyle = new TStyle("gStyle","Karris Plot Style"); 
      
      //Karri's 2D Color Palatte (Inspired by Emma Tolley's Jet Cleaning Plots)
      //const Int_t Number = 6;
      //Double_t red[Number]    = { 1.00, 0.15, 0.60, 1.00, 1.00, 1.00 };
      //Double_t green[Number]  = { 1.00, 0.05, 0.15, 0.00, 0.60, 0.90};
      //Double_t blue[Number]   = { 1.00, 1.00, 1.00, 0.60, 0.05, 0.00 };
      //Double_t stops[Number]  = { 0.00, 0.40, 0.60, 0.80, 0.90, 1.0};
      //Int_t ncontours = 50;
      //TColor::CreateGradientColorTable(Number,stops,red,green,blue,ncontours);
      //gStyle->SetNumberContours(ncontours);
      gStyle->SetPalette(kBird);
      
      Int_t icol=0; // WHITE
      gStyle->SetFrameBorderMode(icol); //Plain colors
      gStyle->SetFrameFillColor(icol);
      gStyle->SetCanvasBorderMode(icol);
      gStyle->SetCanvasColor(icol);
      gStyle->SetPadBorderMode(icol);
      gStyle->SetPadColor(icol);
      gStyle->SetStatColor(icol);
      gStyle->SetTitleFillColor(icol);

      Int_t font = 42; // Helvetica
      //gStyle->SetTextSize(tsize);
      gStyle->SetLabelFont(font,"x"); 
      gStyle->SetTitleFont(font,"x");
      gStyle->SetLabelFont(font,"y");
      gStyle->SetTitleFont(font,"y");
      gStyle->SetLabelFont(font,"z");
      gStyle->SetTitleFont(font,"z");

      Double_t lsize = 0.05; 
      Double_t tsize = 0.05;
      gStyle->SetLabelSize(lsize,"x");
      gStyle->SetLabelSize(lsize,"y");
      gStyle->SetLabelSize(lsize,"z");
      gStyle->SetTitleSize(tsize,"y");
      gStyle->SetTitleSize(tsize,"x");
      gStyle->SetTitleSize(tsize,"z");

      // title offset
      gStyle->SetTitleXOffset(1.6); 
      gStyle->SetTitleYOffset(1.6);

      gStyle->SetMarkerSize(1.5); //large markers
      gStyle->SetHistLineWidth(2.0); // bold lines
      gStyle->SetLineStyleString(2,"[12 12]"); // postscript dashes
 
      gStyle->SetCanvasColor(0); // Canvas
      gStyle->SetCanvasBorderMode(0);
      gStyle->SetCanvasBorderSize(0);

      gStyle->SetTitleBorderSize(0);
      gStyle->SetTitleX(0.1f);
      gStyle->SetTitleW(0.8f);

      //gStyle->SetPaperSize(20,26); // paper size 
      gStyle->SetPadBottomMargin(0.2); //margins...
      gStyle->SetPadTopMargin(0.2);
      gStyle->SetPadLeftMargin(0.2);
      gStyle->SetPadRightMargin(0.2);

      gStyle->SetPadGridX(0); // grids, tickmarks
      gStyle->SetPadGridY(0);
      gStyle->SetPadTickX(1);
      gStyle->SetPadTickY(1);

      gStyle->SetFrameBorderMode(0);
      gStyle->SetFrameBorderSize(40);
      gStyle->SetHatchesLineWidth(2);

      gStyle->SetOptStat(1100); // #Entries Mean
      gStyle->SetOptStat(0);
      gStyle->SetStatX(0.9);
      gStyle->SetStatY(0.9);
      gStyle->SetOptFit(0);    // Fits

      gROOT->SetStyle("gStyle");
      gROOT->ForceStyle();
    }
  }

  int histColour(const int i) {
    if (i==0) return kBlack;
    if (i==1) return kOrange-4;
    if (i==2) return kRed-4;
    if (i==3) return kMagenta+1;
    if (i==4) return kViolet+8;
    return kBlack;
  }
  
  int histFill(const int i) {
    if (i==1) return 3345;
    if (i==2) return 3354;
    if (i==3) return 3344;
    return 1001;
  }
  int wide = 2;
 
  void Plot1D(const std::string name, const std::string title,
	      const float x,
	      const int nbinsx, const float xmin, const float xmax,
	      const float weight=1.) {
    //std::string fname = name;
    //std::replace(fname.begin(), fname.end(), ' ', '_');
    std::map<std::string, TH1F*>::iterator iter = histos_1d.find(name);
    if (iter==histos_1d.end()) { //cout << "Creating " << name << endl;// doesn't exist, so create histo
      TH1F *h = new TH1F(name.c_str(), title.c_str(), nbinsx, xmin, xmax);
      if (doSumW2) h->Sumw2();
      TPaveStats *stats = (TPaveStats*) h->GetListOfFunctions()->FindObject("stats");
      if (stats){
        stats->SetY2NDC(0.9);
        stats->SetY1NDC(0.7);
      }
      //h->SetTitle(name.c_str());
      h->Fill(x, weight);
      h->SetLineColor(histColour(0));
      h->SetLineWidth(wide);
      h->SetFillColor(kAzure-4);
      h->SetFillStyle(histFill(0));
      h->SetStats(kTRUE);
      histos_1d.insert( std::pair<std::string, TH1F*> (name,h) );
    } else {// exists already, so just fill it
      iter->second->Fill(x,weight);
    }
  }

  void Plot1Dlog(const std::string name, const std::string title,
        const float x,
        const float xmin, const float xmax,
        const float weight=1.) {
    //Makes a histogram w/ bins designed for a log scale x-axis
    std::map<std::string, TH1F*>::iterator iter = histos_1dlog.find(name);
    if (iter==histos_1dlog.end()) {// doesn't exist, so create histo

      // Histogram parameters
      const int nbinsx=100;
      float log_xmin = log10(xmin);
      float log_xmax = log10(xmax);
      float spacing = (log_xmax-log_xmin)/nbinsx;

      // For saving bin values
      Double_t edges[nbinsx + 1] = {};

      // Starting
      int i = 0;
      float bin = log_xmin;
      edges[i] = pow(10,bin);

      // Looping
      while (bin <= log_xmax){
        i++;
        bin = bin+spacing;
        edges[i]=pow(10,bin);
      }

      TH1F *h = new TH1F(name.c_str(), title.c_str(), nbinsx, edges);
      if (doSumW2) h->Sumw2();
      TPaveStats *stats = (TPaveStats*) h->GetListOfFunctions()->FindObject("stats");
      if (stats){
        stats->SetY2NDC(0.9);
        stats->SetY1NDC(0.7);
      }
      //h->SetTitle(name.c_str());
      h->Fill(x, weight);
      h->SetLineColor(histColour(0));
      h->SetLineWidth(wide);
      h->SetFillColor(histColour(4));
      h->SetFillStyle(histFill(0));
      h->SetStats(kTRUE);
      histos_1dlog.insert( std::pair<std::string, TH1F*> (name,h) );
    } else {// exists already, so just fill it
      iter->second->Fill(x,weight);
    }
  }

  void Plot1DBin(const std::string name, const std::string title,
        const float xbin, const float xvalue, const float xerror,
        const int nbinsx, const float xmin, const float xmax,
        const char* binname="a") {
    std::map<std::string, TH1F*>::iterator iter = histos_1d.find(name);
    if (iter==histos_1d.end()) {// doesn't exist, so create histo
      TH1F *h = new TH1F(name.c_str(), title.c_str(), nbinsx, xmin, xmax);
      if (doSumW2) h->Sumw2();
      h->SetBinContent(xbin+1, xvalue);
      h->SetBinError(xbin+1, xerror);
      //h->SetTitle(name.c_str());
      if(strncmp(binname,"a", 1) != 0){h->GetXaxis()->SetBinLabel(xbin+1, binname);}
      histos_1d.insert( std::pair<std::string, TH1F*> (name,h) );
    } else {// exists already, so just fill it
      iter->second->SetBinContent(xbin+1, xvalue);
      iter->second->SetBinError(xbin+1, xerror);
      if(strncmp(binname,"a", 1) != 0){iter->second->GetXaxis()->SetBinLabel(xbin+1, binname);}
    }
  }

  void Plot2D(const std::string name, const std::string title,
	      const float x, const float y,
	      const int nbinsx, const float xmin, const float xmax,
	      const int nbinsy, const float ymin, const float ymax,
	      const float weight=1.) {
    std::string fname = name;
    std::replace(fname.begin(), fname.end(), ' ', '_');
    std::map<std::string, TH2F*>::iterator iter = histos_2d.find(fname);
    if (iter==histos_2d.end()) {// doesn't exist, so create histo
      TH2F *h = new TH2F(fname.c_str(), title.c_str(), nbinsx, xmin, xmax, nbinsy, ymin, ymax);
      if (doSumW2) h->Sumw2();
      TPaveStats *stats = (TPaveStats*) h->GetListOfFunctions()->FindObject("stats");
      if (stats){
        stats->SetY2NDC(0.9);
        stats->SetY1NDC(0.7);
      }
      //h->SetTitle(name.c_str());
      h->Fill(x, y, weight);
      //h->Write();
      h->SetStats(kTRUE);  
      histos_2d.insert( std::pair<std::string, TH2F*> (fname,h) );
    } else {// exists already, so just fill it
      iter->second->Fill(x,y,weight);
    }
  }
  void Plot3D(const std::string name, const std::string title,
        const float x, const float y, const float z,
        const int nbinsx, const float xmin, const float xmax,
        const int nbinsy, const float ymin, const float ymax,
        const int nbinsz, const float zmin, const float zmax,
        const float weight=1.) {
    std::string fname = name;
    std::replace(fname.begin(), fname.end(), ' ', '_');
    std::map<std::string, TH3F*>::iterator iter = histos_3d.find(fname);
    if (iter==histos_3d.end()) {// doesn't exist, so create histo
      TH3F *h = new TH3F(fname.c_str(), title.c_str(), nbinsx, xmin, xmax, nbinsy, ymin, ymax, nbinsz, zmin, zmax);
      if (doSumW2) h->Sumw2();
      TPaveStats *stats = (TPaveStats*) h->GetListOfFunctions()->FindObject("stats");
      if (stats){
        stats->SetY2NDC(0.9);
        stats->SetY1NDC(0.7);
      }
      //h->SetTitle(name.c_str());
      h->Fill(x, y, z, weight);
      //h->Write();
      h->SetStats(kTRUE);  
      histos_3d.insert( std::pair<std::string, TH3F*> (fname,h) );
    } else {// exists already, so just fill it
      iter->second->Fill(x,y,z,weight);
    }
  }

  void DrawGraph(TCanvas *c1, TGraph *g){
    c1->cd();
    c1->Clear();
    g->Draw("ALP");
    if(doPng)c1->Print(Form("%s/%s.png",plotdir.c_str(),g->GetName()));
    if(doEps)c1->Print(Form("%s/%s.eps",plotdir.c_str(),g->GetName()));
    if(doC)  c1->Print(Form("%s/%s.C",plotdir.c_str(),g->GetName()));
    g->Write();
  }

  void DrawTriple1D(TCanvas *c1, const char * const title,
		    TH1F *h1, TH1F *h2, TH1F *h3,
		    const char *const s1="Board 16", const char *const s2="Board 17", const char *const s3="Board 17",
		    const int logy=0) {
    c1->cd();
    c1->Clear();
    h1->SetLineColor(histColour(1));
    h1->SetLineWidth(wide);
    h1->SetMarkerColor(histColour(1));
    h1->SetMaximum( 1.09 * std::max( std::max(h1->GetMaximum(), h2->GetMaximum()), h3->GetMaximum() ) );
    h1->Draw("");
    h1->GetYaxis()->SetTitleOffset(2.0); 
    h1->SetTitle(title);

    h2->SetLineColor(histColour(2));
    h2->SetLineWidth(wide);
    h2->SetMarkerColor(histColour(2));
    h2->Draw("sames"); 

    h3->SetLineColor(histColour(3));
    h3->SetLineWidth(wide);
    h3->SetMarkerColor(histColour(3));
    h3->SetStats(0);
    h3->Draw("sames");
 	  
    TPaveStats *stats1 = (TPaveStats*) h1->GetListOfFunctions()->FindObject("stats");
    TPaveStats *stats2 = (TPaveStats*) h2->GetListOfFunctions()->FindObject("stats");
    TPaveStats *stats3 = (TPaveStats*) h3->GetListOfFunctions()->FindObject("stats");

    if (stats1 && stats2 && stats3) {
      stats2->SetY2NDC(0.9);
      stats2->SetY1NDC(0.7);
      stats1->SetY2NDC(0.69);
      stats1->SetY1NDC(0.49);
      stats1->SetBorderSize(1);
      stats2->SetBorderSize(1);
      stats3->SetBorderSize(1);
    }
    
    const int maxLegendStringLength = std::max(strlen(s1),strlen(s2));
    TLegend* leg = new TLegend((maxLegendStringLength<11 ? 0.72 : 0.64), 0.8, 0.9, 0.9);
    leg->AddEntry(h1, s1, "lp");
    leg->AddEntry(h2, s2, "lp");
    leg->AddEntry(h3, s3, "lp");
    leg->SetBorderSize(1);
    leg->SetFillColor(kWhite);
    leg->Draw();
    c1->SetLogy(logy);
    
    if(doPng)c1->Print(Form("%s/%s_trpl.png", plotdir.c_str(), title));
    if(doEps)c1->Print(Form("%s/%s_trpl.eps", plotdir.c_str(), title));
    if(doC)c1->Print(Form("%s/%s_trpl.C", plotdir.c_str(), title));
    c1->Print(Form("%s/%s_trpl.C", plotdir.c_str(), title));
    c1->SetLogy(0);
    h1->Write();
    h2->Write();
    h3->Write();
  }

  void DrawQuadruple1D(TCanvas *c1, const char * const title,
        TH1F *h1, TH1F *h2, TH1F *h3, TH1F *h4,
        const char *const s1="Board 16", const char *const s2="Board 17", const char *const s3="Board 17", const char *const s4="Board 17",
        const int logy=0) {
    c1->cd();
    c1->Clear();
    h1->SetLineColor(histColour(1));
    h1->SetLineWidth(wide);
    h1->SetMarkerColor(histColour(1));
    h1->SetMaximum( 1.09 * std::max( std::max(h1->GetMaximum(), h2->GetMaximum()), h3->GetMaximum() ) );
    h1->Draw("");
    h1->GetYaxis()->SetTitleOffset(2.0);
    h1->SetTitle(title); 

    h2->SetLineColor(histColour(2));
    h2->SetLineWidth(wide);
    h2->SetMarkerColor(histColour(2));
    h2->Draw("sames"); 

    h3->SetLineColor(histColour(3));
    h3->SetLineWidth(wide);
    h3->SetMarkerColor(histColour(3));
    h3->Draw("sames");

    h4->SetLineColor(histColour(4));
    h4->SetLineWidth(wide);
    h4->SetMarkerColor(histColour(4));
    h4->Draw("sames");
    
    TPaveStats *stats1 = (TPaveStats*) h1->GetListOfFunctions()->FindObject("stats");
    TPaveStats *stats2 = (TPaveStats*) h2->GetListOfFunctions()->FindObject("stats");
    TPaveStats *stats3 = (TPaveStats*) h3->GetListOfFunctions()->FindObject("stats");
    TPaveStats *stats4 = (TPaveStats*) h4->GetListOfFunctions()->FindObject("stats");
    
    if (stats1 && stats2 && stats3 && stats4) {
      stats2->SetY2NDC(0.9);
      stats2->SetY1NDC(0.7);
      stats1->SetY2NDC(0.69);
      stats1->SetY1NDC(0.49);
      stats1->SetBorderSize(1);
      stats2->SetBorderSize(1);
      stats3->SetBorderSize(1);
      stats4->SetBorderSize(1);
    }
    
    const int maxLegendStringLength = std::max(strlen(s1),strlen(s2));
    TLegend* leg = new TLegend((maxLegendStringLength<11 ? 0.72 : 0.64), 0.8, 0.9, 0.9);
    leg->AddEntry(h1, s1, "lp");
    leg->AddEntry(h2, s2, "lp");
    leg->AddEntry(h3, s3, "lp");
    leg->AddEntry(h4, s4, "lp");
    leg->SetBorderSize(1);
    leg->SetFillColor(kWhite);
    leg->Draw();
    c1->SetLogy(logy);
    
    if(doPng)c1->Print(Form("%s/%s_qdrpl.png", plotdir.c_str(), title));
    if(doEps)c1->Print(Form("%s/%s_gdrpl.eps", plotdir.c_str(), title));
    if(doC)c1->Print(Form("%s/%s_qdrpl.C", plotdir.c_str(), title));
    c1->Print(Form("%s/%s_qdrpl.C", plotdir.c_str(), title));
    c1->SetLogy(0);
    h1->Write();
    h2->Write();
    h3->Write();
    h4->Write();
  }

  void DrawDouble1D(TCanvas *c1, const char * const title,
		    TH1F *h1, TH1F *h2,
		    const char *const s1="Board 16", const char *const s2="Board 17",
		    const int logy=0) {
    if (!h1 || !h2) return;
    c1->cd();
    c1->Clear();
    //gStyle->SetOptStat(11); 
    h1->SetMaximum( 1.09 * std::max(h1->GetMaximum(), h2->GetMaximum() ) );
    h1->SetBinContent(h1->GetNbinsX(), h1->GetBinContent(h2->GetNbinsX() + 1) + h1->GetBinContent(h1->GetNbinsX())); //deal with over flow
    h1->SetLineColor(histColour(1));
    h1->SetLineWidth(wide);
    h1->SetFillColor(histColour(1));
    h1->SetFillStyle(histFill(1));
    h1->GetYaxis()->SetTitleOffset(1.4);
    h1->Draw("hist");
    h1->SetTitle(title);

    h2->SetBinContent(h2->GetNbinsX(), h2->GetBinContent(h2->GetNbinsX() + 1) + h2->GetBinContent(h2->GetNbinsX())); //deal with over flow 
    h2->SetLineColor(histColour(3));
    h2->SetLineWidth(wide);
    h2->SetFillColorAlpha(histColour(3),0.45);
    h2->SetFillStyle(histFill(2));
    h2->Draw("hist sames"); 
 	  
    TPaveStats *stats1 = (TPaveStats*) h1->GetListOfFunctions()->FindObject("stats");
    TPaveStats *stats2 = (TPaveStats*) h2->GetListOfFunctions()->FindObject("stats");
    //printf("stats1: %p ;  stats2: %p\n", stats1, stats2);
    if (stats1 && stats2) {
      stats2->SetY2NDC(0.9);
      stats2->SetY1NDC(0.7);
      stats1->SetY2NDC(0.69);
      stats1->SetY1NDC(0.49);
      stats1->SetBorderSize(1);
      stats2->SetBorderSize(1);
    }
    
    const int maxLegendStringLength = std::max(strlen(s1),strlen(s2));
    TLegend* leg = new TLegend( //0.25,0.73,0.4,0.83);
			       (maxLegendStringLength<11 ? 0.72 : 0.64), 0.8, 0.9, 0.9);
    leg->AddEntry(h1, s1, "l");
    leg->AddEntry(h2, s2, "l");
    leg->SetBorderSize(1);
    leg->SetFillColor(kWhite);
    leg->Draw();
    c1->SetLogy(logy);
    
    if(doPng)c1->Print(Form("%s/%s_dbl.png", plotdir.c_str(), title));
    if(doEps)c1->Print(Form("%s/%s_dbl.eps", plotdir.c_str(), title));
    if(doC)c1->Print(Form("%s/%s_dbl.C", plotdir.c_str(), title));
    c1->SetLogy(0);
    h1->Write();
    h2->Write();
  }

  void Draw1D(TCanvas *c1, TH1F *h, const int logy=0,
	      const char *const drawopt="hist", bool norm=false) {
    c1->cd();
    c1->Clear();
    h->Draw(drawopt);
    c1->SetLogy(logy);
    //deal with overflow & underflow here
    //h->SetBinContent(h->GetNbinsX(), h->GetBinContent(h->GetNbinsX() + 1) + h->GetBinContent(h->GetNbinsX()));
    //h->SetBinContent(h->GetNbinsX() + 1, 0);
    //h->SetBinContent(1, h->GetBinContent(0) + h->GetBinContent(1));
    if(norm) h->Scale(1.0/h->Integral());
    //h->SetBinError(h->GetNbinsX(), sqrt(pow(h->GetBinError(h->GetNbinsX() + 1), 2) + pow(h->GetBinError(h->GetNbinsX()),2)));
    if(doPng)c1->Print(Form((norm ? "%s/%s_norm.png" : "%s/%s.png"), plotdir.c_str(), h->GetName()));
    if(doEps)c1->Print(Form((norm ? "%s/%s_norm.eps" : "%s/%s.eps"), plotdir.c_str(), h->GetName()));
    if(doC)  c1->Print(Form((norm ? "%s/%s_norm.C"   : "%s/%s.C"  ), plotdir.c_str(), h->GetName()));
    c1->SetLogy(0);
    h->Write();
  }

    void Draw1Dlog(TCanvas *c1, TH1F *h, const int logy=1,
        const char *const drawopt="hist", bool norm=false) {
    c1->cd();
    c1->Clear();
    if (strstr(h->GetName(),"channel_nhit") || strstr(h->GetName(),"channel_charge")) {
      h->SetMinimum(0.);
    }
    h->Draw(drawopt);
    c1->SetLogy(1);
    //deal with overflow here
    h->SetBinContent(h->GetNbinsX(), h->GetBinContent(h->GetNbinsX() + 1) + h->GetBinContent(h->GetNbinsX()));
    h->SetBinContent(h->GetNbinsX() + 1, 0);
    if(norm){
      h->Scale(1.0/h->Integral());
    }
    //h->SetBinContent(1, h->GetBinContent(0) + h->GetBinContent(1));
    //h->SetBinError(h->GetNbinsX(), sqrt(pow(h->GetBinError(h->GetNbinsX() + 1), 2) + pow(h->GetBinError(h->GetNbinsX()),2)));
    if(doPng)c1->Print(Form((norm ? "%s/%s_norm.png" : "%s/%s.png"), plotdir.c_str(), h->GetName()));
    if(doEps)c1->Print(Form((norm ? "%s/%s_norm.eps" : "%s/%s.eps"), plotdir.c_str(), h->GetName()));
    if(doC)  c1->Print(Form((norm ? "%s/%s_norm.C"   : "%s/%s.C"  ), plotdir.c_str(), h->GetName()));
    c1->SetLogy(1);
    h->Write();
  }

  void Draw2D(TCanvas *c1, TH2F *h, const int logy=0, const char *const drawopt="COLZ"){
    c1->cd();
    c1->Clear();
    c1->SetLeftMargin(0.2);
    c1->SetBottomMargin(0.2);
    c1->SetLogy(logy);
    //h->GetYaxis()->SetTitleOffset(1.5);
    h->Draw(drawopt);
    if(doPng)c1->Print(Form("%s/%s.png", plotdir.c_str(), h->GetName()));
    if(doEps)c1->Print(Form("%s/%s.eps", plotdir.c_str(), h->GetName()));
    if(doC)c1->Print(Form("%s/%s.C", plotdir.c_str(), h->GetName()));
    h->Write();

  }
  void Draw3D(TCanvas *c1, TH3F *h, const int logz=0, const char *const drawopt="BOX"){
    c1->cd();
    c1->Clear();
    c1->SetLogz(logz);
    //h->GetYaxis()->SetTitleOffset(1.5);
    h->Draw(drawopt);
    if(doPng)c1->Print(Form("%s/%s.png", plotdir.c_str(), h->GetName()));
    if(doEps)c1->Print(Form("%s/%s.eps", plotdir.c_str(), h->GetName()));
    if(doC)c1->Print(Form("%s/%s.C", plotdir.c_str(), h->GetName()));
    h->Write();

  }

    void Draw2Dlog(TCanvas *c1, TH2F *h, const int logy=1, const char *const drawopt="COLZ"){
    c1->cd();
    c1->Clear();
    c1->SetLogy(logy);
    //h->GetYaxis()->SetTitleOffset(2.5);
    h->Draw(drawopt);
    if(doPng)c1->Print(Form("%s/%s.png", plotdir.c_str(), h->GetName()));
    if(doEps)c1->Print(Form("%s/%s.eps", plotdir.c_str(), h->GetName()));
    if(doC)c1->Print(Form("%s/%s.C", plotdir.c_str(), h->GetName()));
    h->Write();

  }

  void DrawProfileX(TCanvas *c1, TH2F *h, const char *const profopt="") {
    if (!h) return;
    //gStyle->SetOptStat(111111);
    c1->cd();
    c1->Clear();
    TProfile *p = h->ProfileX(Form("%s_pfx",h->GetName()), 1, h->GetNbinsY(), profopt);
    if(!p) return;
    p->GetYaxis()->SetTitle(Form(/*"Mean #pm SD/#sqrt{N} of */"%s",h->GetYaxis()->GetTitle()));
    p->GetYaxis()->SetTitleOffset(1.8);//1.0);
    if (strstr(h->GetName(),"cluster_size_vsScintY")) {
      p->SetMinimum(0.);
      p->SetMaximum(5.);
    }
    if (strstr(h->GetName(),"_tpc2d")) {
      p->SetMinimum(-65.);
      p->SetMaximum(+65.);
    }
    if (strstr(h->GetName(),"_dtime_last2scints")) {
      p->SetMinimum(-75.);
      p->SetMaximum(+75.);
    }
    if(strstr(h->GetName(),"_channel_tdo") || strstr(h->GetName(),"_channel_rawtdo")) {
      p->SetMinimum(300.);
      p->SetMaximum(500.);
    }
    p->Draw();
    TLine *l = 0;
    if (strstr(h->GetName(),"r9003_tpc2d") || strstr(h->GetName(),"r9002_tpc2d") ) l = new TLine(-50.,13.,50.,-13.);
    if (strstr(h->GetName(),"r3104_tpc2d")) l = new TLine(-50.,0.,50.,-25.);
    if (l) {
      l->SetLineColor(kRed);
      l->SetLineWidth(2);
      l->Draw();
    }
    if(doPng)c1->Print(Form("%s/%s_pfx.png", plotdir.c_str(), h->GetName()));
    if(doEps)c1->Print(Form("%s/%s_pfx.eps", plotdir.c_str(), h->GetName()));
    if(doC)c1->Print(Form("%s/%s_pfx.C", plotdir.c_str(), h->GetName()));
    //c1->Print(Form("%s/%s_pfx.C", plotdir.c_str(), h->GetName()));
    p->Write();
  }

  void DrawProfileY(TCanvas *c1, TH2F *h, const char *const profopt="") {
    if (!h) return;
    c1->cd();
    c1->Clear();
    TProfile *p = h->ProfileY(Form("%s_pfy",h->GetName())//);//, 1, h->GetNbinsX(), "s");
			      , 1, h->GetNbinsX(), profopt);
    p->GetYaxis()->SetTitle(Form(/*"Mean #pm SD/#sqrt{N} of */"%s",h->GetXaxis()->GetTitle()));
    p->GetYaxis()->SetTitleOffset(1.8);
    p->GetXaxis()->SetTitleOffset(1.);
    p->Draw();
    if(doPng)c1->Print(Form("%s/%s_pfy.png", plotdir.c_str(), h->GetName()));
    if(doEps)c1->Print(Form("%s/%s_pfy.eps", plotdir.c_str(), h->GetName()));
    if(doC)c1->Print(Form("%s/%s_pfy.C", plotdir.c_str(), h->GetName()));
    p->Write();
  }

  int getColour(const int i) {
    const int ncolours = 7;
    if (i%ncolours==0) return kRed;
    if (i%ncolours==1) return kOrange+7;
    if (i%ncolours==2) return kYellow+1;
    if (i%ncolours==3) return kGreen+3;
    if (i%ncolours==4) return kCyan+1;
    if (i%ncolours==5) return kBlue;
    if (i%ncolours==6) return kMagenta;
    return kBlack;
  }

  void DrawProjectionBinByBin(TCanvas *c1, TH2F *h, const char *const axis="y") {
    c1->cd();
    c1->Clear();
    const int nbins = (!strcmp(axis,"y")) ? h->GetNbinsX() : h->GetNbinsY();
    std::vector<TH1D*> projections;
    std::cout << "Inside DrawProjectionBinByBin: nbins=" << nbins << std::endl;
    std::cout << "Inside DrawProjectionBinByBin: histo name: " << h->GetName() << std::endl;
    float maxy = 0;
    for (int i=0; i<nbins; i++) {
      TH1D *p = 0;
      if (!strcmp(axis,"y")) {
	p = h->ProjectionY(Form("%s_projy_%i",h->GetName(),i+1),i+1,i+1);
      } else {
	p = h->ProjectionX(Form("%s_projx_%i",h->GetName(),i+1),i+1,i+1);
      }
      p->GetYaxis()->SetTitle("Number of events");
      p->GetYaxis()->SetTitleOffset(1.4);
      p->GetXaxis()->SetTitleOffset(1.);
      p->SetStats(0);
      const int thiscol = getColour(i);
      p->SetLineColor(thiscol);
      p->SetMarkerColor(thiscol);
      p->Write();
      projections.push_back(p);
      if (p->GetMaximum()>maxy) maxy = p->GetMaximum();
      std::cout << "       DrawProjBBB: bin " << i;
      std::cout << ": mean=" << p->GetMean() << ",   rms=" << p->GetRMS() << std::endl;
      std::cout << "                   ";
      // Fit range (-24,0) chosen so that we're fitting approx. same range as testbeam note
      p->Fit("gaus","Q","",-24.,0.);
      TF1 *f = (TF1*) p->GetListOfFunctions()->FindObject("gaus");
      if (f) {
	f->SetLineColor(thiscol);
	f->SetLineWidth(1);
	std::cout << "  gaus: mean=" << f->GetParameter(1) << ", sigma=" << f->GetParameter(2);
      }
      //TF1 *weird; weird=0;// Avoid unused var warning
      //weird = new TF1("weird",//"TMath::ATan(gaus(0))"
      //"[0] * TMath::ATan( TMath::Exp( (x*TMath::Pi()/180.-[1])*(x*TMath::Pi()/180.-[1])/([2]*[2]) ) )"
      //,-45.,45.);
      //p->Fit("weird");
      f = (TF1*) p->GetListOfFunctions()->FindObject("weird");
      if (f) {
	f->SetLineColor(thiscol);
	f->SetLineWidth(1);
	std::cout << std::endl;
	std::cout << " weird: mean=" << f->GetParameter(1) << ", sigma=" << f->GetParameter(2);
      }
      std::cout << std::endl;
    }
    std::cout << "Inside BBB: maxy=" << maxy << std::endl;
    const float old0max = projections.at(0)->GetMaximum();
    projections.at(0)->SetMaximum(maxy*1.2);
    TLegend *leg = new TLegend(0.7,0.6,0.9,0.9);
    TString legheader = ((!strcmp(axis,"y")) ? h->GetXaxis()->GetTitle() : h->GetYaxis()->GetTitle());
    for (int i=0; i<nbins; i++) {
      TH1D *p = projections.at(i);
      leg->AddEntry(p, Form("%5.1f %s",
			    ((!strcmp(axis,"y"))
			     ? h->GetXaxis()->GetBinCenter(i+1)
			     : h->GetYaxis()->GetBinCenter(i+1)),
			    (legheader.Contains("(cm)") ? " cm" : "")
			    ), "l");
      if (i==0) {p->Draw("hist");} else {p->Draw("hist same");}
      TF1 *f = (TF1*) p->GetListOfFunctions()->FindObject("gaus");
      if (f) f->Draw("same");
    }
    legheader = legheader.ReplaceAll("Scintillator","Scint.");
    legheader = legheader.ReplaceAll(" (cm)","");
    leg->SetHeader(legheader);
    //leg->SetHeader((!strcmp(axis,"y")) ? h->GetXaxis()->GetTitle() : h->GetYaxis()->GetTitle());
    leg->SetFillColor(kWhite);
    leg->SetBorderSize(1);
    leg->SetTextSize(0.0177);
    leg->SetMargin(0.15);
    leg->Draw();
    if(doPng)c1->Print(Form("%s/%s_proj%s_bbb.png", plotdir.c_str(), h->GetName(), axis));
    if(doEps)c1->Print(Form("%s/%s_proj%s_bbb.eps", plotdir.c_str(), h->GetName(), axis));
    if(doC)c1->Print(Form("%s/%s_proj%s_bbb.C", plotdir.c_str(), h->GetName(), axis));
    delete leg; leg=0;
    projections.at(0)->SetMaximum(1.1*old0max);

    ////////////////////////////////////////////////////////
    // Now save the projections one by one
    for (int i=0; i<nbins; i++) {
      TH1D *p = projections.at(i);
      p->SetLineColor(kBlack);
      p->Draw("hist");
      TF1 *f = 0;
      f = (TF1*) p->GetListOfFunctions()->FindObject("gaus");
      if (f) {
      	f->SetLineColor(kRed);
      	f->SetFillColor(kWhite);
      	f->SetMarkerSize(0);
      }
      const float mysigma = (f ? f->GetParameter(2) : 999.);
      // Only draw fit for tpc2d plots (i.e. with angle as x-axis), for 20degree tracks (i.e. bin i==4)
      const bool whetherToDrawFit =  (i==4) && !strstr(h->GetName(), "_tpcSlope2d");//(sigma<100.);
      const float legbot = (whetherToDrawFit ?
			    0.79//(strstr(h->GetName(), "_tpcSlope2d") ? 0.75 : 0.79)
			    : 0.81);
      TLegend *leg_new = new TLegend(0.67, legbot, 0.9, 0.9);
      legheader = ((!strcmp(axis,"y")) ? h->GetXaxis()->GetTitle() : h->GetYaxis()->GetTitle());
      const float mybincentre = ((!strcmp(axis,"y"))
				 ? h->GetXaxis()->GetBinCenter(i+1)
				 : h->GetYaxis()->GetBinCenter(i+1));
      const char *unit = (strstr(h->GetName(), "_tpc2d") ? "" : "");
      //leg->AddEntry(p,
      //		    //Form("%5.1f %s", mybincentre, (legheader.Contains("(cm)") ? " cm" : "")),
      //	    Form("Mean:%.1f%s; RMS:%.1f%s", p->GetMean(), unit, p->GetRMS(), unit),
      //	    "l");
      legheader = legheader.ReplaceAll("Scintillator y in bottom layer","Scint. y bottom: ");
      const char *angleRange = "";
      if (strstr(h->GetName(),"r3104")) {
	// Do this only for runs with (0,25)deg angular range, not the (-13,+13) range
	if (fabs(mybincentre+50)<0.001) angleRange = " 0#circ";//" (-2,3)#circ";
	if (fabs(mybincentre+30)<0.001) angleRange = " 5#circ";//" (3,8)#circ";
	if (fabs(mybincentre+10)<0.001) angleRange = " 10#circ";//" (8,13)#circ";
	if (fabs(mybincentre-10)<0.001) angleRange = " 15#circ";//" (13,18)#circ";
	if (fabs(mybincentre-30)<0.001) angleRange = " 20#circ";//" (18,23)#circ";
	if (fabs(mybincentre-50)<0.001) angleRange = " 25#circ";//" (23,28)#circ";
      }
      legheader = legheader.ReplaceAll(" (cm)",Form("%+.0fcm",mybincentre));
      //legheader = Form("#splitline{%s}{Angular range w.r.t perp.: %s}",legheader.Data(),angleRange);
      leg_new->SetHeader(legheader);
      leg_new->AddEntry((TObject*)0, Form("Scint. mean angle:%s",angleRange), "h");
      //{
      //TLegendEntry *e = new TLegendEntry(0, "moo", "h");
      //leg->GetListOfPrimitives()->Add((TObject*)e);
      //}
      //leg->SetHeader((!strcmp(axis,"y")) ? h->GetXaxis()->GetTitle() : h->GetYaxis()->GetTitle());
      leg_new->AddEntry(p,
		    //Form("%5.1f %s", mybincentre, (legheader.Contains("(cm)") ? " cm" : "")),
		    Form("Mean:%.1f%s; RMS:%.1f%s", p->GetMean(), unit, p->GetRMS(), unit),
		    "l");
      if (f && whetherToDrawFit) {
	const float mymean = f->GetParameter(1);
	leg_new->AddEntry(f, //(TObject*)0,
		      Form("Mean:%.1f; #sigma:%.1f",mymean,mysigma), "l");//, "");
	// Translate mean of fit to slope distribution, into mean reconstructed angle (from horizontal)
	const float mymidangle = TMath::ATan(mymean)*180./M_PI;
	//leg->AddEntry((TObject*)0, "foobar","h");
	if (strstr(h->GetName(), "_tpcSlope2d")) {
	  std::cout << "mymidangle: " << mymidangle << std::endl;
	  //leg->AddEntry((TObject*)0, Form(" #Rightarrow %4.1f_{-%4.1f}^{+%4.1f} deg. w.r.t. perp.",
	  //				  90.-mymidangle,
	  //				  TMath::ATan(mymean+mysigma)*180./M_PI - mymidangle,
	  //				  mymidangle - TMath::ATan(mymean-mysigma)*180./M_PI
	  //				  ),"");
	}
      }
      leg_new->SetFillColor(kWhite);
      leg_new->SetBorderSize(1);
      leg_new->SetTextSize(0.022);
      leg_new->SetMargin(0.15);
      leg_new->Draw();
      if (f && whetherToDrawFit) {
	  std::cout << "Adding fit to plot for bin " << i << std::endl;
	  f->Draw("same");
      }
      //f = (TF1*) p->GetListOfFunctions()->FindObject("weird");
      //if (f) f->Draw("same");
      if(doPng)c1->Print(Form("%s/%s_proj%s_bin%i.png", plotdir.c_str(), h->GetName(), axis, i));
      if(doEps)c1->Print(Form("%s/%s_proj%s_bin%i.eps", plotdir.c_str(), h->GetName(), axis, i));
      if(doC)c1->Print(Form("%s/%s_proj%s_bin%i.C", plotdir.c_str(), h->GetName(), axis, i));
    }
  }

  void DrawAll1D(TCanvas *c1, const char *const drawopt="hist") {
    std::map<std::string, TH1F*>::iterator iter = histos_1d.begin();
    for (; iter!=histos_1d.end(); ++iter) {
      Draw1D(c1, iter->second, 0, drawopt);
    }
  }
  void DrawAll2D(TCanvas *c1, const char *const drawopt="COLZ") {
    std::map<std::string, TH2F*>::iterator iter = histos_2d.begin();
    for (; iter!=histos_2d.end(); ++iter) {
      Draw2D(c1, iter->second, 0, drawopt);
    }
  }
  void DrawAll3D(TCanvas *c1, const char *const drawopt="BOX") {
    std::map<std::string, TH3F*>::iterator iter = histos_3d.begin();
    for (; iter!=histos_3d.end(); ++iter) {
      Draw3D(c1, iter->second, 0, drawopt);
    }
  }
  void DrawAll1Dlog(TCanvas *c1, const char *const drawopt="hist") {
    std::map<std::string, TH1F*>::iterator iter = histos_1dlog.begin();
    for (; iter!=histos_1dlog.end(); ++iter) {
      Draw1Dlog(c1, iter->second, 1, drawopt);
    }
  }


  void DrawProfileX(TCanvas *c1, const char *const s, const char *const profopt="") {
    DrawProfileX(c1, get2D(s), profopt);
  }
  void DrawProfileY(TCanvas *c1, const char *const s, const char *const profopt="") {
    DrawProfileY(c1, get2D(s), profopt);
  }
  void DrawDouble1DPX(TCanvas *c1, const char *const s1, const char *const s2, const double ylowbound = 300, const double yhighbound = 370) {
    DrawDouble1DPX(c1, get2D(s1), get2D(s2), ylowbound, yhighbound);
  }

  void DrawQuadruple1D(TCanvas *c1, const char * const title,
        const char *const h1, const char *const h2, const char *const h3, const char *const h4,
        const char *const s1="Board 16", const char *const s2="Board 17", const char *const s3="Board 17", const char *const s4="Board 17",
        const int logy=0) {
    DrawQuadruple1D(c1, title, get1D(h1), get1D(h2), get1D(h3), get1D(h4), s1, s2, s3, s4, logy);
  }
  void DrawTriple1D(TCanvas *c1, const char * const title,
		    const char *const h1, const char *const h2, const char *const h3,
		    const char *const s1="Board 16", const char *const s2="Board 17", const char *const s3="Board 17",
		    const int logy=0) {
    DrawTriple1D(c1, title, get1D(h1), get1D(h2), get1D(h3), s1, s2, s3, logy);
  }
  void DrawDouble1D(TCanvas *c1, const char * const title,
		    const char *const h1, const char *const h2,
		    const char *const s1="Board 16", const char *const s2="Board 17",
		    const int logy=0) {
    DrawDouble1D(c1, title, get1D(h1), get1D(h2), s1, s2, logy);
  }
  
   void DrawTriple1DNorm(TCanvas *c1, const char * const title,
       TH1F *h1, TH1F *h2, TH1F *h3,
        const char *const s1="Board 16 n", const char *const s2="Board 17 n", const char *const s3="Board 17 n",
        const int logy=0) {
    //std::cout << "GetEntries: " << h1->GetEntries() << ", " << h2->GetEntries() << std::endl;
    const float tmph1 = h1->GetEntries();
    const float tmph2 = h2->GetEntries();
    const float tmph3 = h3->GetEntries();
    h1->Scale(tmph3/tmph1);
    h2->Scale(tmph3/tmph2);
    
    h1->GetYaxis()->SetTitleOffset(1.69);
    DrawTriple1D(c1, title,  h1, h2, h3, s1, s2, s3, logy);

    // Now set the original histograms back to their original normalization:
      h1->Scale(tmph1/tmph3);
      h2->Scale(tmph2/tmph3);
  }
  void DrawTriple1DNorm(TCanvas *c1, const char * const title,
        const char *const h1, const char *const h2, const char *const h3,
        const char *const s1="Board 16 n", const char *const s2="Board 17 n", const char *const s3="Board 17 n",
        const int logy=0) {
    DrawTriple1DNorm(c1, title, get1D(h1), get1D(h2), get1D(h3), s1, s2, s3, logy);
  }
 
  void DrawDouble1DNorm(TCanvas *c1, const char * const title,
       TH1F *h1, TH1F *h2,
        const char *const s1="Board 16 n", const char *const s2="Board 17 n",
        const int logy=0) {
    if (!h1 || !h2) return;
    //std::cout << "GetEntries: " << h1->GetEntries() << ", " << h2->GetEntries() << std::endl;
    const float tmph1 = h1->GetEntries();
    const float tmph2 = h2->GetEntries();
    //std::cout << "Integrals: " << h1->Integral() << ", " << h2->Integral() << std::endl;
    if (tmph1 < tmph2){
      //std::cout << "About to scale h1 by " << tmph2 << "/" << tmph1 << std::endl;
      h1->Scale(tmph2/tmph1);
    } else if (tmph1 >= tmph2){
      //std::cout << "About to scale h2 by " << tmph1 << "/" << tmph2 << std::endl;
      h2->Scale(tmph1/tmph2);
    }
    //h1->SetMaximum(600.);
    //h2->SetMaximum(600.);
    //std::cout << "Integrals: " << h1->Integral() << ", " << h2->Integral() << std::endl;
    DrawDouble1D(c1, title, h1, h2, s1, s2, logy);

    // Now set the original histograms back to their original normalization:
    if (tmph1<tmph2){
      h1->Scale(tmph1/tmph2);
    } else if (tmph1>=tmph2) {
      h2->Scale(tmph2/tmph1);
    }
  }
  void DrawDouble1DNorm(TCanvas *c1, const char * const title,
        const char *const h1, const char *const h2,
        const char *const s1="Board 16 n", const char *const s2="Board 17 n",
        const int logy=0) {
    DrawDouble1DNorm(c1, title, get1D(h1), get1D(h2), s1, s2, logy);
  }
  //provided other normalization factors
  void DrawDouble1DNormE(TCanvas *c1, const char * const title,
        TH1F *h1, TH1F *h2,
        const char *const s1="Board 16 n", const char *const s2="Board 17 n", 
        double tmph1 = 1, double tmph2 = 1,
        const int logy=0) {
    if (tmph1 < tmph2){
      h1->Scale(tmph2/tmph1);}
    else if (tmph1 >= tmph2){
      h2->Scale(tmph1/tmph2);}
    DrawDouble1D(c1, title, h1, h2, s1, s2, logy);
  }
  void DrawDouble1DNormE(TCanvas *c1, const char * const title,
        const char *const h1, const char *const h2,
        const char *const s1="Board 16 n", const char *const s2="Board 17 n",
        const int logy=0) {
    DrawDouble1DNormE(c1, title, get1D(h1), get1D(h2), s1, s2, logy);
  }


  void DrawDouble1DPX(TCanvas *c1,
        TH2F *f1, TH2F *f2, const double ylowbound = 300, const double yhighbound = 370,
        const char *const s1="Board 16", const char *const s2="Board 17",
        const int logy=0) {
    c1->cd();
    c1->Clear();
    TProfile *h1 = f1->ProfileX(Form("%s_pfx",f1->GetName()));
    h1->GetYaxis()->SetTitle(Form(/*"Mean #pm SD/#sqrt{N} of */"%s",f1->GetYaxis()->GetTitle()));
    TProfile *h2 = f2->ProfileX(Form("%s_pfx",f2->GetName()));
    h2->GetYaxis()->SetTitle(Form(/*"Mean #pm SD/#sqrt{N} of */"%s",f2->GetYaxis()->GetTitle()));
    h2->GetYaxis()->SetRangeUser(ylowbound, yhighbound);
    h2->SetLineColor(kBlue);
    h2->GetYaxis()->SetTitleOffset(1.8);
    h2->Draw("");
    
    h1->SetLineColor(kRed);
    h1->GetYaxis()->SetRangeUser(ylowbound, yhighbound);
    h1->GetYaxis()->SetTitleOffset(1.8);
    h1->Draw("sames");
    
    TPaveStats *stats1 = (TPaveStats*) h1->GetListOfFunctions()->FindObject("stats");
    TPaveStats *stats2 = (TPaveStats*) h2->GetListOfFunctions()->FindObject("stats");
    //printf("stats1: %p ;  stats2: %p\n", stats1, stats2);
    if (stats1 && stats2) {
      stats2->SetY2NDC(0.9);
      stats2->SetY1NDC(0.7);
      stats1->SetY2NDC(0.69);
      stats1->SetY1NDC(0.49);
      stats1->SetBorderSize(1);
      stats2->SetBorderSize(1);
    }
    
    TLegend* leg = new TLegend(0.75,0.8,0.9,0.9);
    leg->AddEntry(h1, s1, "l");
    leg->AddEntry(h2, s2, "l");
    leg->SetBorderSize(1);
    leg->SetFillColor(kWhite);
    leg->Draw();
    c1->SetLogy(logy);
    
    if(doPng)c1->Print(Form("%s/%s_dbl.png", plotdir.c_str(), h1->GetName()));
    if(doEps)c1->Print(Form("%s/%s_dbl.eps", plotdir.c_str(), h1->GetName()));
    if(doC)c1->Print(Form("%s/%s_dbl.C", plotdir.c_str(), h1->GetName()));
    c1->SetLogy(0);
    h1->Write();
    h2->Write();
  }

  TH1F *get1D(const char *const s) {
    //return histos_1d.find(s)->second;
    std::map<std::string, TH1F*>::iterator it = histos_1d.find(s);
    if (it==histos_1d.end()) {
      return 0;
    } else {
      return it->second;
    }
  }
  TH1F *get1Dlog(const char *const s) {
    //return histos_1d.find(s)->second;
    std::map<std::string, TH1F*>::iterator it = histos_1dlog.find(s);
    if (it==histos_1dlog.end()) {
      return 0;
    } else {
      return it->second;
    }
  }
  TH2F *get2D(const char *const s) {
    //return histos_2d.find(s)->second;
    std::map<std::string, TH2F*>::iterator it = histos_2d.find(s);
    if (it==histos_2d.end()) {
      return 0;
    } else {
      return it->second;
    }
  }

  TH3F *get3D(const char *const s) {
    std::map<std::string, TH3F*>::iterator it = histos_3d.find(s);
    if (it==histos_3d.end()) {
      return 0;
    } else {
      return it->second;
    }
  }

  void FitSelect1D(vector<double> &lumi_fitvalue, string fitname) {
    std::map<std::string, TH1F*>::iterator iter = histos_1d.begin();
    for (; iter!=histos_1d.end(); ++iter) {
      TString itername = iter->first;
      if (itername.Contains(fitname.c_str())) {
        TH1F *iter_hist = iter->second;
        iter_hist->Fit("pol1","QM");
        TF1 *myfit = iter_hist->GetFunction("pol1");
        std::cout << iter_hist->GetName() << " fit slope " << myfit->GetParameter(1) << " fit inter " << myfit->GetParameter(0) << std::endl;
        lumi_fitvalue.push_back(myfit->GetParameter(1));
      }
    }
  }

void FitSelectGau(string fitname) {
    std::map<std::string, TH1F*>::iterator iter = histos_1d.begin();
    for (; iter!=histos_1d.end(); ++iter) {
      TString itername = iter->first;
      if (itername.Contains(fitname.c_str())) {
        TH1F *iter_hist = iter->second;
        iter_hist->Fit("gaus","QM");
        TF1 *myfit = iter_hist->GetFunction("gaus");
        std::cout << iter_hist->GetName() << " fit width " << myfit->GetParameter(1) << " fit mean " << myfit->GetParameter(0) << std::endl;
      }
    }
  }

  std::map<std::string, TH1F*> histos_1d;
  std::map<std::string, TH1F*> histos_1dlog;
  std::map<std::string, TH2F*> histos_2d;
  std::map<std::string, TH3F*> histos_3d;

  bool doSumW2;

 private:
  bool doC, doEps, doPng;
};

#endif

