/*
 * ColladaBindMaterial.h
 *
 *  Created on: 18/05/2012
 *      Author: objective
 */

#ifndef COLLADABINDMATERIAL_H_
#define COLLADABINDMATERIAL_H_

#include "ColladaElement.h"
#include "ColladaInstanceMaterial.h"

#include <vector>

DECLARE_VISITOR(ColladaTechniqueCommon);

class ColladaTechniqueCommon : public ColladaElement {
	std::vector<ColladaInstanceMaterial*> instanceMaterial;
public:
	~ColladaTechniqueCommon() {
		eraseVector(instanceMaterial);
	}

	static std::string elementType() {
		return "technique_common";
	}

	virtual void loadFromXml(TiXmlElement* element) {
		ColladaElement::loadFromXml(element);

		instanceMaterial = createElementsFromXml<ColladaInstanceMaterial>(element);
	}

	virtual void accept(engine::Visitor* visitor);

	const std::vector<ColladaInstanceMaterial*>& getInstanceMaterial() const { return instanceMaterial; }
};

DECLARE_VISITOR(ColladaBindMaterial);

class ColladaBindMaterial : public ColladaElement {
	ColladaTechniqueCommon* techniqueCommon;
public:
	~ColladaBindMaterial() {
		delete techniqueCommon;
	}

	static std::string elementType() {
		return "bind_material";
	}

	virtual void loadFromXml(TiXmlElement* element) {
		ColladaElement::loadFromXml(element);

		techniqueCommon = createElementFromXml<ColladaTechniqueCommon>(element);
	}

	virtual void accept(engine::Visitor* visitor);

	ColladaTechniqueCommon* getTechniqueCommon() const { return techniqueCommon; }
};

#endif /* COLLADABINDMATERIAL_H_ */
