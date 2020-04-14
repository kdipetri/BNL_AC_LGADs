#define xmeasure
//KEY: TProfile	profcharge_fraction_v_x_two_0_1;1	
//KEY: TProfile	profcharge_fraction_v_x_two_1_0;1	
//KEY: TProfile	profcharge_fraction_v_x_two_0_2;1	
//KEY: TProfile	profcharge_fraction_v_x_two_2_0;1	
//KEY: TProfile	profcharge_fraction_v_x_three_0_1;1	
//KEY: TProfile	profcharge_fraction_v_x_three_1_2;1	
//KEY: TProfile	profcharge_fraction_v_x_three_2_0;1

TFile *file_prof = TFile::Open("profiles/profiles_safe.root");


// start with 02

float xcenter(int ch_name)
{
	float xcenter = -99;
    if      (ch_name==14)  xcenter = 20.85;
    else if (ch_name== 3)  xcenter = 20.75;
    else if (ch_name==13)  xcenter = 20.65;
    else if (ch_name== 4)  xcenter = 20.55;
    else if (ch_name==12)  xcenter = 20.45;
    else if (ch_name== 5)  xcenter = 20.35;
    else if (ch_name==11)  xcenter = 20.25;
    else if (ch_name== 6)  xcenter = 20.15;
    return xcenter;
}
float xcenter(std::string cfg, int ch)
{
	float x = -99;
	if ("cfg_4_13_12" == cfg) 
	{
		if (ch==0) x = 20.55;//4
		else if (ch==1) x = 20.65;//13
		else if (ch==2) x = 20.45;// 12		
	}
	else if ("cfg_6_5_11" == cfg )
	{
		if (ch==0) x = 20.15;//6
		else if (ch==1) x = 20.35;// 5
		else if (ch==2) x = 20.25;// 11	
	}
	return x;
}
float fit_xmin(std::string cfg, std::vector<int> channels){

	float min = 99;
	for (auto ch : channels){
		if (xcenter(cfg,ch) < min) min = xcenter(cfg,ch);
	}
	//std::cout << "min " << min << std::endl;
	return min;
}
float fit_xmax(std::string cfg, std::vector<int> channels){

	float max = 0;
	for (auto ch : channels){
		if (xcenter(cfg,ch) > max) max = xcenter(cfg,ch);
	}
	//std::cout << "max " << max << std::endl;
	return max;
}

float get_x_from_cr(TH1F *hist, float cf, float min, float max)
{
	//std::cout << "getting x " << std::endl;
	//std::cout << "nbins" << hist->GetNbinsX() << std::endl;
	float xpos = 0;
	float diff = 1;
	std::vector<float> cf_diffs;
	for (int bin=1; bin<hist->GetNbinsX()+1; bin++){

		float xbin = hist->GetBinCenter(bin);
		//std::cout << xbin << " " << min << " " << max << std::endl;
	
		if (xbin < min) continue;
		if (xbin > max) continue;
	
		float content = hist->GetBinContent(bin);
		float diff_tmp = abs(cf-content);
		//std::cout << bin << " " << cf << " " << content << " " << diff << " " << diff_tmp << std::endl;
		if ( diff_tmp < diff ){
			diff = diff_tmp;
			xpos = xbin;
		}	

	}
	return xpos;

}
void Analysis::xpos_lookup(std::string cfg, int ch1,int ch2,int ch3=-1)
{
	if ( ch3 ==-1 )// two channel hit
	{
		// amplitude ordering 
		//std::cout << "finding ordering " << std::endl;
		int ch_1 = amp[ch1] > amp[ch2] ? ch1 : ch2;
		int ch_2 = amp[ch1] > amp[ch2] ? ch2 : ch1;

		// get fraction
		//std::cout << "finding charge fraction " << std::endl;
		//float cf = amp[ch_1]/(amp[ch_1]+amp[ch_2]);
		float cr1 = amp[ch_1]/amp[ch_2];
		float cr2 = amp[ch_2]/amp[ch_1];

		// get range...
		//std::cout << "finding xrange " << std::endl;
		float min = fit_xmin(cfg,{ch1,ch2});
		float max = fit_xmax(cfg,{ch1,ch2});
		//std::cout << min << " " << max <<  std::endl;

		// get hist
		//std::cout << "finding hist " << std::endl;
		TH1F *hist1 = (TH1F*)file_prof->Get(Form("profxcharge_ratio_v_x_any_%i_%i",ch_1,ch_2));
		TH1F *hist2 = (TH1F*)file_prof->Get(Form("profxcharge_ratio_v_x_any_%i_%i",ch_2,ch_1));
		//std::cout << Form("profxcharge_fraction_v_x_two_%i_%i",ch_1,ch_2) << std::endl;
		float xpos1 = get_x_from_cr(hist1,cr1,min,max);
		float xpos2 = get_x_from_cr(hist2,cr2,min,max);
		//std::cout << xpos1 << " " << xpos2 << " " << x_dut[dut] << std::endl;

		float xpos=99; 
		if ( xpos1> 0 && xpos2 > 0) xpos = (xpos1+xpos2)/2.0;
		else if (xpos1 > 0) xpos = xpos1;
		else if (xpos2 > 0) xpos = xpos2;
		
		//std::cout << cf << " " << diff << " " << xpos << " " <<  x_dut[dut] << std::endl;
		//if ( x_dut[dut] > min && x_dut[dut] < max ){
		//	plotter.Plot2D(Form("two_hit_xmeas_xtrack_%i_%i",ch1,ch2),  ";x tracker [mm]; x measured [mm]", x_dut[dut], xpos, 50, min-0.05, max+0.05, 50, min-0.05, max+0.05);
		//	plotter.Plot1D(Form("two_hit_xdiff_%i_%i",ch1,ch2),  ";x tracker - x measured [mm];", x_dut[dut] - xpos, 50, -0.2, 0.2);			
		//}
		//std::cout << cf << " " << diff << " " << xpos << " " <<  x_dut[dut] << std::endl;
		if ( x_dut[dut] > min && x_dut[dut] < max ){
			plotter.Plot2D(Form("xmeas_two_lookup_xmeas_xtrack_%i_%i",ch1,ch2),  ";x tracker [mm]; x measured [mm]", x_dut[dut],  xpos, 100, min-0.05, max+0.05, 100, min-0.05, max+0.05);
			plotter.Plot1D(Form("xmeas_two_lookup_xdiff_%i_%i",ch1,ch2),          ";x tracker - x measured [mm];"  , x_dut[dut] - xpos, 50, -0.3, 0.3);			
		}

		return ;


	}
	else { // three channel hit

		// amplitude ordering 
		//std::cout << "finding ordering " << std::endl;
		int nxpos = 0;
		float xpos_final=0;

		float min = 20.4;
		float max = 20.7;

		for (int c1 =0; c1<3 ; c1++){
			for (int c2 =0; c2<3 ; c2++){

				if (c1 == c2) continue;

				float cr = amp[c1]/amp[c2];

				TH1F *hist = (TH1F*)file_prof->Get(Form("profxcharge_ratio_v_x_any_%i_%i",c1,c2));

				float xpos = get_x_from_cr(hist,cr,min,max);
				if (xpos > -99) {
					xpos_final += xpos;
					nxpos+=1;

				}

			}
		}
		xpos_final = xpos_final/nxpos;

		//std::cout << cf << " " << diff << " " << xpos << " " <<  x_dut[dut] << std::endl;
		if ( x_dut[dut] > min && x_dut[dut] < max ){
			plotter.Plot2D(Form("xmeas_three_lookup_xmeas_xtrack_%i_%i",ch1,ch2),  ";x tracker [mm]; x measured [mm]", x_dut[dut], xpos_final, 100, min-0.05, max+0.05, 100, min-0.05, max+0.05);
			plotter.Plot1D(Form("xmeas_three_lookup_xdiff_%i_%i",ch1,ch2),  ";x tracker - x measured [mm];", x_dut[dut] - xpos_final, 50, -0.3, 0.3);			
		}
		
		return ;

	}
}
void Analysis::xpos_single(std::string cfg, int ch1, int ch2, int ch3=-1)
{

	//std::cout << cf << " " << diff << " " << xpos << " " <<  x_dut[dut] << std::endl;
	if (ch3 == -1){
		
		float min = fit_xmin(cfg,{ch1,ch2});
		float max = fit_xmax(cfg,{ch1,ch2});

		int max_ch = amp[ch1] > amp[ch2] ? ch1 : ch2;

		float xpos = xcenter(cfg,max_ch); //(amp[ch1]*xcenter(ch1)+amp[ch2]*xcenter(ch2))/(amp[ch1]+amp[ch2]);

		if ( x_dut[dut] > min && x_dut[dut] < max ){
		plotter.Plot2D(Form("xmeas_two_highestamp_xmeas_xtrack_%i_%i",ch1,ch2),  ";x tracker [mm]; x measured [mm]", x_dut[dut], xpos, 100, min-0.05, max+0.05, 25, min-0.05, max+0.05);
		plotter.Plot1D(Form("xmeas_two_highestamp_xdiff_%i_%i",ch1,ch2),  ";x tracker - x measured [mm];", x_dut[dut] - xpos, 50, -0.3, 0.3);			
		}
	}
	else {

		//float min = 19.8; // for large plot
    	//float max = 21.8; // for large plot
    	float min = 20.4;
		float max = 20.7;

    	int max_ch_tmp = amp[ch1] > amp[ch2] ? ch1 : ch2;
		int max_ch  = amp[max_ch_tmp] > amp[ch3] ? max_ch_tmp : ch3;

		float xpos = xcenter(cfg,max_ch); //(amp[ch1]*xcenter(ch1)+amp[ch2]*xcenter(ch2))/(amp[ch1]+amp[ch2]);

		if ( x_dut[dut] > min && x_dut[dut] < max ){
		plotter.Plot2D(Form("xmeas_three_higestamp_xmeas_xtrack"),  ";x tracker [mm]; x measured [mm]", x_dut[dut], xpos , 100, min-0.05, max+0.05, 20, min-0.05, max+0.05);
		plotter.Plot1D(Form("xmeas_three_higestamp_xdiff")       ,  ";x tracker - x measured [mm];"   , x_dut[dut] - xpos, 50, -0.3, 0.3);			
		}

	}

}
void Analysis::xpos_weight(std::string cfg, int ch1,int ch2,int ch3=-1)
{

	//std::cout << cf << " " << diff << " " << xpos << " " <<  x_dut[dut] << std::endl;
	if (ch3 == -1){
		
		float min = fit_xmin(cfg,{ch1,ch2});
		float max = fit_xmax(cfg,{ch1,ch2});


		float xpos = (amp[ch1]*xcenter(cfg,ch1)+amp[ch2]*xcenter(cfg,ch2))/(amp[ch1]+amp[ch2]);

		if ( x_dut[dut] > min && x_dut[dut] < max ){
		plotter.Plot2D(Form("xmeas_two_hitweighted_xmeas_xtrack_%i_%i",ch1,ch2),  ";x tracker [mm]; x measured [mm]", x_dut[dut], xpos , 100, min-0.05, max+0.05, 100, min-0.05, max+0.05);
		plotter.Plot1D(Form("xmeas_two_hitweighted_xdiff_%i_%i",ch1,ch2),         ";x tracker - x measured [mm];"   , x_dut[dut] - xpos, 50, -0.3, 0.3);			
		}
	}
	else {

		//float min = 19.8; // for large plot
    	//float max = 21.8; // for large plot
		float min = 20.4;//fit_xmin({ch1,ch2,ch3});
		float max = 20.7;//fit_xmax({ch1,ch2,ch3});

		float xpos = (amp[ch1]*xcenter(cfg,ch1)+amp[ch2]*xcenter(cfg,ch2)+amp[ch3]*xcenter(cfg,ch3))/(amp[ch1]+amp[ch2]+amp[ch3]);

		if ( x_dut[dut] > min && x_dut[dut] < max ){
		plotter.Plot2D(Form("xmeas_three_hitweighted_xmeas_xtrack"),  ";x tracker [mm]; x measured [mm]", x_dut[dut], xpos , 100, min-0.05, max+0.05, 100, min-0.05, max+0.05);
		plotter.Plot1D(Form("xmeas_three_hitweighted_xdiff")       ,  ";x tracker - x measured [mm];"   , x_dut[dut] - xpos, 50, -0.3, 0.3);			
		}

	}

}