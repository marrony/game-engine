/*
 * json_io.h
 *
 *  Created on: 09/04/2013
 *      Author: marrony
 */

#ifndef JSON_IO_H_
#define JSON_IO_H_

#include <stdio.h>

Json json_read(FILE* file);
void json_write(const Json& json, FILE* file);

int json_serialize(const Json& json, char* buffer, size_t size);

#endif /* JSON_IO_H_ */
