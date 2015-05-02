#ifndef GAMEENGINE_H
#define GAMEENGINE

#include "engine/GameConfig.h"
#include "graphics/EngineComm.h"


class GameEngine
{
 public:
  GameEngine();
  virtual ~GameEngine();

  void ReadConfig(const string & fileName);

  virtual void SetupMap(int n);
  virtual void Run();

 protected:
  GameConfig m_config;
  GUIEngineControl m_graphics;
};




#endif //GAMEENGINE
