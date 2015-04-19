#ifndef CREATURE_H
#define CREATURE_H

#include "base/SVector.h"
#include "physics/Interaction.h"

// Note that extras, e.g. weapons, are also "body" parts
class BodyPart
{
 public:
  BodyPart() {}
 
  void SetProperties(const Properties & prop) {
    m_prop = prop;
  }
  void SetMutator(const PropertyMutator & mut) {
    m_mut = mut;
  }
  void SetName(const string & name) {
    m_name = name;
  }
  PropertyMutator & Mutator() {return m_mut;}
  Properties & MyProperties() {return m_prop;}
  const PropertyMutator & GetMutator() const {return m_mut;}
  const Properties & GetProperties() const {return m_prop;}
  const string & Name() const {return m_name;}

 private:
  string m_name;
  PropertyMutator m_mut;
  Properties m_prop;
};

// A creature without any will of its own
class Ragdoll
{
 public:
  Ragdoll() {}
  virtual ~Ragdoll() {}

  void AddPart(const BodyPart & part) {
    m_parts.push_back(part);
  }

  void RemovePart(const string & name) {
    int index = Index(name);
    if (index == -1)
      return;
    m_parts[index] = m_parts[m_parts.isize()-1];
    m_parts.resize(m_parts.isize()-1);
  }

  int isize() const {return m_parts.isize();}
  BodyPart & operator[] (int i) {return m_parts[i];}
  const BodyPart & operator[] (int i) const {return m_parts[i];}
  BodyPart & Get(const string & s) {
    return m_parts[Index(s)];
  }


 protected:
  int Index(const string & name) const {
    for (int i=0; i<m_parts.isize(); i++) {
      if (m_parts[i].Name() == name)
	return i;
    }
    return -1;
  } 

  svec<BodyPart> m_parts;
};



#endif //CREATURE_H
