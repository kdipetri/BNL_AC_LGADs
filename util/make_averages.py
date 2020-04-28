import ROOT
from array import array
ROOT.gStyle.SetOptStat(0)
ROOT.gStyle.SetOptFit(0)
ROOT.gROOT.SetBatch(ROOT.kTRUE)
ROOT.gStyle.SetLabelFont(42,"xyz")
ROOT.gStyle.SetLabelSize(0.05,"xyz")
#ROOT.gStyle.SetTitleFont(42)
ROOT.gStyle.SetTitleFont(42,"xyz")
ROOT.gStyle.SetTitleFont(42,"t")
#ROOT.gStyle.SetTitleSize(0.05)
ROOT.gStyle.SetTitleSize(0.06,"xyz")
ROOT.gStyle.SetTitleSize(0.06,"t")
ROOT.gStyle.SetPadBottomMargin(0.14)
ROOT.gStyle.SetPadLeftMargin(0.14)
ROOT.gStyle.SetTitleOffset(1,'y')
ROOT.gStyle.SetLegendTextSize(0.035)
ROOT.gStyle.SetGridStyle(3)
ROOT.gStyle.SetGridColor(14)
ROOT.gStyle.SetOptFit(1)
one = ROOT.TColor(2001,0.906,0.153,0.094)
two = ROOT.TColor(2002,0.906,0.533,0.094)
three = ROOT.TColor(2003,0.086,0.404,0.576)
four =ROOT.TColor(2004,0.071,0.694,0.18)
five =ROOT.TColor(2005,0.388,0.098,0.608)
six=ROOT.TColor(2006,0.906,0.878,0.094)
seven=ROOT.TColor(2007,0.99,0.677,0.614)
colors = [1,2001,2002,2003,2004,2005,2006,ROOT.kGray+1, 2007,6,2,3,4,6,7,5,1,8,9,29,38,46,1,2001,2002,2003,2004,2005,2006]

f = ROOT.TFile.Open("output/averages_cfg_4_13_12.root")

def cleanHist(hist,i):
    col = colors[i]
    name = hist.GetName()
    hist.SetLineColor(col)
    hist.SetLineWidth(2)
    if i == 0:
    	hist.SetMarkerColor(col)
    	hist.SetMarkerStyle(20)
    hist.GetXaxis().SetTitleSize(0.05)
    hist.GetYaxis().SetTitleSize(0.05)
    hist.GetXaxis().SetLabelSize(0.04)
    hist.GetYaxis().SetLabelSize(0.04)
    hist.GetXaxis().SetNdivisions(505)
    hist.GetYaxis().SetNdivisions(505)
    return

def channel(name):
    if "ch0" in name : return 4
    if "ch1" in name : return 13 
    if "ch2" in name : return 12 
    else : return -1

def label(name):
	if "20325_20375" in name: return "Ch {}: 20.325 < x < 20.375 mm".format(channel(name))
	if "20375_20425" in name: return "Ch {}: 20.375 < x < 20.425 mm".format(channel(name))
	if "20425_20475" in name: return "Ch {}: 20.425 < x < 20.475 mm".format(channel(name))
	if "20475_20525" in name: return "Ch {}: 20.475 < x < 20.525 mm".format(channel(name))
	if "20525_20575" in name: return "Ch {}: 20.525 < x < 20.575 mm".format(channel(name))
	if "20575_20625" in name: return "Ch {}: 20.575 < x < 20.625 mm".format(channel(name))
	if "20625_20675" in name: return "Ch {}: 20.625 < x < 20.675 mm".format(channel(name))
	if "20675_20725" in name: return "Ch {}: 20.675 < x < 20.725 mm".format(channel(name))
	if "2064_2066" in name: return "|x-x_{center}| = 0   #mum" #.format(channel(name))
	if "2062_2064" in name: return "|x-x_{center}| = 20  #mum" #.format(channel(name))
	if "2060_2062" in name: return "|x-x_{center}| = 40  #mum" #.format(channel(name))
	if "2058_2060" in name: return "|x-x_{center}| = 60  #mum" #.format(channel(name))
	if "2056_2058" in name: return "|x-x_{center}| = 80  #mum" #.format(channel(name))
	if "2054_2056" in name: return "|x-x_{center}| = 100 #mum" #.format(channel(name))
	if "2052_2054" in name: return "|x-x_{center}| = 120 #mum" #.format(channel(name))
	if "2050_2052" in name: return "|x-x_{center}| = 140 #mum" #.format(channel(name))
	if "2048_2050" in name: return "|x-x_{center}| = 160 #mum" #.format(channel(name))
	if "2046_2048" in name: return "|x-x_{center}| = 180 #mum" #.format(channel(name))
	if "2044_2046" in name: return "|x-x_{center}| = 200 #mum" #.format(channel(name))
	if "2042_2044" in name: return "|x-x_{center}| = 220 #mum" #.format(channel(name))
	if "2040_2042" in name: return "|x-x_{center}| = 240 #mum" #.format(channel(name))
	if "2038_2040" in name: return "|x-x_{center}| = 250 #mum" #.format(channel(name))

	if "20649_20651" in name: return "|x-x_{center}| = 0 #mum"
	if "20624_20627" in name: return "|x-x_{center}| = 25 #mum"
	if "20599_20601" in name: return "|x-x_{center}| = 50 #mum"
	if "20574_20576" in name: return "|x-x_{center}| = 75 #mum"
	if "20549_20551" in name: return "|x-x_{center}| = 100 #mum"
	if "20524_20527" in name: return "|x-x_{center}| = 125 #mum"
	if "20499_20501" in name: return "|x-x_{center}| = 150 #mum"
	if "20474_20576" in name: return "|x-x_{center}| = 175 #mum"
	if "20449_20451" in name: return "|x-x_{center}| = 200 #mum"


def get_x(name):
	if "20649_20651" in name: return 0 #mum
	if "20624_20627" in name: return 25 #mum
	if "20599_20601" in name: return 50 #mum
	if "20574_20576" in name: return 75 #mum
	if "20549_20551" in name: return 100 #mum
	if "20524_20527" in name: return 125 #mum
	if "20499_20501" in name: return 150 #mum
	if "20474_20576" in name: return 175 #mum
	if "20449_20451" in name: return 200 #mum


def make_graph(names,filename):
   	    
	vals = []
	xs =[]
	valerrs = []
	xerrs = []
	for name in names: 

		hist = f.Get(name)
		vals.append(hist.GetMean())
		xs.append(get_x(name))
		xerrs.append(1.0)
		valerrs.append(hist.GetMeanError())

		#c = ROOT.TCanvas()
		#hist.Draw()
		#c.Print("averages/"+name+".png")

	graph = ROOT.TGraphErrors(len(xs),array("d",xs),array("d",vals),array("d",xerrs),array("d",valerrs))
	graph.SetName("gr{}".format(filename))
	yaxis = "mean t_{peak}-t_{photek} [ns]"
	if "amp" in filename: yaxis = "mean amplitude [mV]"
	xaxis = "|x-x_{center}| [#mum]"
	graph.SetTitle(";{};{}".format(xaxis,yaxis))

	c = ROOT.TCanvas(filename)
	#c.SetGridy()
	#c.SetGridx()
	#cosmetic_tgraph(graph,i,tb)
	graph.Draw("AEP")
	c.Print("averages/graph_%s.png"%(filename))

	return

def make_profile(name):
   
    c = ROOT.TCanvas(name,"",800,600)
    c.SetLeftMargin(0.15)
    hist = f.Get(name)
        
    #hist.RebinX()
    #hist.RebinY()
    profile = hist.ProfileX("profx"+name)
    #profile.Rebin()
    cleanHist(profile,0)

    if "tpeak" in name: hist.GetYaxis().SetTitle("t_{peak} - t_{photek} [ns]")
    if "tdiff" in name: hist.GetYaxis().SetTitle("t_{CFD20} - t_{photek} [ns]")
    if "amp"   in name: hist.GetYaxis().SetTitle("amplitude [mV]")

    hist.Draw("COLZ")
    profile.Draw("same")
    c.Print("averages/{}.png".format(name))

    profile.GetXaxis().SetTitle("|x-x_{center}| [#mum]")
    profile.GetXaxis().SetRangeUser(0,150)
    profile.GetYaxis().SetTitleOffset(1.3)

    if "tpeak" in name: profile.GetYaxis().SetRangeUser(3.75,3.95)
    if "tpeak" in name and "ch1" in name : profile.GetYaxis().SetRangeUser(3.79,3.90)
    if "tpeak" in name: profile.GetYaxis().SetTitle("t_{peak} - t_{photek} [ns]")
    if "tdiff" in name: profile.GetYaxis().SetTitle("t_{CFD20} - t_{photek} [ns]")
    if "amp"   in name: profile.GetYaxis().SetTitle("amplitude [mV]")

    hist.Draw("COLZ")
    profile.Draw("")
    c.Print("averages/prof_{}.png".format(name))

    return

def overlay_ratios(names,filename):
   
	c = ROOT.TCanvas()
	profiles = []
	labels = []
	for i,name in enumerate(names):
	    hist = f.Get(name)
	
	    labels.append(label(name))
	    
	    #hist.RebinX()
	    #hist.RebinY()
	    profile = hist.ProfileX("profx"+name)
	    #profile.Rebin()
	    profiles.append(profile)
	    cleanHist(profile,0)

	    hist.Draw("COLZ")
	    profile.Draw("same")
	    c.Print("averages/{}_prof.png".format(name))

	    
	c = ROOT.TCanvas()
	dy = 0.04*len(profiles)
	leg1 = ROOT.TLegend(0.65,0.5-dy,0.88,0.5)
	leg1.SetBorderSize(0)
	leg2 = ROOT.TLegend(0.15,0.5-dy,0.43,0.5)
	leg2.SetBorderSize(0)
	for i,profile in enumerate(profiles):
	    cleanHist(profile,i+1)
	    
	    #profile.GetYaxis().SetRangeUser(0,1.1)

	    leg1.AddEntry(profile,labels[i],"l")
	    leg2.AddEntry(profile,labels[i],"l")
	    if i==0 : profile.Draw("histc") 
	    else : profile.Draw("histcsame")

	profiles[0].GetXaxis().SetTitle("time - t_{photek} [s]")
	profiles[0].GetYaxis().SetTitle("channel 13 voltage [mV]")

	profiles[0].GetYaxis().SetRangeUser(-700,50)
	profiles[0].GetXaxis().SetRangeUser(2.5e-9,6.0e-9)

	leg1.Draw()
	c.Print("averages/{}.png".format(filename))
	#leg1.Clear()

	profiles[0].GetYaxis().SetRangeUser(-600,50)
	profiles[0].GetXaxis().SetRangeUser(3.0e-9,4.9e-9)
	leg1.Draw()
	c.Print("averages/{}zoom.png".format(filename))

	return
# main 
names = []
make_profile("h_ch0_amp_v_x")	
make_profile("h_ch0_tpeak_v_x")
make_profile("h_ch0_tdiff_v_x")


make_profile("h_ch1_amp_v_x")	
make_profile("h_ch1_tpeak_v_x")
make_profile("h_ch1_tdiff_v_x")

make_profile("h_ch2_amp_v_x")	
make_profile("h_ch2_tpeak_v_x")
make_profile("h_ch2_tdiff_v_x")
#
##names.append("h_sig_ch1_20675_20725")
#names.append("h_sig_ch1_20625_20675")
#names.append("h_sig_ch1_20575_20625")
#names.append("h_sig_ch1_20525_20575")
#names.append("h_sig_ch1_20475_20525")
##names.append("h_sig_ch1_20375_20425")
##names.append("h_sig_ch1_20425_20475")
##names.append("h_sig_ch1_20325_20375")

#names.append("h_sig_ch1_2038_2040")	
#names.append("h_sig_ch1_2040_2042")	
#names.append("h_sig_ch1_2042_2044")	
#names.append("h_sig_ch1_2044_2046")	
#names.append("h_sig_ch1_2046_2048")	
#names.append("h_sig_ch1_2048_2050")

#names.append("h_sig_ch1_2064_2066")
#names.append("h_sig_ch1_2062_2064")	
#names.append("h_sig_ch1_2060_2062")	
#names.append("h_sig_ch1_2058_2060")	
#names.append("h_sig_ch1_2056_2058")	
#names.append("h_sig_ch1_2054_2056")	
#names.append("h_sig_ch1_2052_2054")	
#names.append("h_sig_ch1_2050_2052")	


#names.append("h_sig_ch1_20649_20651")
#names.append("h_sig_ch1_20599_20601")
#names.append("h_sig_ch1_20549_20551")
#names.append("h_sig_ch1_20499_20501")
##names.append("h_sig_ch1_20449_20451")
#overlay_ratios(names,"average_signal_ch1")
names = []
names.append("h_sig_ch1_20649_20651")
names.append("h_sig_ch1_20624_20627")
names.append("h_sig_ch1_20599_20601")
names.append("h_sig_ch1_20574_20576")
names.append("h_sig_ch1_20549_20551")
names.append("h_sig_ch1_20524_20527")
names.append("h_sig_ch1_20499_20501")
#names.append("h_sig_ch1_20474_20576")
#names.append("h_sig_ch1_20449_20451")
overlay_ratios(names,"average_signal_ch1")

names = []
names.append("h_sig_ch1_20649_20651_tpeak")
names.append("h_sig_ch1_20624_20627_tpeak")
names.append("h_sig_ch1_20599_20601_tpeak")
names.append("h_sig_ch1_20574_20576_tpeak")
names.append("h_sig_ch1_20549_20551_tpeak")
names.append("h_sig_ch1_20524_20527_tpeak")
names.append("h_sig_ch1_20499_20501_tpeak")
#names.append("h_sig_ch1_20474_20576_tpeak")
#names.append("h_sig_ch1_20449_20451_tpeak")
make_graph(names,"tpeak_v_x")

names = []
names.append("h_sig_ch1_20649_20651_amp")
names.append("h_sig_ch1_20624_20627_amp")
names.append("h_sig_ch1_20599_20601_amp")
names.append("h_sig_ch1_20574_20576_amp")
names.append("h_sig_ch1_20549_20551_amp")
names.append("h_sig_ch1_20524_20527_amp")
names.append("h_sig_ch1_20499_20501_amp")
#names.append("h_sig_ch1_20474_20576_amp")
#names.append("h_sig_ch1_20449_20451_amp")
make_graph(names,"amp_v_x")

