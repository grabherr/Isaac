#include "physics/Sound.h"

void AddEcho(int i, const Coordinates & loc)
{
  throw;
}

void Sound::UpdateAdd(const string & name, 
		      const string & wavFile,
		      const Coordinates & c)
{

  int index = Find(name);
  if (index == -1) {
    index = m_data.isize();
    SingleSound tmp;
    tmp.SetName(name);
    tmp.SetWavFile(wavFile);
    tmp.SetPosition(c);
    m_data.push_back(tmp);
    cout << "ADDING SOUND " << name << endl;
  }
  m_data[index].SetWavFile(wavFile);
  m_data[index].SetPosition(c);
}

