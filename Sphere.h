#ifndef _SPHERE_H
#define _SPHERE_H

#include "math.h"
#include "Object.h"
#include "Vect.h"
#include "CSColor.h"

class Sphere : public Object {
	Vect center;
	double radius;
	CSColor color;
public:
	Sphere();
	Sphere(Vect, double, CSColor);

	Vect getSphereCenter() { return this->center; }

	double getSphereRadius() { return this->radius; }

	virtual CSColor getColor() { return this->color; }

	virtual Vect getNormalAt(Vect point) {
		// normal always points away from the center of a sphere
		Vect normal = point.addVector(this->center.negative()).normalize();
		return normal;
	}

	virtual double findIntersection(Ray ray) {
		Vect rayOrigin = ray.getRayOrigin();
		double rayOriginX = rayOrigin.getVectX();
		double rayOriginY = rayOrigin.getVectY();
		double rayOriginZ = rayOrigin.getVectZ();

		Vect rayDirection = ray.getRayDirection();
		double rayDirectionX = rayDirection.getVectX();
		double rayDirectionY = rayDirection.getVectY();
		double rayDirectionZ = rayDirection.getVectZ();

		Vect sphereCenter = this->center;
		double sphereCenterX = sphereCenter.getVectX();
		double sphereCenterY = sphereCenter.getVectY();
		double sphereCenterZ = sphereCenter.getVectZ();

		double a = 1; //Normalized
		double b = (2 * (rayOriginX - sphereCenterX) * rayDirectionX) 
				 + (2 * (rayOriginY - sphereCenterY) * rayDirectionY) 
				 + (2 * (rayOriginZ - sphereCenterZ) * rayDirectionZ);
		double c = pow(rayOriginX - sphereCenterX, 2)
				 + pow(rayOriginY - sphereCenterY, 2)
				 + pow(rayOriginZ - sphereCenterZ, 2)
				 - (this->radius * this->radius);

		double discriminant = b * b - 4 * a * c;

		if(discriminant > 0) {
			// The ray intersects the sphere
			
			// The first root
			double root1 = ((-1 * b - sqrt(discriminant)) / (2 * a)) - 0.000001;
			if(root1 > 0) {
				// The first root is the smallest positive root
				return root1;
			}
			double root2 = ((-1 * b + sqrt(discriminant)) / (2 * a)) - 0.000001;
			return root2;
		} else {
			// The ray missed the sphere
			return -1;
		}
	}
};

Sphere::Sphere() {
	this->center = Vect(0, 0, 0);
	this->radius = 1.0;
	this->color = CSColor(0.5, 0.5, 0.5, 0);
}

Sphere::Sphere(Vect center, double radius, CSColor color) {
	this->center = center;
	this->radius = radius;
	this->color = color;
}

#endif