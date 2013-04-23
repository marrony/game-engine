/*
 * ColladaInstanceGeometry.h
 *
 *  Created on: 18/05/2012
 *      Author: objective
 */

#ifndef COLLADAINSTANCEGEOMETRY_H_
#define COLLADAINSTANCEGEOMETRY_H_

#include "ColladaElement.h"

DECLARE_VISITOR(ColladaInstanceGeometry);

class ColladaBindMaterial;

class ColladaInstanceGeometry : public ColladaElement {
	std::string url;
	ColladaBindMaterial* bindMaterial;
public:
	~ColladaInstanceGeometry();

	static std::string elementType();

	virtual void loadFromXml(class TiXmlElement* element);

	virtual void accept(Visitor* visitor);

	const std::string& getUrl() const { return url; }
	ColladaBindMaterial* getBindMaterial() const { return bindMaterial; }
};

#endif /* COLLADAINSTANCEGEOMETRY_H_ */
