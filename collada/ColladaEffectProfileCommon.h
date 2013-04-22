/*
 * ColladaEffectProfileCommon.h
 *
 *  Created on: 18/05/2012
 *      Author: objective
 */

#ifndef COLLADAEFFECTPROFILECOMMON_H_
#define COLLADAEFFECTPROFILECOMMON_H_

#include "ColladaElement.h"
#include "ColladaImage.h"
#include "ColladaEffectNewParam.h"
#include "ColladaEffectTechnique.h"

DECLARE_VISITOR(ColladaEffectProfileCommon);

class ColladaEffectProfileCommon : public ColladaElement {
	std::vector<ColladaImage*> images;
	std::vector<ColladaNewParam*> newParams;
	ColladaEffectTechnique* technique;
public:
	~ColladaEffectProfileCommon() {
		eraseVector(images);
		eraseVector(newParams);
		delete technique;
	}

	static std::string elementType() {
		return "profile_COMMON";
	}

	virtual void loadFromXml(TiXmlElement* element) {
		ColladaElement::loadFromXml(element);

		images = createElementsFromXml<ColladaImage>(element);
		newParams = createElementsFromXml<ColladaNewParam>(element);
		technique = createElementFromXml<ColladaEffectTechnique>(element);
	}

	virtual void accept(engine::Visitor* visitor);

	const std::vector<ColladaImage*>& getImages() const { return images; }
	const std::vector<ColladaNewParam*>& getNewParams() const { return newParams; }
	ColladaEffectTechnique* getTechnique() const { return technique; }
};


#endif /* COLLADAEFFECTPROFILECOMMON_H_ */
