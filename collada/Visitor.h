/*
 * Visitor.h
 *
 *  Created on: 21/06/2012
 *      Author: marrony.neris
 */

#ifndef VISITOR_H_
#define VISITOR_H_

namespace engine {

	struct Visitor {
		virtual ~Visitor() {}
	};


}  // namespace engine

#define DECLARE_VISITOR(name)                    \
struct name##Visitor {                           \
	virtual ~name##Visitor() { }                  \
	virtual void visit(class name* element) = 0;  \
};

#define DEFINE_VISITOR(name)                                              \
void name::accept(engine::Visitor* elementVisitor) {                      \
	name##Visitor* visitor = dynamic_cast<name##Visitor*>(elementVisitor); \
	if(visitor) visitor->visit(this);                                      \
}

#define ACCEPT_METHOD() virtual void accept(engine::Visitor* visitor)

#endif /* VISITOR_H_ */
