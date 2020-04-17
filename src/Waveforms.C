#define Waveforms_cxx
#include "Waveforms.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void Analysis::print_waveform(std::string cfg,std::string sel){

	//TH2F* hist1 = new TH2F(Form("%s_%i_ch0",cfg.c_str(),i_evt),";time [s];channel [mV]", 1600, -2.26986e-07, -1.87011e-07, 250, -150, 100);
	//TH2F* hist2 = new TH2F(Form("%s_%i_ch3",cfg.c_str(),i_evt),";time [s];channel [mV]", 1600, -2.26986e-07, -1.87011e-07, 250, -150, 100);
	//
	//for (int i=0; i<1600; i++){
	//	hist1->Fill(time[0][i], channel[0][i]);
	//	hist2->Fill(time[0][i], channel[3][i]);
	//}
	TGraph *graph0 = new TGraph(1600,time[0],channel[0]);
	TGraph *graph1 = new TGraph(1600,time[0],channel[1]);
	TGraph *graph2 = new TGraph(1600,time[0],channel[2]);
	TGraph *graph3 = new TGraph(1600,time[0],channel[3]);
	graph0->SetName(Form("%s_%s_%i_ch0",cfg.c_str(),sel.c_str(),i_evt));
	graph1->SetName(Form("%s_%s_%i_ch1",cfg.c_str(),sel.c_str(),i_evt));
	graph2->SetName(Form("%s_%s_%i_ch2",cfg.c_str(),sel.c_str(),i_evt));
	graph3->SetName(Form("%s_%s_%i_ch3",cfg.c_str(),sel.c_str(),i_evt));
	graph0->SetTitle("");
	graph1->SetTitle("");	
	graph2->SetTitle("");
	graph3->SetTitle("");

	c1->cd();
	c1->SetLeftMargin(0.2);
	c1->SetBottomMargin(0.21);
	c1->SetRightMargin(0.11);
	graph3->Draw("AL");
	graph3->SetLineColor(kBlack);
	graph3->SetLineWidth(2);
	graph0->Draw("Lsame");
	graph0->SetLineColor(kRed+1);
	graph0->SetLineWidth(2);
	if (sel != "low_amp"){
		graph1->Draw("Lsame");
		graph1->SetLineColor(kOrange+1);
		graph1->SetLineWidth(2);
		graph2->Draw("Lsame");
		graph2->SetLineColor(kBlue+1);
		graph2->SetLineWidth(2);
	}

	graph3->GetHistogram()->GetXaxis()->SetTitleSize(0.06);
	graph3->GetHistogram()->GetYaxis()->SetTitleSize(0.06);
	graph3->GetHistogram()->GetXaxis()->SetLabelSize(0.05);
	graph3->GetHistogram()->GetYaxis()->SetLabelSize(0.05);

	graph3->GetHistogram()->GetXaxis()->SetNdivisions(505);
	graph3->GetHistogram()->GetYaxis()->SetNdivisions(505);
	graph3->GetHistogram()->GetYaxis()->SetRangeUser( (sel == "three_hit") ? -1000 : -200,100);
	graph3->GetHistogram()->GetXaxis()->SetTitle("time [s]");
	graph3->GetHistogram()->GetYaxis()->SetTitleOffset((sel == "three_hit") ? 1.2 : 1.4 );
	graph3->GetHistogram()->GetYaxis()->SetTitle("signal [mV]");
	gPad->SetTicks();

	float min = (sel != "low_amp") ? 0.3 : 0.35; 
	float max = (sel != "low_amp") ? 0.5 : 0.45;

	TLegend *leg = new TLegend(0.22,min,0.5,max);
	leg->SetBorderSize(0);
	leg->AddEntry(graph0,"Channel 4");
	if (sel != "low_amp") leg->AddEntry(graph1,"Channel 13");
	if (sel != "low_amp") leg->AddEntry(graph2,(sel=="dc_high" || sel=="dc_other") ? "DC Pad" : "Channel 12");
	leg->AddEntry(graph3,"Photek");
	leg->Draw();

	c1->Print(Form("plots/waveforms/%s_event_%i.png",sel.c_str(),i_evt));
	c1->Clear();


}
void Analysis::Loop(std::string cfg)
{

	int n_saved_low = 0;
	int n_saved_three = 0;
	int n_saved_dc_high = 0;
	int n_saved_dc_other = 0;
	if (fChain == 0) return;

	Long64_t nentries = fChain->GetEntriesFast();

	Long64_t nbytes = 0, nb = 0;
	for (Long64_t jentry=0; jentry<nentries;jentry++) {
		Long64_t ientry = LoadTree(jentry);
		if (ientry < 0) break;
		nb = fChain->GetEntry(jentry);   nbytes += nb;
		

		// for debugging
		if ( (n_saved_low > 20 && n_saved_three > 20 ) || ( n_saved_dc_high > 20 && n_saved_dc_other > 100 ) ) break;
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

		if (cfg == "cfg_4_13_12"){
 			// in x,y position
			if (x_dut[dut] < xmin) continue;
			if (y_dut[dut] < ymin) continue;
			if (x_dut[dut] > xmax) continue;
			if (y_dut[dut] > ymax) continue;
	
			if (amp[0] > 100 && amp[0]<110 && x_dut[dut] < 20.6 && x_dut[dut] > 20.5 && n_saved_low < 30){
	
				print_waveform(cfg,"low_amp");
				n_saved_low+=1;
			}
			if (amp[0] > 200 && amp[1] > 110 &&  amp[2] > 110 && n_saved_three < 100){
	
				print_waveform(cfg,"three_hit");
				n_saved_three+=1;
			}			
		}
		else if (cfg == "cfg_4_13_DC"){

			if (x_dut[dut] < 19) continue;
			if (y_dut[dut] < 22) continue;
			if (x_dut[dut] > 22) continue;
			if (y_dut[dut] > 25) continue;

			if (amp[2] > 30 && n_saved_dc_high < 30){

				print_waveform(cfg,"dc_high");
				n_saved_dc_high +=1;

			}
			if ( (amp[0] > 30 || amp[1] > 30 ) && amp[2]> 11 && n_saved_dc_other < 100 ){
				print_waveform(cfg,"dc_other");
				n_saved_dc_other+=1;
			}


		}
		else break;


	  
	}
   
}
int main(int argc, char* argv[]){

	// defaults 
	//std::string cfg = "cfg_4_13_12";
	std::string cfg = "cfg_4_13_DC";

	TFile *file = TFile::Open(Form("skims/%s.root",cfg.c_str()));
	TTree *tree = (TTree*)file->Get("pulse");

	gROOT->SetBatch();
    gStyle->SetOptStat(0);
    PlotHelper::setPlotStyle(0);

	// Do analysis 
	Analysis analysis(tree);
	analysis.Loop(cfg);


	// Save histogrmas here
	//TFile *out = TFile::Open(Form("output/hists_%s.root",cfg.c_str()),"RECREATE");
	//out->cd();
	//plotter.DrawAll1D(c1);
	//plotter.DrawAll2D(c1);
	
	return 0;
}