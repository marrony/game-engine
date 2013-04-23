/*
 * ColladaMesh.h
 *
 *  Created on: 20/04/2012
 *      Author: marrony.neris
 */

#ifndef COLLADAMESH_H_
#define COLLADAMESH_H_

#include "ColladaElement.h"
#include "ColladaSource.h"
#include "ColladaVertices.h"
#include "ColladaTriangles.h"
#include "ColladaPolyList.h"
#include "ColladaUtil.h"

DECLARE_VISITOR(ColladaMesh);

class ColladaMesh : public ColladaElement {
	std::vector<ColladaSource*> sources;
	ColladaVertices* vertices;
	std::vector<ColladaTriangles*> triangles;
	std::vector<ColladaPolyList*> polylists;
public:
	~ColladaMesh();

	static std::string elementType();

	virtual void loadFromXml(TiXmlElement* element);

	virtual void accept(Visitor* visitor);

	const std::vector<ColladaSource*>& getSources() const { return sources; }
	ColladaVertices* getVertices() const { return vertices; }
	const std::vector<ColladaTriangles*>& getTriangles() const { return triangles; }
	const std::vector<ColladaPolyList*>& getPolylists() const { return polylists; }
};

#endif /* COLLADAMESH_H_ */
