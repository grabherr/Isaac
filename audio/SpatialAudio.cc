#define FORCE_DEBUG

#include "audio/SpatialAudio.h"

#include "base/RandomStuff.h"

void AudioReceiver::Write(char * buffer, int samples, int channel, int numChannels)
{
  short * p = (short*)buffer;
  for (int i=0; i<samples; i++) {
    p[i*numChannels+channel] += m_audioBuffer[i];
    //cout << "Write to pos " << i*numChannels+channel << endl;
  }
  m_samples = samples;
  cout << "COORDINATES Write audio!!" << endl;
}

short Amplify(short a, double level) 
{
  if (level > 1.2)
    level = 1.2;
  level *= (double)a;
  if (level > 32767)
    level = 32767;
  if (level < -32767)
    level = -32767;

  a = (short)level;
  return a;
}

void AudioReceiver::Add(const char * buffer, int samples, int delay, double dist, int channel, int numChannels)
{
  short * p = (short*)buffer;
  int indel = delay - m_lastDelay;

  

  int i, j;
  // Fill up buffer with leftovers

  cout << "Copy leftovers: " <<  m_lastDelay << "  samples: " << m_samples << endl;

  for (i=0; i<m_toCopy; i++) {
    m_audioBuffer[i] = m_audioBuffer[i+m_samples]; 
    //cout << "COPY DOWN " << i+m_samples << " -> " << i << " " << m_audioBuffer[i] << endl;
  }

  int change = delay - m_lastDelay;
  for (i=0; i<m_insDel.isize(); i++)
    m_insDel[i] = 0;

  int maxChange = samples / 4;
  if (change > maxChange) {
    cout << "Padding buffer..." << endl;
    int toFill = change / maxChange;
    change = change - toFill*maxChange;
    for (j=m_toCopy; j<toFill*maxChange+m_toCopy;  j++) {
      if (j >= m_audioBuffer.isize())
	break;
      m_audioBuffer[j] = 0;      
    }
    m_toCopy = toFill*maxChange+m_toCopy;
    cout << "Padded, change: " << change << " filled: " << m_toCopy << endl;
  }

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
  double level = 1/dist/dist;

  // DEBUG
  //level = 1.;
  cout << "COORDINATES LEVEL " << level << " dist " << dist << endl;
  // DEBUG

  j = m_toCopy;
  for (i=0; i<samples; i++) {
    if (j >= m_audioBuffer.isize())
      break;
    if (m_insDel[i] != -1) {
      m_audioBuffer[j] = Amplify(p[i*numChannels+channel], level);      
      //cout << "COPY " << i*numChannels+channel << " -> " << j << " " << m_audioBuffer[j] << endl;
      j++;
    } else {
    }
    //Duplicate samples
    if (m_insDel[i] == 1) {
      //m_audioBuffer[j] = p[i*numChannels+channel];
      m_audioBuffer[j] = Amplify((p[i*numChannels+channel] + p[(i+1)*numChannels+channel])/2, level);
      //cout << "DUP " << i << endl;
      j++;
    }
    
   }
  //m_lastDelay = delay;
  m_toCopy = j - samples;
  cout << "Have samples: " << j << " delay " << delay << " old delay: " << m_lastDelay << " should be " << m_toCopy << " buffer size: " <<  m_audioBuffer.isize() << endl;
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

void SpatialAudio::GetSound(char * buffer, bool bClear)
{
  int i, j;
  int k;
  if (bClear) {
    for (i=0; i<m_bufferSize*m_rec.isize()*2; i++)
      buffer[i] = 0;
  }

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
    Coordinates ear = m_rec[i].GetPosition();

    //cout << "COORDINATES Source: ";
    //c.Print();
    //cout << "COORDINATES Camera: ";
    //m_pos.Print();
    //cout << "COORDINATES Ear: ";
    //r.Print();

    Coordinates tmp = ear;
    // cout << "EAR before ";
    //ear.Print();
    tmp[2] = ear[1];
    tmp[1] = ear[2];
    SphereCoordinates s = tmp.AsSphere();
    // cout << "phi " << s.phi() << " rot " << m_rot[1]/2/3.1415*360.;
    s.SetPhi(s.phi()+(m_rot[1]/*-3.1415926/4.*/));
    tmp.FromSphere(s);
    //cout << " after " << s.phi() << endl;
    ear[2] = tmp[1];
    ear[1] = tmp[2];
    //cout << "EAR after ";
    //ear.Print();

    Coordinates r = m_pos - ear;

    Coordinates rel = c - r;
    double dist = rel.Length();
    //cout << "COORDINATES Distance: " << dist << " -> "; 
    //rel.Print();



    double t = dist / 340.;
    int delay = (int)(t*(double)m_sampleRate);
    //delay = 0;
    cout << "Add to buffer " << m_bufferSize << " delay " << delay << " dist " << dist << endl;
    m_rec[i].Add(buffer, m_bufferSize, delay, dist, channel, numChannels);
  }
  
}



//==============================================================
MultiSourceAudio::MultiSourceAudio() {
  m_pBuffer = NULL;
  m_rawBufferSize = 0;
  m_bufferSize = 0;
  m_sampleRate = 0;
  m_scale = 2000.;
}

MultiSourceAudio::~MultiSourceAudio() {
  for (int i=0; i<m_sources.isize(); i++) {
    delete m_sources[i];
    if (m_info[i] != NULL)
      delete m_info[i];
  }
  delete m_pBuffer;
}

  
void MultiSourceAudio::SetBufferSize(int bufSize, int channels) {    
  m_bufferSize = bufSize/channels/2;
  m_rawBufferSize = bufSize;
  m_pBuffer = new char[m_rawBufferSize];
}

void MultiSourceAudio::SetSampleRate(int r) {
  m_sampleRate = r;
}

void MultiSourceAudio::SyncAddAudioSource(const string & name,
					  const Coordinates & c, 
					  const string & fileName)
{
  int index = Find(name);
  if (index == -1) {
    SpatialAudio * p = AddAudioSource(name, c, fileName);
  } else {
    // TODO: Change sound when file name changes!!
    m_sources[index]->Position() = c;
  }
  
}

SpatialAudio * MultiSourceAudio::AddAudioSource(const string & name,
						const Coordinates & c_raw, 
						const string & fileName) {
  Coordinates c = c_raw/m_scale; 
  SpatialAudio * spat = new SpatialAudio;
  spat->SetName(name);
  spat->SetStereoSpeakers();
  spat->SetSampleRate(m_sampleRate);
  spat->SetBufferSize(m_rawBufferSize, 2);
  
  spat->Position() = m_pos;
  spat->Rotation() = m_rot;
  
  SourceInfo * info = new SourceInfo(m_rawBufferSize);
  if (fileName != "") 
    info->SetWavFile(fileName);
  info->SetPosition(c);

  m_sources.push_back(spat);
  m_info.push_back(info);
  return spat;
}

void MultiSourceAudio::SetPosition(const Coordinates & pos) {
  m_pos = pos / m_scale;
  //cout << "MULT Position: ";
  m_pos.Print();
  for (int i=0; i<m_sources.isize(); i++)
    m_sources[i]->Position() = m_pos;
}

void MultiSourceAudio::SetRotation(const Coordinates & rot) {
  m_rot = rot;
  for (int i=0; i<m_sources.isize(); i++)
    m_sources[i]->Rotation() = rot;
}


SpatialAudio * MultiSourceAudio::Get(const string & name) {
  for (int i=0; i<m_sources.isize(); i++) {
    if (m_sources[i]->GetName() == name)
      return m_sources[i];
  }
  return NULL;
}

  
bool MultiSourceAudio::GetSound(char * buffer) {
  int i;
  cout << "Raw buffer size " << m_rawBufferSize << endl;
  for (i=0; i<m_rawBufferSize; i++)
    buffer[i] = 0;
  bool b = false;
  for (i=0; i<m_sources.isize(); i++) {
    if (m_info[i]->GetBuffer(m_pBuffer)) {
      m_sources[i]->Position() = m_pos; 
      int channels = m_info[i]->GetWavFile().NumChannels();
      m_sources[i]->AddSound(m_pBuffer, m_info[i]->GetPosition(), 0, channels);
      
      m_sources[i]->GetSound(buffer, false);
      b = true;
    }      
    
  }
  return b;
}
