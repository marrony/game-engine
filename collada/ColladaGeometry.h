/*
 * ColladaGeometry.h
 *
 *  Created on: 20/04/2012
 *      Author: marrony.neris
 */

#ifndef COLLADAGEOMETRY_H_
#define COLLADAGEOMETRY_H_

#include "ColladaElement.h"

DECLARE_VISITOR(ColladaGeometry);

class ColladaMesh;

class ColladaGeometry : public ColladaElement {
	ColladaMesh* mesh;
public:
	~ColladaGeometry();

	static std::string elementType();

	virtual void loadFromXml(TiXmlElement* element);

	virtual void accept(Visitor* visitor);

	const std::string& getName() {
		return name;
	}

	ColladaMesh* getMesh() {
		return mesh;
	}
};

#endif /* COLLADAGEOMETRY_H_ */
