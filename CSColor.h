#ifndef _COLOR_H
#define _COLOR_H

#include "math.h"

class CSColor {
	double red, green, blue, special;
public:
	CSColor();
	CSColor(double, double, double, double);

	double getColorRed(){ return this->red; }

	double getColorGreen(){ return this->green; }

	double getColorBlue(){ return this->blue; }

	double getColorSpecial(){ return this->special; }

	void setColorRed(double value){ this->red = value; }

	void setColorGreen(double value){ this->green = value; }

	void setColorBlue(double value){ this->blue = value; }

	void setColorSpecial(double value){ this->special = value; }

	double brightness() {
		return (this->red + this->green + this->blue) / 3;
	}

	CSColor colorScalar(double scalar) {
		return CSColor(
			this->red * scalar,
			this->green * scalar,
			this->blue * scalar,
			special
		);
	}

	CSColor addColor(CSColor color) {
		return CSColor(
			this->red + color.getColorRed(),
			this->green + color.getColorGreen(),
			this->blue + color.getColorBlue(),
			this->special
		);
	}

	CSColor multColor(CSColor color) {
		return CSColor(
			this->red * color.getColorRed(),
			this->green * color.getColorGreen(),
			this->blue * color.getColorBlue(),
			this->special
		);
	}

	CSColor avgColor(CSColor color) {
		return CSColor(
			(this->red + color.getColorRed()) / 2,
			(this->green + color.getColorGreen()) / 2,
			(this->blue + color.getColorBlue()) / 2,
			this->special
		);
	}

	CSColor clip() {
		double allLight = this->red + this->green + this->blue;
		double excessLight = allLight - 3;
		if(excessLight > 0) {
			this->red = this->red + excessLight * (this->red / allLight);
			this->green = this->green + excessLight * (this->green / allLight);
			this->blue = this->blue + excessLight * (this->blue / allLight);
		}

		if(this->red > 1) { this->red = 1; }

		if(this->red < 0) { this->red = 0; }

		if(this->green > 1) { this->green = 1; }

		if(this->green < 0) { this->green = 0; }

		if(this->blue > 1) { this->blue = 1; }

		if(this->blue < 0) { this->blue = 0; }

		return CSColor(this->red, this->green, this->blue, this->special);
	}
};

CSColor::CSColor() {
	this->red = 0;
	this->green = 0;
	this->blue = 0;
}

CSColor::CSColor(double red, double green, double blue, double special) {
	this->red = red;
	this->green = green;
	this->blue = blue;
	this->special = special;
}

#endif