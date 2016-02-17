#include "npc/Skeleton.h"

void NPCBone::UpdateChildren(NPCSkeleton & s, const NPCBoneCoords & delta)
{
  int i;

  Coordinates me = GetCoords();

  for (i=0; i<m_children.isize(); i++) {
    cout << "Updating children" << endl;
    NPCBone & n = s[m_children[i]];
    n.AddToAbsCoords(delta);
    n.SetBaseCoords(me);
    n.UpdateChildren(s, delta);
  }

}
