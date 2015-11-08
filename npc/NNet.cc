#include "npc/NNet.h"
#include "base/RandomStuff.h"
#include <math.h>


void NeuralNetwork::Setup(int neurons, int dim)
{
  cout << "Setting up NN, dim=" << dim << endl;
  m_neurons.resize(neurons);
  for (int i=0; i<m_neurons.isize(); i++) {
    m_neurons[i].resize(dim);
    cout << "Resize to " << dim << " " << m_neurons[i].isize() << endl;
    for (int j=0; j<dim; j++)
      (m_neurons[i])[j] = RandomFloat(0.3);
  }

}
 
void NeuralNetwork::MatchAndSort(svec<NPCIO_WithCoords> & n)
{
  for (int i=0; i<n.isize(); i++) {
    int index = Best(n[i]);
    n[i].SetIndex(index);
  }
  Sort(n);
}

void NeuralNetwork::Print() const
{
  cout << "PRINT NN" << endl;
  cout << "beta:     " << m_beta << endl;
  cout << "decay:    " << m_decay << endl;
  cout << "floor:    " << m_floor << endl;
  cout << "distance: " << m_distance << endl;

  int i, j;
  for (i=0; i<m_neurons.isize(); i++) {
    cout << "Neuron " << i << " size: " << m_neurons[i].isize() << endl;
    for (j=0; j<m_neurons[i].isize(); j++) {
      cout << (m_neurons[i])[j] << " ";
    }
    cout << endl;
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
  double bestDist = n.Distance(m_neurons[index].Data());

  cout << "BestDist: " << bestDist << endl;

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
    //weight *= bestDist;
    cout << "Update w/ weight " << weight << " dist " << dist << " e " << exp(-dist) << endl;
    m_neurons[i].Update(n, weight);
    
  }
  
  m_beta *= m_decay;
  if (m_beta < m_floor)
    m_beta = m_floor;
}
