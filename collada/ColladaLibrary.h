/*
 * ColladaLibrary.h
 *
 *  Created on: 20/04/2012
 *      Author: marrony.neris
 */

#ifndef COLLADALIBRARY_H_
#define COLLADALIBRARY_H_

#include "ColladaElement.h"
#include "ColladaAsset.h"
#include "ColladaExtra.h"

#include <vector>

template<typename Element>
class ColladaLibrary;

template<typename Element>
struct ColladaLibraryVisitor {
	virtual ~ColladaLibraryVisitor() { }
	virtual void visit(ColladaLibrary<Element>* element) = 0;
};

template<typename Element>
class ColladaLibrary : public ColladaElement {
	ColladaAsset* asset;
	std::vector<ColladaExtra*> extras;
	std::vector<Element*> items;
public:
	virtual ~ColladaLibrary() {
		delete asset;
		eraseVector(extras);
		eraseVector(items);
	}

	static std::string elementType() {
		std::string elementType = Element::elementType();
		std::string key = "y";
		std::size_t found = elementType.rfind(key);

		if(found != std::string::npos) {
			elementType.replace(found, key.length(), "ies");
		} else {
			elementType += "s";
		}

		return "library_" + elementType;
	}

	virtual void loadFromXml(TiXmlElement* element) {
		ColladaElement::loadFromXml(element);

		asset = createElementFromXml<ColladaAsset>(element);
		extras = createElementsFromXml<ColladaExtra>(element);
		items = createElementsFromXml<Element>(element);
	}

	virtual void accept(engine::Visitor* visitor) {
		ColladaLibraryVisitor<Element>* v = dynamic_cast<ColladaLibraryVisitor<Element>*>(visitor);
		if(v) v->visit(this);
	}

	std::vector<Element*> getItems() {
		return items;
	}

//	Element* findElement(const std::string& url) {
//		for(Element* element : items) {
//			if(element->isElement(url))
//				return element;
//		}
//
//		return 0;
//	}
};

#endif /* COLLADALIBRARY_H_ */
