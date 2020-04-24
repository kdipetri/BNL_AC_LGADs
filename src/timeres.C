#define timeres


TFile *file_t0corr2 = TFile::Open("profiles/tres_corr2_safe.root");
TFile *file_t0corr = TFile::Open("profiles/tres_corr_safe.root");
TH1F *hist_t0corr_ch0 = (TH1F*)file_t0corr->Get(Form("profxtimeres_ch0_deltaTun_v_amp"));
TH1F *hist_t0corr_ch1 = (TH1F*)file_t0corr->Get(Form("profxtimeres_ch1_deltaTun_v_amp"));
TH1F *hist_t0corr_ch2 = (TH1F*)file_t0corr->Get(Form("profxtimeres_ch2_deltaTun_v_amp"));

//TH1F *hist_t0corr_ch0 = (TH1F*)file_t0corr->Get(Form("profxtimeres_ch0_deltaTun_v_xpos"));
//TH1F *hist_t0corr_ch1 = (TH1F*)file_t0corr->Get(Form("profxtimeres_ch1_deltaTun_v_xpos"));
//TH1F *hist_t0corr_ch2 = (TH1F*)file_t0corr->Get(Form("profxtimeres_ch2_deltaTun_v_xpos"));

//TH1F *hist_t0corr_ch0_x0 = (TH1F*)file_t0corr->Get(Form("profxtimeres_ch0_deltaTun_v_amp_x0"));
//TH1F *hist_t0corr_ch0_x1 = (TH1F*)file_t0corr->Get(Form("profxtimeres_ch0_deltaTun_v_amp_x1"));
//TH1F *hist_t0corr_ch0_x2 = (TH1F*)file_t0corr->Get(Form("profxtimeres_ch0_deltaTun_v_amp_x2"));
//TH1F *hist_t0corr_ch0_x3 = (TH1F*)file_t0corr->Get(Form("profxtimeres_ch0_deltaTun_v_amp_x3"));
//TH1F *hist_t0corr_ch0_x4 = (TH1F*)file_t0corr->Get(Form("profxtimeres_ch0_deltaTun_v_amp_x4"));
//
//TH1F *hist_t0corr_ch1_x0 = (TH1F*)file_t0corr->Get(Form("profxtimeres_ch1_deltaTun_v_amp_x0"));
//TH1F *hist_t0corr_ch1_x1 = (TH1F*)file_t0corr->Get(Form("profxtimeres_ch1_deltaTun_v_amp_x1"));
//TH1F *hist_t0corr_ch1_x2 = (TH1F*)file_t0corr->Get(Form("profxtimeres_ch1_deltaTun_v_amp_x2"));
//TH1F *hist_t0corr_ch1_x3 = (TH1F*)file_t0corr->Get(Form("profxtimeres_ch1_deltaTun_v_amp_x3"));
//TH1F *hist_t0corr_ch1_x4 = (TH1F*)file_t0corr->Get(Form("profxtimeres_ch1_deltaTun_v_amp_x4"));
//
//TH1F *hist_t0corr_ch2_x0 = (TH1F*)file_t0corr->Get(Form("profxtimeres_ch2_deltaTun_v_amp_x0"));
//TH1F *hist_t0corr_ch2_x1 = (TH1F*)file_t0corr->Get(Form("profxtimeres_ch2_deltaTun_v_amp_x1"));
//TH1F *hist_t0corr_ch2_x2 = (TH1F*)file_t0corr->Get(Form("profxtimeres_ch2_deltaTun_v_amp_x2"));
//TH1F *hist_t0corr_ch2_x3 = (TH1F*)file_t0corr->Get(Form("profxtimeres_ch2_deltaTun_v_amp_x3"));
//TH1F *hist_t0corr_ch2_x4 = (TH1F*)file_t0corr->Get(Form("profxtimeres_ch2_deltaTun_v_amp_x4"));



//float t0_corr(std::string cfg, int ch, float amp)
float t0_corr(std::string cfg, int ch, float amp, float xpos)
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

		//if      (ch==0) t0 = fit_t0corr_ch0->Eval(xpos);
		//else if (ch==1) t0 = fit_t0corr_ch1->Eval(xpos);
		//else if (ch==2) t0 = fit_t0corr_ch2->Eval(xpos);	
		
	}
	else if ("cfg_6_5_11" == cfg )
	{
		if      (ch==0) t0 = 3.232;//6
		else if (ch==1) t0 = 3.320;// 5
		else if (ch==2) t0 = 3.284;// 11	
	}
	return t0;
}
float two_strip_corr(std::string cfg, int ch1, int ch2, int ch)
{
	TF1 *f1 = (TF1*)file_t0corr2->Get(Form("f1_timeres_twohits_%i_%i_ch%i_deltaTcor",ch1,ch2,ch));
	float t0 = f1->GetParameter(1); 
	return t0;
}
float three_strip_corr(std::string cfg, int ch1, int ch2, int ch3, int ch)
{

	TF1 *f1 = (TF1*)file_t0corr2->Get(Form("f1_timeres_threehits_%i_%i_%i_ch%i_deltaTcor",ch1,ch2,ch3,ch));
	float t0 = f1->GetParameter(1); 
	return t0;
}
void Analysis::make_t0corr(std::string cfg, int ch)
{

	float xcenter = 20.55;
	if (ch==1) xcenter = 20.65;
	else if (ch==2) xcenter = 20.45;

	float xdiff = abs(x_dut[dut]-xcenter)*1e3;

	plotter.Plot2D(Form("timeres_ch%i_amp_v_xdiff"     ,ch), ";x [#mum];amplitude [mV]"				, xdiff , amp[ch] 					    , 40,0,200, 100, 0, 2000);
	plotter.Plot2D(Form("timeres_ch%i_amp_v_xpos"      ,ch),";x [mm];amplitude [mV]"			   , x_dut[dut] , amp[ch]   				   , 50 , 20.3, 20.8  , 100, 0, 2000 );

	if ( amp[ch] < 110) return;
	if ( LP2_20[ch] == 0) return ;
	if ( LP2_20[3]  == 0) return ;
	float tdiff = (LP2_20[ch] - LP2_20[3])*1e9; 
	if ( tdiff < 2.0 ) return;
	if ( tdiff > 4.5 ) return;

	plotter.Plot2D(Form("timeres_ch%i_tpeak_v_xdiff"   ,ch), ";x [#mum];t_{peak}-t_{photek} [ns]" 	, xdiff , (t_peak[ch]-LP2_20[3])*1e9 	, 40,0,200, 100, 3.2,4.5);
	plotter.Plot2D(Form("timeres_ch%i_risetime_v_xdiff",ch), ";x [#mum];risetime [ns]" 				, xdiff , 1e9*abs(amp[ch]/risetime[ch]) , 40,0,200, 100,  0.2,1.2);

	plotter.Plot2D(Form("timeres_ch%i_deltaTpeakun_v_xpos",ch),";x [mm];t_{peak}-t_{photek}[ns]"   , x_dut[dut], (t_peak[ch]-LP2_20[3])*1e9    , 20 , 20.3, 20.8  , 100, 3.2,4.5);
	plotter.Plot2D(Form("timeres_ch%i_deltaTun_v_xpos"    ,ch),";x [mm];t_{CFD}-t_{photek}[ns]"    , x_dut[dut], tdiff 						   , 20 , 20.3, 20.8  , 100, 2.5,4.0);
	plotter.Plot2D(Form("timeres_ch%i_risetime_v_xpos"    ,ch),";x [mm];risetime [ns]"		  	   , x_dut[dut], 1e9*abs(amp[ch]/risetime[ch]) , 20 , 20.3, 20.8  , 100, 0.1,1.2);

	plotter.Plot2D(Form("timeres_ch%i_deltaTpeakun_v_amp",ch), ";amplitude [mV];t_{peak}-t_{photek}[ns]"   , amp[ch]   , (t_peak[ch]-LP2_20[3])*1e9 	, 100, 0, 2000 , 100, 3.2,4.5);
	plotter.Plot2D(Form("timeres_ch%i_deltaTun_v_amp"    ,ch), ";amplitude [mV];t_{CFD}-t_{photek}[ns]"    , amp[ch]   , tdiff 							, 100, 0, 2000 , 100, 2.5,4.0);
	plotter.Plot2D(Form("timeres_ch%i_risetime_v_amp"    ,ch), ";amplitude [mV];risetime [ns]"		  	   , amp[ch]   , 1e9*abs(amp[ch]/risetime[ch]) 	, 100, 0, 2000 , 100, 0.1,1.2);
	
	if (x_dut[dut] > 20.3 && x_dut[dut] < 20.4 ) plotter.Plot2D(Form("timeres_ch%i_deltaTun_v_amp_x0",ch), ";amplitude [mV];#Delta t uncorr [ns]"   , amp[ch], tdiff, 20, 0, 2000 ,50, 2.5,4.0);
	if (x_dut[dut] > 20.4 && x_dut[dut] < 20.5 ) plotter.Plot2D(Form("timeres_ch%i_deltaTun_v_amp_x1",ch), ";amplitude [mV];#Delta t uncorr [ns]"   , amp[ch], tdiff, 50, 0, 2000 ,50, 2.5,4.0);
	if (x_dut[dut] > 20.5 && x_dut[dut] < 20.6 ) plotter.Plot2D(Form("timeres_ch%i_deltaTun_v_amp_x2",ch), ";amplitude [mV];#Delta t uncorr [ns]"   , amp[ch], tdiff, 50, 0, 2000 ,50, 2.5,4.0);
	if (x_dut[dut] > 20.6 && x_dut[dut] < 20.7 ) plotter.Plot2D(Form("timeres_ch%i_deltaTun_v_amp_x3",ch), ";amplitude [mV];#Delta t uncorr [ns]"   , amp[ch], tdiff, 50, 0, 2000 ,50, 2.5,4.0);
	if (x_dut[dut] > 20.7 && x_dut[dut] < 20.8 ) plotter.Plot2D(Form("timeres_ch%i_deltaTun_v_amp_x4",ch), ";amplitude [mV];#Delta t uncorr [ns]"   , amp[ch], tdiff, 20, 0, 2000 ,50, 2.5,4.0);


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

		//float t01 = t0_corr(cfg,ch_1,amp[ch_1]);
		//float t02 = t0_corr(cfg,ch_2,amp[ch_2]);
		//float t01 = t0_corr(cfg,ch_1,x_dut[dut]);
		//float t02 = t0_corr(cfg,ch_2,x_dut[dut]);
		float t01 = t0_corr(cfg,ch_1,amp[ch_1],x_dut[dut]);
		float t02 = t0_corr(cfg,ch_2,amp[ch_1],x_dut[dut]);

		//float t01_two = two_strip_corr(cfg,ch_1,ch_2,ch_1);
		//float t02_two = two_strip_corr(cfg,ch_1,ch_2,ch_2);

		// slew rate
		plotter.Plot1D(Form("timeres_twohits_%i_%i_ch%i_slewrate" ,ch_1,ch_2,ch_1), ";slewrate [mV/ns]", abs(risetime[ch_1])/1e9, 50, 0, 2000);
		plotter.Plot1D(Form("timeres_twohits_%i_%i_ch%i_slewrate" ,ch_1,ch_2,ch_2), ";slewrate [mV/ns]", abs(risetime[ch_2])/1e9, 50, 0, 2000);

		plotter.Plot1D(Form("timeres_twohits_%i_%i_ch%i_risetime" ,ch_1,ch_2,ch_1), ";risetime [ns]", 1e9*abs(amp[ch_1]/risetime[ch_1]), 50, 0, 2);
		plotter.Plot1D(Form("timeres_twohits_%i_%i_ch%i_risetime" ,ch_1,ch_2,ch_2), ";risetime [ns]", 1e9*abs(amp[ch_2]/risetime[ch_2]), 50, 0, 2);

		// rms noise
		plotter.Plot1D(Form("timeres_twohits_%i_%i_ch%i_rmsnoise" ,ch_1,ch_2,ch_1), ";baseline [mV]", baseline_RMS[ch_1], 50, 0, 50);
		plotter.Plot1D(Form("timeres_twohits_%i_%i_ch%i_rmsnoise" ,ch_1,ch_2,ch_2), ";baseline [mV]", baseline_RMS[ch_2], 50, 0, 50);

		plotter.Plot1D(Form("timeres_twohits_%i_%i_ch%i_jitter" ,ch_1,ch_2,ch_1), ";jitter [ps]", 1e12 * baseline_RMS[ch_1]/abs(risetime[ch_1]), 100, 0, 30);
		plotter.Plot1D(Form("timeres_twohits_%i_%i_ch%i_jitter" ,ch_1,ch_2,ch_2), ";jitter [ps]", 1e12 * baseline_RMS[ch_2]/abs(risetime[ch_2]), 100, 0, 30);

		plotter.Plot1D(Form("timeres_twohits_%i_%i_ch%i_deltaTun" ,ch_1,ch_2,ch_1), ";#Delta T uncorr [ns]", (LP2_20[ch_1] - LP2_20[3])*1e9, 100, t01+min_T, t01+max_T);
		plotter.Plot1D(Form("timeres_twohits_%i_%i_ch%i_deltaTun" ,ch_1,ch_2,ch_2), ";#Delta T uncorr [ns]", (LP2_20[ch_2] - LP2_20[3])*1e9, 100, t02+min_T, t02+max_T);
		plotter.Plot1D(Form("timeres_twohits_%i_%i_ch%i_deltaTcor",ch_1,ch_2,ch_1), ";#Delta T corr [ns]"  , (LP2_20[ch_1] - LP2_20[3])*1e9 - t01, 100, min_T, max_T);
		plotter.Plot1D(Form("timeres_twohits_%i_%i_ch%i_deltaTcor",ch_1,ch_2,ch_2), ";#Delta T corr [ns]"  , (LP2_20[ch_2] - LP2_20[3])*1e9 - t02, 100, min_T, max_T);
		//plotter.Plot1D(Form("timeres_twohits_%i_%i_ch%i_deltaTcor2",ch_1,ch_2,ch_1), ";#Delta T corr [ns]"  , (LP2_20[ch_1] - LP2_20[3])*1e9 - t01 - t01_two, 100, min_T, max_T);
		//plotter.Plot1D(Form("timeres_twohits_%i_%i_ch%i_deltaTcor2",ch_1,ch_2,ch_2), ";#Delta T corr [ns]"  , (LP2_20[ch_2] - LP2_20[3])*1e9 - t02 - t02_two, 100, min_T, max_T);

		plotter.Plot1D(Form("timeres_twohits_%i_%i_xpos"      ,ch_1,ch_2),      ";x [mm]"        , x_dut[dut], 100, min_X, max_X);
		plotter.Plot1D(Form("timeres_twohits_%i_%i_ch%i_amp"  ,ch_1,ch_2,ch_1), ";Amplitude [mV]", amp[ch_1] , 100, 0, 1800);
		plotter.Plot1D(Form("timeres_twohits_%i_%i_ch%i_amp"  ,ch_1,ch_2,ch_2), ";Amplitude [mV]", amp[ch_2] , 100, 0, 1800);

		// leading hit 
		float tlead = (LP2_20[ch_1] - LP2_20[3])*1e9 - t01;
		//float tlead2 = (LP2_20[ch_1] - LP2_20[3])*1e9 - t01 - t01_two;

		// weighted time res! 
		float tcomb  = (  (LP2_20[ch_1]-LP2_20[3]-t01*1e-9             )*amp[ch_1] + (LP2_20[ch_2]-LP2_20[3]-t02*1e-9             )*amp[ch_2] )/( amp[ch_1]+amp[ch_2] )*1e9 ;
		//float tcomb2 = (  (LP2_20[ch_1]-LP2_20[3]-t01*1e-9-t01_two*1e-9)*amp[ch_1] + (LP2_20[ch_2]-LP2_20[3]-t02*1e-9-t02_two*1e-9)*amp[ch_2] )/( amp[ch_1]+amp[ch_2] )*1e9 ;

		plotter.Plot1D(Form("timeres_twohits_%i_%i_deltaTlead",ch_1,ch_2), ";#Delta T corr [ns]"  , tlead, 100, min_T, max_T);
		plotter.Plot1D(Form("timeres_twohits_%i_%i_deltaTcomb",ch_1,ch_2), ";#Delta T corr [ns]"  , tcomb, 100, min_T, max_T);
		//plotter.Plot1D(Form("timeres_twohits_%i_%i_deltaTlead2",ch_1,ch_2), ";#Delta T corr [ns]" , tlead2, 100, min_T, max_T);
		//plotter.Plot1D(Form("timeres_twohits_%i_%i_deltaTcomb2",ch_1,ch_2), ";#Delta T corr [ns]" , tcomb2, 100, min_T, max_T);

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


		//float t01 = t0_corr(cfg,ch_1,amp[ch_1]); // in ns
		//float t02 = t0_corr(cfg,ch_2,amp[ch_2]); // in ns
		//float t03 = t0_corr(cfg,ch_3,amp[ch_3]); // in ns
		//float t01 = t0_corr(cfg,ch_1,x_dut[dut]); // in ns
		//float t02 = t0_corr(cfg,ch_2,x_dut[dut]); // in ns
		//float t03 = t0_corr(cfg,ch_3,x_dut[dut]); // in ns
		float t01 = t0_corr(cfg,ch_1,amp[ch_1],x_dut[dut]); // in ns
		float t02 = t0_corr(cfg,ch_2,amp[ch_2],x_dut[dut]); // in ns
		float t03 = t0_corr(cfg,ch_3,amp[ch_3],x_dut[dut]); // in ns

		//float t01_two = three_strip_corr(cfg,ch_1,ch_2,ch_3,ch_1);
		//float t02_two = three_strip_corr(cfg,ch_1,ch_2,ch_3,ch_2);
		//float t03_two = three_strip_corr(cfg,ch_1,ch_2,ch_3,ch_3);

		// slew rate
		plotter.Plot1D(Form("timeres_threehits_%i_%i_%i_ch%i_slewrate" ,ch_1,ch_2,ch_3,ch_1), ";slewrate [mV/ns]", abs(risetime[ch_1])/1e9, 50, 0, 2000);
		plotter.Plot1D(Form("timeres_threehits_%i_%i_%i_ch%i_slewrate" ,ch_1,ch_2,ch_3,ch_2), ";slewrate [mV/ns]", abs(risetime[ch_2])/1e9, 50, 0, 2000);
		plotter.Plot1D(Form("timeres_threehits_%i_%i_%i_ch%i_slewrate" ,ch_1,ch_2,ch_3,ch_3), ";slewrate [mV/ns]", abs(risetime[ch_3])/1e9, 50, 0, 2000);

		plotter.Plot1D(Form("timeres_threehits_%i_%i_%i_ch%i_risetime" ,ch_1,ch_2,ch_3,ch_1), ";risetime [ns]", 1e9*abs(amp[ch_1]/risetime[ch_1]), 50, 0, 2);
		plotter.Plot1D(Form("timeres_threehits_%i_%i_%i_ch%i_risetime" ,ch_1,ch_2,ch_3,ch_2), ";risetime [ns]", 1e9*abs(amp[ch_2]/risetime[ch_2]), 50, 0, 2);
		plotter.Plot1D(Form("timeres_threehits_%i_%i_%i_ch%i_risetime" ,ch_1,ch_2,ch_3,ch_3), ";risetime [ns]", 1e9*abs(amp[ch_3]/risetime[ch_3]), 50, 0, 2);
		// rms noise
		plotter.Plot1D(Form("timeres_threehits_%i_%i_%i_ch%i_rmsnoise" ,ch_1,ch_2,ch_3,ch_1), ";baseline [mV]", baseline_RMS[ch_1], 50, 0, 50);
		plotter.Plot1D(Form("timeres_threehits_%i_%i_%i_ch%i_rmsnoise" ,ch_1,ch_2,ch_3,ch_2), ";baseline [mV]", baseline_RMS[ch_2], 50, 0, 50);
		plotter.Plot1D(Form("timeres_threehits_%i_%i_%i_ch%i_rmsnoise" ,ch_1,ch_2,ch_3,ch_3), ";baseline [mV]", baseline_RMS[ch_3], 50, 0, 50);

		plotter.Plot1D(Form("timeres_threehits_%i_%i_%i_ch%i_jitter" ,ch_1,ch_2,ch_3,ch_1), ";jitter [ps]", 1e12 * baseline_RMS[ch_1]/abs(risetime[ch_1]), 100, 0, 30);
		plotter.Plot1D(Form("timeres_threehits_%i_%i_%i_ch%i_jitter" ,ch_1,ch_2,ch_3,ch_2), ";jitter [ps]", 1e12 * baseline_RMS[ch_2]/abs(risetime[ch_2]), 100, 0, 30);
		plotter.Plot1D(Form("timeres_threehits_%i_%i_%i_ch%i_jitter" ,ch_1,ch_2,ch_3,ch_3), ";jitter [ps]", 1e12 * baseline_RMS[ch_3]/abs(risetime[ch_3]), 100, 0, 30);

		// all hits amp ordered
		plotter.Plot1D(Form("timeres_threehits_%i_%i_%i_ch%i_deltaTun" ,ch_1,ch_2,ch_3,ch_1), ";#Delta T uncorr [ns]", (LP2_20[ch_1] - LP2_20[3])*1e9, 100, t01+min_T, t01+max_T);
		plotter.Plot1D(Form("timeres_threehits_%i_%i_%i_ch%i_deltaTun" ,ch_1,ch_2,ch_3,ch_2), ";#Delta T uncorr [ns]", (LP2_20[ch_2] - LP2_20[3])*1e9, 100, t02+min_T, t02+max_T);
		plotter.Plot1D(Form("timeres_threehits_%i_%i_%i_ch%i_deltaTun" ,ch_1,ch_2,ch_3,ch_3), ";#Delta T uncorr [ns]", (LP2_20[ch_3] - LP2_20[3])*1e9, 100, t03+min_T, t03+max_T);
		plotter.Plot1D(Form("timeres_threehits_%i_%i_%i_ch%i_deltaTcor",ch_1,ch_2,ch_3,ch_1), ";#Delta T corr [ns]"  , (LP2_20[ch_1] - LP2_20[3])*1e9 - t01, 100, min_T, max_T);
		plotter.Plot1D(Form("timeres_threehits_%i_%i_%i_ch%i_deltaTcor",ch_1,ch_2,ch_3,ch_2), ";#Delta T corr [ns]"  , (LP2_20[ch_2] - LP2_20[3])*1e9 - t02, 100, min_T, max_T);
		plotter.Plot1D(Form("timeres_threehits_%i_%i_%i_ch%i_deltaTcor",ch_1,ch_2,ch_3,ch_3), ";#Delta T corr [ns]"  , (LP2_20[ch_3] - LP2_20[3])*1e9 - t03, 100, min_T, max_T);
		//plotter.Plot1D(Form("timeres_threehits_%i_%i_%i_ch%i_deltaTcor2",ch_1,ch_2,ch_3,ch_1), ";#Delta T corr [ns]"  , (LP2_20[ch_1] - LP2_20[3])*1e9 - t01 - t01_two, 100, min_T, max_T);
		//plotter.Plot1D(Form("timeres_threehits_%i_%i_%i_ch%i_deltaTcor2",ch_1,ch_2,ch_3,ch_2), ";#Delta T corr [ns]"  , (LP2_20[ch_2] - LP2_20[3])*1e9 - t02 - t02_two, 100, min_T, max_T);
		//plotter.Plot1D(Form("timeres_threehits_%i_%i_%i_ch%i_deltaTcor2",ch_1,ch_2,ch_3,ch_3), ";#Delta T corr [ns]"  , (LP2_20[ch_3] - LP2_20[3])*1e9 - t03 - t03_two, 100, min_T, max_T);

		plotter.Plot1D(Form("timeres_threehits_%i_%i_%i_xpos"      ,ch_1,ch_2,ch_3),      ";x [mm]"        , x_dut[dut], 100, min_X, max_X);
		plotter.Plot1D(Form("timeres_threehits_%i_%i_%i_ch%i_amp"  ,ch_1,ch_2,ch_3,ch_1), ";Amplitude [mV]", amp[ch_1] , 100, 0, 1800);
		plotter.Plot1D(Form("timeres_threehits_%i_%i_%i_ch%i_amp"  ,ch_1,ch_2,ch_3,ch_2), ";Amplitude [mV]", amp[ch_2] , 100, 0, 1800);
		plotter.Plot1D(Form("timeres_threehits_%i_%i_%i_ch%i_amp"  ,ch_1,ch_2,ch_3,ch_3), ";Amplitude [mV]", amp[ch_3] , 100, 0, 1800);

		// leading hit 
		float tlead = (LP2_20[ch_1] - LP2_20[3])*1e9 - t01;
		//float tlead2 = (LP2_20[ch_1] - LP2_20[3])*1e9 - t01 -t01_two;

		// weighted time res! 
		float tcomb  = (  (LP2_20[ch_1]-LP2_20[3]-t01*1e-9             )*amp[ch_1] + (LP2_20[ch_2]-LP2_20[3]-t02*1e-9             )*amp[ch_2] + (LP2_20[ch_3]-LP2_20[3]-t03*1e-9             )*amp[ch_3])/( amp[ch_1]+amp[ch_2]+amp[ch_3] )*1e9 ;
		//float tcomb2 = (  (LP2_20[ch_1]-LP2_20[3]-t01*1e-9-t01_two*1e-9)*amp[ch_1] + (LP2_20[ch_2]-LP2_20[3]-t02*1e-9-t02_two*1e-9)*amp[ch_2] + (LP2_20[ch_3]-LP2_20[3]-t03*1e-9-t03_two*1e-9)*amp[ch_3])/( amp[ch_1]+amp[ch_2]+amp[ch_3] )*1e9 ;

		
		plotter.Plot1D(Form("timeres_threehits_%i_%i_%i_deltaTlead",ch_1,ch_2,ch_3), ";#Delta T corr [ns]"  , tlead, 100, min_T, max_T);
		plotter.Plot1D(Form("timeres_threehits_%i_%i_%i_deltaTcomb",ch_1,ch_2,ch_3), ";#Delta T corr [ns]"  , tcomb, 100, min_T, max_T);
		//plotter.Plot1D(Form("timeres_threehits_%i_%i_%i_deltaTlead2",ch_1,ch_2,ch_3), ";#Delta T corr [ns]"  , tlead2, 100, min_T, max_T);
		//plotter.Plot1D(Form("timeres_threehits_%i_%i_%i_deltaTcomb2",ch_1,ch_2,ch_3), ";#Delta T corr [ns]"  , tcomb2, 100, min_T, max_T);

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