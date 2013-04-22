/*
 * ColladaTriangles.h
 *
 *  Created on: 20/04/2012
 *      Author: marrony.neris
 */

#ifndef COLLADATRIANGLES_H_
#define COLLADATRIANGLES_H_

#include "ColladaGeometricPrimitive.h"

DECLARE_VISITOR(ColladaTriangles);

class ColladaTriangles : public ColladaGeometricPrimitive {
public:
	virtual ~ColladaTriangles() {
	}

	static std::string elementType() {
		return "triangles";
	}

	virtual void loadFromXml(TiXmlElement* element) {
		ColladaGeometricPrimitive::loadFromXml(element);
	}

	virtual void accept(engine::Visitor* visitor);
};

#endif /* COLLADATRIANGLES_H_ */
