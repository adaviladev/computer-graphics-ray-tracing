#ifndef _RAY_H
#define _RAY_H

#include "Vect.h"

class Ray {
	Vect origin, direction;
public:
	Ray();
	Ray(Vect, Vect);

	Vect getRayOrigin()
	{
		return this->origin;
	}

	Vect getRayDirection()
	{
		return this->direction;
	}
};

Ray::Ray() {
	this->origin = Vect(0, 0, 0);
	this->direction = Vect(1, 0, 0);
}

Ray::Ray(Vect origin, Vect direction) {
	this->origin = origin;
	this->direction = direction;
}

#endif