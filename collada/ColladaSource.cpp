/*
 * ColladaSource.cpp
 *
 *  Created on: 20/04/2012
 *      Author: marrony.neris
 */

#include "ColladaSource.h"
#include "ColladaArray.h"
#include "ColladaUtil.h"

DEFINE_VISITOR(ColladaSource)

ColladaSource::~ColladaSource() {
		delete idRefArray;
		delete nameArray;
		delete boolArray;
		delete floatArray;
		delete intArray;
	}

std::string ColladaSource::elementType() {
	return "source";
}

ColladaIDRefArray* ColladaSource::getIdRefArray() {
	return idRefArray;
}

ColladaNameArray* ColladaSource::getNameArray() {
	return nameArray;
}

ColladaBoolArray* ColladaSource::getBoolArray() {
	return boolArray;
}

ColladaFloatArray* ColladaSource::getFloatArray() {
	return floatArray;
}

ColladaIntArray* ColladaSource::getIntArray() {
	return intArray;
}

void ColladaSource::loadFromXml(TiXmlElement* element) {
	ColladaElement::loadFromXml(element);

	idRefArray = ColladaUtil::createElementFromXml<ColladaIDRefArray>(this, element);
	nameArray = ColladaUtil::createElementFromXml<ColladaNameArray>(this, element);
	boolArray = ColladaUtil::createElementFromXml<ColladaBoolArray>(this, element);
	floatArray = ColladaUtil::createElementFromXml<ColladaFloatArray>(this, element);
	intArray = ColladaUtil::createElementFromXml<ColladaIntArray>(this, element);
}
