/*
 * ColladaDocument.cpp
 *
 *  Created on: 18/05/2012
 *      Author: objective
 */

#include "ColladaDocument.h"
#include "ColladaImage.h"
#include "ColladaEffect.h"
#include "ColladaMaterial.h"
#include "ColladaLight.h"
#include "ColladaCamera.h"
#include "ColladaGeometry.h"
#include "ColladaVisualScene.h"
#include "ColladaNode.h"
#include "ColladaScene.h"
#include "ColladaUtil.h"

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
	libraryImages = ColladaUtil::createElementFromXml<ColladaLibrary<ColladaImage> >(this, element);
	libraryEffects = ColladaUtil::createElementFromXml<ColladaLibrary<ColladaEffect> >(this, element);
	libraryMaterials = ColladaUtil::createElementFromXml<ColladaLibrary<ColladaMaterial> >(this, element);
	libraryLights = ColladaUtil::createElementFromXml<ColladaLibrary<ColladaLight> >(this, element);
	libraryCameras = ColladaUtil::createElementFromXml<ColladaLibrary<ColladaCamera> >(this, element);
	libraryGeometries = ColladaUtil::createElementFromXml<ColladaLibrary<ColladaGeometry> >(this, element);
	libraryVisualScenes = ColladaUtil::createElementFromXml<ColladaLibrary<ColladaVisualScene> >(this, element);
	libraryNodes = ColladaUtil::createElementFromXml<ColladaLibrary<ColladaNode> >(this, element);
	scene = ColladaUtil::createElementFromXml<ColladaScene>(this, element);
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
