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
	static std::string elementType();

	virtual void loadFromXml(TiXmlElement* element);

	virtual void accept(Visitor* visitor);

	const std::string& getPath() const { return path; }
};

#endif /* COLLADAIMAGE_H_ */
