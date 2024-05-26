#include "Entity.hpp"

void Entity::addComponent(std::shared_ptr<EComponent> component) {
    components.push_back(component);
}

bool Entity::removeComponent(std::shared_ptr<EComponent> component) {
    auto it = std::find(components.begin(), components.end(), component);
    if (it != components.end()) {
        components.erase(it);
        return true;
    }
    return false;
}

