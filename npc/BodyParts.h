#ifndef BODYPARTS_H
#define BODYPARTS_H

#include "npc/Skeleton.h"


class BodyMap
{
 public:
  BodyMap() {}

  void Read(const string & fileName);

  NPCBoneCoords Get(const string & name, const NPCBoneCoords & def);
  
 private:
  svec<NPCBoneCoords> m_coords;
  svec<string> m_name;
};

class BodyBuilder
{
 public:
  BodyBuilder() {}
  void ReadConfig(const string & fileName) {
    m_map.Read(fileName);
  }

  void GetHand(NPCSkeleton & out, bool bMirror = false);
  void GetFoot(NPCSkeleton & out, bool bMirror = false);
  
  void GetBody(NPCSkeleton & out);
  
  void GetFigure(NPCSkeleton & out);

 private:
  BodyMap m_map;
};


#endif

