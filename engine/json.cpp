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

static bool is_number(int ch) {
	return ch == '+' || ch == '-' || isdigit(ch);
}

static Token json_next_token(const char* data, int data_lenght, int& position) {

	int begin = position;
	int old_begin = begin;

	int type = TP_INVALID;
	int start = 0;
	int end = 0;

	while(begin < data_lenght) {
		if(is_number(data[begin])) {
			begin++;

			while(begin < data_lenght && isdigit(data[begin]))
				begin++;

			type = TP_INT;

			if(data[begin] == '.') {
				begin++;

				while(begin < data_lenght && isdigit(data[begin]))
					begin++;

				type = TP_NUMBER;
			}

			if(data[begin] == 'e' || data[begin] == 'E') {
				begin++;

				if(data[begin] == '-' || data[begin] == '+')
					begin++;

				while(begin < data_lenght && isdigit(data[begin]))
					begin++;

				type = TP_NUMBER;
			}

			//insert_token(tokens, count, capacity, old_begin, begin, type);
			start = old_begin;
			end = begin;
			break;
		} else if(data[begin] == '\"') {
			begin++;
			while(begin < data_lenght) {
				char ch = data[begin++];

				if(ch == '\\') {
					if(begin < data_lenght && (data[begin] == '\"' || data[begin] == '\\' || data[begin] == '/' ||
						data[begin] == 'b' || data[begin] == 'f' || data[begin] == 'n' ||
						data[begin] == 'r' || data[begin] == 't'))
						begin++;
				} else if(ch == '\"')
					break;
			}

			//insert_token(tokens, count, capacity, old_begin+1, begin-1, TP_STRING);
			type = TP_STRING;
			start = old_begin+1;
			end = begin-1;
			break;
		} else if(data[begin] == ',') {
			begin++;
			//insert_token(tokens, count, capacity, old_begin, begin, ',');
			type = ',';
			start = old_begin+1;
			end = begin-1;
			break;
		} else if(data[begin] == '{') {
			begin++;
			//insert_token(tokens, count, capacity, old_begin, begin, '{');
			type = '{';
			start = old_begin+1;
			end = begin-1;
			break;
		} else if(data[begin] == '}') {
			begin++;
			//insert_token(tokens, count, capacity, old_begin, begin, '}');
			type = '}';
			start = old_begin+1;
			end = begin-1;
			break;
		} else if(data[begin] == '[') {
			begin++;
			//insert_token(tokens, count, capacity, old_begin, begin, '[');
			type = '[';
			start = old_begin+1;
			end = begin-1;
			break;
		} else if(data[begin] == ']') {
			begin++;
			//insert_token(tokens, count, capacity, old_begin, begin, ']');
			type = ']';
			start = old_begin+1;
			end = begin-1;
			break;
		} else if(data[begin] == ':') {
			begin++;
			//insert_token(tokens, count, capacity, old_begin, begin, ':');
			type = ':';
			start = old_begin+1;
			end = begin-1;
			break;
		} else if(begin+4 < data_lenght && data[begin] == 't' && data[begin+1] == 'r' && data[begin+2] == 'u' && data[begin+3] == 'e') {
			begin += 4;
			//insert_token(tokens, count, capacity, old_begin, begin, TP_TRUE);
			type = TP_TRUE;
			start = old_begin+1;
			end = begin-1;
			break;
		} else if(begin+5 < data_lenght && data[begin] == 'f' && data[begin+1] == 'a' && data[begin+2] == 'l' && data[begin+3] == 's' && data[begin+4] == 'e') {
			begin += 5;
			//insert_token(tokens, count, capacity, old_begin, begin, TP_FALSE);
			type = TP_FALSE;
			start = old_begin+1;
			end = begin-1;
			break;
		} else if(begin+4 < data_lenght && data[begin] == 'n' && data[begin+1] == 'u' && data[begin+2] == 'l' && data[begin+3] == 'l') {
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

static Array parse_array(const char* data, int data_lenght, int& begin);
static Object parse_object(const char* data, int data_lenght, int& begin);

static void parse_value(const char* data, int data_lenght, const Token& token, Value& value, int& begin) {
	switch(token.token) {
	case TP_STRING: {
		int string_size = token.end - token.start;
		value.type = TP_STRING;
		value.string = (char*)malloc(string_size+1);

		int end = token.start + string_size;
		int src = token.start;
		int dst = 0;
		while (src < end) {
			if(data[src] == '\\') {
				src++;

				if(data[src] == '\"')
					value.string[dst++] = '\"';
				else if(data[src] == '\\')
					value.string[dst++] = '\\';
				else if(data[src] == '/')
					value.string[dst++] = 0xff; //TODO
				else if(data[src] == 'b')
					value.string[dst++] = '\b';
				else if(data[src] == 'f')
					value.string[dst++] = '\f';
				else if(data[src] == 'n')
					value.string[dst++] = '\n';
				else if(data[src] == 'r')
					value.string[dst++] = '\r';
				else if(data[src] == 't')
					value.string[dst++] = '\t';

				src++;
			} else {
				value.string[dst++] = data[src++];
			}
		}
		value.string[dst] = 0;

		break;
	}
	case TP_INT:
		value.type = TP_INT;
		value.integer = atoi(data+token.start);
		break;
	case TP_NUMBER:
		value.type = TP_NUMBER;
		value.number = atof(data+token.start);
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
		value.array = parse_array(data, data_lenght, begin);
		break;
	case '{':
		value.type = TP_OBJECT;
		value.object = parse_object(data, data_lenght, begin);
		break;
	}
}

static Array parse_array(const char* data, int data_lenght, int& begin) {
	Array array = {0};

	Token token;

	while(is_value(token = json_next_token(data, data_lenght, begin))) {
		int index = array.size++;
		array.values = (Value*)realloc(array.values, sizeof(Value) * array.size);

		parse_value(data, data_lenght, token, array.values[index], begin);

		if(begin < data_lenght && (token = json_next_token(data, data_lenght, begin)).token != ',')
			break;
	}

	if(token.token != ']') {
		fprintf(stderr, "parsing error, expected ]");
		return array;
	}

	return array;
}

static Object parse_object(const char* data, int data_lenght, int& begin) {
	Object object = {0};

	Token token;

	while((token = json_next_token(data, data_lenght, begin)).token == TP_STRING) {
		int index = object.size++;
		object.fields = (Pair*)realloc(object.fields, sizeof(Pair) * object.size);

		Value field_name;
		parse_value(data, data_lenght, token, field_name, begin);
		object.fields[index].name = field_name.string;

		if(begin < data_lenght && (token = json_next_token(data, data_lenght, begin)).token != ':') break;

		if(begin < data_lenght && !is_value(token = json_next_token(data, data_lenght, begin))) break;

		parse_value(data, data_lenght, token, object.fields[index].value, begin);

		if(begin < data_lenght && (token = json_next_token(data, data_lenght, begin)).token != ',')
			break;
	}

	if(token.token != '}') {
		fprintf(stderr, "parsing error, expected }");
		return object;
	}

	return object;
}

Json json_parse(const char* data, int data_lenght) {
	Json json = {{TP_INVALID, 0}};

	int begin = 0;
	Token token = json_next_token(data, data_lenght, begin);

	if(token.token == '[') {
		json.value.type = TP_ARRAY;
		json.value.array = parse_array(data, data_lenght, begin);
	} else if(token.token == '{') {
		json.value.type = TP_OBJECT;
		json.value.object = parse_object(data, data_lenght, begin);
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
			cloned.object.fields[i].name = strdup(value.object.fields[i].name);
			cloned.object.fields[i].value = json_clone(value.object.fields[i].value);
		}
	} else if(value.type == TP_STRING) {
		cloned.type = TP_STRING;
		cloned.string = strdup(value.string);
	} else {
		cloned = value;
	}

	return cloned;
}

Value* json_get_attribute(const Value& object, const char* attribute) {
	if(object.type == TP_OBJECT) {
		for(int i = 0; i < object.object.size; i++) {
			if(!strcmp(object.object.fields[i].name, attribute))
				return &object.object.fields[i].value;
		}
	}

	return NULL;
}

void json_set_attribute(Value& object, const char* attribute, const Value& value) {
	if(object.type == TP_OBJECT) {
		for(int i = 0; i < object.object.size; i++) {
			if(!strcmp(object.object.fields[i].name, attribute)) {
				object.object.fields[i].value = value;
				return;
			}
		}

		int index = object.object.size++;
		object.object.fields = (Pair*)realloc(object.object.fields, sizeof(Pair) * object.object.size);

		object.object.fields[index].name = strdup(attribute);
		object.object.fields[index].value = value;
	}
}

Value* json_get_at(const Value& array, int index) {
	if(array.type == TP_ARRAY && index < array.array.size)
		return &array.array.values[index];

	return NULL;
}

void json_set_at(Value& array, int index, const Value& value) {
	if(array.type == TP_ARRAY) {
		if(index >= array.array.size) {
			array.array.size = std::max(index + 1, array.array.size + 1);
			array.array.values = (Value*)realloc(array.array.values, sizeof(Value) * array.array.size);
		}

		array.array.values[index] = value;
	}
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

static void json_print_ident(FILE* fp, int ident) {
	for(int i = 0; i < ident; i++)
		fprintf(fp, "\t");
}

static void json_print(FILE* fp, const Value* value, int ident) {
	switch(value->type) {
	case TP_STRING:
		fprintf(fp, "\"%s\"", value->string);
		break;
	case TP_INT:
		fprintf(fp, "%d", value->integer);
		break;
	case TP_NUMBER:
		fprintf(fp, "%lf", value->number);
		break;
	case TP_OBJECT:
		fprintf(fp, "{\n");
		for(int i = 0; i < value->object.size; i++) {
			json_print_ident(fp, ident+1); fprintf(fp, "\"%s\": ", value->object.fields[i].name);
			json_print(fp, &value->object.fields[i].value, ident+1);
			if(i+1 < value->object.size)
				fprintf(fp, ",\n");
		}
		fprintf(fp, "\n");
		json_print_ident(fp, ident); fprintf(fp, "}");
		break;
	case TP_ARRAY:
		fprintf(fp, "[\n");
		for(int i = 0; i < value->array.size; i++) {
			json_print_ident(fp, ident+1); json_print(fp, &value->array.values[i], ident+1);
			if(i+1 < value->array.size)
				fprintf(fp, ",\n");
		}
		fprintf(fp, "\n");
		json_print_ident(fp, ident); fprintf(fp, "]");
		break;
	case TP_TRUE:
		fprintf(fp, "true");
		break;
	case TP_FALSE:
		fprintf(fp, "false");
		break;
	case TP_NULL:
		fprintf(fp, "null");
		break;
	}
}

void json_write(FILE* file, const Json& json) {
	//SET_BINARY_MODE(file);
	json_print(file, &json.value, 0);
}
