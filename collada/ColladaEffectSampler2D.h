/*
 * ColladaEffectSampler2D.h
 *
 *  Created on: 18/05/2012
 *      Author: objective
 */

#ifndef COLLADAEFFECTSAMPLER2D_H_
#define COLLADAEFFECTSAMPLER2D_H_

#include "ColladaElement.h"

DECLARE_VISITOR(ColladaEffectSampler2D);

class ColladaEffectSampler2D : public ColladaElement {
public:
	static std::string elementType();

	virtual void loadFromXml(TiXmlElement* element);

	virtual void accept(Visitor* visitor);
};

#endif /* COLLADAEFFECTSAMPLER2D_H_ */
