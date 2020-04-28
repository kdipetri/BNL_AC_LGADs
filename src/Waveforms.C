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
void clean2D(TH2F *hist){
	hist->GetXaxis()->SetTitleSize(0.06);
	hist->GetYaxis()->SetTitleSize(0.06);
	hist->GetZaxis()->SetTitleSize(0.06);
	hist->GetXaxis()->SetLabelSize(0.05);
	hist->GetYaxis()->SetLabelSize(0.05);
	hist->GetZaxis()->SetLabelSize(0.05);
	hist->GetXaxis()->SetTitleOffset(1.0);
	hist->GetYaxis()->SetTitleOffset(1.3);
	hist->GetZaxis()->SetTitleOffset(1.0);
	hist->GetXaxis()->SetNdivisions(505);
	hist->GetYaxis()->SetNdivisions(505);
}
void clean1D(TH1F *hist){
	hist->GetXaxis()->SetTitleSize(0.06);
	hist->GetYaxis()->SetTitleSize(0.06);
	hist->GetXaxis()->SetLabelSize(0.05);
	hist->GetYaxis()->SetLabelSize(0.05);
	hist->GetXaxis()->SetTitleOffset(1.0);
	hist->GetYaxis()->SetTitleOffset(1.3);
	hist->GetXaxis()->SetNdivisions(505);
	hist->GetYaxis()->SetNdivisions(505);
	hist->SetLineWidth(2);
}
void Analysis::Loop(std::string cfg)
{

	TH2F *h_dc_pos_high = new TH2F("h_dc_pos_high",";x [mm];y [mm];Events", 60, 19, 22, 40, 22, 25);
	TH2F *h_dc_pos_low  = new TH2F("h_dc_pos_low" ,";x [mm];y [mm];Events", 60, 19, 22, 40, 22, 25);

	TH1F *h_dc_amp_strip = new TH1F("h_dc_amp_strip",";Amplitude [mV];Events", 20,0,100);
	TH1F *h_dc_amp_dc    = new TH1F("h_dc_amp_dc"   ,";Amplitude [mV];Events", 20,0,100);

	TH1F *h_dc_charge    = new TH1F("h_dc_charge"   ,";Collected Charge [fC];Events", 25,0,50);

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
		//if ( (n_saved_low > 20 && n_saved_three > 20 ) || ( n_saved_dc_high > 20 && n_saved_dc_other > 100 ) ) break;
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

			if (amp[2] > 30 ) {
				h_dc_pos_high->Fill(x_dut[dut],y_dut[dut]);

				if ( n_saved_dc_high < 30){
					print_waveform(cfg,"dc_high");
					n_saved_dc_high +=1;
				}	

			}
			if ( (amp[0] > 30 || amp[1] > 30 ) && amp[2]> 11 ){
				h_dc_pos_low->Fill(x_dut[dut],y_dut[dut]);

				if ( n_saved_dc_other < 100 ){
					print_waveform(cfg,"dc_other");
					n_saved_dc_other+=1;
				}
			}

			if (x_dut[dut] > 19.8 && x_dut[dut] < 21.4){// center on strips 4,13
				if (y_dut[dut] < 22.85 && y_dut[dut] > 22.68 && amp[2] > 11 ) h_dc_amp_strip->Fill(amp[2]);
				if (y_dut[dut] < 22.68 && y_dut[dut] > 22.5  && amp[2] > 11 ) h_dc_amp_dc   ->Fill(amp[2]);

			}

			if (amp[2] > 30 ) h_dc_charge->Fill(-1000*integral[2]*1e9*50/4700);


		}
		else break;


	  
	}
	c1->cd();
	c1->SetLeftMargin(0.15);
	c1->SetBottomMargin(0.2);
	c1->SetRightMargin(0.2);

	clean2D(h_dc_pos_high);
	h_dc_pos_high->Draw("COLZ");
   	c1->Print(Form("plots/dc_pad/pos_dc_high.png"));
	
	clean2D(h_dc_pos_low);
	h_dc_pos_low->Draw("COLZ");
   	c1->Print(Form("plots/dc_pad/pos_dc_low.png"));


	c1->SetLeftMargin(0.18);
	c1->SetBottomMargin(0.18);
	c1->SetRightMargin(0.05);
	c1->SetTopMargin(0.05);
	clean1D(h_dc_amp_strip);
	clean1D(h_dc_amp_strip);
   	h_dc_amp_strip->SetLineColor(kRed);
	h_dc_amp_dc   ->SetLineColor(kBlue);  
	h_dc_amp_strip->Draw("hist");
	h_dc_amp_dc->Draw("hist same");
   	c1->Print(Form("plots/dc_pad/amplitude_compare.png"));

   	
   	clean1D(h_dc_charge);
   	
   	h_dc_charge->SetLineColor(kBlack);
   	h_dc_charge->Draw("hist");
   	h_dc_charge->GetYaxis()->SetRangeUser(0,h_dc_charge->GetMaximum()*1.3);

   	TF1 *f1 = new TF1("f1","landau",0,50);
   	h_dc_charge->Fit(f1);
   	f1->Draw("same");
   	std::cout << "MPV :" << f1->GetParameter(1) << std::endl;
   	c1->Print(Form("plots/dc_pad/dc_charge.png"));

	c1->Clear();


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