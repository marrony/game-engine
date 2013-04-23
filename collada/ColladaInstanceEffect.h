/*
 * ColladaInstanceEffect.h
 *
 *  Created on: 18/05/2012
 *      Author: objective
 */

#ifndef COLLADAINSTANCEEFFECT_H_
#define COLLADAINSTANCEEFFECT_H_

#include "ColladaElement.h"

DECLARE_VISITOR(ColladaInstanceEffect);

class ColladaInstanceEffect : public ColladaElement {
	std::string url;
public:
	static std::string elementType();

	virtual void loadFromXml(class TiXmlElement* element);

	virtual void accept(Visitor* visitor);

	const std::string& getUrl() const { return url; }
};

#endif /* COLLADAINSTANCEEFFECT_H_ */
