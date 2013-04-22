/*
 * ColladaInstanceMaterial.h
 *
 *  Created on: 18/05/2012
 *      Author: objective
 */

#ifndef COLLADAINSTANCEMATERIAL_H_
#define COLLADAINSTANCEMATERIAL_H_

#include "ColladaElement.h"

#include <string>

DECLARE_VISITOR(ColladaInstanceMaterial);

class ColladaInstanceMaterial : public ColladaElement {
	std::string target;
	std::string symbol;
public:
	static std::string elementType();

	virtual void loadFromXml(TiXmlElement* element);

	virtual void accept(engine::Visitor* visitor);

	const std::string& getTarget() const { return target; }
	const std::string& getSymbol() const { return symbol; }
};

#endif /* COLLADAINSTANCEMATERIAL_H_ */
