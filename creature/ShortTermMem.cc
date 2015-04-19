#define FORCE_DEBUG

#include "creature/ShortTermMem.h"




void STMemChannel::Process(const MemoryUnit & prop, 
			   const Reaction & react, 
			   double weight)
{
  int i, j;

  int n = (int)(weight * 10.);
  cout << "size: " <<  m_mem.isize() << endl;
  for (i=0; i<n; i++) {
    cout << "Learning [0]... " << i << " " << prop.isize() << endl;
    m_mem[0].LearnAct(prop, prop.Valid(), react);
    cout << "Done." << endl;
  }
  /*for (i=m_mem.isize()-1; i>0; i--) {
    cout << "Learning... " << i << endl;
    m_mem[i].LearnFromMemory(m_mem[i-1], true);
    }*/
}

void STMemChannel::Propagate(bool forget)
{
  //cout << "Call propagate." << endl;
  // Route the info through the memory
  int i;
  for (i=m_mem.isize()-1; i>0; i--) {
    cout << "Learning... " << i << endl;
    m_mem[i].LearnFromMemory(m_mem[i-1], forget);
  }

}

bool STMemChannel::DensestMemory(Reaction & react, MemoryUnit & out, int layer) 
{
  svec<double> v;
  bool b = m_mem[layer].DensestVector(v);
  if (!b)
    return false;

  out.SetFromVec(react, v);
  return true;
}


void ShortTermMemory::Feed(const MemoryUnit & prop, 
			   const Reaction & react, 
			   double weight)
{
  int i;
  double min = 999999.;
  int best = -1;

  cout << "ShortTermMemory::Feed" << endl;

  for (i=0; i<m_mem.isize(); i++) {
    Reaction act; 
    cout << i << endl;
    double d = m_mem[i].Retrieve(act, prop);
    if (d < min) {
      min = d;
      best = i;
    }
  }
  cout << "Best: " << best << endl;
  m_mem[best].Process(prop, react);
  m_curr[best] = 1;
  PushWinner(best);
  cout << "Done processing." << endl;
}

void ShortTermMemory::Propagate()
{
  int i;
  //cout << "Top Propagate() " << endl;
  for (i=0; i<m_mem.isize(); i++) {
    if (m_curr[i])
      m_mem[i].Propagate(true);
    m_curr[i] = 0;
  }

}

void ShortTermMemory::Print() const
{
  cout << "Printing short-term memory." << endl;
  for (int i=0; i<m_mem.isize(); i++) {
    cout << "Channel: " << i << endl;
    m_mem[i].Print();
  }
}

void ShortTermMemory::PushWinner(int w)
{
  int i;
  for (i=m_winner.isize()-1; i>0; i--)
    m_winner[i] = m_winner[i-1];
  m_winner[0] = w;
}

void ShortTermMemory::MemoryDump(svec<Reaction> & react, 
				 svec<MemoryUnit> & mu,
				 const MemoryUnit & templ)
{
  
  svec<int> layer;
  layer.resize(m_mem.isize(), 0);

  int i, j;
  for (i=0; i<m_winner.isize(); i++) {
    int index = m_winner[i];
    if (index == -1)
      break;
    int n = layer[index];
    cout << "Winner " << index << " n: " << n << endl;
    Reaction r;
    MemoryUnit u;
    u = templ;
    if (n >= m_mem[index].isize())
      continue;
    bool b = m_mem[index].DensestMemory(r, u, n);
    layer[index]++;
    if (b) {
      react.push_back(r);
      mu.push_back(u);
    } else {
      cout << "No node" << endl;
    }
  }

}
