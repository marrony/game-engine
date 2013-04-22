/*
 * ColladaBind.h
 *
 *  Created on: 18/05/2012
 *      Author: objective
 */

#ifndef COLLADABIND_H_
#define COLLADABIND_H_

#include "ColladaElement.h"

#include <string>

DECLARE_VISITOR(ColladaBind);

class ColladaBind : public ColladaElement {
	std::string semantic;
	std::string target;
public:
	static std::string elementType();

	virtual void loadFromXml(class TiXmlElement* element);

	virtual void accept(engine::Visitor* visitor);
};

#endif /* COLLADABIND_H_ */
