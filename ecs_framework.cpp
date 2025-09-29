// this file contains the framework of the ECS system to be used globally,
// mainly an entity manager, component storage template.
#include <vector>
#include <iostream>
#include <unordered_map>
#include <cassert>

#include "ecs_framework.h"

namespace bs {

    // EntityManager Implementations
    Entity EntityManager::create() {
        Entity e;
        if (!free_list.empty()) { e = free_list.back(); free_list.pop_back(); }
        else e = next_id++;
        return e;
    }

    void EntityManager::destroy(Entity e) {
        free_list.push_back(e);
    }

}