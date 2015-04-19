#ifndef RESOURCES_H
#define RESOURCES_H

#include "base/SVector.h"
#include <math.h>

class IResource
{
 public:
  IResource() {
    m_max = 1.;
    m_min = 0.;
    m_res = 0.;
  }
  IResource(const string & name, double min = 0., double max = 1.) {
    m_max = max;
    m_min = min;
    m_res = 0.;
    m_name = name;
  }

  virtual ~IResource() {}

  // Absolute value of resources
  double Get() const {return m_res;}
  // Relative value of resources
  double GetRel() const {return Get()/(m_max - m_min);}

  void SetName(const string & name) {
    m_name = name;
  } 

  const string & Name() {return m_name;}

  void SetRange(double min, double max) {
    m_min = min;
    m_max = max;
  }

  double Add(double d = 1.) {
    double bb = m_res;
    m_res += d;
    if (m_res > m_max)
      m_res = m_max;
    return (m_res - bb);
  }

  double Remove(double d = 1.) {
    double bb = m_res;
    m_res -= d;
    if (m_res < m_min)
      m_res = m_min;
    return (bb - m_res);
  }

  // Assess the value characteristics of the resource
  virtual double Assess() const = 0;

 protected:
  double m_min;
  double m_max;
  double m_res;
  string m_name;
};

class LinearResource : public IResource
{
 public:
  virtual double Assess() const {
    return GetRel();
  }
 
};


class ExpResource : public IResource
{
 public:
  ExpResource() {m_curve = 1.;}

  // The higher the value, the steeper the curve
  void SetCurve(double curve) {
    m_curve = curve;
  }

  virtual double Assess() const {
    double d = 1. - exp(-GetRel()*m_curve);
    return d;
  }

 protected:
  double m_curve;
};


//======================================================
class ResourcePool
{
 public:
  ResourcePool() {}
  virtual ~ResourcePool();

  // Takes over the ownership of the object!
  void Add(IResource * p) {
    m_pool.push_back(p);
  }

  int isize() const {return m_pool.isize();}
  IResource * operator [] (int i) {return m_pool[i];}
  const IResource * operator [] (int i) const {return m_pool[i];}

  // NULL if not found
  IResource * GetRes(const string & name) const;
  int GetIndex(const string & name) const;

  IResource * Get(int i) const {
    return m_pool[i];
  }

  void GetAll(svec<double> & all) const;
  
  double Get(const string & name) const;
  double GetRel(const string & name) const;
  double Add(const string & name, double d=1.);
  double Remove(const string & name, double d=1.);

  
 protected:
  svec<IResource*> m_pool;
};




#endif //RESOURCES_H


