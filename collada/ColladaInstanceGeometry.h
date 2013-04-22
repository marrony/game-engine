/*
 * ColladaInstanceGeometry.h
 *
 *  Created on: 18/05/2012
 *      Author: objective
 */

#ifndef COLLADAINSTANCEGEOMETRY_H_
#define COLLADAINSTANCEGEOMETRY_H_

#include "ColladaElement.h"

#include <string>

DECLARE_VISITOR(ColladaInstanceGeometry);

class ColladaInstanceGeometry : public ColladaElement {
	std::string url;
	class ColladaBindMaterial* bindMaterial;
public:
	~ColladaInstanceGeometry();

	static std::string elementType();

	virtual void loadFromXml(class TiXmlElement* element);

	virtual void accept(engine::Visitor* visitor);

	const std::string& getUrl() const { return url; }
	class ColladaBindMaterial* getBindMaterial() const { return bindMaterial; }
};

#endif /* COLLADAINSTANCEGEOMETRY_H_ */
