#define timeres


TFile *file_t0corr = TFile::Open("profiles/tres_corr_safe.root");
TH1F *hist_t0corr_ch0 = (TH1F*)file_t0corr->Get(Form("profxtimeres_ch0_deltaTun_v_amp"));
TH1F *hist_t0corr_ch1 = (TH1F*)file_t0corr->Get(Form("profxtimeres_ch1_deltaTun_v_amp"));
TH1F *hist_t0corr_ch2 = (TH1F*)file_t0corr->Get(Form("profxtimeres_ch2_deltaTun_v_amp"));

float t0_corr(std::string cfg, int ch, float amp)
{
	float t0 = -99; // in ns
	if ("cfg_4_13_12" == cfg) 
	{
		//if      (ch==0) t0 = 3.293;//4
		//else if (ch==1) t0 = 3.333;//13
		//else if (ch==2) t0 = 3.346;// 12

		if      (ch==0) t0 = hist_t0corr_ch0->GetBinContent(hist_t0corr_ch0->FindBin(amp));
		else if (ch==1) t0 = hist_t0corr_ch1->GetBinContent(hist_t0corr_ch1->FindBin(amp));
		else if (ch==2) t0 = hist_t0corr_ch2->GetBinContent(hist_t0corr_ch2->FindBin(amp));	
		
	}
	else if ("cfg_6_5_11" == cfg )
	{
		if      (ch==0) t0 = 3.232;//6
		else if (ch==1) t0 = 3.320;// 5
		else if (ch==2) t0 = 3.284;// 11	
	}
	return t0;
}
void Analysis::make_t0corr(std::string cfg, int ch)
{
	if (LP2_20[ch] == 0) return ;
	if (LP2_20[3] == 0) return ;
	plotter.Plot2D(Form("timeres_ch%i_deltaTun_v_amp",ch), "; amplitude [mV];#Delta t uncorr [ns]", amp[ch],(LP2_20[ch] - LP2_20[3])*1e9, 100, 0, 2000, 100, 2.0,4.5);
	return ;
}
void Analysis::time_res(std::string cfg, int ch1,int ch2,int ch3=-1)
{

	float min_T = -0.5;
	float max_T = 0.5;

	float min_X = 20.3;
	float max_X = 20.8;

	if (LP2_20[ch1] == 0) return;
	if (LP2_20[ch2] == 0) return;
	if (LP2_20[3] == 0) return;
	//std::cout << cf << " " << diff << " " << xpos << " " <<  x_dut[dut] << std::endl;
	if (ch3 == -1){
		
		int ch_1 = amp[ch1]>amp[ch2] ? ch1 : ch2;
		int ch_2 = amp[ch1]>amp[ch2] ? ch2 : ch1;

		float t01 = t0_corr(cfg,ch_1,amp[ch_1]);
		float t02 = t0_corr(cfg,ch_2,amp[ch_2]);

		// slew rate
		plotter.Plot1D(Form("timeres_twohits_%i_%i_ch%i_slewrate" ,ch_1,ch_2,ch_1), ";slewrate [mV/ns]", abs(risetime[ch_1])/1e9, 50, 0, 2000);
		plotter.Plot1D(Form("timeres_twohits_%i_%i_ch%i_slewrate" ,ch_1,ch_2,ch_2), ";slewrate [mV/ns]", abs(risetime[ch_2])/1e9, 50, 0, 2000);

		// rms noise
		plotter.Plot1D(Form("timeres_twohits_%i_%i_ch%i_rmsnoise" ,ch_1,ch_2,ch_1), ";baseline [mV]", baseline_RMS[ch_1], 50, 0, 50);
		plotter.Plot1D(Form("timeres_twohits_%i_%i_ch%i_rmsnoise" ,ch_1,ch_2,ch_2), ";baseline [mV]", baseline_RMS[ch_2], 50, 0, 50);

		plotter.Plot1D(Form("timeres_twohits_%i_%i_ch%i_deltaTun" ,ch_1,ch_2,ch_1), ";#Delta T uncorr [ns]", (LP2_20[ch_1] - LP2_20[3])*1e9, 100, t01+min_T, t01+max_T);
		plotter.Plot1D(Form("timeres_twohits_%i_%i_ch%i_deltaTun" ,ch_1,ch_2,ch_2), ";#Delta T uncorr [ns]", (LP2_20[ch_2] - LP2_20[3])*1e9, 100, t02+min_T, t02+max_T);
		plotter.Plot1D(Form("timeres_twohits_%i_%i_ch%i_deltaTcor",ch_1,ch_2,ch_1), ";#Delta T corr [ns]"  , (LP2_20[ch_1] - LP2_20[3])*1e9 - t01, 100, min_T, max_T);
		plotter.Plot1D(Form("timeres_twohits_%i_%i_ch%i_deltaTcor",ch_1,ch_2,ch_2), ";#Delta T corr [ns]"  , (LP2_20[ch_2] - LP2_20[3])*1e9 - t02, 100, min_T, max_T);

		plotter.Plot1D(Form("timeres_twohits_%i_%i_xpos"      ,ch_1,ch_2),      ";x [mm]"        , x_dut[dut], 100, min_X, max_X);
		plotter.Plot1D(Form("timeres_twohits_%i_%i_ch%i_amp"  ,ch_1,ch_2,ch_1), ";Amplitude [mV]", amp[ch_1] , 100, 0, 1800);
		plotter.Plot1D(Form("timeres_twohits_%i_%i_ch%i_amp"  ,ch_1,ch_2,ch_2), ";Amplitude [mV]", amp[ch_2] , 100, 0, 1800);

		// leading hit 
		float tlead = (LP2_20[ch_1] - LP2_20[3])*1e9 - t01;

		// weighted time res! 
		float tcomb = (  (LP2_20[ch_1]-LP2_20[3]-t01*1e-9)*amp[ch_1] + (LP2_20[ch_2]-LP2_20[3]-t02*1e-9)*amp[ch_2] )/( amp[ch_1]+amp[ch_2] )*1e9 ;

		plotter.Plot1D(Form("timeres_twohits_%i_%i_deltaTlead",ch_1,ch_2), ";#Delta T corr [ns]"  , tlead, 100, min_T, max_T);
		plotter.Plot1D(Form("timeres_twohits_%i_%i_deltaTcomb",ch_1,ch_2), ";#Delta T corr [ns]"  , tcomb, 100, min_T, max_T);

		// any two hits
		plotter.Plot1D(Form("timeres_twohits_deltaTlead"), ";#Delta T corr [ns]"  , tlead, 100, min_T, max_T);
		plotter.Plot1D(Form("timeres_twohits_deltaTcomb"), ";#Delta T corr [ns]"  , tcomb, 100, min_T, max_T);
		plotter.Plot1D(Form("timeres_twohits_xpos"      ), ";x [mm]"              , x_dut[dut], 100, min_X, max_X);

	}
	else if ( ch3 >= 0 && LP2_20[ch3] != 0){
		int ch_1;
		int ch_2;
		int ch_3;

		if (amp[ch1] > amp[ch2] && amp[ch1] > amp[ch3] && amp[ch2] > amp[ch3]){ ch_1 = ch1; ch_2 = ch2; ch_3 = ch3; }
		if (amp[ch1] > amp[ch2] && amp[ch1] > amp[ch3] && amp[ch3] > amp[ch2]){ ch_1 = ch1; ch_2 = ch3; ch_3 = ch2; }
		if (amp[ch2] > amp[ch1] && amp[ch2] > amp[ch3] && amp[ch1] > amp[ch3]){ ch_1 = ch2; ch_2 = ch1; ch_3 = ch3; } //
		if (amp[ch2] > amp[ch1] && amp[ch2] > amp[ch3] && amp[ch3] > amp[ch1]){ ch_1 = ch2; ch_2 = ch3; ch_3 = ch1; } //
		if (amp[ch3] > amp[ch1] && amp[ch3] > amp[ch2] && amp[ch1] > amp[ch2]){ ch_1 = ch3; ch_2 = ch1; ch_3 = ch2; }
		if (amp[ch3] > amp[ch1] && amp[ch3] > amp[ch2] && amp[ch2] > amp[ch1]){ ch_1 = ch3; ch_2 = ch2; ch_3 = ch1; }


		float t01 = t0_corr(cfg,ch_1,amp[ch_1]); // in ns
		float t02 = t0_corr(cfg,ch_2,amp[ch_2]); // in ns
		float t03 = t0_corr(cfg,ch_3,amp[ch_3]); // in ns

		// slew rate
		plotter.Plot1D(Form("timeres_threehits_%i_%i_%i_ch%i_slewrate" ,ch_1,ch_2,ch_3,ch_1), ";slewrate [mV/ns]", abs(risetime[ch_1])/1e9, 50, 0, 2000);
		plotter.Plot1D(Form("timeres_threehits_%i_%i_%i_ch%i_slewrate" ,ch_1,ch_2,ch_3,ch_2), ";slewrate [mV/ns]", abs(risetime[ch_2])/1e9, 50, 0, 2000);
		plotter.Plot1D(Form("timeres_threehits_%i_%i_%i_ch%i_slewrate" ,ch_1,ch_2,ch_3,ch_3), ";slewrate [mV/ns]", abs(risetime[ch_3])/1e9, 50, 0, 2000);

		// rms noise
		plotter.Plot1D(Form("timeres_threehits_%i_%i_%i_ch%i_rmsnoise" ,ch_1,ch_2,ch_3,ch_1), ";baseline [mV]", baseline_RMS[ch_1], 50, 0, 50);
		plotter.Plot1D(Form("timeres_threehits_%i_%i_%i_ch%i_rmsnoise" ,ch_1,ch_2,ch_3,ch_2), ";baseline [mV]", baseline_RMS[ch_2], 50, 0, 50);
		plotter.Plot1D(Form("timeres_threehits_%i_%i_%i_ch%i_rmsnoise" ,ch_1,ch_2,ch_3,ch_3), ";baseline [mV]", baseline_RMS[ch_3], 50, 0, 50);

		//plotter.Plot1D(Form("timeres_threehits_%i_%i_%i_ch%i_rmsnoise" ,ch_1,ch_2,ch_3,ch_1), ";#Delta T uncorr [ns]", (LP2_20[ch_1] - LP2_20[3])*1e9, 100, t01+min_T, t01+max_T);
		//plotter.Plot1D(Form("timeres_threehits_%i_%i_%i_ch%i_rmsnoise" ,ch_1,ch_2,ch_3,ch_2), ";#Delta T uncorr [ns]", (LP2_20[ch_2] - LP2_20[3])*1e9, 100, t02+min_T, t02+max_T);
		//plotter.Plot1D(Form("timeres_threehits_%i_%i_%i_ch%i_rmsnoise" ,ch_1,ch_2,ch_3,ch_3), ";#Delta T uncorr [ns]", (LP2_20[ch_3] - LP2_20[3])*1e9, 100, t03+min_T, t03+max_T);

		// all hits amp ordered
		plotter.Plot1D(Form("timeres_threehits_%i_%i_%i_ch%i_deltaTun" ,ch_1,ch_2,ch_3,ch_1), ";#Delta T uncorr [ns]", (LP2_20[ch_1] - LP2_20[3])*1e9, 100, t01+min_T, t01+max_T);
		plotter.Plot1D(Form("timeres_threehits_%i_%i_%i_ch%i_deltaTun" ,ch_1,ch_2,ch_3,ch_2), ";#Delta T uncorr [ns]", (LP2_20[ch_2] - LP2_20[3])*1e9, 100, t02+min_T, t02+max_T);
		plotter.Plot1D(Form("timeres_threehits_%i_%i_%i_ch%i_deltaTun" ,ch_1,ch_2,ch_3,ch_3), ";#Delta T uncorr [ns]", (LP2_20[ch_3] - LP2_20[3])*1e9, 100, t03+min_T, t03+max_T);
		plotter.Plot1D(Form("timeres_threehits_%i_%i_%i_ch%i_deltaTcor",ch_1,ch_2,ch_3,ch_1), ";#Delta T corr [ns]"  , (LP2_20[ch_1] - LP2_20[3])*1e9 - t01, 100, min_T, max_T);
		plotter.Plot1D(Form("timeres_threehits_%i_%i_%i_ch%i_deltaTcor",ch_1,ch_2,ch_3,ch_2), ";#Delta T corr [ns]"  , (LP2_20[ch_2] - LP2_20[3])*1e9 - t02, 100, min_T, max_T);
		plotter.Plot1D(Form("timeres_threehits_%i_%i_%i_ch%i_deltaTcor",ch_1,ch_2,ch_3,ch_3), ";#Delta T corr [ns]"  , (LP2_20[ch_3] - LP2_20[3])*1e9 - t03, 100, min_T, max_T);

		plotter.Plot1D(Form("timeres_threehits_%i_%i_%i_xpos"      ,ch_1,ch_2,ch_3),      ";x [mm]"        , x_dut[dut], 100, min_X, max_X);
		plotter.Plot1D(Form("timeres_threehits_%i_%i_%i_ch%i_amp"  ,ch_1,ch_2,ch_3,ch_1), ";Amplitude [mV]", amp[ch_1] , 100, 0, 1800);
		plotter.Plot1D(Form("timeres_threehits_%i_%i_%i_ch%i_amp"  ,ch_1,ch_2,ch_3,ch_2), ";Amplitude [mV]", amp[ch_2] , 100, 0, 1800);
		plotter.Plot1D(Form("timeres_threehits_%i_%i_%i_ch%i_amp"  ,ch_1,ch_2,ch_3,ch_3), ";Amplitude [mV]", amp[ch_3] , 100, 0, 1800);

		// leading hit 
		float tlead = (LP2_20[ch_1] - LP2_20[3])*1e9 - t01;

		// weighted time res! 
		float tcomb = (  (LP2_20[ch_1]-LP2_20[3]-t01*1e-9)*amp[ch_1] + (LP2_20[ch_2]-LP2_20[3]-t02*1e-9)*amp[ch_2] + (LP2_20[ch_3]-LP2_20[3]-t03*1e-9)*amp[ch_3])/( amp[ch_1]+amp[ch_2]+amp[ch_3] )*1e9 ;

		
		plotter.Plot1D(Form("timeres_threehits_%i_%i_%i_deltaTlead",ch_1,ch_2,ch_3), ";#Delta T corr [ns]"  , tlead, 100, min_T, max_T);
		plotter.Plot1D(Form("timeres_threehits_%i_%i_%i_deltaTcomb",ch_1,ch_2,ch_3), ";#Delta T corr [ns]"  , tcomb, 100, min_T, max_T);

		// just max amp
		plotter.Plot1D(Form("timeres_threehits_%i_deltaTlead",ch_1), ";#Delta T corr [ns]"  , tlead, 100, min_T, max_T);
		plotter.Plot1D(Form("timeres_threehits_%i_deltaTcomb",ch_1), ";#Delta T corr [ns]"  , tcomb, 100, min_T, max_T);
		plotter.Plot1D(Form("timeres_threehits_%i_xpos"      ,ch_1), ";x [mm]"        , x_dut[dut], 100, min_X, max_X);

		// all three hits
		plotter.Plot1D(Form("timeres_threehits_deltaTlead"), ";#Delta T corr [ns]"  , tlead, 100, min_T, max_T);
		plotter.Plot1D(Form("timeres_threehits_deltaTcomb"), ";#Delta T corr [ns]"  , tcomb, 100, min_T, max_T);
		plotter.Plot1D(Form("timeres_threehits_xpos"      ), ";x [mm]"              , x_dut[dut], 100, min_X, max_X);

	}

}