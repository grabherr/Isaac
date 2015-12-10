#include "physics/Gas.h"
#include "base/RandomStuff.h"
#include <math.h>

class IndexDist
{
public:
  IndexDist() {
    dist = 0.;
    index = -1;
  }

  IndexDist(int i, double d) {
    dist = d;
    index = i;
  }

  double dist;
  int index;

  bool operator < (const IndexDist & d) const {
    return dist < d.dist;
  }
};


void Gas::SetNumParticles()
{

 
  double x = m_top[0] - m_bottom[0];
  double y = m_top[1] - m_bottom[1];
  double z = m_top[2] - m_bottom[2];

  int n = (int)(x*y*z/20);

  m_particles.resize(n);

  int i;
  for (i=0; i<m_particles.isize(); i++) {
    GasParticle & p = m_particles[i];
    p.Pos()[0] = RandomFloat(m_bottom[0]+x);
    p.Pos()[1] = RandomFloat(m_bottom[1]+x);
    p.Pos()[2] = RandomFloat(m_bottom[2]+x);

    //double v = sqrt(3*GAS_CONSTANT*p.GetTemperature()/p.GetMass());
    double v = 1.;
    p.Impulse()[0] = RandomFloat(v*p.GetMass());
    p.Impulse()[1] = RandomFloat(v*p.GetMass());
    p.Impulse()[2] = RandomFloat(v*p.GetMass());
  }
  
  cout << "Num particles " << n << endl;

}

double FixZero(double x, double p, double low, double hi)
{
  if (x < low) {
    if (p < 0)
      return -p;    
  }
  if (x > hi) {
    if (p > 0)
      return -p;
  }
  return p;
}


void Gas::Update(double deltatime)
{
  //cout << "Enter" << endl;

  
  int i, j;
  double gravity = 9.81;
  // Update positions
  for (i=0; i<m_particles.isize(); i++) {
    GasParticle & p = m_particles[i];
    p.Pos()[0] += deltatime*p.Impulse()[0]/p.GetMass();
    p.Pos()[1] += deltatime*p.Impulse()[1]/p.GetMass();
    p.Pos()[2] += deltatime*p.Impulse()[2]/p.GetMass();
    
    //p.Impulse() *= 0.9;

    p.Impulse()[1] -= deltatime*gravity*p.GetMass();
    
    p.Impulse()[0] = FixZero(p.Pos()[0], p.Impulse()[0], m_bottom[0], m_top[0]);
    p.Impulse()[1] = FixZero(p.Pos()[1], p.Impulse()[1], m_bottom[0], m_top[0]);
    p.Impulse()[2] = FixZero(p.Pos()[2], p.Impulse()[2], m_bottom[0], m_top[0]);

  }

  
  m_search.SetTo(m_particles);
  
  
  
  for (i=0; i<m_particles.isize(); i++) {
    int index = RandomInt(m_particles.isize());
    GasParticle & p = m_particles[index];

    svec<int> out;
    m_search.Search(out, m_particles, p, p.GetDistance());
  
    Coordinates delta;
    double n = 1.;
    for (j=0; j<out.isize(); j++) {      
      int k = out[j];
      if (index == k)
	continue;
      Coordinates dir = p.Pos()-m_particles[k].Pos();
      double d = dir.Length();
       
      double w = 10./(d + 10.);
      delta += dir * w;
      n += 1;
    }
    p.Impulse() += delta * deltatime;
  }
  //cout << "Exit" << endl;
   
}
















