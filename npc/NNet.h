#ifndef NNET_H
#define NNET_H

#include "npc/NPCIO.h"


class Neuron
{
 public:
  Neuron() {
    m_hit = 0.;
  }

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
  
  double GetHit() const {return m_hit;}

  void AddHit(double a = 1.) {   
    double r = 0.5 * a;
    
    double d = 1.-m_hit;
    d *= (1.-r);
    m_hit = 1. - d;
  }

  void DecayHit(double dd=1.) {
    double d = 1. - 0.01*dd;
    m_hit *= d;
  }

  void AsNPCIO(NPCIO & out, const NPCIO & valid) const {
    out.resize(m_data.isize());
    cout << "new size " << out.isize() << endl;
    for (int i=0; i<m_data.isize(); i++) {
      out[i] = m_data[i];
      if (valid.IsValid(i))
	out.SetValid(i, true);
      else
	out.SetValid(i, false);
	
    }
  }
  void AsNPCIO(NPCIO & out) const {
    out.resize(m_data.isize());
    //cout << "new size " << out.isize() << endl;
    for (int i=0; i<m_data.isize(); i++) {
      out[i] = m_data[i];
    }
  }
  
  double Distance(const Neuron & n) const {
    double d = 0.;
    int i;
    for (int i=0; i<m_data.isize(); i++) {
      d += (m_data[i]-n[i])*(m_data[i]-n[i]);
    }
    return d;
  }

 private:
  double m_hit;
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
  void SetNeuronDistance(double d) {m_distance = d;}


  int isize() const {return m_neurons.isize();}
  const Neuron & operator[] (int i) const {return m_neurons[i];}

  void Setup(int neurons, int dim);
 
  void MatchAndSort(svec<NPCIO_WithCoords> & n);

  double BestDist(const Neuron & n) const;
  int Best(const NPCIO & n);
  void Retrieve(NPCIO & n);
  void Learn(const NPCIO & n, double weight = 1., bool bUpHit = true);

  // This addes new info while preserving old stuff
  void LearnButPreserve(const NPCIO & n, int iter = 20);

  double GetDistance(const NeuralNetwork & n) const;

  void Train(const NeuralNetwork & n, int iter);

  void Print() const;

 private:
  svec<Neuron> m_neurons;
  double m_decay;
  double m_beta;
  double m_floor;
  double m_distance;
};


// This is a chain of NNs, which store temporal information
class TemporalNN
{
 public:
  TemporalNN() {
    m_pointer = 0;
  }
  
  void SetSize(int neurons, int depth);

  // Get the best match
  void Retrieve(NPCIO & out, const NPCIO & in);
  void Learn(const NPCIO & n);

  // Learn from a single event, but don't destroy the memory
  // that's there already. 
  void LearnButPreserve(const NPCIO & n);

  // Switch frames, i.e. advance time
  void NewFrame();

  // Distance between two temporal memories
  double Compare(const TemporalNN & t);
  
  // Train it
  void Train(const TemporalNN & t, int iter);

  int Pointer() const {return m_pointer;}

  int isize() const {return m_nn.isize();}
  const NeuralNetwork & operator[] (int i) const {return m_nn[i];}
  NeuralNetwork & operator[] (int i) {return m_nn[i];}

  // Set the pointer to 0 (for easier debugging)
  void ReOrg();

 protected:
  svec<NeuralNetwork> m_nn;
  int m_pointer;
};

// This class holds different memories.
// TODO: organize it in a ring and update 
// multiple adjacent NNs (aka "dreaming")
class TemporalNNCluster
{
 public:
  TemporalNNCluster() {}

  void SetSize(int neurons, int depth, int clusters);

  void ReOrg();
  
  void Train(const TemporalNN & t, int iter);

  //const TemporalNN & GetBestOutcome

 private:
  svec<TemporalNN> m_cluster;
};



#endif //NNET_H

