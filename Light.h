#ifndef _LIGHT_H
#define _LIGHT_H

#include "Source.h"
#include "Vect.h"
#include "CSColor.h"

class Light : public Source {
	Vect position;
	CSColor color;
public:
	Light();
	Light(Vect, CSColor);

	virtual Vect getLightPosition()
	{
		return this->position;
	}

	virtual CSColor getLightColor()
	{
		return this->color;
	}
};

Light::Light() {
	this->position = Vect(0, 0, 0);
	this->color = CSColor(1, 1, 1, 0);
}

Light::Light(Vect position, CSColor color) {
	this->position = position;
	this->color = color;
}

#endif