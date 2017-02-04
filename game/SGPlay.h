#ifndef SGPLAY_H
#define SGPLAY_H

#include "engine/GameEngine.h"
#include "engine/DynModels.h"
#include <math.h>
#include "npc/BodyParts.h"
#include "game/CharManip.h"
class Measures
{
public:
  Measures() {}
  string name;
  string sex;
  double spine;
  double sholder;
  double arm;
  double wrist;
  double pelvis;
  double leg;
  double ankle;
};


class AllCharacters
{
public:
  AllCharacters() {
    FlatFileParser parser;
    double scale = 0.032;

    parser.Open("applications_data/schoolgame/measures.txt");
    while (parser.ParseLine()) {
      if (parser.GetItemCount() == 0)
	continue;
      Measures m;
      m.name = parser.AsString(0);
      m.sex = parser.AsString(1);
      parser.ParseLine();
      m.spine = parser.AsFloat(parser.GetItemCount()-1)*scale;
      parser.ParseLine();
      m.sholder = parser.AsFloat(parser.GetItemCount()-1)*scale/2;
      parser.ParseLine();
      m.arm = parser.AsFloat(parser.GetItemCount()-1)*scale;
      parser.ParseLine();
      m.wrist = parser.AsFloat(parser.GetItemCount()-1)*scale;
      parser.ParseLine();
      m.pelvis = parser.AsFloat(parser.GetItemCount()-1)*scale/2;
      parser.ParseLine();
      m.leg = parser.AsFloat(parser.GetItemCount()-1)*scale;      
      parser.ParseLine();
      m.ankle = parser.AsFloat(parser.GetItemCount()-1)*scale;
  
      m_maxln.push_back(m);
    }
  }
  
  ~AllCharacters() {
    int i;
    for (i=0; i<m_p.isize(); i++)
      delete m_p[i];
    for (i=0; i<m_p.isize(); i++)
      delete m_h[i];
    for (i=0; i<m_i.isize(); i++)
      delete m_i[i];
  }

  svec<CharManipulator*> Char() {return m_p;}
  svec<HeadManipulator*> Head() {return m_h;}
  svec<ItemManipulator*> Item() {return m_i;}

  void AddSchool(GameEngine & eng);

  void AddCharacter(GameEngine & eng,
		    BodyBuilder & bb,
		    const StreamCoordinates & pos,
		    const string & name,
		    const string & modelName);

  
  const svec<Measures> & GetMaxln() const {return m_maxln;}
  void GetVector(svec<double> & out, int i);
  void GetVector(svec<double> & out, const string & name) {
    int idx = MaxlIndex(name);
    GetVector(out, idx);
  }
  int GetCount() const {return m_maxln.isize();}
  
private:
  int MaxlIndex(const string & name) {
    int i;
    for (i=0; i<m_maxln.isize(); i++) {
      if (m_maxln[i].name == name)
	return i;
    }
    return -1;
  }
  
  svec<CharManipulator*> m_p;
  svec<HeadManipulator*> m_h;
  svec<ItemManipulator*> m_i;
  svec<Measures> m_maxln;
};



#endif

