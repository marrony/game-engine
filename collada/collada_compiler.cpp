/*
 * collada_compiler.cpp
 *
 *  Created on: 22/04/2013
 *      Author: marrony
 */
#include "ColladaDocument.h"
#include "CreateGeometry.h"

#include "mesh.h"

bool load_from_file(const char* filename, ColladaDocument& colladaDocument) {
	TiXmlDocument xmlDoc;
	xmlDoc.LoadFile(filename);

	if(xmlDoc.Error()) {
		//throw Exception("COLLADA: Parse error");
		return false;
	}

	TiXmlElement* root = xmlDoc.RootElement();

	if(root->Value() != std::string("COLLADA")) {
		//throw Exception("Not a COLLADA file");
		return false;
	}

	if(root->Attribute("version") != std::string("1.4.1")) {
		//throw Exception("Invalid version");
		return false;
	}

	colladaDocument.loadFromXml(root);

	return true;
}

int main(int argc, char* argv[]) {
	ColladaDocument colladaDocument;

	if(!load_from_file(argv[1], colladaDocument))
		return 1;

	std::vector<ColladaGeometry*>& items = colladaDocument.libraryGeometries->getItems();
	for(size_t i = 0; i < items.size(); i++) {
		ColladaGeometry* geometry = items[i];

		CreateGeometry createGeometry;
		geometry->accept(&createGeometry);
	}

	return 0;
}
