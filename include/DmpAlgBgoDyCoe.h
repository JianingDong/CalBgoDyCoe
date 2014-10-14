/*
 *  $Id:DmpAlgBgoDyCoe.h, 2014-09-24 20:02:20 DAMPE $
 *  Author(s):
 *    Jianing Dong (jndong@mail.ustc.edu.cn)24/09/2014
*/ 

#ifndef DmpAlgCalBgoDyCoe_H
#define DmpAlgCalBgoDyCoe_H

#include "DmpVAlg.h"
#include <map>

class DmpEvtHeader;
class DmpEvtBgoRaw;
class DmpEvtBgoDyCoe;
class TH2F;

class DmpAlgBgoDyCoe : public DmpVAlg{
/*  
 *  DmpAlgBgoDyCoe
 *
 */
public:
  DmpAlgBgoDyCoe();
  ~DmpAlgBgoDyCoe();

  //void Set(const std::string &type,const std::string &value);
  // if you need to set some options for your algorithm at run time. Overload Set()
  bool Reset();
  bool Initialize();
  bool ProcessThisEvent();    // only for algorithm
  bool Finalize();

private:
  DmpEvtHeader          *fEvtHeader;
  DmpEvtBgoRaw          *fBgoRaw;
  DmpEvtBgoDyCoe        *fBgoDyCoe;
  std::map<short,TH2F*>  fDy5vsDy2Hist;
  std::map<short,TH2F*>  fDy8vsDy5Hist;
  std::map<short,short>  FitRangeDy8_l;
  std::map<short,short>  FitRangeDy8_h;
  std::map<short,short>  FitRangeDy5_l;
  std::map<short,short>  FitRangeDy5_h;

  double adc_dy2[14][24][2];
  double adc_dy5[14][24][2];
  double adc_dy8[14][24][2];
};

#endif
