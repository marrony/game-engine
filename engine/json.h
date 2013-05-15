/*
 * json.h
 *
 *  Created on: 28/03/2013
 *      Author: marrony
 */

#ifndef JSON_H_
#define JSON_H_

#include <stdint.h>
#include <stddef.h>

enum Type {
	TP_INVALID,
	TP_STRING,
	TP_INT,
	TP_NUMBER,
	TP_OBJECT,
	TP_ARRAY,
	TP_TRUE,
	TP_FALSE,
	TP_NULL,
};

struct Pair;
struct Value;

struct Object {
	int size;
	Pair* fields;
};

struct Array {
	int size;
	Value* values;
};

struct Value {
	Type type;
	union {
		int string;
		int integer;
		double number;
		Object object;
		Array array;
	};
};

struct Pair {
	int name;
	Value value;
};

struct Json {
	Value root;
	const char* data;
	int bytes_allocated;
	int bytes_used;
};

Json json_initialize(const char* data = 0, size_t size = 0);

Json json_parse(const char* data, int data_lenght, bool clone_data = false);
void json_free(Json& json);

Value json_clone(const Value& value);

Value* json_get_attribute(const Json& json, const Value& object, const char* attribute);
void json_set_attribute(Json& json, Value& object, const char* attribute, const Value& value);

Value* json_get_at(const Json& json, const Value& array, int index);
void json_set_at(Json& json, Value& array, int index, const Value& value);

Value json_create_string(Json& json, const char* value);
Value json_create_int(Json& json, int value);
Value json_create_number(Json& json, double value);
Value json_create_array(Json& json);
Value json_create_object(Json& json);
Value json_create_null(Json& json);
Value json_create_true(Json& json);
Value json_create_false(Json& json);

#define JSON_NULL          {TP_NULL, 0}
#define JSON_TRUE          {TP_TRUE, 0}
#define JSON_FALSE         {TP_FALSE, 0}
#define JSON_ARRAY         {TP_ARRAY, 0}
#define JSON_OBJECT        {TP_OBJECT, 0}
#define JSON_INT(value)    {TP_INT, value}
#define JSON_NUMBER(value) {TP_NUMBER, value}

#endif /* JSON_H_ */
