/*
 * ColladaInput.h
 *
 *  Created on: 20/04/2012
 *      Author: marrony.neris
 */

#ifndef COLLADAINPUT_H_
#define COLLADAINPUT_H_

#include "ColladaElement.h"

DECLARE_VISITOR(ColladaInput);

class ColladaInput : public ColladaElement {
	std::string semantic;
	std::string source;
	std::string offset;
	std::string set;
public:
	static std::string elementType();

	virtual void loadFromXml(class TiXmlElement* element);

	virtual void accept(engine::Visitor* visitor);

	const std::string& getSemantic();

	class ColladaSource* getSourceElement();

	int getOffset();

	int getSet();
};

#endif /* COLLADAINPUT_H_ */
