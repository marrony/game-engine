/*
 * ColladaDocument.h
 *
 *  Created on: 18/05/2012
 *      Author: objective
 */

#ifndef COLLADADOCUMENT_H_
#define COLLADADOCUMENT_H_

#include "ColladaLibrary.h"
#include "ColladaElement.h"

#include <string>

DECLARE_VISITOR(ColladaDocument);

class ColladaImage;
class ColladaEffect;
class ColladaMaterial;
class ColladaLight;
class ColladaCamera;
class ColladaGeometry;
class ColladaVisualScene;
class ColladaNode;
class ColladaScene;

class ColladaDocument : public ColladaElement {
public:
	ColladaLibrary<ColladaImage>* libraryImages;
	ColladaLibrary<ColladaEffect>* libraryEffects;
	ColladaLibrary<ColladaMaterial>* libraryMaterials;
	ColladaLibrary<ColladaLight>* libraryLights;
	ColladaLibrary<ColladaCamera>* libraryCameras;
	ColladaLibrary<ColladaGeometry>* libraryGeometries;
	ColladaLibrary<ColladaVisualScene>* libraryVisualScenes;
	ColladaLibrary<ColladaNode>* libraryNodes;
	ColladaScene* scene;

	~ColladaDocument();

	static std::string elementType();

	void loadFromXml(class TiXmlElement* element);

	virtual void accept(Visitor* visitor);

	ColladaImage* findImage(const std::string& url);
	ColladaEffect* findEffect(const std::string& url);
	ColladaMaterial* findMaterial(const std::string& url);
	ColladaLight* findLight(const std::string& url);
	ColladaCamera* findCamera(const std::string& url);
	ColladaGeometry* findGeometry(const std::string& url);
	ColladaVisualScene* findVisualScene(const std::string& url);
	ColladaNode* findNode(const std::string& url);
};

#endif /* COLLADADOCUMENT_H_ */
