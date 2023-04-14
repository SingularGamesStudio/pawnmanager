#ifndef BUILDINGREGISTRY_H
#define BUILDINGREGISTRY_H
#include <vector>

class Recipe;
class BuildRecipe;

struct BuildingIdea {
    std::vector<Recipe*> available;
    int radius;
    int index;
    BuildingIdea(int radius, int index) : radius(radius), index(index) {}
};

class BuildingRegisty {
public:
    static std::vector<BuildRecipe*> database;

    static void init();
};
#endif