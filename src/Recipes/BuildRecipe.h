#include "Recipe.h"
#include "../Entities/Buildings/BuildingRegisty.h"
#include "../Resource.h"
#include <utility>

class BuildRecipe: public Recipe {
public:
    std::pair<double, double> pos;
    BuildingIdea toBuild;

    BuildRecipe(std::pair<double, double> pos, int index):pos(pos), toBuild(BuildingRegisty::database[index]){
        inResources.push_back(Resource::DummyOre);//TODO:get from database
        duration = 4;
    }

    void deserialize() override {};
    void finish() override;
    std::string serialize() const override{};
};