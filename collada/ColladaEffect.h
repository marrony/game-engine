/*
 * ColladaEffect.h
 *
 *  Created on: 18/05/2012
 *      Author: objective
 */

#ifndef COLLADAEFFECT_H_
#define COLLADAEFFECT_H_

#include "ColladaElement.h"
#include "ColladaEffectNewParam.h"
#include "ColladaEffectProfileCommon.h"

DECLARE_VISITOR(ColladaEffect);

class ColladaEffect : public ColladaElement {
	std::vector<ColladaImage*> images;
	std::vector<ColladaNewParam*> newParams;
	ColladaEffectProfileCommon* profileCommon;
public:
	~ColladaEffect() {
		eraseVector(images);
		eraseVector(newParams);
		delete profileCommon;
	}

	static std::string elementType() {
		return "effect";
	}

	virtual void loadFromXml(TiXmlElement* element) {
		ColladaElement::loadFromXml(element);

		images = createElementsFromXml<ColladaImage>(element);
		newParams = createElementsFromXml<ColladaNewParam>(element);
		profileCommon = createElementFromXml<ColladaEffectProfileCommon>(element);
	}

	virtual void accept(engine::Visitor* visitor);

	const std::vector<ColladaImage*>& getImages() const { return images; }
	const std::vector<ColladaNewParam*>& getNewParams() const { return newParams; }
	ColladaEffectProfileCommon* getProfileCommon() const { return profileCommon; }
};

#endif /* COLLADAEFFECT_H_ */
