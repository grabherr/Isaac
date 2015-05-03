
#ifndef _MAUDDAT_H_
#define _MAUDDAT_H_


#include "util/mutil.h"
#include "sound/mauddef.h"									


const unsigned long c_ui32DefaultBufferSize = 3200;
const unsigned long c_ui32DefaultBufferSizeMs = 100;

const MAUDIO_DATAFORMAT c_defaultFormat = AUDIOFORMAT_PCM_16M16;

class CMAudioData
{
public:
	MDLLEXPORT CMAudioData(unsigned long size = c_ui32DefaultBufferSize);
	MDLLEXPORT CMAudioData(const MAUDIO_DATAFORMAT &, unsigned long size = c_ui32DefaultBufferSizeMs);
	MDLLEXPORT virtual ~CMAudioData();

    //---------------------------------------------------------------------------------------------
	MDLLEXPORT virtual char * GetBufferPtr(void) const;
	MDLLEXPORT virtual unsigned long Size(void) const;
	MDLLEXPORT virtual unsigned long TotalSize(void) const;
	MDLLEXPORT virtual unsigned long SizeMs(void) const;
	MDLLEXPORT virtual MAUDIO_DATAFORMAT GetFormat(void) const;
	MDLLEXPORT virtual bool SetFormat(const MAUDIO_DATAFORMAT &);

    MDLLEXPORT virtual bool SetSizeBytes(unsigned long);
	MDLLEXPORT virtual bool SetSizeMs(unsigned long);

    MDLLEXPORT CMAudioData & operator = (const CMAudioData & dat);

protected:
	CMCharList  		m_theDataBuffer;
	MAUDIO_DATAFORMAT	m_audioFormat;
	MAUDIO_LENGTH		m_sizeMs;

private:

};




#endif	// _MAUDDAT_H_

