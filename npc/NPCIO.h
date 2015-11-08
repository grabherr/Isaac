#ifndef NPCIO_H
#define NPCIO_H

#include "base/SVector.h"


class NPCIO
{
 public:
  NPCIO() {}

  void AddElement(const string & name, double val, bool bValid = true);

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

  virtual void Print() const;

 protected:
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

//========================================================
class NPCIO_WithCoords : public NPCIO
{
 public:
  NPCIO_WithCoords();

  void SetRelativeTo(const NPCIO_WithCoords & n);
  
  void Enable();
  void Disable();

  void SetPosition(double x, double y, double z) {
    m_data[0] = x;
    m_data[1] = y;
    m_data[2] = z;
  }

  void SetRotation(double x, double y, double z) {
    m_data[3] = x;
    m_data[4] = y;
    m_data[5] = z;
  }

  void Invert() {
    for (int i=0; i<3; i++)
      m_data[0] = 1./(m_data[0]+1.);
  }

  void ReInvert() {
    for (int i=0; i<3; i++)
      m_data[0] = 1./m_data[0]-1.;   
  }

  int Index() const {return m_index;}
  void SetIndex(int i) {m_index = i;}

  bool operator < (const NPCIO_WithCoords & c) const {
    return m_index < c.m_index;
  }

  virtual void Print() const {
    cout << "+++++ My index: " << m_index << endl;
    NPCIO::Print();
  }

 protected:
  int m_index;
};


#endif //NPCIO_H
