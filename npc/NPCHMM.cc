#include "npc/NPCHMM.h"
#include "npc/SkeletonControl.h"


void NPCHMM::Update(const svec<double> & dist)
{
  int i;

  for (i=0; i<m_states.isize(); i++) {
    if (i == m_lastMin)
      continue;
    if (m_trans < m_states[i].Score()) {
      m_states[i].Score() = m_trans;
      m_states[i].AddHist(m_lastMin);
    } else {
      m_states[i].AddHist(i);
    }    
  }
  
  for (i=0; i<m_states.isize(); i++) {
    m_states[i].Score() += dist[i] + m_stay; 
  }  

  // Subtract min score
  double min = m_states[0].Score();
  for (i=0; i<m_states.isize(); i++) {
    if (m_states[i].Score() < min) {
      min = m_states[i].Score();
      m_lastMin = i;
    }
  }
  for (i=0; i<m_states.isize(); i++) {
    m_states[i] -= min;
  }
}



void NPCControl::SetSuccess(const SuccessFeature & f)
{
}

void NPCControl::Retrieve(svec<double> & out, const svec<double> & in_raw, double success)
{
  //cout << "Retrieve" << endl;
  svec<double> intime = in_raw;
  intime.push_back(success);
  bool b = m_time.IsNew(intime);
  //cout << "Retrieve 0.5" << endl;

  int i, j;
  int k = 0;

  svec<double> in = in_raw;

  out.resize(m_controls.isize()*m_out, 0.);
  //cout << "Out " << out.isize() << endl;
  svec<double> suggest;
  //suggest.resize(

  int x;
  cout << "Input ";
  for (x=0; x<in_raw.isize(); x++) {
    cout << " " << in_raw[x];
  }
  cout << endl;
  
  
  for (i=0; i<m_controls.isize(); i++) {
    svec<double> tmp;
    tmp.resize(m_out, 0.);
    in = in_raw;
    m_controls[i].Retrieve(tmp, in, suggest, 0);
    //cout << "Tmp " << tmp.isize() << endl;
    cout << "Output ";
    for (x=0; x<in.isize(); x++) {
      cout << " " << in[x];
    }
    cout << endl;
      
    for (j=0; j<tmp.isize(); j++) {
      out[k] = tmp[j];
      k++;
    }
    
  }
  //cout << "Retrieve 2" << endl;
  for (i=1; i<m_success.isize(); i++)
    m_success[i-1] = m_success[i];
  m_success[m_success.isize()-1] = success;

  m_frame++;
  //cout << "Retrieve 3, frame=" << m_frame << endl;
  Print();

}

void NPCControl::LearnOrAvoid()
{
  //cout << "LearnOrAvoid" << endl;
  const svec<int> & tb = m_time.TraceBack();
  int last = tb[tb.isize()-1];
  int lastFrame = tb.isize()-1;

  if (last == -1)
    return;
  
  int n = 0;
  double s = 0.;
  int i;

 
  for (int j=0; j<m_controls.isize(); j++) {
    for (i = tb.isize()-3; i>=0; i--) {
      if (tb[i] < 0.)
	break;
      //int from = m_frame + i + 1 - tb.isize();
      //int to = m_frame + lastFrame - tb.isize();
      int from = m_frame + i + 1 - tb.isize();
      int to = from;
      int idx = i+2;
      //if (m_success[idx] > 0.) {
      cout << "Learn ++++++++++++++++ " << from << " -> " << to << " succ: " << m_success[idx] << endl;
      m_controls[j].Learn(m_success[idx], from, to);
	//} else {
	//cout << "Learn avoid ---------- " << from << " -> " << to << " succ: " << m_success[idx] << endl;
	//m_controls[j].LearnAvoid(m_success[idx], from, to);
	//m_controls[j].AntiLearn(0.5, from, to);
	//}
    }
  }
  
  //cout << "LearnOrAvoid TB " << tb.isize() << " " << m_success.isize() << endl;
  /*
  for (i=tb.isize()-1; i>=0; i--) {
    if (tb[i] == last) {
      cout << "Traceback " << i << " " << tb[i] << " score " << m_success[i] << endl;
      s += m_success[i];
      n++;
      continue;
    }
    cout << "Learn frames " << m_frame + i + 1 - tb.isize() << " " << m_frame + lastFrame - tb.isize() << " frame=" << m_frame << " score: " << s << " frames: " << lastFrame - i - 1 << "  ";
    //if (s > 0) {
    if (s > 0) {
      cout << "++++++ " << endl;
      for (int j=0; j<m_controls.isize(); j++) {
	//cout << "LearnOrAvoid/Smart " << j << endl;

	//m_controls[j].LearnSmart(1., m_frame + i + 1 - tb.isize(), m_frame + lastFrame - tb.isize(), s);
 	m_controls[j].Learn(1., m_frame + i + 1 - tb.isize(), m_frame + lastFrame - tb.isize());
      }
    } else {
      cout << "------ " << endl;

      for (int j=0; j<m_controls.isize(); j++) {
	//cout << "LearnOrAvoid/Smart " << j << endl;

	//m_controls[j].LearnSmart(1., m_frame + i + 1 - tb.isize(), m_frame + lastFrame - tb.isize(), s);
 	m_controls[j].LearnAvoid(1., m_frame + i + 1 - tb.isize(), m_frame + lastFrame - tb.isize());
 	m_controls[j].AntiLearn(1., m_frame + i + 1 - tb.isize(), m_frame + lastFrame - tb.isize());
      }
      }
    
    if (tb[i] == -1)
      break;
    s = m_success[i];
    lastFrame = i;
    }*/
  //cout << "LearnOrAvoid done" << endl;
}

void NPCControl::UnSuccess()
{
}
