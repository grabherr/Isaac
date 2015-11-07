#include "npc/NNet.h"
#include "base/RandomStuff.h"
#include <math.h>


void NeuralNetwork::Setup(int neurons, int dim)
{
  m_neurons.resize(neurons);
  for (int i=0; i<m_neurons.isize(); i++) {
    m_neurons[i].resize(dim);
    for (int j=0; j<dim; j++)
      (m_neurons[i])[j] = RandomFloat(0.3);
  }

}

int NeuralNetwork::Best(const NPCIO & n)
{
  int index = 0;
  int i;
  double dist = -1.;
  for (i=0; i<m_neurons.isize(); i++) {
    double d = n.Distance(m_neurons[i].Data());
    if (d < dist || dist < 0.) {
      index = i;
      dist = d;
    }      
  }
  return index;
}

void NeuralNetwork::Retrieve(NPCIO & n)
{
  int index = Best(n);
  n = m_neurons[index].Data();
}

void NeuralNetwork::Learn(const NPCIO & n)
{
  int index = Best(n);
  int i;

  for (i=0; i<m_neurons.isize(); i++) {
    double dist = i - index;
    if (dist < 0)
      dist = -dist;
    double dist2 = i - (index - m_neurons.isize());
    if (dist2 < 0)
      dist2 = -dist2;
    if (dist2 < dist)
      dist = dist2;
    dist *= m_distance;
    
    double weight = exp(-dist);
    weight *= m_beta;
    m_neurons[i].Update(n, weight);
    
  }
  
  m_beta *= m_decay;
  if (m_beta < m_floor)
    m_beta = m_floor;
}
