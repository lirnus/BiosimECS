#pragma once

#include <vector>
#include <cstdint>
#include <cassert>
#include "global_params.h"

namespace bs {
	using Entity = uint32_t;
	static constexpr Entity INVALID = UINT32_MAX;

	// ENTITY MANAGER
	class EntityManager {
	public:
		Entity create();
		void destroy(Entity e);
	private:
		Entity next_id = 0;
		std::vector<Entity> free_list;
	};

	// COMPONENT STORAGE
	template<typename T>
	class ComponentStorage {
	public:
		ComponentStorage() {
			entity_to_index.assign(numberOfPixies, INVALID); // set the size of entity_to_index to numPixies to prevent reallocation
		}

		void add(Entity e, T comp) {
			assert(e < INVALID-3); // check if e < UINT32_MAX-3 (reserved numbers: INVALID, EMPTY, BARRIER, FOOD)
			if (entity_to_index.size() <= e) entity_to_index.resize(e + 10, INVALID); // if entity_to_index to small, extend it by ten
			assert(!has(e)); // check if e already has a component assigned

			entity_to_index[e] = static_cast<Entity>(components.size()); // assign current size as new index
			components.push_back(comp); // add components to vector (grows by 1)
			entities.push_back(e); // add entity to the internal entities list
		}

		void remove(Entity e) {
			assert(has(e)); // check if the entity really has this component

			Entity idx = entity_to_index[e];
			Entity backIdx = static_cast<Entity>(components.size() - 1);

			if (idx != backIdx) { 
				components[idx] = std::move(components[backIdx]); // overwrite the object to be removed with the last element
				Entity movedEntity = entities[backIdx]; // and update "bookkeeping" accordingly
				entities[idx] = movedEntity;
				entity_to_index[movedEntity] = idx;
			}

			components.pop_back(); // remove the now redundant last element
			entities.pop_back();
			entity_to_index[e] = INVALID;
		}

		bool has(Entity e) const {
			return e < entity_to_index.size() && entity_to_index[e] != INVALID;
		}

		T& get(Entity e) {
			return components[entity_to_index[e]];
		}

		const T& get(Entity e) const {
			return components[entity_to_index[e]];
		}

		template<typename Funk>
		void for_each(Funk f) {
			const size_t n = components.size();
			for (size_t i = 0; i < n; ++i) {
				f(entities[i], components[i]);
			}
		}

		size_t size() const {
			return components.size();
		}

	private:
		std::vector<T> components;
		std::vector<Entity> entities;
		std::vector<Entity> entity_to_index;
	};
}
