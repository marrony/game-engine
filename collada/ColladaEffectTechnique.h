/*
 * ColladaEffectTechnique.h
 *
 *  Created on: 18/05/2012
 *      Author: objective
 */

#ifndef COLLADAEFFECTTECHNIQUE_H_
#define COLLADAEFFECTTECHNIQUE_H_

#include "ColladaElement.h"

DECLARE_VISITOR(ColladaEffectTechnique);

class ColladaEffectConstant;

class ColladaEffectTechnique : public ColladaElement {
	std::string sid;
	ColladaEffectConstant* shader;
public:
	~ColladaEffectTechnique();

	static std::string elementType();

	virtual void loadFromXml(TiXmlElement* element);

	virtual void accept(Visitor* visitor);
};

#endif /* COLLADAEFFECTTECHNIQUE_H_ */
