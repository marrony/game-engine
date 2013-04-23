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
	static std::string elementType();

	virtual void loadFromXml(TiXmlElement* element);

	virtual void accept(Visitor* visitor);
};

#endif /* COLLADATRIANGLES_H_ */
