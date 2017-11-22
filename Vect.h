#ifndef _VECT_H
#define _VECT_H

#include "math.h"

class Vect {
	double x, y, z;
public:
	Vect();
	Vect(double, double, double);

	double getVectX(){ return this->x; }

	double getVectY(){ return this->y; }

	double getVectZ(){ return this->z; }

	double magnitude() {
		return sqrt((this->x * this->x) + (this->y * this->y) + (this->z * this->z));
	}

	Vect normalize() {
		double magnitude = this->magnitude();

		return Vect(this->x / magnitude, this->y / magnitude, this->z / magnitude);
	}

	Vect negative() {
		return Vect(-this->x, -this->y, -this->z);
	}

	double dotProduct(Vect vect) {
		return this->x * vect.getVectX() + this->y * vect.getVectY() + this->z * vect.getVectZ();
	}

	Vect crossProduct(Vect vect) {
		return Vect(
			this->y * vect.getVectZ() - this->z * vect.getVectY(),
			this->z * vect.getVectX() - this->x * vect.getVectZ(),
			this->x * vect.getVectY() - this->y * vect.getVectX()
		);
	}

	Vect addVector(Vect vect) {
		return Vect(this->x + vect.getVectX(), this->y + vect.getVectY(), this->z + vect.getVectZ());
	}

	Vect multVector(double scalar) {
		return Vect(this->x * scalar, this->y * scalar, this->z * scalar);
	}
};

Vect::Vect() {
	this->x = 0;
	this->y = 0;
	this->z = 0;
}

Vect::Vect(double x, double y, double z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

#endif