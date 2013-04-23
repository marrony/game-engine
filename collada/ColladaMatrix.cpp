/*
 * ColladaMatrix.cpp
 *
 *  Created on: 18/05/2012
 *      Author: objective
 */

#include "ColladaMatrix.h"

DEFINE_VISITOR(ColladaMatrix)

std::string ColladaMatrix::elementType() {
	return "matrix";
}

void ColladaMatrix::loadFromXml(TiXmlElement* element) {
	ColladaTransformation::loadFromXml(element);

	std::vector<float> values;
	ColladaUtil::loadArray(element, values);

	std::copy(values.begin(), values.end(), matrix);
}

void ColladaMatrix::getMatrix(float matrix[16]) const {
	for(int i = 0; i < 16; i++)
		matrix[i] = this->matrix[i];
}

DEFINE_VISITOR(ColladaRotate)

std::string ColladaRotate::elementType() {
	return "rotate";
}

void ColladaRotate::loadFromXml(TiXmlElement* element) {
	ColladaTransformation::loadFromXml(element);

	std::vector<float> values;
	ColladaUtil::loadArray(element, values);

	axis[0] = values[0];
	axis[1] = values[1];
	axis[2] = values[2];
	angle = values[3];
}

void ColladaRotate::getAxis(float axis[3]) {
	axis[0] = this->axis[0];
	axis[1] = this->axis[1];
	axis[2] = this->axis[2];
}

DEFINE_VISITOR(ColladaScale)

std::string ColladaScale::elementType() {
	return "scale";
}

void ColladaScale::loadFromXml(TiXmlElement* element) {
	ColladaTransformation::loadFromXml(element);

	std::vector<float> values;
	ColladaUtil::loadArray(element, values);

	scale[0] = values[0];
	scale[1] = values[1];
	scale[2] = values[2];
}

void ColladaScale::getScale(float scale[3]) {
	scale[0] = this->scale[0];
	scale[1] = this->scale[1];
	scale[2] = this->scale[2];
}

DEFINE_VISITOR(ColladaSkew)

std::string ColladaSkew::elementType() {
	return "skew";
}

void ColladaSkew::loadFromXml(TiXmlElement* element) {
	ColladaTransformation::loadFromXml(element);

	std::vector<float> values;
	ColladaUtil::loadArray(element, values);

	angle = values[0];
	rotation[0] = values[1];
	rotation[1] = values[2];
	rotation[2] = values[3];
	translation[0] = values[4];
	translation[1] = values[5];
	translation[2] = values[6];
}

DEFINE_VISITOR(ColladaTranslate)

std::string ColladaTranslate::elementType() {
	return "translate";
}

void ColladaTranslate::loadFromXml(TiXmlElement* element) {
	ColladaTransformation::loadFromXml(element);

	std::vector<float> values;
	ColladaUtil::loadArray(element, values);

	translation[0] = values[0];
	translation[1] = values[1];
	translation[2] = values[2];
}

void ColladaTranslate::getTranslation(float scale[3]) {
	translation[0] = this->translation[0];
	translation[1] = this->translation[1];
	translation[2] = this->translation[2];
}
