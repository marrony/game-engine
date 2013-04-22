/*
 * ColladaMaterial.h
 *
 *  Created on: 18/05/2012
 *      Author: objective
 */

#ifndef COLLADAMATERIAL_H_
#define COLLADAMATERIAL_H_

#include "ColladaElement.h"

DECLARE_VISITOR(ColladaMaterial);

class ColladaMaterial : public ColladaElement {
	class ColladaInstanceEffect* instanceEffect;
public:
	~ColladaMaterial();

	static std::string elementType();

	virtual void loadFromXml(class TiXmlElement* element);

	virtual void accept(engine::Visitor* visitor);

	class ColladaInstanceEffect* getInstanceEffect() const { return instanceEffect; }
};

#endif /* COLLADAMATERIAL_H_ */
