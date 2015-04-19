#ifndef SHORTTERMMEM_H
#define SHORTTERMMEM_H

#include "core/Memory.h"
#include "physics/Interaction.h"
#include "creature/ActReact.h"
#include "creature/MemUnit.h"

class STMemChannel
{
 public:
  STMemChannel() {}
  
  // Size of the unit
  void Setup(const MemoryUnit & templ, int depth, int neurons = 100) {
    m_mem.resize(depth);
    for (int i=0; i<depth; i++)
      m_mem[i].SetNumNeurons(neurons, templ);
  }

  void Process(const MemoryUnit & unit, 
	       const Reaction & react, 
	       double weight = 1.);

  void Propagate(bool forget);

  // Use top only for now...
  double Retrieve(Reaction & react, const MemoryUnit & unit) {
    //cout << "Channel retrieve. " << unit.isize() << endl;
    return m_mem[0].RetrieveAct(react, unit);
    //cout << "Done." << endl;
  }

  void Print() const {
    for (int i=0; i<m_mem.isize(); i++) {
      cout << "Layer " << i << endl;
      m_mem[i].Print();
    }
  }

  bool DensestMemory(Reaction & react, MemoryUnit & out, int layer);

  int isize() const {return m_mem.isize();}
  void resize(int n) {m_mem.resize(n);}
  const TMemory< MemoryUnit > & operator[] (int i) const {return m_mem[i];}
  TMemory< MemoryUnit > & operator[] (int i) {return m_mem[i];}
  const svec< TMemory< MemoryUnit > > & GetMemory() const {return m_mem;}
  svec< TMemory< MemoryUnit > > & GetMemoryDirect() {return m_mem;}

 protected:
  svec< TMemory< MemoryUnit > > m_mem;
};

class ShortTermMemory
{
 public:
  ShortTermMemory(int channels, const MemoryUnit & templ, int depth = 10, int neurons = 30) {
    m_mem.resize(channels);
    m_curr.resize(channels, 0);
    for (int i=0; i<channels; i++)
      m_mem[i].Setup(templ, depth, neurons);
    m_winner.resize(depth*channels, -1);
  }

  // Feed all events from a single frame
  void Feed(const MemoryUnit & prop, 
	    const Reaction & react, 
	    double weight = 1.);
 
  // Prpagate down the temporal queue
  void Propagate();

  const svec<STMemChannel> & GetMemory() const {return m_mem;}
  void Print() const;
  
  // Result comes as: youngest first, oldest last
  void MemoryDump(svec<Reaction> & react,
		  svec<MemoryUnit> & mu,
		  const MemoryUnit & templ);


  int isize() const {return m_mem.isize();}
  const STMemChannel & operator[] (int i) const {return m_mem[i];}
  STMemChannel & operator[] (int i) {return m_mem[i];}
 private:
  void PushWinner(int w);
  svec<int> m_curr;
  svec<STMemChannel> m_mem;
  svec<int> m_winner;
};

#endif //SHORTTERMMEM_H




