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
ROOT.gStyle.SetPadTickX(1)
ROOT.gStyle.SetPadTickY(1)
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
    hist.GetXaxis().SetTitleSize(0.06)
    hist.GetYaxis().SetTitleSize(0.06)
    hist.GetXaxis().SetLabelSize(0.05)
    hist.GetYaxis().SetLabelSize(0.05)
    hist.GetXaxis().SetNdivisions(505)
    hist.GetYaxis().SetNdivisions(505)
    return

def channel(name):
    if "ch0" in name : return 4
    if "ch1" in name : return 13 
    if "ch2" in name : return 12 
    else : return -1

def label(name):
	if "h_threehits_sig_ch0" in name: return "Channel 4"
	if "h_threehits_sig_ch1" in name: return "Channel 13"
	if "h_threehits_sig_ch2" in name: return "Channel 12"
	if "h_threehits_sig_ch3" in name: return "Photek"

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

	    
	c = ROOT.TCanvas(filename,"",800,800)
	c.SetLeftMargin(0.2)
	c.SetBottomMargin(0.21)
	c.SetRightMargin(0.12)
	dy = 0.04*len(profiles)
	leg1 = ROOT.TLegend(0.5,0.55-dy,0.86,0.55)
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

	profiles[0].GetXaxis().SetTitle("Time - t_{photek} [ns]")
	profiles[0].GetYaxis().SetTitle("Voltage [mV]")
	profiles[0].GetYaxis().SetTitleOffset(1.5)

	profiles[0].GetYaxis().SetRangeUser(-700,100)
	profiles[0].GetXaxis().SetRangeUser(0.0,10.0)

	leg1.Draw()
	c.Print("averages/{}.png".format(filename))
	c.Print("averages/{}.pdf".format(filename))
	#leg1.Clear()

	profiles[0].GetYaxis().SetRangeUser(-600,50)
	profiles[0].GetXaxis().SetRangeUser(3.0,4.9)
	leg1.Draw()
	c.Print("averages/{}zoom.png".format(filename))
	c.Print("averages/{}zoom.pdf".format(filename))

	return


def cluster_waveforms(names,filename):
   
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

	    
	c = ROOT.TCanvas(filename,"",800,800)
	c.SetLeftMargin(0.2)
	c.SetBottomMargin(0.21)
	c.SetRightMargin(0.12)
	dy = 0.05*len(profiles)
	leg1 = ROOT.TLegend(0.55,0.5-dy,0.86,0.5)
	leg1.SetBorderSize(0)
	leg1.SetTextSize(0.045)
	leg2 = ROOT.TLegend(0.15,0.5-dy,0.43,0.5)
	leg2.SetBorderSize(0)
	leg2.SetTextSize(0.045)

	for i,profile in enumerate(profiles):
	    cleanHist(profile,i+1)
	    
	    #profile.GetYaxis().SetRangeUser(0,1.1)

	    leg1.AddEntry(profile,labels[i],"l")
	    leg2.AddEntry(profile,labels[i],"l")
	    if i==0 : profile.Draw("histc") 
	    else : profile.Draw("histcsame")

	profiles[0].GetXaxis().SetTitle("Time - t_{photek} [ns]")
	profiles[0].GetYaxis().SetTitle("Voltage [mV]")

	profiles[0].GetYaxis().SetRangeUser(-700,100)
	profiles[0].GetXaxis().SetRangeUser(0.0,10.0)
	profiles[0].GetYaxis().SetTitleOffset(1.4)

	leg1.Draw()
	c.Print("averages/{}.png".format(filename))
	c.Print("averages/{}.pdf".format(filename))
	#leg1.Clear()

	profiles[0].GetYaxis().SetRangeUser(-700,50)
	profiles[0].GetXaxis().SetRangeUser(3.0,4.9)
	leg1.Draw()
	c.Print("averages/{}zoom.png".format(filename))
	c.Print("averages/{}zoom.pdf".format(filename))

	return

names=[]
names.append("h_threehits_sig_ch0")
names.append("h_threehits_sig_ch1")
names.append("h_threehits_sig_ch2")
#names.append("h_threehits_sig_ch3")
cluster_waveforms(names,"threehit_signals")

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

