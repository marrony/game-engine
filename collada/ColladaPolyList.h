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
	static std::string elementType();

	virtual void loadFromXml(TiXmlElement* element);

	virtual void accept(Visitor* visitor);

	const std::vector<int>& getVcount() const { return vcount; }
};

#endif /* COLLADAPOLYLIST_H_ */
