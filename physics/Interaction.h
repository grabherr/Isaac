#ifndef INTERACTION_H
#define INTERACTION_H

#include "base/SVector.h"
#include "base/SVector.h"
#include "physics/Coordinates.h"

// Defines the properties underlying any interaction 
// between the physical world and the creature
//=======================================================
class Properties
{
 public:
  Properties() {}

  int isize() const {return m_data.isize();}
  const double & operator[] (int i) const {return m_data[i];}
  double & operator[] (int i) {return m_data[i];}
  void resize(int n, double d=0.) {
    m_name.resize(n);
    m_data.resize(n, d);
  }
  const string & Name(int i) const {return m_name[i];} 
  void SetName(int i, const string & n) {m_name[i] = n;} 

  double Get(const string & name) const;
  void Set(const string & name, double val);
  void Add(const string & name, double val);
  void operator += (const Properties & ia);

 protected:
  int Index(const string & name) const;

  svec<string> m_name;
  svec<double> m_data;
};

//=======================================================
class PropertyMutator
{
 public:
  PropertyMutator() {}
  virtual ~PropertyMutator() {}

  void SetTemplate(const Properties & p) {
    m_template = p;
    m_min.resize(p.isize(), 0.);
    m_max.resize(p.isize(), 1.);
  }
  Properties New() {
    return m_template;
  }
  
  const Properties & GetTemplate () const {return m_template;}
  Properties & Template () {return m_template;}

  int isize() const {return m_template.isize();}
  double & operator[] (int i) {return m_template[i];}
  const double & operator[] (int i) const {return m_template[i];}

  double Min(int i) const {return m_min[i];}
  double Max(int i) const {return m_max[i];}
  double Rel(int i) const {
    return (m_template[i] - m_min[i])/(m_max[i]-m_min[i]);
  }

  void SetMin(int i, double v) {
    m_min[i] = v;
  }
  void SetMax(int i, double v) {
    m_max[i] = v;
  }
  
  // Reject returns how much was in excess
  void Bounds(Properties & reject, Properties & prop) {
    int i;
    reject = m_template;
    for (i=0; i<m_template.isize(); i++) {
      reject[i] = 0.;
      if (prop[i] > m_max[i]) {
	reject[i] = prop[i] - m_max[i];
	prop[i] = m_max[i];
      }
      if (prop[i] < m_min[i]) {
	reject[i] = prop[i] - m_min[i];
	prop[i] = m_min[i];
      }
    }
  }
  void Bounds(Properties & prop) {
    Properties reject;
    Bounds(reject, prop);
  }

 protected:
  
  Properties m_template;
  svec<double> m_min;
  svec<double> m_max;
  
};


// An entity is a thing with properties, coordinates, and name
class Entity
{
 public:
  Entity() {}

  Entity(const Properties & prop, const Coordinates & c, const string & name = "") {
    m_pos = c;
    m_prop = prop;
    m_name = name;
  }

  virtual ~Entity() {}
  
  int isize() const {return m_prop.isize(); }
  void resize(int n, double d = 0.)  {
    m_prop.resize(n, d);
  }
  
  const double & operator[] (int i) const {return m_prop[i];}
  double & operator[] (int i) {return m_prop[i];}
  const string & Name(int i) const {return m_prop.Name(i);}


  const Coordinates & GetCoordinates() const {return m_pos;}
  void SetCoordinates(const Coordinates & pos) {m_pos = pos;}

  const Coordinates & GetSpeed() const {return m_speed;}
  void SetSpeed(const Coordinates & s) {m_speed = s;}

  const string & GetName() const {return m_name;}
  void SetName(const string & s) {m_name = s ;}

  void operator += (const Entity & e) {
    m_prop += e.m_prop;
  }

  const Properties & GetProperties() const {return m_prop;}
  Properties & GetProperties() {return m_prop;}
  void SetProperties(const Properties & prop) {m_prop = prop;}

  double Get(const string & name) const {return m_prop.Get(name);}
  void Set(const string & name, double val) {m_prop.Set(name, val);}
  void Add(const string & name, double val) {m_prop.Add(name, val);}
 
 private:
  Coordinates m_pos;
  Coordinates m_speed;
  Properties m_prop;
  string m_name;
};




//=======================================================
class Interaction
{
 public:
  Interaction() {}
  virtual ~Interaction() {}

  // Relative coordinates
  void AddInteraction(const string & cause, 
		      const string & effect,
		      double level,
		      const Coordinates & c);
  
  // Note: effect must be properly set up here!!!
  // All changes are incremental.
  void Interact(Properties & effect, 
		const Properties & cause, 
		const Coordinates & c);

 protected:
  svec<Coordinates> m_coords;
  svec<string> m_cause;
  svec<string> m_effect;
  svec<double> m_diff;
};











#endif //INTERACTION_H


