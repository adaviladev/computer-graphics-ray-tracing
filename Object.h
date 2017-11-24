#ifndef _OBJECT_H
#define _OBJECT_H

#include "Ray.h"
#include "Vect.h"
#include "CSColor.h"

class Object {
public:
	Object();

	virtual CSColor getColor() { return CSColor(0.0,0.0,0.0,0); }

	virtual Vect getNormalAt(Vect intersectionPosition) { 
		return Vect(0, 0, 0); 
	}

	virtual double findIntersection(Ray ray) {
		return 0;
	}
};

Object::Object() {}

#endif