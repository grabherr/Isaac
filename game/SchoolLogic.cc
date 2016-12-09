#include "game/SchoolLogic.h"

void SchoolLogic::BeginRound()
{
  int i;
  for (i=0; i<m_char.isize(); i++) {
    m_char[i].Reset();
  }

}

double SchoolLogic::Compete(double interact, double ext, double is)
{
  double r = 0.;
  if (ext > is) {
    r = interact*0.1;
  } else {
    r = -interact*0.1;
  }
  return r;
}

void SchoolLogic::Print() const
{
  for (int i=0; i<m_char.isize(); i++) {
    cout << "Character: " << m_char[i].GetName() << endl;
    cout << "   gender:   " << m_char[i].GetGender() << endl;
    cout << "   strength: " << m_char[i].GetStrength() << endl;
    cout << "   attract:  " << m_char[i].GetAttract() << endl;
    cout << endl;
  }
}

void SchoolLogic::EndRound()
{
  int i;
  m_last = m_char;
  svec<double> inter;
  inter.resize(m_char.isize(), 0.);
  svec<double> strength;
  strength.resize(m_char.isize(), 0.);
  svec<int> winloose;
  winloose.resize(m_char.isize(), 0);

  double totalstrength = 0.;
  double totalattract = 0.;
  
  // Collect
  for (i=0; i<m_char.isize(); i++) {
    totalstrength += m_char[i].GetStrength();
    totalattract += m_char[i].GetAttract();
    int t = m_char[i].GetTarget();
    if (t < 0)
      continue;
    double d = m_char[i].GetInteract();
    double s = m_char[i].GetStrength();
    inter[t] += d;
    strength[t] += s;
  }
  //Print();
  // Execute
  for (i=0; i<m_char.isize(); i++) {
    if (inter[i] < 0.) {
      double diff = Compete(inter[i], strength[i], m_char[i].GetStrength());
      double rest = m_char[i].AddStrength(diff);
      strength[i] = -diff + rest;      
    } else {
      m_char[i].AddAttract(inter[i]*0.1);
    }
  }

  double str = 0.;
  double attract = 0.;
  
  for (i=0; i<m_char.isize(); i++) {
    str += m_char[i].GetStrength();
    attract += m_char[i].GetAttract();
    int t = m_char[i].GetTarget();
    if (t < 0)
      continue;
    //m_char[i].AddStrength(strength[i]);
    m_char[i].AddStrength(strength[t]);
  }

  //Print();
  //cout << totalstrength << " " << str << endl;
  for (i=0; i<m_char.isize(); i++) {
    m_char[i].AdjustStrength(totalstrength/str);
    m_char[i].AdjustAttract(totalattract/attract);
  }
}
