#ifndef RESOURCEENTITY_H
#define RESOURCEENTITY_H
///misha does not believe in me  :(
#include <utility>

#include "../Resource.h"
#include "Entity.h"

class ResourceEntity : public Entity {
public:
    Resource resource;
    ResourceEntity(int id, Resource res, std::pair<double, double> pos);
    virtual ~ResourceEntity() = default;
};
#endif//RESOURCEENTITY_H