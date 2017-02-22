#ifndef BUILDING_H
#define BUILDING_H

#include "physics/Coordinates.h"

class SchoolBuilding
{
 public:
  SchoolBuilding();


  double & Value(const Coordinates & c) {
    int i, j;
    Get(i, j, c[0], c[2]);
    return (m_data[i])[j];
  }
  
  const double & Value(const Coordinates & c) const {
    int i, j;
    Get(i, j, c[0], c[2]);
    return (m_data[i])[j];
  }

  void Fill(double x1, double x2, double z1, double z2, double val);
  void Fill(double x, double z, double val) {
    int i, j;
    Get(i, j, x, z);
    (m_data[i])[j] = val;    
  }

  bool Adjust(double & rot, const Coordinates & pos);

  
 private:
  void Print() const;
  double Get(int & i, int & j, double x, double z) const {
    int n = m_data.isize();
    i = (int)(((x-m_xoff)/m_scale)+0.5);
    j = (int)(((z-m_zoff)/m_scale)+0.5);
    //cout << "Check building " << x << " " << z << " -> " << i << " " << j << endl;
    if (i < 0)
      i = 0;
    if (j < 0)
      j = 0;
    if (i >= n)
      i = n-1;
    if (j >= n)
      j = n-1;
    return (m_data[i])[j];
  }
  
  double m_xoff;
  double m_zoff;
  double m_scale;
  svec< svec <double> > m_data;
};

#endif

