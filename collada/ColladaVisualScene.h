/*
 * ColladaVisualScene.h
 *
 *  Created on: 18/05/2012
 *      Author: objective
 */

#ifndef COLLADAVISUALSCENE_H_
#define COLLADAVISUALSCENE_H_

#include "ColladaElement.h"

DECLARE_VISITOR(ColladaVisualScene);

class ColladaNode;

class ColladaVisualScene : public ColladaElement {
	std::vector<ColladaNode*> nodes;
public:
	~ColladaVisualScene();

	static std::string elementType();

	virtual void loadFromXml(TiXmlElement* element);

	virtual void accept(Visitor* visitor);

	const std::vector<ColladaNode*>& getNodes() const { return nodes; }
};


#endif /* COLLADAVISUALSCENE_H_ */
