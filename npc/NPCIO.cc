#include "npc/NPCIO.h"


void NPCIO::AddElement(const string & name)
{
  m_data.push_back(0.);
  m_valid.push_back(1);
  m_name.push_back(name);
}

void NPCIO::SetElement(const string & name, double v)
{
  int index = Index(name);
  m_data[index] = v;
}

void NPCIO::SetValid(const string & name, bool b)
{
  int index = Index(name);
  SetValid(index, b);
}

double NPCIO::GetElement(const string & name) const
{
  int index = Index(name);
  return m_data[index];
}

double NPCIO::IsValid(const string & name) const
{
  int index = Index(name);
  return IsValid(index);
}

double NPCIO::Distance(const NPCIO & n) const
{
  double d = 0.;
  double nn = 0.;

  for (int i=0; i<m_data.isize(); i++) {
    if (!IsValid(i) || n.IsValid(i))
      continue;

    double a = m_data[i];
    double b = n[i];
    nn += 1.;
    d += (a-b)*(a-b);
  }
  return d / nn;
}

double NPCIO::Distance(const svec<double> & n) const
{
  double d = 0.;
  double nn = 0.;

  for (int i=0; i<m_data.isize(); i++) {
    if (!IsValid(i))
      continue;

    double a = m_data[i];
    double b = n[i];
    nn += 1.;
    d += (a-b)*(a-b);
  }
  return d / nn;
}
