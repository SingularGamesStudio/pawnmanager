#include "Pawn.h"
#include "Building.h"
#include "Task.h"
#include "Entity.h"
#include <set>
#include <unordered_map>
#include <queue>
#include <vector>
enum class expertisesID{DummySmeltery, DummyMetalworking, DummtTrainership};
class WorkerPawn: public Pawn{
public:
    Building* positionBuilding;
    
    void create(Building* placeOfCreation) {
        currentTask = Task(TaskID::Idle, placeOfCreation);
        travelling = false;
        holding = nullptr;
        owner = placeOfCreation->owner;
        destination = placeOfCreation;
        inside = placeOfCreation;
    }

    void assignTask(Task toAssign){
        switch(toAssign.id){
            case TaskID::Get:
                moveTo(toAssign.destination);
                holding = toAssign.resource;
                (*holding).beingHolded = true;
            case TaskID::Move:
                moveTo(toAssign.destination);
            case TaskID::BeIngridient:
                moveTo(toAssign.destination);
            default:
                throw("Unexpected WorkerPawn TaskID: ", toAssign.id);
        }
    }
    std::set<expertisesID> expertises;
    void moveToBuilding(Building* dest){
        travelling = true;
        std::unordered_map<Building*, std::vector<Building*> > visited;
        std::queue<Building*> q;
        destination = dest;
        q.push(positionBuilding);
        while(!q.empty()){
            Building* currentB = q.front();
            q.pop();
            std::vector<Building*> way = visited[currentB];
            for(Building* toGo:currentB->children)
            {
                if(!visited[toGo].empty())
                    continue;
                visited[toGo] = way;
                visited[toGo].push_back(toGo);
                q.push(toGo);
            }
            if(!visited[currentB->parent].empty())
                continue;
            visited[currentB->parent] = way;
            visited[currentB->parent].push_back(currentB->parent);
            q.push(currentB->parent);
        }
        for(Building* v : visited[dest])
        {
            moveToNeighbour(v);
        }
        travelling = false;
    }
    void moveToNeighbour(Building* dest){
        double currentTime = 0;
        double timePerMove = sqrt(abs(position.first - dest->position.first) * abs(position.first - dest->position.first) +
            abs(position.second - dest->position.second) * abs(position.second - dest->position.second));
        while(currentTime < timePerMove)
        {
            currentTime += timePerMove / ticksPerSecond;
            position.first = currentTime / ticksPerSecond * (*positionBuilding).position.first
                             + (1 - currentTime / ticksPerSecond) * (*positionBuilding).position.first;
            position.second = currentTime / ticksPerSecond * (*dest).position.second
                             + (1 - currentTime / ticksPerSecond) * (*dest).position.second;
        }
        positionBuilding = dest;
        position = dest->position;
    }
    void takeResourceFromBuilding(Building* dest, Resource res) {
        moveToBuilding(dest);
        dest->resources.erase(dest->resources.find(res));
        holding = res;
    }
    void moveResourceTo(Building* dest) {
        moveToBuilding(dest);
        drop();
    }
};
