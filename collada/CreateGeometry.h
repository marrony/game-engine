/*
 * CreateGeometry.h
 *
 *  Created on: 21/05/2012
 *      Author: marrony
 */

#ifndef CREATEGEOMETRY_H_
#define CREATEGEOMETRY_H_

#include "ColladaElement.h"
#include "ColladaGeometry.h"
#include "ColladaMesh.h"
#include "ColladaPolyList.h"
#include "ColladaTriangles.h"

#include <string>
#include <vector>
#include <map>

using namespace engine;

class CreateGeometry : public Visitor,
							public ColladaGeometryVisitor,
							public ColladaMeshVisitor,
							public ColladaPolyListVisitor,
							public ColladaTrianglesVisitor {
	class ResourceManager* manager;
	Mesh* geometryData;
	std::string name;
public:
	CreateGeometry(const std::string& name, class ResourceManager* manager);
	virtual ~CreateGeometry();

	Mesh* getModelData() { return geometryData; }

	virtual void visit(ColladaGeometry* geometry);
	virtual void visit(ColladaMesh* mesh);
	virtual void visit(ColladaPolyList* polylist);
	virtual void visit(ColladaTriangles* triangles);
};

#endif /* CREATEGEOMETRY_H_ */
