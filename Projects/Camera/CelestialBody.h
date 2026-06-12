#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class CELESTIAL {
private:
	float bodySize;
	float rotationSpeed;
	float axisTilt;
	float orbitRadius;
	float orbitSpeed;

public:
	//celestialBody(float bodySize,float rotationSpeed,float axisTilt,float orbitRadius,float orbitSpeed);
	void setbodySize(float size) {
		bodySize = size;
	}
};