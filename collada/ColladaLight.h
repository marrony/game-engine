/*
 * ColladaLight.h
 *
 *  Created on: 20/04/2012
 *      Author: marrony.neris
 */

#ifndef COLLADALIGHT_H_
#define COLLADALIGHT_H_

#include "ColladaElement.h"

DECLARE_VISITOR(ColladaLight);

class ColladaLight : public ColladaElement {
public:
	static std::string elementType();

	virtual void loadFromXml(TiXmlElement* element);

	virtual void accept(Visitor* visitor);
};

#endif /* COLLADALIGHT_H_ */
