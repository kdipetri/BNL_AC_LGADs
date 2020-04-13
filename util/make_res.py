import ROOT
#ROOT.gStyle.SetOptStat(0)
#ROOT.gStyle.SetOptFit(0)
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
colors = [1,2001,2002,2003,2004,2005,2006,6,2,3,4,6,7,5,1,8,9,29,38,46,1,2001,2002,2003,2004,2005,2006]

f = ROOT.TFile.Open("hists.root")

def clean(hist,opt=""):
    hist.GetXaxis().SetTitleOffset(1.0)
    hist.GetXaxis().SetNdivisions(505)
    hist.GetYaxis().SetNdivisions(505)

    hist.GetXaxis().SetTitleSize(0.05)
    hist.GetYaxis().SetTitleSize(0.05)
    hist.GetXaxis().SetLabelSize(0.04)
    hist.GetYaxis().SetLabelSize(0.04)

    if "2D" in opt:
        hist.GetZaxis().SetNdivisions(505)
        hist.GetZaxis().SetTitleSize(0.05)
        hist.GetZaxis().SetLabelSize(0.04)
    if "hit_xdiff"   in hist.GetName(): hist.SetBinContent(hist.GetNbinsX(),0)
    return

def clean1D(histname):
    c = ROOT.TCanvas()
    hist = f.Get(histname)
    clean(hist)
    hist.Draw()
    c.Print("plots/clean_{}.png".format(histname))
    return 

def clean2D(histname):
    c = ROOT.TCanvas(histname,"",900,800)
    c.SetTopMargin(0.1)
    c.SetLeftMargin(0.2)
    c.SetBottomMargin(0.2)
    #c.SetRightMargin(0.2)
    hist = f.Get(histname)
    clean(hist)
    hist.Draw("COLZ")
    c.Print("plots/clean_{}.png".format(histname))
    return

names = []
names.append("three_hit_xdiff_0_1")
names.append("three_hitweighted_xdiff")	
names.append("two_hit_xdiff_0_2")	
names.append("two_hit_xdiff_0_1")	
names.append("two_hit_xdiff_1_2")	
names.append("two_hitweighted_xdiff_0_1")	
names.append("two_hitweighted_xdiff_0_2")	
names.append("two_hitweighted_xdiff_1_2")	
for name in names: 
    clean1D(name)
names = []
names.append("three_hit_xmeas_xtrack_0_1")
names.append("three_hitweighted_xmeas_xtrack")	
names.append("two_hit_xmeas_xtrack_0_2")	
names.append("two_hit_xmeas_xtrack_0_1")	
names.append("two_hit_xmeas_xtrack_1_2")	
names.append("two_hitweighted_xmeas_xtrack_0_1")	
names.append("two_hitweighted_xmeas_xtrack_0_2")	
names.append("two_hitweighted_xmeas_xtrack_1_2")
for name in names: 
    clean2D(name)
