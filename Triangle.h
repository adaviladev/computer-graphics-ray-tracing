#ifndef _TRIANGLE_H
#define _TRIANGLE_H

#include "math.h"
#include "Object.h"
#include "Vect.h"
#include "Ray.h"
#include "Color.h"

class Triangle : public Object {
	Vect vertexA, vertexB, vertexC;
	Vect normal;
	double distance;
	Color color;
public:
	Triangle();
	Triangle(Vect, Vect, Vect, Color);

	Vect getTriangleNormal()
	{
		Vect CA(
			this->vertexC.getVectX() - this->vertexA.getVectX(), 
			this->vertexC.getVectY() - this->vertexA.getVectY(), 
			this->vertexC.getVectZ() - this->vertexA.getVectZ()
		);
		Vect BA(
			this->vertexB.getVectX() - this->vertexA.getVectX(), 
			this->vertexB.getVectY() - this->vertexA.getVectY(), 
			this->vertexB.getVectZ() - this->vertexA.getVectZ()
		);
		this->normal = CA.crossProduct(BA).normalize();
		return this->normal;
	}

	double getTriangleDistance()
	{
		this->normal = getTriangleNormal();
		this->distance = this->normal.dotProduct(this->vertexA);
		return this->distance;
	}

	virtual Color getColor()
	{
		return this->color;
	}

	virtual Vect getNormalAt(Vect point) {
		this->normal = getTriangleNormal();
		return this->normal;
	}

	virtual double findIntersection(Ray ray) {
		Vect rayDirection = ray.getRayDirection();
		Vect rayOrigin = ray.getRayOrigin();

		this->normal = getTriangleNormal();
		this->distance = getTriangleDistance();

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
		double distanceToPlane = -1 * b / a;
		double Qx = rayDirection.multVector(distanceToPlane).getVectX() + rayOrigin.getVectX();
		double Qy = rayDirection.multVector(distanceToPlane).getVectY() + rayOrigin.getVectY();
		double Qz = rayDirection.multVector(distanceToPlane).getVectZ() + rayOrigin.getVectZ();
		Vect Q(Qx, Qy, Qz);

		// [CAxQA] * n >= 0
		Vect CA(
			this->vertexC.getVectX() - this->vertexA.getVectX(), 
			this->vertexC.getVectY() - this->vertexA.getVectY(), 
			this->vertexC.getVectZ() - this->vertexA.getVectZ()
		);
		Vect QA(
			Q.getVectX() - this->vertexA.getVectX(), 
			Q.getVectY() - this->vertexA.getVectY(), 
			Q.getVectZ() - this->vertexA.getVectZ()
		);
		double test1 = (CA.crossProduct(QA)).dotProduct(this->normal);
		// [BCxQC] * n >= 0
		Vect BC(
			this->vertexB.getVectX() - this->vertexC.getVectX(), 
			this->vertexB.getVectY() - this->vertexC.getVectY(), 
			this->vertexB.getVectZ() - this->vertexC.getVectZ()
		);
		Vect QC(
			Q.getVectX() - this->vertexC.getVectX(), 
			Q.getVectY() - this->vertexC.getVectY(), 
			Q.getVectZ() - this->vertexC.getVectZ()
		);
		double test2 = (BC.crossProduct(QC)).dotProduct(this->normal);
		// [ABxQB] * n >= 0
		Vect AB(
			this->vertexA.getVectX() - this->vertexB.getVectX(), 
			this->vertexA.getVectY() - this->vertexB.getVectY(), 
			this->vertexA.getVectZ() - this->vertexB.getVectZ()
		);
		Vect QB(
			Q.getVectX() - this->vertexB.getVectX(), 
			Q.getVectY() - this->vertexB.getVectY(), 
			Q.getVectZ() - this->vertexB.getVectZ()
		);
		double test3 = (AB.crossProduct(QB)).dotProduct(this->normal);

		if(test1 >= 0 && test2 >= 0 && test3 >= 0) {
			return -1 * b / a;
		}

		return -1;
	}
};

Triangle::Triangle() {
	this->vertexA = Vect(1,0,0);
	this->vertexB = Vect(0,1,0);
	this->vertexC = Vect(0,0,1);
	this->color = Color(0.5, 0.5, 0.5, 0);
}

Triangle::Triangle(Vect pointA, Vect pointB, Vect pointC, Color color) {
	this->vertexA = pointA;
	this->vertexB = pointB;
	this->vertexC = pointC;
	this->color = color;
}

#endif