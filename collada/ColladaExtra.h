/*
 * ColladaExtra.h
 *
 *  Created on: 20/04/2012
 *      Author: marrony.neris
 */

#ifndef COLLADAEXTRA_H_
#define COLLADAEXTRA_H_

#include "ColladaElement.h"

DECLARE_VISITOR(ColladaExtra);

class ColladaExtra : public ColladaElement {
public:
	static std::string elementType();

	virtual void loadFromXml(TiXmlElement* element);

	virtual void accept(Visitor* visitor);
};

#endif /* COLLADAEXTRA_H_ */
