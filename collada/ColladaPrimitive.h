/*
 * ColladaPrimitive.h
 *
 *  Created on: 20/04/2012
 *      Author: marrony.neris
 */

#ifndef COLLADAPRIMITIVE_H_
#define COLLADAPRIMITIVE_H_

#include "ColladaElement.h"

DECLARE_VISITOR(ColladaPrimitive);

class ColladaPrimitive : public ColladaElement {
	std::vector<int> indices;
public:
	static std::string elementType();

	virtual void loadFromXml(TiXmlElement* element);

	std::vector<int> getIndices() const { return indices; }

	virtual void accept(Visitor* visitor);
};

#endif /* COLLADAPRIMITIVE_H_ */
