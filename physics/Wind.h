#ifndef WIND_H
#define WIND_H

#include "physics/Coordinates.h"
#include "base/RandomStuff.h"


class WindCheckPoint
{
 public:
  WindCheckPoint() {}

  Coordinates & Direction() {return m_dir;}
  const Coordinates & Direction() const {return m_dir;}

  double UpdateFrom(const Coordinates & me, 
		    const WindCheckPoint & w, 
		    const Coordinates & other,
		    double deltatime);
  
 private:
  Coordinates m_dir;
};


class WindParticle
{
 public:
  WindParticle() {}

  Coordinates & Position() {return m_x;}
  Coordinates & Velocity() {return m_v;}
  
  const Coordinates & Position() const {return m_x;}
  const Coordinates & Velocity() const {return m_v;}
  

 private:
  Coordinates m_x;
  Coordinates m_v;
 
};

//================================================

class IWindShape
{
 public:
  IWindShape() {}
  virtual ~IWindShape() {}
  
  virtual void Compute(Coordinates & out, const Coordinates & in) const = 0;
  virtual void Update(double deltatime) = 0;

};


class LinearWindShape : public IWindShape
{
 public:
  LinearWindShape() {
    m_speed = 0;
    m_time = 0;
    m_decay = 0.005;
    m_width = 280.;
    m_size = 50.;
  }

  void Reset() {
    m_speed = 0;
    m_time = 0;
    m_decay = 0.02;
    m_width = 20.;
    m_size = 50.;  
  }

  void SetSpeed(double s) {m_speed = s;}
  void SetWidth(double s) {m_width = s;}
  void SetSize(double s) {m_size = s;}

  void Set(const Coordinates & origin, const Coordinates & direction) {
    m_origin = origin;
    m_direction = direction.Einheitsvector();
    m_time = 0;
  }


  virtual void Compute(Coordinates & out, const Coordinates & in) const
  {
    double dist = m_origin.Length() - 1.;

    /*double far =  (m_origin - in).Length();
    if (far > m_size) {
      out[0] = 0;
      out[0] = 0;
      out[0] = 0;
      return;
      }*/
      
    
    int i;
    double d = 0.;
    for (i=0; i<3; i++) {
      double x = in[i];
      if (m_direction[i] != 0.)
	x /= m_direction[i];
      else
	x = 0.;
      d += x;
    }
    
    double w = m_width;
    double val = w/(w+(d-dist)*(d-dist));
    out = m_direction * val * m_speed;
  }
  
  virtual void Update(double deltatime) {
    m_time += deltatime;
    m_origin += m_direction * m_speed * deltatime;
    m_speed *= (1. - m_decay*deltatime);
    m_width /= (1. - m_decay*deltatime);
    m_size /= (1. - m_decay*deltatime);
    
  }

 protected:
  double m_speed;
  double m_time;
  double m_decay;
  double m_width;
  double m_size;
  Coordinates m_origin;
  Coordinates m_direction;
  
};


class UpDownWindShape : public IWindShape
{
 public:
  UpDownWindShape() {
    m_width = 5.;

  }
  void SetWidth(double d) {
    m_width = d;
  } 
  void SetCenter(const Coordinates & c) {
    m_center = c;
  } 

  virtual void Compute(Coordinates & out, const Coordinates & in) const {
    out = Coordinates(0, 0, 0);
    if (in[2] < m_center[2])
      return;
    if (in[2] > m_center[2]+20)
      return;
    
    double v = in[0] - m_center[0];
    //cout << "Try. " << in[0] << " " << m_center[0] << " " << v << endl;
   
    if (v > m_width || v < -m_width)
      return;

    //cout << "Set val." << endl;
    if (v < m_width/2. && v > -m_width/2.)
      out[2] = 1.;
    else
      out[2] = 0.1;

  }
  
  virtual void Update(double deltatime) {
  }

 protected:
  
  double m_width;
  Coordinates m_center;
};



class CircleWindShape : public IWindShape
{
 public:
  CircleWindShape() {
    m_v = 0.;
    m_size = 0.;
  }

  void SetCenter(const Coordinates & c, double size, double v) {
    m_center = c;
    m_v = v;
    m_size = size;
  }

  virtual void Compute(Coordinates & out, const Coordinates & in) const {
    double d = (in - m_center).Length();

    Coordinates rel = in - m_center;
    out = Coordinates(0, 0, 0);
    if (d > m_size - 2 && d < m_size + 2) {
      Coordinates c = rel.CrossProduct(Coordinates(0., 1., 0));
      out = rel.Einheitsvector() * m_v;
    }
  }

  virtual void Update(double deltatime) {
  }

 protected:
  Coordinates m_center;
  double m_v;
  double m_size;
};

//================================================
class Wind
{
 public:
  Wind() {
    m_decay = 0.99;
  }

  void SetSize(int x, int y, int z) {
    int i, j;
    m_points.resize(x);
    for (i=0; i<x; i++) {
      m_points[i].resize(y);
      for (j=0; j<y; j++) {
	m_points[i][j].resize(z);
      }
    }
  }

  bool Valid(int x, int y, int z) const {
    if (x < 0 || y < 0 || z < 0)
      return false;
    if (x >= X() || y >= Y() || z >= Z())
      return false;
    //cout << "Valid: " << x << " " << y << " " << z << endl;
    return true;
  }

  int X() const {return m_points.isize();}

  int Y() const {
    if (X() == 0)
      return 0;
    return m_points[0].isize();
  }

  int Z() const {
    if (X() == 0)
      return 0;
    if (Y() == 0)
      return 0;
    return m_points[0][0].isize();
  }

  WindCheckPoint & Get(int x, int y, int z) {
    //cout << "Ask for " << x << " " << y << " " << z << endl;
    return ((m_points[x])[y])[z];
  }
  const WindCheckPoint & Get(int x, int y, int z) const {
    return ((m_points[x])[y])[z];
  }

  void Add(int x, int y, int z, const Coordinates & dir) {
    Get(x, y, z).Direction() += dir;
  }

  void Update(double deltatime);

  void Process(const IWindShape & shape);
  void Clear();

  void Print() const;
 private:
  svec < svec < svec < WindCheckPoint > > > m_points;
  double m_decay;
};

//===============================================================
class ParticleMovement
{
 public:
  ParticleMovement() {
    m_decay = 0.98;
    m_used = 0;
    m_first = 0;
  }
  
  void Add(int n, const Coordinates & c, double spread);
  void AddRandom(int n);
  void AddRandomNoise(double d);

  int isize() const {return m_particles.isize();}
  int used() const {return m_first;}
  WindParticle & operator [] (int i) {return m_particles[i];}
  const WindParticle & operator [] (int i) const {return m_particles[i];}

  void Setup(int n, const Coordinates & bounds);
  void Move(const Wind & wind, double deltatime);

 private:
  int m_used;
  int m_first;
  Coordinates m_bounds;
  svec<WindParticle> m_particles;
  double m_decay;
};


#endif
