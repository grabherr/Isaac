

#include "sound/mwavef.h"
#include "sound/mauddat.h"





CMWaveFileHeader::CMWaveFileHeader()
{
  m_dataLengthInSamples = 0;
  m_bytesPerSample = 2;
  m_sampleRate = 16000;
}

CMWaveFileHeader::~CMWaveFileHeader()
{
}


bool CMWaveFileHeader::Read(IMReadStream & file)
{
  long lCnt2 = 0;
  
  unsigned short val = 0;

  char szBuffer[128];

  char * pszRiff = "RIFF";
  char * pszWaveFmt = "WAVEfmt ";

  file.Read(szBuffer, strlen(pszRiff));
  file.Read(lCnt2);
  file.Read(szBuffer, strlen(pszWaveFmt));

  file.Read(val);
  //val = 0x10;
  file.Read(val);
  //val = 0x0;
  file.Read(val);
  //val = 0x01;
  file.Read(val);
  //val = 0x01;
  file.Read(val);
  //val = 0x3E80;
  file.Read(val);
  //val = 0x0;
  file.Read(val);
  //val = 0x7D00;
  file.Read(val);
  //val = 0x0;
  file.Read(val);
  //val = 0x02;
  file.Read(val);
  //val = 0x10;

 
  //char * pszData = "data";

  //file.Read(szBuffer, strlen(pszData));

  char byte;
  while (!file.IsEnd()) {
	file.Read(byte);
    if (byte != 'd')
      continue;
	file.Read(byte);
    if (byte != 'a')
      continue;
	file.Read(byte);
    if (byte != 't')
      continue;
	file.Read(byte);
    if (byte != 'a')
      continue;
    break;
  }
 



  file.Read(lCnt2);

  m_dataLengthInSamples = lCnt2 / m_bytesPerSample;

  return true;
}

bool CMWaveFileHeader::Write(IMWriteStream & file)
{
  long lCnt2 = m_bytesPerSample * m_dataLengthInSamples + 0x24;
  
  unsigned short val;

  char * pszRiff = "RIFF";
  char * pszWaveFmt = "WAVEfmt ";

  file.Write(pszRiff, strlen(pszRiff));
  file.Write(lCnt2);
  file.Write(pszWaveFmt, strlen(pszWaveFmt));

  val = 0x10;
  file.Write(val);
  val = 0x0;
  file.Write(val);
  val = 0x01;
  file.Write(val);
  val = 0x01;
  file.Write(val);
  val = 0x3E80;
  file.Write(val);
  val = 0x0;
  file.Write(val);
  val = 0x7D00;
  file.Write(val);
  val = 0x0;
  file.Write(val);
  val = 0x02;
  file.Write(val);
  val = 0x10;
  file.Write(val);

  lCnt2 = m_bytesPerSample * m_dataLengthInSamples;
  
  char * pszData = "data";

  file.Write(pszData, strlen(pszData));

    
  file.Write(lCnt2);

  return true;
}


long CMWaveFileHeader::GetSampleLength()
{
  return m_dataLengthInSamples;
}

void CMWaveFileHeader::SetSampleLength(long len)
{
  m_dataLengthInSamples = len;
}





//=======================================================
//=======================================================
//=======================================================
//=======================================================

CMWriteWaveFile::CMWriteWaveFile()
{
  m_lCount = 0;
}

CMWriteWaveFile::~CMWriteWaveFile()
{
  //Save();
}


bool CMWriteWaveFile::AppendData(CMAudioData & dat)
{
  if (dat.Size() + m_lCount > m_dataInBytes.length())
    m_dataInBytes.reshape(dat.Size() + m_lCount + 160000);

  char * pData = dat.GetBufferPtr();

  for (int i=0; i<dat.Size(); i++) {
    m_dataInBytes(m_lCount) = pData[i]; 
    m_lCount++;
  }

  return true;
}

bool CMWriteWaveFile::Open(const CMString & fileName)
{
  m_fileName = fileName;
  m_waveFile.Open(m_fileName);
  return true;
}

bool CMWriteWaveFile::Close()
{
  m_waveFile.Close();
  return true;
}

bool CMWriteWaveFile::Save()
{

  //Hard coded 16 bit
  m_fileHeader.SetSampleLength(m_lCount / 2);

  m_fileHeader.Write(m_waveFile);

  char * pData = (char*)m_dataInBytes.data();

  m_waveFile.Write(pData, m_lCount);

  return true;
}




//=======================================================
//=======================================================
//=======================================================
//=======================================================
CMReadWaveFile::CMReadWaveFile()
{
  m_lCountInSamples = 0;
  m_lBytesRead = 0;
  m_bIsEof = true;
}


CMReadWaveFile::~CMReadWaveFile()
{
}

bool CMReadWaveFile::Open(const CMString & fileName)
{
  m_fileName = fileName;
  m_waveFile.Open(m_fileName);
  m_fileHeader.Read(m_waveFile);
  m_lCountInSamples = m_fileHeader.GetSampleLength();
  m_lBytesRead = 0;
  m_bIsEof = false;
  return true;
}

bool CMReadWaveFile::Close()
{
  m_waveFile.Close();
  m_lCountInSamples = 0;
  m_bIsEof = true;
  return true;
}

bool CMReadWaveFile::GetData(CMAudioData & dat)
{
  unsigned long bytesToRead = dat.Size();

  char * pData = dat.GetBufferPtr();

  unsigned long countInBytes = m_lCountInSamples * m_fileHeader.BytesPerSample();

  
  if (m_lBytesRead + bytesToRead > countInBytes) {
    bytesToRead = countInBytes - m_lBytesRead;
    dat.SetSizeBytes(bytesToRead);
    pData = dat.GetBufferPtr();
    m_bIsEof = true;
  }

  m_lBytesRead += bytesToRead;

  m_waveFile.Read(pData, bytesToRead);

  return m_bIsEof;
}



