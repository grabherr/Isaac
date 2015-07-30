// (c) Manfred G. Grabherr

// This code is licensed under the GNU GENERAL PUBLIC LICENSE:
// http://www.gnu.org/licenses/gpl-3.0.en.html

#ifndef _CROSSCORR_H_
#define _CROSSCORR_H_

#include "base/SVector.h"
#include "extern/RealFFT/FFTReal.h"



class CrossCorrelation
{
 public:
  CrossCorrelation();
  ~CrossCorrelation();
  void CrossCorrelate(svec<float> & out, const svec<double> & a, const svec<double> & b);
  
  void AutoCorrelate(vector<float> &out, vector<float> &in);

  void DoOne(svec<float> & o, const svec<float> & in1, const svec<float> & in2);

 private:
  FFTReal<float> * m_pFFT;
  int m_size;

};





#endif

