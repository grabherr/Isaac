#ifndef NNET_H
#define NNET_H

#include "npc/NPCIO.h"


class Neuron
{
 public:
  Neuron() {
    m_hit = 0.;
    m_avoid = 0.;
  }

  int isize() const {return m_data.isize();}
  void resize(int n) {return m_data.resize(n);}
  double & operator[] (int i) {return m_data[i];}
  const double & operator[] (int i) const {return m_data[i];}

  const svec<double> & Data() {return m_data;}

   void Update(const NPCIO & n, double weight) {
    int i;
    for (i=0; i<m_data.isize(); i++) {
      //if (n.IsValid(i)) {
      m_data[i] = m_data[i]*(1.-weight) + weight*n[i];
	//}
    }
  }

  void DeUpdate(const NPCIO & n, double weight) {
    int i;
    for (i=0; i<m_data.isize(); i++) {
      //if (n.IsValid(i)) {
      m_data[i] = m_data[i]*(1.-weight) - weight*n[i];
	//}
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

  double GetAvoid() const {return m_avoid;}
  void SetAvoid(double d) {m_avoid = d;}
  void IncAvoid() {
    AddAvoid(0.1);
  }
  void DecAvoid(double weight) {
    double fac = (1. - weight);
    m_avoid *= (1. - 0.1*fac);
  }

  void AddAvoid(double d, double limit = 1.) {
    m_avoid += d;
    if (m_avoid > limit)
      m_avoid = limit;
  }

  void DecayAvoid(double v) {
    m_avoid *= v;
  }

 private:
  double m_hit;
  double m_avoid;
  svec<double> m_data;
};


class NeuralNetwork
{
 public:
  NeuralNetwork() {
    m_decay = 0.999;
    m_decayAvoid = 0.95;
    m_beta = .3;
    m_floor = 0.01;
    m_distance = 0.5;
  }

  void SetDecay(double d) {m_decay = d;}
  void SetDecayAvoid(double d) {m_decayAvoid = d;}
  void SetBeta(double d) {m_beta = d;}
  void SetFloor(double d) {m_floor = d;}
  void SetNeuronDistance(double d) {m_distance = d;}


  int isize() const {return m_neurons.isize();}
  const Neuron & operator[] (int i) const {return m_neurons[i];}

  void Setup(int neurons, int dim);
  void ReSetup(int dim, double minus, double plus);
  void ReSetup(double minus, double plus);
 
  void MatchAndSort(svec<NPCIO_WithCoords> & n);

  double BestDist(const Neuron & n) const;
  int Best(const NPCIO & n);
  void Retrieve(NPCIO & n);
  void Learn(const NPCIO & n, double weight = 1., bool bUpHit = true);
  void LearnAvoid(const NPCIO & n, double weight = 1.);

  // This addes new info while preserving old stuff
  void LearnButPreserve(const NPCIO & n, int iter = 20);

  double GetDistance(const NeuralNetwork & n) const;

  void Train(const NeuralNetwork & n, int iter);

  void Print() const;

  const svec<double> & AllDist() const {return m_allDist;}

 private:
  double GetAvgAvoid() const {
    double a = 0.;
    for (int i=0; i<m_neurons.isize(); i++)
      a += m_neurons[i].GetAvoid();
    return a/(double)m_neurons.isize();
  }

  svec<Neuron> m_neurons;
  svec<double> m_low;
  svec<double> m_high;
  double m_decay;
  double m_decayAvoid;
  double m_beta;
  double m_floor;
  double m_distance;
  svec<double> m_allDist;
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

