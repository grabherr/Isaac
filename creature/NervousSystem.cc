#define FORCE_DEBUG

#include "creature/NervousSystem.h"


double LaplaceDist(double a, double b)
{
  double c = a - b;
  if (c < 0.)
    c = -c;
  return c;
}

double CurrDist(double a, double b)
{
  return LaplaceDist(a, b);
}

void SingleLayerSOM::Setup(int dataSize) 
{
  int i, j;
  int n = 1;
  for (i=0; i<m_dim.isize(); i++) {
    n *= m_dim[i];
  }
  m_neurons.resize(n);
  
  svec<int> tmp;
  tmp.resize(m_dim.isize(), 0);
  for (i=0; i<n; i++) {
    m_neurons[i].Setup(dataSize, m_dim.isize());
    for (j=0; j<tmp.isize(); j++) {
      m_neurons[i].SetCoord(j, tmp[j]);
    }
    tmp[0]++;
    for (j=0; j<tmp.isize()-1; j++) {
      if (tmp[j] >= m_dim[j]) {
	tmp[j+1]++;
	tmp[j] = 0;
      }
    }
  }
  m_me.Setup(dataSize, m_dim.isize());
}

void SingleLayerSOM::Update(const SerialImpulse & imp, double distance) 
{
  double dist;
  int index = BestSingle(dist, imp);
  for (int i=0; i<m_neurons.isize(); i++) {
    double dd = m_neurons[i].CoordDistance(m_neurons[index], m_max) + distance;
    m_neurons[i].Update(imp, dd);
  }
}
 
double SingleLayerSOM::Best(Neuron & out, const SerialImpulse & imp) const
{
  double dist;
  int index = BestSingle(dist, imp);
  out = m_neurons[index];
  return dist;
}

double SingleLayerSOM::Best(Neuron & out, const Neuron & n) const
{
  double bestDist = m_neurons[0].Distance(n);
  int index = 0;
  for (int i=1; i<m_neurons.isize(); i++) {
    double d = m_neurons[i].Distance(n);
    if (d < bestDist) {
      bestDist = d;
      index = i;
    }
  }
  out = m_neurons[index];
  return bestDist;
}

void SingleLayerSOM::Update(const Neuron & n, bool bUseCoords)
{
  int i;
  if (bUseCoords) {
    cout << "WARNING: Override bUseCoords" << endl;
    bUseCoords = false;
  }
  if (bUseCoords) {
    //cout << "Use coords" << endl;
    for (i=0; i<m_neurons.isize(); i++) {
      double dd = m_neurons[i].CoordDistance(n, m_max);
      
      m_neurons[i].Update(n, dd);
    }
  } else {
    //cout << "No coords" << endl;
    double bestDist = m_neurons[0].Distance(n);
    int index = 0;
    for (i=1; i<m_neurons.isize(); i++) {
      double d = m_neurons[i].Distance(n);
      if (d < bestDist) {
	bestDist = d;
	index = i;
      }
    }
    for (i=0; i<m_neurons.isize(); i++) {
      double dd = m_neurons[i].CoordDistance(m_neurons[index], m_max);      
      m_neurons[i].Update(n, dd);
    }
    
  }
 
}

void SingleLayerSOM::Update(const SingleLayerSOM & n, NCMP bRotate, double distance) 
{
  double dist;
  int i, j;
  int shift = BestMultShift(dist, n, bRotate);
  for (j=0; j<m_neurons.isize(); j++) {
    int k = (j+shift) % m_neurons.isize();
    
    for (i=0; i<m_neurons.isize(); i++) {
      double dd = m_neurons[i].CoordDistance(m_neurons[j], m_max) + distance;
      m_neurons[i].Update(n[k], dd);
    }
  }
  m_me.Update(n.Me(), 0.);
}


int SingleLayerSOM::BestSingle(double & dist, const SerialImpulse & imp) const
{
  double bestDist = m_neurons[0].Distance(imp);
  int index = 0;
  for (int i=1; i<m_neurons.isize(); i++) {
    double d = m_neurons[i].Distance(imp);
    if (d < bestDist) {
      bestDist = d;
      index = i;
    }
  }
  dist = bestDist;
  return index;
}

int SingleLayerSOM::BestMultShift(double & dist, const SingleLayerSOM & n, NCMP bRotate) const
{
  double bestDist = -1.;
  int index = -1;
  int i, j;
  
  if (bRotate == NCMP_LIN || bRotate == NCMP_ROT) {
    //cout << "NOT" << endl;
    int nn = m_neurons.isize();
    if (bRotate == NCMP_LIN)
      nn = 1;
    for (i=0; i<nn; i++) {
      double sum = m_me.Distance(n.Me());
      for (j=0; j<m_neurons.isize(); j++) {
	int k = (j+i) % m_neurons.isize();
	double d = m_neurons[j].Distance(n[k]);
	if (d < bestDist || bestDist < 0.) {
	  bestDist = d;
	  index = i;
	}
      }
    }
  } 
  if (bRotate == NCMP_FUZ) {
    //cout << "FUZZY" << endl;
    index = 0;
    double sum = m_me.Distance(n.Me());
    for (i=0; i<m_neurons.isize(); i++) {
      Neuron dummy;
      sum += n.Best(dummy, m_neurons[i]);
    }    
  }
  return index;
}

//----------------------------------------------
void MultiLayerSOM::Train(const MultiLayerSOM & s)
{
  cout << "Associate " << endl;
  int index = Associate(s, 0);
  int i, j;
  cout << "Index: " << index << endl;
  //for (i=index; i<s.isize(); i++) {
  //  m_soms[i].Update(s[i-index], m_bRotate);
  //}
  for (j=0; j<s.isize(); j++) {
    for (i=0; i<m_soms.isize(); i++) {
      double dist = CurrDist(index + j, i)/m_weight;
      m_soms[i].Update(s[j], m_bRotate, dist);
    }
  }
}

void MultiLayerSOM::Bubble(const SingleLayerSOM & s)
{
  int i;
  for (i=0; i<m_soms.isize()-1; i++) {
    m_soms[i].Update(m_soms[i+1], m_bRotate, 0.);
    m_soms[i].Me().Update(m_soms[i+1].Me(), 0.);
  }
  m_soms[m_soms.isize()-1].Update(s, m_bRotate, 0.);
  m_soms[m_soms.isize()-1].Me().Update(s.Me(), 0.);
}
 
void MultiLayerSOM::Train(const SingleLayerSOM & n, int index)
{
  int i;
  if (index == -1) {
    double minDist = 999999999.;
    for (i=0; i<m_soms.isize(); i++) {
      double dist;
      m_soms[i].BestMultShift(dist, n, m_bRotate);
      if (dist < minDist) {
	minDist = dist;
	index = i;
      }
    }
  }
  for (i=0; i<m_soms.isize(); i++) {
    double dist = CurrDist(i, index)/m_weight;
    m_soms[i].Update(n, m_bRotate, dist);
  }
  IncDecay();
}

int MultiLayerSOM::Retrieve(Neuron & out, const Neuron & n, int index) const
{
  int i, j;
  if (index == -1) {
    double minDist = 999999999.;
    for (i=0; i<m_soms.isize(); i++) {
      Neuron tmp;
      double d = m_soms[i].Best(tmp, n);
      if (d < minDist) {
	minDist = d;
	index = i;
	out = tmp;
      }
    }
  } else {
    m_soms[index].Best(out, n);    
  }
  return index;
}

void MultiLayerSOM::Train(const Neuron & n, bool bUseCoords, int index)
{

  int i, j;
  if (index == -1) {
    Neuron tmp;
    index = Retrieve(tmp, n);
  }
  m_soms[index].Update(n, bUseCoords);
  IncDecay();
}

int MultiLayerSOM::Associate(const MultiLayerSOM & n, int shift) const
{
  int i, j;
  int index = -1;

  double minDist = 999999999.;
  for (i=0; i<=m_soms.isize()-n.isize()-shift; i++) {
    double dist = 0.;
    for (j=i; j<n.isize(); j++) {
      double dd;
      m_soms[j].BestMultShift(dd, n[j-i], m_bRotate);
      dist += dd;
    }
    if (dist < minDist) {
      minDist = dist;
      index = i;
    }
  }
  return index;
}


void MultiLayerSOM::RetrieveList(svec<Candidates> & out, const MultiLayerSOM & n, int shift) const
{
  out.clear();
  int i, j;
  
  for (i=0; i<=m_soms.isize()-n.isize()-shift; i++) {
    //cout << "i=" << i << endl; 
    double dist = 0.;
    for (j=i; j<n.isize(); j++) {
      //cout << "j=" << j << endl; 
      double dd;
      m_soms[j].BestMultShift(dd, n[j-i], m_bRotate);
      dist += dd;
    }
    Candidates cc;
    cc.Set(i+shift, dist);
    out.push_back(cc);
  }
  Sort(out);
}



void MultiLayerSOM::Retrieve(MultiLayerSOM & out, const MultiLayerSOM & s, int shift) const
{
  int index = Associate(s, shift);
  for (int i=0; i<s.isize()+shift; i++) {
    out.AddSOM(m_soms[index+i]);
  }
}

//==========================================
void MultiLayerSOMBuildSTM::Construct(MultiLayerSOM & som, int dataSize)
{
  int depth = 5;
  int dim = 3;
  int i, j;
  for (i=0; i<depth; i++) {
    SingleLayerSOM s;
    s.SetDim(dim);
    for (j=0; j<dim; j++)
      s.SetSize(j, 4);
    s.Setup(dataSize);
    s.SetWeight(1.);
    som.AddSOM(s);
  }
  som.SetDecay(1.);
}

//==========================================
void MultiLayerSOMBuildLTM::Construct(MultiLayerSOM & som, int dataSize)
{
  int depth = 25;
  int dim = 3;
  int i, j;
  som.SetRotate(m_bRot);
  for (i=0; i<depth; i++) {
    SingleLayerSOM s;
    s.SetDim(dim);   
    for (j=0; j<dim; j++)
      s.SetSize(j, 4);
    s.Setup(dataSize);
    s.SetWeight(0.9);
    som.AddSOM(s);
  }
  som.SetDecay(1.);
}

void MultiLayerSOMBuildConcept::Construct(MultiLayerSOM & som, int dataSize)
{
  int depth = 1;
  int dim = 5;
  int i, j;
  for (i=0; i<depth; i++) {
    SingleLayerSOM s;
    s.SetDim(dim);
    for (j=0; j<dim; j++)
      s.SetSize(j, 4);
    s.Setup(dataSize);
    s.SetWeight(0.9);
    som.AddSOM(s);
  }
  som.SetDecay(0.9);
}


//==========================================================
 

int SearchStack::Pop(Candidates & out)
{
  m_pointer--;
  if (m_pointer >= 0)
    out = m_stack[m_pointer];

  return m_pointer;
}

void SearchStack::Push(const Candidates & in, int last)
{
  m_pointer++;
  if (m_pointer >= m_stack.isize()) {
    m_stack.resize(m_pointer + 4096);
    m_score.resize(m_pointer + 4096, 0.);
    m_last.resize(m_pointer + 4096, -1);
  }
  
  m_stack[m_pointer] = in;
  double lastScore = 0;
  if (last != -1)
    lastScore = m_score[last];
  lastScore += in.Dist();
  m_score[m_pointer] = lastScore;
  m_last[m_pointer] = last;  
}

 

//==========================================================
void MultiLayerSOMSearch::CandsToMSOM(MultiLayerSOM & out, 
				      const MultiLayerSOM & mem, 
				      const svec<Candidates> & in) const
{
  out.clear();
  for (int i=in.isize()-1; i>=0; i--) {
    out.AddSOM(mem[in[i].Index()]);
  }
  
}


bool MultiLayerSOMSearch::Search(svec<Candidates> & out, 
				 const MultiLayerSOM & mem, 
				 const MultiLayerSOM & start)
{
  out.clear();
  int i, j;

  SearchStack stack;
  SearchStack best;
  svec<int> used;
  used.resize(mem.isize(), 0);

 
  MultiLayerSOM work;
  work = start;
  int index = -1;
  int maxCand = 4;

  j = 0;
  
  int len = start.isize();

  double minScore = 999999999999.;

  do {
    svec<Candidates> cands;
    cout << "Retrieve" << endl;
    mem.RetrieveList(cands, work, m_ahead);

    int curr = stack.Current();
    if (cands.isize() < maxCand)
      maxCand = cands.isize();
    if (j < m_maxDepth) {
      for (i=0; i<maxCand; i++) {
	cout << "Push" << endl;
	stack.Push(cands[i], curr);
      }
      j++;
    }
    Candidates wc;
    index = stack.Pop(wc);
    if (index < 0)
      break;
    cout << "Pop" << endl;
    work.clear();
    for (int k=wc.Index(); k<wc.Index()+len; k++) {
      work.AddSOM(mem[k]);
    }


    if (index != -1) {
      svec<Candidates> goal;
      work.RetrieveList(goal, m_goal, 0);
      double s = goal[0].Dist();
      double score = s + stack.CurrScore()/(double)m_maxDepth;
      if (score < minScore) {
	cout << "Got best" << endl;
	minScore = score;
	best = stack;
	best.Push(wc, best.Current());
      }
    }

  } while (index >= 0);

  cout << "Traceback" << endl;
  // Traceback
  int theLast = best.Current();
  int back = -1;
  //out.push_back(stack.Direct(back, theLast));
  cout << "Best " << theLast << endl;
  while (theLast != -1) {
    cout << "Add " << endl;
    const Candidates & cc = stack.Direct(back, theLast);
    theLast = back;      
    out.push_back(cc);
  }
  cout << "Out size " << out.isize() << endl;

  return true;
}


