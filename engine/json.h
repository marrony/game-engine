/*
 * json.h
 *
 *  Created on: 28/03/2013
 *      Author: marrony
 */

#ifndef JSON_H_
#define JSON_H_

#include <stdint.h>
#include <stdio.h>

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
		char* string;
		int integer;
		double number;
		Object object;
		Array array;
	};
};

struct Pair {
	char* name;
	Value value;
};

struct Token {
	int8_t token;
	int16_t start;
	int16_t end;
};

struct Json {
	Value value;
};

void json_tokenize(const char* data, int data_lenght, Token*& tokens, int& count);
Json json_parse(const char* data, int data_lenght, Token* tokens, int count);
Json json_parse(const char* data, int data_lenght);
void json_free(Json& json);

Value json_clone(const Value& value);

Value* json_get_attribute(const Value& object, const char* attribute);
void json_set_attribute(Value& object, const char* attribute, const Value& value);

Value* json_get_at(const Value& array, int index);
void json_set_at(Value& array, int index, const Value& value);

Json json_read(FILE* file);
void json_write(FILE* file, const Json& json);

#endif /* JSON_H_ */