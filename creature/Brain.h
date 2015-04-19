#ifndef BRAIN_H
#define BRAIN_H

#include "creature/NervousSystem.h"
#include "creature/MemUnit.h"

class Brain
{
 private:
  Brain(int dataSize) {
    Init(dataSize);
  }

 public:
  Brain(const MemoryUnit & temp) {
    m_dataSize = temp.isize();
    m_template = temp;
    Init(m_dataSize);
  }

  void Init(int dataSize);

  void StartFeed();
  void Feed(const MemoryUnit & in);
  void FeedMe(const MemoryUnit & in);
  void DoneFeed();
  
  bool NextAction(MemoryUnit & out);

  void SetGoalExplicit(const MemoryUnit & in);

 protected:
  void Feed(const MemoryUnit & in, SingleLayerSOM & curr, bool bAssoc = true);
  void SLSOM2MU(MemoryUnit & temp, const Neuron & curr);
  
  MultiLayerSOM m_shortTerm;
  MultiLayerSOM m_longTermNoRot;
  MultiLayerSOM m_longTermRot;
  MultiLayerSOM m_concept;
  int m_dataSize;
  SingleLayerSOM m_curr;
  SingleLayerSOM m_goal;
  MemoryUnit m_template;

  svec<MemoryUnit> m_response;
};








#endif //BRAIN_H
