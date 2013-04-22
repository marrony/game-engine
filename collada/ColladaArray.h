/*
 * ColladaArray.h
 *
 *  Created on: 20/04/2012
 *      Author: marrony.neris
 */

#ifndef COLLADAARRAY_H_
#define COLLADAARRAY_H_

#include "ColladaElement.h"

#include <vector>

template<typename Element>
class ColladaArray : public ColladaElement {
	std::vector<Element> array;
public:
	virtual void loadFromXml(TiXmlElement* element) {
		ColladaElement::loadFromXml(element);

		loadArray(element, array, true);
	}

	std::vector<Element> getArray() {
		return array;
	}
};

DECLARE_VISITOR(ColladaIDRefArray);

class ColladaIDRefArray : public ColladaArray<std::string> {
public:
	static std::string elementType() {
		return "IDREF_array";
	}

	virtual void accept(engine::Visitor* visitor);
};

DECLARE_VISITOR(ColladaNameArray);

class ColladaNameArray : public ColladaArray<std::string> {
public:
	static std::string elementType() {
		return "Name_array";
	}

	virtual void accept(engine::Visitor* visitor);
};

DECLARE_VISITOR(ColladaBoolArray);

class ColladaBoolArray : public ColladaArray<bool> {
public:
	static std::string elementType() {
		return "bool_array";
	}

	virtual void accept(engine::Visitor* visitor);
};

DECLARE_VISITOR(ColladaFloatArray);

class ColladaFloatArray : public ColladaArray<float> {
public:
	static std::string elementType() {
		return "float_array";
	}

	virtual void accept(engine::Visitor* visitor);
};

DECLARE_VISITOR(ColladaIntArray);

class ColladaIntArray : public ColladaArray<int> {
public:
	static std::string elementType() {
		return "int_array";
	}

	virtual void accept(engine::Visitor* visitor);
};

#endif /* COLLADAARRAY_H_ */
