/*
 * ColladaElement.h
 *
 *  Created on: 20/04/2012
 *      Author: marrony.neris
 */

#ifndef COLLADAELEMENT_H_
#define COLLADAELEMENT_H_

#include "Visitor.h"
#include "tinyxml.h"

#include <string>
#include <vector>
#include <iterator>
#include <sstream>

class ColladaElement {
public:
	template<typename T>
	static void eraseVector(std::vector<T*>& vector) {
		for(size_t i = 0; i < vector.size(); i++)
			delete vector[i];

		vector.clear();
	}

	static void loadAttribute(TiXmlElement* element, const char* attrName, std::string& value) {
		const char* attrValue = element->Attribute(attrName);

		if(attrValue)
			value = attrValue;
	}

	static void loadAttribute(TiXmlElement* element, const char* attrName, int& value) {
		element->Attribute(attrName, &value);
	}

	static void loadAttribute(TiXmlElement* element, const char* attrName, double& value) {
		element->Attribute(attrName, &value);
	}

	template<typename T>
	static void loadArray(TiXmlElement* element, std::vector<T>& values, bool validadeCount = false) {
		if(!element) return;

		std::string value = element->GetText();
		std::stringstream stream(value);
		std::istream_iterator<T> beginOfStream(stream);
		std::istream_iterator<T> endOfStream;

		std::copy(beginOfStream, endOfStream, std::back_inserter(values));

		if(validadeCount) {
			int count;
			if(element->Attribute("count", &count) == 0) {
				//throw Exception("attribute count is required");
			}
		}
	}

	template<typename Element>
	Element* createElementFromXml(TiXmlElement* element) {
		const std::string elemenType = Element::elementType();

		TiXmlElement* eleTag = element->FirstChildElement(elemenType.c_str());
		return createElementFromXml2<Element>(eleTag);
	}

	template<typename Element>
	Element* createElementFromXml2(TiXmlElement* element) {
		if(element != 0) {
			Element* newElement = new Element;
			newElement->loadFromXml(element);

			addElement(newElement);
			return newElement;
		}

		return 0;
	}

	template<typename Element>
	std::vector<Element*> createElementsFromXml(TiXmlElement* element) {
		std::vector<Element*> elements;

		Element* newElement = 0;

		const std::string elemenType = Element::elementType();

		TiXmlElement* eleTag = element->FirstChildElement(elemenType.c_str());
		while(eleTag != 0) {
			newElement = new Element;
			newElement->loadFromXml(eleTag);

			elements.push_back(newElement);
			addElement(newElement);

			eleTag = eleTag->NextSiblingElement(elemenType.c_str());
		}

		return elements;
	}

	void addElement(ColladaElement* element) {
		if(element) {
			element->parent = this;
			elements.push_back(element);
		}
	}

	ColladaElement* findElement(const std::string& url) {
		for(size_t i = 0; i < elements.size(); i++) {
			ColladaElement* element = elements[i];

			if(element->isElement(url))
				return element;
		}

		if(parent)
			return parent->findElement(url);

		return 0;
	}

	virtual void accept(engine::Visitor* visitor) = 0;

	const std::string& getId() const {
		return id;
	}

	const std::string& getName() const {
		return name;
	}
protected:
	std::string id;
	std::string name;
	ColladaElement* parent;
	std::vector<ColladaElement*> elements;

	ColladaElement() :
			id(), name(), parent(), elements() {
	}
public:
	virtual ~ColladaElement() {
	}

	virtual void loadFromXml(TiXmlElement* element) {
		loadAttribute(element, "id", id);
		loadAttribute(element, "name", name);
	}

	bool isElement(const std::string& url) const {
		std::string u = url.substr(1);

		return u == id || u == name;
	}
};

#endif /* COLLADAELEMENT_H_ */
