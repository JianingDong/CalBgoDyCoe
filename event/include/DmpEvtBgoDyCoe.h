/*   $Id: DmpEvtBgoDyCoe.h, 2014-09-24 20:02:57+08:00 DAMPE $
 *--------------------------------------------------------
 *  Author(s):
 *    Jianing Dong (jndong@mail.ustc.edu.cn) 24/09/2014
 *--------------------------------------------------------
*/

#ifndef DmpEvtBgoDyCoe_H
#define DmpEvtBgoDyCoe_H

#include "TObject.h"

class DmpEvtBgoDyCoe : public TObject{
/*
 *  DmpEvtBgoDyCoe
 *
 */
public:
  DmpEvtBgoDyCoe();
  DmpEvtBgoDyCoe(const DmpEvtBgoDyCoe &r);
  DmpEvtBgoDyCoe(const DmpEvtBgoDyCoe *&r);
  ~DmpEvtBgoDyCoe();

  void Reset();

public:
  std::string UsedFileName;    //file name of raw data
  int StartTime;               //the time of the first event used to cal . DyCoe
  int StopTime;
 // std::vector<short>   GlobalDynodeID;
  std::vector<short>    GlobalPMTID;
  std::vector<double>   Inc_Dy5vsDy2;
  std::vector<double>   Inc_Dy8vsDy5;
  std::vector<double>   Slp_Dy5vsDy2;
  std::vector<double>   Slp_Dy8vsDy5;

  std::vector<double>   Inc_Err_Dy5vsDy2;
  std::vector<double>   Inc_Err_Dy8vsDy5;
  std::vector<double>   Slp_Err_Dy5vsDy2;
  std::vector<double>   Slp_Err_Dy8vsDy5;

  std::vector<double>   ChiS_Dy5vsDy2;
  std::vector<double>   ChiS_Dy8vsDy5;

  ClassDef(DmpEvtBgoDyCoe,1)

};

#endif
