/*
 * CreateScene.cpp
 *
 *  Created on: 22/05/2012
 *      Author: marrony.neris
 */

#include "CreateScene.h"
#include "ColladaInstanceNode.h"
#include "ColladaInstanceGeometry.h"
#include "ColladaInstanceMaterial.h"
#include "ColladaInstanceEffect.h"
#include "ColladaBindMaterial.h"
#include "ColladaArray.h"
#include "CreateGeometry.h"
#include "Material.h"
#include "ResourceManager.h"

void CreateScene::pushNode(Node* node) {
	nodes.push(node);
	matrix.push(node->getTransformation());
}

void CreateScene::popNode() {
	math::Vector3 translation, scale;
	math::Quaternion orientation;

	matrix.top().decompose(orientation, translation, scale);
	nodes.top()->setOrientation(orientation);
	nodes.top()->setPosition(translation);
	nodes.top()->setScale(scale);

	nodes.top();
	matrix.top();
}

void CreateScene::concatNode(Node* node) {
	nodes.top()->addChild(node);
}

void CreateScene::concatMatrix(math::Matrix4 m) {
	matrix.top() = matrix.top() * m;
}

CreateScene::CreateScene(ResourceCompiler* compiler, ResourceManager* manager, ResourceLoader* loader, ColladaDocument* document) :
		compiler(compiler), manager(manager), loader(loader), document(document), product(0) {
}

CreateScene::~CreateScene() {
	delete product;
}

void CreateScene::visit(ColladaScene* scene) {
	ColladaInstanceVisualScene* instanceVisualScene = scene->getInstanceVisualScene();

	ColladaVisualScene* visualScene = document->findVisualScene(instanceVisualScene->getUrl());

	if(!visualScene)
		return;

	product = new Scene(visualScene->getId(), manager);

	pushNode(product->getRoot());

	for(ColladaNode* node : visualScene->getNodes()) {
		if(!node->getInstanceControllers().empty()) //TODO animation
			continue;

		node->accept(this);
	}

	popNode();
}

void CreateScene::visit(ColladaNode* colladaNode) {
	Node* newNode = product->createNode();

	concatNode(newNode);

	pushNode(newNode);

	for(ColladaTransformation* transformation : colladaNode->getTransformations())
		transformation->accept(this);

	for(ColladaNode* child : colladaNode->getNodes())
		child->accept(this);

	for(ColladaInstanceNode* instanceNode : colladaNode->getInstanceNodes()) {
		ColladaNode* node = document->findNode(instanceNode->getUrl());

		if(node)
			node->accept(this);
	}

	for(ColladaInstanceGeometry* instanceGeometry : colladaNode->getInstanceGeometries())
		instanceGeometry->accept(this);

	popNode();
}

void CreateScene::visit(ColladaInstanceGeometry* instanceGeometry) {
	//materials.clear();

	for(ColladaInstanceMaterial* instanceMaterial : instanceGeometry->getBindMaterial()->getTechniqueCommon()->getInstanceMaterial()) {
		const std::string& target = instanceMaterial->getTarget();
		ColladaMaterial* material = document->findMaterial(instanceMaterial->getTarget());

		material->accept(this);
	}

	ColladaGeometry* geometry = document->findGeometry(instanceGeometry->getUrl());

	CreateGeometry createGeometry("", manager);
	geometry->accept(&createGeometry);

	Mesh* geometryData = createGeometry.getModelData();
	Model* model = new Model(geometryData->getName(), manager);
	product->createModelInstance(model, model->getBoundingBox(), nodes.top());
}

void CreateScene::visit(ColladaMaterial* colladaMaterial) {
//	ResourceId materialId = compiler->findResource(colladaMaterial->getId().c_str(), Material::TYPE);
//
//	if(materialId == ResourceId(-1)) {
//		ColladaInstanceEffect* instanceEffect = colladaMaterial->getInstanceEffect();
//		ColladaEffect* effect = document->findEffect(instanceEffect->getUrl());
//
//		ColladaEffectProfileCommon* profileCommon = effect->getProfileCommon();
//
//		for(ColladaNewParam* newParam : profileCommon->getNewParams()) {
//			ColladaEffectSurface* surface = newParam->getSurface();
//			if(surface) {
//				ColladaInitFrom* initFrom = surface->getInitFrom();
//
//				//ColladaImage* image = document->findImage(initFrom->getValue());
//				//std::string path = image->getPath();
//
//				//Material* material = new Material(colladaMaterial->getId());
//
//				break;
//			}
//		}
//	}
//
//	materials[colladaMaterial->getId()] = materialId;
}

void CreateScene::visit(ColladaMatrix* colladaMatrix) {
	math::Matrix4 transformation;

	colladaMatrix->getMatrix(transformation.matrix);

	concatMatrix(transformation.transpose());
}

void CreateScene::visit(ColladaRotate* rotate) {
	math::Vector3 axis;

	rotate->getAxis(axis.vector);

	math::Quaternion quat(math::AxisAngle(rotate->getAngle(), axis));

	concatMatrix(math::Matrix4::rotationMatrix(quat));
}

void CreateScene::visit(ColladaScale* scale) {
	math::Vector3 s;

	scale->getScale(s.vector);

	concatMatrix(math::Matrix4::scaleMatrix(s));
}

void CreateScene::visit(ColladaSkew* skew) {

}

void CreateScene::visit(ColladaTranslate* translate) {
	math::Vector3 t;

	translate->getTranslation(t.vector);

	concatMatrix(math::Matrix4::translateMatrix(t));
}
