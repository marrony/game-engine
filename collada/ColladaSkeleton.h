/*
 * ColladaSkeleton.h
 *
 *  Created on: 18/05/2012
 *      Author: objective
 */

#ifndef COLLADASKELETON_H_
#define COLLADASKELETON_H_

#include "ColladaElement.h"

DECLARE_VISITOR(ColladaSkeleton);

class ColladaSkeleton : public ColladaElement {
	std::string uri;
public:
	static std::string elementType();

	virtual void loadFromXml(TiXmlElement* element);

	virtual void accept(Visitor* visitor);
};

#endif /* COLLADASKELETON_H_ */
