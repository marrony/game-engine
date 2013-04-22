/*
 * ColladaNode.h
 *
 *  Created on: 18/05/2012
 *      Author: objective
 */

#ifndef COLLADANODE_H_
#define COLLADANODE_H_

#include "ColladaElement.h"

#include <vector>

DECLARE_VISITOR(ColladaNode);

class ColladaNode : public ColladaElement {
	std::vector<class ColladaNode*> nodes;
	std::vector<class ColladaInstanceGeometry*> instanceGeometries;
	std::vector<class ColladaInstanceController*> instanceControllers;
	std::vector<class ColladaInstanceNode*> instanceNodes;
	std::vector<class ColladaTransformation*> transformations;
public:
	~ColladaNode();

	static std::string elementType();

	virtual void loadFromXml(TiXmlElement* element);

	virtual void accept(engine::Visitor* visitor);

	const std::vector<class ColladaNode*>& getNodes() const { return nodes; }
	const std::vector<class ColladaInstanceGeometry*>& getInstanceGeometries() const { return instanceGeometries; }
	const std::vector<class ColladaInstanceController*>& getInstanceControllers() const { return instanceControllers; }
	const std::vector<class ColladaInstanceNode*>& getInstanceNodes() const { return instanceNodes; }
	const std::vector<class ColladaTransformation*>& getTransformations() const { return transformations; }
};

#endif /* COLLADANODE_H_ */
