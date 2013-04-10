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
void json_write(FILE* file, const Json& json);

#endif /* JSON_IO_H_ */
