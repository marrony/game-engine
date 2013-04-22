/*
 * ColladaInstanceNode.h
 *
 *  Created on: 22/05/2012
 *      Author: marrony.neris
 */

#ifndef COLLADAINSTANCENODE_H_
#define COLLADAINSTANCENODE_H_

#include "ColladaElement.h"

#include <string>

DECLARE_VISITOR(ColladaInstanceNode);

class ColladaInstanceNode : public ColladaElement {
	std::string url;
public:
	static std::string elementType() {
		return "instance_node";
	}

	virtual void loadFromXml(TiXmlElement* element) {
		loadAttribute(element, "url", url);
	}

	virtual void accept(engine::Visitor* visitor);

	const std::string& getUrl() const { return url; }
};

#endif /* COLLADAINSTANCENODE_H_ */
