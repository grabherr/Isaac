#ifndef CHARACTER_H
#define CHARACTER_H

#include "base/SVector.h"
#include "physics/Coordinates.h"
#include "npc/NNet.h"
#include "npc/TopLevel.h"
#include "util/mutil.h"


class Character
{
 public:
  Character();
  virtual ~Character() {

  }

  // Dimensionality of vector
  void SetupPeople(int n, int index);
  void SetName(const string & s) {
    m_name = s;
  }

  void FeedNeutral(const svec<double> & prop, const Coordinates & c, int index);
  void FeedAction(const svec<double> & prop, double in);
  void FeedDone(const svec<double> & prop, double in);

  void SetScore(double s);
  
  void SetCoords(const Coordinates & s) {
    m_self = s;
  }
  const Coordinates & GetCoords() const {return m_self;}

  int GetDesire() const {return m_desire;}
  int GetAvoid() const {return m_avoid;}
  double GetAct() const {return m_act;}

  void Print() const;

  void Read(const string & fileName) {
    CMReadFileStream s;
    s.Open(fileName.c_str());
    Read(s);
    s.Close();
  }
  void Write(const string & fileName) {
    CMWriteFileStream s;
    s.Open(fileName.c_str());
    Write(s);
    s.Close();
  }
  
  void Read(CMReadFileStream & s) {
    m_nn.Read(s);
    m_top.Read(s);
    s.Read(m_name);

    s.Read(m_size);
    s.Read(m_myIndex);
    s.Read(m_desire);
    s.Read(m_avoid);
    s.Read(m_act);
    s.Read(m_score);

    
    int n;
    s.Read(n);
    m_map.resize(n);
    for (int i=0; i<m_map.isize(); i++)
      s.Read(m_map[i]);
  }
  
  void Write(CMWriteFileStream & s) {
    m_nn.Write(s);
    m_top.Write(s);
    s.Write(m_name);

    s.Write(m_size);
    s.Write(m_myIndex);
    s.Write(m_desire);
    s.Write(m_avoid);
    s.Write(m_act);
    s.Write(m_score);
    s.Write(m_map.isize());

    for (int i=0; i<m_map.isize(); i++)
      s.Write(m_map[i]);
  }
 private:
  void Select();
  NeuralNetwork m_nn;
  TopLevel m_top;
  string m_name;
  Coordinates m_self;
  int m_size;
  int m_myIndex;
  int m_desire;
  int m_avoid;
  double m_act;
  double m_score;
  svec<int> m_map;
};





#endif 
