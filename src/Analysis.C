#define Analysis_cxx
#include "Analysis.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include "xmeasure.C"
#include "timeres.C"
#include "chargesharing.C"



int Analysis::n_hits()
{
	int nhits = 0;
	if (amp[0] > thresh) nhits++;
	if (amp[1] > thresh) nhits++;
	if (amp[2] > thresh) nhits++;
	return nhits;
}
void Analysis::charge_ratio(std::string cat, int ch1, int ch2)
{
	float ratio = amp[ch1]/amp[ch2];
	plotter.Plot2D(Form("charge_ratio_v_x_%s_%i_%i",cat.c_str(),ch1,ch2),  ";x [mm]; amplitude ratio", x_dut[dut], ratio, 100, xmin, xmax, 50, 0, 10);
	return;
}
void Analysis::charge_fraction(std::string cat, int ch1, int ch2, int ch3=-1)
{
	float num = amp[ch1];
	float den = amp[ch1]+amp[ch2];
	if ( ch3 != -1 ) den+=amp[ch3];

	float fraction = num/den;
	plotter.Plot2D(Form("charge_fraction_v_x_%s_%i_%i",cat.c_str(),ch1,ch2),  ";x [mm]; amplitude ratio", x_dut[dut], fraction, 100, xmin, xmax, 60, 0, 1.1);
	return;
}
void Analysis::makeProfiles()
{
		int nhits = n_hits();
		std::string category = "any";
		for (int ch1=0; ch1<3; ch1++){
			for (int ch2=0; ch2<3; ch2++){
				if (ch1==ch2) continue;
				if (amp[ch1] > thresh && amp[ch2] > thresh) charge_ratio(category,ch1,ch2);
			}
		}

		if      (nhits ==1) category = "one";
		else if (nhits ==2) {
			category = "two";
			for (int ch1=0; ch1<3; ch1++){
				for (int ch2=0; ch2<3; ch2++){
					if (ch1==ch2) continue;
					if (amp[ch1] > thresh && amp[ch2] > thresh) {
						charge_ratio(category,ch1,ch2);
						charge_fraction(category,ch1,ch2);
					}
				}
			}
		}

		else if (nhits ==3) {
			category = "three";
			charge_fraction(category,0,1,2);
			charge_fraction(category,1,2,0);
			charge_fraction(category,2,0,1);
			for (int ch1=0; ch1<3; ch1++){
				for (int ch2=0; ch2<3; ch2++){
					if (ch1==ch2) continue;
					charge_ratio(category,ch1,ch2);
					
				}
			}
		}
		return;
}
void Analysis::Loop(std::string cfg)
{

	
	if (fChain == 0) return;

	Long64_t nentries = fChain->GetEntriesFast();

	Long64_t nbytes = 0, nb = 0;
	for (Long64_t jentry=0; jentry<nentries;jentry++) {
		Long64_t ientry = LoadTree(jentry);
		if (ientry < 0) break;
		nb = fChain->GetEntry(jentry);   nbytes += nb;
		

		// for debugging
		//if (ientry > 500) break;
		if (ientry%10000==0) std::cout << "Event" << ientry << std::endl;

		// good track
		if (ntracks!=1) continue;
		if (nplanes<15) continue;
		if (npix <3) continue;
		if (nback<2) continue;
		
 		// in x,y position
		if (x_dut[dut] < xmin) continue;
		if (y_dut[dut] < ymin) continue;
		if (x_dut[dut] > xmax) continue;
		if (y_dut[dut] > ymax) continue;
		if (abs(yResidBack) > 500) continue;
		if (abs(xResidBack) > 500) continue;

		// photek
		if (amp[3] < 50  ) continue;
		if (amp[3] > 250 ) continue;




		int nhits = n_hits();
		if ( nhits == 1 )
		{
			//std::cout << "2 HITS" << std::endl;
			if (amp[0] > thresh) { charge_sharing(cfg,0);}
			if (amp[1] > thresh) { charge_sharing(cfg,1);}
			if (amp[2] > thresh) { charge_sharing(cfg,2);}
		}
		else if ( nhits == 2 )
		{
			//std::cout << "2 HITS" << std::endl;
			if (amp[0] > thresh && amp[1] > thresh) { xpos_single(cfg,0,1); xpos_weight(cfg,0,1); time_res(cfg,0,1); charge_sharing(cfg,0,1);}
			if (amp[0] > thresh && amp[2] > thresh) { xpos_single(cfg,0,2); xpos_weight(cfg,0,2); time_res(cfg,0,2); charge_sharing(cfg,0,2);}
			if (amp[1] > thresh && amp[2] > thresh) { xpos_single(cfg,1,2); xpos_weight(cfg,1,2); time_res(cfg,1,2); charge_sharing(cfg,1,2);}

		}
		else if (nhits == 3){
			xpos_single(cfg,0,1,2);
			xpos_weight(cfg,0,1,2);
			xpos_lookup(cfg,0,1,2);

			time_res(cfg,0,1,2);
			charge_sharing(cfg,0,1,2);
		}
		// try any combination of two hits
		if (amp[0] > thresh) make_t0corr(cfg,0);
		if (amp[1] > thresh) make_t0corr(cfg,1);
		if (amp[2] > thresh) make_t0corr(cfg,2);
		//if (amp[0] > thresh && amp[1] > thresh) xpos_lookup(cfg,0,1);
		//if (amp[0] > thresh && amp[2] > thresh) xpos_lookup(cfg,0,2);
		//if (amp[1] > thresh && amp[2] > thresh) xpos_lookup(cfg,1,2);


	  
	}
   
}
int main(int argc, char* argv[]){

	// defaults 
	std::string cfg = "cfg_4_13_12";
	//std::string cfg = "cfg_6_5_11";

	TFile *file = TFile::Open(Form("skims/%s.root",cfg.c_str()));
	TTree *tree = (TTree*)file->Get("pulse");

	gROOT->SetBatch();
    gStyle->SetOptStat(0);
    PlotHelper::setPlotStyle(0);

	// Do analysis 
	Analysis analysis(tree);
	analysis.Loop(cfg);

	TFile *out = TFile::Open(Form("output/hists_%s.root",cfg.c_str()),"RECREATE");
	out->cd();
	plotter.DrawAll1D(c1);
	plotter.DrawAll2D(c1);
	// Save histogrmas here
	return 0;
}