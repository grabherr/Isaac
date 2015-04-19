#include "creature/MatchSOMs.h"

double MatchSOMs::GetMatchScore(const SelfOrgFeatMap & a, const SelfOrgFeatMap & b)
{
  int i;
  svec<double> one, two;
  a.Serialize(one);
  b.Serialize(two);

  int size = one.isize();
  if (two.isize() > size)
    size = two.isize();

  int mult = 2;
  while (mult < size) {
    mult *= 2;
  }
  one.resize(mult, 0.);
  two.resize(mult, 0.);

  svec<float> out;
  //cout << "XC " << one.isize() << " " << two.isize() << endl;
  m_xc.CrossCorrelate(out, one, two);
  //cout << "XC done." << endl;
  
  double max = 0.;
  for (i=0; i<out.isize(); i++) {
    if (out[i] > max)
      max = out[i];
  }
  
  return max;
}




