/*
 * ColladaElement.cpp
 *
 *  Created on: 20/04/2012
 *      Author: marrony.neris
 */

#include "ColladaElement.h"
#include "ColladaUtil.h"

void ColladaElement::addElement(ColladaElement* element) {
	if(element) {
		element->parent = this;
		elements.push_back(element);
	}
}

void ColladaElement::accept(Visitor* visitor) {
	for(size_t i = 0; i < elements.size(); i++) {
		ColladaElement* element = elements[i];
		element->accept(visitor);
	}

	if(ColladaElementVisitor* elementVisitor = dynamic_cast<ColladaElementVisitor*>(visitor))
		elementVisitor->visit(this);
}

ColladaElement* ColladaElement::findElement(const std::string& url) {
	for(size_t i = 0; i < elements.size(); i++) {
		ColladaElement* element = elements[i];

		if(element->isElement(url))
			return element;
	}

	if(parent)
		return parent->findElement(url);

	return 0;
}

ColladaElement::ColladaElement() :
		id(), name(), parent(), elements() {
}

ColladaElement::~ColladaElement() {
}

void ColladaElement::loadFromXml(TiXmlElement* element) {
	ColladaUtil::loadAttribute(element, "id", id);
	ColladaUtil::loadAttribute(element, "name", name);
}

bool ColladaElement::isElement(const std::string& url) const {
	std::string u = url.substr(1);

	return u == id || u == name;
}

