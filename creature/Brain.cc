#include "creature/Brain.h"


void Brain::Init(int dataSize)
{
  m_dataSize = dataSize;

  MultiLayerSOMBuildSTM st;
  MultiLayerSOMBuildLTM ltr(NCMP_FUZ);
  MultiLayerSOMBuildLTM ltnr(NCMP_FUZ);
  MultiLayerSOMBuildConcept concept;

  st.Construct(m_shortTerm, dataSize);
  ltnr.Construct(m_longTermNoRot, dataSize);
  ltr.Construct(m_longTermRot, dataSize);
  concept.Construct(m_concept, dataSize);

  m_curr.SetDim(m_shortTerm[0].CoordNum());
  for (int i=0; i<m_shortTerm[0].CoordNum(); i++)
    m_curr.SetSize(i, m_shortTerm[0].GetCoordSizeI(i));
  
  m_curr.Setup(m_dataSize);
  m_curr.SetWeight(1.);

  m_goal = m_curr;
  

  
}

void Brain::SetGoalExplicit(const MemoryUnit & in)
{
  Feed(in, m_goal, false);
}

void Brain::StartFeed()
{
}

void Brain::Feed(const MemoryUnit & in)
{
  Feed(in, m_curr);
}

void Brain::FeedMe(const MemoryUnit & in)
{
  if (m_template.isize() == 0)
    m_template = in;
  Neuron n;
  n.Setup(in.isize(), 0);
  for (int i=0; i<in.isize(); i++)
    n[i] = in[i];

  m_curr.Me().Update(n, 0.);
}

void Brain::Feed(const MemoryUnit & in, SingleLayerSOM & curr, bool bAssoc)
{
  
  if (m_template.isize() == 0)
    m_template = in;

  Neuron n;
  n.Setup(m_dataSize, m_shortTerm[0].CoordNum());
  SphereCoordinates cc = in.GetCoordinates();

  //cout << "IN " << cc.r() << " " << cc.phi() << " "  << cc.theta() << endl;

  double inv_r = m_shortTerm[0].GetCoordSize(0)/(cc.r()+1);
  double theta = m_shortTerm[0].GetCoordSize(2)/2 + m_shortTerm[0].GetCoordSize(2)*(cc.theta()-PI_P/2)/PI_P;
  double phi = cc.phi();
  if (phi > PI_P/2)
    phi -= PI_P;
  phi = m_shortTerm[0].GetCoordSize(1)/2 + phi*m_shortTerm[0].GetCoordSize(1)/PI_P;

  // Set up spatial relation
  n.SetCoord(0, inv_r);
  n.SetCoord(1, phi);
  n.SetCoord(2, theta);
  
  cout << "IN" << endl;
  in.Print();
  cout << "SET " << inv_r << " " << phi << " "  << theta << endl;

  for (int i=0; i<in.isize(); i++)
    n[i] = in[i];
  //n.Print();

  curr.Update(n, false);
  
  cout << "Before assoc, n.isize()=" << n.isize() << endl;
  
  // Conceptual memory
  if (bAssoc) {
    Neuron assoc;
    cout << "1" << endl;
    m_concept.Train(n, false);
    cout << "2" << endl;
    m_concept.Retrieve(assoc, n);
    // Make configurable...
    cout << "3" << endl;
    assoc.SetWeight(0.4);
    cout << "4" << endl;
    curr.Update(assoc, false);
  }
  cout << "done" << endl;
}

void Brain::DoneFeed()
{
  int i, j;
  cout << "Bubble" << endl;
  m_shortTerm.Bubble(m_curr);

  cout << "Set goal" << endl;
  MultiLayerSOMSearch search;
  search.SetGoal(m_goal);
  MultiLayerSOM outN;
  cout << "Search" << endl;
  search.Search(outN, m_longTermNoRot, m_shortTerm);
  //MultiLayerSOM outR;
  //Search(outR, m_longTermRot, m_shortTerm);
  
  cout << "Train" << endl;
  m_longTermNoRot.Train(m_shortTerm);
  m_longTermRot.Train(m_shortTerm);
  
  // TODO: reconcile memory types...
  
  int off = m_shortTerm.isize()-1;
  
  m_response.clear();

  cout << "Collect response" << endl;

  for (i=0; i<outN.isize(); i++) {
    const Neuron & me = outN[i].Me();
    cout << "PREDICT" << endl;
    for (j=0; j<outN[i].isize(); j++) {
      cout << "Neuron " << j << endl;
      SLSOM2MU(m_template, (outN[i])[j]);
      m_template.Print();
    }
    cout << "---------------------" << endl;
    //outN[i].Print();
    //cout << "++++++++++ ME, size " << me.isize() << endl;
    //for (j=0; j<me.isize(); j++)
    //m_template[j] = me[j];
    SLSOM2MU(m_template, me);
    cout << "Temp size " << m_template.isize() << endl;
    m_response.push_back(m_template);
  }
  //m_curr.Me().Update(n, 0.);
}

void Brain::SLSOM2MU(MemoryUnit & temp, const Neuron & curr)
{
  for (int j=0; j<curr.isize(); j++)
    temp[j] = curr[j];  
}

bool Brain::NextAction(MemoryUnit & out)
{
  if (m_response.isize() == 0) {
    cout << "No response" << endl;
    return false;
  }
  out = m_response[0];
  for (int i=1; i<m_response.isize()-1; i++) {
    m_response[i-1] = m_response[i];
  }
  m_response.resize(m_response.isize()-1);
  return true;
}


