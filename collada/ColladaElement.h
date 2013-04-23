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

DECLARE_VISITOR(ColladaElement);

class ColladaElement {
public:
	void addElement(ColladaElement* element);

	ColladaElement* findElement(const std::string& url);

	virtual void accept(Visitor* visitor);

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

	ColladaElement();
public:
	virtual ~ColladaElement();

	virtual void loadFromXml(TiXmlElement* element);

	bool isElement(const std::string& url) const;

	const std::vector<ColladaElement*>& getElements() const { return elements; }
};

#endif /* COLLADAELEMENT_H_ */
