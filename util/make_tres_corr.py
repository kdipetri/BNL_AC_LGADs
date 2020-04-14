import ROOT
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
colors = [1,2001,2002,2003,2004,2005,2006,6,2,3,4,6,7,5,1,8,9,29,38,46,1,2001,2002,2003,2004,2005,2006]

f = ROOT.TFile.Open("output/hists_cfg_4_13_12.root")
fout = ROOT.TFile.Open("profiles/tres_corr_safe.root","RECREATE")

def cleanHist2D(hist):
    hist.GetXaxis().SetTitleSize(0.06)
    hist.GetYaxis().SetTitleSize(0.06)
    hist.GetZaxis().SetTitleSize(0.06)
    hist.GetXaxis().SetLabelSize(0.05)
    hist.GetYaxis().SetLabelSize(0.05)
    hist.GetZaxis().SetLabelSize(0.05)
    hist.GetXaxis().SetNdivisions(505)
    hist.GetYaxis().SetNdivisions(505)
    return

def cleanHist(hist,i):
    col = colors[i]
    name = hist.GetName()
    hist.SetLineColor(col)
    hist.SetMarkerColor(col)
    hist.SetMarkerStyle(20)
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

def make_correction(name):
   
    c = ROOT.TCanvas(name,"",900,800)
    c.SetTopMargin(0.1)
    c.SetLeftMargin(0.2)
    c.SetBottomMargin(0.2)

    hist = f.Get(name)
    cleanHist2D(hist)
        
    profile = hist.ProfileX("profx"+name)
    cleanHist(profile,0)

    hist.Draw("COLZ")
    profile.Draw("same")
    c.Print("profiles/{}_prof.png".format(name))

        
    fout.cd()
    profile.Write()
    hist.Write()


# main 
make_correction("timeres_ch0_deltaTun_v_amp")
make_correction("timeres_ch1_deltaTun_v_amp")
make_correction("timeres_ch2_deltaTun_v_amp")
