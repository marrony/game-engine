/*
 * ColladaSceneLoader.cpp
 *
 *  Created on: 15/02/2011
 *      Author: marrony
 */

#include "tinyxml.h"

#include <iterator>

#include "ColladaDocument.h"
#include "CreateGeometry.h"
#include "CreateScene.h"

class ColladaLoader : public ResourceLoader {
	ResourceCompiler* compiler;
	ResourceManager* manager;

	void parse(const char* fileName, ColladaDocument& colladaDocument) {
		TiXmlDocument xmlDoc;
		xmlDoc.LoadFile(fileName);

		if(xmlDoc.Error()) {
			throw Exception("COLLADA: Parse error");
		}

		TiXmlElement* root = xmlDoc.RootElement();

		if(root->Value() != std::string("COLLADA")) {
			throw Exception("Not a COLLADA file");
		}

		if(root->Attribute("version") != std::string("1.4.1")) {
			throw Exception("Invalid version");
		}

		colladaDocument.loadFromXml(root);
	}

	virtual void compileResource(const char* fileName, std::map<std::string, std::string>& options) {
		try {
			ColladaDocument colladaDocument;

			parse(fileName, colladaDocument);

			if(options["only-geometry"] == "true") {
				for(ColladaGeometry* geometry : colladaDocument.libraryGeometries->getItems()) {
					CreateGeometry createGeometry(options["geometry-name"], manager);

					geometry->accept(&createGeometry);

					saveModel(file::getPath(fileName), createGeometry.getModelData());
				}
			} else {
				CreateScene createScene(compiler, manager, this, &colladaDocument);
				colladaDocument.scene->accept(&createScene);

				saveScene(file::getPath(fileName), createScene.getScene());
			}
		} catch(...) {
			throw;
		}
	}
};
