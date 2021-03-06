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
    if "lead" in name: return "Leading Channel"
    return ""

def get_time_res(name):
    ROOT.gStyle.SetOptStat(0)
    ROOT.gStyle.SetOptFit(0)
    hist = f.Get(name)
    c = ROOT.TCanvas()
    leg = ROOT.TLegend(0.37,0.88-0.05,0.88,0.88)

    cleanHist(hist,0)
    hist.Draw("hist") 
    lab = label(hist)

    if "deltaT"  in name: 
        f1 = ROOT.TF1("f1_"+name,"gaus",-0.5,0.5)
        hist.Fit(f1,"Q")
        f1.Draw("same")
        t0_str = ": t0={:.1f} ps, #sigma={:.1f} ps".format(f1.GetParameter(1)*1e3, f1.GetParameter(2)*1e3)
        lab += t0_str


    if "xpos" not in name:
        leg.AddEntry(hist,lab,"l")
        leg.Draw()
    else:
        hist.Rebin()

    #c.Print("plots/time_res/{}.png".format(name))
    if "deltaT" in name: return hist,f1
    else : return hist

    
# main 
def get_comparison(hists,fits,name):
    ROOT.gStyle.SetOptStat(0)
    ROOT.gStyle.SetOptFit(0)
    c = ROOT.TCanvas()
    dy = len(hists)*0.05
    if "deltaT" in name: leg = ROOT.TLegend(0.37,0.88-dy,0.88,0.88)
    elif "slewrate" in name or "rmsnoise" in name: leg = ROOT.TLegend(0.35,0.88-dy,0.88,0.88)
    else : leg = ROOT.TLegend(0.55,0.88-dy,0.88,0.88)
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
            t0_str = ": t0={:.1f} ps, #sigma={:.1f} ps".format(fits[i].GetParameter(1)*1e3, fits[i].GetParameter(2)*1e3)
            lab+= t0_str
        if "slewrate" in name :
            mean = hist.GetMean()
            mean_str = ": mean slewrate = {:.0f} [mV/ns]".format(mean)
            lab+= mean_str
        if "rmsnoise" in name:
            mean = hist.GetMean()
            mean_str = ": rms noise = {:.0f} [mV]".format(mean)
            lab+= mean_str

        leg.AddEntry(hist,lab, "l") 
    leg.Draw()
    hists[0].SetMaximum(ymax*1.3)
    #c.Print("plots/time_res/compare_{}.png".format(name))







hist1, fit1 = get_time_res("timeres_twohits_0_1_ch0_deltaTun")
hist2, fit2 = get_time_res("timeres_twohits_1_0_ch0_deltaTun")
t01 = fit1.GetParameter(1)*1e3
t02 = fit2.GetParameter(1)*1e3
er1 = fit1.GetParError(1)*1e3
er2 = fit2.GetParError(1)*1e3
err = (er1*er1+er2*er2)**0.5
print( "Ch 4,13 Diff = {:.2f} pm {:.2f}".format(t02-t01, err) )

hist1, fit1 = get_time_res("timeres_twohits_1_0_ch1_deltaTun")
hist2, fit2 = get_time_res("timeres_twohits_0_1_ch1_deltaTun")
t01 = fit1.GetParameter(1)*1e3
t02 = fit2.GetParameter(1)*1e3
er1 = fit1.GetParError(1)*1e3
er2 = fit2.GetParError(1)*1e3
err = (er1*er1+er2*er2)**0.5
print( "Ch 13,4 Diff = {:.2f} pm {:.2f}".format(t02-t01, err) )

hist1, fit1 = get_time_res("timeres_twohits_0_2_ch0_deltaTun")
hist2, fit2 = get_time_res("timeres_twohits_2_0_ch0_deltaTun")
t01 = fit1.GetParameter(1)*1e3
t02 = fit2.GetParameter(1)*1e3
er1 = fit1.GetParError(1)*1e3
er2 = fit2.GetParError(1)*1e3
err = (er1*er1+er2*er2)**0.5
print( "Ch 4,12 Diff = {:.2f} pm {:.2f}".format(t02-t01, err) )


hist1, fit1 = get_time_res("timeres_twohits_2_0_ch2_deltaTun")
hist2, fit2 = get_time_res("timeres_twohits_0_2_ch2_deltaTun")
t01 = fit1.GetParameter(1)*1e3
t02 = fit2.GetParameter(1)*1e3
er1 = fit1.GetParError(1)*1e3
er2 = fit2.GetParError(1)*1e3
err = (er1*er1+er2*er2)**0.5
print( "Ch 12,4 Diff = {:.2f} pm {:.2f}".format(t02-t01, err) )

diff = (17.62+24.55+15.78+22.98)/4.0
err = (2.62*2.62+2.70*2.70+2.58*2.58+2.80*2.80)*0.5
print( "Comb Diff = {:.2f} pm {:.2f}".format(diff, err) )




