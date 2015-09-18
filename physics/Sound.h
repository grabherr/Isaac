#ifndef SOUND_H
#define SOUND_H

#include "base/SVector.h"
#include "physics/Coordinates.h"


class SingleSound
{
 public:
  SingleSound() {
    m_volume = 1.;
  }
  const Coordinates & GetPosition() {return m_coords;}
  void SetPosition(const Coordinates & c) {m_coords = c;}

  const string & GetWavFile() const {return m_soundFile;}
  void SetWavFile(const string & s) {m_soundFile = s;}

  const string & GetName() const {return m_name;}
  void SetName(const string & s) {m_name = s;}

  void SetVolume(double & d) {m_volume = d;}
  double GetVolume() const {return m_volume;}
  
 private:
  double m_volume;
  Coordinates m_coords;
  string m_soundFile;
  string m_name;
};


class Sound
{
 public:
  Sound() {}

  void push_back(const SingleSound & s) {
    m_data.push_back(s);
  }
  int isize() const {return m_data.isize();}
  SingleSound & operator [] (int i) {return m_data[i];}
  const SingleSound & operator [] (int i) const {return m_data[i];}
  
  void UpdateAdd(const string & name, 
		 const string & wavFile,
		 const Coordinates & c);

  void AddEcho(int i, const Coordinates & loc); 

 private:
  int Find(const string & name) {
    for (int i=0; i<m_data.isize(); i++) {
      if (m_data[i].GetName() == name)
	return i;
    }
    return -1;
  }

  svec<SingleSound> m_data;
};

#endif // SOUND_H

