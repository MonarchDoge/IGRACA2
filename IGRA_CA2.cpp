// IGRA_LAB.cpp : Defines the entry point for the application.

#include "stdafx.h"
#include "IGRA_CA2.h"
#include "resource.h"
#include <math.h>
#include <iostream>
#include <stdio.h>
#include <windows.h>	

//for igra - openGL
#include <gl\GL.h>
#include<gl\GLU.h>

#include "Square.h"
#include "Shapes.h"
#include "Tank.h"
#include <vector>
#include "Vector3f.h"
#include "Clock.h"
#include <sstream> // stringstream
using namespace std;

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst; // current instance
WCHAR szTitle[MAX_LOADSTRING];    // The title bar S
WCHAR szWindowClass[MAX_LOADSTRING]; // the main window class name
float xPosCircle = 0.0;	 // The position of the jumping circle
float yPosCircle = 0.0;

//added for IGRA
int width = 400;
int height = 400;
HWND hWnd = NULL;
//Added for IGRA CA2
Tank * newTank;
float yrot = 0.0;
GLUquadric *mySphere;
__int64 startTimeInCounts = 0;
__int64 lastTimeInCounts = 0;
__int64 countsPerSecond;
// The current rotation angle of the cube
float yRot = 0;
// We assume cube takes 10 seconds to make a full rotation
double rotationalVelocityInDegreesPerSeconds = 36.0;
//for cube speeds
double currentvelocity;
double accleration;
double deltatime;
enum Cubestate {
	ACCELERATING, MAX_VELOCITY, DECELERATION, STANDSTIL
};
Cubestate cubestate = ACCELERATING;
double timestamp;
//clock
Clock* clock;
double frameratesegment;
double frameratetimer;

//added for assignment
//bool Square, Circle, Triangle, Red, Green, Blue, Wireframe, Solid;
Shape::color Colorshape;
Shape::mode Modeshape;
Shape::shape Shapeshape;
std::vector<Shape> drawShape;

GLuint PixelFormat;
GLubyte image[64][64][3]; //for image texturing
bool keys[256];			// Array Used For The Keyboard Routine
HDC hDC = NULL;
HGLRC hRC = NULL;

static PIXELFORMATDESCRIPTOR pfd{
	sizeof(PIXELFORMATDESCRIPTOR),
	1,// Version Number
	PFD_DRAW_TO_WINDOW | // Format Must Support Window
	PFD_SUPPORT_OPENGL | // Format Must Support OpenGL
	PFD_DOUBLEBUFFER,// Must Support Double Buffering
	PFD_TYPE_RGBA, // Request An RGBA Format
	16, // Select Our Color Depth = 16
	0, 0, 0, 0, 0, 0, // Color Bits Ignored
	0,// No Alpha Buffer
	0,// Shift Bit Ignored
	0,// No Accumulation Buffer
	0, 0, 0, 0, // Accumulation Bits Ignored
	32, // 32 Bit Z-Buffer (Depth Buffer)
	0,// No Stencil Buffer
	0,// No Auxiliary Buffer
	PFD_MAIN_PLANE,// Main Drawing Layer
	0,// Reserved
	0, 0, 0// Layer Masks Ignored
};

int InitOpenGL();
void DrawGLScene();
void GLScene(GLsizei width, GLsizei height);

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void ReSizeGLScene(GLsizei width, GLsizei height);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_IGRA_LAB, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_IGRA_LAB));

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			//added for igra
			DrawGLScene();
			SwapBuffers(hDC);
		}
	}

	return (int)msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_IGRA_LAB));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDR_MENU1);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable
	int maximisable = WS_OVERLAPPEDWINDOW;
	int fixedwindow = (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX);

	//to disable resize and maximise
	//changed for igra
	hWnd = CreateWindowW(szWindowClass, szTitle, fixedwindow, 0, 0, width, height,
		nullptr, nullptr, hInstance, nullptr);

	InitOpenGL();

	if (!hWnd)
	{
		return FALSE;
	}
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	//added for igra
	ReSizeGLScene(width, height);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
void convertWindowToOpenGLCoordinates(int xWinPos, int yWinPos, float &xOpenGLPos, float &yOpenGLPos) {
	//Normalise pixels and fix offsets
		//stop user from fucking around
	float normaliseXWinPos = xWinPos * 2.0f / width;
	float normaliseYWinPos = (yWinPos + 60) * 2.0 / height;
	xOpenGLPos = normaliseXWinPos > 1 ? normaliseXWinPos - 1 : 0 - (1 - normaliseXWinPos);
	yOpenGLPos = normaliseYWinPos > 1 ? ((0 - (1 - normaliseYWinPos)) * -1) : abs(normaliseYWinPos - 1);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case ID_DANKSHAPES_DANKSQUARE:
			CheckMenuItem(GetMenu(hWnd), ID_DANKSHAPES_DANKSQUARE, MF_CHECKED);
			CheckMenuItem(GetMenu(hWnd), ID_DANKSHAPES_DANKCIRCLE, MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd), ID_DANKSHAPES_DANKTRIANGLE, MF_UNCHECKED);
			Shapeshape = Shape::shape::SQUARE_SHAPE;
			break;
		case ID_DANKSHAPES_DANKCIRCLE:
			CheckMenuItem(GetMenu(hWnd), ID_DANKSHAPES_DANKSQUARE, MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd), ID_DANKSHAPES_DANKCIRCLE, MF_CHECKED);
			CheckMenuItem(GetMenu(hWnd), ID_DANKSHAPES_DANKTRIANGLE, MF_UNCHECKED);
			Shapeshape = Shape::shape::CIRCLE_SHAPE;
			break;
		case ID_DANKSHAPES_DANKTRIANGLE:
			CheckMenuItem(GetMenu(hWnd), ID_DANKSHAPES_DANKSQUARE, MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd), ID_DANKSHAPES_DANKCIRCLE, MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd), ID_DANKSHAPES_DANKTRIANGLE, MF_CHECKED);
			Shapeshape = Shape::shape::TRIANGLE_SHAPE;
			break;

		case ID_DANKERCOLOUR_DANKERRED:
			CheckMenuItem(GetMenu(hWnd), ID_DANKERCOLOUR_DANKERRED, MF_CHECKED);
			CheckMenuItem(GetMenu(hWnd), ID_DANKERCOLOUR_DANKERGREEN, MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd), ID_DANKERCOLOUR_DANKERBLUE, MF_UNCHECKED);
			Colorshape = Shape::color::RED_COLOR;
			break;
		case ID_DANKERCOLOUR_DANKERGREEN:
			CheckMenuItem(GetMenu(hWnd), ID_DANKERCOLOUR_DANKERRED, MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd), ID_DANKERCOLOUR_DANKERGREEN, MF_CHECKED);
			CheckMenuItem(GetMenu(hWnd), ID_DANKERCOLOUR_DANKERBLUE, MF_UNCHECKED);
			Colorshape = Shape::color::GREEN_COLOR;
			break;
		case ID_DANKERCOLOUR_DANKERBLUE:
			CheckMenuItem(GetMenu(hWnd), ID_DANKERCOLOUR_DANKERRED, MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd), ID_DANKERCOLOUR_DANKERGREEN, MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd), ID_DANKERCOLOUR_DANKERBLUE, MF_CHECKED);
			Colorshape = Shape::color::BLUE_COLOR;
			break;

		case ID_DANKESTMODE_DANKESTWIREFRAME:
			CheckMenuItem(GetMenu(hWnd), ID_DANKESTMODE_DANKESTWIREFRAME, MF_CHECKED);
			CheckMenuItem(GetMenu(hWnd), ID_DANKESTMODE_DANKESTFILLED, MF_UNCHECKED);
			Modeshape = Shape::mode::DRAW_WIREFRAME_MODE;
			break;
		case ID_DANKESTMODE_DANKESTFILLED:
			CheckMenuItem(GetMenu(hWnd), ID_DANKESTMODE_DANKESTWIREFRAME, MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd), ID_DANKESTMODE_DANKESTFILLED, MF_CHECKED);
			Modeshape = Shape::mode::DRAW_SOLID_MODE;
			break;

		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code that uses hdc here...
		EndPaint(hWnd, &ps);
	}
	break;
	//added for igra
	case WM_SIZE:
	{
		ReSizeGLScene(LOWORD(lParam), HIWORD(lParam));
		break;
	}

	case WM_LBUTTONDOWN:
	{
		// Get the left mouse press coordinates
		int mouseX = LOWORD(lParam);
		int mouseY = HIWORD(lParam);
		//display mouse coordinate in a dialog box
		/*
		//display mouse coordinate in a MessageBox
		char message[255];
		sprintf_s(message, "x= %d, y=%d", mouseX, mouseY);
		MessageBoxA(NULL, message, "Mouse Press Event", MB_OK | MB_ICONINFORMATION);
		*/
		//convert mouse click coordinate to opengl coordinate
		convertWindowToOpenGLCoordinates(mouseX, mouseY, xPosCircle, yPosCircle);

		//sq->HandleLButtonDown(xPosCircle, yPosCircle);
		//Shapes sq(xPosCircle, yPosCircle, Square, Circle, Triangle, Red, Green, Blue, Wireframe, Solid);
		Shape sq(xPosCircle, yPosCircle, Colorshape, Modeshape, Shapeshape);
		drawShape.push_back(sq);

		break;
	}

	case WM_TIMER: {
		break;
	}

				   //dont use
	case WM_MOUSEMOVE:
	{
		// Get the left mouse press coordinates
		int mouseX = LOWORD(lParam);
		int mouseY = HIWORD(lParam);

		convertWindowToOpenGLCoordinates(mouseX, mouseY, xPosCircle, yPosCircle);
		//if (wParam & MK_LBUTTON)
			//sq->HandleButtonMove(xPosCircle, yPosCircle);
		break;
	}

	case WM_KEYDOWN:							// Is A Key Being Held Down?
	{
		newTank->HandleKeyDown(wParam);
		keys[wParam] = TRUE;					// If So, Mark It As TRUE
		return 0;								// Jump Back
	}

	case WM_KEYUP:								// Has A Key Been Released?
	{
		keys[wParam] = FALSE;					// If So, Mark It As FALSE
		return 0;								// Jump Back
	}

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

void ReSizeGLScene(GLsizei width, GLsizei height) {
	if (height == 0)// Prevent A Divide By Zero By
		height = 1; // Making Height Equal One
					// Reset The Current Viewport
	glViewport(0, 0, width, height);
	// Select The Projection Matrix
	glMatrixMode(GL_PROJECTION);
	// Reset The Projection Matrix
	glLoadIdentity();
	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f); //for 3d
	//gluOrtho2D(-1.0, 1.0, -1.0, 1.0); //for 2D
}

void SetupLight() {
	glShadeModel(GL_SMOOTH);
	GLfloat LightAmbient[] = { 0.5, 0.5, 0.5, 1 };
	GLfloat LightDiffuse[] = { 0.5, 0.5, 0.5, 1 };
	GLfloat LightSpecular[] = { 0.5, 0.5, 0.5, 1 };
	GLfloat LightPosition[] = { 10, 10, 10, 0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpecular);
	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);
	glEnable(GL_LIGHT0);
}

void CreateCheckerBoardTexture() {
	// Visit all the pixels in the image 
	// Calculate in which checkerboard
	// rectangle the pixel falls 
	// this will determine the pixel colour  


	// Check if pixel in even or odd checkerboard row  

	// Even rows start with black checkerboard rectangle 
	// In even row all even columns checkerboard rectangles
	// will be black       

	// Odd rows start with red checkerboard rectangle
	// In odd rows all even column checkerboard rectangles
	// will be red       

	// Those pixels at border will be assigned green
	// Assign the final colour to pixel
	int nrOfCheckersOnRow = 8;
	float dim = 64.0 / nrOfCheckersOnRow;

	int red = 0;
	int green = 0;
	int blue = 0;

	for (int i = 0; i < 64; i++) {
		for (int j = 0; j < 64; j++)
		{
			// Calculate in which checkerboard 
			//rectangle the pixel falls
			int row = (int)(i / dim);
			int col = (int)(j / dim);

			int c = 0;
			if (row % 2 == 0) { // Even rows start with black 
				if (col % 2 == 0)
				{ // All even column will be black
					red = green = blue = 0;
				}
				else
				{
					green = blue = 0;
					red = 255;
				}
			}
			else { // Odd rows start with red
				if (col % 2 == 0)
				{
					// All even column will be red
					green = blue = 0;
					red = 255;
				}
				else {
					red = green = blue = 0;
				}
			}

			// Drawing a green border around the image
			if (i == 0 || i == 63 || j == 0 || j == 63)
			{
				red = 0;
				green = 255;
				blue = 0;
			}
			image[i][j][0] = (GLubyte)red;
			image[i][j][1] = (GLubyte)green;
			image[i][j][2] = (GLubyte)blue;
		}
	}
}

void CreateColuredRectTexture() {
	int nrOfRows = 8;
	float dim = 64.0 / nrOfRows;

	int red = 0;
	int green = 0;
	int blue = 0;

	//row
	for (int i = 0; i < 64; i++) {
		//column (thickness)
		for (int j = 0; j < 64; j++)
		{
			// Calculate in which checkerboard 
			//rectangle the pixel falls
			int row = (int)(i / dim);
			int col = (int)(j / dim);

			int red = 255;
			int green = 255;
			int blue = 0; //draw the yellow first

			if ((i >= 16 && i <= 23) || (i >= 37 && i <= 45) || (j >= 16 && j <= 23) || (j >= 37 && j <= 45))
			{
				red = 0;
				green = 0;
				blue = 255;
			}

			if ((i >= 8 && i <= 15) || (i >= 46 && i <= 54) || (j >= 8 && j <= 15) || (j >= 46 && j <= 54))
			{
				red = 0;
				green = 255;
				blue = 0;
			}
			if ((i >= 1 && i <= 7) || (i >= 55 && i <= 63) || (j >= 1 && j <= 7) || (j >= 55 && j <= 63))
			{
				red = 255;
				green = 0;
				blue = 0;
			}
			// Drawing a black border around the image
			//0 is the black column so start from black, the colour wil override
			if (i == 0 || i == 63 || j == 0 || j == 63)
			{
				red = 0;
				green = 0;
				blue = 0;
			}
			image[i][j][0] = (GLubyte)red;
			image[i][j][1] = (GLubyte)green;
			image[i][j][2] = (GLubyte)blue;
		}
	}
}

//use for image texturing
int InitOpenGL() {
	// Get Device Dontext
	if (!(hDC = GetDC(hWnd))) {
		MessageBox(NULL, L"Can't Create A GL Device Context.",
			L"ERROR", MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}
	// Check if Windows can find a matching Pixel Format
	if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd))) {
		MessageBox(NULL, L"Can't Find A Suitable PixelFormat.",
			L"ERROR", MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}
	// Try to set pixel format
	if (!SetPixelFormat(hDC, PixelFormat, &pfd)) {
		MessageBox(NULL, L"Can't Set The PixelFormat.", L"ERROR",
			MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}
	// Get a Rendering context
	if (!(hRC = wglCreateContext(hDC))) {
		MessageBox(NULL, L"Can't Create A GL Rendering Context.",
			L"ERROR", MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}
	// Activate Rendering Context
	if (!wglMakeCurrent(hDC, hRC)) {
		MessageBox(NULL, L"Can't Activate The GL Rendering Context.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}

	SetupLight();

	mySphere = gluNewQuadric();
	gluQuadricDrawStyle(mySphere, GLU_FILL);

	SetTimer(hWnd, NULL, 1, NULL);
	//fix depth buffer issue
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	CreateColuredRectTexture();

	clock = new Clock;
	clock->Start();

	newTank = new Tank();
	return 1;
}

float degToRad(float degAngle) {
	double pi = 3.1415926535; // You could be more precise!
	return degAngle / 180.0 * pi;
}

//IGRA 1 + 2
void DrawFilledTiangle() {
	glBegin(GL_POLYGON);

	//red triangle
	glColor3f(1, 0, 0); // Red
	glVertex2f(0, 0.5); // Top
	glVertex2f(-0.5, -0.5); // Left
	glVertex2f(0.5, -0.5); // Right

	//rainbow polygon
	glColor3f(1, 0, 0); // Red
	glVertex2f(0, 0.5); // Top
	glColor3f(0, 1, 0); // Green
	glVertex2f(-0.5, -0.5); // Left
	glColor3f(0, 0, 1); // Blue
	glVertex2f(0.5, -0.5); // Right 
	glColor3f(1, 1, 0); // Blue
	glVertex2f(0.5, 0.5); // Right 

	glEnd();
}

void DrawWireframeTiangle() {
	glBegin(GL_LINE_STRIP);
	glColor3f(0, 0, 0);
	glVertex2f(0, 0.5);
	glVertex2f(-0.5, -0.5);
	glVertex2f(0.5, -0.5);
	glVertex2f(0, 0.5);
	glEnd();

	glBegin(GL_LINE_STRIP);
	glColor3f(0, 1, 0);
	glVertex2f(0, 0);
	glVertex2f(height / 2, 0.5);
	glEnd();

	glBegin(GL_LINE_STRIP);
	glColor3f(1, 0, 0);
	glVertex2f(0, 0);
	glVertex2f(0.5, width / 2);
	glEnd();
}

void DrawBlockySpiral(int nrOfLines)
{
	glBegin(GL_LINE_STRIP);
	glColor3f(0, 0, 0);

	int currentline = 0; //start from centre
	float increment = 0.05f;
	float x = 0;
	float y = 0;
	int direction = -1;
	glVertex2f(x, y);
	x = increment;
	glVertex2f(x, y);
	y = increment;
	glVertex2f(x, y);
	do
	{
		glColor3f(direction * x, x, y); // some color shit
		for (size_t i = 0; i < currentline + 2; i++)
		{
			x = x + increment * direction;
			glVertex2f(x, y);
		}
		glColor3f(y, x, direction * y);
		for (size_t i = 0; i < currentline + 2; i++)
		{
			y = y + increment * direction;
			glVertex2f(x, y);
		}
		direction = direction * -1;
		currentline++;
	} while (currentline <= nrOfLines && (x <= 1.0f && y <= 1.0f));

	glEnd();
}

void DrawCircle(double r)
{
	glBegin(GL_LINE_STRIP);
	glColor3f(0, 0, 0);
	for (int i = 0; i < 1000; i++)
	{
		glVertex2f(cos(2 * 3.14159*i / 1000) *r, sin(2 * 3.14159*i / 1000) * r);
	}
	glVertex2f(cos(2 * 3.14159 * 0 / 1000) *r, sin(2 * 3.14159 * 0 / 1000)*r);
	glEnd();
}

void DrawSineCurves(float startX, float startY, int nrOfSineCurves, float scale, int definition) {
	//defination = how smooth

	glBegin(GL_LINE_STRIP);
	glColor3f(0, 0, 0);
	for (int i = 0; i < 5 * definition * nrOfSineCurves; i++)
	{
		//scale = scale <= 0 ? 0 : scale - 0.001f; // reduce sin curve at every length
		glVertex2f(startX, abs(sin(degToRad((startX + 1) * (180 * nrOfSineCurves))) * scale + startY)); //must be absolute to remain above Xasix (invert)
		startX += (0.5f / definition);
		//scale -= 0.0023; //simpler method
	}

}

void Draw3D_AxisSystem() {
	glBegin(GL_LINES);
	// Draw positive x-axis as red
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(0, 0, 0);
	glVertex3f(1000, 0, 0);
	// Draw positive x-axis as green
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 1000, 0);
	// Draw positive z-axis as blue
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 1000);
	glEnd();
}

//IGRA 3 + 4
void DrawCube() {
	glEnable(GL_LIGHTING);

	GLfloat blue[] = { 0, 0, 1, 0 };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW); // Vertices in clockwise order
	glPolygonMode(GL_FRONT, GL_FILL); // Solid cube
									  // Front Face in green
									  // Vertices in clock wise order v0, v1, v2, v3

	//Back
	glBegin(GL_POLYGON);
	//glColor3f(1.0, 0.0, 0);//Red
	glNormal3f(0, 0, -1); // The surface normal
	glVertex3f(1.0, -1.0, -1); // v7
	glVertex3f(1.0, 1.0, -1); // v6
	glVertex3f(-1.0, 1.0, -1.0); // v5
	glVertex3f(-1.0, -1.0, -1.0); // v4
	glEnd();

	//Bottom
	glBegin(GL_POLYGON);
	//glColor3f(0.5, 0.5, 0.5);//Grey
	glNormal3f(0, -1, 0); // The surface normal
	glVertex3f(1.0, -1.0, 1.0); // v3
	glVertex3f(1.0, -1.0, -1.0); // v7
	glVertex3f(-1.0, -1.0, -1); // v4
	glVertex3f(-1.0, -1.0, 1); // v0
	glEnd();

	//Front
	glBegin(GL_POLYGON); // 0, 1, 2, 3
	//glColor3f(0.0, 1.0, 0);//green
	glNormal3f(0, 0, 1); // The surface normal
	glVertex3f(-1.0, -1.0, 1); // v0 Left Bottom
	glVertex3f(-1.0, 1.0, 1); // v1 Left Top
	glVertex3f(1.0, 1.0, 1); // v2 Right Top
	glVertex3f(1.0, -1.0, 1); // v3 Right Bottom
	glEnd();

	//Top
	glBegin(GL_POLYGON);
	//glColor3f(0.0, 0.0, 0);//black
	glNormal3f(0, 1, 0); // The surface normal
	glVertex3f(-1.0, 1.0, 1); // v1
	glVertex3f(-1.0, 1.0, -1.0); // v5
	glVertex3f(1.0, 1.0, -1.0); // v6
	glVertex3f(1.0, 1.0, 1); // v2
	glEnd();

	//Right
	glBegin(GL_POLYGON);
	//glColor3f(0.0, 0.0, 1);//Blue
	glNormal3f(1, 0, 0); // The surface normal
	glVertex3f(1.0, -1.0, 1); // v3
	glVertex3f(1.0, 1.0, 1); // v2
	glVertex3f(1.0, 1.0, -1.0); // v6
	glVertex3f(1.0, -1.0, -1.0); // v7
	glEnd();

	//Left
	glBegin(GL_POLYGON);
	//glColor3f(1.0, 1.0, 0);//Yellow
	glNormal3f(-1, 0, 0); // The surface normal
	glVertex3f(-1.0, -1.0, 1); // v0
	glVertex3f(-1.0, -1.0, -1); // v4
	glVertex3f(-1.0, 1.0, -1.0); // v5
	glVertex3f(-1.0, 1.0, 1.0); // v1
	glEnd();

	glDisable(GL_LIGHTING);
}

//uses multidimensiona arrays; storing arrays inside array
void DrawBetterCube() {
	float cubeVertices[][3] = {
		{ -1.0f, -1.0f, 1.0f }, // v0
		{ -1.0f, 1.0f, 1.0f }, // v1
		{ 1.0f, 1.0f, 1.0f }, // v2
		{ 1.0f, -1.0f, 1.0f }, // v3
		{ -1.0f, -1.0f, -1.0f }, // v4
		{ -1.0f, 1.0f, -1.0f }, // v5
		{ 1.0f, 1.0f, -1.0f }, // v6
		{ 1.0f, -1.0f, -1.0f }, // v7
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
		0, 1, 0, // Front is Green
		1, 0, 0, // Back is Red
		0, 0, 1, // Right is Blue
		1, 1, 0, // Left is Yellow
		0, 0, 0, // Top is Black
		0.5, 0.5, 0.5 // Bottom is Grey
	};

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW); // Front face is clockwise
	glPolygonMode(GL_FRONT, GL_FILL);
	glColor3f(0.0, 0.0, 0.0);
	// The index in cubeIndices array
	// points to next vertex to draw.
	int index = 0;
	// Draw the cube quad by quad
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

void DrawHourglass() {

	float HourglassVertices[][3] = {
		{ 0.0f, 2.0f, 0.0f }, // v0

		{ -1.0f, 0.0f, -1.0f }, // v1
		{ -1.0f, 0.0f, 1.0f }, // v2
		{ 1.0f, 0.0f, 1.0f }, // v3
		{ 1.0f, 0.0f, -1.0f }, // v4

		{ 1.0f, 4.0f, 1.0f }, // v5
		{ 1.0f, 4.0f, -1.0f }, // v6
		{ -1.0f, 4.0f, -1.0f }, // v7
		{ -1.0f, 4.0f, 1.0f }, // v8
	};

	int HourglassIndices[] = {
		1, 2, 3, // Bottom Face
		1, 3, 4, // Bottom Face

		4, 0, 1, // back Face
		1, 0, 2, // left Face
		3, 0, 4, // right Face
		2, 0, 3, // front Face

		7, 0, 6, // back Face
		8, 0, 7, // left Face
		6, 0, 5, // right Face
		5, 0, 8, // front Face

		7, 6, 5, // Bottom Face
		8, 7, 5, // Bottom Face
	};

	float HourglassColors[][3] = {
		0, 0, 0, // bottom1 is Black
		0.5, 0.5, 0.5, // Bottom2 is Grey

		1, 0, 0, // Back is Red
		1, 1, 0, // Left is Yellow
		0, 0, 1, // Right is Blue
		0, 1, 0, // Front is Green

		1, 0, 0, // Back is Red
		1, 1, 0, // Left is Yellow
		0, 0, 1, // Right is Blue
		0, 1, 0, // Front is Green

		0.5, 0.5, 0.5, // Bottom2 is Grey
		0, 0, 0, // bottom1 is Black
	};

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW); // Front face is clockwise
	glPolygonMode(GL_FRONT, GL_FILL);
	// The index in cubeIndices array
	// points to next vertex to draw.
	int index = 0;
	for (int qd = 0; qd < 12; qd++) {
		glBegin(GL_TRIANGLES);
		glColor3f(HourglassColors[qd][0], HourglassColors[qd][1], HourglassColors[qd][2]);
		for (int v = 0; v < 3; v++) {// three vertices for one triangle
			glVertex3f(
				HourglassVertices[HourglassIndices[index]][0],
				HourglassVertices[HourglassIndices[index]][1],
				HourglassVertices[HourglassIndices[index]][2]);
			index++; // Move to next vertex in quad
		}
		glEnd();
	}
}

/*
void NormalizeVector(float &Px, float &Py, float &Pz) {
float length = (Px * Px + Py * Py + Pz * Pz);

length /= 5;

Px /= length;
Py /= length;
Pz /= length;
}

void DrawNormal(float nx, float ny, float nz, float cx, float cy, float cz) {
glLineWidth(2.5);
glColor3f(1.0, 0.0, 0.0);
glBegin(GL_LINES);
glVertex3f(nx, ny, nz);
glVertex3f(cx, cy, cz);
glEnd();
}


void CalculateNormal(float firstX, float firstY, float firstZ,
float secondX, float secondY, float secondZ,
float thirdX, float thirdY, float thirdZ) {

float Nx, Ny, Nz;

//formula for normal, IMPORTANT
Nx = firstY * (secondZ - thirdZ) + secondY * (thirdZ - firstZ) + thirdY * (firstZ - secondZ);
Ny = firstZ * (secondX - thirdX) + secondZ * (thirdX - firstX) + thirdZ * (firstX - secondX);
Nz = firstX * (secondY - thirdY) + secondX * (thirdY - firstY) + thirdX * (firstY - secondY);

NormalizeVector(Nx, Ny, Nz);

glNormal3f(Nx, Ny, Nz);

float Cx, Cy, Cz;

//calcutate weird line thing coming out of the middle of the normal
Cx = (firstX + secondX + thirdX) / 3.0;
Cy = (firstY + secondY + thirdY) / 3.0;
Cz = (firstZ + secondZ + thirdZ) / 3.0;

DrawNormal(Nx, Ny, Nz, Cx, Cy, Cz);
}

void DrawPyramid() {
glEnable(GL_LIGHTING);

float pyramidVertices[][3] = {
{ 0.0f, 2.0f, 0.0f }, // v0
{ -1.0f, 0.0f, -1.0f }, // v1
{ -1.0f, 0.0f, 1.0f }, // v2
{ 1.0f, 0.0f, 1.0f }, // v3
{ 1.0f, 0.0f, -1.0f }, // v4
};

int pyramidIndices[] = {
1, 2, 3, // Bottom Face
1, 3, 4, // Bottom Face
4, 0, 1, // back Face
1, 0, 2, // left Face
3, 0, 4, // right Face
2, 0, 3, // front Face
};

glDisable(GL_CULL_FACE);
glFrontFace(GL_CCW); // Front face is clockwise
glPolygonMode(GL_FRONT, GL_FILL);
// The index in cubeIndices array
// points to next vertex to draw.
int index = 0;
for (int qd = 0; qd < 6; qd++) {
glBegin(GL_TRIANGLES);
CalculateNormal(
pyramidVertices[pyramidIndices[index]][0], pyramidVertices[pyramidIndices[index]][1], pyramidVertices[pyramidIndices[index]][2],
pyramidVertices[pyramidIndices[index + 1]][0], pyramidVertices[pyramidIndices[index + 1]][1], pyramidVertices[pyramidIndices[index + 1]][2],
pyramidVertices[pyramidIndices[index + 2]][0], pyramidVertices[pyramidIndices[index + 2]][1], pyramidVertices[pyramidIndices[index + 2]][2]);

for (int v = 0; v < 3; v++) {// three vertices for one triangle
glVertex3f(
pyramidVertices[pyramidIndices[index]][0],
pyramidVertices[pyramidIndices[index]][1],
pyramidVertices[pyramidIndices[index]][2]);
index++; // Move to next vertex in quad
}
glEnd();
}
glDisable(GL_LIGHTING);
}
*/

void DrawNormal(float xN, float yN, float zN, float sX, float sY, float sZ)
{
	glBegin(GL_LINES);
	glVertex3f(sX, sY, sZ);
	glVertex3f(xN, yN, zN);
	glEnd();
}

void DrawNormalPyramid(int config)
{
	GLfloat Lightgrey[] = { 0.8,0.8, 0.8, 0 };
	Vector3f normal;
	Vector3f surface;
	Vector3f linenormal;
	vector<Vector3f> vertices;

	//glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW); // Vertices in clockwise order

	if (config == GL_TRIANGLES)
	{
		glEnable(GL_LIGHTING);
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Lightgrey);
	}
	else
	{
		glLineWidth(1.0f);
		glColor3f(0, 0, 0);
	}
	//Back
	vertices = { Vector3f(-1.0, -1.0, -1.0), Vector3f(0, 1.0, 0), Vector3f(1.0, -1.0, -1) };
	normal = Vector3f::CalculateNormal(vertices[0], vertices[1], vertices[2]);
	normal.Normalise();
	surface = Vector3f::CalculateCenter(vertices[0], vertices[1], vertices[2]);
	linenormal = surface + normal;
	DrawNormal(linenormal.X, linenormal.Y, linenormal.Z, surface.X, surface.Y, surface.Z);
	glBegin(config);
	glNormal3f(normal.X, normal.Y, normal.Z);
	for (size_t i = 0; i < 3; i++)
	{
		glVertex3f(vertices[i].X, vertices[i].Y, vertices[i].Z);
	}
	glEnd();

	//Right
	vertices = { Vector3f(1.0, -1.0, -1.0), Vector3f(0, 1.0, 0), Vector3f(1.0, -1.0, 1) };
	normal = Vector3f::CalculateNormal(vertices[0], vertices[1], vertices[2]);
	normal.Normalise();
	surface = Vector3f::CalculateCenter(vertices[0], vertices[1], vertices[2]);
	linenormal = surface + normal;
	DrawNormal(linenormal.X, linenormal.Y, linenormal.Z, surface.X, surface.Y, surface.Z);
	glBegin(config);
	glNormal3f(normal.X, normal.Y, normal.Z);
	for (size_t i = 0; i < 3; i++)
	{
		glVertex3f(vertices[i].X, vertices[i].Y, vertices[i].Z);
	}
	glEnd();
	//Bottom
	vertices = { Vector3f(1.0, -1.0, -1.0),Vector3f(1.0, -1.0, 1.0), Vector3f(-1.0, -1.0, -1) };
	normal = Vector3f::CalculateNormal(vertices[0], vertices[1], vertices[2]);
	normal.Normalise();
	surface = Vector3f::CalculateCenter(vertices[0], vertices[1], vertices[2]);
	linenormal = surface + normal;
	DrawNormal(linenormal.X, linenormal.Y, linenormal.Z, surface.X, surface.Y, surface.Z);
	glBegin(config);
	glNormal3f(normal.X, normal.Y, normal.Z);
	for (size_t i = 0; i < 3; i++)
	{
		glVertex3f(vertices[i].X, vertices[i].Y, vertices[i].Z);
	}
	glEnd();

	vertices = { Vector3f(-1.0, -1.0, 1),Vector3f(-1.0, -1.0, -1.0), Vector3f(1.0, -1.0, 1.0) };
	normal = Vector3f::CalculateNormal(vertices[0], vertices[1], vertices[2]);
	normal.Normalise();
	surface = Vector3f::CalculateCenter(vertices[0], vertices[1], vertices[2]);
	linenormal = surface + normal;
	DrawNormal(linenormal.X, linenormal.Y, linenormal.Z, surface.X, surface.Y, surface.Z);
	glBegin(config);
	glNormal3f(normal.X, normal.Y, normal.Z);
	for (size_t i = 0; i < 3; i++)
	{
		glVertex3f(vertices[i].X, vertices[i].Y, vertices[i].Z);
	}
	glEnd();

	//Left
	vertices = { Vector3f(-1.0, -1.0, -1),Vector3f(-1.0, -1.0, 1), Vector3f(0, 1.0, 0) };
	normal = Vector3f::CalculateNormal(vertices[0], vertices[1], vertices[2]);
	normal.Normalise();
	surface = Vector3f::CalculateCenter(vertices[0], vertices[1], vertices[2]);
	linenormal = surface + normal;
	DrawNormal(linenormal.X, linenormal.Y, linenormal.Z, surface.X, surface.Y, surface.Z);
	glBegin(config);
	glNormal3f(normal.X, normal.Y, normal.Z);
	for (size_t i = 0; i < 3; i++)
	{
		glVertex3f(vertices[i].X, vertices[i].Y, vertices[i].Z);
	}
	glEnd();
	//Front
	vertices = { Vector3f(0, 1.0, 0),Vector3f(-1.0, -1.0, 1), Vector3f(1.0, -1.0, 1) };
	normal = Vector3f::CalculateNormal(vertices[0], vertices[1], vertices[2]);
	normal.Normalise();
	surface = Vector3f::CalculateCenter(vertices[0], vertices[1], vertices[2]);
	linenormal = surface + normal;
	DrawNormal(linenormal.X, linenormal.Y, linenormal.Z, surface.X, surface.Y, surface.Z);
	glBegin(config);
	glNormal3f(normal.X, normal.Y, normal.Z);
	for (size_t i = 0; i < 3; i++)
	{
		glVertex3f(vertices[i].X, vertices[i].Y, vertices[i].Z);
	}
	glEnd();

	if (config == GL_TRIANGLES)
	{
		glDisable(GL_LIGHTING);
	}
}

//typedef struct materialStruct
//{
//	GLfloat ambient[4];
//	GLfloat diffuse[4];
//	GLfloat specular[4];
//	GLfloat shininess[4];
//} Material;
//
//Material redPlasticMaterial = {
//	{ 0.4, 0.0, 0.0, 1.0 }, // Ambient
//	{ 0.9, 0.0, 0.0, 1.0 }, // Diffuse
//	{ 0.8, 0.8, 0.8, 1.0 }, // Specular
//	32// Shininess
//};
//
//void SetMaterial(Material* m)
//{
//	glMaterialfv(GL_FRONT, GL_AMBIENT, m->ambient);
//	glMaterialfv(GL_FRONT, GL_DIFFUSE, m->diffuse);
//	glMaterialfv(GL_FRONT, GL_SPECULAR, m->specular);
//	glMaterialfv(GL_FRONT, GL_SHININESS, m->shininess);
//};
//
//void DrawSphere(float radius)
//{
//	glEnable(GL_LIGHTING);
//	gluQuadricNormals(mySphere, GLU_SMOOTH);
//	SetMaterial(&redPlasticMaterial);
//	gluSphere(mySphere, radius, 64, 64);
//	glDisable(GL_LIGHTING);
//}
//
//void DrawGod() {
//	DrawSphere(1);
//	for (int j = 0; j < 3; j++)
//	{
//		float translate = 1.5;
//		float size = 0.5;
//		for (int i = 0; i < 4; i++)
//		{
//			if (i == 3)
//			{
//				translate = -2.625;
//			}
//			switch (j) {
//			case 0:
//				glTranslatef(0, translate, 0);
//				break;
//			case 1:
//				glTranslatef(translate, 0, 0);
//				break;
//			case 2:
//				glTranslatef(0, 0, translate);
//				break;
//			}
//			DrawSphere(size);
//			translate /= 2;
//			size /= 2;
//		}
//	}
//}

//IGRA 6 + 7
void DrawTexturedShittySquare() {
	glEnable(GL_TEXTURE_2D);
	glColor3f(1, 1, 1);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 64, 64, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// Draw a rectangle with a texture mapped onto it
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-1.0, -1.0, 0); // v0 Left Bottom
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-1.0, 1.0, 0); // v1 Left Top
	glTexCoord2f(1.0, 1.0);
	glVertex3f(1.0, 1.0, 0); // v2 Right Top
	glTexCoord2f(1.0, 0.0);
	glVertex3f(1.0, -1.0, 0); // v3 Right Bottom
	glEnd();
	glDisable(GL_TEXTURE_2D); // stop colours from overriding
}

void DrawTexturedBetterSquareOrTriangle() {
	glEnable(GL_TEXTURE_2D);
	glColor3f(1, 1, 1);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 64, 64, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// Specify an indent between top two vertices 
	// If indent is 1 then we have a square  
	// If indent is 0 then we have a triangle 
	float indent = 0;//Try these values 0.75, 0.5, 0.25, 0  

	glBegin(GL_POLYGON); // 0, 1, 2, 3
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-1.0, -1.0, 0); // v0 Left Bottom 

	glTexCoord2f(0.0, 1.0);
	glVertex3f(-indent, 1.0, 0);// v1 Left-Centre Top

	glTexCoord2f(0.5, 1.0); //Use this instead of (1.0, 1.0) to prevent distortion
	glVertex3f(indent, 1.0, 0);// v2 Right-Centre Top 

	glTexCoord2f(1.0, 0.0);
	glVertex3f(1.0, -1.0, 0); // v3 Right Bottom 

	glEnd();
	glDisable(GL_TEXTURE_2D); // stop colours from overriding
}

void DrawTexturedCube() {

	float cubeVertices[][3] = {
		{ -1.0f, -1.0f, 1.0f }, // v0
		{ -1.0f, 1.0f, 1.0f }, // v1
		{ 1.0f, 1.0f, 1.0f }, // v2
		{ 1.0f, -1.0f, 1.0f }, // v3
		{ -1.0f, -1.0f, -1.0f }, // v4
		{ -1.0f, 1.0f, -1.0f }, // v5
		{ 1.0f, 1.0f, -1.0f }, // v6
		{ 1.0f, -1.0f, -1.0f }, // v7
	};

	int cubeIndices[] = {
		0, 1, 2, 3, // Front Face
		7, 6, 5, 4, // Back Face
		2, 6, 7, 3, // Right Face
		0, 4, 5, 1, // Left Face
		1, 5, 6, 2, // Top Face
		3, 7, 4, 0 // Bottom Face
	};

	float faceTextureCoordinates[4][2] = {
		{0.0, 0.0},
		{0.0, 1.0},
		{1.0, 1.0},
		{1.0, 0.0}
	};

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW); // Front face is clockwise
	glPolygonMode(GL_FRONT, GL_FILL);
	glColor3f(0.0, 0.0, 0.0);
	int index = 0;
	// Draw the cube quad by quad
	for (int qd = 0; qd < 6; qd++) {
		glEnable(GL_TEXTURE_2D);
		glColor3f(1, 1, 1);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 64, 64, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glBegin(GL_QUADS);
		for (int v = 0; v < 4; v++) {
			switch (v) {
			case 0:
				glTexCoord2f(0.0, 0.0);
				break;
			case 1:
				glTexCoord2f(0.0, 1.0);
				break;
			case 2:
				glTexCoord2f(1.0, 1.0);
				break;
			case 3:
				glTexCoord2f(1.0, 0.0);
				break;
			}
			// Four vertices for one quad
			glVertex3f(cubeVertices[cubeIndices[index]][0],
				cubeVertices[cubeIndices[index]][1],
				cubeVertices[cubeIndices[index]][2]);
			index++; // Move to next vertex in quad
		}
		glEnd();
		glDisable(GL_TEXTURE_2D);
	}
}

void StartTimer() {
	__int64 currentTimeInCounts;
	// countsPerSecond depends on your PC
	if (!QueryPerformanceFrequency(
		(LARGE_INTEGER *)&countsPerSecond)) {
		MessageBox(NULL, L"QueryPerformanceFrequency Failed.", L"ERROR", MB_OK | MB_ICONINFORMATION);
		return;
	}
	QueryPerformanceCounter((LARGE_INTEGER *)&currentTimeInCounts);
	startTimeInCounts = currentTimeInCounts;
	lastTimeInCounts = currentTimeInCounts;
}

// Return absolute time in seconds -since start
double GetTimePassedSinceStart() {
	__int64 currentTimeInCounts;
	double timePassedSeconds;

	// Calculate time passed in seconds since timer was started
	QueryPerformanceCounter((LARGE_INTEGER *)&currentTimeInCounts);
	timePassedSeconds = (currentTimeInCounts - startTimeInCounts) / (double)countsPerSecond;
	return timePassedSeconds;
}

// Return relative time in seconds - since last measurement
double GetTimePassedSinceLastTime() {
	__int64 currentTimeInCounts, timePassedSinceLastTimeInCounts;

	// Calculate time passed in seconds since last call to
	// GetTimePassedSinceLastTime
	QueryPerformanceCounter((LARGE_INTEGER *)&currentTimeInCounts);
	timePassedSinceLastTimeInCounts = currentTimeInCounts - lastTimeInCounts;
	double timePassedSinceLastTimeInSeconds = (currentTimeInCounts - lastTimeInCounts) / (double)countsPerSecond;

	lastTimeInCounts = currentTimeInCounts;
	return timePassedSinceLastTimeInSeconds;
}

void GottaGoFast() {
	string statetext;
	deltatime = clock->TimePassedSincePreviousTime();
	glPushMatrix();

	switch (cubestate) {
	case ACCELERATING:
		if (currentvelocity >= 360) {
			cubestate = MAX_VELOCITY;
			timestamp = clock->TimePassedSinceStartTime() + 2;
		}
		accleration = 20;
		statetext = "ACCERLERATING";
		break;
	case MAX_VELOCITY:
		accleration = 0;
		if (clock->TimePassedSinceStartTime() >= timestamp) {
			cubestate = DECELERATION;
		}
		statetext = "MAX_VELOCITY";
		break;
	case DECELERATION:
		accleration = -40;
		if (currentvelocity <= 0) {
			cubestate = STANDSTIL;
		}
		statetext = "DECELERATION";
		break;
	case STANDSTIL:
		accleration = 0;
		statetext = "STAND_STIL";
		break;
	default:
		statetext = "STAND_STIL";
		break;
	}

	currentvelocity += accleration * deltatime;

	yRot += deltatime * currentvelocity;
	glRotatef(yRot, 0, 1, 0);
	DrawTexturedCube();
	glPopMatrix();

	frameratetimer += deltatime;
	frameratesegment++;
	if (frameratetimer >= 1)
	{
		int framerate = (frameratesegment / frameratetimer) * 10000;
		string text = std::to_string(framerate);
		std::string str = text + " Fps, state is " + statetext;
		LPSTR s = const_cast<char *>(str.c_str());
		SetWindowTextA(hWnd, s);
		frameratetimer = 0;
		frameratesegment = 0;
	}
}

//IGRA CA2
void DrawDankPlane() {
	glEnable(GL_TEXTURE_2D);
	glColor3f(1, 1, 1);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 64, 64, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// Draw a rectangle with a texture mapped onto it
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-9.0, -0.55, 10); // v0 Left Bottom
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-9.0, -0.55, -8); // v1 Left Top
	glTexCoord2f(1.0, 1.0);
	glVertex3f(9.0, -0.55, -8); // v2 Right Top
	glTexCoord2f(1.0, 0.0);
	glVertex3f(9.0, -0.55, 10); // v3 Right Bottom
	glEnd();
	glDisable(GL_TEXTURE_2D); // stop colours from overriding
}

//ayy lmao
void DrawGLScene() {
	void StartTimer();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1, 1, 1, 1); // White

	//Locate camera in the (1,1,1) position then look at
	//origin (the intersection of the axis system)
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity(); // IMPORTANT
	gluLookAt(xPosCircle * 10, yPosCircle * 10, 10, // Camera's position
		0, 0, 0, // Camera's target to look at
		0, 1, 0); // Orientation of camera
	// Draw axis system
	Draw3D_AxisSystem();

	glEnable(GL_LIGHTING);
	newTank->Draw();
	DrawDankPlane();
	glEnd();
	glDisable(GL_LIGHTING);
	glPopMatrix();
}

