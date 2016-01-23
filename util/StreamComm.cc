#include "util/StreamComm.h"

#include <stdio.h>

#include <stdlib.h>

#include <unistd.h>

#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <netdb.h>
#include "base/ThreadHandler.h"

// Make this work an a Mac.
#ifndef MSG_CONFIRM
#define MSG_CONFIRM 0
#endif 

void error(const char * p)
{
  cout << "ERROR: " << p << endl;
}

DataPacket::DataPacket(int size) 
{
  m_size = size;
  m_data.resize(m_size);
  m_ptr = 0;
}

DataPacket::~DataPacket() 
{

}

void DataPacket::Set(const char * p, int size)
{
  m_ptr = 0;
  for (int i=0; i<size; i++)
    m_data[i] = p[i];
}

bool DataPacket::Read(string & s)
{
  s = &m_data[m_ptr];
  m_ptr += s.size()+1;
  return true;
}
bool DataPacket::Read(float & v)
{
  memcpy(&v, &m_data[m_ptr], sizeof(v));
  m_ptr += sizeof(v);
  return true;
}
bool DataPacket::Read(double & v)
{
  memcpy(&v, &m_data[m_ptr], sizeof(v));
  m_ptr += sizeof(v);
  return true;
}
bool DataPacket::Read(int & v)
{
  memcpy(&v, &m_data[m_ptr], sizeof(v));
  m_ptr += sizeof(v);
  return true;
}
bool DataPacket::Read(char & v)
{
  memcpy(&v, &m_data[m_ptr], sizeof(v));
  m_ptr += sizeof(v);
  return true;
}
bool DataPacket::ReadBool(bool & b)
{
  //cout << "READ BOOL " << endl;
  int v = 0;
  memcpy(&v, &m_data[m_ptr], sizeof(v));
  m_ptr += sizeof(v);
  b = false;
  if (v)
    b = true;
  return true;
}

bool DataPacket::Write(const string & s)
{
  for (int i=0; i<s.size(); i++) {
    m_data[m_ptr] = s[i];
    m_ptr++;
  }
  m_data[m_ptr] = 0;
  m_ptr++;
  return true;
}
bool DataPacket::Write(float v)
{
  memcpy(&m_data[m_ptr], &v, sizeof(v));
  m_ptr += sizeof(v);
  return true;
}
bool DataPacket::Write(double v)
{
  memcpy(&m_data[m_ptr], &v, sizeof(v));
  m_ptr += sizeof(v);
  return true;
}
bool DataPacket::Write(int v)
{
  memcpy(&m_data[m_ptr], &v, sizeof(v));
  m_ptr += sizeof(v);
  return true;
}
bool DataPacket::Write(char v)
{
  memcpy(&m_data[m_ptr], &v, sizeof(v));
  m_ptr += sizeof(v);
  return true;
}
bool DataPacket::WriteBool(bool b)
{
  cout << "WRITE BOOL " << endl;
  int v = 0;
  if (b)
    v = 1;
  memcpy(&m_data[m_ptr], &v, sizeof(v));
  m_ptr += sizeof(v);
  return true;
}


//=======================================================
class UDPCommTransmitter : public StreamCommTransmitter
{
 public:
  UDPCommTransmitter(const string & host, int port = DEFPORT);

  virtual ~UDPCommTransmitter();

  virtual void Send(const DataPacket & data) {
    m_mutex.Lock();
    m_buffer.Push(data);
    m_mutex.Unlock();
  }

  bool GetPacket(DataPacket & data) {
    m_mutex.Lock();
    bool b = m_buffer.Pop(data);
    m_mutex.Unlock();
    return b;
  }

private:
  TRingBuffer<DataPacket> m_buffer;
  int m_port;
  string m_host;
  ThreadMutex m_mutex;
  ThreadHandler m_th;
};



class UDPCommReceiver : public StreamCommReceiver
{
 public:
  UDPCommReceiver(int port = DEFPORT);
  virtual ~UDPCommReceiver() {}

 // Retrieves the packets in order in which they were received
  virtual bool Get(DataPacket & data) {
    m_mutex.Lock();
    bool b = m_buffer.Pop(data);
    m_mutex.Unlock();
    return b;
  }

  virtual bool PeekLast(DataPacket & data) {
    m_mutex.Lock();
    bool b = m_buffer.PeekLast(data);
    m_mutex.Unlock();
    return b;
  }

  // Clears out the queue
  virtual void ClearQueue() {
    m_buffer.Clear();
  }

  virtual void Push(const DataPacket & data) {
    m_mutex.Lock();
    m_buffer.Push(data);
    m_mutex.Unlock();
  }



private:
  TRingBuffer<DataPacket> m_buffer;
  int m_port;
  ThreadMutex m_mutex;
  ThreadHandler m_th;
};

//=================================================
class SendThread : public IOneThread
{
public:
  SendThread(const string & host, 
	     int port, 
	     UDPCommTransmitter * pTrans,
	     int attempts = 10) {
    m_port = port;
    m_host = host;
    m_pTrans = pTrans;
    m_attempts = attempts;
  }


protected:

  virtual bool OnDie() {
    return true;
  }

  virtual bool OnDo(const string & msg);

  virtual bool OnInitialize(const string & msg) {
    return true;
  }
  int m_port;
  string m_host;
  UDPCommTransmitter * m_pTrans;
  int m_attempts;
};

bool SendThread::OnDo(const string & msg)
{
   int sock, n;
   unsigned int length;
   struct sockaddr_in server, from;
   struct hostent *hp;
   //char buffer[256];
   
   sock= socket(AF_INET, SOCK_DGRAM, 0);
   if (sock < 0) error("socket");

   server.sin_family = AF_INET;
   hp = gethostbyname(m_host.c_str());
   if (hp==0) error("Unknown host");

   bcopy((char *)hp->h_addr, 
        (char *)&server.sin_addr,
         hp->h_length);
   server.sin_port = htons(m_port);
   length=sizeof(struct sockaddr_in);

   
   struct timeval tv;
   tv.tv_sec = 0;
   tv.tv_usec = 100000;
   if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv)) < 0) {
     error("Error");
   }

   while (true) {
     //cout << "Loop" << endl;

     int counter = 0;

     DataPacket d;
     //d.Write("blah");
     //cout << "Check packet." << endl;
     bool b = m_pTrans->GetPacket(d);
     if (!b) {
       usleep(100);
       continue;
     }
     char retBuff[1024];
     strcpy(retBuff, "");
     //cout << "Start send, efective size " << d.effective_size() << endl;
     bool bYes = false;
     do {
        
       n = sendto(sock,d.Data(),
		  d.effective_size(),0,(const struct sockaddr *)&server,length);

       unsigned int length2=sizeof(struct sockaddr_in);
       n = recvfrom(sock,retBuff,sizeof(retBuff),0,(struct sockaddr *)&from, &length2);
       counter++;
       cout << counter << " " << n << " " << retBuff << endl;
       if (n >= 0)
	 bYes = true;
     } while (n < 0 && counter < m_attempts);
     //cout << "Out" << endl;
     // if (bYes) {
     //write(1,"Got an ack: ",12);
     // write(1,retBuff,n);
     //}
   }
   close(sock);
   return true;
}

//=============================================
//=================================================
class ReadThread : public IOneThread
{
public:
  ReadThread(int port, 
	     UDPCommReceiver * pRec) {
    m_port = port;
    m_pRec = pRec;
    m_attempts = 10;
  }


protected:

  virtual bool OnDie() {
    return true;
  }

  virtual bool OnDo(const string & msg);

  virtual bool OnInitialize(const string & msg) {
    return true;
  }

  int m_port;
  UDPCommReceiver * m_pRec;
  int m_attempts;
};

bool ReadThread::OnDo(const string & msg)
{
   int sock, length, n;
   socklen_t fromlen;
   struct sockaddr_in server;
   struct sockaddr_in from;
   

   sock=socket(AF_INET, SOCK_DGRAM, 0);
   if (sock < 0) error("Opening socket");
   length = sizeof(server);
   bzero(&server,length);
   server.sin_family=AF_INET;
   server.sin_addr.s_addr=INADDR_ANY;
   server.sin_port=htons(m_port);
   if (bind(sock,(struct sockaddr *)&server,length)<0) 
       error("binding");
   fromlen = sizeof(struct sockaddr_in);

   char myHostName[512];
   gethostname(myHostName, sizeof(myHostName));

   //char retBuff[1024];
   while (1) {
     DataPacket d;
     n = recvfrom(sock,d.Data(),d.size(),0,(struct sockaddr *)&from,&fromlen);
     if (n < 0) error("recvfrom");
     //write(1,"Received a datagram: ",21);
     //write(1,buf,n);
     m_pRec->Push(d);
     int counter = 0;
     do {
       // n = sendto(sock,"Got your message\n",17,
       //	  0,(struct sockaddr *)&from,fromlen);
       n = sendto(sock,myHostName,strlen(myHostName),
		  0,(struct sockaddr *)&from,fromlen);
       counter++;
     } while (n < 0 && counter < m_attempts);
     //if (n  < 0) error("sendto");
   }
   return 0;

}

//=============================================

UDPCommTransmitter::UDPCommTransmitter(const string & host, int port)
{
  m_port = port;
  m_host = host;
  m_th.AddThread(new SendThread(host, port, this), ""); 
  m_th.Feed(0, "do");

}

UDPCommTransmitter::~UDPCommTransmitter()
{
}


UDPCommReceiver::UDPCommReceiver(int port)
{
  m_port = port;
  m_th.AddThread(new ReadThread(port, this), ""); 
  m_th.Feed(0, "do");
}











StreamCommTransmitter * GetTransmitter(const string & serverName, int port)
{
  return new UDPCommTransmitter(serverName, port);
}

StreamCommReceiver * GetReceiver(int port)
{
  return new UDPCommReceiver(port);
}

