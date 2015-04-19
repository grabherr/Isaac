#define FORCE_DEBUG

#include "core/Memory.h"
#include "base/RandomStuff.h"

Memory::Memory() 
{
  m_featSizeIn = 6; // Hard coded
  m_featSizeOut = 3;
  // Pain, reward, unsuccess
  SetNumNeurons(100, m_featSizeIn);
}

void Memory::SetNumNeurons(int n, int featSize)
{
  m_featSizeIn = featSize;
  OneDimSOMOrganizer one;

  int somSize = n;
  int k = m_featSizeIn + m_featSizeOut;
  one.SetDimRange(k, 0., 0.05);

  cout << "Organizing SOM nodes: size=" << somSize << " feat=" << featSize << " total=" << k << endl;
  m_act.Organize(one, somSize);

}

void Memory::LearnAct(const svec<double> & in, const Reaction & react)
{
  svec<int> valid;
  valid.resize(in.isize(), 1);
  LearnAct(in, valid, react);
}

void Memory::LearnAct(const svec<double> & in, const svec<int> & valid, const Reaction & react)
{
  int i;
  svec<double> feat;
  feat.resize(m_featSizeIn + m_featSizeOut, 0.);
  for (i=0; i<in.isize(); i++)
    feat[i] = in[i];

  feat[m_featSizeIn] = react.Pain();
  feat[m_featSizeIn+1] = react.Reward();
  feat[m_featSizeIn+2] = react.Unsuccess();
  if (react.Unsuccess() > 0.5) {
    cout << "INTERNAL: Unsuccess." << endl;
  }


  svec<int> valid_b = valid;
  valid_b.resize(valid.isize() + m_featSizeOut, 1);
  m_act.Train(feat, valid_b);
}

void Memory::Print() const
{
  for (int i=0; i<m_act.GetNodeCount(); i++) {
    cout << "Neuron " << i << endl; 
    const SOMNode & node = m_act.GetNodeConst(i);
    const svec<double> & cc = node.GetCoords();
    for (int j=0; j<cc.isize(); j++)
      cout << cc[j] << " ";
    cout << endl;
  }

}

double Memory::RetrieveAct(Reaction & react, const svec<double> & in)
{
  int i;
  svec<double> feat;

  //cout << "total: " << m_featSizeIn + m_featSizeOut 

  feat.resize(m_featSizeIn + m_featSizeOut, 0.);
 
  for (i=0; i<in.isize(); i++)
    feat[i] = in[i];


  svec<int> valid;
  valid.resize(feat.isize(), 1);
  feat[m_featSizeIn] = 0;
  feat[m_featSizeIn+1] = 0;
  feat[m_featSizeIn+2] = 0;
 
  //cout << "Get BestIndex: " << endl;
  //for (i=0; i<feat.isize(); i++)
  //cout << "Feature " << i << " = " << feat[i] << endl;
  int index = m_act.BestMatch(feat, valid);
  //cout << "BestIndex: " << index << endl;
  SOMNode & node = m_act.GetNode(index);
  const svec<double> & cc = node.GetCoords();

  react.SetPain(cc[m_featSizeIn]);
  react.SetReward(cc[m_featSizeIn+1]);
  react.SetUnsuccess(cc[m_featSizeIn+2]);

  return m_act.GetBestDistance();
}

bool Memory::DensestVector(svec<double> & out) 
{
  int index = m_act.DensestNode();
  if (index < 0)
    return false;
  const SOMNode & node = m_act.GetNodeConst(index);
  const svec<double> & cc = node.GetCoords();
  out = cc;
  return true;
}


void Memory::LearnFromMemory(const Memory & mem, bool forget)
{
  const SelfOrgFeatMap & som = mem.m_act;
  
  int i, j;
  int n = 4;
  svec<int> valid;

  if (forget)
    m_act.SetCounter(0);

  for (i=0; i<4*som.GetNodeCount(); i++) {
    int index = RandomInt(som.GetNodeCount());
    const SOMNode & node = som.GetNodeConst(index);
    svec<double> cc = node.GetCoords();
    //cout << "Size " << cc.isize() << endl;
    valid.resize(cc.isize(), 1);
    m_act.Train(cc, valid);
  }

  if (forget)
    m_act.SetCounter(0);

}
