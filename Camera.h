#ifndef _CAMERA_H
#define _CAMERA_H

#include "Vect.h"

class Camera {
	Vect camPos, camDir, camRight, camDown;

public:
	Camera();
	Camera(Vect, Vect, Vect, Vect);

	Vect getCameraPosition() {
		return this->camPos;
	}

	Vect getCameraDirection() {
		return this->camDir;
	}

	Vect getCameraRight() {
		return this->camRight;
	}

	Vect getCameraDown() {
		return this->camDown;
	}
};

Camera::Camera() {
	this->camPos = Vect(0,0,0);
	this->camDir = Vect(0,0,1);
	this->camRight = Vect(0,0,0);
	this->camDown = Vect(0,0,0);
}

Camera::Camera(Vect camPos, Vect camDir, Vect camRight, Vect camDown ) {
	this->camPos = camPos;
	this->camDir = camDir;
	this->camRight = camRight;
	this->camDown = camDown;
}

#endif