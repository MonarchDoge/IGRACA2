#pragma once

#include <gl/GL.h>
#include <gl/GLU.h>
#include "Material.h"

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

	double gravity = 9.81;

public:
	Projectile(double posX, double posY, double posZ, double velX, double velY, double velZ);

	void Draw();
	void Update(double deltaTime);

	~Projectile();
};

