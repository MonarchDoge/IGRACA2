#pragma once

#include "stdafx.h"
#include "RobotArm.h"
#include <math.h>
#include <stdio.h>

//for igra - openGL
#include <gl\GL.h>
#include<gl\GLU.h>


RobotArm::RobotArm() {
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
	ArmHeight = 1;
	BuildTree();

}

void RobotArm::Draw() {
	DrawTree(base);
}

void RobotArm::DrawFunction(int id)
{
	switch (id) {
	case DRAW_BASE_FUNCTION_ID:
		DrawBase();
		break;
	case DRAW_LOWERARM_FUNCTION_ID:
		DrawLowerArm();
		break;
	case DRAW_UPPERARM_FUNCTION_ID:
		DrawUpperArm();
		break;
	case DRAW_JOINT_FUNCTION_ID:
		DrawJoint();
		break;
	}
}


void RobotArm::DrawTree(TreeNode* root) {

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

void RobotArm::HandleKeyDown(WPARAM wParam) {
	glPushMatrix();
	glLoadIdentity();
	switch (wParam) {
	case VK_LEFT:
		glMultMatrixf(base->matrix);
		glRotatef(-10, 0, 0, 1);
		glGetFloatv(GL_MODELVIEW_MATRIX, base->matrix);
		break;
	case VK_RIGHT:
		glMultMatrixf(base->matrix);
		glRotatef(10, 0, 0, 1);
		glGetFloatv(GL_MODELVIEW_MATRIX, base->matrix);
		break;
	case VK_UP:
		glMultMatrixf(lowerarmnode->matrix);
		glRotatef(-10, 1, 0, 0);
		glGetFloatv(GL_MODELVIEW_MATRIX, lowerarmnode->matrix);
		break;
	case VK_DOWN:
		glMultMatrixf(lowerarmnode->matrix);
		glRotatef(10, 1, 0, 0);
		glGetFloatv(GL_MODELVIEW_MATRIX, lowerarmnode->matrix);
		break;
	case VK_PRIOR:
		glMultMatrixf(upperarmnode->matrix);
		glRotatef(-10, 1, 0, 0);
		glGetFloatv(GL_MODELVIEW_MATRIX, upperarmnode->matrix);
		break;
	case VK_NEXT:
		glMultMatrixf(upperarmnode->matrix);
		glRotatef(10, 1, 0, 0);
		glGetFloatv(GL_MODELVIEW_MATRIX, upperarmnode->matrix);
		break;

	}
	glPopMatrix();
}

void RobotArm::DrawJoint() {
	GLUquadric *Object = gluNewQuadric();
	glMaterialfv(GL_FRONT, GL_AMBIENT, yellowPlasticMaterial.ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, yellowPlasticMaterial.diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, yellowPlasticMaterial.specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, yellowPlasticMaterial.shininess);
	gluSphere(Object, 0.25, 64, 64);
}

void RobotArm::DrawBase() {
	GLUquadric *Object = gluNewQuadric();
	glMaterialfv(GL_FRONT, GL_AMBIENT, redPlasticMaterial.ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, redPlasticMaterial.diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, redPlasticMaterial.specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, redPlasticMaterial.shininess);
	gluCylinder(Object, .4, .4, .45, 16, 16);
	glPushMatrix();
	glTranslatef(0, 0, 0.45);
	gluDisk(Object, 0, .4, 16, 16);
	glPopMatrix();
}


void RobotArm::DrawUpperArm() {
	GLUquadric *Object = gluNewQuadric();
	glMaterialfv(GL_FRONT, GL_AMBIENT, greenPlasticMaterial.ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, greenPlasticMaterial.diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, greenPlasticMaterial.specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, greenPlasticMaterial.shininess);
	gluCylinder(Object, .15, .15, ArmHeight, 16, 16);
}

void RobotArm::DrawLowerArm() {
	GLUquadric *Object = gluNewQuadric();
	glMaterialfv(GL_FRONT, GL_AMBIENT, redPlasticMaterial.ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, redPlasticMaterial.diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, redPlasticMaterial.specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, redPlasticMaterial.shininess);
	gluCylinder(Object, .15, .15, ArmHeight, 16, 16);
}


void RobotArm::BuildTree() {

	glMatrixMode(GL_MODELVIEW); // The root of the tree (represents the base of Robot Arm)   
	base = new TreeNode;
	base->child = 0;
	base->drawFunctionID = DRAW_BASE_FUNCTION_ID;
	glLoadIdentity();
	glRotatef(-90, 1, 0, 0);
	glGetFloatv(GL_MODELVIEW_MATRIX, base->matrix);

	glLoadIdentity();
	lowerarmjoint = new TreeNode;
	lowerarmjoint->child = 0;
	lowerarmjoint->drawFunctionID = DRAW_JOINT_FUNCTION_ID;
	glTranslatef(0, 0, 0.4);
	glGetFloatv(GL_MODELVIEW_MATRIX, lowerarmjoint->matrix);

	glLoadIdentity();
	lowerarmnode = new TreeNode;
	lowerarmnode->child = 0;
	lowerarmnode->drawFunctionID = DRAW_LOWERARM_FUNCTION_ID;
	glTranslatef(0, 0, 0.1);
	glGetFloatv(GL_MODELVIEW_MATRIX, lowerarmnode->matrix);

	glLoadIdentity();
	upperarmjoint = new TreeNode;
	upperarmjoint->child = 0;
	upperarmjoint->drawFunctionID = DRAW_JOINT_FUNCTION_ID;
	glTranslatef(0, 0, ArmHeight);
	glGetFloatv(GL_MODELVIEW_MATRIX, upperarmjoint->matrix);

	glLoadIdentity();
	upperarmnode = new TreeNode;
	upperarmnode->child = 0;
	upperarmnode->drawFunctionID = DRAW_UPPERARM_FUNCTION_ID;
	glTranslatef(0, 0, 0.1);
	glGetFloatv(GL_MODELVIEW_MATRIX, upperarmnode->matrix);

	base->child = lowerarmjoint;
	lowerarmjoint->child = lowerarmnode;
	lowerarmnode->child = upperarmjoint;
	upperarmjoint->child = upperarmnode;
}


