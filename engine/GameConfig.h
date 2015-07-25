#ifndef GAMECONFIG_H
#define GAMECONFIG_H


#include "base/FileParser.h"
#include "graphics/Messages.h"
#include "physics/Interaction.h"

//==============================================
class BasicConfig
{
  friend class GameConfig;
 public:
  BasicConfig() {
    m_resX = 1024;
    m_resY = 768;
    m_bFS = false;
  }
  const string & GetGraphicsEngine() const {return m_graphicsEng;}
  
  int ResX() const {return m_resX;}
  int ResY() const {return m_resY;}
  int Full() const {return m_bFS;}
  
 protected:
  string m_graphicsEng;
  int m_resX;
  int m_resY;
  bool m_bFS;

};

//==============================================
class MapConfig
{
  friend class GameConfig;
 public:
  MapConfig() {
    m_gravity = 9.81;
  }

  const string & GetName() const {return m_name;}
  const Terrain & GetTerrain() const {return m_terrain;}

  int GetNodeCount() const {return m_nodes.isize();}
  const SceneNode & GetNode(int i) const {return m_nodes[i];}

  int GetAnimatedNodeCount() const {return m_animNodes.isize();}
  const AnimatedSceneNode & GetAnimatedNode(int i) const {return m_animNodes[i];}

  int GetPhysModelCount() const {return m_physNodes.isize();}
  const AnimatedSceneNode & GetPhysModelNode(int i) const {return m_physNodes[i];}

  double GetGravity() const {return m_gravity;}
  
 protected:
  string m_name;
  Terrain m_terrain;
  double m_gravity;
  svec<SceneNode> m_nodes;
  svec<AnimatedSceneNode> m_animNodes;
  svec<AnimatedSceneNode> m_physNodes;
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
  const Entity & GetTemplate() const {return m_template;}

 private:
  BasicConfig m_basics;
  svec<MapConfig> m_maps;
  Entity m_template;
  Properties m_actions;
  svec<Entity> m_all;
};






#endif //GAMECONFIG_H


