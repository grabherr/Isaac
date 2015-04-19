#include "world/SomeEntities.h"
#include "base/RandomStuff.h"

Entity GetEmpty()
{
  Properties prop;
  
  prop.Add("clip", 0.);
  prop.Add("red", 0.);
  prop.Add("green", 0.);
  prop.Add("blue", 0.);
  prop.Add("heavy", 0.);
  prop.Add("movable", 0.);
  prop.Add("tasty", 0.);
  prop.Add("spiky", 0.);
  prop.Add("wet", 0.);

  Entity ent;
  ent.SetProperties(prop);

  return ent;
}


Entity GetTree(const string & name)
{
  Entity ent = GetEmpty();
  ent.SetName(name);
  ent.GetProperties().Set("clip", 1.);
  ent.GetProperties().Set("red", 0.3);
  ent.GetProperties().Set("blue", 0.2);
  ent.GetProperties().Set("green", 0.1);
  ent.GetProperties().Set("heavy", 1.);
  ent.GetProperties().Set("movable", 0.);
  ent.GetProperties().Set("tasty", 0.);
  ent.GetProperties().Set("spiky", 0.);
  ent.GetProperties().Set("wet", 0.);
  return ent;
}

Entity GetCactus(const string & name)
{
  Entity ent = GetEmpty();
  ent.SetName(name);
  ent.GetProperties().Set("clip", 0.9);
  ent.GetProperties().Set("red", 0.);
  ent.GetProperties().Set("blue", 0.1);
  ent.GetProperties().Set("green", 0.9);
  ent.GetProperties().Set("heavy", 1.);
  ent.GetProperties().Set("movable", 0.);
  ent.GetProperties().Set("tasty", 0.);
  ent.GetProperties().Set("spiky", 1.);
  ent.GetProperties().Set("wet", 0.);
  return ent;
}

Entity GetPuddle(const string & name)
{
  Entity ent = GetEmpty();
  ent.SetName(name);
  ent.GetProperties().Set("clip", 0.2);
  ent.GetProperties().Set("red", 0.6);
  ent.GetProperties().Set("blue", 0.9);
  ent.GetProperties().Set("green", 0.7);
  ent.GetProperties().Set("heavy", 0.2);
  ent.GetProperties().Set("movable", 0.5);
  ent.GetProperties().Set("tasty", 0.4);
  ent.GetProperties().Set("spiky", 0.);
  ent.GetProperties().Set("wet", 1.);
  return ent;
}

Entity GetBush(const string & name)
{
  Entity ent = GetEmpty();
  ent.SetName(name);
  ent.GetProperties().Set("clip", 0.7);
  ent.GetProperties().Set("red", 0.2);
  ent.GetProperties().Set("blue", 0.3);
  ent.GetProperties().Set("green", 0.9);
  ent.GetProperties().Set("heavy", 0.7);
  ent.GetProperties().Set("movable", 0.);
  ent.GetProperties().Set("tasty", 0.);
  ent.GetProperties().Set("spiky", 0.);
  ent.GetProperties().Set("wet", 0.);
  return ent;
}

Entity GetFruit(const string & name)
{
  Entity ent = GetEmpty();
  ent.SetName(name);
  ent.GetProperties().Set("clip", 0.7);
  ent.GetProperties().Set("red", 0.9);
  ent.GetProperties().Set("blue", 0.1);
  ent.GetProperties().Set("green", 0.1);
  ent.GetProperties().Set("heavy", 0.2);
  ent.GetProperties().Set("movable", 0.2);
  ent.GetProperties().Set("tasty", 1.);
  ent.GetProperties().Set("spiky", 0.);
  ent.GetProperties().Set("wet", 0.1);
  return ent;
}

Entity GetInsect(const string & name)
{
  Entity ent = GetEmpty();
  ent.SetName(name);
  ent.GetProperties().Set("clip", 0.7);
  ent.GetProperties().Set("red", 0.1);
  ent.GetProperties().Set("blue", 0.1);
  ent.GetProperties().Set("green", 0.1);
  ent.GetProperties().Set("heavy", 0.1);
  ent.GetProperties().Set("movable", 0.9);
  ent.GetProperties().Set("tasty", 1.);
  ent.GetProperties().Set("spiky", 0.);
  ent.GetProperties().Set("wet", 0.);
  return ent;
}

Entity GetCreature(const string & name)
{
  Entity ent = GetEmpty();
  ent.SetName(name);
  ent.GetProperties().Set("clip", 1.);
  ent.GetProperties().Set("red", 0.9);
  ent.GetProperties().Set("blue", 0.8);
  ent.GetProperties().Set("green", 0.8);
  ent.GetProperties().Set("heavy", 0.3);
  ent.GetProperties().Set("movable", 1.);
  ent.GetProperties().Set("tasty", 0.);
  ent.GetProperties().Set("spiky", 0.);
  ent.GetProperties().Set("wet", 0.);
  return ent;
}

Interaction GetInteractions()
{
  Interaction inter;
  Coordinates cc;
  cc[0] = cc[1] = cc[2] = 1.;
  inter.AddInteraction("clip", "unsuccess", 1, cc);
  inter.AddInteraction("tasty", "satisfying", 1, cc);
  inter.AddInteraction("spiky", "pain", 1, cc);
  inter.AddInteraction("clip", "pain", 0.3, cc);

  return inter;
}
