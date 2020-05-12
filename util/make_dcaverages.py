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


