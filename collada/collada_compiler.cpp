/*
 * collada_compiler.cpp
 *
 *  Created on: 22/04/2013
 *      Author: marrony
 */
#include "ColladaDocument.h"
#include "CreateGeometry.h"

int main(int argc, char* argv[]) {
	ColladaDocument colladaDocument;

	TiXmlDocument xmlDoc;
	xmlDoc.LoadFile(argv[1]);

	if(xmlDoc.Error()) {
		//throw Exception("COLLADA: Parse error");
		return 1;
	}

	TiXmlElement* root = xmlDoc.RootElement();

	if(root->Value() != std::string("COLLADA")) {
		//throw Exception("Not a COLLADA file");
		return 1;
	}

	if(root->Attribute("version") != std::string("1.4.1")) {
		//throw Exception("Invalid version");
		return 1;
	}

	colladaDocument.loadFromXml(root);

	std::vector<ColladaGeometry*>& items = colladaDocument.libraryGeometries->getItems();
	for(size_t i = 0; i < items.size(); i++) {
		ColladaGeometry* geometry = items[i];

		CreateGeometry createGeometry("teste");
		geometry->accept(&createGeometry);
	}

	return 0;
}
