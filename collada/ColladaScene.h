/*
 * ColladaScene.h
 *
 *  Created on: 21/05/2012
 *      Author: marrony
 */

#ifndef COLLADASCENE_H_
#define COLLADASCENE_H_

#include "ColladaElement.h"

DECLARE_VISITOR(ColladaInstanceVisualScene)

class ColladaInstanceVisualScene : public ColladaElement {
	std::string sid;
	std::string url;
public:
	static std::string elementType();

	virtual void loadFromXml(class TiXmlElement* element);

	virtual void accept(Visitor* visitor);

	const std::string& getSid() const { return sid; }
	const std::string& getUrl() const { return url; }
};

DECLARE_VISITOR(ColladaScene)

class ColladaScene : public ColladaElement {
	class ColladaInstanceVisualScene* instanceVisualScene;
public:
	virtual ~ColladaScene();

	static std::string elementType();

	virtual void loadFromXml(class TiXmlElement* element);

	virtual void accept(Visitor* visitor);

	class ColladaInstanceVisualScene* getInstanceVisualScene() const { return instanceVisualScene; }
};

#endif /* COLLADASCENE_H_ */
