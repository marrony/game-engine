/*
 * ColladaInstanceNode.h
 *
 *  Created on: 22/05/2012
 *      Author: marrony.neris
 */

#ifndef COLLADAINSTANCENODE_H_
#define COLLADAINSTANCENODE_H_

#include "ColladaElement.h"

DECLARE_VISITOR(ColladaInstanceNode);

class ColladaInstanceNode : public ColladaElement {
	std::string url;
public:
	static std::string elementType();

	virtual void loadFromXml(TiXmlElement* element);

	virtual void accept(Visitor* visitor);

	const std::string& getUrl() const { return url; }
};

#endif /* COLLADAINSTANCENODE_H_ */
