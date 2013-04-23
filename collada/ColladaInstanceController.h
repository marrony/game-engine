/*
 * ColladaInstanceController.h
 *
 *  Created on: 18/05/2012
 *      Author: objective
 */

#ifndef COLLADAINSTANCECONTROLLER_H_
#define COLLADAINSTANCECONTROLLER_H_

#include "ColladaElement.h"

DECLARE_VISITOR(ColladaInstanceController);

class ColladaSkeleton;
class ColladaBindMaterial;

class ColladaInstanceController : public ColladaElement {
	std::vector<ColladaSkeleton*> skeletons;
	ColladaBindMaterial* bindMaterial;
public:
	~ColladaInstanceController();

	static std::string elementType();

	virtual void loadFromXml(TiXmlElement* element);

	virtual void accept(Visitor* visitor);
};

#endif /* COLLADAINSTANCECONTROLLER_H_ */
