#ifndef RESOURCEENTITY_H
#define RESOURCEENTITY_H
#include <utility>

#include "../Resource.h"
#include "Entity.h"

class ResourceEntity : public Entity {
public:
    Resource resource;
    ResourceEntity(Resource res, std::pair<double, double> pos);
    void destroy() override;
};
#endif//RESOURCEENTITY_H