/*
 * json.cpp
 *
 *  Created on: 28/03/2013
 *      Author: marrony
 */

#include "json.h"

#include <stdlib.h>
#include <cstring>
#include <cctype>
#include <algorithm>
#include <sys/stat.h>

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

static bool is_number(int ch) {
	return ch == '+' || ch == '-' || isdigit(ch);
}

void json_tokenize(const char* data, int data_lenght, Token*& tokens, int& count) {
	int old_begin = 0;
	int begin = 0;

	int capacity = data_lenght / 2;

	count = 0;
	tokens = (Token*)malloc(capacity * sizeof(Token));

	while(begin < data_lenght) {
		if(is_number(data[begin])) {
			begin++;

			while(begin < data_lenght && isdigit(data[begin]))
				begin++;

			int type = TP_INT;

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

			insert_token(tokens, count, capacity, old_begin, begin, type);
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

			insert_token(tokens, count, capacity, old_begin+1, begin-1, TP_STRING);
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
			type == TP_INT ||
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

		int end = tokens[begin].start + string_size;
		int src = tokens[begin].start;
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

		begin++;
		break;
	}
	case TP_INT:
		value.type = TP_INT;
		value.integer = atoi(data+tokens[begin].start);
		begin++;
		break;
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
	Json json = {{TP_INVALID, 0}};

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
	Token* tokens = 0;
	int count = 0;

	json_tokenize(data, data_lenght, tokens, count);
	Json json = json_parse(data, data_lenght, tokens, count);

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
