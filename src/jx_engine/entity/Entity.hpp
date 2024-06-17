#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <vector>
#include <memory>
#include <algorithm> // for std::find and std::remove_if

#include "jx_engine/component/EComponent.hpp"

class Entity {
    
    public:
        
        void addComponent(std::shared_ptr<EComponent> component);
    
        bool removeComponent(std::shared_ptr<EComponent> component);
        
        std::vector<std::shared_ptr<EComponent>> components;
};

#endif // ENTITY_HPP
