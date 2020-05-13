import ROOT
import langaus as lg

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
ROOT.gStyle.SetPadTickY(1)
ROOT.gStyle.SetPadTickX(1)
ROOT.gStyle.SetPadBottomMargin(0.14)
ROOT.gStyle.SetPadLeftMargin(0.15)
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

f = ROOT.TFile.Open("skims/cfg_4_13_DC.root")

t = f.Get("pulse")

dut=12
photek = "amp[3]>50&&amp[3]<250"
track_sel = "ntracks==1&&nplanes>15&&npix>2&&nback>1&&abs(yResidBack)<500&&xResidBack>-500&&xResidBack<500"
#track_sel = "ntracks==1&&nplanes>17&&npix>3&&nback>1&&abs(yResidBack)<300&&xResidBack>40&&xResidBack<260"
track_pos = "x_dut[%i] > 19 && x_dut[%i] < 22 && y_dut[%i] > 22 && y_dut[%i] < 25"%(dut,dut,dut,dut)
ch=2


histname = "h_integral_dc"
hist = ROOT.TH1D(histname,";Collected Charge [fC];Events",25,0,50)
charge = "-1000*integral[%i]*1e9*50/4700"%ch
sel = photek + "&&" + track_sel + "&&" + track_pos + "&& amp[%i]>30"%ch

t.Project(histname,charge,sel)

c = ROOT.TCanvas("c1","",800,800)
c.SetLeftMargin(0.18);
c.SetBottomMargin(0.18);
c.SetRightMargin(0.05);
c.SetTopMargin(0.05);

hist.Draw()
hist.SetLineColor(ROOT.kBlack)
hist.SetLineWidth(2)
hist.GetYaxis().SetTitleOffset(1.3)
hist.GetYaxis().SetNdivisions(505)
hist.GetXaxis().SetNdivisions(505)
hist.SetMaximum(hist.GetMaximum()*1.2)

fitter = lg.LanGausFit()
f1 = fitter.fit(hist,(0,50))
f1.Draw("same")

ROOT.gStyle.SetOptStat(0)
ROOT.gStyle.SetOptFit(0)


mpv = f1.GetParameter(1)
err = f1.GetParError(1)
text = "MPV: {:.1f} #pm {:.1f} fC".format(mpv,err)
y=0.88

txt = ROOT.TPaveText(0.48,0.86,0.86,0.86-0.06, "NDC")
txt.SetTextAlign(13)
txt.SetTextFont(42)
txt.SetTextSize(0.05)
txt.SetFillColor(0)
txt.SetBorderSize(0)
txt.AddText(text);
txt.Draw()
#leg = ROOT.TLegend(0.45,y-0.1,0.88,y)
#leg.SetBorderSize(0)
#leg.SetTextSize(0.04)
#leg.AddEntry(hist,"DC-pad","l")
#leg.AddEntry(f1,txt,"l")
#leg.Draw()

c.Print("plots/dc_pad/dc_charge_langaus.pdf")


