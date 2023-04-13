#include "Entity.h"

#include <cstring>

#include "../Player.h"

void Entity::changeHealth(double delta) {
    hp += delta;
    if (hp <= 0) { delete this; }
}

std::vector<uint8_t> Entity::serialize() const {
    std::vector<uint8_t> result(sizeof(Entity));
    std::memcpy(result.data(), this, sizeof(Entity));
    return result;
}

void Entity::deserialize(const std::vector<uint8_t>& data) { std::memcpy(this, data.data(), sizeof(Entity)); }