#include "audio/Wavefile.h"


void WaveFile::SetBufferSize(int size)
{
  m_buffSize = size;
}

bool WaveFile::GetBuffer(char * buff)
{
  bool b =  m_waveFile.Read(buff, m_buffSize);
  return b;
}

int WaveFile::SampleRate()
{
  return m_sampleRate;
}

int WaveFile::NumChannels()
{
  return m_numChannels;
}

void WaveFile::Open(const string & fileName)
{
  int lCnt2 = 0;
  m_waveFile.Open(fileName.c_str());

  unsigned short val = 0;

  char szBuffer[128];
  memset(szBuffer, 0, sizeof(szBuffer));

  char pszRiff[256];
  char pszWaveFmt[256];

  strcpy(pszRiff, "RIFF");
  strcpy(pszWaveFmt, "WAVEfmt ");

  m_waveFile.Read(szBuffer, strlen(pszRiff));
  //cout << szBuffer << endl;
  m_waveFile.Read(lCnt2);
  m_waveFile.Read(szBuffer, strlen(pszWaveFmt));

  cout << lCnt2 << " " << szBuffer << endl;

  m_waveFile.Read(val);
  //val = 0x10;
  m_waveFile.Read(val);
   //val = 0x0;
  m_waveFile.Read(val);
  //val = 0x01;
  m_waveFile.Read(val);
  m_numChannels = val;
  //val = 0x01;
  
  m_waveFile.Read(val);
  m_sampleRate = val; /// Sample rate
  //val = 0x3E80;
  
  m_waveFile.Read(val);
  //val = 0x0;
  m_waveFile.Read(val);
  //val = 0x7D00;
  m_waveFile.Read(val);
  //val = 0x0;
  m_waveFile.Read(val);
  //val = 0x02;
  m_waveFile.Read(val);
  //val = 0x10;



  cout << "Sample rate " <<  m_sampleRate << " channels " << m_numChannels << endl;
 
  //char * pszData = "data";

  //m_waveFile.Read(szBuffer, strlen(pszData));

  char byte;
  while (!m_waveFile.IsEnd()) {
	m_waveFile.Read(byte);
    if (byte != 'd')
      continue;
	m_waveFile.Read(byte);
    if (byte != 'a')
      continue;
	m_waveFile.Read(byte);
    if (byte != 't')
      continue;
	m_waveFile.Read(byte);
    if (byte != 'a')
      continue;
    break;
  }
 



  m_waveFile.Read(lCnt2);

  //m_dataLengthInSamples = lCnt2 / m_bytesPerSample;
}

void WaveFile::Close()
{
  m_waveFile.Close();
}
