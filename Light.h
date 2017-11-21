#ifndef _LIGHT_H
#define _LIGHT_H

#include "Source.h"
#include "Vect.h"
#include "Color.h"

class Light : public Source {
	Vect position;
	Color color;
public:
	Light();
	Light(Vect, Color);

	virtual Vect getLightPosition()
	{
		return this->position;
	}

	virtual Color getLightColor()
	{
		return this->color;
	}
};

Light::Light() {
	this->position = Vect(0, 0, 0);
	this->color = Color(1, 1, 1, 0);
}

Light::Light(Vect position, Color color) {
	this->position = position;
	this->color = color;
}

#endif