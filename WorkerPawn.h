#include "Pawn.h"
#include "Building.h"
#include "Task.h"
#include <set>
#include <unordered_map>
#include <queue>
#include <vector>
enum class expertisesID{DummySmeltery, DummyMetalworking, DummtTrainership};
class WorkerPawn: public Pawn{
public:
    Building* positionBuilding;
    bool travelling;
    bool manning;
    Resource* holding;
    Task CurrentTask;
    void assignTask(Task toAssign){
        switch(toAssign.id){
            case TaskID::PawnGet:
                moveTo(toAssign.destination)
                holding = toAssign.resource;
                (*holding).beingHolded = true;
            case TaskID::PawnMove:
                moveTo(toAssign.destination)
            case TaskID::Man:
                moveTo(toAssign.destination);
                manning = true;
            default:
                throw("Unexpected WorkerPawn TaskID: ", toAssign.id);
        }
    }
    std::set<expertisesID> expertises;
private:
    const double timePerMove = 3;
    const int ticksPerSecond = 100'000'000
    void takePresentResource(Resource* toTake);
    void moveTo(Building* dest){
        travelling = true;
        std::unordered_map<Building*, std::vector<Building*> > visited;
        std::queue<Building*> q;
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
        while(currentTime < timePerMove)
        {
            currentTime += timePerMove / ticksPerSecond;
            position.first = currentTime / ticksPerSecond * (*positionBuilding).position.first
                             + (1 - currentTime / ticksPerSecond) * (*positionBuilding).position.first;
            position.second = currentTime / ticksPerSecond * (*dest).position.second
                             + (1 - currentTime / ticksPerSecond) * (*dest).position.second;
        }
        positionBuilding = dest;
    }
};
