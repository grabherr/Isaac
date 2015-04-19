
#include "physics/Interaction.h"
#include "creature/Creature.h"
#include <math.h>

int main(int argc,char** argv)
{
  Properties prop;
  Properties feel;
  PropertyMutator mut;

  prop.Add("wet", 0.);
  prop.Add("sticky", 0.);
  prop.Add("hot", 0.);
  prop.Add("poisonous", 0.);
  prop.Add("exploding", 0.);
  prop.Add("stingy", 0.);

  feel.Add("happy", 0.);
  feel.Add("pain", 0.);
  feel.Add("thirsty", 0.);

  mut.SetTemplate(prop);


  Interaction ia;
  Coordinates cc(1., 1., 1.);

  ia.AddInteraction("wet", "thirsty", -.5, cc);
  ia.AddInteraction("poisonous", "pain", .7, cc);
  ia.AddInteraction("poisonous", "happy", -.5, cc);
  ia.AddInteraction("hot", "pain", .3, cc);
  ia.AddInteraction("stingy", "pain", .6, cc);
  ia.AddInteraction("exploding", "pain", 1., cc);
  ia.AddInteraction("exploding", "happy", -1., cc);


  return 0;
}
