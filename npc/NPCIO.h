#ifndef NPCIO_H
#define NPCIO_H

#include "base/SVector.h"


class NPCIO
{
 public:
  NPCIO() {}

  void AddElement(const string & name);
  void SetElement(const string & name, double v);
  void SetValid(const string & name, bool b);

  double GetElement(const string & name) const;
  double IsValid(const string & name) const;
  double Distance(const NPCIO & n) const;
  double Distance(const svec<double> & n) const;

  int isize() const {return m_data.isize();}
  const double & operator[] (int i) const {return m_data[i];}
  double & operator[] (int i) {return m_data[i];}

  const string Name(int i) const {return m_name[i];}
 
  bool IsValid(int i) const {return (m_valid[i] > 0);}
  void SetValid(int i, bool b) {
    m_valid[i] = 0;
    if (b)
      m_valid[i] = 1;
  }
  
  void operator = (const svec<double> & n) {
    m_data = n;
  }
  

 private:
  int Index(const string & name) const {
    for (int i=0; i<m_name.isize(); i++) {
      if (m_name[i] == name)
	return i;
    }
    return -1;
  }

  svec<double> m_data;
  svec<string> m_name;
  svec<int> m_valid;


};


#endif //NPCIO_H
