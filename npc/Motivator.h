#ifndef MOTIVATION_H
#define MOTIVATION_H

#include "npc/NNet.h"
#include "npc/Environment.h"
#include "util/mutil.h"

class MotivationCategory
{
 public:
  MotivationCategory() {
    m_val = 0.;
    m_weight = 1.;
    m_show = 1.;
  }
  
  MotivationCategory(const string & name,
		     double val,
		     double weight,
		     double show) {
    m_name = name;
    m_val = val;
    m_weight = weight;
    m_show = show;
  }
  
  const string & GetName() const {return m_name;}
  double GetVal() const {return m_val;}
  double GetWeight() const {return m_weight;}  
  double GetShow() const {return m_show;}  

  
  void SetName(const string & s) {m_name = s;}
  void SetVal(double v) {m_val = v;}
  void SetWeight(double w) {m_weight = w;}
  void SetShow(double s) {m_show = s;}
  
  void AddVal(double v) {
    m_val += v;
    if (m_val > 1.)
      m_val = 1.;
    if (m_val < 1.)
      m_val = -1.;
  }
  
  void Read(CMReadFileStream &s);
  void Write(CMWriteFileStream &s);
 private:
  string m_name;
  double m_val;
  double m_weight;
  double m_show;
};


class EnvToMotMatrix
{
 public:
  EnvToMotMatrix() {}

  void Resize(int out, int in) {
    m_data.resize(out);
    for (int i=0; i<out; i++)
      m_data[i].resize(in, 0.);
  }
  
  void Set(int out, int in, double v) {
    (m_data[out])[in] = v;
  }
  void Add(int out, int in, double v) {
    (m_data[out])[in] += v;
  }
	   
  void MatMult(svec<MotivationCategory> & out,
	       const svec<EnvFactor> & in,
	       const Coordinates &);
  
  void Read(CMReadFileStream &s);
  void Write(CMWriteFileStream &s);

 private:
  svec < svec < double > > m_data;
};

//==================================================
class Motivator
{
 public:
  Motivator();
  
  void Setup(int mot, int in) {
    m_matrix.Resize(mot, in);
    m_mot.resize(mot);
    m_is.resize(mot);
  }
  
  const MotivationCategory & Get(const string & name) const {
    return m_mot[Index(name)];
  }
  MotivationCategory & Get(const string & name) {
    return m_mot[Index(name)];
  }

  const MotivationCategory & operator [] (int i) const {return m_mot[i];}
  MotivationCategory & operator [] (int i)  {return m_mot[i];}
  int isize() const {return m_mot.isize();}
  
  void PreSet();
  void Read(CMReadFileStream &s);
  void Write(CMWriteFileStream &s);

  void Read(const string & fileName) {
    CMReadFileStream s;
    s.Open(fileName.c_str());
    Read(s);
    s.Close();
  }
  
  void Write(const string & fileName) {
    CMWriteFileStream s;
    s.Open(fileName.c_str());
    Write(s);
    s.Close();
  }

  const Coordinates & Coords() const {return m_coords;}
  Coordinates & Coords() {return m_coords;}

  void ApplyEnv(const svec<EnvFactor> & in);
  
  void Print() const;
  
  EnvToMotMatrix & Matrix() {return m_matrix;}
  const EnvToMotMatrix & Matrix() const {return m_matrix;}

  void AddToMatrix(const string & mot, svec<EnvFactor> & env) {
    int index = Index(mot);
    for (int i=0; i<env.isize(); i++)
      m_matrix.Add(index, i, env[i].GetVal());
  }
  
 private:
  int Index(const string & name) const {
    for (int i=0; i<m_mot.isize(); i++) {
      if (m_mot[i].GetName() == name)
	return i;
    }
    cout << "ERROR, not found: " << name << endl;
    return -1;
  }
  
  svec<MotivationCategory> m_mot;
  svec<MotivationCategory> m_is;
  EnvToMotMatrix m_matrix;
  Coordinates m_coords;
};


#endif



