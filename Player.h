#pragma once
// for IGRA - OPENGL
#include <gl\gl.h> // Header File For The OpenGL32 Library
#include <gl\glu.h> // Header File For The GLu32 Library

typedef struct Material {
	GLfloat ambient[4];
	GLfloat diffuse[4];
	GLfloat specular[4];
	GLfloat shininess[4];
};
Material;

class Player {
public:

	Material redPlasticMaterial;
	Material yellowPlasticMaterial;
	Material greenPlasticMaterial;

	GLUquadricObj * cone;
	Material material;
	double xPos;
	double yPos;
	double zPos;
	double yRotation;
	Player();
	void Draw();
	void HandleKeyDown(WPARAM wParam);
	void Rotate(double angle);
	void MoveForward(double dist);
	~Player();
};
