/*
 * ColladaPolyList.h
 *
 *  Created on: 20/04/2012
 *      Author: marrony.neris
 */

#ifndef COLLADAPOLYLIST_H_
#define COLLADAPOLYLIST_H_

#include "ColladaGeometricPrimitive.h"

DECLARE_VISITOR(ColladaPolyList);

class ColladaPolyList : public ColladaGeometricPrimitive {
	std::vector<int> vcount;
public:
	virtual ~ColladaPolyList() {
	}

	static std::string elementType() {
		return "polylist";
	}

	virtual void loadFromXml(TiXmlElement* element) {
		ColladaGeometricPrimitive::loadFromXml(element);

		loadArray(element->FirstChildElement("vcount"), vcount);
	}

	virtual void accept(engine::Visitor* visitor);

	const std::vector<int>& getVcount() const { return vcount; }
};

#endif /* COLLADAPOLYLIST_H_ */
