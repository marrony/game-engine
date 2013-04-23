/*
 * ColladaBindMaterial.h
 *
 *  Created on: 18/05/2012
 *      Author: objective
 */

#ifndef COLLADABINDMATERIAL_H_
#define COLLADABINDMATERIAL_H_

#include "ColladaElement.h"

#include <vector>

DECLARE_VISITOR(ColladaTechniqueCommon);

class ColladaInstanceMaterial;

class ColladaTechniqueCommon : public ColladaElement {
	std::vector<ColladaInstanceMaterial*> instanceMaterial;
public:
	~ColladaTechniqueCommon();

	static std::string elementType();

	virtual void loadFromXml(TiXmlElement* element);

	virtual void accept(Visitor* visitor);

	const std::vector<ColladaInstanceMaterial*>& getInstanceMaterial() const { return instanceMaterial; }
};

DECLARE_VISITOR(ColladaBindMaterial);

class ColladaBindMaterial : public ColladaElement {
	ColladaTechniqueCommon* techniqueCommon;
public:
	~ColladaBindMaterial();

	static std::string elementType();

	virtual void loadFromXml(TiXmlElement* element);

	virtual void accept(Visitor* visitor);

	ColladaTechniqueCommon* getTechniqueCommon() const { return techniqueCommon; }
};

#endif /* COLLADABINDMATERIAL_H_ */
