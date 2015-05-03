
#ifndef _MWAVEF_H_
#define _MWAVEF_H_

#include "util/mutil.h"
#include <stdio.h>



//#define MAXDATA 8000000

class CMAudioData;


//16 KHz 16 bit mono only!!!!
class CMWaveFileHeader
{
public:
    CMWaveFileHeader();
    ~CMWaveFileHeader();

    bool Read(IMReadStream & file);
    bool Write(IMWriteStream & file);

    long GetSampleLength();
    void SetSampleLength(long len);

    long BytesPerSample() {return m_bytesPerSample;}

private:
    long m_dataLengthInSamples;
    long m_bytesPerSample;
    long m_sampleRate;

};




//=======================================================
//=======================================================
//=======================================================
//=======================================================
class CMWriteWaveFile
{
public:
    MDLLEXPORT CMWriteWaveFile();
    MDLLEXPORT ~CMWriteWaveFile();

    //bool AddValue(short);
    MDLLEXPORT bool AppendData(CMAudioData & dat);

    MDLLEXPORT bool Open(const CMString & fileName);
    MDLLEXPORT bool Close();

    MDLLEXPORT bool Save();

protected:
	CMCharList m_dataInBytes;
	
    long  m_lCount;

    CMString  m_fileName;
    CMWaveFileHeader m_fileHeader;

    CMWriteFileStream m_waveFile;
};



//=======================================================
//=======================================================
//=======================================================
//=======================================================
class CMReadWaveFile
{
public:
    MDLLEXPORT CMReadWaveFile();
    MDLLEXPORT ~CMReadWaveFile();

    MDLLEXPORT bool GetData(CMAudioData & dat);

    MDLLEXPORT bool Open(const CMString & fileName);
    MDLLEXPORT bool Close();

    MDLLEXPORT bool IsEOF() {return m_bIsEof;}

protected:

    CMString  m_fileName;

    CMWaveFileHeader m_fileHeader;
    unsigned long  m_lCountInSamples;
    unsigned long  m_lBytesRead;

	bool m_bIsEof;

    CMReadFileStream m_waveFile;
};



#endif
