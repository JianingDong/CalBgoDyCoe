/*   $Id: DmpEvtBgoDyCoe.cc, 2014-09-24 20:02:57+08:00 DAMPE $
 *--------------------------------------------------------
 *  Author(s):
 *    Jianing Dong (jndong@mail.ustc.edu.cn) 24/09/2014
 *--------------------------------------------------------
*/

#include "DmpEvtBgoDyCoe.h"

ClassImp(DmpEvtBgoDyCoe)

DmpEvtBgoDyCoe::DmpEvtBgoDyCoe(){
  Reset();
}

DmpEvtBgoDyCoe::DmpEvtBgoDyCoe(const DmpEvtBgoDyCoe &r){
  Reset();
  UsedFileName = r.UsedFileName;
  StartTime = r.StartTime;
  StopTime = r.StopTime;
  short n = GlobalPMTID.size();
  for(size_t i = 0;i<n;++i){
    GlobalPMTID.push_back(r.GlobalPMTID[i]);
    Inc_Dy5vsDy2.push_back(r.Inc_Dy5vsDy2[i]);
    Slp_Dy5vsDy2.push_back(r.Slp_Dy5vsDy2[i]);
    Inc_Dy8vsDy5.push_back(r.Inc_Dy8vsDy5[i]);
    Slp_Dy8vsDy5.push_back(r.Slp_Dy8vsDy5[i]);
  }
}

DmpEvtBgoDyCoe::DmpEvtBgoDyCoe(const DmpEvtBgoDyCoe *&r){
  Reset();
  UsedFileName = r->UsedFileName;
  StartTime = r->StartTime;
  StopTime = r->StopTime;
  short n = GlobalPMTID.size();
  for(size_t i = 0;i<n;++i){
    GlobalPMTID.push_back(r->GlobalPMTID[i]);
    Inc_Dy5vsDy2.push_back(r->Inc_Dy5vsDy2[i]);
    Slp_Dy5vsDy2.push_back(r->Slp_Dy5vsDy2[i]);
    Inc_Dy8vsDy5.push_back(r->Inc_Dy8vsDy5[i]);
    Slp_Dy8vsDy5.push_back(r->Slp_Dy8vsDy5[i]);
  }
}

DmpEvtBgoDyCoe::~DmpEvtBgoDyCoe()
{
}



void DmpEvtBgoDyCoe::Reset()
{
  UsedFileName = "NO";
  StartTime = 0;
  StopTime = 0xafffffff;
  GlobalPMTID.clear();
  Inc_Dy5vsDy2.clear();
  Slp_Dy5vsDy2.clear();
  Inc_Dy8vsDy5.clear();
  Slp_Dy8vsDy5.clear();
}
