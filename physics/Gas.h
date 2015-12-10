#ifndef GAS_H
#define GAS_H

#include "physics/Coordinates.h"
#include "physics/Search3D.h"

#define GAS_CONSTANT 8.3144598

class GasParticle
{
 public:
  GasParticle() {
    m_dist = 10.;
    m_compress = 1.;
    m_mass = 1.;
    m_content = 0;
    m_temp = 293.;
  }

  void SetDistance(double d) {m_dist = d;}
  double GetDistance() const {return m_dist;}

  void SetCompress(double d) {m_compress = d;}
  double GetCompress() const {return m_compress;}

  void SetMass(double d) {m_mass = d;}
  double GetMass() const {return m_mass;}

  void SetTemperature(double d) {m_temp = d;}
  double GetTemperature() {return m_temp;}

  void SetContent(int c) {m_content = c;}
  int GetContent() const {return m_content;}
  
  const Coordinates & Pos() const {return m_pos;}
  const Coordinates & Impulse() const {return m_impulse;}
  Coordinates & Pos() {return m_pos;}
  Coordinates & Impulse() {return m_impulse;}

 protected:
  Coordinates m_pos;
  Coordinates m_impulse;
  double m_dist;
  double m_compress;
  double m_mass;
  double m_temp;
  int m_content;
};


class Gas
{
 public:
  Gas() {}

  void SetBoundaries(const Coordinates & bottom, const Coordinates & top) {
    m_bottom = bottom;
    m_top = top;
    SetNumParticles();
  }

  void SetNumParticles();

  void Update(double deltatime);


  int isize() const {return m_particles.isize();}
  const GasParticle & operator[] (int i) const {return m_particles[i];}
  GasParticle & operator[] (int i) {return m_particles[i];}

 private:
  Coordinates m_bottom;
  Coordinates m_top;
  svec<GasParticle> m_particles;

  Search3D<GasParticle> m_search;

};



#endif
