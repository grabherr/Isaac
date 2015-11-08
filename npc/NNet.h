#ifndef NNET_H
#define NNET_H

#include "npc/NPCIO.h"


class Neuron
{
 public:
  Neuron() {}

  int isize() const {return m_data.isize();}
  void resize(int n) {return m_data.resize(n);}
  double & operator[] (int i) {return m_data[i];}
  const double & operator[] (int i) const {return m_data[i];}

  const svec<double> & Data() {return m_data;}

  void Update(const NPCIO & n, double weight) {
    int i;
    for (i=0; i<m_data.isize(); i++) {
      if (n.IsValid(i)) {
	m_data[i] = m_data[i]*(1.-weight) + weight*n[i];
      }
    }
  }

 private:
  svec<double> m_data;
};


class NeuralNetwork
{
 public:
  NeuralNetwork() {
    m_decay = 0.999;
    m_beta = .3;
    m_floor = 0.01;
    m_distance = 0.5;
  }

  void SetDecay(double d) {m_decay = d;}
  void SetBeta(double d) {m_beta = d;}
  void SetFloor(double d) {m_floor = d;}
  void SetDistance(double d) {m_distance = d;}


  int isize() const {return m_neurons.isize();}
  const Neuron & operator[] (int i) const {return m_neurons[i];}

  void Setup(int neurons, int dim);
 
  void MatchAndSort(svec<NPCIO_WithCoords> & n);

  int Best(const NPCIO & n);
  void Retrieve(NPCIO & n);
  void Learn(const NPCIO & n);

  void Print() const;

 private:
  svec<Neuron> m_neurons;
  double m_decay;
  double m_beta;
  double m_floor;
  double m_distance;
};


#endif //NNET_H

