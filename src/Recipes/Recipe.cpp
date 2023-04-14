#include "Recipe.h"

#include <cassert>
#include <set>
#include <vector>

#include "../Entities/Buildings/CraftBuilding.h"
#include "../Entities/Pawns/FighterPawn.h"
#include "../Entities/Pawns/Pawn.h"
#include "../Entities/Pawns/WorkerPawn.h"
#include "../Resource.h"

#ifdef SERVER_SIDE

bool Recipe::checkRequirements(ptr<CraftBuilding> place, bool start) {
    std::set<ptr<WorkerPawn>> workersInside;
    std::set<ptr<FighterPawn>> fightersInside;
    std::multiset<Resource> resourcesInside;
    std::vector<ptr<Pawn>> usedPawns;
    std::vector<ptr<WorkerPawn>> workingPawns;
    std::vector<Resource> usedResources;
    for (Resource r: place->reservedResources) { resourcesInside.insert(r); }
    for (ptr<Pawn> p: place->pawns) {
        if (ptr<WorkerPawn> worker = p.dyn_cast<WorkerPawn>(); worker) {
            workersInside.insert(worker);
        } else if (ptr<FighterPawn> fighter = p.dyn_cast<FighterPawn>(); fighter) {
            fightersInside.insert(fighter);
        } else
            assert(0);
    }
    // TODO:копипаста
    for (FighterPawnType t: inFighters) {
        bool ok = false;
        for (auto it = fightersInside.begin(); it != fightersInside.end(); it++) {
            if ((*it)->currentTask.id == TaskID::Craft && (*it)->getType() == t) {
                usedPawns.push_back((*it).dyn_cast<Pawn>());
                fightersInside.erase(it);
                ok = true;
                break;
            }
        }
        if (!ok) return false;
    }

    for (expertisesID t: inWorkers) {// TODO:rewrite for many expertises
        bool ok = false;
        for (auto it = workersInside.begin(); it != workersInside.end(); it++) {
            if ((*it)->currentTask.id == TaskID::Craft && (*it)->expertises.contains(t)) {
                usedPawns.push_back((*it).dyn_cast<Pawn>());
                workersInside.erase(it);
                ok = true;
                break;
            }
        }
        if (!ok) { return false; }
    }

    for (expertisesID t: reqWorkers) {
        bool ok = false;
        for (auto it = workersInside.begin(); it != workersInside.end(); it++) {
            if ((*it)->currentTask.id == TaskID::Craft && (*it)->expertises.contains(t)) {
                workingPawns.push_back(*it);
                workersInside.erase(it);
                ok = true;
                break;
            }
        }
        if (!ok) { return false; }
    }

    for (Resource t: inResources) {
        bool ok = false;
        for (auto it = resourcesInside.begin(); it != resourcesInside.end(); it++) {
            if ((*it) == t) {
                usedResources.push_back(t);
                resourcesInside.erase(it);
                ok = true;
                break;
            }
        }
        if (!ok) return false;
    }

    if (start) {
        place->pawns.clear();
        place->reservedResources.clear();
        for (Resource p: resourcesInside) { place->reservedResources.insert(p); }
        for (ptr<WorkerPawn> p: workersInside) { place->addPawn(p.dyn_cast<Pawn>()); }
        for (ptr<FighterPawn> p: fightersInside) { place->addPawn(p.dyn_cast<Pawn>()); }

        procResources = usedResources;
        procPawns = usedPawns;
        workers = workingPawns;

        // for(ptr<Pawn> p:usedPawns){p->beIngridient();}
        // for(ptr<WorkerPawn>
        // p:workingPawns){static_cast<ptr<Pawn>>(p)->beIngridient();}
    }
    return true;
}

void Recipe::start(ptr<CraftBuilding> place) {
    this->place = place;
    progress = 0;
    checkRequirements(place, true);
}

void Recipe::tick(double deltaTime) {
    progress += deltaTime;
    if (progress >= duration) finish();
}

void Recipe::cleanup(ptr<Building> where) {
    if (!where) {
        where = place.dyn_cast<Building>();
        place->current = nullptr;
    }
    for (ptr<WorkerPawn> p: workers) {
        where->addPawn(p.dyn_cast<Pawn>());
        p->assignTask(Task(TaskID::Idle));
    }
    procPawns.clear();
    procResources.clear();
    workers.clear();
    progress = 0;
}

void Recipe::cancel() {
    for (ptr<Pawn> p: procPawns) {
        place->addPawn(p);
        p->assignTask(Task(TaskID::Idle));
    }
    for (Resource p: procResources) { place->addResource(p); }
    cleanup();
}
#endif


Recipe *Recipe::clone() {
    Recipe *res = cloneSelf();
#ifdef SERVER_SIDE
    res->progress = 0;
#endif
    res->inResources = inResources;
    res->inWorkers = inWorkers;
    res->inFighters = inFighters;
    res->duration = duration;
    res->reqWorkers = reqWorkers;
    return res;
}

RecipeType Recipe::getType() const {
    return RecipeType::BASIC_RECIPE;
}

std::vector<uint8_t> Recipe::serialize() const { return serializeSelf(); }
size_t Recipe::deserialize(const uint8_t* data) { return deserializeSelf(data); }

std::vector<uint8_t> Recipe::serializeSelf() const {
    size_t size = sizeof(double) + sizeof(size_t) * 4 + sizeof(uint8_t) * (reqWorkers.size() +
            inFighters.size() + inWorkers.size() + inResources.size());
    std::vector<uint8_t> result(size);
    uint8_t* curr = result.data();
    curr += copyVariable(curr, duration);

    curr += copyVariable(curr, reqWorkers.size());
    for(auto i : reqWorkers) {
        curr += copyVariable(curr, i);
    }

    curr += copyVariable(curr, inFighters.size());
    for(auto i : inFighters) {
        curr += copyVariable(curr, i);
    }

    curr += copyVariable(curr, inWorkers.size());
    for(auto i : inWorkers) {
        curr += copyVariable(curr, i);
    }

    curr += copyVariable(curr, inResources.size());
    for(auto i : inResources) {
        curr += copyVariable(curr, i);
    }
    result.insert(result.begin(), static_cast<uint8_t>(getType()));
    return result;
}

size_t Recipe::deserializeSelf(const uint8_t* data) {
    const uint8_t* curr = data;
    curr += initializeVariable(curr, duration);
    size_t size;

    curr += initializeVariable(curr, size);
    reqWorkers.resize(size);
    for(auto& i : reqWorkers){
        curr += initializeVariable(curr, i);
    }

    curr += initializeVariable(curr, size);
    inFighters.resize(size);
    for(auto& i : inFighters){
        curr += initializeVariable(curr, i);
    }

    curr += initializeVariable(curr, size);
    inWorkers.resize(size);
    for(auto& i : inWorkers){
        curr += initializeVariable(curr, i);
    }

    curr += initializeVariable(curr, size);
    inResources.resize(size);
    for(auto& i : inResources){
        curr += initializeVariable(curr, i);
    }
    return curr - data;
}