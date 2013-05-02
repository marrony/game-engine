/*
 * ColladaUtil.h
 *
 *  Created on: 22/04/2013
 *      Author: marrony
 */

#ifndef COLLADAUTIL_H_
#define COLLADAUTIL_H_

#include <tinyxml.h>
#include <vector>

namespace ColladaUtil {
	static inline void loadAttribute(TiXmlElement* element, const char* attrName, std::string& value) {
		const char* attrValue = element->Attribute(attrName);

		if(attrValue)
			value = attrValue;
	}

	static inline void loadAttribute(TiXmlElement* element, const char* attrName, int& value) {
		element->Attribute(attrName, &value);
	}

	static inline void loadAttribute(TiXmlElement* element, const char* attrName, double& value) {
		element->Attribute(attrName, &value);
	}

	template<typename T>
	static inline void eraseVector(std::vector<T*>& vector) {
		for(size_t i = 0; i < vector.size(); i++)
			delete vector[i];

		vector.clear();
	}

	template<typename T>
	static inline void loadArray(TiXmlElement* element, std::vector<T>& values, bool validadeCount = false) {
		if(!element) return;

		std::string value = element->GetText();
		std::stringstream stream(value);
		std::istream_iterator<T> beginOfStream(stream);
		std::istream_iterator<T> endOfStream;

		std::copy(beginOfStream, endOfStream, std::back_inserter(values));

		if(validadeCount) {
			int count;
			if(element->Attribute("count", &count)) {
				if(count != values.size()) {
					printf("invalid number of elements %d %zu", count, values.size());
				}
			}
		}
	}

	template<typename Element>
	static inline Element* createElementFromXml2(ColladaElement* colladaElement, TiXmlElement* element) {
		if(element != 0) {
			Element* newElement = new Element;
			newElement->loadFromXml(element);
			colladaElement->addElement(newElement);
			return newElement;
		}

		return 0;
	}

	template<typename Element>
	static inline Element* createElementFromXml(ColladaElement* colladaElement, TiXmlElement* element) {
		const std::string elemenType = Element::elementType();

		TiXmlElement* eleTag = element->FirstChildElement(elemenType.c_str());
		return createElementFromXml2<Element>(colladaElement, eleTag);
	}

	template<typename Element>
	static inline std::vector<Element*> createElementsFromXml(ColladaElement* colladaElement, TiXmlElement* element) {
		std::vector<Element*> elements;

		Element* newElement = 0;

		const std::string elemenType = Element::elementType();

		TiXmlElement* eleTag = element->FirstChildElement(elemenType.c_str());
		while(eleTag != 0) {
			newElement = new Element;
			newElement->loadFromXml(eleTag);

			elements.push_back(newElement);
			colladaElement->addElement(newElement);

			eleTag = eleTag->NextSiblingElement(elemenType.c_str());
		}

		return elements;
	}
}

#endif /* COLLADAUTIL_H_ */
