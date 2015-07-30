// (c) Manfred G. Grabherr

// This code is licensed under the GNU GENERAL PUBLIC LICENSE:
// http://www.gnu.org/licenses/gpl-3.0.en.html

#ifndef NERVOUSSYSTEM_H
#define NERVOUSSYSTEM_H

#include "base/SVector.h"
#include "base/RandomStuff.h"

#include <math.h>

enum NCMP
  {
    NCMP_LIN,
    NCMP_ROT,
    NCMP_FUZ
  };

//========================================
class SerialImpulse
{
 public:
  SerialImpulse() {}
  virtual ~SerialImpulse() {}

  void resize(int n) {
    m_data.resize(n, 0.);
    m_weight.resize(n, 1.);
  }
  int isize() const {return m_data.isize();}
  double & operator [] (int i) {return m_data[i];}
  const double & operator [] (int i) const {return m_data[i];}

  double Weight(int i) const {return m_weight[i];}
  void SetWeight(int i, double v) {m_weight[i] = v;}
  
  const svec<double> & Get() {return m_data;}

  
 private:
  svec<double> m_data;
  svec<int> m_weight;
};


//========================================
class Neuron
{
 public:
  Neuron() {
    m_weight = 1.;
  }

  void Setup(int dataSize, int coordSize) {
    m_data.resize(dataSize, 0.);
    for (int i=0; i<m_data.isize(); i++) {
      m_data[i] = RandomFloat(.1);
    }
    m_coords.resize(coordSize, 0.);
  }

  int isize() const {return m_data.isize();}
  double & operator [] (int i) {return m_data[i];}
  const double & operator [] (int i) const {return m_data[i];}
  
  int CoordSize() const {return m_coords.isize();}
  double Coord(int i) const {return m_coords[i];}
  void SetCoord(int i, double v) {m_coords[i] = v;}

  void SetDistanceFrom(const Neuron & n) {
    m_coords.resize(n.CoordSize());
    for (int i=0; i<m_coords.isize(); i++)
      m_coords[i] = n.Coord(i);
  }

  double Weight() const {return m_weight;}
  void SetWeight(double v) {m_weight = v;}

  double CoordDistance(const Neuron & n, const svec<double> & coordMax) const {
    double dist = 0;
    int i;
    for (i=0; i<m_coords.isize(); i++) {
      if (m_coords[i] > n.Coord(i)) {
	double a = m_coords[i] - n.Coord(i);
	if (a > coordMax[i]/2.) {
	  a = -(m_coords[i] - n.Coord(i) - coordMax[i]);
	}
	dist += a*a;
      } else {
	double a = - m_coords[i] + n.Coord(i);
	if (a > coordMax[i]/2.) {
	  a = -(-m_coords[i] + n.Coord(i) - coordMax[i]);
	}
	dist += a*a;
      }
    }
    return sqrt(dist);
  }

  void Update(const Neuron & n, double distance) {
    double w = 1/(distance*distance + 1.) * m_weight * n.m_weight;
    for (int i=0; i<m_data.isize(); i++) {
      m_data[i] = m_data[i]*(1.-w) + n[i]*w;
    }
  }

  void Update(const SerialImpulse & imp, double distance, int from = 0) {
    double w = 1/(distance*distance + 1.) * m_weight;
    for (int i=0; i<m_data.isize(); i++) {
      double ww = w * imp.Weight(i+from);
      m_data[i] = m_data[i]*(1.-ww) + imp[i+from]*ww;
    }
  }

  double Distance(const SerialImpulse & imp) const {
    double sum = 0.;
    for (int i=0; i<m_data.isize(); i++) {
      sum += (m_data[i]-imp[i])*(m_data[i]-imp[i]);
    }
    return sqrt(sum);
  }

  double Distance(const Neuron & n) const {
    double sum = 0.;
    for (int i=0; i<m_data.isize(); i++) {
      sum += (m_data[i]-n[i])*(m_data[i]-n[i]);
    }
    return sqrt(sum);
  }

  void Serialize(SerialImpulse & imp, bool bAppend = false) const {
    int from = 0;
    if (bAppend) {
      from = imp.isize();
      imp.resize(imp.isize() + m_data.isize());      
    } else {
     imp.resize(m_data.isize());      
    }
    for (int i=0; i<m_data.isize(); i++) {
      imp[from + i] = m_data[i];
    }
  }

  void Initialize(const SerialImpulse & imp, int from = 0) {
    for (int i=0; i<m_data.isize(); i++) {
      m_data[i] = imp[from + i];
    }
  }

  void Print() const {
    cout << "NEURON" << endl;
    cout << "Weight: " << m_weight << endl;
    int i;
    cout << "Coordinates:";
    for (i=0; i<m_coords.isize(); i++)
      cout << " " << m_coords[i];
    cout << endl;
    for (i=0; i<m_data.isize(); i++)
      cout << m_data[i] << endl;
  }


 private:
  svec<double> m_data;
  svec<double> m_coords;
  double m_weight;
};

//========================================
class SingleLayerSOM
{
 public:
  SingleLayerSOM() {}
  void SetDim(int dim) {
    m_dim.resize(dim, 0);
    m_max.resize(dim, 0.);
  }

  void SetSize(int i, int size) {
    m_dim[i] = size;
    m_max[i] = size;
  }

  int CoordNum() const {return m_max.isize();}
  double GetCoordSize(int i) const {return m_max[i];}
  int GetCoordSizeI(int i) const {return m_dim[i];}
  
  int isize() const {return m_neurons.isize();}
  const Neuron & operator[] (int i) const {return m_neurons[i];}
  Neuron & operator[] (int i) {return m_neurons[i];}
  const Neuron & Me() const {return m_me;}
  Neuron & Me() {return m_me;}

  // Call this to set up the structure!!
  void Setup(int dataSize);

  void SetWeight(double w) {
    for (int i=0; i<m_neurons.isize(); i++)
      m_neurons[i].SetWeight(w);
    m_me.SetWeight(w);
  }
  
  void Serialize(SerialImpulse & imp) const {   
    for (int i=0; i<m_neurons.isize(); i++) {
      m_neurons[i].Serialize(imp, true);
    }
    m_me.Serialize(imp, true);
  }

  void Initialize(const SerialImpulse & imp) {   
    int i;
    for (i=0; i<m_neurons.isize(); i++) {
      m_neurons[i].Initialize(imp, i*m_neurons[i].isize());
    }
    m_me.Initialize(imp, i*m_neurons[i].isize());
  }

  // Note: the size of imp must match the size of the neuron!
  void Update(const SerialImpulse & imp, double distance = 0.);
  void Update(const SingleLayerSOM & s, NCMP bRotate, double distance);
  void Update(const Neuron & n, bool bUseCoords = true);

  double Best(Neuron & out, const SerialImpulse & imp) const;
  double Best(Neuron & out, const Neuron & n) const;
 
  double Distance(const SerialImpulse & imp) const {
    double dist;
    BestSingle(dist, imp);
    return dist;
  }

  double Distance(const SingleLayerSOM & s, NCMP bRotate) const {
    double dist;
    BestMultShift(dist, s, bRotate);
    return dist;
  }

  int BestMultShift(double & dist, const SingleLayerSOM & n, NCMP bRotate) const;

  void Print() const {
    cout << "SINGLELAYER SOM" << endl;
    int i;
    cout << "Dimensions:";
    for (i=0; i<m_dim.isize(); i++)
      cout << " " << m_dim[i];
    cout << endl;
    for (i=0; i<m_neurons.isize(); i++)
      m_neurons[i].Print();
    cout << "-------------------" << endl;
  }

  //--------------------------------------
 private:
  int BestSingle(double & dist, const SerialImpulse & imp) const;

 
  svec<Neuron> m_neurons;
  Neuron m_me;
  svec<int> m_dim;
  svec<double> m_max;
};

//----------------------------------------
class Candidates
{
 public:
  Candidates() {
    m_index = -1;
    m_dist = 0.;
  }

  int Index() const {return m_index;}
  double Dist() const {return m_dist;}

  void Set(int index, double dist) {
    m_index = index;
    m_dist = dist;
  }

  bool operator < (const Candidates & s) const {
    return m_dist < s.m_dist;
  }

 private:
  int m_index;
  double m_dist;
};


//========================================
class MultiLayerSOM
{
 public:
  MultiLayerSOM() {
    m_decay = 0.;
    m_bRotate = NCMP_FUZ;
    m_weight = 0.3;
  }
  
  void AddSOM(const SingleLayerSOM & som) {
    m_soms.push_back(som);
  }
  
  void SetWeight(double d) {
    for (int i=0; i<m_soms.isize(); i++)
      m_soms[i].SetWeight(d);
  }

  void SetRotate(NCMP b) {m_bRotate = b;}

  int isize() const {return m_soms.isize();}
  const SingleLayerSOM & operator [] (int i) const {return m_soms[i];}
  SingleLayerSOM & operator [] (int i) {return m_soms[i];}
  void clear() {m_soms.clear();}

  int GetSingleSize() const {return m_soms[0].isize();}

  void SetDecay(double d) {m_decay = d;}

  void Train(const MultiLayerSOM & s);
  // Specify the index
  void Train(const SingleLayerSOM & s, int index = -1);
  void Train(const Neuron & n, bool bUseCoords=true, int index = -1);

  // Bubbles the info through the chain of single SOMs (for e.g. STM)
  void Bubble(const SingleLayerSOM & s);
 
  // The parameter plus specifies the # of layers into the future
  void Retrieve(MultiLayerSOM & out, const MultiLayerSOM & s, int plus = 0) const;
  int Retrieve(Neuron & out, const Neuron & n, int index = -1) const;

  void RetrieveList(svec<Candidates> & out, const MultiLayerSOM & s, int plus = 0) const;

  void Print() const {
    cout << "MULTILAYER SOM" << endl;
    int i;
    switch (m_bRotate) {
    case NCMP_LIN:
	cout << "Mode = linear" << endl;
	break;
    case NCMP_ROT:
      cout << "Mode = rotate" << endl;
      break;
    case NCMP_FUZ:
      cout << "Mode = fuzzy" << endl;
      break;
    }
      
    cout << "Decay: " << m_decay << endl;
    for (i=0; i<m_soms.isize(); i++)
      m_soms[i].Print();
    cout << "==================" << endl;
  }
 private:
  void IncDecay() {
    m_decay *= m_decay;
    for (int i=0; i<m_soms.isize(); i++)
      m_soms[i].SetWeight(m_decay);
  }

  int Associate(const MultiLayerSOM & s, int shift = 0) const;
  
  svec<SingleLayerSOM> m_soms;
  double m_decay;
  NCMP m_bRotate;
  double m_weight;
};


//========================================
class IMultiLayerSOMBuilder
{
 public:
  IMultiLayerSOMBuilder() {}
  virtual ~IMultiLayerSOMBuilder() {}
  virtual void Construct(MultiLayerSOM & som, int dataSize) = 0;
};

class MultiLayerSOMBuildSTM : public IMultiLayerSOMBuilder
{
 public:
  MultiLayerSOMBuildSTM() {}
  virtual void Construct(MultiLayerSOM & som, int dataSize);

};

class MultiLayerSOMBuildLTM : public IMultiLayerSOMBuilder
{
 public:
  MultiLayerSOMBuildLTM(NCMP bRot) {
    m_bRot = bRot;
  }
  virtual void Construct(MultiLayerSOM & som, int dataSize);
 private:
  NCMP m_bRot;
};

class MultiLayerSOMBuildConcept : public IMultiLayerSOMBuilder
{
 public:
  MultiLayerSOMBuildConcept() {}
  virtual void Construct(MultiLayerSOM & som, int dataSize);

};

//========================================
class SearchStack
{
 public:
  SearchStack() {
    m_pointer = -1;
  }

  int Pop(Candidates & out);
  void Push(const Candidates & in, int last);
  int Current() const {return m_pointer;}
  double CurrScore() const {return m_score[m_pointer];}
  
  const Candidates & Direct(int & last, int i) const {
    return m_stack[i];
    last = m_last[i];
  }

 private:
  svec<Candidates> m_stack;
  svec<double> m_score;
  svec<int> m_last;
  int m_pointer;
};

//========================================
class MultiLayerSOMSearch
{
 public:
  MultiLayerSOMSearch() {
    m_ahead = 1;
    m_maxDepth = 5;
  }

  void SetAhead(int i) {m_ahead = i;}
  void SetMaxDepth(int i) {m_maxDepth = i;}

  void SetGoal(const MultiLayerSOM & s) {
    m_goal = s;
  }

  void SetGoal(const SingleLayerSOM & s) {
    m_goal.clear();
    m_goal.AddSOM(s);
  }

  bool Search(svec<Candidates> & out, 
	      const MultiLayerSOM & mem, 
	      const MultiLayerSOM & start);

  bool Search(svec<Candidates> & out, 
	      const MultiLayerSOM & mem, 
	      const SingleLayerSOM & start) {
    MultiLayerSOM tmp;
    tmp.AddSOM(start);
    return Search(out, mem, tmp);
  }

 
  bool Search(MultiLayerSOM  & out, 
	      const MultiLayerSOM & mem, 
	      const MultiLayerSOM & start) 
  {
    svec<Candidates> tmp;
    bool b = Search(tmp, mem, start);
    CandsToMSOM(out, mem, tmp);
    return b;
  }

  bool Search(MultiLayerSOM & out, 
	      const MultiLayerSOM & mem, 
	      const SingleLayerSOM & start)  {
    svec<Candidates> tmp;
    bool b = Search(tmp, mem, start);
    CandsToMSOM(out, mem, tmp);
    return b;
  }

 private:
    void CandsToMSOM(MultiLayerSOM & out, const MultiLayerSOM & mem, const svec<Candidates> & in) const;

  MultiLayerSOM m_goal;
  int m_ahead;
  int m_maxDepth;
  
};


#endif //NERVOUSSYSTEM_H
