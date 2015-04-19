#include "creature/LongTermMem.h"

void LongTermMemory::Feed(const STMemChannel & channel, 
			  const Reaction & react)
{
  int index, index_sub;
  cout << "Find" << endl;
  Find(index, index_sub, channel, react);

  svec < TMemory < MemoryUnit > > & mT = m_mem[index].GetMemoryDirect();
  int i;
  
  for (i=0; i<channel.isize(); i++) {
    int what = (index_sub + i) % mT.isize();
    cout << "Learning " << what << " " << i << endl;
    mT[what].LearnFromMemory(channel[i]);
  }

}

void LongTermMemory::Remember(STMemChannel & out,
			      int futuredepth,
			      const STMemChannel & channel, 
			      const Reaction & react)
{
  int i, j;

  int index, index_sub;
  Find(index, index_sub, channel, react);

  const svec < TMemory < MemoryUnit > > & mT = m_mem[index].GetMemoryDirect();

  out.resize(channel.isize());
  for (i=0; i<out.isize(); i++) {
    int what = (index_sub + futuredepth + i) % mT.isize();
    out[i] = mT[what];
  }
}

void LongTermMemory::Find(int & index, int & index_sub, 
			  const STMemChannel & channel, 
			  const Reaction & react)
{
  int i, j;
  
  const svec < TMemory < MemoryUnit > > & mQ = channel.GetMemory();
  
  double max = 0.;
  index = -1;
  index_sub = -1;
  for (i=0; i<m_mem.isize(); i++) {
    svec < TMemory < MemoryUnit > > & mT = m_mem[i].GetMemoryDirect();
    svec<double> scores;
    cout << "GetMatchScore " << i << endl;
    double s = m_match.GetMatchScore(scores,
				     mT, 
				     mQ);
    if (s > max) {
      max = s;
      index = i;
      double local = 0.;
      for (j=0; j<scores.isize(); j++) {
	cout << "GetMatchScore local " << i << endl;
	if (scores[j] > local) {
	  local = scores[j];
	  index_sub = j;
	}	  
      }
    }
  }
  
}
