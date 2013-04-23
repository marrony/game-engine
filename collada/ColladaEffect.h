/*
 * ColladaEffect.h
 *
 *  Created on: 18/05/2012
 *      Author: objective
 */

#ifndef COLLADAEFFECT_H_
#define COLLADAEFFECT_H_

#include "ColladaElement.h"

DECLARE_VISITOR(ColladaEffect);

class ColladaImage;
class ColladaNewParam;
class ColladaEffectProfileCommon;

class ColladaEffect : public ColladaElement {
	std::vector<ColladaImage*> images;
	std::vector<ColladaNewParam*> newParams;
	ColladaEffectProfileCommon* profileCommon;
public:
	~ColladaEffect();

	static std::string elementType();

	virtual void loadFromXml(TiXmlElement* element);

	virtual void accept(Visitor* visitor);

	const std::vector<ColladaImage*>& getImages() const { return images; }
	const std::vector<ColladaNewParam*>& getNewParams() const { return newParams; }
	ColladaEffectProfileCommon* getProfileCommon() const { return profileCommon; }
};

#endif /* COLLADAEFFECT_H_ */
