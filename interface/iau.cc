#include "interface/iau.h"
#include "core/MemBot.h"
#include "world/ExternalWorld.h"



class IAUImp : public IAU
{
 public:
  IAUImp() {
    Coordinates pos;
    m_world.Register(&m_bot, pos);
  }

  virtual ~IAUImp() {}

  virtual void AddCapability(const string & cap, double status) {
  }

  virtual void Hint(const Element & e,
		    double pain,
		    double reward,
		    double collision) {
    m_world.Clear();
    Reaction train;
    AddElement(e);
    train.SetPain(pain);
    train.SetReward(reward);
    train.SetUnsuccess(1.-collision);

    svec<Entity> ent;
    Coordinates pos;
    m_world.GetEntities(ent, pos);

    m_bot.Feedback(train, ent[0], pos);
    m_world.Clear();
   
  }

  virtual void StartFrame() {
    m_world.Clear();
  }

  virtual void SetIAUDirection(const RelativeCoordinates & dir) {
    // Do nothing here
    Coordinates pos;
    m_world.SetCoordinates(pos);
  }

  virtual void SetIAUStatus(double pain, 
			    double reward, 
			    double collision,
			    bool bDeath = false) {
    m_world.SetStatus(pain, reward, collision);
  }

  virtual void SetIAUHealth(double health) {
    // Stub
  }

  virtual void SetIAUCapability(const string & inv, double status) {
    // Stub
  }


  virtual void AddElement(const Element & e) {
    const ElementProperties & prop = e.GetProperties();
    const RelativeCoordinates & relPos = e.GetCoordinates();
    const RelativeCoordinates & facePos = e.GetFaceCoordinates();
    const string & name = e.GetName();
    int id = e.GetID();

    Entity ent;
    ent.resize(prop.isize());

    int i;
    for (i=0; i<prop.isize(); i++)
      ent[i] = prop[i];

    Coordinates pos;
    pos.SetX(relPos.X());
    pos.SetY(relPos.Y());
    pos.SetZ(relPos.Z());

    ent.SetCoordinates(pos);
    
    pos.SetX(facePos.X());
    pos.SetY(facePos.Y());
    pos.SetZ(facePos.Z());
    
    ent.SetSpeed(pos);

    ent.SetName(name);

    m_world.AddEntity(ent);
  }
  
  virtual void CloseFrame() {
    Coordinates botPos;
   
    m_bot.Feedback(m_outcome, m_encountered, botPos);
 
    m_bot.GetAction(m_action);
      
    m_world.Try(m_outcome, botPos, m_encountered, m_bot.GetHandle(), m_action);

    svec<Entity> ent;
    Coordinates pos;
    m_world.GetEntities(ent, pos);
    m_bot.Feed(ent); 
    
  }
 
  virtual RelativeCoordinates GetDirection() {
    RelativeCoordinates relPos(m_action.Get(0),
			    m_action.Get(1),
			    m_action.Get(2));
    return relPos;
    
  }
  
  virtual RelativeCoordinates GetMovement() {
    return GetDirection();
  }
  
  virtual int GetNumCapabilities() {
    return 0;
  }
  
  virtual bool ApplyCapability(string & cap, double & intensity) {
    return false;
  }
  
private:
  // Mirror of the world
  MemBot m_bot;
  ExternalWorld m_world;
  Reaction m_outcome;
  Entity m_encountered;
  ExternalAction m_action;
};

IAU * CreateIAU(const string & datafile)
{
  return new IAUImp;
}

void DestroyIAU(IAU * p, bool bSaveData)
{
  delete p;
}

