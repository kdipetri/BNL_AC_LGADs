#define Average_cxx
#include "Average.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void Analysis::avg_waveform(std::string cfg, int ch){

	if (LP2_20[3]==0) return;

	std::string sel = "";
	//if      ( x_dut[dut] > 20.325 && x_dut[dut] < 20.375) sel  = Form("h_sig_ch%i_20325_20375",ch);
	//else if ( x_dut[dut] > 20.375 && x_dut[dut] < 20.425) sel  = Form("h_sig_ch%i_20375_20425",ch);
	//else if ( x_dut[dut] > 20.425 && x_dut[dut] < 20.475) sel  = Form("h_sig_ch%i_20425_20475",ch);
	//else if ( x_dut[dut] > 20.475 && x_dut[dut] < 20.525) sel  = Form("h_sig_ch%i_20475_20525",ch);
	//else if ( x_dut[dut] > 20.525 && x_dut[dut] < 20.575) sel  = Form("h_sig_ch%i_20525_20575",ch);
	//else if ( x_dut[dut] > 20.575 && x_dut[dut] < 20.625) sel  = Form("h_sig_ch%i_20575_20625",ch);
	//if 		( x_dut[dut] > 20.64 && x_dut[dut] < 20.66) sel  = Form("h_sig_ch%i_2064_2066",ch);
	//else if ( x_dut[dut] > 20.62 && x_dut[dut] < 20.64) sel  = Form("h_sig_ch%i_2062_2064",ch);
	//else if ( x_dut[dut] > 20.60 && x_dut[dut] < 20.62) sel  = Form("h_sig_ch%i_2060_2062",ch);
	//else if ( x_dut[dut] > 20.58 && x_dut[dut] < 20.60) sel  = Form("h_sig_ch%i_2058_2060",ch);
	//else if ( x_dut[dut] > 20.56 && x_dut[dut] < 20.58) sel  = Form("h_sig_ch%i_2056_2058",ch);
	//else if ( x_dut[dut] > 20.54 && x_dut[dut] < 20.56) sel  = Form("h_sig_ch%i_2054_2056",ch);
	//else if ( x_dut[dut] > 20.52 && x_dut[dut] < 20.54) sel  = Form("h_sig_ch%i_2052_2054",ch);
	//else if ( x_dut[dut] > 20.50 && x_dut[dut] < 20.52) sel  = Form("h_sig_ch%i_2050_2052",ch);
	//else if ( x_dut[dut] > 20.48 && x_dut[dut] < 20.50) sel  = Form("h_sig_ch%i_2048_2050",ch);
	//else if ( x_dut[dut] > 20.46 && x_dut[dut] < 20.48) sel  = Form("h_sig_ch%i_2046_2048",ch);
	//else if ( x_dut[dut] > 20.44 && x_dut[dut] < 20.46) sel  = Form("h_sig_ch%i_2044_2046",ch);
	//else if ( x_dut[dut] > 20.42 && x_dut[dut] < 20.44) sel  = Form("h_sig_ch%i_2042_2044",ch);
	//else if ( x_dut[dut] > 20.40 && x_dut[dut] < 20.42) sel  = Form("h_sig_ch%i_2040_2042",ch);
	//else if ( x_dut[dut] > 20.38 && x_dut[dut] < 20.40) sel  = Form("h_sig_ch%i_2038_2040",ch);

	if 		( x_dut[dut] > 20.649 && x_dut[dut] < 20.651) sel  = Form("h_sig_ch%i_20649_20651",ch);
	else if ( x_dut[dut] > 20.624 && x_dut[dut] < 20.627) sel  = Form("h_sig_ch%i_20624_20627",ch);
	else if ( x_dut[dut] > 20.599 && x_dut[dut] < 20.601) sel  = Form("h_sig_ch%i_20599_20601",ch);
	else if ( x_dut[dut] > 20.574 && x_dut[dut] < 20.576) sel  = Form("h_sig_ch%i_20574_20576",ch);
	else if ( x_dut[dut] > 20.549 && x_dut[dut] < 20.551) sel  = Form("h_sig_ch%i_20549_20551",ch);
	else if ( x_dut[dut] > 20.524 && x_dut[dut] < 20.527) sel  = Form("h_sig_ch%i_20524_20527",ch);
	else if ( x_dut[dut] > 20.499 && x_dut[dut] < 20.501) sel  = Form("h_sig_ch%i_20499_20501",ch);
	else if ( x_dut[dut] > 20.474 && x_dut[dut] < 20.576) sel  = Form("h_sig_ch%i_20474_20576",ch);
	else if ( x_dut[dut] > 20.449 && x_dut[dut] < 20.451) sel  = Form("h_sig_ch%i_20449_20451",ch);
	else return ;

	for (int i=0; i<1600; i++){

		plotter.Plot2D(sel, ";time [s];channel [mV]", time[0][i]-LP2_20[3], channel[ch][i], 1600, -2.26986e-07+210e-9, -1.87011e-07+210e-9, 250, -2000, 100);
	}
	plotter.Plot1D(Form("%s_amp"  ,sel.c_str()), "", amp[ch] 				    , 50, 0, 2000);
	plotter.Plot1D(Form("%s_tpeak",sel.c_str()), "", (t_peak[ch]-LP2_20[3])*1e9, 50, 3.2, 4.5);

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
		//if (ientry < 1000) break;
		if (ientry%10000==0) std::cout << "Event" << ientry << std::endl;

		// good track
		if (ntracks!=1) continue;
		if (nplanes<15) continue;
		if (npix <3) continue;
		if (nback<2) continue;
		
		if (abs(yResidBack) > 500) continue;
		if (abs(xResidBack) > 500) continue;

		// photek
		if (amp[3] < 50  ) continue;
		if (amp[3] > 250 ) continue;

		if (y_dut[dut] > 24.2) continue;
		if (y_dut[dut] < 22.8) continue;

		if (x_dut[dut] > 21.0) continue;
		if (x_dut[dut] < 20.0) continue;

		float dist;
		float tdiff;
		float tdiff2;

		dist = fabs(x_dut[dut] - 20.65)*1e3;
		tdiff = (t_peak[1]- LP2_20[3])*1e9;
		tdiff2 = (LP2_20[1]- LP2_20[3])*1e9;
		if (amp[1]> 110) plotter.Plot2D("h_ch1_amp_v_x"  ,";|x-x_{strip}| [#mum]; amplitude [mV]"            , dist, amp[1]                 , 30, 0, 300, 100, 0, 2000);
		if (amp[1]> 110) plotter.Plot2D("h_ch1_tpeak_v_x",";|x-x_{strip}| [#mum]; t_{peak} - t_{photek} [ns]", dist, tdiff , 30, 0, 300, 100, 3.2, 4.5);
		if (amp[1]> 110) plotter.Plot2D("h_ch1_tdiff_v_x",";|x-x_{strip}| [#mum]; t_{peak} - t_{photek} [ns]", dist, tdiff2, 30, 0, 300, 100, 2.5, 4.0);

		dist = fabs(x_dut[dut] - 20.55)*1e3;
		tdiff = (t_peak[0]- LP2_20[3])*1e9;
		tdiff2 = (LP2_20[0]- LP2_20[3])*1e9;
		if (amp[0]> 110) plotter.Plot2D("h_ch0_amp_v_x"  ,";|x-x_{strip}| [#mum]; amplitude [mV]"            , dist, amp[0]                  , 30, 0, 300, 100, 0, 2000);
		if (amp[2]> 110) plotter.Plot2D("h_ch0_tpeak_v_x",";|x-x_{strip}| [#mum]; t_{peak} - t_{photek} [ns]", dist, tdiff  , 30, 0, 300, 100, 3.2, 4.5);
		if (amp[2]> 110) plotter.Plot2D("h_ch0_tdiff_v_x",";|x-x_{strip}| [#mum]; t_{peak} - t_{photek} [ns]", dist, tdiff2 , 30, 0, 300, 100, 2.5, 4.0);

		dist = fabs(x_dut[dut] - 20.45)*1e3;
		tdiff = (t_peak[2]- LP2_20[3])*1e9;
		tdiff2 = (LP2_20[2]- LP2_20[3])*1e9;
		if (amp[2]> 110) plotter.Plot2D("h_ch2_amp_v_x"  ,";|x-x_{strip}| [#mum]; amplitude [mV]"            , dist, amp[2]                 , 30, 0, 300, 100, 0, 2000);
		if (amp[2]> 110) plotter.Plot2D("h_ch2_tpeak_v_x",";|x-x_{strip}| [#mum]; t_{peak} - t_{photek} [ns]", dist, tdiff , 30, 0, 300, 100, 3.2, 4.5);
		if (amp[2]> 110) plotter.Plot2D("h_ch2_tdiff_v_x",";|x-x_{strip}| [#mum]; t_{peak} - t_{photek} [ns]", dist, tdiff2, 30, 0, 300, 100, 2.5, 4.0);


		if (x_dut[dut] > 20.8) continue;
		if (x_dut[dut] < 20.3) continue;

		avg_waveform(cfg,1);
		//avg_waveform(cfg,2);

	}
   
}
int main(int argc, char* argv[]){

	// defaults 
	std::string cfg = "cfg_4_13_12";
	//std::string cfg = "cfg_4_13_DC";

	TFile *file = TFile::Open(Form("skims/%s.root",cfg.c_str()));
	TTree *tree = (TTree*)file->Get("pulse");

	gROOT->SetBatch();
    gStyle->SetOptStat(0);
    PlotHelper::setPlotStyle(0);

	// Do analysis 
	Analysis analysis(tree);
	analysis.Loop(cfg);


	// Save histogrmas here
	TFile *out = TFile::Open(Form("output/averages_%s.root",cfg.c_str()),"RECREATE");
	out->cd();
	plotter.DrawAll1D(c1);
	plotter.DrawAll2D(c1);
	
	return 0;
}