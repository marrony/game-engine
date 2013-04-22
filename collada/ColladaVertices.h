/*
 * ColladaVertices.h
 *
 *  Created on: 20/04/2012
 *      Author: marrony.neris
 */

#ifndef COLLADAVERTICES_H_
#define COLLADAVERTICES_H_

#include "ColladaElement.h"
#include "ColladaInput.h"

DECLARE_VISITOR(ColladaVertices);

class ColladaVertices : public ColladaElement {
	std::vector<ColladaInput*> inputs;
public:
	~ColladaVertices() {
		eraseVector(inputs);
	}

	static std::string elementType() {
		return "vertices";
	}

	virtual void loadFromXml(TiXmlElement* element) {
		ColladaElement::loadFromXml(element);

		inputs = createElementsFromXml<ColladaInput>(element);
		//TODO deve existir um input com semmantic=POSITION
	}

	virtual void accept(engine::Visitor* visitor);

	ColladaInput* findInputWithSemantic(const std::string& semantic) {
		std::vector<ColladaInput*>::iterator input;

		for(input = inputs.begin(); input != inputs.end(); input++) {
			if((*input)->getSemantic() == semantic) {
				return *input;
			}
		}

		return 0;
	}
};

#endif /* COLLADAVERTICES_H_ */
