/*
 * ColladaDocument.cpp
 *
 *  Created on: 18/05/2012
 *      Author: objective
 */

#include "ColladaDocument.h"

DEFINE_VISITOR(ColladaDocument)

ColladaDocument::~ColladaDocument() {
	delete libraryImages;
	delete libraryEffects;
	delete libraryMaterials;
	delete libraryLights;
	delete libraryCameras;
	delete libraryGeometries;
	delete libraryVisualScenes;
	delete libraryNodes;
	delete scene;
}

std::string ColladaDocument::elementType() {
	return "COLLADA";
}

void ColladaDocument::loadFromXml(TiXmlElement* element) {
	libraryImages = createElementFromXml<ColladaLibrary<ColladaImage> >(element);
	libraryEffects = createElementFromXml<ColladaLibrary<ColladaEffect> >(element);
	libraryMaterials = createElementFromXml<ColladaLibrary<ColladaMaterial> >(element);
	libraryLights = createElementFromXml<ColladaLibrary<ColladaLight> >(element);
	libraryCameras = createElementFromXml<ColladaLibrary<ColladaCamera> >(element);
	libraryGeometries = createElementFromXml<ColladaLibrary<ColladaGeometry> >(element);
	libraryVisualScenes = createElementFromXml<ColladaLibrary<ColladaVisualScene> >(element);
	libraryNodes = createElementFromXml<ColladaLibrary<ColladaNode> >(element);
	scene = createElementFromXml<ColladaScene>(element);
}

ColladaImage* ColladaDocument::findImage(const std::string& url) {
	return dynamic_cast<ColladaImage*>(libraryImages->findElement(url));
}

ColladaEffect* ColladaDocument::findEffect(const std::string& url) {
	return dynamic_cast<ColladaEffect*>(libraryEffects->findElement(url));
}

ColladaMaterial* ColladaDocument::findMaterial(const std::string& url) {
	return dynamic_cast<ColladaMaterial*>(libraryMaterials->findElement(url));
}

ColladaLight* ColladaDocument::findLight(const std::string& url) {
	return dynamic_cast<ColladaLight*>(libraryLights->findElement(url));
}

ColladaCamera* ColladaDocument::findCamera(const std::string& url) {
	return dynamic_cast<ColladaCamera*>(libraryCameras->findElement(url));
}

ColladaGeometry* ColladaDocument::findGeometry(const std::string& url) {
	return dynamic_cast<ColladaGeometry*>(libraryGeometries->findElement(url));
}

ColladaVisualScene* ColladaDocument::findVisualScene(const std::string& url) {
	return dynamic_cast<ColladaVisualScene*>(libraryVisualScenes->findElement(url));
}

ColladaNode* ColladaDocument::findNode(const std::string& url) {
	return dynamic_cast<ColladaNode*>(libraryNodes->findElement(url));
}
