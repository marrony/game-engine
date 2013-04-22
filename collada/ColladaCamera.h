/*
 * ColladaCamera.h
 *
 *  Created on: 20/04/2012
 *      Author: marrony.neris
 */

#ifndef COLLADACAMERA_H_
#define COLLADACAMERA_H_

#include "ColladaElement.h"

#include <string>

DECLARE_VISITOR(ColladaCamera);

class ColladaCamera : public ColladaElement {
public:
	static std::string elementType();

	virtual void loadFromXml(class TiXmlElement* element);

	virtual void accept(engine::Visitor* visitor);
};

#endif /* COLLADACAMERA_H_ */
