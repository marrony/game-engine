/*
 * ColladaSource.h
 *
 *  Created on: 20/04/2012
 *      Author: marrony.neris
 */

#ifndef COLLADASOURCE_H_
#define COLLADASOURCE_H_

#include "ColladaElement.h"

#include <string>

DECLARE_VISITOR(ColladaSource);

class ColladaSource : public ColladaElement {
	class ColladaIDRefArray* idRefArray;
	class ColladaNameArray* nameArray;
	class ColladaBoolArray* boolArray;
	class ColladaFloatArray* floatArray;
	class ColladaIntArray* intArray;
public:
	~ColladaSource();

	static std::string elementType();

	ColladaIDRefArray* getIdRefArray();

	ColladaNameArray* getNameArray();

	ColladaBoolArray* getBoolArray();

	ColladaFloatArray* getFloatArray();

	ColladaIntArray* getIntArray();

	virtual void loadFromXml(class TiXmlElement* element);

	virtual void accept(engine::Visitor* visitor);
};

#endif /* COLLADASOURCE_H_ */
