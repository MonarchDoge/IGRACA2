#include "stdafx.h"
#include "Projectile.h"


Projectile::Projectile(double posX, double posY, double posZ, double velX, double velY, double velZ) :
	posX(posX), posY(posY), posZ(posZ), velX(velX), velY(velY), velZ(velZ)
{
	projectileMaterial = {
	{ 0.0f, 0.4f, 0.0f, 1.0f }, // Ambient 
	{ 0.0f, 0.9f, 0.0f, 1.0f }, // Diffuse 
	{ 0.8f, 0.8f, 0.8f, 1.0f }, // Specular  
	32.0    // Shininess 
	};
}

void Projectile::Draw()
{
	// Direction
	/*double */

	GLUquadric *Object = gluNewQuadric();
	glFrontFace(GL_CCW); // Front face is clockwise
	glMaterialfv(GL_FRONT, GL_AMBIENT, projectileMaterial.ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, projectileMaterial.diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, projectileMaterial.specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, projectileMaterial.shininess);
	glPushMatrix();
	glTranslatef(posX, posY, posZ);
	gluCylinder(Object, .15, .15, 1, 16, 16);
	glPopMatrix();
}

void Projectile::Update(double deltaTime)
{
	// Apply gravity
	velY -= gravity * deltaTime;

	// Apply velocity
	posX += velX * deltaTime;
	posY += velY * deltaTime;
	posZ += velZ * deltaTime;

	if (posY < -0.55) {
		posY = -0.55;

		velX = 0;
		velY = 0;
		velZ = 0;
	}
}

Projectile::~Projectile()
{
}
