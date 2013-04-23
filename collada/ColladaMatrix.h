/*
 * ColladaMatrix.h
 *
 *  Created on: 18/05/2012
 *      Author: objective
 */

#ifndef COLLADAMATRIX_H_
#define COLLADAMATRIX_H_

#include "ColladaElement.h"
#include "ColladaUtil.h"

#include <vector>

class ColladaTransformation : public ColladaElement {
public:
};

DECLARE_VISITOR(ColladaMatrix);

class ColladaMatrix : public ColladaTransformation {
	float matrix[16];
public:
	static std::string elementType();

	virtual void loadFromXml(TiXmlElement* element);

	void getMatrix(float matrix[16]) const;

	virtual void accept(Visitor* visitor);
};

DECLARE_VISITOR(ColladaRotate);

class ColladaRotate : public ColladaTransformation {
	float axis[3];
	float angle;
public:
	static std::string elementType();

	virtual void loadFromXml(TiXmlElement* element);

	float getAngle() const { return angle; }

	void getAxis(float axis[3]);

	virtual void accept(Visitor* visitor);
};

DECLARE_VISITOR(ColladaScale);

class ColladaScale : public ColladaTransformation {
	float scale[3];
public:
	static std::string elementType();

	virtual void loadFromXml(TiXmlElement* element);

	void getScale(float scale[3]);

	virtual void accept(Visitor* visitor);
};

DECLARE_VISITOR(ColladaSkew);

class ColladaSkew : public ColladaTransformation {
	float angle;
	float rotation[3];
	float translation[3];
public:
	static std::string elementType();

	virtual void loadFromXml(TiXmlElement* element);

	virtual void accept(Visitor* visitor);
};

DECLARE_VISITOR(ColladaTranslate);

class ColladaTranslate : public ColladaTransformation {
	float translation[3];
public:
	static std::string elementType();

	virtual void loadFromXml(TiXmlElement* element);

	void getTranslation(float scale[3]);

	virtual void accept(Visitor* visitor);
};

#endif /* COLLADAMATRIX_H_ */
