/*
 * ColladaArray.cpp
 *
 *  Created on: 20/04/2012
 *      Author: marrony.neris
 */

#include "ColladaArray.h"

DEFINE_VISITOR(ColladaIDRefArray)

std::string ColladaIDRefArray::elementType() {
	return "IDREF_array";
}

DEFINE_VISITOR(ColladaNameArray)

std::string ColladaNameArray::elementType() {
	return "Name_array";
}

DEFINE_VISITOR(ColladaBoolArray)

std::string ColladaBoolArray::elementType() {
	return "bool_array";
}

DEFINE_VISITOR(ColladaFloatArray)

std::string ColladaFloatArray::elementType() {
	return "float_array";
}

DEFINE_VISITOR(ColladaIntArray)

std::string ColladaIntArray::elementType() {
	return "int_array";
}
