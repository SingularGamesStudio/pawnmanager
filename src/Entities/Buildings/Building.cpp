#include "Building.h"
#include "../ResourceEntity.h"

#include <cassert>
#include <set>
#include <vector>
#include <random>

#include "../../Resource.h"
#include "../Entity.h"
#include "../Pawns/Pawn.h"

void Building::addResource(Resource resource) { resources.insert(resource); }

bool Building::removeResource(Resource resource) {
    auto x = resources.find(resource);
    if (x == resources.end()) return false;
    resources.erase(x);
    return true;
}

void Building::addPawn(ptr<Pawn> pawn) {
    if (pawns.contains(pawn)) assert(0);
    pawns.insert(pawn);
}

void Building::removePawn(ptr<Pawn> pawn) { pawns.erase(pawn); }

Building::~Building(){
    for(ptr<Building> b : children){
        b.del();
    }
    for(ptr<Pawn> p : pawns) {
        p.del();
    }
    for(Resource r: reservedResources){
        resources.insert(r);
    }
    std::default_random_engine rng;
    std::uniform_real_distribution<float> dist(-1, 1);
    std::uniform_real_distribution<float> dist2(0, std::numbers::pi_v<float>);
    for(Resource r:resources) {
        float x = 2;
        float y = 2;
        while (x * x + y * y >= 1) {
            x = dist(rng);
            y = dist(rng);
        }
        std::pair<double, double> pos = position;
        pos.first += x*radius;
        pos.second += x*radius;
        makeptr<ResourceEntity>(r, pos);
    }
}