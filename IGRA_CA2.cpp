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
#include <gl\GLU.h>

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
//__int64 startTimeInCounts = 0;
//__int64 lastTimeInCounts = 0;
//__int64 countsPerSecond;
// The current rotation angle of the cube
float yRot = 0;
// We assume cube takes 10 seconds to make a full rotation
//double rotationalVelocityInDegreesPerSeconds = 36.0;
////for cube speeds
//double currentvelocity;
//double accleration;
//double deltatime;
//enum Cubestate {
//	ACCELERATING, MAX_VELOCITY, DECELERATION, STANDSTIL
//};
//Cubestate cubestate = ACCELERATING;
//double timestamp;
////clock
//Clock* clock;
//double frameratesegment;
//double frameratetimer;

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

void convertWindowToOpenGLCoordinates(int xWinPos, int yWinPos, float &xOpenGLPos, float &yOpenGLPos) {
	//Normalise pixels and fix offsets
		//stop user from fucking around
	float normaliseXWinPos = xWinPos * 2.0f / width;
	float normaliseYWinPos = (yWinPos + 60) * 2.0f / height;
	xOpenGLPos = normaliseXWinPos > 1 ? normaliseXWinPos - 1 : 0 - (1 - normaliseXWinPos);
	yOpenGLPos = normaliseYWinPos > 1 ? ((0 - (1 - normaliseYWinPos)) * -1) : abs(normaliseYWinPos - 1);
}

void SetBulletSpeed(int wmId) {
	CheckMenuRadioItem(GetMenu(hWnd), ID_BULLETSPEED_SLOW, ID_BULLETSPEED_FAST, wmId, MF_BYCOMMAND);
	if (wmId == ID_BULLETSPEED_SLOW) newTank->bulletSpeed = BULLETSPEED_SLOW;
	if (wmId == ID_BULLETSPEED_MEDIUM) newTank->bulletSpeed = BULLETSPEED_MEDIUM;
	if (wmId == ID_BULLETSPEED_FAST) newTank->bulletSpeed = BULLETSPEED_FAST;
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
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case ID_BULLETSPEED_FAST:
		case ID_BULLETSPEED_MEDIUM:
		case ID_BULLETSPEED_SLOW:
			SetBulletSpeed(wmId);
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
		newTank->HandleKeyUp(wParam);
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
	float dim = 64.0f / nrOfCheckersOnRow;

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
	float dim = 64.0f / nrOfRows;

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

	//clock = new Clock;
	//clock->Start();

	newTank = new Tank();
	SetBulletSpeed(ID_BULLETSPEED_MEDIUM);
	return 1;
}

//IGRA CA2
void DrawDankPlane() {
	glFrontFace(GL_CW); // Front face is clockwise
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
	glVertex3f(-9, -0.55f, 10); // v0 Left Bottom
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-9, -0.55f, -8); // v1 Left Top
	glTexCoord2f(1.0, 1.0);
	glVertex3f(9, -0.55f, -8); // v2 Right Top
	glTexCoord2f(1.0, 0.0);
	glVertex3f(9, -0.55f, 10); // v3 Right Bottom
	glEnd();
	glDisable(GL_TEXTURE_2D); // stop colours from overriding
}

//ayy lmao
void DrawGLScene() {
	//void StartTimer();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1, 1, 1, 1); // White

	//Locate camera in the (1,1,1) position then look at
	//origin (the intersection of the axis system)
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity(); // IMPORTANT
	newTank->OrbitTank(xPosCircle, yPosCircle);

	glEnable(GL_LIGHTING);
	newTank->Update();
	newTank->Draw();
	glEnd();
	glDisable(GL_LIGHTING);
	DrawDankPlane();
}
