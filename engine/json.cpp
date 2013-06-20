/*
 * json.cpp
 *
 *  Created on: 28/03/2013
 *      Author: marrony
 */

#include "json.h"
#include "json_io.h"

#include <cstdlib>
#include <cstring>
#include <cctype>
#include <algorithm>
#include <sys/stat.h>

struct Token {
	int8_t token;
	int16_t start;
	int16_t end;
};

static int json_append_data(Json& json, const char* data) {
	size_t len = strlen(data) + 1;

	if(!json.data || json.bytes_allocated > 0) {
		if(json.bytes_used+len >= json.bytes_allocated) {
			json.bytes_allocated = (json.bytes_used+len) * 2;
			json.data = (const char*)realloc((void*)json.data, json.bytes_allocated);
		}
	}

	strncpy((char*)json.data+json.bytes_used, data, len);

	int offset = json.bytes_used;

	json.bytes_used += len;

	return offset;
}

static bool is_number(int ch) {
	return ch == '+' || ch == '-' || isdigit(ch);
}

static Token json_next_token(const Json& json, int& position) {

	int begin = position;
	int old_begin = begin;

	int type = TP_INVALID;
	int start = 0;
	int end = 0;

	while(begin < json.bytes_used) {
		if(is_number(json.data[begin])) {
			begin++;

			while(begin < json.bytes_used && isdigit(json.data[begin]))
				begin++;

			type = TP_INT;

			if(json.data[begin] == '.') {
				begin++;

				while(begin < json.bytes_used && isdigit(json.data[begin]))
					begin++;

				type = TP_NUMBER;
			}

			if(json.data[begin] == 'e' || json.data[begin] == 'E') {
				begin++;

				if(json.data[begin] == '-' || json.data[begin] == '+')
					begin++;

				while(begin < json.bytes_used && isdigit(json.data[begin]))
					begin++;

				type = TP_NUMBER;
			}

			//insert_token(tokens, count, capacity, old_begin, begin, type);
			start = old_begin;
			end = begin;
			break;
		} else if(json.data[begin] == '\"') {
			begin++;
			while(begin < json.bytes_used) {
				char ch = json.data[begin++];

				if(ch == '\\') {
					if(begin < json.bytes_used && (json.data[begin] == '\"' || json.data[begin] == '\\' || json.data[begin] == '/' ||
							json.data[begin] == 'b' || json.data[begin] == 'f' || json.data[begin] == 'n' ||
							json.data[begin] == 'r' || json.data[begin] == 't'))
						begin++;
				} else if(ch == '\"')
					break;
			}

			//insert_token(tokens, count, capacity, old_begin+1, begin-1, TP_STRING);
			type = TP_STRING;
			start = old_begin+1;
			end = begin-1;
			break;
		} else if(json.data[begin] == ',') {
			begin++;
			//insert_token(tokens, count, capacity, old_begin, begin, ',');
			type = ',';
			start = old_begin+1;
			end = begin-1;
			break;
		} else if(json.data[begin] == '{') {
			begin++;
			//insert_token(tokens, count, capacity, old_begin, begin, '{');
			type = '{';
			start = old_begin+1;
			end = begin-1;
			break;
		} else if(json.data[begin] == '}') {
			begin++;
			//insert_token(tokens, count, capacity, old_begin, begin, '}');
			type = '}';
			start = old_begin+1;
			end = begin-1;
			break;
		} else if(json.data[begin] == '[') {
			begin++;
			//insert_token(tokens, count, capacity, old_begin, begin, '[');
			type = '[';
			start = old_begin+1;
			end = begin-1;
			break;
		} else if(json.data[begin] == ']') {
			begin++;
			//insert_token(tokens, count, capacity, old_begin, begin, ']');
			type = ']';
			start = old_begin+1;
			end = begin-1;
			break;
		} else if(json.data[begin] == ':') {
			begin++;
			//insert_token(tokens, count, capacity, old_begin, begin, ':');
			type = ':';
			start = old_begin+1;
			end = begin-1;
			break;
		} else if(begin+4 < json.bytes_used && json.data[begin] == 't' && json.data[begin+1] == 'r' && json.data[begin+2] == 'u' && json.data[begin+3] == 'e') {
			begin += 4;
			//insert_token(tokens, count, capacity, old_begin, begin, TP_TRUE);
			type = TP_TRUE;
			start = old_begin+1;
			end = begin-1;
			break;
		} else if(begin+5 < json.bytes_used && json.data[begin] == 'f' && json.data[begin+1] == 'a' && json.data[begin+2] == 'l' && json.data[begin+3] == 's' && json.data[begin+4] == 'e') {
			begin += 5;
			//insert_token(tokens, count, capacity, old_begin, begin, TP_FALSE);
			type = TP_FALSE;
			start = old_begin+1;
			end = begin-1;
			break;
		} else if(begin+4 < json.bytes_used && json.data[begin] == 'n' && json.data[begin+1] == 'u' && json.data[begin+2] == 'l' && json.data[begin+3] == 'l') {
			begin += 4;
			//insert_token(tokens, count, capacity, old_begin, begin, TP_NULL);
			type = TP_NULL;
			start = old_begin+1;
			end = begin-1;
			break;
		} else {
			begin++;
		}
		old_begin = begin;
	}

	position = begin;

	Token token = {type, start, end};
	return token;
}

static bool is_value(Token& token) {
	char type = token.token;

	return type == TP_STRING ||
			type == TP_INT ||
			type == TP_NUMBER ||
			type == TP_TRUE ||
			type == TP_FALSE ||
			type == TP_NULL ||
			type == '[' ||
			type == '{';
}

static Array parse_array(const Json& json, int& begin);
static Object parse_object(const Json& json, int& begin);

static void parse_value(const Json& json, const Token& token, Value& value, int& begin) {
	switch(token.token) {
	case TP_STRING: {
		int string_size = token.end - token.start;
		value.type = TP_STRING;
		value.string = token.start;

		char* src_ptr = (char*)json.data+token.start;
		char* dst_ptr = (char*)json.data+token.start;
		char* end_ptr = (char*)json.data+token.start+string_size;

		while (src_ptr < end_ptr) {
			if(*src_ptr == '\\') {
				src_ptr++;

				if(*src_ptr == '\"')
					*dst_ptr++ = '\"';
				else if(*src_ptr == '\\')
					*dst_ptr++ = '\\';
				else if(*src_ptr == '/')
					*dst_ptr++ = 0xff; //TODO
				else if(*src_ptr == 'b')
					*dst_ptr++ = '\b';
				else if(*src_ptr == 'f')
					*dst_ptr++ = '\f';
				else if(*src_ptr == 'n')
					*dst_ptr++ = '\n';
				else if(*src_ptr == 'r')
					*dst_ptr++ = '\r';
				else if(*src_ptr == 't')
					*dst_ptr++ = '\t';

				src_ptr++;
			} else {
				*dst_ptr++ = *src_ptr++;
			}
		}
		*dst_ptr = 0;

		break;
	}
	case TP_INT:
		value.type = TP_INT;
		value.integer = atoi(json.data+token.start);
		break;
	case TP_NUMBER:
		value.type = TP_NUMBER;
		value.number = atof(json.data+token.start);
		break;
	case TP_TRUE:
		value.type = TP_TRUE;
		break;
	case TP_FALSE:
		value.type = TP_FALSE;
		break;
	case TP_NULL:
		value.type = TP_NULL;
		break;
	case '[':
		value.type = TP_ARRAY;
		value.array = parse_array(json, begin);
		break;
	case '{':
		value.type = TP_OBJECT;
		value.object = parse_object(json, begin);
		break;
	}
}

static Array parse_array(const Json& json, int& begin) {
	Array array = {0};

	Token token;

	while(is_value(token = json_next_token(json, begin))) {
		int index = array.size++;
		array.values = (Value*)realloc(array.values, sizeof(Value) * array.size);

		parse_value(json, token, array.values[index], begin);

		if(begin < json.bytes_used && (token = json_next_token(json, begin)).token != ',')
			break;
	}

	if(token.token != ']') {
		fprintf(stderr, "parsing error, expected ]");
		return array;
	}

	return array;
}

static Object parse_object(const Json& json, int& begin) {
	Object object = {0};

	Token token;

	while((token = json_next_token(json, begin)).token == TP_STRING) {
		int index = object.size++;
		object.fields = (Pair*)realloc(object.fields, sizeof(Pair) * object.size);

		Value field_name;
		parse_value(json, token, field_name, begin);
		object.fields[index].name = field_name.string;

		if(begin < json.bytes_used && (token = json_next_token(json, begin)).token != ':') break;

		if(begin < json.bytes_used && !is_value(token = json_next_token(json, begin))) break;

		parse_value(json, token, object.fields[index].value, begin);

		if(begin < json.bytes_used && (token = json_next_token(json, begin)).token != ',')
			break;
	}

	if(token.token != '}') {
		fprintf(stderr, "parsing error, expected }");
		return object;
	}

	return object;
}

Json json_parse(const char* data, int data_lenght, bool clone_data) {
	Json json = {{TP_INVALID, 0}};

	if(clone_data) {
		json.data = (const char*)malloc(data_lenght);
		strncpy((char*)json.data, data, data_lenght);
		json.bytes_allocated = data_lenght;
		json.bytes_used = data_lenght;
	} else {
		json.data = data;
		json.bytes_allocated = 0;
		json.bytes_used = data_lenght;
	}

	int begin = 0;
	Token token = json_next_token(json, begin);

	if(token.token == '[') {
		json.root.type = TP_ARRAY;
		json.root.array = parse_array(json, begin);
	} else if(token.token == '{') {
		json.root.type = TP_OBJECT;
		json.root.object = parse_object(json, begin);
	}

	return json;
}

static void free_value(Value& v) {
	if(v.type == TP_ARRAY) {
		for(int i = 0; i < v.array.size; i++)
			free_value(v.array.values[i]);
		free(v.array.values);
	} else if(v.type == TP_OBJECT) {
		for(int i = 0; i < v.object.size; i++) {
			free_value(v.object.fields[i].value);
		}
		free(v.object.fields);
	}
}

Json json_initialize(const char* data, size_t size) {
	Json json = {{TP_INVALID}, 0};

	json.data = data;

	return json;
}

void json_free(Json& json) {
	free_value(json.root);

	if(json.bytes_allocated > 0)
		free((void*)json.data);
}

Value json_clone(const Value& value) {
	Value cloned = {TP_INVALID, 0};

	if(value.type == TP_ARRAY) {
		cloned.type = TP_ARRAY;
		cloned.array.size = value.array.size;
		cloned.array.values = (Value*)malloc(sizeof(Value) * cloned.array.size);

		for(int i = 0; i < value.array.size; i++)
			cloned.array.values[i] = json_clone(value.array.values[i]);
	} else if(value.type == TP_OBJECT) {
		cloned.type = TP_OBJECT;
		cloned.object.size = value.array.size;
		cloned.object.fields = (Pair*)malloc(sizeof(Pair) * cloned.object.size);

		for(int i = 0; i < value.object.size; i++) {
			cloned.object.fields[i].name = value.object.fields[i].name;
			cloned.object.fields[i].value = json_clone(value.object.fields[i].value);
		}
	} else {
		cloned = value;
	}

	return cloned;
}

Value* json_get_attribute(const Json& json, const Value& object, const char* attribute) {
	if(object.type == TP_OBJECT) {
		for(int i = 0; i < object.object.size; i++) {
			if(!strcmp(json.data+object.object.fields[i].name, attribute))
				return &object.object.fields[i].value;
		}
	}

	return NULL;
}

void json_set_attribute(Json& json, Value& object, const char* attribute, const Value& value) {
	if(object.type == TP_OBJECT) {
		for(int i = 0; i < object.object.size; i++) {
			if(!strcmp(json.data+object.object.fields[i].name, attribute)) {
				object.object.fields[i].value = value;
				return;
			}
		}

		int index = object.object.size++;
		object.object.fields = (Pair*)realloc(object.object.fields, sizeof(Pair) * object.object.size);
		object.object.fields[index].name = json_append_data(json, attribute);
		object.object.fields[index].value = value;
	}
}

Value* json_get_at(const Json& json, const Value& array, int index) {
	if(array.type == TP_ARRAY && index < array.array.size)
		return &array.array.values[index];

	return NULL;
}

void json_set_at(Json& json, Value& array, int index, const Value& value) {
	if(array.type == TP_ARRAY) {
		if(index >= array.array.size) {
			array.array.size = std::max(index, array.array.size) + 1;
			array.array.values = (Value*)realloc(array.array.values, sizeof(Value) * array.array.size);
		}

		array.array.values[index] = value;
	}
}

Value json_create_string(Json& json, const char* str) {
	int offset = json_append_data(json, str);

	Value value = {TP_STRING, offset};

	return value;
}

Value json_create_int(Json& json, int value) {
	Value v = JSON_INT(value);
	return v;
}

Value json_create_number(Json& json, double value) {
	Value v; // = JSON_NUMBER(value);
	v.type = TP_NUMBER;
	v.number = value;
	return v;
}

Value json_create_array(Json& json) {
	Value v = JSON_ARRAY;
	return v;
}

Value json_create_object(Json& json) {
	Value v = JSON_OBJECT;
	return v;
}

Value json_create_null(Json& json) {
	Value v = JSON_NULL;
	return v;
}

Value json_create_true(Json& json) {
	Value v = JSON_TRUE;
	return v;
}

Value json_create_false(Json& json) {
	Value v = JSON_FALSE;
	return v;
}

#if defined(MSDOS) || defined(OS2) || defined(WIN32) || defined(__CYGWIN__)
#  include <fcntl.h>
#  include <io.h>
#  define SET_BINARY_MODE(file) setmode(fileno(file), O_BINARY)
#else
#  define SET_BINARY_MODE(file)
#endif

Json json_read(FILE* file) {
	struct stat st;

	//SET_BINARY_MODE(file);

	fstat(fileno(file), &st);

	char* data = (char*)malloc(st.st_size+1);
	int read = fread(data, sizeof(char), st.st_size, file);
	data[read] = 0;

	Json json = json_parse(data, read);

	free(data);

	return json;
}

#include <stdarg.h>

struct PrintfBuffer {
	char* buffer;
	char* begin;
	size_t size;

	void snprintf(const char* fmt, ...) {
		va_list list;
		va_start(list, fmt);

		int nbytes = vsnprintf(begin, size, fmt, list);
		if(nbytes != -1) {
			begin += nbytes;
			size -= nbytes;
		}

		va_end(list);
	}
};

static void json_print_ident(PrintfBuffer& buffer, int ident) {
	for(int i = 0; i < ident; i++)
		buffer.snprintf("\t");
}

static void json_print(const Json& json, PrintfBuffer& buffer, const Value* value, int ident) {
	switch(value->type) {
	case TP_STRING:
		buffer.snprintf("\"%s\"", json.data+value->string);
		break;
	case TP_INT:
		buffer.snprintf("%d", value->integer);
		break;
	case TP_NUMBER:
		buffer.snprintf("%f", value->number);
		break;
	case TP_OBJECT:
		buffer.snprintf("{");
		//buffer.snprintf("\n");
		for(int i = 0; i < value->object.size; i++) {
			//json_print_ident(buffer, ident+1);
			buffer.snprintf("\"%s\": ", json.data+value->object.fields[i].name);
			json_print(json, buffer, &value->object.fields[i].value, ident+1);
			if(i+1 < value->object.size) {
				buffer.snprintf(",");
				//buffer.snprintf("\n");
			}
		}
		//buffer.snprintf("\n");
		//json_print_ident(fp, ident);
		buffer.snprintf("}");
		break;
	case TP_ARRAY:
		buffer.snprintf("[");
		//buffer.snprintf("\n");
		for(int i = 0; i < value->array.size; i++) {
			//json_print_ident(fp, ident+1);
			json_print(json, buffer, &value->array.values[i], ident+1);
			if(i+1 < value->array.size) {
				buffer.snprintf(",");
				//buffer.snprintf("\n");
			}
		}
		//buffer.snprintf("\n");
		//json_print_ident(ident);
		buffer.snprintf("]");
		break;
	case TP_TRUE:
		buffer.snprintf("true");
		break;
	case TP_FALSE:
		buffer.snprintf("false");
		break;
	case TP_NULL:
		buffer.snprintf("null");
		break;
	}
}

void json_write(const Json& json, FILE* file) {
	//SET_BINARY_MODE(file);
	//json_print(json, file, &json.root, 0);
}

int json_serialize(const Json& json, char* buffer, size_t size) {
	PrintfBuffer printf_buffer = {buffer, buffer, size};

	json_print(json, printf_buffer, &json.root, 0);

	int nbytes = printf_buffer.begin - printf_buffer.buffer;

	buffer[nbytes] = 0;

	return nbytes;
}
