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

f = ROOT.TFile.Open("output/averages_cfg_4_13_DC.root")

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
	if "ch0" in name: return "Channel 4"
	if "ch1" in name: return "Channel 13"
	if "ch2" in name: return "DC pad"
	if "ch3" in name: return "Photek"
	return ""

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
	    if i==0 : cleanHist(profile,i+1)
	    if i==1 : cleanHist(profile,i+2)
	    if i==2 : cleanHist(profile,0)
	     
	    #profile.GetYaxis().SetRangeUser(0,1.1)

	    leg1.AddEntry(profile,labels[i],"l")
	    leg2.AddEntry(profile,labels[i],"l")
	    if i==0 : profile.Draw("histc") 
	    else : profile.Draw("histcsame")

	profiles[0].GetXaxis().SetTitle("Time - t_{photek} [ns]")
	profiles[0].GetYaxis().SetTitle("Voltage [mV]")

	profiles[0].GetYaxis().SetRangeUser(-100,50)
	profiles[0].GetXaxis().SetRangeUser(-3.0,7.0)
	profiles[0].GetYaxis().SetTitleOffset(1.4)

	leg1.Draw()
	c.Print("averages/{}.png".format(filename))
	c.Print("averages/{}.pdf".format(filename))
	#leg1.Clear()

	profiles[0].GetYaxis().SetRangeUser(-100,50)
	profiles[0].GetXaxis().SetRangeUser(-2.0,4.0)
	leg1.Draw()
	c.Print("averages/{}zoom.png".format(filename))
	c.Print("averages/{}zoom.pdf".format(filename))

	return

names=[]
names.append("h_nomini_ch4hit_ch0")	
names.append("h_nomini_ch4hit_ch2")	 	
#names.append("h_nomini_ch4hit_ch3")	 	
cluster_waveforms(names,"nominicircuit_ch4hit")

names=[]
names.append("h_nomini_dchit_ch0")	
names.append("h_nomini_dchit_ch2")
#names.append("h_nomini_dchit_ch3")
cluster_waveforms(names,"nominicircuit_dchit")


