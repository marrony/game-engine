/*
 * entity.h
 *
 *  Created on: 24/06/2013
 *      Author: marrony
 */

#ifndef ENTITY_SYSTEM_H_
#define ENTITY_SYSTEM_H_

#include <vector>

#include <stdint.h>

class EntitySystem {
	std::vector<const char*> component_types;
	std::vector<std::vector<int32_t> > ids;

	std::vector<const char*> entities;
public:
	int32_t register_component(const char* component_type);

	int32_t create_entity(const char* entity_name);

	void add_component(int32_t entity, int32_t type, int32_t component);
	int32_t get_component(int32_t entity, int32_t type);
};

#endif /* ENTITY_SYSTEM_H_ */
