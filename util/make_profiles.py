import ROOT
ROOT.gStyle.SetOptStat(0)
ROOT.gStyle.SetOptFit(0)
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
    dy = 0.07 #*len(profiles)
    leg = ROOT.TLegend(0.2,0.8-dy,0.8,0.86)
    leg.SetBorderSize(0)
    leg.SetTextSize(0.05)
    leg.SetNColumns(3)
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

        

        fout.cd()
        profile.Write()

    leg.AddEntry(profiles[2],labels[2],"l")
    leg.AddEntry(profiles[0],labels[0],"l")
    leg.AddEntry(profiles[1],labels[1],"l")

    ymax=1
    lin1 = ROOT.TLine(20.55+0.04,0,20.55+0.04,ymax)
    lin1.SetLineStyle(3)
    lin1.SetLineColor(colors[1])
    lin1.Draw()
    lin2 = ROOT.TLine(20.55-0.04,0,20.55-0.04,ymax)
    lin2.SetLineStyle(3)
    lin2.SetLineColor(colors[1])
    lin2.Draw()
    lin3 = ROOT.TLine(20.65+0.04,0,20.65+0.04,ymax)
    lin3.SetLineStyle(3)
    lin3.SetLineColor(colors[2])
    #lin3.Draw()
    lin4 = ROOT.TLine(20.65-0.04,0,20.65-0.04,ymax)
    lin4.SetLineStyle(3)
    lin4.SetLineColor(colors[2])
    lin4.Draw()
    lin5 = ROOT.TLine(20.45+0.04,0,20.45+0.04,ymax)
    lin5.SetLineStyle(3)
    lin5.SetLineColor(colors[3])
    lin5.Draw()
    lin6 = ROOT.TLine(20.45-0.04,0,20.45-0.04,ymax)
    lin6.SetLineStyle(3)
    lin6.SetLineColor(colors[3])
    #lin6.Draw()

    #txt = ROOT.TPaveText(0.20,0.86-0.07,0.5,0.86, "NDC")
    #txt.SetTextAlign(12)
    #txt.SetTextFont(42)
    #txt.SetTextSize(0.05)
    #txt.SetFillColor(0)
    #txt.SetBorderSize(0)
    #txt.AddText("Three hit clusters");
    #txt.Draw()
    #txt.Draw()
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
