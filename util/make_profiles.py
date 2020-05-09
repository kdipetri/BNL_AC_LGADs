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
colors = [1,2001,2002,2003,2004,2005,2006,6,2,3,4,6,7,5,1,8,9,29,38,46,1,2001,2002,2003,2004,2005,2006]

#f = ROOT.TFile.Open("profiles/amplitudes_safe.root")
f = ROOT.TFile.Open("output/hists_cfg_4_13_12.root")
fout = ROOT.TFile.Open("profiles/profiles_safe.root","RECREATE")

def cleanHist(hist,i):
    col = colors[i]
    name = hist.GetName()
    hist.SetLineColor(col)
    hist.SetLineWidth(2)
    #hist.SetMarkerColor(col)
    #hist.SetMarkerStyle(20)
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

def overlay_ratios(names,filename):
   
    c = ROOT.TCanvas()
    profiles = []
    labels = []
    chs = []
    for i,name in enumerate(names):
        hist = f.Get(name)
        ch1 = name.split("_")[5].strip("ch")
        ch2 = name.split("_")[6].strip("ch")
        chs.append(int(ch1))
        if i==0 : label = "Center Strip"
        if i==1 : label = "Right Strip"
        if i==2 : label = "Left Strip"
        labels.append(label)
        
        #hist.RebinX()
        #hist.RebinY()
        profile = hist.ProfileX("profx"+name)
        #profile.Rebin()
        profiles.append(profile)
        cleanHist(profile,0)

        hist.Draw("COLZ")
        profile.Draw("same")
        c.Print("profiles/{}_prof.png".format(name))

        
    c = ROOT.TCanvas()
    dy = 0.06*len(profiles)
    leg = ROOT.TLegend(0.5,0.86-dy,0.86,0.86)
    leg.SetBorderSize(0)
    leg.SetTextSize(0.05)
    for i,profile in enumerate(profiles):

        #profile.GetXaxis().SetRangeUser(20.4,20.7)
        profile.GetXaxis().SetRangeUser(20.42,20.68)
        profile.GetYaxis().SetRangeUser(0,1)
        profile.GetYaxis().SetTitle("Mean Amplitude Fraction")
        profile.GetXaxis().SetTitle("x [mm]")
        cleanHist(profile,chs[i]+1)
        profile.GetXaxis().SetNdivisions(510)
        if i==0 : profile.Draw("histe") 
        else : profile.Draw("histesame")
        #if i==0 : profile.Draw("hist") 
        #else : profile.Draw("histsame")

        leg.AddEntry(profile,labels[i],"l")

        fout.cd()
        profile.Write()


    txt = ROOT.TPaveText(0.20,0.86,0.5,0.86-0.06, "NDC")
    txt.SetTextAlign(13)
    txt.SetTextFont(42)
    txt.SetTextSize(0.05)
    txt.SetFillColor(0)
    txt.SetBorderSize(0)
    txt.AddText("Three hit clusters");
    txt.Draw()
    txt.Draw()
    leg.Draw()
    c.Print("profiles/{}.png".format(filename))
    c.Print("profiles/{}.pdf".format(filename))

# main 
#names = []
#names.append("charge_ratio_v_x_any_0_1")	
#names.append("charge_ratio_v_x_any_0_2")	
#names.append("charge_ratio_v_x_any_1_0")	
#names.append("charge_ratio_v_x_any_1_2")	
#names.append("charge_ratio_v_x_any_2_0")	
#names.append("charge_ratio_v_x_any_2_1")
#overlay_ratios(names,"charge_ratio_v_x_overlay")

#names = []
#names.append("charge_fraction_v_x_two_0_1")	
#names.append("charge_fraction_v_x_two_1_0")	
#overlay_ratios(names,"charge_fraction_two_v_x_0_1")
#
#names = []
#names.append("charge_fraction_v_x_two_0_2")	
#names.append("charge_fraction_v_x_two_2_0")	
#overlay_ratios(names,"charge_fraction_two_v_x_0_2")

#names.append("charge_fraction_v_x_two_2_1")
#names.append("charge_fraction_v_x_two_1_2")	

names = []
names.append("chargesharing_threehits_0_1_2_ch0_chargefrac_v_xpos")	
names.append("chargesharing_threehits_0_1_2_ch1_chargefrac_v_xpos")	
names.append("chargesharing_threehits_0_1_2_ch2_chargefrac_v_xpos")
overlay_ratios(names,"charge_fraction_three_v_x_overlay")
