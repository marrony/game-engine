/*
 * ColladaBindVertexInput.h
 *
 *  Created on: 18/05/2012
 *      Author: objective
 */

#ifndef COLLADABINDVERTEXINPUT_H_
#define COLLADABINDVERTEXINPUT_H_

#include "ColladaElement.h"

#include <string>

DECLARE_VISITOR(ColladaBindVertexInput);

class ColladaBindVertexInput : public ColladaElement {
	std::string semantic;
	std::string inputSemantic;
	int inputSet;
public:
	static std::string elementType();

	virtual void loadFromXml(class TiXmlElement* element);

	virtual void accept(engine::Visitor* visitor);
};

#endif /* COLLADABINDVERTEXINPUT_H_ */
