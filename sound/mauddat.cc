
#include "sound/mauddat.h"									



CMAudioData::CMAudioData(unsigned long size)
{
  m_theDataBuffer.reshape(size);
  //Default to 16 KHz, 16 bit mono
  m_audioFormat = AUDIOFORMAT_PCM_16M16;
  m_sizeMs = m_theDataBuffer.length() / 32;
}

CMAudioData::CMAudioData(const MAUDIO_DATAFORMAT & format, unsigned long sizeMS)
{
  m_audioFormat = format;
  m_sizeMs = sizeMS;

  long size = m_sizeMs * 32;
  m_theDataBuffer.reshape(size);
}

CMAudioData::~CMAudioData()
{
}

CMAudioData & CMAudioData::operator = (const CMAudioData & dat)
{

  m_audioFormat = dat.m_audioFormat;
  m_sizeMs = dat.m_sizeMs;

  //long size = dat.m_theDataBuffer.length();

  m_theDataBuffer = dat.m_theDataBuffer;

  return *this;
}

    //---------------------------------------------------------------------------------------------
char * CMAudioData::GetBufferPtr(void) const
{
  return (char * )m_theDataBuffer.data();
}

unsigned long CMAudioData::Size(void) const
{
  return m_theDataBuffer.length();
}

unsigned long CMAudioData::TotalSize(void) const
{
  return m_theDataBuffer.length();
}


unsigned long CMAudioData::SizeMs(void) const
{
  return m_sizeMs;
}

MAUDIO_DATAFORMAT CMAudioData::GetFormat(void) const
{
  return m_audioFormat;
}

bool CMAudioData::SetFormat(const MAUDIO_DATAFORMAT & format)
{
  m_audioFormat = format;
  return true;
}

bool CMAudioData::SetSizeBytes(unsigned long size)
{
  m_theDataBuffer.reshape(size);
  m_sizeMs = m_theDataBuffer.length() / 32;
  return true;
}

bool CMAudioData::SetSizeMs(unsigned long sizeMs)
{
  //throw;
  m_sizeMs = sizeMs;
  return false;
}





