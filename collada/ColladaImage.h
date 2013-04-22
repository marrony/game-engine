/*
 * ColladaImage.h
 *
 *  Created on: 18/05/2012
 *      Author: objective
 */

#ifndef COLLADAIMAGE_H_
#define COLLADAIMAGE_H_

#include "ColladaElement.h"

DECLARE_VISITOR(ColladaImage);

class ColladaImage : public ColladaElement {
	std::string path;
public:
	static std::string elementType() {
		return "image";
	}

	virtual void loadFromXml(TiXmlElement* element) {
		ColladaElement::loadFromXml(element);

		TiXmlElement* data = element->FirstChildElement("data");
		if(data != 0) {
			//throw Exception("not supported");
		}

		TiXmlElement* init_from = element->FirstChildElement("init_from");
		if(init_from != 0) {
			path = init_from->GetText();
		}
	}

	virtual void accept(engine::Visitor* visitor);

	const std::string& getPath() const { return path; }
};

#endif /* COLLADAIMAGE_H_ */
