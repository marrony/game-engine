/*
 * ColladaAsset.h
 *
 *  Created on: 20/04/2012
 *      Author: marrony.neris
 */

#ifndef COLLADAASSET_H_
#define COLLADAASSET_H_

#include "ColladaElement.h"

#include <string>

DECLARE_VISITOR(ColladaAsset);

class ColladaAsset : public ColladaElement {
public:
	static std::string elementType() {
		return "asset";
	}

	virtual void loadFromXml(class TiXmlElement* element) {
	}

	virtual void accept(engine::Visitor* visitor);
};

#endif /* COLLADAASSET_H_ */
