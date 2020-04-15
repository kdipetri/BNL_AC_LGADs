#define chargesharing



void Analysis::charge_sharing(std::string cfg, int ch1,int ch2=-1,int ch3=-1)
{

	//float min_T = -0.5;
	//float max_T = 0.5;
	//
	float min_X = 20.3;
	float max_X = 20.8;

	//if (LP2_20[ch1] == 0) return;
	//if (LP2_20[3] == 0) return;
	//std::cout << cf << " " << diff << " " << xpos << " " <<  x_dut[dut] << std::endl;
	if (ch2 == -1 && ch3 == -1){

		plotter.Plot1D(Form("chargesharing_one_%i_xpos"      ,ch1),     ";x [mm]"        , x_dut[dut], 50, min_X, max_X);
		plotter.Plot1D(Form("chargesharing_one_%i_ch%i_amp"  ,ch1,ch1), ";Amplitude [mV]", amp[ch1] , 50, 0, 1800);
	}
	else if (ch3 ==-1 && ch2 >= 0 ){
		
		//int ch_1 = amp[ch1]>amp[ch2] ? ch1 : ch2;
		//int ch_2 = amp[ch1]>amp[ch2] ? ch2 : ch1;
		//if (LP2_20[ch2] == 0) return;
		plotter.Plot1D(Form("chargesharing_twohits_%i_%i_xpos"      ,ch1,ch2),     ";x [mm]"        , x_dut[dut], 50, min_X, max_X);
		plotter.Plot1D(Form("chargesharing_twohits_%i_%i_ch%i_amp"  ,ch1,ch2,ch1), ";Amplitude [mV]", amp[ch1] , 50, 0, 1800);
		plotter.Plot1D(Form("chargesharing_twohits_%i_%i_ch%i_amp"  ,ch1,ch2,ch2), ";Amplitude [mV]", amp[ch2] , 50, 0, 1800);

		float sum_charge = amp[ch1]+amp[ch2];
		plotter.Plot1D(Form("chargesharing_twohits_%i_%i_sum_amp"       ,ch1,ch2),         ";Sum Amplitude [mV]"            , sum_charge , 50, 0, 3000);
		plotter.Plot2D(Form("chargesharing_twohits_%i_%i_sharing_%i_%i" ,ch1,ch2,ch1,ch2), ";Amplitude [mV]; Amplitude [mV]", amp[ch1], amp[ch2] , 50, 0, 1800, 50, 0, 1800);

		// leading hit 

	}
	else if ( ch2 >=0 && ch3 >= 0 ){
	//if ( ch2 >=0 && ch3 >= 0 && LP2_20[ch2] != 0 && LP2_20[ch3] != 0){
		
		//int ch_1;
		//int ch_2;
		//int ch_3;
		//
		//if (amp[ch1] > amp[ch2] && amp[ch1] > amp[ch3] && amp[ch2] > amp[ch3]){ ch_1 = ch1; ch_2 = ch2; ch_3 = ch3; }
		//if (amp[ch1] > amp[ch2] && amp[ch1] > amp[ch3] && amp[ch3] > amp[ch2]){ ch_1 = ch1; ch_2 = ch3; ch_3 = ch2; }
		//if (amp[ch2] > amp[ch1] && amp[ch2] > amp[ch3] && amp[ch1] > amp[ch3]){ ch_1 = ch2; ch_2 = ch1; ch_3 = ch3; } //
		//if (amp[ch2] > amp[ch1] && amp[ch2] > amp[ch3] && amp[ch3] > amp[ch1]){ ch_1 = ch2; ch_2 = ch3; ch_3 = ch1; } //
		//if (amp[ch3] > amp[ch1] && amp[ch3] > amp[ch2] && amp[ch1] > amp[ch2]){ ch_1 = ch3; ch_2 = ch1; ch_3 = ch2; }
		//if (amp[ch3] > amp[ch1] && amp[ch3] > amp[ch2] && amp[ch2] > amp[ch1]){ ch_1 = ch3; ch_2 = ch2; ch_3 = ch1; }
		//if (LP2_20[ch2] == 0) return;
		//if (LP2_20[ch3] == 0) return;

		plotter.Plot1D(Form("chargesharing_threehits_%i_%i_%i_xpos"      ,ch1,ch2,ch3),     ";x [mm]"        , x_dut[dut], 50 , min_X, max_X);
		plotter.Plot1D(Form("chargesharing_threehits_%i_%i_%i_ch%i_amp"  ,ch1,ch2,ch3,ch1), ";Amplitude [mV]", amp[ch1]  , 100, 0, 1800);
		plotter.Plot1D(Form("chargesharing_threehits_%i_%i_%i_ch%i_amp"  ,ch1,ch2,ch3,ch2), ";Amplitude [mV]", amp[ch2]  , 100, 0, 1800);
		plotter.Plot1D(Form("chargesharing_threehits_%i_%i_%i_ch%i_amp"  ,ch1,ch2,ch3,ch3), ";Amplitude [mV]", amp[ch3]  , 100, 0, 1800);

		float sum_charge = amp[ch1]+amp[ch2]+amp[ch3];
		plotter.Plot1D(Form("chargesharing_threehits_%i_%i_%i_sum_amp"  		,ch1,ch2,ch3),         ";Sum Amplitude [mV]"            , sum_charge , 50, 0, 3000);
		plotter.Plot2D(Form("chargesharing_threehits_%i_%i_%i_sharing_%i_%i"  ,ch1,ch2,ch3,ch1,ch2), ";Amplitude [mV]; Amplitude [mV]", amp[ch1], amp[ch2] , 50, 0, 1800, 50, 0, 1800);
		plotter.Plot2D(Form("chargesharing_threehits_%i_%i_%i_sharing_%i_%i"  ,ch1,ch2,ch3,ch1,ch3), ";Amplitude [mV]; Amplitude [mV]", amp[ch1], amp[ch3] , 50, 0, 1800, 50, 0, 1800);
		plotter.Plot2D(Form("chargesharing_threehits_%i_%i_%i_sharing_%i_%i"  ,ch1,ch2,ch3,ch2,ch3), ";Amplitude [mV]; Amplitude [mV]", amp[ch2], amp[ch3] , 50, 0, 1800, 50, 0, 1800);


	}

}