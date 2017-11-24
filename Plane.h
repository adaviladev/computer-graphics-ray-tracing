#ifndef _PLANE_H
#define _PLANE_H

#include "math.h"
#include "Object.h"
#include "Vect.h"
#include "Ray.h"
#include "CSColor.h"

class Plane : public Object {
	Vect normal;
	double distance;
	CSColor color;
public:
	Plane();
	Plane(Vect, double, CSColor);

	Vect getPlaneNormal()
	{
		return this->normal;
	}

	double getPlaneDistance()
	{
		return this->distance;
	}

	virtual CSColor getColor()
	{
		return this->color;
	}

	virtual Vect getNormalAt(Vect point) {
		return this->normal;
	}

	virtual double findIntersection(Ray ray) {
		Vect rayDirection = ray.getRayDirection();
		double a = rayDirection.dotProduct(this->normal);

		if(a == 0) {
			// ray is parallel to the plane
			return -1;
		}

		double b = this->normal.dotProduct(
			ray.getRayOrigin()
			.addVector(
				this->normal.multVector(this->distance)
				.negative()
			)
		);

		return -1 * b / a;
	}
};

Plane::Plane() {
	this->normal = Vect(1, 0, 0);
	this->distance = 1.0;
	this->color = CSColor(0.5, 0.5, 0.5, 0);
}

Plane::Plane(Vect normal, double distance, CSColor color) {
	this->normal = normal;
	this->distance = distance;
	this->color = color;
}

#endif