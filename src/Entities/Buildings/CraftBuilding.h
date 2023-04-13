#ifndef CRAFTBUILDING_H
#define CRAFTBUILDING_H
///misha does not believe in me  :(
#include <iostream>
#include <vector>

#include "../../Recipes/Recipe.h"
#include "Building.h"

class CraftBuilding : public Building {

    void stopRecipe();

public:
    CraftBuilding(std::pair<double, double> pos, ptr<Player> owner, unsigned int hp, double radius = baseBuildingRadius,
                  ptr<Building> parent = ptr<Building>())
        : Building(pos, owner, hp, radius, parent),
          Entity(pos, owner, hp, radius),
          current(nullptr) {}
    std::vector<Recipe*> recipes;
    Recipe* current;

    bool assignRecipe(Recipe* recipe);

    void tick(double deltaTime) override;
};
#endif//CRAFTBUILDING_H