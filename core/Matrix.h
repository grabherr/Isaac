// (c) Manfred G. Grabherr

// This code is licensed under the GNU GENERAL PUBLIC LICENSE:
// http://www.gnu.org/licenses/gpl-3.0.en.html

#ifndef MATRIX_H
#define MATRIX_H

#include "base/SVector.h"

template<class T>
class Matrix2D
{
 public:
  Matrix2D() {}
  Matrix2D(int x, int y) {
    SetSize(x, y);
  }

  Matrix2D(int x, int y, const T & def) {
    SetSize(x, y, def);
  }

  void SetSize(int x, int y) {
    m_data.resize(x);
    for (int i=0; i<x; i++)
      m_data[i].resize(y);
  }

  void SetSize(int x, int y, const T & def) {
    m_data.resize(x);
    for (int i=0; i<x; i++) {
      m_data[i].resize(y);
      for (int j=0; j<y; j++)
	(m_data[i])[j] = def;
    }
  }
 
  int SizeX() const {return m_data.isize();}
  int SizeY() const {return m_data[0].isize();}

  void Set(int x, int y, const T & v) {
    (m_data[x])[y] = v;
  }
  const T & Get(int x, int y) const {
    return (m_data[x])[y];
  }
  const T & operator()(int x, int y) const {
    return (m_data[x])[y];
  }
  T & operator()(int x, int y) {
    return (m_data[x])[y];
  }



 protected:
  

  svec< svec < T > > m_data;
};


#endif //MATRIX_H

