/*
 * entity_system.cpp
 *
 *  Created on: 24/06/2013
 *      Author: marrony
 */

#include "entity.h"

#include <string.h>

int32_t EntitySystem::register_component(const char* component_type) {
	for(size_t i = 0; i < component_types.size(); i++) {
		if(!strcmp(component_types[i], component_type))
			return i;
	}

	int32_t index = component_types.size();
	component_types.push_back(component_type);
	ids.push_back(std::vector<int32_t>(entities.size(), -1));
	return index;
}

int32_t EntitySystem::create_entity(const char* entity_name) {
	int32_t id = entities.size();

	entities.push_back(entity_name);
	for(int i = 0; i < ids.size(); i++)
		ids[i].push_back(-1);

	return id;
}

void EntitySystem::add_component(int32_t entity, int32_t type, int32_t component) {
	ids[type][entity] = component;
}
