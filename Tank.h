#pragma once

#include "resource.h"
#include <windows.h>	
// for IGRA - OPENGL
#include <gl\gl.h> // Header File For The OpenGL32 Library
#include <gl\glu.h> // Header File For The GLu32 Library
#include "Clock.h"
#include "Projectile.h"
#include <cmath>
#include <vector>
#include <map>
#include "Material.h"
#include "RadDeg.h"

struct TreeNode {
	float matrix[16] = {};
	int drawFunctionID;
	struct TreeNode* child;
};

#define DRAW_BASE_FUNCTION_ID  0 
#define DRAW_UPPERBASE_FUNCTION_ID  1
#define DRAW_TURRENT_FUNCTION_ID 2 

#define BULLETSPEED_SLOW 0
#define BULLETSPEED_MEDIUM 1
#define BULLETSPEED_FAST 2

class Tank {
public:
	//Drawing the tank
	TreeNode* base;
	TreeNode* upperbase;
	TreeNode* turrent;

	//Materials
	Material redPlasticMaterial;
	Material yellowPlasticMaterial;
	Material greenPlasticMaterial;

	//Movement
	double xPos = 0;
	double yPos = 0; 

	double xVel = 0;
	double yVel = 0;

	double yRotation = 0;

	double turnSpeed = 90;
	double maxSpeed = 3;
	double acceleration = 6;
	double friction = 3;

	int bulletSpeed = BULLETSPEED_SLOW;

	//Turret
	float TurrentHeight;

	double turretRotHorizontal = 0;
	double turretRotVertical = 0;

	double turretTurnSpeed = 180;
	double turretAltSpeed = 45;

	std::vector<Projectile> projectiles;

	//for timer/clock
	__int64 startTimeInCounts = 0;
	__int64 lastTimeInCounts = 0;
	__int64 countsPerSecond;
	Clock* clock;

	// I/O
	std::map<WPARAM, bool> keysDown;

	//functions and methods
	Tank();
	void BuildTree();
	void DrawTree(TreeNode* root);
	void DrawFunction(int id);
	void Draw();
	void DrawBase();
	void DrawUpperBase();
	void DrawTurrent();
	void TurrentRotate(float angle);
	void Update();
	void HandleKeyDown(WPARAM wParam);
	void HandleKeyUp(WPARAM wParam);
	void OrbitTank(float x, float y);
	double GetRealBulletSpeed(int bulletSpeed);
	void ClearProjectiles();
	~Tank();
};

