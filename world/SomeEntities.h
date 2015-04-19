#ifndef SOMEENTITIES_H
#define SOMEENTITIES_H

#include "physics/Interaction.h"

// Basic entities for testing.
Entity GetEmpty();

Entity GetTree(const string & name = "");

Entity GetCactus(const string & name = "");

Entity GetPuddle(const string & name = "");

Entity GetBush(const string & name = "");

Entity GetFruit(const string & name = "");

Entity GetInsect(const string & name = "");

Entity GetCreature(const string & name = "");

Interaction GetInteractions();




#endif //SOMEENTITIES_H



