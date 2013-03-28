/*
 * json.cpp
 *
 *  Created on: 28/03/2013
 *      Author: marrony
 */

#include "json.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static void insert_token(Token*& tokens, int& count, int& capacity, int start, int end, int type) {
	if(count >= capacity) {
		capacity = (capacity * 3) / 2;

		tokens = (Token*)realloc(tokens, capacity * sizeof(Token));
	}

	Token* tok = &tokens[count++];

	tok->token = type;
	tok->start = start;
	tok->end = end;
}

void json_tokenize(const char* data, int data_lenght, Token*& tokens, int& count) {
	int old_begin = 0;
	int begin = 0;

	int capacity = data_lenght / 2;

	count = 0;
	tokens = (Token*)malloc(capacity * sizeof(Token));

	while(begin < data_lenght) {
		if(isalnum(data[begin])) {
			while(begin < data_lenght && isalnum(data[begin]))
				begin++;

			insert_token(tokens, count, capacity, old_begin, begin, TP_NUMBER);
		} else if(data[begin] == '\"') {
			begin++;
			while(begin < data_lenght && data[begin] != '\"')
				begin++;

			insert_token(tokens, count, capacity, old_begin+1, begin, TP_STRING);
			begin++;
		} else if(data[begin] == ',') {
			begin++;
			insert_token(tokens, count, capacity, old_begin, begin, ',');
		} else if(data[begin] == '{') {
			begin++;
			insert_token(tokens, count, capacity, old_begin, begin, '{');
		} else if(data[begin] == '}') {
			begin++;
			insert_token(tokens, count, capacity, old_begin, begin, '}');
		} else if(data[begin] == '[') {
			begin++;
			insert_token(tokens, count, capacity, old_begin, begin, '[');
		} else if(data[begin] == ']') {
			begin++;
			insert_token(tokens, count, capacity, old_begin, begin, ']');
		} else if(data[begin] == ':') {
			begin++;
			insert_token(tokens, count, capacity, old_begin, begin, ':');
		} else if(begin+4 < data_lenght && data[begin] == 't' && data[begin+1] == 'r' && data[begin+2] == 'u' && data[begin+3] == 'e') {
			begin += 4;
			insert_token(tokens, count, capacity, old_begin, begin, TP_TRUE);
		} else if(begin+5 < data_lenght && data[begin] == 'f' && data[begin+1] == 'a' && data[begin+2] == 'l' && data[begin+3] == 's' && data[begin+4] == 'e') {
			begin += 5;
			insert_token(tokens, count, capacity, old_begin, begin, TP_FALSE);
		} else if(begin+4 < data_lenght && data[begin] == 'n' && data[begin+1] == 'u' && data[begin+2] == 'l' && data[begin+3] == 'l') {
			begin += 4;
			insert_token(tokens, count, capacity, old_begin, begin, TP_NULL);
		} else {
			begin++;
		}
		old_begin = begin;
	}
}

static bool is_value(Token& token) {
	char type = token.token;

	return type == TP_STRING ||
			type == TP_NUMBER ||
			type == TP_TRUE ||
			type == TP_FALSE ||
			type == TP_NULL ||
			type == '[' ||
			type == '{';
}

static Array parse_array(const char* data, int data_lenght, int& begin, Token* tokens, int count);
static Object parse_object(const char* data, int data_lenght, int& begin, Token* tokens, int count);

static void parse_value(const char* data, int data_lenght, Value& value, int& begin, Token* tokens, int count) {
	switch(tokens[begin].token) {
	case TP_STRING: {
		int string_size = tokens[begin].end - tokens[begin].start;
		value.type = TP_STRING;
		value.string = (char*)malloc(string_size+1);
		strncpy(value.string, data+tokens[begin].start, string_size);
		value.string[string_size] = 0;

		begin++;
		break;
	}
	case TP_NUMBER:
		value.type = TP_NUMBER;
		value.number = atof(data+tokens[begin].start);
		begin++;
		break;
	case TP_TRUE:
		value.type = TP_TRUE;
		begin++;
		break;
	case TP_FALSE:
		value.type = TP_FALSE;
		begin++;
		break;
	case TP_NULL:
		value.type = TP_NULL;
		begin++;
		break;
	case '[':
		value.type = TP_ARRAY;
		value.array = parse_array(data, data_lenght, begin, tokens, count);
		break;
	case '{':
		value.type = TP_OBJECT;
		value.object = parse_object(data, data_lenght, begin, tokens, count);
		break;
	}
}

static Array parse_array(const char* data, int data_lenght, int& begin, Token* tokens, int count) {
	Array array = {0};

	begin++;

	while(begin < count) {
		if(!is_value(tokens[begin])) break;

		int index = array.size++;
		array.values = (Value*)realloc(array.values, sizeof(Value) * array.size);

		parse_value(data, data_lenght, array.values[index], begin, tokens, count);

		if(begin < count && tokens[begin].token != ',')
			break;

		begin++;
	}

	if(tokens[begin].token != ']') {
		fprintf(stderr, "parsing error, expected ]");
		return array;
	}

	begin++;

	return array;
}

static Object parse_object(const char* data, int data_lenght, int& begin, Token* tokens, int count) {
	Object object = {0};

	begin++;

	while(begin < count) {
		if(tokens[begin].token != TP_STRING) break;

		int index = object.size++;
		object.fields = (Pair*)realloc(object.fields, sizeof(Pair) * object.size);

		Value field_name;
		parse_value(data, data_lenght, field_name, begin, tokens, count);
		object.fields[index].name = field_name.string;

		if(begin < count && tokens[begin].token != ':') break;
		begin++;

		if(begin < count && !is_value(tokens[begin])) break;

		parse_value(data, data_lenght, object.fields[index].value, begin, tokens, count);

		if(begin < count && tokens[begin].token != ',')
			break;

		begin++;
	}

	if(tokens[begin].token != '}') {
		fprintf(stderr, "parsing error, expected }");
		return object;
	}

	begin++;

	return object;
}

Json json_parse(const char* data, int data_lenght, Token* tokens, int count) {
	Json json;

	int begin = 0;

	if(tokens[begin].token == '[') {
		json.value.type = TP_ARRAY;
		json.value.array = parse_array(data, data_lenght, begin, tokens, count);
	} else if(tokens[begin].token == '{') {
		json.value.type = TP_OBJECT;
		json.value.object = parse_object(data, data_lenght, begin, tokens, count);
	}

	return json;
}

Json json_parse(const char* data, int data_lenght) {
	Token* tokens;
	int count;
	json_tokenize(data, data_lenght, tokens, count);

	Json json = json_parse(data, sizeof(data), tokens, count);

	free(tokens);

	return json;
}

static void free_value(Value& v) {
	if(v.type == TP_ARRAY) {
		for(int i = 0; i < v.array.size; i++)
			free_value(v.array.values[i]);
		free(v.array.values);
	} else if(v.type == TP_OBJECT) {
		for(int i = 0; i < v.object.size; i++) {
			free(v.object.fields[i].name);
			free_value(v.object.fields[i].value);
		}
		free(v.object.fields);
	} else if(v.type == TP_STRING) {
		free(v.string);
	}
}

void json_free(Json& json) {
	free_value(json.value);
}
