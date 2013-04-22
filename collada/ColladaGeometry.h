/*
 * ColladaGeometry.h
 *
 *  Created on: 20/04/2012
 *      Author: marrony.neris
 */

#ifndef COLLADAGEOMETRY_H_
#define COLLADAGEOMETRY_H_

#include "ColladaElement.h"
#include "ColladaMesh.h"

DECLARE_VISITOR(ColladaGeometry);

class ColladaGeometry : public ColladaElement {
	ColladaMesh* mesh;
public:
	~ColladaGeometry() {
		delete mesh;
	}

	static std::string elementType() {
		return "geometry";
	}

	virtual void loadFromXml(TiXmlElement* element) {
		ColladaElement::loadFromXml(element);

		TiXmlElement* geometryChild;

		geometryChild = element->FirstChildElement("convex_mesh");
		if(geometryChild != 0) {
			//throw Exception("Does not support <convex_mesh>");
		}

		geometryChild = element->FirstChildElement("spline");
		if(geometryChild != 0) {
			//throw Exception("Does not support <spline>");
		}

		mesh = createElementFromXml<ColladaMesh>(element);
	}

	virtual void accept(engine::Visitor* visitor);

	const std::string& getName() {
		return name;
	}

	ColladaMesh* getMesh() {
		return mesh;
	}
};

#endif /* COLLADAGEOMETRY_H_ */
