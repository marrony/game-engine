/*
 * ColladaInstanceController.h
 *
 *  Created on: 18/05/2012
 *      Author: objective
 */

#ifndef COLLADAINSTANCECONTROLLER_H_
#define COLLADAINSTANCECONTROLLER_H_

#include "ColladaElement.h"
#include "ColladaSkeleton.h"
#include "ColladaBindMaterial.h"

DECLARE_VISITOR(ColladaInstanceController);

class ColladaInstanceController : public ColladaElement {
	std::vector<ColladaSkeleton*> skeletons;
	ColladaBindMaterial* bindMaterial;
public:
	~ColladaInstanceController() {
		eraseVector(skeletons);
		delete bindMaterial;
	}

	static std::string elementType() {
		return "instance_controller";
	}

	virtual void loadFromXml(TiXmlElement* element) {
		ColladaElement::loadFromXml(element);

		skeletons = createElementsFromXml<ColladaSkeleton>(element);
		bindMaterial = createElementFromXml<ColladaBindMaterial>(element);
	}

	virtual void accept(engine::Visitor* visitor);
};

#endif /* COLLADAINSTANCECONTROLLER_H_ */
