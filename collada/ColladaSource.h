/*
 * ColladaSource.h
 *
 *  Created on: 20/04/2012
 *      Author: marrony.neris
 */

#ifndef COLLADASOURCE_H_
#define COLLADASOURCE_H_

#include "ColladaElement.h"

DECLARE_VISITOR(ColladaSource);

class ColladaIDRefArray;
class ColladaNameArray;
class ColladaBoolArray;
class ColladaFloatArray;
class ColladaIntArray;

class ColladaSource : public ColladaElement {
	ColladaIDRefArray* idRefArray;
	ColladaNameArray* nameArray;
	ColladaBoolArray* boolArray;
	ColladaFloatArray* floatArray;
	ColladaIntArray* intArray;
public:
	~ColladaSource();

	static std::string elementType();

	ColladaIDRefArray* getIdRefArray();
	ColladaNameArray* getNameArray();
	ColladaBoolArray* getBoolArray();
	ColladaFloatArray* getFloatArray();
	ColladaIntArray* getIntArray();

	virtual void loadFromXml(class TiXmlElement* element);

	virtual void accept(Visitor* visitor);
};

#endif /* COLLADASOURCE_H_ */
