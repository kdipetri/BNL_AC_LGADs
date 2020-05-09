import ROOT

gRandom = ROOT.TRandom3()

def getComb(nmeas,res):
   
    comb_meas = 0 
    for i in range(0,nmeas):
        meas = gRandom.Gaus(0,res)
        comb_meas+=meas

    comb_meas = comb_meas/nmeas
    return comb_meas

# Testing how resolution 
# improves with N measurements
# res/sqrt(N)
res = 30
end_nmeas = 10
hists=[]
#for nmeas in range(1,end_nmeas+1):
#
#    hist = ROOT.TH1D("combined_meas_n{}".format(nmeas),";x;N",200,-30,30)
#
#    for evt in range(0,10000):
#        hist.Fill(getComb(nmeas,res))
#
#    f1 = ROOT.TF1("f1_n{}".format(nmeas),"gaus",-30,30)
#    hist.Fit(f1,"Q")
#    print(nmeas,f1.GetParameter(2),res/(nmeas**0.5))



# Testing guess for time resolution
#for res1 in {20,30,40,50,60}:
#    for res2 in {20,30,40,50,60}:
for res1 in {47}:
    for res2 in {90}:
        res3 = 90
        hist = ROOT.TH1D("combined_tres_{}_{}".format(res1,res2),";x;N",400,-100,100)
        for evt in range(0,10000):
            
            tres_1 = gRandom.Gaus(0,res1)
            tres_2 = gRandom.Gaus(0,res2)
            tres_3 = gRandom.Gaus(0,res3)
        
            #tres_comb = (tres_1+tres_2)/2.0
            tres_comb = (tres_1+tres_2+tres_3)/3.0
            hist.Fill(tres_comb)
    
        f1 = ROOT.TF1("f1_tres","gaus",-100,100)
        hist.Fit(f1,"Q")
        #print("{},{},{:.2f},{:.2f}".format(res1,res2,f1.GetParameter(2), (res1**2+res2**2)**0.5/2.0))
        print("{},{},{:.2f},{:.2f}".format(res1,res2,f1.GetParameter(2), (res1**2+res2**2+res3**2)**0.5/3.0))
