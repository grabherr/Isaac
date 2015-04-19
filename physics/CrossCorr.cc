#ifndef FORCE_DEBUG
#define NDEBUG
#endif


 
#include "physics/CrossCorr.h"
#include "extern/RealFFT/FFTReal.hpp"
#include <math.h>





void ComplexMult(float & r_o, float & i_o, float r1, float i1, float r2, float i2) 
{
  r_o = r1 * r2 - i1 * i2;
  i_o = i1 * r2 + i2 * r1;

  //  cout << "r1=" << r1 << " i1=" << i1 << " r2=" << r2 << " i2=" << i2 << " r_o=" << r_o << " i_o=" << i_o << endl;
}

double Ent(double p) 
{
  if (p < 0.001)
    return 0;
  return p * log(p) / 0.69314718056;
}


//==================================================================
CrossCorrelation::CrossCorrelation()
{
  m_pFFT = NULL;
  m_size = 0;
}

CrossCorrelation::~CrossCorrelation()
{
  if (m_pFFT != NULL)
    delete m_pFFT;
}


void CrossCorrelation::AutoCorrelate(vector<float> &out, vector<float> &in)
{
  out.clear();

  int N=FFTReal_get_next_pow2((long) in.size());

  in.resize(N,0);
  out.resize(N,0);
  
  vector<float> X(N,0),tmp(N,0);

  if (N != m_size) {
    if (m_pFFT != NULL) {
      cout << "WARNING: re-instantiating FFT object!" << endl;
      delete m_pFFT;
    }
    m_pFFT = new FFTReal<float> (N);
    m_size = N;
  }

  m_pFFT->do_fft(&X[0],&in[0]);
  
  for (int i=0; i<=N/2; ++i )
    tmp[i] = (X[i]+X[i+N/2])*(X[i]-X[i+N/2]);
  
  m_pFFT->do_ifft(&tmp[0],&out[0]);
  m_pFFT->rescale(&out[0]);
}


void CrossCorrelation::CrossCorrelate(svec<float> & out, const svec<double> & a, const svec<double> & b)
{
  //cout << "CrossCorrelate" << endl;

  out.clear();

  //m_size = FFTReal_get_next_pow2(a.isize());

  m_size = 2;
  while (m_size < a.isize()) {
    m_size *= 2;
  }

  //cout << m_size << " " << a.isize() << " " << b.isize() << endl;
  out.resize(m_size, 0.);

  svec<float> aa, bb;
  aa.resize(m_size, 0.);
  bb.resize(m_size, 0.);

  int i, j;
  float sumA = 0.;
  float sumB = 0.;
  for (i=0; i<a.isize(); i++) {
    aa[i] = (float)a[i];
    bb[i] = (float)b[i];
    sumA += aa[i];
    sumB += bb[i];
  }
 
  sumA /= (float)m_size;
  sumB /= (float)m_size;

  for (i=0; i<m_size; i++) {
    aa[i] -= sumA;
    bb[i] -= sumB;
  }

  DoOne(out, aa, bb);
  
}



void CrossCorrelation::DoOne(svec<float> & o, const svec<float> & in1, const svec<float> & in2)
{
  int i;
  o.resize(in1.size(), 0);

  if (m_pFFT == NULL || m_size != in1.isize()) {
    m_size = in1.isize();
    if (m_pFFT != NULL) {
      cout << "WARNING: re-instantiating FFT object!" << endl;
      delete m_pFFT;
    }
    //cout << "Initializing FFT." << endl;
    m_pFFT = new FFTReal<float>(m_size);
    //cout << "done." << endl;
  }

  svec<float> tmp1;
  tmp1.resize(in1.size(), 0.);
  svec<float> tmp2;
  tmp2.resize(in2.size(), 0.);

  float * p1 = &tmp1[0];
  float * p2 = &tmp2[0];



  m_pFFT->do_fft(p1, &in1[0]);


  m_pFFT->do_fft(p2, &in2[0]);


  int N = tmp1.isize() / 2;
  //cout << "N=" << N << endl;

  tmp1[0] *= tmp2[0];
  //tmp1[N] *= tmp2[N];
  for (i=1; i<N-1; i++) {
    float fr, fi;

    //ComplexMult(fr, fi, tmp1[i], tmp1[i+N+1], tmp2[i], -tmp2[i+N+1]);
    //tmp1[i] = fr;
    //tmp1[i+N+1] = -fi;

    ComplexMult(fr, fi, tmp1[i], tmp1[i+N], tmp2[i], -tmp2[i+N]);
    tmp1[i] = fr;
    tmp1[i+N] = -fi;
  }
  m_pFFT->do_ifft(p1, p2);
  m_pFFT->rescale(p2);

  //for (i=0; i<2*N; i++)
  //o[i] = tmp2[i];

  
  for (i=0; i<N; i++) {
    o[i] = tmp2[i+N];
  }
  for (i=N; i<2*N; i++) {
    o[i] = tmp2[i-N];
  }
 
}





