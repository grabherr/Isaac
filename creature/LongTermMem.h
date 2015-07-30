// (c) Manfred G. Grabherr

// This code is licensed under the GNU GENERAL PUBLIC LICENSE:
// http://www.gnu.org/licenses/gpl-3.0.en.html

#ifndef LONGTERMMEM_H
#define LONGTERMMEM_H

#include "creature/ShortTermMem.h"
#include "creature/MatchSOMs.h"

class LongTermMemory
{
 public:
  LongTermMemory(int channels, const MemoryUnit & templ, int depth = 10, int neurons = 30) {
    m_mem.resize(channels);
    for (int i=0; i<channels; i++)
      m_mem[i].Setup(templ, depth, neurons);
  }

  void Feed(const STMemChannel & channel, 
	    const Reaction & react);

  void Remember(STMemChannel & out,
		int futuredepth,
		const STMemChannel & channel, 
		const Reaction & react);
 
  
  
 private:
  void Find(int & index, 
	    int & sub,
	    const STMemChannel & channel, 
	    const Reaction & react);

  svec<STMemChannel> m_mem;
  TMatchMemory< TMemory < MemoryUnit > > m_match;
};


#endif 
