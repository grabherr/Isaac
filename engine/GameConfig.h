#ifndef GAMECONFIG_H
#define GAMECONFIG_H


#include "base/FileParser.h"
#include "graphics/Messages.h"

//==============================================
class BasicConfig
{
  friend class GameConfig;
 public:
  BasicConfig() {}
  const string & GetGraphicsEngine() const {return m_graphicsEng;}
  
 protected:
  string m_graphicsEng;
};

//==============================================
class MapConfig
{
  friend class GameConfig;
 public:
  MapConfig() {}

  const string & GetName() const {return m_name;}
  const Terrain & GetTerrain() const {return m_terrain;}

  int GetNodeCount() const {return m_nodes.isize();}
  const SceneNode & GetNode(int i) const {return m_nodes[i];}

  int GetAnimatedNodeCount() const {return m_animNodes.isize();}
  const AnimatedSceneNode & GetAnimatedNode(int i) const {return m_animNodes[i];}

 protected:
  string m_name;
  Terrain m_terrain;
  svec<SceneNode> m_nodes;
  svec<AnimatedSceneNode> m_animNodes;
};

//==============================================
class GameConfig
{
 public:
  GameConfig() {}
  void Read(const string & fileName);
  
  const BasicConfig & GetBasicConfig() const {return m_basics;}

  int NumMaps() {return m_maps.isize();}
  const MapConfig & GetMap(int i) const {return m_maps[i];}

 private:
  BasicConfig m_basics;
  svec<MapConfig> m_maps;
};






#endif //GAMECONFIG_H


