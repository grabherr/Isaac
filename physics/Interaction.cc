#include "physics/Interaction.h"



void Properties::operator += (const Properties & ia) 
{
  bool bOK = true;
  int i;
  for (int i=0; i<m_name.isize(); i++) {
    if (m_name[i] == ia.m_name[i]) {
      m_data[i] += ia.m_data[i];
    } else {
	bOK = false;
	break;
    }
  }
  if (bOK)
    return;
  for (i=0; i<ia.isize(); i++) {
    Add(ia.m_name[i], ia.m_data[i]);
  }
}

double Properties::Get(const string & name) const
{
  int index = Index(name);
  if (index == -1) {
    cout << "Properties ERROR, not found: " << name << endl;
    return -1.;
  }
  return m_data[index];
}

void Properties::Set(const string & name, double val)
{
  int index = Index(name);
  if (index == -1) {
    cout << "Properties ERROR, not found: " << name << endl;
    return;
  }
  m_data[index] = val;
}

void Properties::Add(const string & name, double val)
{
  int index = Index(name);
  if (index == -1) {
    //cout << "Adding " << name << endl;
    m_data.push_back(0);
    m_name.push_back(name); 
    index = m_name.isize()-1;
  }
  m_data[index] += val;
}

int Properties::Index(const string & name) const
{
  int i;
  for (i=0; i<m_name.isize(); i++) {
    if (m_name[i] == name)
      return i;
  }
  //cout << "Properties ERROR, not found: " << name << endl;
  return -1;
}


//========================================================
void Interaction::AddInteraction(const string & cause, 
				 const string & effect,
				 double diff,
				 const Coordinates & c)
{
  m_cause.push_back(cause);
  m_effect.push_back(effect);
  m_diff.push_back(diff);
  m_coords.push_back(c);
}

  // Relative coordinates
void Interaction::Interact(Properties & effect, 
			   const Properties & cause, 
			   const Coordinates & c) 
{
  int i, j;
  for (i=0; i<cause.isize(); i++) {
    for (j=0; j<m_cause.isize(); j++) {
      if (m_cause[j] == cause.Name(i)) {
	if (c < m_coords[i])
	  effect.Add(cause.Name(i), m_diff[i]*cause[i]);
      }
    }
  }
}
