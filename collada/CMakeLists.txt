cmake_minimum_required(VERSION 2.6)
project(Collada CXX C)

SET(COLLADA_SRC
	ColladaArray.cpp
	ColladaAsset.cpp
	ColladaBind.cpp
	ColladaBindMaterial.cpp
	ColladaBindVertexInput.cpp
	ColladaCamera.cpp
	ColladaDocument.cpp
	ColladaEffect.cpp
	ColladaEffectConstant.cpp
	ColladaEffectNewParam.cpp
	ColladaEffectProfileCommon.cpp
	ColladaEffectSampler2D.cpp
	ColladaEffectSemantic.cpp
	ColladaEffectSurface.cpp
	ColladaEffectTechnique.cpp
	ColladaElement.cpp
	ColladaExtra.cpp
	ColladaGeometricPrimitive.cpp
	ColladaGeometry.cpp
	ColladaImage.cpp
	ColladaInput.cpp
	ColladaInstanceController.cpp
	ColladaInstanceEffect.cpp
	ColladaInstanceGeometry.cpp
	ColladaInstanceMaterial.cpp
	ColladaInstanceNode.cpp
	ColladaLibrary.cpp
	ColladaLight.cpp
	ColladaMaterial.cpp
	ColladaMatrix.cpp
	ColladaMesh.cpp
	ColladaNode.cpp
	ColladaPolyList.cpp
	ColladaPrimitive.cpp
	ColladaScene.cpp
	ColladaSkeleton.cpp
	ColladaSource.cpp
	ColladaTriangles.cpp
	ColladaVertices.cpp
	ColladaVisualScene.cpp
	
#	ColladaSceneLoader.cpp
#	CreateGeometry.cpp
#	CreateScene.cpp
)

include_directories(../tinyxml)
add_library(collada_static ${COLLADA_SRC})
