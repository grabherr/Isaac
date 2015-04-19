#ifndef SPATASSOCMEM_H
#define SPATASSOCMEM_H

#include "core/SOM.h"
#include "physics/Interaction.h"

class AssociativeMemory
{
 public:
  AssociativeMemory() {
    m_weight = 5.;
    m_size = 0;
    m_win = 0;
    m_thresh = 0.01;
  }
  
  void Setup(int entsize, int window);
  int GetSize() const {return m_size;}

  // Bot learning and remebering
  void Associate(svec<Entity> & out, const svec<Entity> & ent, const Entity & which);
 

 private:
  bool Good(const Entity & one, const Entity & two);

  SelfOrgFeatMap m_mem;

  double m_weight;
  int m_size;
  int m_win;
  double m_thresh;

};

#endif //SPATASSOCMEM_H
