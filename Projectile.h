#pragma once

#include <gl/GL.h>
#include <gl/GLU.h>
#include "Material.h"
#include <cmath>
#include "RadDeg.h"

class Projectile
{
private:
	Material projectileMaterial;

	double posX;
	double posY;
	double posZ;

	double velX;
	double velY;
	double velZ;

	double rotHorizontal = 0;
	double rotVertical = 0;

	double gravity = 9.81;
	double stopY = -0.4;

public:
	Projectile(double posX, double posY, double posZ, double velX, double velY, double velZ);

	void Draw();
	void Update(double deltaTime);

	~Projectile();
};

