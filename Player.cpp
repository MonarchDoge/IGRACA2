#include "stdafx.h"
#include "Player.h"
#include <math.h>;

Player::Player()
{
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
}


Player::~Player()
{
}

void Player::Draw() {
	SetMaterial(&material);
	glPushMatrix();
	// At startup time we make player face the negative z-axis
	glTranslatef(xPos, yPos + 0.075, zPos);
	// Local rotation
	glRotatef(yRotation + 180, 0, 1, 0);
	// Draw the player as a cone
	gluCylinder(cone, 0.15, 0.0, 0.5, 64, 64);
	glPopMatrix();
}

void Player::HandleKeyDown(WPARAM wParam) {
	double dist = 0.1;
	if (wParam == VK_LEFT)
		Rotate(5); // Counter Clockwise
	if (wParam == VK_RIGHT)
		Rotate(-5); // Clockwise
	if (wParam == VK_UP)
		MoveForward(0.1);
	if (wParam == VK_DOWN)
		MoveForward(-0.1);
}

void Player::MoveForward(double dist) {
	// Movement must be based on orientation of player
	double deltaX = 0;
	double deltaZ = 0;
	// Calculate translation as based on current yRotation angle
	deltaX = -dist * sin(degToRad(yRotation));
	deltaZ = -dist * cos(degToRad(yRotation));
	// Update the position
	xPos = xPos + deltaX;
	zPos = zPos + deltaZ;
}
void Player::Rotate(double angle) {
	// Update the rotation (yaw)
	yRotation = yRotation + angle;
}
