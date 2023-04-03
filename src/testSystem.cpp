#include "Player.h"
#include "Entities/Buildings/Building.h"
#include "Entities/Buildings/CraftBuilding.h"
#include "Entities/Pawns/WorkerPawn.h"
#include "Recipes/Recipe.h"
#include "testSystem.h"
#include "Recipes/CraftRecipe.h"
#include "Entities/Buildings/BuildingRegisty.h"
#include <random>


CraftRecipe* recipe;
ptr<Player> player;
ptr<CraftBuilding> crafter;

ptr<Player> initTest() {

    player = makeptr<Player>();
    player->manager.owner = player;
    BuildingRegisty::init();
    player->hub = makeptr<Building>(std::make_pair(80, -120), player, 100);
    for(int i = 0; i<30; i++){
        player->hub->addResource(Resource::DummyOre);
    }
    for(int i = 0; i<5; i++){
        ptr<WorkerPawn> pawn = makeptr<WorkerPawn>();
        pawn->create(player->hub);
        player->pawns.push_back(static_cast<ptr<Pawn>>(pawn));
    }
    crafter = makeptr<CraftBuilding>(std::make_pair(100, 120), player, 100);
    player->hub->children.push_back(static_cast<ptr<Building>>(crafter));
    crafter->parent = player->hub;
    recipe = new CraftRecipe();
    recipe->inResources.push_back(Resource::DummyOre);
    recipe->outResources.push_back(Resource::DummyIngot);
    recipe->duration = 5;
    return player;
}

void tickBuildings(ptr<Building> place, double deltaTime) {
    place->tick(deltaTime);
    for(ptr<Building> ch:place->children){
        tickBuildings(ch, deltaTime);
    }
}
std::mt19937 rnd(42);
void tick(double deltaTime) {
    player->tick();
    if(crafter->current== nullptr)
        crafter->assignRecipe(dynamic_cast<Recipe*>(recipe));
    for(ptr<Pawn> p:player->pawns){
        p->tick(deltaTime);
        if(p->currentTask.id==TaskID::Idle && rnd()%5000==0){
            p->assignTask(Task(TaskID::Transport, player->hub, static_cast<ptr<Building>>(crafter), Resource::DummyOre));
        }
    }
    tickBuildings(player->hub, deltaTime);
}

