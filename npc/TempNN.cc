#include "npc/TempNN.h"
#include "base/RandomStuff.h"

void TNN::Setup(int in, int out)
{
  m_in = in;
  m_out = out;
  svec<double> i;
  svec<double> o;
  i.resize(in, 0);
  o.resize(out, 0);

  NPCIO tmp;
  Make(tmp, o, i);
  m_nn.Setup(25, tmp.isize());
  m_nn.ReSetup(-5, 5);
  
  m_success.resize(m_nn.isize(), 0.);

}

void TNN::Feed(svec<double> & out, const svec<double> & in, double lastScore)
{
  int i;
  NPCIO feat;

  out.resize(m_out);
  Make(feat, out, in);
  //m_nn.Print();
  //cout << "Retrieve for " << feat.isize() << endl;
  m_nn.Retrieve(feat);

  //cout << "Got it." << endl;
  m_hist.push_back(feat);
  m_scores.push_back(lastScore);

  int max = 100;
  if (m_hist.isize() >= max) {
    for (i=max/2; i<m_hist.isize(); i++) {
      m_hist[i-max/2] = m_hist[i];
      m_scores[i-max/2] = m_scores[i];
    }
    m_hist.resize(max/2);
    m_scores.resize(max/2);
  }

  if (lastScore > 0.) {
    int howMany = 20;
    for (i=0; i<20; i++) {
      int n = m_hist.isize() - 1 - RandomInt(howMany);
      if (n > 0) {
	m_nn.Learn(m_hist[n], 1.);
	int index = m_nn.Best(m_hist[n]);
	m_success[index] += lastScore;
      }
    }
    cout << "Last score: " << lastScore << endl;
    m_nn.Print();
  }

  
  
  for (i=0; i<m_out; i++)
    out[i] = feat[i];

  // Be more creative here!!!
  if (m_total < 450) {
    if (m_lastOut.isize() == 0)
      m_lastOut.resize(m_out, 0);
    if (RandomFloat(1.) < 0.5) {
      for (i=0; i<m_out; i++)
	m_lastOut[i] = RandomFloat(10.)-5.;
       
    }      
    out = m_lastOut;
  } else {
    cout << "Real data" << endl;   
  }

  m_total++;
}

void TNN::Make(NPCIO & o, const svec<double> & out, const svec<double> & in)
{
  int i, j;
  int n = m_remember*(m_in + m_out);
  o.resize(n);
  int k = 0;
  for (i=0; i<m_out; i++) {
    o[k] = out[i];
    k++;
  }
  for (i=0; i<m_in; i++) {
    o[k] = in[i];
    k++;
  }
  for (i=m_hist.isize()-1; i>=0; i--) {
    for (j=0; j<m_in+m_out; j++) {
      //cout << k << endl;
      o[k] = (m_hist[i])[j];
      k++;
    }
    if (k == n)
      break;
  }
  o.SetAllValid(true);
}
