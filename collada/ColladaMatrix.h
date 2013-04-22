/*
 * ColladaMatrix.h
 *
 *  Created on: 18/05/2012
 *      Author: objective
 */

#ifndef COLLADAMATRIX_H_
#define COLLADAMATRIX_H_

#include "ColladaElement.h"

#include <vector>

class ColladaTransformation : public ColladaElement {
public:
};

DECLARE_VISITOR(ColladaMatrix);

class ColladaMatrix : public ColladaTransformation {
	float matrix[16];
public:
	~ColladaMatrix() {
	}

	static std::string elementType() {
		return "matrix";
	}

	virtual void loadFromXml(TiXmlElement* element) {
		ColladaTransformation::loadFromXml(element);

		std::vector<float> values;
		loadArray(element, values);

		std::copy(values.begin(), values.end(), matrix);
	}

	virtual void accept(engine::Visitor* visitor);

	void getMatrix(float matrix[16]) const {
		for(int i = 0; i < 16; i++)
			matrix[i] = this->matrix[i];
	}
};

DECLARE_VISITOR(ColladaRotate);

class ColladaRotate : public ColladaTransformation {
	float axis[3];
	float angle;
public:
	~ColladaRotate() {
	}

	static std::string elementType() {
		return "rotate";
	}

	virtual void loadFromXml(TiXmlElement* element) {
		ColladaTransformation::loadFromXml(element);

		std::vector<float> values;
		loadArray(element, values);

		axis[0] = values[0];
		axis[1] = values[1];
		axis[2] = values[2];
		angle = values[3];
	}

	virtual void accept(engine::Visitor* visitor);

	float getAngle() const { return angle; }

	void getAxis(float axis[3]) {
		axis[0] = this->axis[0];
		axis[1] = this->axis[1];
		axis[2] = this->axis[2];
	}
};

DECLARE_VISITOR(ColladaScale);

class ColladaScale : public ColladaTransformation {
	float scale[3];
public:
	~ColladaScale() {
	}

	static std::string elementType() {
		return "scale";
	}

	virtual void loadFromXml(TiXmlElement* element) {
		ColladaTransformation::loadFromXml(element);

		std::vector<float> values;
		loadArray(element, values);

		scale[0] = values[0];
		scale[1] = values[1];
		scale[2] = values[2];
	}

	virtual void accept(engine::Visitor* visitor);

	void getScale(float scale[3]) {
		scale[0] = this->scale[0];
		scale[1] = this->scale[1];
		scale[2] = this->scale[2];
	}
};

DECLARE_VISITOR(ColladaSkew);

class ColladaSkew : public ColladaTransformation {
	float angle;
	float rotation[3];
	float translation[3];
public:
	~ColladaSkew() {
	}

	static std::string elementType() {
		return "skew";
	}

	virtual void loadFromXml(TiXmlElement* element) {
		ColladaTransformation::loadFromXml(element);

		std::vector<float> values;
		loadArray(element, values);

		angle = values[0];
		rotation[0] = values[1];
		rotation[1] = values[2];
		rotation[2] = values[3];
		translation[0] = values[4];
		translation[1] = values[5];
		translation[2] = values[6];
	}

	virtual void accept(engine::Visitor* visitor);
};

DECLARE_VISITOR(ColladaTranslate);

class ColladaTranslate : public ColladaTransformation {
	float translation[3];
public:
	~ColladaTranslate() {
	}

	static std::string elementType() {
		return "translate";
	}

	virtual void loadFromXml(TiXmlElement* element) {
		ColladaTransformation::loadFromXml(element);

		std::vector<float> values;
		loadArray(element, values);

		translation[0] = values[0];
		translation[1] = values[1];
		translation[2] = values[2];
	}

	virtual void accept(engine::Visitor* visitor);

	void getTranslation(float scale[3]) {
		translation[0] = this->translation[0];
		translation[1] = this->translation[1];
		translation[2] = this->translation[2];
	}
};

#endif /* COLLADAMATRIX_H_ */
