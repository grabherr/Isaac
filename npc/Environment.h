#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "base/SVector.h"
#include "physics/Coordinates.h"
#include "util/mutil.h"

class EnvFactor
{
 public:
  EnvFactor() {
    m_val = 0.;
    m_dist = 2.;    
  }

  const string & GetName() const {return m_name;}
  double GetVal() const {return m_val;}
  double GetDist() const {return m_dist;}
  const Coordinates & GetCoords() const {return m_coords;}

  void SetName(const string & n) {m_name = n;}
  void SetVal(double v) {m_val = v;}
  void SetDist(double d) {m_dist = d;}
  void SetCoords(const Coordinates & c) {m_coords = c;}


  void Read(CMReadFileStream &s);
  void Write(CMWriteFileStream &s);

 private:
  string m_name;
  double m_val;
  double m_dist;
  Coordinates m_coords;
};



#endif

