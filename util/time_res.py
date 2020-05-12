import ROOT
ROOT.gStyle.SetOptStat(1)
ROOT.gStyle.SetOptFit(1)
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
fout = ROOT.TFile.Open("profiles/tres_corr2_safe.root","RECREATE")
def cleanHist(hist,i):
    col = colors[i]
    name = hist.GetName()
    hist.SetLineColor(col)
    hist.SetMarkerColor(col)
    hist.SetMarkerStyle(20)
    hist.SetFillColor(0)
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
    if "ch0" in name: return 1
    if "ch1" in name: return 2
    if "ch2" in name: return 3
    if "comb" in name: return 0
    return 0

def label(obj):
    name = obj.GetName()
    if "ch0" in name: return "Channel 4"
    if "ch1" in name: return "Channel 13"
    if "ch2" in name: return "Channel 12"
    if "comb" in name: return "Weighted Avg"
    if "lead" in name: 
        ch = name.split("_")[2]
        if "0" in ch: return "Channel 4"
        if "1" in ch: return "Channel 13"
        if "2" in ch: return "Channel 12"        
    return ""

def paper_label(i,len):
    if len == 3: 
        if i==2: return "Leading Channel"
        if i==1: return "2nd Leading Channel"
        if i==0: return "3rd Leading Channel"
    elif len==2:
        if i==1: return "Leading Channel"
        if i==0: return "2rd Leading Channel"        
    return ""

def get_time_res(name):
    ROOT.gStyle.SetOptStat(0)
    ROOT.gStyle.SetOptFit(0)
    hist = f.Get(name)
    #if "deltaT" in name: hist.Rebin()
    c = ROOT.TCanvas()
    
    if "deltaT" in name: 
        leg = ROOT.TLegend(0.6,0.86-0.12,0.86,0.86)
    else : 
        leg = ROOT.TLegend(0.6,0.86-0.06,0.86,0.86)
    leg.SetBorderSize(0)
    leg.SetTextSize(0.045)
    cleanHist(hist,0)
    hist.Draw("hist") 
    hist.GetYaxis().SetRangeUser(0,(hist.GetMaximum()*1.2))
    hist.GetYaxis().SetTitle("Events")
    lab1 = label(hist)


    if "deltaT"  in name: 
        #hist.Rebin()
        if "deltaTun" in name : f1 = ROOT.TF1("f1_"+name,"gaus",2.85,3.85)
        else                  : f1 = ROOT.TF1("f1_"+name,"gaus",-0.5,0.5)              
        hist.Fit(f1,"Q")
        f1.Draw("same")
        t0_str = "#sigma={:.1f} ps".format(f1.GetParameter(2)*1e3)
        hist.GetXaxis().SetTitle("t_{0}-t_{ref} [ns]")
        #t0_str = "#mu={:.1f} ps, #sigma={:.1f} ps".format(f1.GetParameter(1)*1e3, f1.GetParameter(2)*1e3)
        #lab += t0_str

    if "xpos" not in name:
        leg.AddEntry(hist,lab1,"l")
        if "deltaT" in name: leg.AddEntry(f1,t0_str,"l")
        leg.Draw()
    else:
        hist.Rebin()

    c.Print("plots/time_res/{}.png".format(name))
    if "deltaT" in name: 
        fout.cd()
        hist.Write()
        f1.Write()
        return hist,f1
    else : return hist

    
# main 
def get_comparison(hists,fits,name):
    ROOT.gStyle.SetOptStat(0)
    ROOT.gStyle.SetOptFit(0)
    c = ROOT.TCanvas(name,"",900,800)
    c.SetLeftMargin(0.2)

    dy = len(hists)*0.06
    if "deltaT" in name     : leg = ROOT.TLegend(0.25,0.86-dy,0.86,0.86)
    elif "slewrate" in name : leg = ROOT.TLegend(0.35,0.86-dy,0.86,0.86)
    elif "rmsnoise" in name : leg = ROOT.TLegend(0.35,0.86-dy,0.86,0.86)
    else                    : leg = ROOT.TLegend(0.55,0.86-dy,0.86,0.86)
    leg.SetBorderSize(0)
    leg.SetTextSize(0.045)

    ymax = 0
    labels=[]
    for i,hist in enumerate(hists): 
        
        # color handling
        icol = colorindex(hist)
        cleanHist(hist,icol)
        if i==0: hist.Draw("hist")
        else : hist.Draw("histsame")
        labels.append(paper_label(i,len(hists)))
        if hist.GetMaximum() > ymax: ymax = hist.GetMaximum()
        if len(fits) > 0: 
            fits[i].SetLineColor(colors[icol])
            #fits[i].SetLineStyle(1)
            fits[i].Draw("same")
            #t0_str = ": #sigma={:.1f} ps".format(fits[i].GetParameter(2)*1e3)
            #t0_str = ": t0={:.1f} ps, #sigma={:.1f} ps".format(fits[i].GetParameter(1)*1e3, fits[i].GetParameter(2)*1e3)
            #lab+= t0_str
        #if "slewrate" in name :
        #    mean = hist.GetMean()
        #    mean_str = ": mean slewrate = {:.0f} [mV/ns]".format(mean)
        #    lab+= mean_str
        #if "rmsnoise" in name:
        #    mean = hist.GetMean()
        #    mean_str = ": rms noise = {:.0f} [mV]".format(mean)
        #    lab+= mean_str

        #leg.AddEntry(hist,lab, "l") 
    if len(hists)==3: leg.AddEntry(hists[2],labels[2],"l")
    leg.AddEntry(hists[1],labels[1],"l")
    leg.AddEntry(hists[0],labels[0],"l")
    leg.Draw()
    hists[0].SetMaximum(ymax*1.3)
    if "deltaTcor" in name: hists[0].GetXaxis().SetRangeUser(-0.4,0.4)
    c.Print("plots/time_res/compare_{}.png".format(name))
    c.Print("plots/time_res/compare_{}.pdf".format(name))


def get_comparison_timeCorr(hists,name):
    ROOT.gStyle.SetOptStat(0)
    ROOT.gStyle.SetOptFit(0)
    c = ROOT.TCanvas(name,"",900,800)
    c.SetLeftMargin(0.2)

    dy = (len(hists)-1)*0.06
    if "deltaT" in name     : leg = ROOT.TLegend(0.25,0.86-dy,0.86,0.86)
    leg.SetBorderSize(0)
    leg.SetTextSize(0.045)

    #hists[0].Add(hists[1])
    ymax = 0
    labels=[]
    for i,hist in enumerate(hists): 
        if i==0: continue
        
        # color handling
        if i==1 :icol = 3
        else : icol = 1 
        #hist.Scale(1.0/hist.Integral(0,-1))
        cleanHist(hist,icol)
        if i==1: hist.Draw("hist")
        else : hist.Draw("histsame")
        labels.append(paper_label(i,len(hists)))
        if hist.GetMaximum() > ymax: ymax = hist.GetMaximum()
        f1 = ROOT.TF1("f1_"+hist.GetName(),"gaus",-0.5,0.5)    
        hist.Fit(f1,"Q")
        f1.SetLineColor(colors[icol])      
        f1.Draw("same")

    leg.AddEntry(hists[2],"Leading Strip","l")
    leg.AddEntry(hists[1],"Subleading Strip","l")
    #leg.AddEntry(hists[0],"Subleading Strip","l")
    leg.Draw()
    hists[1].SetMaximum(ymax*1.2)
    hists[1].GetYaxis().SetTitle("Events")
    if "deltaTcor" in name: hists[0].GetXaxis().SetRangeUser(-0.4,0.4)
    c.Print("plots/time_res/clean_compare_{}.png".format(name))
    c.Print("plots/time_res/clean_compare_{}.pdf".format(name))



hist1, fit1 = get_time_res("timeres_twohits_%i_%i_ch%i_deltaTun"%(0,1,0))
hist2, fit2 = get_time_res("timeres_twohits_%i_%i_ch%i_deltaTun"%(1,0,0))
two_hists   = [hist1,hist2]
two_fits    = [fit1 ,fit2 ]
get_comparison(two_hists,two_fits,"compare_twohits_%i_%i_ch%i_deltaTun"%(0,1,1))




for ch1 in range(0,3):
    for ch2 in range(0,3):
        if ch1==ch2: continue
        #"timeres_twohits_2_0_ch0_amp"
        #"timeres_twohits_2_0_ch2_amp"
        #"timeres_twohits_2_0_ch2_xpos"
        if ch1==1 and ch2==2: continue
        if ch1==2 and ch2==1: continue

        hist1, fit1 = get_time_res("timeres_twohits_%i_%i_ch%i_deltaTun"%(ch1,ch2,ch1))
        hist2, fit2 = get_time_res("timeres_twohits_%i_%i_ch%i_deltaTun"%(ch1,ch2,ch2))
        two_hists   = [hist2,hist1]
        two_fits    = [fit2 ,fit1 ]
        get_comparison(two_hists,two_fits,"timeres_twohits_%i_%i_deltaTun"%(ch1,ch2))

        hist1, fit1 = get_time_res("timeres_twohits_%i_%i_ch%i_deltaTcor"%(ch1,ch2,ch1))
        hist2, fit2 = get_time_res("timeres_twohits_%i_%i_ch%i_deltaTcor"%(ch1,ch2,ch2))
        two_hists   = [hist2,hist1]
        two_fits    = [fit2 ,fit1 ]
        get_comparison(two_hists,two_fits,"timeres_twohits_%i_%i_deltaTcorr"%(ch1,ch2))

        #hist1, fit1 = get_time_res("timeres_twohits_%i_%i_ch%i_deltaTcor2"%(ch1,ch2,ch1))
        #hist2, fit2 = get_time_res("timeres_twohits_%i_%i_ch%i_deltaTcor2"%(ch1,ch2,ch2))
        #two_hists   = [hist1,hist2]
        #two_fits    = [fit1 ,fit2 ]
        #get_comparison(two_hists,two_fits,"timeres_twohits_%i_%i_deltaTcorr2"%(ch1,ch2))

        hist1 = get_time_res("timeres_twohits_%i_%i_ch%i_amp"%(ch1,ch2,ch1))
        hist2 = get_time_res("timeres_twohits_%i_%i_ch%i_amp"%(ch1,ch2,ch2))
        two_hists   = [hist2,hist1]
        two_fits    = []
        get_comparison(two_hists,two_fits,"timeres_twohits_%i_%i_amp"%(ch1,ch2))

        hist1 = get_time_res("timeres_twohits_%i_%i_ch%i_slewrate"%(ch1,ch2,ch1))
        hist2 = get_time_res("timeres_twohits_%i_%i_ch%i_slewrate"%(ch1,ch2,ch2))
        two_hists   = [hist2,hist1]
        two_fits    = []
        get_comparison(two_hists,two_fits,"timeres_twohits_%i_%i_slewrate"%(ch1,ch2))

        hist1 = get_time_res("timeres_twohits_%i_%i_ch%i_risetime"%(ch1,ch2,ch1))
        hist2 = get_time_res("timeres_twohits_%i_%i_ch%i_risetime"%(ch1,ch2,ch2))
        two_hists   = [hist2,hist1]
        two_fits    = []
        get_comparison(two_hists,two_fits,"timeres_twohits_%i_%i_risetime"%(ch1,ch2))

        hist1 = get_time_res("timeres_twohits_%i_%i_ch%i_rmsnoise"%(ch1,ch2,ch1))
        hist2 = get_time_res("timeres_twohits_%i_%i_ch%i_rmsnoise"%(ch1,ch2,ch2))
        two_hists   = [hist2,hist1]
        two_fits    = []
        get_comparison(two_hists,two_fits,"timeres_twohits_%i_%i_rmsnoise"%(ch1,ch2))

        hist1 = get_time_res("timeres_twohits_%i_%i_ch%i_jitter"%(ch1,ch2,ch1))
        hist2 = get_time_res("timeres_twohits_%i_%i_ch%i_jitter"%(ch1,ch2,ch2))
        two_hists   = [hist2,hist1]
        two_fits    = []
        get_comparison(two_hists,two_fits,"timeres_twohits_%i_%i_jitter"%(ch1,ch2))

        
        get_time_res("timeres_twohits_%i_%i_ch%i_deltaTun"%(ch1,ch2,ch1))
        get_time_res("timeres_twohits_%i_%i_ch%i_deltaTun"%(ch1,ch2,ch2))
        get_time_res("timeres_twohits_%i_%i_deltaTcomb"%(ch1,ch2))
        get_time_res("timeres_twohits_%i_%i_deltaTlead"%(ch1,ch2))
        #get_time_res("timeres_twohits_%i_%i_deltaTcomb2"%(ch1,ch2))
        #get_time_res("timeres_twohits_%i_%i_deltaTlead2"%(ch1,ch2))
        get_time_res("timeres_twohits_%i_%i_xpos"%(ch1,ch2))



        for ch3 in range(0,3):
            if ch2==ch3: continue
            if ch1==ch3: continue

            hist1, fit1 = get_time_res("timeres_threehits_%i_%i_%i_ch%i_deltaTun"%(ch1,ch2,ch3,ch1))
            hist2, fit2 = get_time_res("timeres_threehits_%i_%i_%i_ch%i_deltaTun"%(ch1,ch2,ch3,ch2))
            hist3, fit3 = get_time_res("timeres_threehits_%i_%i_%i_ch%i_deltaTun"%(ch1,ch2,ch3,ch3))
            three_hists = [hist3,hist2,hist1]
            three_fits  = [fit3 ,fit2 ,fit1 ]
            get_comparison(three_hists,three_fits,"timeres_threehits_%i_%i_%i_deltaTun"%(ch1,ch2,ch3))

            hist1, fit1 = get_time_res("timeres_threehits_%i_%i_%i_ch%i_deltaTcor"%(ch1,ch2,ch3,ch1))
            hist2, fit2 = get_time_res("timeres_threehits_%i_%i_%i_ch%i_deltaTcor"%(ch1,ch2,ch3,ch2))
            hist3, fit3 = get_time_res("timeres_threehits_%i_%i_%i_ch%i_deltaTcor"%(ch1,ch2,ch3,ch3))
            three_hists = [hist3,hist2,hist1]
            three_fits  = [fit3 ,fit2 ,fit1 ]
            get_comparison(three_hists,three_fits,"timeres_threehits_%i_%i_%i_deltaTcorr"%(ch1,ch2,ch3))
            get_comparison_timeCorr(three_hists,"timeres_threehits_%i_%i_%i_deltaTcorr"%(ch1,ch2,ch3))

            #hist1, fit1 = get_time_res("timeres_threehits_%i_%i_%i_ch%i_deltaTcor2"%(ch1,ch2,ch3,ch1))
            #hist2, fit2 = get_time_res("timeres_threehits_%i_%i_%i_ch%i_deltaTcor2"%(ch1,ch2,ch3,ch2))
            #hist3, fit3 = get_time_res("timeres_threehits_%i_%i_%i_ch%i_deltaTcor2"%(ch1,ch2,ch3,ch3))
            #three_hists = [hist1,hist2,hist3]
            #three_fits  = [fit1 ,fit2 ,fit3 ]
            #get_comparison(three_hists,three_fits,"timeres_threehits_%i_%i_%i_deltaTcorr2"%(ch1,ch2,ch3))

            hist1 = get_time_res("timeres_threehits_%i_%i_%i_ch%i_jitter"%(ch1,ch2,ch3,ch1))
            hist2 = get_time_res("timeres_threehits_%i_%i_%i_ch%i_jitter"%(ch1,ch2,ch3,ch2))
            hist3 = get_time_res("timeres_threehits_%i_%i_%i_ch%i_jitter"%(ch1,ch2,ch3,ch3))
            three_hists = [hist3,hist2,hist1]
            three_fits  = []
            get_comparison(three_hists,three_fits,"timeres_threehits_%i_%i_%i_jitter"%(ch1,ch2,ch3))

            hist1 = get_time_res("timeres_threehits_%i_%i_%i_ch%i_rmsnoise"%(ch1,ch2,ch3,ch1))
            hist2 = get_time_res("timeres_threehits_%i_%i_%i_ch%i_rmsnoise"%(ch1,ch2,ch3,ch2))
            hist3 = get_time_res("timeres_threehits_%i_%i_%i_ch%i_rmsnoise"%(ch1,ch2,ch3,ch3))
            three_hists = [hist3,hist2,hist1]
            three_fits  = []
            get_comparison(three_hists,three_fits,"timeres_threehits_%i_%i_%i_rmsnoise"%(ch1,ch2,ch3))

            hist1 = get_time_res("timeres_threehits_%i_%i_%i_ch%i_slewrate"%(ch1,ch2,ch3,ch1))
            hist2 = get_time_res("timeres_threehits_%i_%i_%i_ch%i_slewrate"%(ch1,ch2,ch3,ch2))
            hist3 = get_time_res("timeres_threehits_%i_%i_%i_ch%i_slewrate"%(ch1,ch2,ch3,ch3))
            three_hists = [hist3,hist2,hist1]
            three_fits  = []
            get_comparison(three_hists,three_fits,"timeres_threehits_%i_%i_%i_slewrate"%(ch1,ch2,ch3))

            hist1 = get_time_res("timeres_threehits_%i_%i_%i_ch%i_risetime"%(ch1,ch2,ch3,ch1))
            hist2 = get_time_res("timeres_threehits_%i_%i_%i_ch%i_risetime"%(ch1,ch2,ch3,ch2))
            hist3 = get_time_res("timeres_threehits_%i_%i_%i_ch%i_risetime"%(ch1,ch2,ch3,ch3))
            three_hists = [hist3,hist2,hist1]
            three_fits  = []
            get_comparison(three_hists,three_fits,"timeres_threehits_%i_%i_%i_risetime"%(ch1,ch2,ch3))

            hist1 = get_time_res("timeres_threehits_%i_%i_%i_ch%i_amp"%(ch1,ch2,ch3,ch1))
            hist2 = get_time_res("timeres_threehits_%i_%i_%i_ch%i_amp"%(ch1,ch2,ch3,ch2))
            hist3 = get_time_res("timeres_threehits_%i_%i_%i_ch%i_amp"%(ch1,ch2,ch3,ch3))
            three_hists = [hist3,hist2,hist1]
            three_fits  = []
            get_comparison(three_hists,three_fits,"timeres_threehits_%i_%i_%i_amp"%(ch1,ch2,ch3))
            
            get_time_res("timeres_threehits_%i_%i_%i_ch%i_deltaTun"%(ch1,ch2,ch3,ch1))
            get_time_res("timeres_threehits_%i_%i_%i_ch%i_deltaTun"%(ch1,ch2,ch3,ch2))
            get_time_res("timeres_threehits_%i_%i_%i_ch%i_deltaTun"%(ch1,ch2,ch3,ch3))
            get_time_res("timeres_threehits_%i_%i_%i_deltaTcomb"%(ch1,ch2,ch3))
            get_time_res("timeres_threehits_%i_%i_%i_deltaTlead"%(ch1,ch2,ch3))
            #get_time_res("timeres_threehits_%i_%i_%i_deltaTcomb2"%(ch1,ch2,ch3))
            #get_time_res("timeres_threehits_%i_%i_%i_deltaTlead2"%(ch1,ch2,ch3))
            get_time_res("timeres_threehits_%i_%i_%i_xpos"%(ch1,ch2,ch3))
        

