#pragma once

#include "stdafx.h"
#include "Tank.h"
#include <math.h>
#include <stdio.h>
#include "Clock.h"

//for igra - openGL
#include <gl\GL.h>
#include<gl\GLU.h>

Tank::Tank() {
	redPlasticMaterial = {
	{ 0.4f, 0.0f, 0.0f, 1.0f }, // Ambient 
	{ 0.9f, 0.0f, 0.0f, 1.0f }, // Diffuse 
	{ 0.8f, 0.8f, 0.8f, 1.0f }, // Specular  
	32.0    // Shininess 
	};
	greenPlasticMaterial = {
	{ 0.0f, 0.4f, 0.0f, 1.0f }, // Ambient 
	{ 0.0f, 0.9f, 0.0f, 1.0f }, // Diffuse 
	{ 0.8f, 0.8f, 0.8f, 1.0f }, // Specular  
	32.0    // Shininess 
	};
	yellowPlasticMaterial = {
	{ 0.4f,  0.4f, 0.0f, 1.0f }, // Ambient 
	{ 0.9f, 0.9f, 0.0f, 1.0f }, // Diffuse 
	{ 0.8f, 0.8f, 0.8f, 1.0f }, // Specular  
	32   // Shininess 
	};

	clock = new Clock;
	clock->Start();
	TurrentHeight = 1.5;

	BuildTree();
}

void Tank::Draw() {
	DrawTree(base);

	for (int i = 0; i < projectiles.size(); i++) {
		projectiles[i].Draw();
	}
}

void Tank::DrawFunction(int id)
{
	switch (id) {
	case DRAW_BASE_FUNCTION_ID:
		DrawBase();
		break;
	case DRAW_UPPERBASE_FUNCTION_ID:
		DrawUpperBase();
		break;
	case DRAW_TURRENT_FUNCTION_ID:
		DrawTurrent();
		break;
	}
}

void Tank::DrawTree(TreeNode* root)
{
	if (root == 0)
		return;
	glColor3f(0, 0, 0);

	glPushMatrix();
	glMultMatrixf(root->matrix);
	DrawFunction(root->drawFunctionID);

	if (root->child != 0)
		DrawTree(root->child);
	glPopMatrix();
}

void Tank::HandleKeyDown(WPARAM wParam)
{
	keysDown[wParam] = true;
	switch (wParam) {
	case VK_SPACE:
		// Spawn projectile
		double velX = sin(degToRad(yRotation + turretRotHorizontal)) * cos(degToRad(turretRotVertical)) * GetRealBulletSpeed(bulletSpeed);
		double velY = sin(degToRad(turretRotVertical)) * GetRealBulletSpeed(bulletSpeed);
		double velZ = cos(degToRad(yRotation + turretRotHorizontal)) * cos(degToRad(turretRotVertical)) * GetRealBulletSpeed(bulletSpeed);
		Projectile newProjectile = Projectile(xPos, 0, yPos, velX, velY, velZ);
		projectiles.push_back(newProjectile);
		break;
	}
}

void Tank::HandleKeyUp(WPARAM wParam) {
	keysDown[wParam] = false;
}

void Tank::DrawBase() {
	GLUquadric *Object = gluNewQuadric();
	glMaterialfv(GL_FRONT, GL_AMBIENT, redPlasticMaterial.ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, redPlasticMaterial.diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, redPlasticMaterial.specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, redPlasticMaterial.shininess);
	float cubeVertices[][3] = {
	{ -0.5f, -1.0f, 0.3f }, // v0
	{ -0.2f, 0.8f, 0.3f }, // v1
	{ 0.2f, 0.8f, 0.3f }, // v2
	{ 0.5f, -1.0f, 0.3f }, // v3
	{ -0.5f, -1.0f, -0.6f }, // v4
	{ -0.2f, 1.0f, -0.6f }, // v5
	{ 0.2f, 1.0f, -0.6f }, // v6
	{ 0.5f, -1.0f, -0.6f }, // v7
	};

	int cubeIndices[] = {
		0, 1, 2, 3, // Front Face
		7, 6, 5, 4, // Back Face
		2, 6, 7, 3, // Right Face
		0, 4, 5, 1, // Left Face
		1, 5, 6, 2, // Top Face
		3, 7, 4, 0 // Bottom Face
	};

	float cubeColors[][3] = {
		0.5, 0.5, 0.5 ,
		0.5, 0.5, 0.5 ,
		0.5, 0.5, 0.5 ,
		0.5, 0.5, 0.5 ,
		0.5, 0.5, 0.5 ,
		0.5, 0.5, 0.5
	};

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW); // Front face is clockwise
	glPolygonMode(GL_FRONT, GL_FILL);
	glColor3f(0.0, 0.0, 0.0);
	// The index in cubeIndices array
	int index = 0;
	// the cube quad by quad
	for (int qd = 0; qd < 6; qd++) {
		glBegin(GL_QUADS);
		glColor3f(cubeColors[qd][0], cubeColors[qd][1],
			cubeColors[qd][2]);
		for (int v = 0; v < 4; v++) {// Four vertices for one quad
			glVertex3f(cubeVertices[cubeIndices[index]][0],
				cubeVertices[cubeIndices[index]][1],
				cubeVertices[cubeIndices[index]][2]);
			index++; // Move to next vertex in quad
		}
		glEnd();
	}
}

void Tank::DrawUpperBase() {
	GLUquadric *Object = gluNewQuadric();
	glFrontFace(GL_CCW); // Front face is clockwise
	glMaterialfv(GL_FRONT, GL_AMBIENT, yellowPlasticMaterial.ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, yellowPlasticMaterial.diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, yellowPlasticMaterial.specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, yellowPlasticMaterial.shininess);
	gluSphere(Object, 0.25, 64, 64);
}

void Tank::DrawTurrent() {
	GLUquadric *Object = gluNewQuadric();
	glFrontFace(GL_CCW); // Front face is clockwise
	glMaterialfv(GL_FRONT, GL_AMBIENT, greenPlasticMaterial.ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, greenPlasticMaterial.diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, greenPlasticMaterial.specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, greenPlasticMaterial.shininess);
	glPushMatrix();
	glRotatef(90, 1, 0, 0);
	gluCylinder(Object, .15, .15, TurrentHeight, 16, 16);
	glPopMatrix();
}

float Tank::degToRad(float degAngle) {
	float pi = 3.1415f; // You could be more precise!
	return degAngle / 180 * pi;
}

void Tank::BuildTree() {
	glMatrixMode(GL_MODELVIEW); // The root of the tree (represents the base of Robot Arm)   
	base = new TreeNode;
	base->child = 0;
	base->drawFunctionID = DRAW_BASE_FUNCTION_ID;
	glLoadIdentity();
	glRotatef(-90, 1, 0, 0);
	glGetFloatv(GL_MODELVIEW_MATRIX, base->matrix);

	glLoadIdentity();
	upperbase = new TreeNode;
	upperbase->child = 0;
	upperbase->drawFunctionID = DRAW_UPPERBASE_FUNCTION_ID;
	glTranslatef(0, 0, 0.4f);
	glGetFloatv(GL_MODELVIEW_MATRIX, upperbase->matrix);

	glLoadIdentity();
	turrent = new TreeNode;
	turrent->child = 0;
	turrent->drawFunctionID = DRAW_TURRENT_FUNCTION_ID;
	glTranslatef(0, 0, 0.1f);
	glGetFloatv(GL_MODELVIEW_MATRIX, turrent->matrix);

	base->child = upperbase;
	upperbase->child = turrent;
}

void Tank::Update() {
	double deltaTime = clock->TimePassedSincePreviousTime();

	for (int i = 0; i < projectiles.size(); i++) {
		projectiles[i].Update(deltaTime);
	}

	// Rotate
	if (keysDown['A']) yRotation += turnSpeed * deltaTime;
	if (keysDown['D']) yRotation -= turnSpeed * deltaTime;

	// Accelerate
	double currentAccel = 0;
	if (keysDown['W']) currentAccel += acceleration;
	if (keysDown['S']) currentAccel -= acceleration;
	// dir has magnitude of 1
	double xDir = sin(degToRad(yRotation));
	double yDir = cos(degToRad(yRotation));
	double xAccel = xDir * currentAccel;
	double yAccel = yDir * currentAccel;

	// Apply acceleration
	xVel += xAccel * deltaTime;
	yVel += yAccel * deltaTime;

	{
		double velMag = sqrt(xVel*xVel + yVel * yVel);
		if (velMag > 0.01)
		{
			// Limit speed
			if (velMag > maxSpeed)
			{
				double limitedMag = maxSpeed;
				double constant = limitedMag / velMag;
				xVel *= constant;
				yVel *= constant;
			}
		}
	}

	{
		double velMag = sqrt(xVel*xVel + yVel * yVel);
		if (velMag > 0.01)
		{
			// Apply friction
			double afterFrictionVelMag = max(velMag - friction * deltaTime, 0);
			double constant = afterFrictionVelMag / velMag;
			xVel *= constant;
			yVel *= constant;
		}
	}

	// Apply velocity
	xPos += xVel * deltaTime;
	yPos += yVel * deltaTime;

	// Turn turret
	if (keysDown[VK_RIGHT]) turretRotHorizontal -= turretTurnSpeed * deltaTime;
	if (keysDown[VK_LEFT]) turretRotHorizontal += turretTurnSpeed * deltaTime;
	if (keysDown[VK_UP]) TurrentRotate(turretAltSpeed * deltaTime);
	if (keysDown[VK_DOWN]) TurrentRotate(-turretAltSpeed * deltaTime);

	// Update base matrix
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(xPos, 0, yPos);
	glRotatef(yRotation, 0, 1, 0);
	glRotatef(-90, 1, 0, 0);
	glGetFloatv(GL_MODELVIEW_MATRIX, base->matrix);
	glPopMatrix();

	// Update turret matrix
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(0, 0, 0.1f);
	glRotatef(turretRotHorizontal, 0, 0, 1);
	glRotatef(-turretRotVertical, 1, 0, 0);
	glGetFloatv(GL_MODELVIEW_MATRIX, turrent->matrix);
	glPopMatrix();
}

void Tank::OrbitTank(float x, float y) {
	gluLookAt(xPos + x * 10, max(y * 10, 0), yPos - 10, // Camera's position
		xPos, 0, yPos, // Camera's target to look at
		0, 1, 0); // Orientation of camera
}

double Tank::GetRealBulletSpeed(int bulletSpeed)
{
	switch (bulletSpeed) {
	case BULLETSPEED_SLOW: return 5;
	case BULLETSPEED_MEDIUM: return 10;
	case BULLETSPEED_FAST: return 20;
	}
}

void Tank::TurrentRotate(float angle) {
	double originalRotY = turretRotVertical;

	turretRotVertical += angle;
	if (turretRotVertical > 45) turretRotVertical = 45;
	if (turretRotVertical < 0) turretRotVertical = 0;
}
