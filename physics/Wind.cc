#define FORCE_DEBUG

#include "physics/Wind.h"
#include "base/RandomStuff.h"

double WindCheckPoint::UpdateFrom(const Coordinates & me, 
				  const WindCheckPoint & w, 
				  const Coordinates & other,
				  double deltatime)
{

  Coordinates loc = me - other;
  Coordinates dir = w.Direction();

  double proj = loc.Scalar(dir);
  if (proj < 0)
    return 0.;

  double v = dir.Length();
  //v = 1.;
  
  // Do the rotation in euklidean coords, not spheres.
  SphereCoordinates sloc = loc.AsSphere();
  //cout << "theta sloc " << sloc.theta() << endl;
  //cout << "phi sloc   " << sloc.phi() << endl;
  SphereCoordinates ls = dir.AsSphere();
  //cout << "theta ls   " << ls.theta() << endl;
  //cout << "phi ls     " << ls.phi() << endl;
  ls -= sloc;
  //cout << "theta diff " << ls.theta() << endl;
  //cout << "phi   diff " << ls.phi() << endl;

  double theta = ls.theta();
  double phi = ls.phi();
  double fac = 1./1.4; // Whatever factor

  double div = 6 + 12./sqrt(2.) + 8./sqrt(3.); 

  
  //cout << "Before theta: " << theta << endl; 
  //theta += PI_P/2.;
  if (theta <= PI_P/2. && theta >= -PI_P/2.) {
    theta *= fac; 
  } else {
    //cout << "This one: " << theta << " -> " << PI_P + (theta - PI_P)*fac << " " << PI_P + (theta + PI_P)*fac << endl;
    if (theta > 0)
      theta = PI_P + (theta - PI_P)*fac;
    else
      theta = - (PI_P + (theta + PI_P)*fac);
  }
  //theta -= PI_P/2.;
  
  
  //cout << "Before phi " << phi << endl;
  if (phi <= PI_P/2. && phi >= -PI_P/2.) {
    phi *= fac; 
  } else {
    if (phi > 0)
      phi = PI_P + (phi - PI_P)*fac;
    else
      phi = - (PI_P + (phi + PI_P)*fac);
  }
  
  //cout << "After theta:  " << theta << " phi " << phi << endl;

  ls.SetTheta(theta);
  ls.SetPhi(phi);
  double r = v*deltatime;
  r *= 1./(loc.Length()*div);
  //cout << "r=" << r << " v=" << v << " div=" << div << " len=" << loc.Length() << endl;
  ls.SetR(r);
  Coordinates t1;
  t1.FromSphere(ls);
  //cout << "Before back " << t1[0] << " " << t1[1] << " " << t1[2] << " theta " << ls.theta() << " phi " << ls.phi() <<  endl;
  
  ls += sloc;
  //cout << "Before back (2) "  << " theta " << ls.theta() << " phi " << ls.phi() << endl;
  
  Coordinates t;
  t.FromSphere(ls);
  //cout << "Back to " << t[0] << " " << t[1] << " " << t[2] << endl;
  //cout << "Speed other: " << v << " " << t[0] << " " << t[1] << " " << t[2] << endl;

  m_dir += t;
  return r;

}

void Wind::Print() const 
{
  int i, j, k;
  for (i=0; i<X(); i++) {
    for (j=0; j<Y(); j++) {
      for (k=0; k<Z(); k++) {
	const WindCheckPoint & a = Get(i, j, k);
	cout << i << " " << j << " " << k << " -> ";
	cout << a.Direction()[0] << " ";
 	cout << a.Direction()[1] << " ";
 	cout << a.Direction()[2] << endl;
      }
    }
  }
}

void Wind::Clear()
{
  int i, j, k;
  for (i=0; i<X(); i++) {
    for (j=0; j<Y(); j++) {
      for (k=0; k<Z(); k++) {
	WindCheckPoint & a = Get(i, j, k);
	a.Direction() = Coordinates(0., 0., 0.);
      }
    }
  }

}

void Wind::Process(const IWindShape & shape)
{
  int i, j, k;
  for (i=0; i<X(); i++) {
    for (j=0; j<Y(); j++) {
      for (k=0; k<Z(); k++) {
	WindCheckPoint & a = Get(i, j, k);
	Coordinates plus;
	shape.Compute(plus, Coordinates(i, j, k));
	a.Direction() += plus;
      }
    }
  }
}

void Wind::Update(double deltatime)
{

  int i, j, k;
  int l, m, n;

  Wind tmp = *this;
  //tmp.SetSize(X(), Y(), Z());

  for (i=0; i<X(); i++) {
    for (j=0; j<Y(); j++) {
      for (k=0; k<Z(); k++) {
	//cout << "**** From " << i << " " << j << " " << k << endl;
	const WindCheckPoint & a = Get(i, j, k);
	double out = 0.;
	for (l=i-1; l<=i+1; l++) {
	  for (m=j-1; m<=j+1; m++) {
	    for (n=k-1; n<=k+1; n++) {
	      //cout << "**** (1)" << l << " " << m << " " << n << endl;
	      if (!Valid(l, m, n))
		continue;
	      //cout << "**** (2)" << l << " " << m << " " << n << endl;
	      if (i == l && j == m && k == n)
		continue;
	      WindCheckPoint & b = tmp.Get(l, m, n);
	      //cout << "**** Got " << l << " " << m << " " << n << endl;
	      out += b.UpdateFrom(Coordinates(l, m, n), 
				  a, 
				  Coordinates(i, j, k), 
				  deltatime);
	      //b.Direction() *= m_decay;
	    }
	  }
	}
	const WindCheckPoint & d = Get(i, j, k);
	WindCheckPoint & c = tmp.Get(i, j, k);
	//cout << i << " " << j << " " << k << endl;
	//cout << "Speed before " << d.Direction().Length() << " removed: " << out << endl;
	double v = d.Direction().Length();
	//v = 1.;
	if (i == 0) {
	  //cout << "Subtract " << d.Direction().Length() * deltatime * v << " from " << c.Direction().Length() << endl;
	    
	}
	c.Direction() -= d.Direction() * deltatime * v;
	//cout << "Speed after  " << c.Direction().Length() << endl;
      }      
    }    
  }
  *this = tmp;
}






void ParticleMovement::Setup(int n, const Coordinates & bounds)
{
  m_bounds = bounds;
  m_particles.resize(n);
  m_used = 0;
}

void ParticleMovement::AddRandomNoise(double v)
{
  int i;

 
  for (i=0; i<m_particles.isize(); i++) {
     m_particles[i].Velocity() += Coordinates(v/2. - RandomFloat(v),
					      RandomFloat(0.),
					      v/2. - RandomFloat(v));
     //if (m_particles[i].Velocity()[2] < 5.3)
     //m_particles[i].Velocity()[2] += .2;
  }
}
void ParticleMovement::AddRandom(int n)
{
  int i;

  double v = 0.9;

  for (i=0; i<n; i++) {
     m_particles[i].Position() = Coordinates(RandomFloat(m_bounds[0]),
					     RandomFloat(m_bounds[1]),
    					    RandomFloat(m_bounds[2]));
    //m_particles[i].Position() = Coordinates(RandomFloat(bounds[0]),
    //					    RandomFloat(bounds[1]),
    //					    RandomFloat(12));
    m_particles[i].Velocity() = Coordinates(v/2. - RandomFloat(v),
					    RandomFloat(0.),
					    v/2. - RandomFloat(v));
    //cout << m_particles[i].Position()[0] << endl;
  }
}

void FixZeroZ(double & x, double & p, double low, double hi)
{
  if (x < low) {
    x = 0.;
    if (p < 0)
      p = -p;    
  }
  if (x >= hi) {
    x = hi-0.5;
    //cout << "Fixing " << p << endl;
    if (p > 0)
      p = -p;
    //cout << "Fixed " << p << endl;
  }
}

void ParticleMovement::Move(const Wind & wind, double deltatime)
{

  int i;
  for (i=0; i<m_first; i++) {
    WindParticle & w = m_particles[i];

    int x = w.Position()[0];
    int y = w.Position()[1];
    int z = w.Position()[2];

    if (wind.Valid(x, y, z)) {
      const WindCheckPoint & c = wind.Get(x, y, z);
      //if (w.Velocity().Length() < c.Direction().Length())
      w.Velocity() += c.Direction();
      
    }

    w.Position() += w.Velocity()*deltatime;
    w.Velocity() *= m_decay;

    FixZeroZ(w.Position()[0], w.Velocity()[0], 0, m_bounds[0]);
    FixZeroZ(w.Position()[1], w.Velocity()[1], 0, m_bounds[1]);
    FixZeroZ(w.Position()[2], w.Velocity()[2], 0, m_bounds[2]);
    //cout << "Moving " << i << " " << w.Position()[0] << " " << w.Position()[2] << " " << w.Velocity()[2] << endl;
  }
  //cout << "Moved " << m_first << endl;
}


void ParticleMovement::Add(int n, const Coordinates & coords, double spread)
{
  int i;
  if (m_used == m_particles.isize())
    m_used = 0;
  //cout << "Adding " << n << " " << m_used << " " << m_first << endl;
  //cout << "Try add" << endl;
  int kk = 0;
  for (i=m_used; i<m_used+n; i++) {
    m_first++;
    kk++;
    if (i >= m_particles.isize()) {
      n -= (i - m_used);
      i = 0;
      m_used = 0;      
    }

    //cout << "Adding " << i << endl;

    double a = coords[0] + spread/2 - RandomFloat(spread);
    double b = coords[1] + spread/2 - RandomFloat(spread);
    double c = coords[2] + spread/2 - RandomFloat(spread);

    if (a < 0)
      a = 0;
    if (a >= m_bounds[0])
      a = m_bounds[0]-0.5;

    if (b < 0)
      b = 0;
    if (b >= m_bounds[1])
      b = m_bounds[1]-0.5;

    if (c < 0)
      c = 0;
    if (c >= m_bounds[2])
      c = m_bounds[2]-0.5;

    m_particles[i].Position() = Coordinates(a, b, c);
    double v = 0.9;
    m_particles[i].Velocity() = Coordinates(2*v/2. - RandomFloat(2*v),
    					    RandomFloat(0.),
    					    v/2. - RandomFloat(v));
    //m_particles[i].Velocity() = Coordinates(0,
    //					    0,
    //					    0);


    //cout << "Adding in " << i << " " << a << " " << b << " " << c << " " << m_particles[i].Velocity()[2] << endl;
  }
  m_used += n;
  if (m_first >= m_particles.isize())
    m_first = m_particles.isize();

  // cout << "Added " << kk << endl;
}
