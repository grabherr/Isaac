#include "npc/NNet.h"
#include "base/RandomStuff.h"
#include <math.h>


void NeuralNetwork::Setup(int neurons, int dim)
{
  cout << "Setting up NN, dim=" << dim << endl;
  m_neurons.resize(neurons);
  for (int i=0; i<m_neurons.isize(); i++) {
    m_neurons[i].resize(dim);
    // cout << "Resize to " << dim << " " << m_neurons[i].isize() << endl;
    for (int j=0; j<dim; j++) {
      (m_neurons[i])[j] = RandomFloat(0.3);
    }
  }
  m_low.resize(dim, 0.);
  m_high.resize(dim, 0.3);
  m_allDist.resize(neurons, 0.);
}

void NeuralNetwork::ReSetup(double minus, double plus)
{
  for (int i=0; i<m_neurons.isize(); i++) {
    for (int j=0; j<m_neurons[i].isize(); j++) {
      //cout << i << " " << j << endl;
      (m_neurons[i])[j] = minus + RandomFloat(plus-minus);
      m_low[j] = minus;
      m_high[j] = plus;
    }
    //cout << "i=" << i << " " << "dim=" << dim << " val=" << (m_neurons[i])[dim] << endl;
  }
}
void NeuralNetwork::ReSetup(int dim, double minus, double plus)
{
  cout << "Re-Setting up NN for dim=" << dim << endl;
  for (int i=0; i<m_neurons.isize(); i++) {
    (m_neurons[i])[dim] = minus + RandomFloat(plus-minus);
  }
  m_low[dim] = minus;
  m_high[dim] = plus;
   
    //cout << "i=" << i << " " << "dim=" << dim << " val=" << (m_neurons[i])[dim] << endl;
  m_allDist.resize(dim, 0.);


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
    cout << "Neuron " << i << " size: " << m_neurons[i].isize() << " -> ";
    for (j=0; j<m_neurons[i].isize(); j++) {
      cout << (m_neurons[i])[j] << " ";
    }
    cout << " hits: " << m_neurons[i].GetHit(); 
    cout << " avoid: " << m_neurons[i].GetAvoid(); 
    cout << endl;
  }

}

double NeuralNetwork::BestDist(const Neuron & n) const
{
  int i;
  double dist = -1.;
  for (i=0; i<m_neurons.isize(); i++) {
    double d = n.Distance(m_neurons[i]);
    if (d < dist || dist < 0.) {     
      dist = d;
    }      
  }
  return dist;
}

int NeuralNetwork::Best(const NPCIO & n)
{
  int index = 0;
  int i;
  double dist = -1.;

  //cout << "Best " << m_allDist.isize() << endl;
  for (i=0; i<m_neurons.isize(); i++) {
    double d = n.Distance(m_neurons[i].Data());
    //cout << "Neuron " << i << " " << n.Distance(m_neurons[i].Data()) << " " <<  m_neurons[i].GetAvoid() << endl;
    m_allDist[i] = d;
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
  //cout << "Search for best hit: " << index << " " << n[0] << " " << n[1] << endl;
  n = m_neurons[index].Data();
  n.SetNeuron(index);
  double avoid = m_neurons[index].GetAvoid();

  
  double all = GetAvgAvoid();
  if (avoid > all*0.9 && avoid > 0.01) {
    //cout << "Venturing a random guess!" << endl;
    //cout << "WARNING: Imformed guess not available!!" << endl;
    for (int i=0; i<n.isize(); i++) {
      //if (RandomFloat(1.) > 0.5)
      //n[i] = m_low[i];
      //else
      //n[i] = m_high[i];
	
      //n[i] = m_low[i] + RandomFloat(m_high[i]-m_low[i]);
    }
  }

  //cout << "Best hit: " << index << " " << n[0] << " " << n[1];
  //cout << " avoid " << m_neurons[index].GetAvoid() << endl;
  
}

void NeuralNetwork::Learn(const NPCIO & n, double ext_weight, bool bUpHit)
{
  int index = Best(n);
  //cout << "Best hit for learn: " << index << " " << n[0] << " " << n[1] << " avoid " << m_neurons[index].GetAvoid() << endl;
  int i;
  double bestDist = n.Distance(m_neurons[index].Data());
  if (bUpHit)
    m_neurons[index].AddHit(ext_weight);
  
  //cout << "Best: " << index << " -> " << bestDist << endl;

  for (i=0; i<m_neurons.isize(); i++) {
    double dist = i - index;
    int tDist = i-index;
    if (dist < 0)
      dist = -dist;
    
    if (dist > m_neurons.isize()/2) {
      dist = m_neurons.isize() - dist;
      tDist = -tDist;
    }
    
    
    dist *= m_distance;
    
    double weight = exp(-dist);
    weight *= m_beta;
    weight *= ext_weight;
    
    m_neurons[i].Update(n, weight, m_timeShift*tDist);
    m_neurons[i].DecAvoid(weight);
    if (bUpHit)
      m_neurons[i].DecayHit(ext_weight);
    m_neurons[i].DecayAvoid(m_decayAvoid);
  }
  
  m_beta *= m_decay;
  if (m_beta < m_floor)
    m_beta = m_floor;
}

void NeuralNetwork::LearnAvoid(const NPCIO & n, double weight)
{
  int index = Best(n);
  int i;
  double bestDist = n.Distance(m_neurons[index].Data());

  //cout << "Avoid neuron " << index << endl;
  double sum = 0.;
  for (i=0; i<m_neurons.isize(); i++) {
    double dist = i - index;
    if (dist < 0)
      dist = -dist;
     
    if (dist > m_neurons.isize()/2)
      dist = m_neurons.isize() - dist;

    dist *= m_distance;
    
    double w = exp(-dist);
    w *= weight;
    w *= m_beta;
    m_neurons[i].AddAvoid(w);
    //cout << "De-update with weight " << w << " dist: " << dist << endl;
    //m_neurons[i].DeUpdate(n, w);
    m_neurons[i].DecayAvoid(m_decayAvoid);
    sum += m_neurons[i].GetAvoid();
  }
  
}

double NeuralNetwork::GetDistance(const NeuralNetwork & n) const
{
  double d = 0.;
  for (int i=0; i<n.isize(); i++) {
    d += BestDist(n[i]);
  }
  return d;
}

void NeuralNetwork::Train(const NeuralNetwork & n, int iter)
{
  int i, j;
  
  svec<NPCIO> tmp;
  svec<double> w;

  tmp.resize(n.isize());
  w.resize(n.isize());
  for (i=0; i<n.isize(); i++) {
    n[i].AsNPCIO(tmp[i]);
    w[i] = n[i].GetHit();
  }

  for (j=0; j<iter; j++) {    
    for (i=0; i<tmp.isize(); i++) {    
      Learn(tmp[i], w[i]);
    }
  }

}

void NeuralNetwork::LearnButPreserve(const NPCIO & n, int iter)
{
  int i, j;
  
  //cout << "Into loop." << endl;
  svec<NPCIO> tmp;
  svec<double> w;

  tmp.resize(m_neurons.isize());
  w.resize(m_neurons.isize());
  for (i=0; i<m_neurons.isize(); i++) {
    m_neurons[i].AsNPCIO(tmp[i], n);
    w[i] = m_neurons[i].GetHit();
  }

  for (j=0; j<iter; j++) {
    Learn(n);
    for (i=0; i<m_neurons.isize(); i++) {
      //cout << i << endl;
      Learn(tmp[i], w[i]);
    }
  }
}


//===================================================


void TemporalNN::SetSize(int neurons, int depth)
{
  m_nn.resize(depth);
  for (int i=0; i<m_nn.isize(); i++)
    m_nn[i].Setup(neurons, 1);
}

void TemporalNN::Retrieve(NPCIO & out, const NPCIO & in)
{
  out = in;
  m_nn[m_pointer].Retrieve(out);
}

void TemporalNN::Learn(const NPCIO & n)
{
  m_nn[m_pointer].Learn(n);
}

void TemporalNN::LearnButPreserve(const NPCIO & n)
{
  m_nn[m_pointer].LearnButPreserve(n);
}

void TemporalNN::NewFrame()
{
  int old = m_pointer;
  m_pointer++;
  if (m_pointer >= m_nn.isize())
    m_pointer = 0;

  m_nn[m_pointer] = m_nn[old];
}

double TemporalNN::Compare(const TemporalNN & t)
{
  int i;
  double dist = 0.;
  double p1 = m_pointer;
  double p2 = t.Pointer();
  for (i=0; i<m_nn.isize(); i++) {
    dist += m_nn[p1].GetDistance(t[p2]);
    p1--;
    if (p1 < 0)
      p1 = m_nn.isize()-1;
    p2--;
    if (p2 < 0)
      p2 = t.isize()-1;
  }
  return dist;
}

void TemporalNN::Train(const TemporalNN & t, int iter)
{
  int i;
 
  double p1 = m_pointer;
  double p2 = t.Pointer();
  for (i=0; i<m_nn.isize(); i++) {
    m_nn[p1].Train(t[p2], iter);
    p1--;
    if (p1 < 0)
      p1 = m_nn.isize()-1;
    p2--;
    if (p2 < 0)
      p2 = t.isize()-1;
  }
 
}

void TemporalNN::ReOrg()
{
  TemporalNN tmp = *this;
  double p1 = m_pointer;
  for (int i=0; i<m_nn.isize(); i++) {
    tmp[i] = m_nn[p1];
      p1++;
    if (p1 >= m_nn.isize())
      p1 = 0;
  }
  *this = tmp;
}

//==============================================
void TemporalNNCluster::SetSize(int neurons, int depth, int clusters)
{
  m_cluster.resize(clusters);
  for (int i=0; i<clusters; i++)
    m_cluster[i].SetSize(neurons, depth);
}

void TemporalNNCluster::ReOrg()
{
  for (int i=0; i<m_cluster.isize(); i++)
    m_cluster[i].ReOrg();
}

void TemporalNNCluster::Train(const TemporalNN & t, int iter)
{
  double dist = -1.;
  int i;
  int index = -1;
  for (i=0; i<m_cluster.isize(); i++) {
    double d = m_cluster[i].Compare(t);
    if (index < 0 || d < dist) {
      dist = d;
      index = i;
    }
  }
  m_cluster[i].Train(t, iter);
}
