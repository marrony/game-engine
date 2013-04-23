/*
 * ColladaEffectSurface.h
 *
 *  Created on: 18/05/2012
 *      Author: objective
 */

#ifndef COLLADAEFFECTSURFACE_H_
#define COLLADAEFFECTSURFACE_H_

#include "ColladaElement.h"
#include "ColladaUtil.h"

DECLARE_VISITOR(ColladaInitFrom);

class ColladaInitFrom : public ColladaElement {
	std::string value;
public:
	static std::string elementType();

	virtual void loadFromXml(TiXmlElement* element);

	virtual void accept(Visitor* visitor);

	const std::string& getValue() const { return value; }
};

DECLARE_VISITOR(ColladaEffectSurface);

class ColladaEffectSurface : public ColladaElement {
	std::string type;
	ColladaInitFrom* initFrom;
public:
	virtual ~ColladaEffectSurface();

	static std::string elementType();

	virtual void loadFromXml(TiXmlElement* element);

	virtual void accept(Visitor* visitor);

	const std::string& getType() const { return type; }
	ColladaInitFrom* getInitFrom() const { return initFrom; }
};


#endif /* COLLADAEFFECTSURFACE_H_ */
