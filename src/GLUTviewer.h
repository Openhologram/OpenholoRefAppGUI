#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include <string>
using namespace std;

#include <stdio.h>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>

//OpenGL : for Point Cloud Visualization & GUI
#include <GL/freeglut.h>

//OpenHolo
#include "vec.h"

using namespace oph;


namespace glwin {

#define _WIDTH	900
#define _HEIGHT	900

	//Color LUT : Back Ground Color or Model Voxel Color
#define LUT_BLACK	vec3(0.f, 0.f, 0.f)	
#define LUT_WHITE	vec3(1.f, 1.f, 1.f) //default Voxel
#define LUT_GRAY	vec3(0.4f, 0.4f, 0.4f)
#define LUT_RED		vec3(1.f, 0.f, 0.f)
#define LUT_GREEN	vec3(0.f, 1.f, 0.f)
#define LUT_BLUE	vec3(0.f, 0.f, 1.f)
#define LUT_CYAN	vec3(0.f, 1.f, 1.f)
#define LUT_MAGENTA	vec3(1.f, 0.f, 1.f)
#define LUT_YELLOW	vec3(1.f, 1.f, 0.f)
#define LUT_ORANGE	vec3(1.f, 0.5f, 0.f)
#define LUT_NAVY	vec3(0.f, 0.f, 0.4f) //default BG

#ifndef _X 
#define _X 0
#endif
#ifndef _Y 
#define _Y 1
#endif
#ifndef _Z 
#define _Z 2
#endif


	class GLUTviewer
	{
	public:
		GLUTviewer();
		GLUTviewer(const vector<float> &VertexArray);
		GLUTviewer(const vector<vec3> &VertexArray);
		GLUTviewer(const vector<float> &VertexArray, const vector<vec3> &ColorArray);
		GLUTviewer(const vector<vec3> &VertexArray, const vector<vec3> &ColorArray);
		GLUTviewer(const vector<float> &VertexArray, const vector<float> &ColorArray);
		GLUTviewer(const vector<vec3> &VertexArray, const vector<float> &ColorArray);
		~GLUTviewer();

		void play(int *argc, char **argv);
		void initGLUT(int *argc, char **argv);
		void setModelVertexArray(const vector<float> &VertexArray);
		void setModelVertexArray(const vector<vec3> &VertexArray);
		void setModelColorArray(const vector<vec3> &ColorArray);
		void setModelColorArray(const vector<float> &ColorArray);
		void setModelColorArray(const vector<uchar> &ColorArray);

	protected:
		void doDebugTest();
		void doCaption();
		void doLightShading();
		void draw3dAxis(const float unitMM);
		void drawGrid(const float unitMM, const int grid);
		void drawVoxel(const vec3 &vertex, const vec3 &color);

		//GLUT Call-Back Function
		void doKeyboard(GLubyte key, GLint x, GLint y);
		void doSpecialKey(GLint key, GLint x, GLint y);
		void doMouse(GLint button, GLint state, GLint x, GLint y);
		void doMotion(GLint x, GLint y);
		void doReshape(GLsizei width, GLsizei height);
		void doMenu(GLint value);
		void doDisplay();

		//static Function for GLUT Call-Back Function : SingleTon Pattern
		static GLUTviewer *Instance; //Main Static Object for SingleTon Pattern
		static void staticKeyboard(GLubyte key, GLint x, GLint y) { Instance->doKeyboard(key, x, y); }
		static void staticSpecialKey(GLint key, GLint x, GLint y) { Instance->doSpecialKey(key, x, y); }
		static void staticMouse(GLint button, GLint state, GLint x, GLint y) { Instance->doMouse(button, state, x, y); }
		static void staticMotion(GLint x, GLint y) { Instance->doMotion(x, y); }
		static void staticReshape(GLsizei width, GLsizei height) { Instance->doReshape(width, height); }
		static void staticMenu(GLint value) { Instance->doMenu(value); }
		static void staticDisplay() { Instance->doDisplay(); }

		//Menu
		bool bDrawAxis = true;
		bool bDrawGrid = true;
		bool bDrawLight = false;
		bool bDrawCaption = true;
		bool bDrawVxColor = true;
		bool bDebug = false;

		GLsizei width = _WIDTH;
		GLsizei height = _HEIGHT;

		// Model Tranform : Model's R & T
		GLfloat angleX = 0.f;
		GLfloat angleY = 0.f;
		GLfloat angleZ = 0.f;
		GLfloat transX = 0.f;
		GLfloat transY = 0.f;
		GLfloat transZ = -1000.f; //직교투영은 멀어도 똑같이 보임(근거리 절단영역 넓히기 위해 멀리서 찍음)
		GLfloat scale = 100.f;

		//Viewing Transform : Camera's R & T, for Arcball Controll
		//for Arcball(Mouse Event)
		bool bArcball = false;
		bool bParallel = false;

		GLfloat cameraRX = 0.f;
		GLfloat cameraRY = 0.f;
		GLfloat cameraTX = 0.f;
		GLfloat cameraTY = 0.f;

		GLint preCursorX = 0;
		GLint preCursorY = 0;
		GLint nowCursorX = 0;
		GLint nowCursorY = 0;

		//GL Projection Parameters
		GLdouble vLeft = -0.5f * _WIDTH;
		GLdouble vRight = _WIDTH - 1 - 0.5f * _WIDTH;
		GLdouble vBottom = -0.5f * _HEIGHT;
		GLdouble vTop = _HEIGHT - 1 - 0.5f * _HEIGHT;
		GLdouble vNear = 0.0; //Perspective면 1.0, Orthographic이면 0.0
		const GLdouble vFar = 64000000.0;

		//Light Transform
		GLfloat lightX = 0.f;
		GLfloat lightY = 0.f;
		GLfloat lightZ = 0.f;
		GLfloat lightScale = 1.f;

		//Color Info
		GLfloat voxelSize = 0.01f;
		vec3 voxelColor = LUT_WHITE;
		vec3 bgColor = LUT_NAVY;

		//3D Model
		int n_points = 0;
		vector<vec3> ModelVertex;
		vector<vec3> ModelColor;

		//Bitmap String Caption
		std::vector<string> caption;
	};

#define ARCBALL_SENS 0.2f //Arcball Mouse Event Sensing

	//GLUT Menu Switch Flag
#define MENU_EXIT_ESCAPE		0x1000001
#define MENU_DRAW_XYZ_AXIS		0x1000002
#define MENU_DRAW_GRID_PLANE	0x1000004
#define MENU_DRAW_LIGHTING		0x1000008
#define MENU_DRAW_INFO			0x1000010
#define MENU_DEBUG_SWITCH		0x1000020
#define MENU_DRAW_VX_COLOR		0x1000040

#define COLOR_VX_BLACK			0x1100001
#define COLOR_VX_WHITE			0x1100002
#define COLOR_VX_GRAY			0x1100004
#define COLOR_VX_RED			0x1100008
#define COLOR_VX_GREEN			0x1100010
#define COLOR_VX_BLUE			0x1100020
#define COLOR_VX_CYAN			0x1100040
#define COLOR_VX_MAGENTA		0x1100080
#define COLOR_VX_YELLOW			0x1100100
#define COLOR_VX_ORANGE			0x1100200
#define COLOR_VX_NAVY			0x1100400

#define COLOR_BG_BLACK			0x1110001
#define COLOR_BG_WHITE			0x1110002
#define COLOR_BG_GRAY			0x1110004
#define COLOR_BG_RED			0x1110008
#define COLOR_BG_GREEN			0x1110010
#define COLOR_BG_BLUE			0x1110020
#define COLOR_BG_CYAN			0x1110040
#define COLOR_BG_MAGENTA		0x1110080
#define COLOR_BG_YELLOW			0x1110100
#define COLOR_BG_ORANGE			0x1110200
#define COLOR_BG_NAVY			0x1110400

}