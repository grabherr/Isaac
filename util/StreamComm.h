#ifndef _STREAMCOMM_H_
#define _STREAMCOMM_H_

#include "base/FileParser.h"

/* the port users will be connecting to */
#define DEFPORT 3491

class DataPacket
{
 public:
  DataPacket(int size = 4096);

  ~DataPacket();

  void Clear() {
    m_ptr = 0;
  }

  const char * Data() const {return &m_data[0];}
  char * Data() {return &m_data[0];}
  int size() const {return m_size;}
  int effective_size() const {return m_ptr;}
  void Set(const char * p, int size);

  
  bool Read(string & s);
  bool Read(float & v);
  bool Read(double & v);
  bool Read(int & v);
  bool Read(char & v);

  bool Write(const string & s);
  bool Write(float v);
  bool Write(double v);
  bool Write(int v);
  bool Write(char v);


 private:
 
  svec<char> m_data;
  int m_size;
  int m_ptr;  
};




class StreamCommTransmitter
{
 public:
  virtual ~StreamCommTransmitter() {}

  // Returns immediately, no feedback if successful
  virtual void Send(const DataPacket & data) = 0;
  
};


class StreamCommReceiver
{
 public:
  virtual ~StreamCommReceiver() {}

  // Retrieves the packets in order in which they were received
  virtual bool Get(DataPacket & data) = 0;

  // Clears out the queue
  virtual void ClearQueue() = 0;
};

// This is how to get an actual instance
StreamCommTransmitter * GetTransmitter(const string & serverName, int port = DEFPORT);
StreamCommReceiver * GetReceiver(int port = DEFPORT);

bool StreamGetHostName(string & host);


template <class T>
class TRingBuffer
{
 public:
  TRingBuffer(int n = 256) {
    m_data.resize(n);
    m_read = 0;
    m_write = 0;
  }

  void SetSize(int n) {
    m_data.resize(n);
    m_read = 0;
    m_write = 0;
  }

  bool Pop(T & t) {
    //cout << "Pop " << m_read << " " <<  m_write << endl;
    if (m_read == m_write)
      return false;
    t = m_data[m_read];
    m_read++;
    if (m_read == m_data.isize())
      m_read = 0;
    return true;
  }

  bool Push(const T & t) {
    //cout << "Pushing " << m_write << " " << m_read << endl;
    m_data[m_write] = t;
    m_write++;
    if (m_write == m_data.isize())
      m_write = 0;
    if (m_write == m_read) {
      m_read++;
      if (m_read == m_data.isize())
	m_read = 0;
    }
    return true;
  }
 
  void Clear() {
    m_read = m_write = 0;
  }

 private:
  svec<T> m_data;
  int m_read;
  int m_write;
};


#endif 


