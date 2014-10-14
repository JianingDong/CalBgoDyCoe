/*
 *  $Id: DmpAlgBgoDyCoe.cc, 2014-09-24 20:02:20 DAMPE $
 *  Author(s):
 *    Jianing Dong (jndong@mail.ustc.edu.cn) 24/09/2014
*/

#include <stdio.h>

#include "TH1F.h"
#include "TF1.h"
#include "TH2F.h"
#include "TFile.h"

#include "DmpEvtHeader.h"
#include "DmpEvtBgoRaw.h"
#include "DmpEvtBgoDyCoe.h"
#include "DmpAlgBgoDyCoe.h"
#include "DmpDataBuffer.h"
#include "DmpParameterBgo.h"
#include "DmpBgoBase.h"
#include "DmpCore.h"

//-------------------------------------------------------------------
DmpAlgBgoDyCoe::DmpAlgBgoDyCoe()
 :DmpVAlg("Cal/Bgo/DyCoe"),
  fEvtHeader(0),
  fBgoRaw(0),
  fBgoDyCoe(0)
{
  Reset();
}

//-------------------------------------------------------------------
bool DmpAlgBgoDyCoe::Reset(){
  for(short layer=0;layer<14;++layer){
    for(short bar=0;bar<24;++bar){
      for(short side=0;side<2;++side){
	adc_dy2[layer][bar][side] = 0.;
	adc_dy5[layer][bar][side] = 0.;
	adc_dy8[layer][bar][side] = 0.;
      }
    }
  }
}

//-------------------------------------------------------------------
DmpAlgBgoDyCoe::~DmpAlgBgoDyCoe(){
}

//-------------------------------------------------------------------
bool DmpAlgBgoDyCoe::Initialize(){
  //read input data
  //gDataBuffer->LinkRootFile("Event/Rdc/EventHeader",fEvtHeader);
  fEvtHeader = dynamic_cast<DmpEvtHeader*>(gDataBuffer->ReadObject("Event/Rdc/EventHeader"));
  //gDataBuffer->LinkRootFile("Event/Rdc/Bgo",fBgoRaw);
  fBgoRaw = dynamic_cast<DmpEvtBgoRaw*>(gDataBuffer->ReadObject("Event/Rdc/Bgo"));
  //create output data holder
  fBgoDyCoe = new DmpEvtBgoDyCoe();
  gDataBuffer->RegisterObject("Calibration/Bgo/DyCoe",fBgoDyCoe,"DmpEvtBgoDyCoe");
  fBgoDyCoe->UsedFileName = gRootIOSvc->GetInputFileName();
  gRootIOSvc->PrepareEvent(gCore->GetCurrentEventID());
  fBgoDyCoe->StartTime = fEvtHeader->GetSecond();

  //create Hist map
  short layerNo = DmpParameterBgo::kPlaneNo*2;
  short barNo = DmpParameterBgo::kBarNo;
  short sideNo = DmpParameterBgo::kSideNo;
  for(short l=0;l<layerNo;++l){ 
    for(short b=0;b<barNo;++b){ 
      for(short s=0;s<sideNo;++s){
	char name[50];
	short gid_pmt = DmpBgoBase::ConstructGlobalPMTID(l,b,s);
	FitRangeDy5_l[gid_pmt] = 0;
	FitRangeDy5_h[gid_pmt] = 0;
	FitRangeDy8_l[gid_pmt] = 0;
	FitRangeDy8_h[gid_pmt] = 0;
	snprintf(name,50,"BgoDyCoe_%05d-L%02d_B%02d_S%2d_Dy5/Dy2",gid_pmt,l,b,s);
	fDy5vsDy2Hist.insert(std::make_pair(gid_pmt,new TH2F(name,name,400,0,800,550,0,16500)));
	snprintf(name,50,"BgoDyCoe_%05d-L%02d_B%02d_S%2d_Dy8/Dy5",gid_pmt,l,b,s);
	fDy8vsDy5Hist.insert(std::make_pair(gid_pmt,new TH2F(name,name,400,0,800,550,0,16500)));
       } 
     }
  } 
  return true;
} 

//-------------------------------------------------------------------
bool DmpAlgBgoDyCoe::ProcessThisEvent(){ 
  short gid = 0,l=-1,b=-1,s=-1,d=-1;
  double adc = 0.;
  short nSignal = fBgoRaw->fADC.size();
  for(short i=0;i<nSignal;++i){
    gid = fBgoRaw->fGlobalDynodeID[i];
    adc = fBgoRaw->fADC[i];
    DmpBgoBase::LoadLBSDID(gid,l,b,s,d);
    if(d == 2){  
      adc_dy2[l][b][s] = adc;
    }
    else if(d == 5){ 
      adc_dy5[l][b][s] = adc;
    }
    else if(d == 8){
      adc_dy8[l][b][s] = adc;
    }  
  }
  for(short layer=0;layer<14;++layer){
    for (short bar=0;bar<22;++bar){
      for(short side=0;side<2;++side){
	short gid_pmt = DmpBgoBase::ConstructGlobalPMTID(layer,bar,side);
	//if(adc_dy2[layer][bar][side]>0 && adc_dy5[layer][bar][side]>0){
          fDy5vsDy2Hist[gid_pmt]->Fill(adc_dy2[layer][bar][side],adc_dy5[layer][bar][side]);
 	//}
	//if(adc_dy5[layer][bar][side]>0 && adc_dy8[layer][bar][side]>0){
	  fDy8vsDy5Hist[gid_pmt]->Fill(adc_dy5[layer][bar][side],adc_dy8[layer][bar][side]);
 	//}
	if(adc_dy5[layer][bar][side]> 2000 && adc_dy5[layer][bar][side]<3500 && FitRangeDy5_l[gid_pmt]<50){
	  FitRangeDy5_l[gid_pmt] = adc_dy2[layer][bar][side];
	}
	else if(adc_dy5[layer][bar][side]>8000 && adc_dy5[layer][bar][side]<9000 && FitRangeDy5_h[gid_pmt]<50){
	  FitRangeDy5_h[gid_pmt] = adc_dy2[layer][bar][side];
	}
	if(adc_dy8[layer][bar][side]>1500 && adc_dy8[layer][bar][side]<3500 && FitRangeDy8_l[gid_pmt]<50){
          FitRangeDy8_l[gid_pmt] = adc_dy5[layer][bar][side];
	}
	else if(adc_dy8[layer][bar][side]>8000 && adc_dy8[layer][bar][side]<10000 && FitRangeDy8_h[gid_pmt]<50){
          FitRangeDy8_h[gid_pmt] = adc_dy5[layer][bar][side];
	}
      }
    }
  }   
  Reset(); 
  return true;
} 

//-------------------------------------------------------------------
bool DmpAlgBgoDyCoe::Finalize(){
  TF1 *LinearFit = new TF1("LinearFit","[0]+[1]*x",-400,2500);
  LinearFit->SetParNames("Intercept","Slope");
  std::string histFileName = "./DyCoe/Histograms"+gRootIOSvc->GetOutputStem()+"_Hist.root";
  TFile *histFile = new TFile(histFileName.c_str(),"RECREATE");
  fBgoDyCoe->StopTime = fEvtHeader->GetSecond();
  for(std::map<short,TH2F*>::iterator bHist=fDy8vsDy5Hist.begin();bHist!=fDy8vsDy5Hist.end();++bHist){
    fBgoDyCoe->GlobalPMTID.push_back(bHist->first);
    LinearFit->SetRange(FitRangeDy8_l[bHist->first],FitRangeDy8_h[bHist->first]);
    if(FitRangeDy8_l[bHist->first]*FitRangeDy8_h[bHist->first] != 0){
      bHist->second->Fit(LinearFit,"R");
      double Inc_Dy8vsDy5 = LinearFit->GetParameter(0),Slp_Dy8vsDy5 = LinearFit->GetParameter(1);
      double Inc_Err_Dy8vsDy5 = LinearFit->GetParError(0),Slp_Err_Dy8vsDy5 = LinearFit->GetParError(1);
      double ChiS_Dy8vsDy5 = LinearFit->GetChisquare();
      fBgoDyCoe->Inc_Dy8vsDy5.push_back(Inc_Dy8vsDy5);
      fBgoDyCoe->Inc_Err_Dy8vsDy5.push_back(Inc_Err_Dy8vsDy5);
      fBgoDyCoe->Slp_Dy8vsDy5.push_back(Slp_Dy8vsDy5);
      fBgoDyCoe->Slp_Err_Dy8vsDy5.push_back(Slp_Err_Dy8vsDy5);
      fBgoDyCoe->ChiS_Dy8vsDy5.push_back(ChiS_Dy8vsDy5);
    }
    else{
      fBgoDyCoe->Inc_Dy8vsDy5.push_back(0.);
      fBgoDyCoe->Inc_Err_Dy8vsDy5.push_back(0.);
      fBgoDyCoe->Slp_Dy8vsDy5.push_back(0.);
      fBgoDyCoe->Slp_Err_Dy8vsDy5.push_back(0.);
      fBgoDyCoe->ChiS_Dy8vsDy5.push_back(0.);
    }
    bHist->second->SetMarkerStyle(25);
    bHist->second->Write();
    delete bHist->second;
  }
  for(std::map<short,TH2F*>::iterator aHist=fDy5vsDy2Hist.begin();aHist!=fDy5vsDy2Hist.end();++aHist){
    //fBgoDyCoe->GlobalPMTID.push_back(aHist->first);
    LinearFit->SetRange(FitRangeDy5_l[aHist->first],FitRangeDy5_h[aHist->first]);
    if(FitRangeDy5_l[aHist->first]*FitRangeDy5_h[aHist->first] != 0){
    //DmpLogInfo<<"gid_pmt="<<aHist->first<<" FitRangeDy5_l="<<FitRangeDy5_l[aHist->first]<<" FitRangeDy5_h="<<FitRangeDy5_h[aHist->first];
      aHist->second->Fit(LinearFit,"R");                         
      double Inc_Dy5vsDy2 = LinearFit->GetParameter(0),Slp_Dy5vsDy2 = LinearFit->GetParameter(1);
      double Inc_Err_Dy5vsDy2 = LinearFit->GetParError(0),Slp_Err_Dy5vsDy2 = LinearFit->GetParError(1);
      double ChiS_Dy5vsDy2 = LinearFit->GetChisquare();          
      fBgoDyCoe->Inc_Dy5vsDy2.push_back(Inc_Dy5vsDy2);           
      fBgoDyCoe->Inc_Err_Dy5vsDy2.push_back(Inc_Err_Dy5vsDy2);
      fBgoDyCoe->Slp_Dy5vsDy2.push_back(Slp_Dy5vsDy2);
      fBgoDyCoe->Slp_Err_Dy5vsDy2.push_back(Slp_Err_Dy5vsDy2);
      fBgoDyCoe->ChiS_Dy5vsDy2.push_back(ChiS_Dy5vsDy2);
    }
    else{
      fBgoDyCoe->Inc_Dy5vsDy2.push_back(0.);
      fBgoDyCoe->Inc_Err_Dy5vsDy2.push_back(0.);
      fBgoDyCoe->Slp_Dy5vsDy2.push_back(0.);
      fBgoDyCoe->Slp_Err_Dy5vsDy2.push_back(0.);
      fBgoDyCoe->ChiS_Dy5vsDy2.push_back(0.);
    }
    aHist->second->SetMarkerStyle(25);
    aHist->second->Write();
    delete aHist->second;
  }
  delete histFile;
  return true;
} 
