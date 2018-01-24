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

void Tank::DrawTree(TreeNode* root) {

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

void Tank::HandleKeyDown(WPARAM wParam) {
	glPushMatrix();
	glLoadIdentity();
	switch (wParam) {
	case 'W':
		WisPressed = true;
		break;
	case 'S':
		SisPressed = true;
		break;
	case 'D':
		yRotation += 1;
		break;
	case 'A':
		yRotation -= 1;
		break;
	case VK_LEFT:
		glMultMatrixf(upperbase->matrix);
		glRotatef(-10, 0, 0, 1);
		glGetFloatv(GL_MODELVIEW_MATRIX, upperbase->matrix);
		break;
	case VK_RIGHT:
		glMultMatrixf(upperbase->matrix);
		glRotatef(10, 0, 0, 1);
		glGetFloatv(GL_MODELVIEW_MATRIX, upperbase->matrix);
		break;
	case VK_UP:
		glMultMatrixf(turrent->matrix);
		TurrentRotate(-5);
		glGetFloatv(GL_MODELVIEW_MATRIX, turrent->matrix);
		break;
	case VK_DOWN:
		glMultMatrixf(turrent->matrix);
		TurrentRotate(5);
		glGetFloatv(GL_MODELVIEW_MATRIX, turrent->matrix);
		break;
	}

	glPopMatrix();
}

void Tank::HandleKeyUp(WPARAM wParam) {
	glPushMatrix();
	glLoadIdentity();
	switch (wParam) {
	case 'W':
		dist = 1;
		glMultMatrixf(base->matrix);
		glGetFloatv(GL_MODELVIEW_MATRIX, base->matrix);
		WisPressed = false;
		break;
	case 'S':
		dist = -1;
		glMultMatrixf(base->matrix);
		glGetFloatv(GL_MODELVIEW_MATRIX, base->matrix);
		SisPressed = false;
		break;
	}

	glPopMatrix();
}

void Tank::DrawBase() {
	GLUquadric *Object = gluNewQuadric();
	glMaterialfv(GL_FRONT, GL_AMBIENT, redPlasticMaterial.ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, redPlasticMaterial.diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, redPlasticMaterial.specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, redPlasticMaterial.shininess);
	glPushMatrix();
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
	glPopMatrix();
}

void Tank::DrawUpperBase() {
	GLUquadric *Object = gluNewQuadric();
	glMaterialfv(GL_FRONT, GL_AMBIENT, yellowPlasticMaterial.ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, yellowPlasticMaterial.diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, yellowPlasticMaterial.specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, yellowPlasticMaterial.shininess);
	gluSphere(Object, 0.25, 64, 64);
}

void Tank::DrawTurrent() {
	GLUquadric *Object = gluNewQuadric();
	glMaterialfv(GL_FRONT, GL_AMBIENT, greenPlasticMaterial.ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, greenPlasticMaterial.diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, greenPlasticMaterial.specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, greenPlasticMaterial.shininess);
	glPushMatrix();
	glRotatef(-45, 1, 0, 0);
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
}

void Tank::TurrentRotate(float angle) {
	if (Rotationlimit > -45)
	{
		Rotationlimit = -45;
		angle = 0;
	}
	if (Rotationlimit < -90)
	{
		Rotationlimit = -90;
		angle = 0;
	}
	glRotatef(angle, 1, 0, 0);
}
