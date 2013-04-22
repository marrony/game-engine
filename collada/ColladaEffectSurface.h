/*
 * ColladaEffectSurface.h
 *
 *  Created on: 18/05/2012
 *      Author: objective
 */

#ifndef COLLADAEFFECTSURFACE_H_
#define COLLADAEFFECTSURFACE_H_

#include "ColladaElement.h"

DECLARE_VISITOR(ColladaInitFrom);

class ColladaInitFrom : public ColladaElement {
	std::string value;
public:
	static std::string elementType() {
		return "init_from";
	}

	virtual void loadFromXml(TiXmlElement* element) {
		ColladaElement::loadFromXml(element);

		const char* v = element->Value();
		if(v)
			value = v;
	}

	virtual void accept(engine::Visitor* visitor);

	const std::string& getValue() const { return value; }
};

DECLARE_VISITOR(ColladaEffectSurface);

class ColladaEffectSurface : public ColladaElement {
	std::string type;
	ColladaInitFrom* initFrom;
public:
	virtual ~ColladaEffectSurface() {
		delete initFrom;
	}

	static std::string elementType() {
		return "surface";
	}

	virtual void loadFromXml(TiXmlElement* element) {
		ColladaElement::loadFromXml(element);

		loadAttribute(element, "type", type);

		initFrom = createElementFromXml<ColladaInitFrom>(element);
	}

	virtual void accept(engine::Visitor* visitor);

	const std::string& getType() const { return type; }
	ColladaInitFrom* getInitFrom() const { return initFrom; }
};


#endif /* COLLADAEFFECTSURFACE_H_ */
