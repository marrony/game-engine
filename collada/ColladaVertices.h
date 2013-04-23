/*
 * ColladaVertices.h
 *
 *  Created on: 20/04/2012
 *      Author: marrony.neris
 */

#ifndef COLLADAVERTICES_H_
#define COLLADAVERTICES_H_

#include "ColladaElement.h"

DECLARE_VISITOR(ColladaVertices);

class ColladaInput;

class ColladaVertices : public ColladaElement {
	std::vector<ColladaInput*> inputs;
public:
	~ColladaVertices();

	static std::string elementType();

	virtual void loadFromXml(TiXmlElement* element);

	ColladaInput* findInputWithSemantic(const std::string& semantic);

	virtual void accept(Visitor* visitor);
};

#endif /* COLLADAVERTICES_H_ */
