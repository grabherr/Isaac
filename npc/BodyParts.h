#ifndef BODYPARTS_H
#define BODYPARTS_H

#include "npc/Skeleton.h"

class BodyBuilder
{
 public:
  BodyBuilder() {}

  void GetHand(NPCSkeleton & out, bool bMirror = false);
  void GetFoot(NPCSkeleton & out, bool bMirror = false);
  
  void GetBody(NPCSkeleton & out);
};


#endif

