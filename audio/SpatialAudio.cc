#define FORCE_DEBUG

#include "audio/SpatialAudio.h"

#include "base/RandomStuff.h"

void AudioReceiver::Write(char * buffer, int samples, int channel, int numChannels)
{
  short * p = (short*)buffer;
  for (int i=0; i<samples; i++) {
    p[i*numChannels+channel] = m_audioBuffer[i];
    //cout << "Write to pos " << i*numChannels+channel << endl;
  }
  m_samples = samples;
}

void AudioReceiver::Add(const char * buffer, int samples, int delay, int channel, int numChannels)
{
  short * p = (short*)buffer;
  int indel = delay - m_lastDelay;

  

  int i, j;
  // Fill up buffer with leftovers

  cout << "Copy leftovers: " <<  m_lastDelay << endl;

  for (i=0; i<m_toCopy; i++) {
    m_audioBuffer[i] = m_audioBuffer[i+m_samples]; 
    //cout << "COPY DOWN " << i+m_samples << " -> " << i << " " << m_audioBuffer[i] << endl;
  }

  int change = delay - m_lastDelay;
  for (i=0; i<m_insDel.isize(); i++)
    m_insDel[i] = 0;

  if (change > 0) {
    for (i=0; i<change; i++) {
      bool b = false;
      do {
	int n = RandomInt(samples-1);
	if (m_insDel[n] == 0)
	  b = true;
	m_insDel[n] = 1;
      } while (!b);
    }
  } else {
    for (i=0; i<-change; i++) {
      bool b = false;
      do {
	int n = RandomInt(samples-1);
	if (m_insDel[n] == 0)
	  b = true;
	m_insDel[n] = -1;
      } while (!b);
    }
  }

  cout << "Delay: " << delay << " last: " << m_lastDelay << " samples: " << samples <<  endl;

  j = m_toCopy;
  for (i=0; i<samples; i++) {
    if (m_insDel[i] != -1) {
      m_audioBuffer[j] = p[i*numChannels+channel];      
      //cout << "COPY " << i*numChannels+channel << " -> " << j << " " << m_audioBuffer[j] << endl;
      j++;
    } else {
    }
    //Duplicate samples
    if (m_insDel[i] == 1) {
      //m_audioBuffer[j] = p[i*numChannels+channel];
      m_audioBuffer[j] = (p[i*numChannels+channel] + p[(i+1)*numChannels+channel])/2;
      //cout << "DUP " << i << endl;
      j++;
    }
    
   }
  //m_lastDelay = delay;
  m_toCopy = j - samples;
  cout << "Have samples: " << j << " delay " << delay << " old delay: " << m_lastDelay << " should be " << m_toCopy << endl;
  m_lastDelay = delay;


}

//=====================================


void SpatialAudio::SetStereoSpeakers()
{
  AudioReceiver r1, r2;
  
  r1.SetPosition(Coordinates(-1., 0, 0));
  r1.SetPosition(Coordinates(1., 0, 0));

  AddReceiver(r1);
  AddReceiver(r2);
}

void SpatialAudio::SetHeadphones()
{
 AudioReceiver r1, r2;
  
  r1.SetPosition(Coordinates(-.1, 0, 0));
  r1.SetPosition(Coordinates(.1, 0, 0));

  AddReceiver(r1);
  AddReceiver(r2);
}

void SpatialAudio::NewFrame()
{
  for (int i=0; i<m_rec.isize(); i++)
    m_rec[i].NewFrame();
}

void SpatialAudio::GetSound(char * buffer)
{
  int i, j;
  int k;

  for (i=0; i<m_bufferSize*m_rec.isize()*2; i++)
    buffer[i] = 0;

  for(j=0; j<m_rec.isize(); j++) {
    cout << "Write channel " << j << endl;
    m_rec[j].Write(buffer, m_bufferSize, j, m_rec.isize());
    // DEBUG
    //if (j==0)
    //break;
  }

  //short * p = (short*)buffer;
  //for (i=0; i<m_bufferSize*m_rec.isize(); i+=2)
  //cout << p[i] << " " << p[i+1] << endl;


  cout << "Wrote " << m_bufferSize * 2 * m_rec.isize() << " bytes." << endl;
}

void SpatialAudio::AddSound(const char * buffer, const Coordinates & c, int channel, int numChannels)
{
  int i, j;
  for (i=0; i<m_rec.isize(); i++) {
    m_rec[i].SetSampleCount(m_bufferSize*256);
    Coordinates r = m_pos + m_rec[i].GetPosition();
    Coordinates rel = c - r;
    double dist = rel.Length();
    double t = dist / 340.;
    int delay = (int)(t*(double)m_sampleRate);
    //delay = 0;
    m_rec[i].Add(buffer, m_bufferSize, delay, channel, numChannels);
  }
  
}
