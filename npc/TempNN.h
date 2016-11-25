#ifndef TEMPNN_H
#define TEMPNN_H

#include "npc/NNet.h"


class TNN : public NeuralNetwork
{
 public:
  TNN() {}

  virtual void Learn(const NPCIO & n, double weight = 1., bool bUpHit = true);

};







#endif

