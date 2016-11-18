#ifndef NLOPT_H
#define NLOPT_H

#include "base/SVector.h"
#include <math.h>
#include "base/RandomStuff.h"

class NLOItem
{
 public:
  NLOItem() {}

  double & operator [] (int i) {return m_data[i];}
  const double & operator [] (int i) const {return m_data[i];}
  int isize() const {return m_data.isize();}
  void resize(int n, double d = 0.) {
    m_data.resize(n, d);
  } 
  const svec<double> Data() const {return m_data;}

  double Dist(const NLOItem & o) const {
    double d = 0;
    for (int i=0; i<m_data.isize(); i++) {
      d += (m_data[i]-o[i])*(m_data[i]-o[i]);
    }
    return d;
  }
  
 private:
  svec<double> m_data;
};

class NLOptimizer
{
 public:
  NLOptimizer() {
    m_target = 0;
    m_rWeight = 1.;
    m_frame = 0;
  }

  void SetDim(int dim, double lo, double hi) {
    m_curr.resize(dim);
    m_lo.resize(dim);
    m_hi.resize(dim);

    int i;
    for (i=0; i<dim; i++) {
      m_lo[i] = lo;
      m_hi[i] = hi;
    }
    int n = 100;
    m_grid.resize(n);
    for (i=0; i<n; i++) {
      m_grid[i].resize(n);
      for (int j=0; j<n; j++) {
	(m_grid[i])[j] = lo+RandomFloat(hi - lo);
      }
    }
    m_curr = m_grid[0];
  }

  void UnSuccess() {
    Pick();
    m_rWeight = 1.;

    //cout << "PICK (force) Target " << m_target << " 0: " << (m_grid[m_target])[0] << " 1: " << (m_grid[m_target])[1] << endl;
  }
  
  double Suggest(svec<double> & out, double opt) {
    // Do based on optimal curve!!
    if (m_frame % 30 == 0) {
      Pick();
      //cout << "PICK Target " << m_target << " 0: " << (m_grid[m_target])[0] << " 1: " << (m_grid[m_target])[1] << endl;
    }

    m_frame++;
    int i;
    
    for (i=0; i<m_curr.isize(); i++) {
      m_curr[i] += ((m_grid[m_target])[i]-m_curr[i])/3.;
    }
    
    out = m_curr.Data();
    //cout << "RETURN Target " << m_target << " 0: " << out[0] << " 1: " << out[1] << endl;

    m_rWeight *= 0.991;
    
    return m_rWeight;
  }
  
 public:
  void Pick() {
    int n = 3;
    int i;
    double max = 0.;
    int t = -1;
    for (i=0; i<n; i++) {
      int index = RandomInt(m_grid.isize());
      double d = m_grid[m_target].Dist(m_grid[index]);
      if (d > max) {
	max = d;
	t = index;
      }
    }
    m_target = t;
  }

  
  
  NLOItem m_lo;
  NLOItem m_hi;

  NLOItem m_curr;
  int m_target;
  svec<NLOItem> m_grid;
  svec<double> m_optQueue;
  double m_rWeight;
  int m_frame;
};






#endif

