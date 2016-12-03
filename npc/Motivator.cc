#include "npc/Motivator.h"




void MotivationCategory::Read(CMReadFileStream &s)
{
  s.Read(m_name);
  s.Read(m_val);
  s.Read(m_weight);
  s.Read(m_show);
}

void MotivationCategory::Write(CMWriteFileStream &s)
{
  s.Write(m_name);
  s.Write(m_val);
  s.Write(m_weight);
  s.Write(m_show);
}

void EnvToMotMatrix::MatMult(svec<MotivationCategory> & out,
			     const svec<EnvFactor> & in,
			     const Coordinates & coords)
{
  int i, j;
  out.resize(m_data.isize());
  for (int i=0; i<m_data.isize(); i++) {
    double val = 0;
    for (int j=0; j<m_data[i].isize(); j++) {
      double v = in[j].GetVal();
      double d = in[j].GetDist();
      const Coordinates & c = in[j].GetCoords();
      double dd = (coords-c).Length();
      if (dd > d)
	continue;

      //if (dd < 1.) {
      //dd = 1;
      //} else {
      dd = 1-dd/d;
      //}
      
      val += (m_data[i])[j]*in[j].GetVal()*dd;
    }
    out[i].AddVal(val);
    
      
  }
}

void EnvToMotMatrix::Read(CMReadFileStream &s)
{

  int n, m;
  s.Read(n);
  s.Read(m);
  Resize(n, m);
  for (int i=0; i<m_data.isize(); i++) {
    for (int j=0; j<m_data[i].isize(); j++) {
      s.Read((m_data[i])[j]);
    }
  }
}

void EnvToMotMatrix::Write(CMWriteFileStream &s)
{
  s.Write(m_data.isize());
  s.Write(m_data[0].isize());
  for (int i=0; i<m_data.isize(); i++) {
    for (int j=0; j<m_data[i].isize(); j++) {
      s.Write((m_data[i])[j]);
    }
  }
}

//=====================================================

Motivator::Motivator()
{
}

void Motivator::PreSet() {
  m_mot.push_back(MotivationCategory("nopain", 1., 1., 1.));
  m_mot.push_back(MotivationCategory("eat", 1., 1., 1.));
  m_mot.push_back(MotivationCategory("sleep", 1., 1., 1.));
  m_mot.push_back(MotivationCategory("pee", 1., 1., 1.));
  m_mot.push_back(MotivationCategory("attractfemale", 1., 1., 1.));
  m_mot.push_back(MotivationCategory("attractmale", 1., 1., 1.));
  m_mot.push_back(MotivationCategory("annoy", 1., 1., 1.));
}


void Motivator::Read(CMReadFileStream &s)
{
  int i;
  int n;
  s.Read(n);
  m_mot.resize(n);
  for (i=0; i<n; i++)
    m_mot[i].Read(s);
  m_matrix.Read(s);
}

void Motivator::Write(CMWriteFileStream &s)
{
  s.Write(m_mot.isize());
  for (int i=0; i<m_mot.isize(); i++)
    m_mot[i].Write(s);
  m_matrix.Write(s);
}

void Motivator::ApplyEnv(const svec<EnvFactor> & in)
{
  m_matrix.MatMult(m_is, in, m_coords);
}

void Motivator::Print() const
{
  cout << "Print Motivator." << endl;
  for (int i=0; i<m_mot.isize(); i++) {
    cout << m_mot[i].GetName() << "\t" << m_mot[i].GetVal() << " <- " << m_is[i].GetVal() << endl;
  }
}
