#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <limits>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "Vect.h"
#include "Ray.h"
#include "Camera.h"
#include "Color.h"
#include "Source.h"
#include "Light.h"
#include "Object.h"
#include "Sphere.h"
#include "Plane.h"
#include "Triangle.h"

using namespace std;

int currentPixelIndex;
vector<Object*> scene_objects;

struct RGBType {
	double red;
	double green;
	double blue;
};

void saveImg (const char *filename, int w, int h, int dpi, RGBType *data) {
	FILE *f;
	int k = w*h;
	int s = 4*k;
	int filesize = 54 + s;
	
	double factor = 39.375;
	int m = static_cast<int>(factor);
	
	int ppm = dpi*m;
	
	unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0,0,0, 54,0,0,0};
	unsigned char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0,24,0};
	
	bmpfileheader[ 2] = (unsigned char)(filesize);
	bmpfileheader[ 3] = (unsigned char)(filesize>>8);
	bmpfileheader[ 4] = (unsigned char)(filesize>>16);
	bmpfileheader[ 5] = (unsigned char)(filesize>>24);
	
	bmpinfoheader[ 4] = (unsigned char)(w);
	bmpinfoheader[ 5] = (unsigned char)(w>>8);
	bmpinfoheader[ 6] = (unsigned char)(w>>16);
	bmpinfoheader[ 7] = (unsigned char)(w>>24);
	
	bmpinfoheader[ 8] = (unsigned char)(h);
	bmpinfoheader[ 9] = (unsigned char)(h>>8);
	bmpinfoheader[10] = (unsigned char)(h>>16);
	bmpinfoheader[11] = (unsigned char)(h>>24);
	
	bmpinfoheader[21] = (unsigned char)(s);
	bmpinfoheader[22] = (unsigned char)(s>>8);
	bmpinfoheader[23] = (unsigned char)(s>>16);
	bmpinfoheader[24] = (unsigned char)(s>>24);
	
	bmpinfoheader[25] = (unsigned char)(ppm);
	bmpinfoheader[26] = (unsigned char)(ppm>>8);
	bmpinfoheader[27] = (unsigned char)(ppm>>16);
	bmpinfoheader[28] = (unsigned char)(ppm>>24);
	
	bmpinfoheader[29] = (unsigned char)(ppm);
	bmpinfoheader[30] = (unsigned char)(ppm>>8);
	bmpinfoheader[31] = (unsigned char)(ppm>>16);
	bmpinfoheader[32] = (unsigned char)(ppm>>24);
	
	f = fopen(filename,"wb");
	
	fwrite(bmpfileheader,1,14,f);
	fwrite(bmpinfoheader,1,40,f);
	
	for (int i = 0; i < k; i++) {
		RGBType rgb = data[i];
		
		double red = (data[i].red)*255;
		double green = (data[i].green)*255;
		double blue = (data[i].blue)*255;
		
		unsigned char color[3] = {(int)floor(blue),(int)floor(green),(int)floor(red)};
		
		fwrite(color,1,3,f);
	}
	
	fclose(f);
}

int winningObjectIndex(vector<double> intersections) {
	int minIndexValue;

	if(intersections.size() == 0) {
		return -1;
	} else if(intersections.size() == 1) {
		if(intersections.at(0) > 0) {
			// If that intersection is greater than zero then its our min index
			return 0;
		}
		// Otherwise the only intersection value is negative
		return -1;
	}

	// Otherwise there is more than on intersection
	// First find the maxmum value
	
	double max = 0;
	for(int i = 0; i < intersections.size(); i++) {
		if(intersections[i] > max) {
			max = intersections.at(i);
		}
	}

	if(max > 0) {
		for(int i = 0; i < intersections.size(); i++) {
			if(intersections.at(i) > 0 && intersections.at(i) <= max) {
				max = intersections.at(i);
				minIndexValue = i;
			}
		}

		return minIndexValue;
	}

	return -1;
}

Color getColorAt(Vect intersectionPosition, Vect intersectionRayDirection, vector<Object*> scene_objects, int indexOfWinningObject, vector<Source*> lightSources, double accuracy, double ambientLight) {
	Color winningObjectColor = scene_objects.at(indexOfWinningObject)->getColor();
	Vect winningObjectNormal = scene_objects.at(indexOfWinningObject)->getNormalAt(intersectionPosition);

	if(winningObjectColor.getColorSpecial() == 2) {
		int square = (int)floor(intersectionPosition.getVectX()) 
				   // + (int)floor(intersectionPosition.getVectY()) 
				   + (int)floor(intersectionPosition.getVectZ());
		if(square % 2 == 0) {
			winningObjectColor.setColorRed(0);
			winningObjectColor.setColorGreen(0);
			winningObjectColor.setColorBlue(0);
		} else {
			winningObjectColor.setColorRed(1);
			winningObjectColor.setColorGreen(1);
			winningObjectColor.setColorBlue(1);
		}
	}

	Color finalColor = winningObjectColor.colorScalar(ambientLight);

	if(winningObjectColor.getColorSpecial() > 0 && winningObjectColor.getColorSpecial() <= 1) {
		double dot1 = winningObjectNormal.dotProduct(intersectionRayDirection.negative());
		Vect scalar1 = winningObjectNormal.multVector(dot1);
		Vect add1 = scalar1.addVector(intersectionRayDirection);
		Vect scalar2 = add1.multVector(2);
		Vect add2 = intersectionRayDirection.negative().addVector(scalar2);
		Vect reflectionDirection = add2.normalize();

		Ray reflectionRay(intersectionPosition, reflectionDirection);

		vector<double> reflectionIntersections;

		for(int reflectionIndex = 0; reflectionIndex < scene_objects.size(); reflectionIndex++) {
			reflectionIntersections.push_back(scene_objects.at(reflectionIndex)->findIntersection(reflectionRay));
		}

		int indexOfWinningObjectWithReflection = winningObjectIndex(reflectionIntersections);

		if(indexOfWinningObjectWithReflection != -1) {
			if(reflectionIntersections.at(indexOfWinningObjectWithReflection) > accuracy) {
				Vect reflectionIntersectionPosition = intersectionPosition.addVector(reflectionDirection.multVector(reflectionIntersections.at(indexOfWinningObjectWithReflection)));
				Vect reflectionIntersectionRayDirection = reflectionDirection;

				Color reflectionIntersectionColor = getColorAt(reflectionIntersectionPosition, reflectionIntersectionRayDirection, scene_objects, indexOfWinningObjectWithReflection, lightSources, accuracy, ambientLight);

				finalColor = finalColor.addColor(reflectionIntersectionColor.colorScalar(winningObjectColor.getColorSpecial()));
			}
		}
	}

	for(int lightIndex = 0; lightIndex < lightSources.size(); lightIndex++) {
		Vect lightDirection = lightSources.at(lightIndex)->getLightPosition()
														  .addVector(intersectionPosition.negative())
														  .normalize();


		float cosineAngle = winningObjectNormal.dotProduct(lightDirection);

		if(cosineAngle > 0) {
			// Test for shadows
			bool shadowed = false;

			Vect distanceToLight = lightSources.at(lightIndex)->getLightPosition()
															   .addVector(intersectionPosition.negative())
															   .normalize();

			float distanceToLightMagnitude = distanceToLight.magnitude();

			Ray shadowRay(
				intersectionPosition, 
				lightSources.at(lightIndex)->getLightPosition().addVector(intersectionPosition.negative()).normalize());
			vector<double> secondaryIntersections;
			for(int objectIndex = 0; objectIndex < scene_objects.size() && !shadowed; objectIndex++) {
				secondaryIntersections.push_back(scene_objects.at(objectIndex)->findIntersection(shadowRay));
			}

			for(int c = 0; c < secondaryIntersections.size(); c++) {
				if(secondaryIntersections.at(c) > accuracy) {
					if(secondaryIntersections.at(c) <= distanceToLightMagnitude) {
						shadowed = true;
					}
					break;
				}
			}

			if(!shadowed) {
				finalColor = finalColor.addColor(winningObjectColor.multColor(lightSources.at(lightIndex)->getColor()).colorScalar(cosineAngle));
				if(winningObjectColor.getColorSpecial() > 0 && winningObjectColor.getColorSpecial() <= 1) {
					// Special (0-1]
					double dot1 = winningObjectNormal.dotProduct(intersectionRayDirection.negative());
					Vect scalar1 = winningObjectNormal.multVector(dot1);
					Vect add1 = scalar1.addVector(intersectionRayDirection);
					Vect scalar2 = add1.multVector(2);
					Vect add2 = intersectionRayDirection.negative().addVector(scalar2);
					Vect reflectionDirection = add2.normalize();

					double specular = reflectionDirection.dotProduct(lightDirection);
					if(specular > 0) {
						specular = pow(specular, 10);
						finalColor = finalColor.addColor(lightSources.at(lightIndex)->getColor().colorScalar(specular*winningObjectColor.getColorSpecial()));
					}
				}
			}
		}
	}
	return finalColor.clip();
}

void makeCube(Vect corner1, Vect corner2, Color color) {
	// Corner 1
	double c1x = corner1.getVectX();
	double c1y = corner1.getVectY();
	double c1z = corner1.getVectZ();
	// Corner 2
	double c2x = corner2.getVectX();
	double c2y = corner2.getVectY();
	double c2z = corner2.getVectZ();

	Vect A(c2x, c1y, c1z);
	Vect B(c2x, c1y, c2z);
	Vect C(c1x, c1y, c2z);

	Vect D(c2x, c2y, c1z);
	Vect E(c1x, c2y, c1z);
	Vect F(c1x, c2y, c2z);

	// Left face
	scene_objects.push_back(new Triangle(D, A, corner1, color));
	scene_objects.push_back(new Triangle(corner1, E, D, color));
	// Far side
	scene_objects.push_back(new Triangle(corner2, B, A, color));
	scene_objects.push_back(new Triangle(A, D, corner2, color));
	// Right side
	scene_objects.push_back(new Triangle(F, C, B, color));
	scene_objects.push_back(new Triangle(B, corner2, F, color));
	// Front side
	scene_objects.push_back(new Triangle(E, corner1, C, color));
	scene_objects.push_back(new Triangle(C, F, E, color));
	// Top side
	scene_objects.push_back(new Triangle(D, E, F, color));
	scene_objects.push_back(new Triangle(F, corner2, D, color));
	// Bottom side
	scene_objects.push_back(new Triangle(corner1, A, B, color));
	scene_objects.push_back(new Triangle(B, C, corner1, color));
	
}

int main(int argc, char *argv[])
{
	cout << "rendering..." << endl;

	clock_t startTime, endTime;
	startTime = clock();

	int dpi = 72;
	int width = 640;
	int height = 480;
	int area = width * height;
	RGBType *pixels = new RGBType[area];

	int aadepth = 2;
	double aathreshold = 0.1;
	double aspectRatio = (double)width / (double)height;
	double ambientLight = 0.2;
	double accuracy = 0.000001;

	Vect origin(0,0,0);
	Vect X(1,0,0);
	Vect Y(0,1,0);
	Vect Z(0,0,1);

	Vect sphereLocation2(1.75, -0.25, 0);

	Vect camPos(3, 1.5, -4);

	Vect look_at(0,0,0);
	Vect diff_btw(
		camPos.getVectX() - look_at.getVectX(),
		camPos.getVectY() - look_at.getVectY(),
		camPos.getVectZ() - look_at.getVectZ()
	);

	Vect camDir = diff_btw.negative().normalize();
	Vect camRight = Y.crossProduct(camDir).normalize();
	Vect camDown = camRight.crossProduct(camDir);
	Camera scene_camera(camPos, camDir, camRight, camDown);

	Color white_light(1.0, 1.0, 1.0, 0);
	Color pretty_green(0.5, 1.0, 0.5, 0.3);
	Color tile_floor(1, 1, 1, 2);
	Color maroon(0.5, 0.25, 0.25, 0);
	Color orange(0.94, 0.75, 0.31, 0);
	Color gray(0.5, 0.5, 0.5, 0);
	Color black(0, 0, 0, 0);

	Vect light_position(-7, 10, -10);
	Light scene_light(light_position, white_light);
	vector<Source*> lightSources;
	lightSources.push_back(dynamic_cast<Source*>(&scene_light));

	// Scene Objects
	Sphere scene_sphere(origin, 1, pretty_green);
	Sphere scene_sphere2(sphereLocation2, .5, maroon);
	Plane scene_plane(Y, -1, tile_floor);
	Triangle scene_triangle(Vect(3,0,0), Vect(0,3,0), Vect(0,0,3), orange);

	scene_objects.push_back(dynamic_cast<Object*>(&scene_sphere));
	scene_objects.push_back(dynamic_cast<Object*>(&scene_sphere2));
	scene_objects.push_back(dynamic_cast<Object*>(&scene_plane));
	// scene_objects.push_back(dynamic_cast<Object*>(&scene_triangle));
	
	makeCube(Vect(1, 1, 1), Vect(-1, -1, -1), orange);

	int aaIndex = 0;
	double xAmount, yAmount;
	double tempRed, tempGreen, tempBlue;

	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			currentPixelIndex = y * width + x;

			// Start with a blank pixel
			double tempRed[aadepth * aadepth];
			double tempGreen[aadepth * aadepth];
			double tempBlue[aadepth * aadepth];

			/**
			 * 45.40
			 */
			for(int aax = 0; aax < aadepth; aax++) {
				for(int aay = 0; aay < aadepth; aay++) {

					aaIndex = aay * aadepth + aax;
					srand(time(0));

					// Create the ray from the camera to this pixel
					if(aadepth == 1) {
						// Start with no anti-aliasing
						if(width > height) {
							// The image is wider than it is tall
							xAmount = ((x + 0.5)/width) * aspectRatio - (((width - height) / (double)height) / 2);
							yAmount = ((height - y) + 0.5) / height;
						} else if(height > width) {
							// The image is taller than it is wide
							xAmount = (x + 0.5) / width;
							yAmount = (((height - y) + 0.5) / height) / aspectRatio - (((height - width) / (double)width) / 2);
						} else {
							// The image is square
							xAmount = (x + 0.5) / width;
							yAmount = ((height - y) + 0.5) / height;
						}
					} else {
						// Anti-aliasing
						if(width > height) {
							// The image is wider than it is tall
							xAmount = ((x + (double)aax/((double)aadepth - 1))/width) * aspectRatio - (((width - height) / (double)height) / 2);
							yAmount = ((height - y) + (double)aax/((double)aadepth - 1)) / height;
						} else if(height > width) {
							// The image is taller than it is wide
							xAmount = (x + (double)aax/((double)aadepth - 1)) / width;
							yAmount = (((height - y) + (double)aax/((double)aadepth - 1)) / height) / aspectRatio - (((height - width) / (double)width) / 2);
						} else {
							// The image is square
							xAmount = (x + (double)aax/((double)aadepth - 1)) / width;
							yAmount = ((height - y) + (double)aax/((double)aadepth - 1)) / height;
						}
					}

					Vect camRayOrigin = scene_camera.getCameraPosition();
					Vect camRayDirection = camDir.addVector(
						camRight.multVector(xAmount - 0.5)
						.addVector(
							camDown.multVector(yAmount - 0.5)
						)
					).normalize();

					Ray camRay(camRayOrigin, camRayDirection);

					vector<double> intersections;

					for(int i = 0; i < scene_objects.size(); i++) {
						intersections.push_back(scene_objects.at(i)->findIntersection(camRay));
					}

					int indexOfWinningObject = winningObjectIndex(intersections);

					if(indexOfWinningObject == -1) {
						tempRed[aaIndex] = 0;
						tempGreen[aaIndex] = 0;
						tempBlue[aaIndex] = 0;
					} else {
						if(intersections.at(indexOfWinningObject) > accuracy) {
							// Determine the position and direction vectors at the point of intersection
							Vect intersectionPosition = camRayOrigin.addVector(
								camRayDirection.multVector(
									intersections.at(indexOfWinningObject)
								)
							);
							Vect intersectionRayDirection = camRayDirection;

							Color intersectionColor = getColorAt(intersectionPosition, intersectionRayDirection, scene_objects, indexOfWinningObject, lightSources, accuracy, ambientLight);

							tempRed[aaIndex] = intersectionColor.getColorRed();
							tempGreen[aaIndex] = intersectionColor.getColorGreen();
							tempBlue[aaIndex] = intersectionColor.getColorBlue();
						}
					}
				}	
			}

			double totalRed = 0;
			double totalGreen = 0;
			double totalBlue = 0;

			for(int iRed = 0; iRed < aadepth * aadepth; iRed++) {
				totalRed += tempRed[iRed];
			}

			for(int iGreen = 0; iGreen < aadepth * aadepth; iGreen++) {
				totalGreen += tempGreen[iGreen];
			}

			for(int iBlue = 0; iBlue < aadepth * aadepth; iBlue++) {
				totalBlue += tempBlue[iBlue];
			}

			double avgRed = totalRed / (aadepth * aadepth);
			double avgGreen = totalGreen / (aadepth * aadepth);
			double avgBlue = totalBlue / (aadepth * aadepth);

			pixels[currentPixelIndex].red = avgRed;
			pixels[currentPixelIndex].green = avgGreen;
			pixels[currentPixelIndex].blue = avgBlue;
		}
	}

	saveImg("scene_aa_x2.bmp", width, height, dpi, pixels);

	delete pixels, tempRed, tempGreen, tempBlue;

	endTime = clock();
	float totalTime = ((float)endTime - (float)(startTime)) / 1000;

	cout << totalTime << "sec" << endl;

	return 0;
}