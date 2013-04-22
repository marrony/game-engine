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

//////////////////////////////////////////////////////////////////

class ColladaEffectLambert : public ColladaEffectConstant {
public:
	static std::string elementType() {
		return "lambert";
	}

	virtual void loadFromXml(TiXmlElement* element) {
		ColladaElement::loadFromXml(element);
	}
};

//////////////////////////////////////////////////////////////////

class ColladaEffectBlinn : public ColladaEffectLambert {
public:
	static std::string elementType() {
		return "blinn";
	}

	virtual void loadFromXml(TiXmlElement* element) {
		ColladaElement::loadFromXml(element);
	}
};

//////////////////////////////////////////////////////////////////

class ColladaEffectPhong : public ColladaEffectBlinn {
public:
	static std::string elementType() {
		return "phong";
	}

	virtual void loadFromXml(TiXmlElement* element) {
		ColladaElement::loadFromXml(element);
	}
};

//////////////////////////////////////////////////////////////////

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

	void saveModel(const std::string& path, Mesh* geometryData) {
		std::string outputName = path + "/" + geometryData->getName() + ".model";
		FileStream fileStream(outputName);
		ResourceBinStream resourceStream(fileStream);
		geometryData->writeToStream(resourceStream);
	}

	void saveScene(const std::string& path, Scene* scene) {
		std::string outputName = path + "/" + scene->getName() + ".scene";
		FileStream fileStream(outputName);
		ResourceBinStream resourceStream(fileStream);
		SceneUtils::write(resourceStream, *manager, scene);
	}
public:
	virtual ~ColladaLoader() {
	}

	virtual void release() {
		delete this;
	}

	virtual void initialize(ResourceCompiler* compiler, ResourceManager* manager) {
		this->compiler = compiler;
		this->manager = manager;

		compiler->registerLoader(this, "Collada Loader", "dae");
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

static Module* dynamicLib;

extern "C" Module* getModule() {
	return dynamicLib;
}

extern "C" EXPIMP Plugin* initializePlugin(Module* dynLib) {
	dynamicLib = dynLib;
	return new ColladaLoader;
}
