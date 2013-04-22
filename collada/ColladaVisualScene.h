/*
 * ColladaVisualScene.h
 *
 *  Created on: 18/05/2012
 *      Author: objective
 */

#ifndef COLLADAVISUALSCENE_H_
#define COLLADAVISUALSCENE_H_

#include "ColladaElement.h"
#include "ColladaNode.h"

DECLARE_VISITOR(ColladaVisualScene);

class ColladaVisualScene : public ColladaElement {
	std::vector<ColladaNode*> nodes;
public:
	~ColladaVisualScene() {
		eraseVector(nodes);
	}

	static std::string elementType() {
		return "visual_scene";
	}

	virtual void loadFromXml(TiXmlElement* element) {
		ColladaElement::loadFromXml(element);

		nodes = createElementsFromXml<ColladaNode>(element);
	}

	virtual void accept(engine::Visitor* visitor);

	const std::vector<ColladaNode*>& getNodes() const { return nodes; }
};


#endif /* COLLADAVISUALSCENE_H_ */
