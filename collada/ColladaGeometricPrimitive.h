/*
 * ColladaGeometricPrimitive.h
 *
 *  Created on: 20/04/2012
 *      Author: marrony.neris
 */

#ifndef COLLADAGEOMETRICPRIMITIVE_H_
#define COLLADAGEOMETRICPRIMITIVE_H_

#include "ColladaElement.h"

#include <string>
#include <vector>

class ColladaInput;
class ColladaSource;

DECLARE_VISITOR(ColladaGeometricPrimitive);

class ColladaGeometricPrimitive : public ColladaElement {
protected:
	std::string material;
	std::vector<ColladaInput*> inputs;
	std::vector<std::vector<int> > primitive;
public:
	virtual ~ColladaGeometricPrimitive();

	virtual void loadFromXml(TiXmlElement* element);

	virtual void accept(Visitor* visitor);

	ColladaInput* findInputWithSemantic(const std::string& semantic);

	const std::string& getMaterial() const { return material; }
	const std::vector<ColladaInput*>& getInputs() const { return inputs; }
	const std::vector<std::vector<int> >& getPrimitive() const { return primitive; }
};

#endif /* COLLADAGEOMETRICPRIMITIVE_H_ */
