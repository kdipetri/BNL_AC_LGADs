
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
colors = [1,2001,2002,2003,2004,2005,2006,2007,6,2,3,4,6,7,5,1,8,9,29,38,46,1,2001,2002,2003,2004,2005,2006]
f = ROOT.TFile.Open("output/hists_cfg_4_13_12.root")


def clean_hist(hist):
	hist.GetXaxis().SetTitleSize(0.06)
	hist.GetYaxis().SetTitleSize(0.06)
	hist.GetZaxis().SetTitleSize(0.06)

	hist.GetXaxis().SetLabelSize(0.05)
	hist.GetYaxis().SetLabelSize(0.05)
	hist.GetZaxis().SetLabelSize(0.05)

	hist.GetXaxis().SetNdivisions(505)
	hist.GetYaxis().SetNdivisions(505)
	hist.GetZaxis().SetNdivisions(505)

	return

def clean_profile(hist):
	hist.GetXaxis().SetTitleSize(0.06)
	hist.GetYaxis().SetTitleSize(0.06)

	hist.GetXaxis().SetLabelSize(0.05)
	hist.GetYaxis().SetLabelSize(0.05)

	hist.GetXaxis().SetNdivisions(505)
	hist.GetYaxis().SetNdivisions(505)

	hist.SetLineColor(ROOT.kBlack)
	hist.SetMarkerColor(ROOT.kBlack)
	hist.SetMarkerStyle(20)

	return

def make_profile(name):

	c = ROOT.TCanvas()
	hist = f.Get(name)

	if "v_amp"      in name: hist.GetXaxis().SetRangeUser(0,1000)
	if "v_xdiff"    in name: hist.GetXaxis().SetTitle("|x-x_{center}| [#mum]")
	hist.RebinX()

	prof = hist.ProfileX("prof"+name)
	clean_hist(hist)
	clean_profile(prof)

	hist.Draw("COLZ")
	prof.Draw("same")

	c.Print("plots/signalshape/"+name+".png")
	yaxis = hist.GetYaxis().GetTitle()
	prof.GetYaxis().SetTitle("mean " + yaxis)
	if "deltaTpeakun" in name: prof.GetYaxis().SetRangeUser(3.65,3.95)
	if "risetime"     in name: prof.GetYaxis().SetRangeUser(0.45,0.65)
	if "v_amp"        in name: prof.GetXaxis().SetRangeUser(0,1000)
	clean_profile(prof)
	
	prof.Draw()
	c.Print("plots/signalshape/prof_"+name+".png")
 

for ch in range(0,3):

	make_profile("timeres_ch{}_amp_v_xdiff"     .format(ch))  
	make_profile("timeres_ch{}_tpeak_v_xdiff"   .format(ch))  
	make_profile("timeres_ch{}_risetime_v_xdiff".format(ch)) 

	make_profile("timeres_ch{}_amp_v_xpos"         .format(ch))  
	make_profile("timeres_ch{}_deltaTpeakun_v_xpos".format(ch))  
	make_profile("timeres_ch{}_deltaTun_v_xpos"    .format(ch))  
	make_profile("timeres_ch{}_risetime_v_xpos"    .format(ch))  

	make_profile("timeres_ch{}_deltaTpeakun_v_amp".format(ch)) 
	make_profile("timeres_ch{}_deltaTun_v_amp"    .format(ch)) 
	make_profile("timeres_ch{}_risetime_v_amp"    .format(ch)) 



