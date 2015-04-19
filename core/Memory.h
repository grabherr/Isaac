#ifndef MEMORY_H
#define MEMORY_H

#include "core/SOM.h"
#include "creature/ActReact.h"

class Memory
{
 public:
  Memory();
  void SetNumNeurons(int n, int featSize);

  void LearnAct(const svec<double> & feat, const Reaction & react);
  void LearnAct(const svec<double> & feat, const svec<int> & valid, const Reaction & react);
  double RetrieveAct(Reaction & react, const svec<double> & feat);

  void LearnFromMemory(const Memory & mem, bool forget = true);
  void Print() const;

  bool DensestVector(svec<double> & out);
  const SelfOrgFeatMap & GetSOM() const {return m_act;}

 private:
  SelfOrgFeatMap m_act;
  int m_featSizeIn;
  int m_featSizeOut;
  
};

template <class T>
class TMemory
{
 public:
  TMemory() {}

  void SetNumNeurons(int n, const T & t) {
    m_mem.SetNumNeurons(n, t.isize());
  }

  void LearnAct(const T & feat, const Reaction & react) {
    svec<double> d;
    d.resize(feat.isize(), 0);
    for (int i=0; i<feat.isize(); i++)
      d[i] = feat[i];
    m_mem.LearnAct(d, react);
  }

  void LearnAct(const T & feat, const svec<int> & valid, const Reaction & react) {
    svec<double> d;
    d.resize(feat.isize(), 0);
    for (int i=0; i<feat.isize(); i++)
      d[i] = feat[i];
    m_mem.LearnAct(d, valid, react);
  }
  
  double RetrieveAct(Reaction & react, const T & feat) {
    svec<double> d;
    d.resize(feat.isize(), 0);
    for (int i=0; i<feat.isize(); i++)
      d[i] = feat[i];
    return m_mem.RetrieveAct(react, d);
  }
 
  void LearnFromMemory(const TMemory<T> & mem, bool forget = true) {
    m_mem.LearnFromMemory(mem.m_mem, forget);
  }
  void Print() const {m_mem.Print();}

  bool DensestVector(svec<double> & out) {
    return m_mem.DensestVector(out);
  }
  const SelfOrgFeatMap & GetSOM() const {return m_mem.GetSOM();}

 private:
  Memory m_mem;
  
};

#endif //MEMORY_H


