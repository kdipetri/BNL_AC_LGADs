import ROOT
import langaus as lg

ROOT.gStyle.SetOptStat(1)
ROOT.gStyle.SetOptFit(1)
ROOT.gROOT.SetBatch(ROOT.kTRUE)
ROOT.gStyle.SetLabelFont(42,"xyz")
ROOT.gStyle.SetLabelSize(0.05,"xyz")
ROOT.gStyle.SetTitleFont(42,"xyz")
ROOT.gStyle.SetTitleFont(42,"t")
ROOT.gStyle.SetTitleSize(0.06,"xyz")
ROOT.gStyle.SetTitleSize(0.06,"t")
ROOT.gStyle.SetPadBottomMargin(0.14)
ROOT.gStyle.SetPadLeftMargin(0.14)
ROOT.gStyle.SetPadTickX(1)
ROOT.gStyle.SetPadTickY(1)
ROOT.gStyle.SetTitleOffset(1,'y')
ROOT.gStyle.SetLegendTextSize(0.035)
ROOT.gStyle.SetGridStyle(3)
ROOT.gStyle.SetGridColor(14)
ROOT.gStyle.SetOptFit(1)
one   = ROOT.TColor(2001,143/255.,45 /255.,86/255.,"darkPurple")#quinacridone magenta
two   = ROOT.TColor(2002,119/255.,104/255.,174/255.,"blahBlue")#blue-violet
three = ROOT.TColor(2003,239/255.,71 /255.,111/255.,"pinkRed")#paradise pink
four  = ROOT.TColor(2004,247/255.,178/255.,103/255.,"orange")#orange
five  = ROOT.TColor(2005,42 /255.,157/255.,143/255.,"PersianGreen")# persian green
six   = ROOT.TColor(2006,38 /255.,70 /255.,83 /255.,"Charcol")# charcol
seven = ROOT.TColor(2007,116/255.,165/255.,127/255.,"Green")#forest green
eight = ROOT.TColor(2008,233/255.,196/255.,106/255.,"Maize")# maize
nine  = ROOT.TColor(2009,8/255.,103/255.,136/255.,"RussianViolet")#russian violet 
ten   = ROOT.TColor(2010,231/255.,111/255.,81 /255.,"TerraCotta")# terra cotta
colors = [] #[2001,2002,2003,2004,2005,2006,2007,2008,2009,2010]
colors.append(ROOT.kBlack)
colors.append(2003)#paradise
colors.append(2004)#orange
colors.append(2005)#persian green
colors.append(2002)#blue-violet
colors.append(2001)#quinacridone magenta
colors.append(2010)#terra cotta
colors.append(2008)#maize
colors.append(2007)#forest green
colors.append(2009)#bluesapphire
colors.append(2006)#charcol

f = ROOT.TFile.Open("output/hists_cfg_4_13_12.root")
def cleanHist(hist,i,opt=""):
    col = colors[i]
    name = hist.GetName()
    hist.SetLineColor(col)
    hist.SetMarkerColor(col)
    hist.SetMarkerStyle(20)
    hist.SetFillColor(0)
    if "stack" in opt:
    	hist.SetFillColorAlpha(col,0.8)
    hist.GetXaxis().SetTitleSize(0.06)
    hist.GetYaxis().SetTitleSize(0.06)
    hist.GetXaxis().SetLabelSize(0.05)
    hist.GetYaxis().SetLabelSize(0.05)
    hist.GetXaxis().SetNdivisions(505)
    hist.GetYaxis().SetNdivisions(505)
    return

def channel(ch):
    if ch == "0" : return 4
    if ch == "1" : return 13 
    if ch == "2" : return 12 
    if ch == 0 : return 4
    if ch == 1 : return 13 
    if ch == 2 : return 12 
    else : return -1

def colorindex(obj):
    name = obj.GetName()
    if "xpos" in name or "sum_amp" in name:
    	if "threehits_0_1_2" in name: return 1
    	if "twohits_0_1" in name: return 2
    	if "twohits_0_2" in name: return 3
    	if "twohits_1_2" in name: return 4
    	if "one_0" in name: return 7
    	if "one_1" in name: return 6
    	if "one_2" in name: return 5
    if "ch0" in name: return 1
    if "ch1" in name: return 2
    if "ch2" in name: return 3
    if "comb" in name: return 0
    return 0

def label(obj):
    name = obj.GetName()
    if "xpos" in name or "sum_amp" in name:
    	if "threehits_0_1_2" in name: return "Nhits=3 (4,13,12)"
    	if "twohits_0_1" in name: return "Nhits=2 (4,13)"
    	if "twohits_0_2" in name: return "Nhits=2 (4,12)"
    	if "twohits_1_2" in name: return "Nhits=2 (12,13)"
    	if "one_0" in name: return "Nhits=1 (4)"
    	if "one_1" in name: return "Nhits=1 (13)"
    	if "one_2" in name: return "Nhits=1 (12)"
    if "ch0" in name: return "Channel 4"
    if "ch1" in name: return "Channel 13"
    if "ch2" in name: return "Channel 12"
    if "comb" in name: return "Weighted Avg"
    if "lead" in name: return "Leading Channel"
    return ""

def get_distribution(name):
	ROOT.gStyle.SetOptStat(0)
	ROOT.gStyle.SetOptFit(0)
	hist = f.Get(name)
	c = ROOT.TCanvas(name,"",900,800)
	c.SetLeftMargin(0.2)
	leg = ROOT.TLegend(0.37,0.88-0.05,0.88,0.88)

	cleanHist(hist,0)
	hist.SetMaximum(600)
	hist.Draw("hist") 
	lab = label(hist)

	if "sum_amp"  in name: 
		fitter = lg.LanGausFit()
		f1 = fitter.fit(hist,(0,3000))

		#f1 = ROOT.TF1("f1_"+name,"landau",0,3000)
		#hist.Scale(1.0/hist.Integral(0,-1))
		hist.Fit(f1,"Q")
		hist.GetYaxis().SetTitle("Events")
		hist.GetXaxis().SetTitle("Amplitude Sum [mV]")
		f1.Draw("same")

		mpv = f1.GetParameter(1)
		err = f1.GetParError(1)
		text = "MPV: {:.0f} #pm {:.0f} mV".format(mpv,err)
		y=0.88
		
		txt = ROOT.TPaveText(0.5,0.82,0.86,0.82-0.06, "NDC")
		txt.SetTextAlign(13)
		txt.SetTextFont(42)
		txt.SetTextSize(0.05)
		txt.SetFillColor(0)
		txt.SetBorderSize(0)
		txt.AddText(text);
		txt.Draw()

	#if "xpos"  in name:
	#	hist.Rebin()
	#    leg.AddEntry(hist,lab,"l")
	#    leg.Draw()
	#else:
	#    hist.Rebin()

	c.Print("plots/charge_sharing/{}.png".format(name))
	
	if "sum_amp" in name: 
		c.Print("plots/charge_sharing/{}.pdf".format(name))
		return hist,f1
	else : return hist

    
# main 
def get_comparison(hists,fits,name):
	ROOT.gStyle.SetOptStat(0)
	ROOT.gStyle.SetOptFit(0)
	c = ROOT.TCanvas()
	dy = len(hists)*0.06
	leg = ROOT.TLegend(0.55,0.88-dy,0.88,0.88)
	if "xpos" in name:
		leg1 = ROOT.TLegend(0.2 ,0.88-0.06*2,0.42,0.88)
		leg2 = ROOT.TLegend(0.42,0.88-0.06*2,0.64,0.88)
		leg3 = ROOT.TLegend(0.64,0.88-0.05  ,0.86,0.88)
		leg1.SetBorderSize(0)
		leg2.SetBorderSize(0)
		leg3.SetBorderSize(0)
	ymax = 0
	for i,hist in enumerate(hists): 
	    
		# color handling
		icol = colorindex(hist)
		cleanHist(hist,icol)
		if i==0: hist.Draw("hist")
		else : hist.Draw("histsame")
		lab = label(hist)
		if hist.GetMaximum() > ymax: ymax = hist.GetMaximum()
		if len(fits) > 0: 
		    fits[i].SetLineColor(colors[icol])
		    fits[i].Draw("same")
		    #t0_str = ": t0={:.1f} ps, #sigma={:.1f} ps".format(fits[i].GetParameter(1)*1e3, fits[i].GetParameter(2)*1e3)
		    #lab+= t0_str
		if "xpos" in name:
			if i < 3 : leg1.AddEntry(hist,lab, "l") 
			elif i < 6: leg2.AddEntry(hist,lab,"l")
			else : leg3.AddEntry(hist,lab, "lc") 
		leg.AddEntry(hist,lab, "l") 
	if "xpos" in name:
		leg1.Draw()
		leg2.Draw()
		leg3.Draw() 
	else : 
		leg.Draw()
	hists[0].SetMaximum(ymax*1.3)
	if "xpos" in name : hists[0].GetYaxis().SetTitle("Events")
	if "sum_amp" in name: hists[0].SetMaximum(0.29)
	c.Print("plots/charge_sharing/compare_{}.png".format(name))
	c.Print("plots/charge_sharing/compare_{}.pdf".format(name))

# main 
def do_stack(hists,name):
	ROOT.gStyle.SetOptStat(0)
	ROOT.gStyle.SetOptFit(0)
	c = ROOT.TCanvas()
	c.SetBottomMargin(0.15)
	c.SetLeftMargin(0.15)
	dy = len(hists)*0.06
	leg1 = ROOT.TLegend(0.2 ,0.88-0.06*2,0.42,0.88)
	leg2 = ROOT.TLegend(0.42,0.88-0.06*2,0.64,0.88)
	leg3 = ROOT.TLegend(0.64,0.88-0.05  ,0.86,0.88)
	leg1.SetBorderSize(0)
	leg2.SetBorderSize(0)
	leg3.SetBorderSize(0)
	ymax = 0
	stack = ROOT.THStack("stack", ";x [mm]")
	for i,hist in enumerate(hists): 
		# color handling
		icol = colorindex(hist)
		cleanHist(hist,icol,"stack")
		#hist.Rebin()
		stack.Add(hist)
		lab = label(hist)

		
		if i < 3 : leg1.AddEntry(hist,lab, "f") 
		elif i < 6: leg2.AddEntry(hist,lab,"f")
		else : leg3.AddEntry(hist,lab, "f") 

	stack.Draw("hist")
	stack.SetMaximum(700)
	stack.GetXaxis().SetNdivisions(505)
	stack.GetYaxis().SetNdivisions(505)
	stack.GetYaxis().SetTitle("Events")
	leg1.Draw()
	leg2.Draw()
	leg3.Draw()
	  

	#hists[0].SetMaximum(ymax*1.3)
	c.Print("plots/charge_sharing/stack_{}.png".format(name))
	c.Print("plots/charge_sharing/stack_{}.pdf".format(name))
        
def clean2D(histname):
	c = ROOT.TCanvas(histname,"",900,800)
	c.SetTopMargin(0.1)
	c.SetLeftMargin(0.2)
	c.SetBottomMargin(0.20)
	c.SetRightMargin(0.2)

	ch1 = histname.split("sharing_")[2].split("_")[0]
	ch2 = histname.split("sharing_")[2].split("_")[1]
	print(ch1,ch2)

	hist = f.Get(histname)

	hist.GetXaxis().SetTitleOffset(1.15)
	hist.GetZaxis().SetTitleOffset(1.1)
	hist.GetXaxis().SetNdivisions(505)
	hist.GetYaxis().SetNdivisions(505)

	hist.GetXaxis().SetTitleSize(0.06)
	hist.GetYaxis().SetTitleSize(0.06)
	hist.GetXaxis().SetLabelSize(0.06)
	hist.GetYaxis().SetLabelSize(0.06)

	#hist.GetZaxis().SetNdivisions(505)
	hist.GetZaxis().SetTitleSize(0.06)
	hist.GetZaxis().SetLabelSize(0.05)

	hist.GetXaxis().SetTitle("Ch {} Amplitude [mV]".format(channel(ch1)))
	hist.GetYaxis().SetTitle("Ch {} Amplitude [mV]".format(channel(ch2)))
	hist.GetZaxis().SetTitle("Events")

	hist.Draw("COLZ")
	c.Print("plots/charge_sharing/{}.png".format(histname))
	c.Print("plots/charge_sharing/{}.pdf".format(histname))
    

hist1 = get_distribution("chargesharing_threehits_0_1_2_ch0_amp")	
hist2 = get_distribution("chargesharing_threehits_0_1_2_ch1_amp")	
hist3 = get_distribution("chargesharing_threehits_0_1_2_ch2_amp")
hists = [hist1,hist2,hist3]
fits = []
get_comparison(hists,fits,"chargesharing_threehits_0_1_2_amp")

hist1 = get_distribution("chargesharing_twohits_0_1_ch0_amp")	
hist2 = get_distribution("chargesharing_twohits_0_1_ch1_amp")	
hists = [hist1,hist2]
fits = []
get_comparison(hists,fits,"chargesharing_twohits_0_1_amp")
	
hist1 = get_distribution("chargesharing_twohits_0_2_ch0_amp")	
hist2 = get_distribution("chargesharing_twohits_0_2_ch2_amp")
hists = [hist1,hist2]
fits = []
get_comparison(hists,fits,"chargesharing_twohits_0_2_amp")	

hist1, fit1 = get_distribution("chargesharing_threehits_0_1_2_sum_amp")
hist2, fit2 = get_distribution("chargesharing_twohits_0_1_sum_amp")
hist3, fit3 = get_distribution("chargesharing_twohits_0_2_sum_amp")
hists = [hist1,hist2,hist3]
fits  = [fit1 ,fit2 ,fit3 ]
get_comparison(hists,fits,"chargesharing_sum_amp")	
hists = [hist1]
fits  = [fit1 ]
get_comparison(hists,fits,"chargesharing_three_sum_amp")	

hist1 = get_distribution("chargesharing_threehits_0_1_2_xpos")	
hist2 = get_distribution("chargesharing_twohits_0_1_xpos")
hist3 = get_distribution("chargesharing_twohits_0_2_xpos")	
hist4 = get_distribution("chargesharing_twohits_1_2_xpos")	
hist5 = get_distribution("chargesharing_one_0_xpos")
hist6 = get_distribution("chargesharing_one_1_xpos")
hist7 = get_distribution("chargesharing_one_2_xpos")
hists = [hist1,hist2,hist3]
hists = [hist7,hist6,hist5,hist4,hist3,hist2,hist1]

fits  = []
get_comparison(hists,fits,"chargesharing_xpos")	
hists = [hist7,hist6,hist5,hist4,hist3,hist2,hist1]
do_stack(hists,"chargesharing_xpos")



names = []
names.append("chargesharing_threehits_0_1_2_sharing_0_1")	
names.append("chargesharing_threehits_0_1_2_sharing_0_2")	
names.append("chargesharing_threehits_0_1_2_sharing_1_2")	
names.append("chargesharing_twohits_0_1_sharing_0_1")	
names.append("chargesharing_twohits_0_2_sharing_0_2")	
names.append("chargesharing_twohits_1_2_sharing_1_2")	
for name in names:
	clean2D(name)


