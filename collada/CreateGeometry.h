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

#include "vector2.h"
#include "vector3.h"
#include "vector4.h"

#include <string>
#include <vector>
#include <map>

struct MeshVertex {
	Vector3 position;
	Vector3 normal;
	Vector3 sTangent;
	Vector3 tTangent;
	Vector3 color;
	Vector2 texCoord;
	Vector4 boneIds;
	Vector4 weights;

	enum VertexFlags {
		POSITION = 0x01,
		NORMAL = 0x02,
		TEXTURE = 0x04,
		sTANGENT = 0x08,
		tTANGENT = 0x10,
		COLOR = 0x20,
		BONES = 0x40
	};
};

struct Batch {
	unsigned short offset; //offset in indices array
	unsigned short count;  //number os elements to draw
	unsigned short start;  //minimum index in range [offset, offset+count]
	unsigned short end;    //maximum index in range [offset, offset+count]
};

class CreateGeometry : public Visitor,
							public ColladaGeometryVisitor,
							public ColladaMeshVisitor,
							public ColladaPolyListVisitor,
							public ColladaTrianglesVisitor {
	std::string name;
	std::vector<Vector3> position;
	std::vector<Vector3> normal;
	std::vector<Vector3> sTangent;
	std::vector<Vector3> tTangent;
	std::vector<Vector3> color;
	std::vector<Vector2> texCoord;
	std::vector<Vector4> boneIds;
	std::vector<Vector4> weights;
	std::vector<unsigned short> indices;

	int elementsPerVertex;
	int attributeOffsets[8];

	void addVertexData(const std::vector<MeshVertex>& vertexArray, const std::vector<unsigned short>& newIndices, int material, int flags);
public:
	CreateGeometry(const std::string& name);
	virtual ~CreateGeometry();

	virtual void visit(ColladaGeometry* geometry);
	virtual void visit(ColladaMesh* mesh);
	virtual void visit(ColladaPolyList* polylist);
	virtual void visit(ColladaTriangles* triangles);
};

#endif /* CREATEGEOMETRY_H_ */
