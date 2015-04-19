#ifndef MEMUNIT_H
#define MEMUNIT_H


#include "physics/Interaction.h"
#include "creature/ActReact.h"

// Re-use the properties class for now
class Action : public Properties
{
};


// Stores the various aspects
class MemoryUnit
{
 public:
  MemoryUnit() {}

  MemoryUnit(const Entity & ent) {
    Action act;
    Reaction react;
    SphereCoordinates coord = ent.GetCoordinates().AsSphere();
    Set(ent, act, react, coord);
  }

  void Set(const Entity & ent, 
	   const Action & act, 
	   const Reaction & react,
	   const SphereCoordinates & coords) {
    m_ent = ent;
    m_act = act;
    m_react = react;
    m_coords.SetPhi(coords.phi()/2./PI_P);
    m_coords.SetTheta(coords.theta()/2./PI_P);
    m_coords.SetR(1./(coords.r()+1.));
    // cout << "Coords " << coords.r() << " " << m_coords.r() << endl;
  }

  void Set(const Entity & ent, 
	   const Action & act, 
	   const Reaction & react,
	   const Coordinates & coords) {
    SphereCoordinates sphere = coords.AsSphere();
    Set(ent, act, react, sphere);
  }
  
  // Indicates which fields are valid
  
  void Enable(bool bEnt, bool bAct, bool bReact, bool bCoords) {
    m_valid.resize(isize(), 0);
    int i;
    for (i=0; i<m_ent.isize(); i++) {
      if (bEnt)
	m_valid[i] = 1;
      else
	m_valid[i] = 0;
    }
    for (; i<m_act.isize(); i++) {
     if (bAct)
	m_valid[i] = 1;
      else
	m_valid[i] = 0;
    }
    for (; i<m_react.isize(); i++) {
     if (bReact)
	m_valid[i] = 1;
      else
	m_valid[i] = 0;
    }
    for (; i<m_coords.isize(); i++) {
     if (bCoords)
	m_valid[i] = 1;
      else
	m_valid[i] = 0;
    }  

    //cout << "Check " << m_coords.r() << endl;
  }

  void SetFromVec(Reaction & react, const svec<double> & v) {
    //if (v.isize() != isize())
    //cout << "SIZE ERROR!!" << endl;
    for (int i=0; i<isize(); i++)
      (*this)[i] = v[i];

    react.SetPain(v[v.isize()-3]);
    react.SetReward(v[v.isize()-2]);
    react.SetUnsuccess(v[v.isize()-1]);
  }

  int isize() const {
    return m_ent.isize() + m_act.isize() + m_react.isize() + m_coords.isize();
  }


  double & operator [] (int i) {
    if (i < m_ent.isize())
      return m_ent[i];

    if (i < m_ent.isize()+m_act.isize())
      return m_act[i-m_ent.isize()];

    if (i < m_ent.isize()+m_act.isize()+m_react.isize())
      return m_react[i-m_ent.isize()-m_act.isize()];

    // Return distance as special numbers
    return m_coords[i-m_ent.isize()-m_act.isize()-m_react.isize()];
  }

  const double & operator [] (int i) const {
    if (i < m_ent.isize())
      return m_ent[i];

    if (i < m_ent.isize()+m_act.isize())
      return m_act[i-m_ent.isize()];

    if (i < m_ent.isize()+m_act.isize()+m_react.isize())
      return m_react[i-m_ent.isize()-m_act.isize()];
    
    //cout << "operator[] " << i << " " << i-m_ent.isize()-m_act.isize()-m_react.isize() << " -> " << m_coords[i-m_ent.isize()-m_act.isize()-m_react.isize()] << endl;
    //cout << "coords: " << m_coords.r() << endl;
    return m_coords[i-m_ent.isize()-m_act.isize()-m_react.isize()];
  }


  const Entity & GetEntity() const {return m_ent;}
  const Action & GetAction() const {return m_act;}
  const Reaction & GetReaction() const {return m_react;}
  const SphereCoordinates GetCoordinates() const {
    SphereCoordinates tmp;
    tmp.SetPhi(m_coords.phi()*2.*PI_P);
    tmp.SetTheta(m_coords.theta()*2.*PI_P);
    tmp.SetR(1./m_coords.r()-1.);
    return tmp;
  }

  Coordinates GetCartCoordinates() const {
    Coordinates tmp;
    tmp.FromSphere(GetCoordinates());
    return tmp;
  }
  
  const svec<int> & Valid() const {return m_valid;} 

  void Print() const {
    cout << "Entity:" << endl;
    for (int i=0; i<m_ent.isize(); i++)
      cout << m_ent.Name(i) << " " << m_ent[i] << endl;
   
    cout << "Action:" << endl;
    for (int i=0; i<m_act.isize(); i++)
      cout << m_act.Name(i) << " " <<  m_act[i] << endl;
    
    cout << "Reaction: " << endl;
    m_react.Print();
    cout << "Coordinates (internal): r=" << m_coords.r() << " phi=" << m_coords.phi();
    cout << " theta=" << m_coords.theta() << endl;
    SphereCoordinates tmp = GetCoordinates();
    cout << "Coordinates (external): r=" << tmp.r() << " phi=" << tmp.phi();
    cout << " theta=" << tmp.theta() << endl;
  }


 protected:
  
  Entity m_ent;
  Action m_act;
  Reaction m_react;
  SphereCoordinates m_coords;
 
  
  svec<int> m_valid;
};






#endif //MEMUNIT_H

