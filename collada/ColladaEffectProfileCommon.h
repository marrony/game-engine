/*
 * ColladaEffectProfileCommon.h
 *
 *  Created on: 18/05/2012
 *      Author: objective
 */

#ifndef COLLADAEFFECTPROFILECOMMON_H_
#define COLLADAEFFECTPROFILECOMMON_H_

#include "ColladaElement.h"

DECLARE_VISITOR(ColladaEffectProfileCommon);

class ColladaImage;
class ColladaNewParam;
class ColladaEffectTechnique;

class ColladaEffectProfileCommon : public ColladaElement {
	std::vector<ColladaImage*> images;
	std::vector<ColladaNewParam*> newParams;
	ColladaEffectTechnique* technique;
public:
	~ColladaEffectProfileCommon();

	static std::string elementType();

	virtual void loadFromXml(TiXmlElement* element);

	virtual void accept(Visitor* visitor);

	const std::vector<ColladaImage*>& getImages() const { return images; }
	const std::vector<ColladaNewParam*>& getNewParams() const { return newParams; }
	ColladaEffectTechnique* getTechnique() const { return technique; }
};


#endif /* COLLADAEFFECTPROFILECOMMON_H_ */
