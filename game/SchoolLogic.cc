#include "game/SchoolLogic.h"
#include <math.h>


void SchoolLogic::BeginRound()
{
  int i;
  for (i=0; i<m_char.isize(); i++) {
    m_char[i].Reset();
  }

}

double SchoolLogic::Attract(SchoolCharacter & in, SchoolCharacter & out)
{
  throw;
  // Female - female
  if (in.GetGender() < 0. && out.GetGender() < 0.) {
    if (in.GetAttract() < out.GetAttract()) {
      double m = (out.GetAttract() - in.GetAttract())*0.1;
      in.AddAttract(m);
    } else {
      double m = (in.GetAttract() - out.GetAttract())*0.1;
      in.AddAttract(-m);
      out.AddAttract(m/2.);
    }
  }
  // Female - male
  if (in.GetGender() < 0. && out.GetGender() < 0.) {
    out.AddAttract(.1);    
  }
  return 0.;
}

double SchoolLogic::Compete(double interact, double ext, const SchoolCharacter & c)
{
  throw;
  double is = c.GetStrength();
  double r = 0.;
  if (ext > is) {
    r = interact*0.1;
  } else {
    r = -interact*0.1;
  }
  return r;
}

void SchoolLogic::Resolve(SchoolCharacter & out, SchoolCharacter & in)
{
  double d_in = in.GetInteract();
  double s_in = in.GetSocialStatus();
  double s_out = out.GetSocialStatus();

  if (d_in > 0.) {
    double transIn = -(1.-s_out)*d_in*m_rate;
    double transOut = (s_in)*d_in*m_rate;
    in.AddSocialStatus(transIn);
    out.AddSocialStatus(transOut);
    cout << "Trans (+): " << transIn << " " << transOut << endl;
  } else {
    //double transOut = (1.-s_out)*d_in*m_rate;
    double floor = 0.2;
    double s_in_ad = (s_in+floor)/(1.+floor);
    double transIn = -s_in_ad*d_in*m_rate*exp(-(1.-s_in));
    
    if (transIn > out.GetSocialStatus())
      transIn = out.GetSocialStatus();
    double transOut = -transIn/2.;
   
    in.AddSocialStatus(transIn);
    out.AddSocialStatus(transOut);
    cout << "Trans (-): " << transIn << " " << transOut << endl;
    
  }
}
 

void SchoolLogic::Print() const
{
  for (int i=0; i<m_char.isize(); i++) {
    cout << "Character: " << m_char[i].GetName() << endl;
    cout << "   status:   " << m_char[i].GetSocialStatus() << endl;
    //cout << "   strength: " << m_char[i].GetStrength() << endl;
    //cout << "   attract:  " << m_char[i].GetAttract() << endl;
    cout << endl;
  }
}

void SchoolLogic::EndRound()
{
  int i;
  m_last = m_char;
  //svec<double> inter;
  //inter.resize(m_char.isize(), 0.);
  //svec<double> strength;
  //strength.resize(m_char.isize(), 0.);
  //svec<int> winloose;
  //winloose.resize(m_char.isize(), 0);

  double totalstrength = 0.;
  double totalattract = 0.;
  double totalsocial = 0.;


  for (i=0; i<m_char.isize(); i++) {
    totalstrength += m_char[i].GetStrength();
    totalattract += m_char[i].GetAttract();
    totalsocial += m_char[i].GetSocialStatus();
    
    int t = m_char[i].GetTarget();
    if (t < 0)
      continue;
    m_char[t].AddExtInteract(i, m_char[i].GetInteract());
    Resolve(m_char[t], m_char[i]);    
  }

  double str = 0.;
  double attr = 0.;
  double stat = 0.;
  
  for (i=0; i<m_char.isize(); i++) {
    str += m_char[i].GetStrength();
    attr += m_char[i].GetAttract();
    stat += m_char[i].GetSocialStatus();
  }

  double dist = 0.;
  double div = 0.;
  int nn = 0;
  for (i=0; i<m_char.isize(); i++) {
    m_char[i].AdjustStrength(totalstrength/str);
    m_char[i].AdjustAttract(totalattract/attr);
    m_char[i].AdjustSocialStatus(totalsocial/stat);
    if (m_char[i].GetSocialStatus() > 1.) {
      dist += m_char[i].GetSocialStatus() - 1.;
      m_char[i].SetSocialStatus(1.);
    } else {
      div += 1.;
      nn++;
    }
  }

  dist /= div;
  
  for (i=0; i<m_char.isize(); i++) {
    if (m_char[i].GetSocialStatus() < 1.) {
      m_char[i].SetSocialStatus(m_char[i].GetSocialStatus()+dist);      
    }
  }


  
  /*
  // Collect
  for (i=0; i<m_char.isize(); i++) {
    totalstrength += m_char[i].GetStrength();
    totalattract += m_char[i].GetAttract();
    totalsocial += m_char[i].GetSocialStatus();
    
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
      double diff = Compete(inter[i], strength[i], m_char[i]);
      double rest = m_char[i].AddStrength(diff);
      strength[i] = -diff + rest;      
    } else {
      //m_char[i].AddAttract(inter[i]*0.1);
    }
  }
  for (i=0; i<m_char.isize(); i++) {
    int t = m_char[i].GetTarget();
    if (t < 0)
      continue;
    Attract(m_char[i], m_char[t]);
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
  */
}
