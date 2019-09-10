#include "stdafx.h"
#include "GLUTviewer.h"


//for using static Member variable in Class : SingleTon Pattern
glwin::GLUTviewer *glwin::GLUTviewer::Instance;


glwin::GLUTviewer::GLUTviewer() {
	Instance = this;
}


glwin::GLUTviewer::GLUTviewer(const vector<float> &VertexArray) {
	Instance = this;
	setModelVertexArray(VertexArray);
}


glwin::GLUTviewer::GLUTviewer(const vector<vec3> &VertexArray) {
	Instance = this;
	setModelVertexArray(VertexArray);
}


glwin::GLUTviewer::GLUTviewer(const vector<float> &VertexArray, const vector<vec3> &ColorArray) {
	Instance = this;
	setModelVertexArray(VertexArray);
	setModelColorArray(ColorArray);
}


glwin::GLUTviewer::GLUTviewer(const vector<vec3> &VertexArray, const vector<vec3> &ColorArray) {
	Instance = this;
	setModelVertexArray(VertexArray);
	setModelColorArray(ColorArray);
}


glwin::GLUTviewer::GLUTviewer(const vector<float> &VertexArray, const vector<float> &ColorArray) {
	Instance = this;
	setModelVertexArray(VertexArray);
	setModelColorArray(ColorArray);
}


glwin::GLUTviewer::GLUTviewer(const vector<vec3> &VertexArray, const vector<float> &ColorArray) {
	Instance = this;
	setModelVertexArray(VertexArray);
	setModelColorArray(ColorArray);
}


glwin::GLUTviewer::~GLUTviewer() {
	delete Instance;
	ModelVertex.clear();
	ModelColor.clear();
}


void glwin::GLUTviewer::play(int *argc, char **argv) {
	cout << "Initialize GLUT Main Viewer..." << endl;
	this->initGLUT(argc, argv);
}


void glwin::GLUTviewer::initGLUT(int *argc, char **argv) {
	//Projection Parameter for Viewport Transform
	this->vLeft = -0.5f * this->width;
	this->vRight = this->width - 1 - 0.5f * this->width;
	this->vBottom = -0.5f * this->height;
	this->vTop = this->height - 1 - 0.5f * this->height;

	glutInit(argc, argv);
	glutInitWindowSize(width, height);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutCreateWindow("Point Cloud Viewer"); {
		//Create GLUT Pop-Up Menu & Sub Menu
		GLint menuColorBG = glutCreateMenu(GLUTviewer::staticMenu); {
			glutAddMenuEntry("Black", COLOR_BG_BLACK);
			glutAddMenuEntry("White", COLOR_BG_WHITE);
			glutAddMenuEntry("Gray", COLOR_BG_GRAY);
			glutAddMenuEntry("Red", COLOR_BG_RED);
			glutAddMenuEntry("Green", COLOR_BG_GREEN);
			glutAddMenuEntry("Blue", COLOR_BG_BLUE);
			glutAddMenuEntry("Cyan", COLOR_BG_CYAN);
			glutAddMenuEntry("Magenta", COLOR_BG_MAGENTA);
			glutAddMenuEntry("Yellow", COLOR_BG_YELLOW);
			glutAddMenuEntry("Orange", COLOR_BG_ORANGE);
			glutAddMenuEntry("Navy", COLOR_BG_NAVY);
		}
		GLint menuColorVX = glutCreateMenu(GLUTviewer::staticMenu); {
			glutAddMenuEntry("Voxel Color", MENU_DRAW_VX_COLOR);
			glutAddMenuEntry("Black", COLOR_VX_BLACK);
			glutAddMenuEntry("White", COLOR_VX_WHITE);
			glutAddMenuEntry("Gray", COLOR_VX_GRAY);
			glutAddMenuEntry("Red", COLOR_VX_RED);
			glutAddMenuEntry("Green", COLOR_VX_GREEN);
			glutAddMenuEntry("Blue", COLOR_VX_BLUE);
			glutAddMenuEntry("Cyan", COLOR_VX_CYAN);
			glutAddMenuEntry("Magenta", COLOR_VX_MAGENTA);
			glutAddMenuEntry("Yellow", COLOR_VX_YELLOW);
			glutAddMenuEntry("Orange", COLOR_VX_ORANGE);
			glutAddMenuEntry("Navy", COLOR_VX_NAVY);
		}
		glutCreateMenu(GLUTviewer::staticMenu); {
			glutAddMenuEntry("(ESC) Program Exit", MENU_EXIT_ESCAPE);
			glutAddMenuEntry("(F1) 3D Axis On/Off", MENU_DRAW_XYZ_AXIS);
			glutAddMenuEntry("(F2) Grid Plane On/Off", MENU_DRAW_GRID_PLANE);
			glutAddMenuEntry("(F3) Lighting On/Off", MENU_DRAW_LIGHTING);
			glutAddMenuEntry("(F4) Info Caption On/Off", MENU_DRAW_INFO);
			glutAddMenuEntry("(F5) Debug Switch", MENU_DEBUG_SWITCH);
			glutAddSubMenu("Voxel Color", menuColorVX);
			glutAddSubMenu("BackGround Color", menuColorBG);			
		} glutAttachMenu(GLUT_RIGHT_BUTTON);

		glutDisplayFunc(GLUTviewer::staticDisplay);
		glutKeyboardFunc(GLUTviewer::staticKeyboard);
		glutSpecialFunc(GLUTviewer::staticSpecialKey);
		glutMouseFunc(GLUTviewer::staticMouse);
		glutMotionFunc(GLUTviewer::staticMotion);
		glutReshapeFunc(GLUTviewer::staticReshape);
	} glutMainLoop();
}


void glwin::GLUTviewer::setModelVertexArray(const vector<float> &VertexArray) {
	if (ModelVertex.size()) ModelVertex.clear();

	n_points = VertexArray.size() / 3;
	for (int i = 0; i < n_points; ++i) {
		vec3 Vertex(VertexArray[3 * i + 0], VertexArray[3 * i + 1], VertexArray[3 * i + 2]);
		ModelVertex.push_back(Vertex);
	}
}


void glwin::GLUTviewer::setModelVertexArray(const vector<vec3> &VertexArray) {
	if (ModelVertex.size()) ModelVertex.clear();

	n_points = VertexArray.size();
	ModelVertex = VertexArray;
}


void glwin::GLUTviewer::setModelColorArray(const vector<vec3> &ColorArray) {
	if (ModelColor.size()) ModelColor.clear();

	ModelColor = ColorArray;
}


void glwin::GLUTviewer::setModelColorArray(const vector<float> &ColorArray) {
	if (ModelColor.size()) ModelColor.clear();

	for (int i = 0; i < n_points; ++i) {
		vec3 Color(ColorArray[3 * i + 0], ColorArray[3 * i + 1], ColorArray[3 * i + 2]);
		ModelColor.push_back(Color);
	}
}


void glwin::GLUTviewer::setModelColorArray(const vector<uchar> &ColorArray) {
	if (ModelColor.size()) ModelColor.clear();

	for (int i = 0; i < n_points; ++i) {
		vec3 Color(ColorArray[3 * i + 0] / 255., ColorArray[3 * i + 1] / 255., ColorArray[3 * i + 2] / 255.);
		ModelColor.push_back(Color);
	}
}


void glwin::GLUTviewer::doDebugTest() {

}


void glwin::GLUTviewer::doCaption() {
	
}


void glwin::GLUTviewer::doLightShading() {
	////광원 위치 스피어로 표시			
	//glPushMatrix(); {
	//	glTranslatef(lightX, lightY, lightZ);
	//	glColor3f(1.f, 1.f, 1.f);
	//	glutSolidSphere(lightScale / 3., 10, 10);
	//} glPopMatrix();

	//기본 조명 설정 : 상속 시 오버라이딩 안 하면 기본 호출
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0); //Set Local Light
	GLfloat lightPos[4] = { lightX, lightY, lightZ, 1.f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos); //Set Light Position
	GLfloat ambient[4] = { 0.f, 0.f, 0.f, 1.f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient); //Set Ambient Value
	GLfloat diffuse[4] = { lightScale, lightScale, lightScale, 1.f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse); //Set Diffuse Value
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);

	glEnable(GL_COLOR_MATERIAL); //Vertex Color is Material Color
	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
}


void glwin::GLUTviewer::draw3dAxis(const float unitMM) {
	glDisable(GL_LIGHTING);
	GLUquadricObj *Axis = gluNewQuadric();
	gluQuadricDrawStyle(Axis, GLU_FILL);

	GLdouble thickness = 0.05;
	GLdouble sphere = 0.1;

	// X-Axis : Red
	glColor3f(1.f, 0.f, 0.f);
	glPushMatrix(); {
		glRotatef(90.f, 0.f, 1.f, 0.f);
		gluCylinder(Axis, thickness, thickness, unitMM * 0.3, 8, 1);
	} glPopMatrix();

	glPushMatrix(); {
		glTranslatef(unitMM * 0.3f, 0.f, 0.f);
		glRotatef(90.f, 0.f, 1.f, 0.f);
		gluCylinder(Axis, sphere, 0., unitMM * 0.2, 8, 1);
	} glPopMatrix();

	// Y-Axis : Green
	glColor3f(0.f, 1.f, 0.f);
	glPushMatrix(); {
		glRotatef(-90.f, 1.f, 0.f, 0.f);
		gluCylinder(Axis, thickness, thickness, unitMM * 0.3, 8, 1);
	} glPopMatrix();

	glPushMatrix(); {
		glRotatef(-90.f, 1.f, 0.f, 0.f);
		glTranslatef(0.f, 0.f, unitMM * 0.3f);
		gluCylinder(Axis, sphere, 0., unitMM * 0.2, 8, 1);
	} glPopMatrix();

	// Z-Axis : Blue
	glColor3f(0.f, 0.f, 1.f);
	glPushMatrix(); {
		gluCylinder(Axis, thickness, thickness, unitMM * 0.3, 8, 1);
	} glPopMatrix();

	glPushMatrix(); {
		glTranslatef(0.f, 0.f, unitMM * 0.3f);
		gluCylinder(Axis, sphere, 0., unitMM * 0.2, 8, 1);
	} glPopMatrix();
	
	//Zero Point : Yellow
	glColor3f(1.f, 1.f, 0.f);
	gluSphere(Axis, sphere, 20, 20);
	glColor3f(1.f, 1.f, 1.f); //이후에 그려지는 오브젝트를 위해 기본색으로

	gluDeleteQuadric(Axis);
	if (this->bDrawLight) glEnable(GL_LIGHTING);
}


void glwin::GLUTviewer::drawGrid(const float unitMM, const int grid) {
	glDisable(GL_LIGHTING);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glColor3f(0.5f, 0.5f, 0.5f);
	for (int i = -grid; i < grid + 1; i++) {
		glBegin(GL_LINES); {
			glVertex3f(i*unitMM, 0.f, -grid * unitMM);
			glVertex3f(i*unitMM, 0.f, grid * unitMM);

			glVertex3f(-grid * unitMM, 0.f, i * unitMM);
			glVertex3f(grid * unitMM, 0.f, i * unitMM);
		} glEnd();
	}
	glColor3f(1.f, 1.f, 1.f); //이후에 그려지는 오브젝트를 위해 기본색으로
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (this->bDrawLight) glEnable(GL_LIGHTING);
}


void glwin::GLUTviewer::drawVoxel(const vec3 &vertex, const vec3 &color) {
	glColor3f(color[0], color[1], color[2]);
	glBegin(GL_QUADS); {
		glVertex3f(vertex[_X] + voxelSize / 2, vertex[_Y] + voxelSize / 2, vertex[_Z] + voxelSize / 2);
		glVertex3f(vertex[_X] + voxelSize / 2, vertex[_Y] - voxelSize / 2, vertex[_Z] + voxelSize / 2);
		glVertex3f(vertex[_X] + voxelSize / 2, vertex[_Y] - voxelSize / 2, vertex[_Z] - voxelSize / 2);
		glVertex3f(vertex[_X] + voxelSize / 2, vertex[_Y] + voxelSize / 2, vertex[_Z] - voxelSize / 2);

		glVertex3f(vertex[_X] + voxelSize / 2, vertex[_Y] + voxelSize / 2, vertex[_Z] - voxelSize / 2);
		glVertex3f(vertex[_X] + voxelSize / 2, vertex[_Y] - voxelSize / 2, vertex[_Z] - voxelSize / 2);
		glVertex3f(vertex[_X] - voxelSize / 2, vertex[_Y] - voxelSize / 2, vertex[_Z] - voxelSize / 2);
		glVertex3f(vertex[_X] - voxelSize / 2, vertex[_Y] + voxelSize / 2, vertex[_Z] - voxelSize / 2);

		glVertex3f(vertex[_X] - voxelSize / 2, vertex[_Y] + voxelSize / 2, vertex[_Z] - voxelSize / 2);
		glVertex3f(vertex[_X] - voxelSize / 2, vertex[_Y] - voxelSize / 2, vertex[_Z] - voxelSize / 2);
		glVertex3f(vertex[_X] - voxelSize / 2, vertex[_Y] - voxelSize / 2, vertex[_Z] + voxelSize / 2);
		glVertex3f(vertex[_X] - voxelSize / 2, vertex[_Y] + voxelSize / 2, vertex[_Z] + voxelSize / 2);

		glVertex3f(vertex[_X] - voxelSize / 2, vertex[_Y] + voxelSize / 2, vertex[_Z] + voxelSize / 2);
		glVertex3f(vertex[_X] - voxelSize / 2, vertex[_Y] - voxelSize / 2, vertex[_Z] + voxelSize / 2);
		glVertex3f(vertex[_X] + voxelSize / 2, vertex[_Y] - voxelSize / 2, vertex[_Z] + voxelSize / 2);
		glVertex3f(vertex[_X] + voxelSize / 2, vertex[_Y] + voxelSize / 2, vertex[_Z] + voxelSize / 2);

		glVertex3f(vertex[_X] + voxelSize / 2, vertex[_Y] + voxelSize / 2, vertex[_Z] + voxelSize / 2);
		glVertex3f(vertex[_X] + voxelSize / 2, vertex[_Y] + voxelSize / 2, vertex[_Z] - voxelSize / 2);
		glVertex3f(vertex[_X] - voxelSize / 2, vertex[_Y] + voxelSize / 2, vertex[_Z] - voxelSize / 2);
		glVertex3f(vertex[_X] - voxelSize / 2, vertex[_Y] + voxelSize / 2, vertex[_Z] + voxelSize / 2);

		glVertex3f(vertex[_X] + voxelSize / 2, vertex[_Y] - voxelSize / 2, vertex[_Z] + voxelSize / 2);
		glVertex3f(vertex[_X] + voxelSize / 2, vertex[_Y] - voxelSize / 2, vertex[_Z] - voxelSize / 2);
		glVertex3f(vertex[_X] - voxelSize / 2, vertex[_Y] - voxelSize / 2, vertex[_Z] - voxelSize / 2);
		glVertex3f(vertex[_X] - voxelSize / 2, vertex[_Y] - voxelSize / 2, vertex[_Z] + voxelSize / 2);
	} glEnd();
}


void glwin::GLUTviewer::doKeyboard(GLubyte key, GLint x, GLint y) {
	//Model Transformation : Rotation & Scale
	switch (key) {
	case 27:  break;
	case 'A': case 'a': angleY += 0.5f; break;
	case 'D': case 'd': angleY -= 0.5f; break;
	case 'W': case 'w': angleX += 0.5f; break;
	case 'S': case 's': angleX -= 0.5f; break;
	case 'Q': case 'q': angleZ += 0.5f; break;
	case 'E': case 'e': angleZ -= 0.5f; break;
	case 'R': case 'r': angleX = angleY = angleZ = cameraRX = cameraRY = 0.f; break;

	case '+': scale += 1.0f; break;
	case '-': scale -= 1.0f; break;
	case 'Z': case 'z': scale = 1.f; break;

	case '[': voxelSize -= 0.001f; break;
	case ']': voxelSize += 0.001f; break;
	case 'P': case 'p': voxelSize = 0.05f; break;
	}

	//Lighting Transform : Translation
	switch (key) {
	case 'J': case 'j': lightX -= 0.05f; break;
	case 'L': case 'l': lightX += 0.05f; break;
	case 'I': case 'i': lightY += 0.05f; break;
	case 'K': case 'k': lightY -= 0.05f; break;
	case 'U': case 'u': lightZ -= 0.05f; break;
	case 'O': case 'o': lightZ += 0.05f; break;

	case '9': lightScale -= 0.05f; if (lightScale < 0.f) lightScale = 0.f; break;
	case '0': lightScale += 0.05f; if (lightScale > 10.f) lightScale = 10.f; break;
	case ' ': lightX = lightY = lightZ = 0.f; break;
	}

	char titleInfo[128] = { 0 };
	sprintf_s(titleInfo, "Model R(%.2f, %.2f, %.2f), T(%.2f, %.2f, %.2f), S(%.3f), Light %.2f : (%.2f, %.2f, %.2f), Voxel %.3f",
		angleX, angleY, angleZ, transX, transY, transZ, scale, lightScale, lightX, lightY, lightZ, voxelSize);
	glutSetWindowTitle(titleInfo);

	glutPostRedisplay();
}


void glwin::GLUTviewer::doSpecialKey(GLint key, GLint x, GLint y) {
	//Model Transformation : Translation
	switch (key) {
	case GLUT_KEY_UP: transY += 0.05f; break;
	case GLUT_KEY_DOWN: transY -= 0.05f; break;
	case GLUT_KEY_LEFT: transX -= 0.05f; break;
	case GLUT_KEY_RIGHT: transX += 0.05f; break;
	case GLUT_KEY_PAGE_UP: transZ += 0.05f; break;
	case GLUT_KEY_PAGE_DOWN: transZ -= 0.05f; break;
	case GLUT_KEY_HOME:	transX = transY = transZ = cameraTX = cameraTY = 0.f; break;
	}

	switch (key) {
	case GLUT_KEY_F1: //Drawing Axis On/Off Switch
		if (bDrawAxis) bDrawAxis = false;
		else bDrawAxis = true;
		break;
	case GLUT_KEY_F2: //Drawing Grid Area On/Off Switch
		if (bDrawGrid) bDrawGrid = false;
		else bDrawGrid = true;
		break;
	case GLUT_KEY_F3: //Drawing 3D Lighting and Shading Render
		if (bDrawLight) bDrawLight = false;
		else bDrawLight = true;
		break;
	case GLUT_KEY_F4: //Drawing Information Caption On/Off Switch
		if (bDrawCaption) bDrawCaption = false;
		else bDrawCaption = true;
		break;
	case GLUT_KEY_F5:
		if (bDebug) bDebug = false;
		else bDebug = true;
		break;
	}

	char titleInfo[128] = { 0 };
	sprintf_s(titleInfo, "Model R(%.2f, %.2f, %.2f), T(%.2f, %.2f, %.2f), S(%.3f), Light %.2f : (%.2f, %.2f, %.2f), Voxel %.3f",
		angleX, angleY, angleZ, transX, transY, transZ, scale, lightScale, lightX, lightY, lightZ, voxelSize);
	glutSetWindowTitle(titleInfo);

	glutPostRedisplay();
}


void glwin::GLUTviewer::doMouse(GLint button, GLint state, GLint x, GLint y) {
	//Arcball Function : Left Drag = Camera Rotation
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) { //When First Pressed Button
		bArcball = true;
		preCursorX = nowCursorX = x;
		preCursorY = nowCursorY = y;
	}
	else {
		bArcball = false;
	}

	//Arcball Function : Wheel Drag = Camera Perallel Translation
	if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) { //When First Pressed Button
		bParallel = true;
		preCursorX = nowCursorX = x;
		preCursorY = nowCursorY = y;
	}
	else {
		bParallel = false;
	}
}


void glwin::GLUTviewer::doMotion(GLint x, GLint y) {
	//Viewing Camera Transformation
	if (bArcball == true) { //if Mouse Left Button Pressed : include Dragging
		nowCursorX = x;
		nowCursorY = y;

		//Arcball Rotation
		if (nowCursorX != preCursorX || nowCursorY != preCursorY) {
			cameraRX += (nowCursorX - preCursorX)*ARCBALL_SENS;
			cameraRY += (nowCursorY - preCursorY)*ARCBALL_SENS;

			preCursorX = nowCursorX;
			preCursorY = nowCursorY;
		}
	}
	else if (bParallel == true) { //if Mouse Wheel Button Pressed : include Dragging
		nowCursorX = x;
		nowCursorY = y;

		//Parellel Translation
		if (nowCursorX != preCursorX || nowCursorY != preCursorY) {
			cameraTX += (nowCursorX - preCursorX)*ARCBALL_SENS;
			cameraTY -= (nowCursorY - preCursorY)*ARCBALL_SENS;

			preCursorX = nowCursorX;
			preCursorY = nowCursorY;
		}
	}

	char titleInfo[128] = { 0 };
	sprintf_s(titleInfo, "View R(%.1f, %.1f), T(%.f, %.f), S(%.1f)", cameraRX, cameraRY, cameraTX, cameraTY, scale);
	glutSetWindowTitle(titleInfo);

	glutPostRedisplay();
}


void glwin::GLUTviewer::doReshape(GLsizei width, GLsizei height) {
	//Update Window Size
	this->width = width;
	this->height = height;

	//Viewport Transformation : Basic Display Size == Camera Image Size
	glViewport(0, 0, width, height);

	//Projection Parameter for Viewport Transform
	this->vLeft = -0.5f * width;
	this->vRight = width - 1 - 0.5f * width;
	this->vBottom = -0.5f * height;
	this->vTop = height - 1 - 0.5f * height;

	//Projection Transformation for Viewport
	glMatrixMode(GL_PROJECTION); {
		glLoadIdentity();
		glOrtho(this->vLeft, this->vRight, this->vBottom, this->vTop, this->vNear, this->vFar);
	}

	char titleInfo[128] = { 0 };
	sprintf_s(titleInfo, "Viewport %d x %d", width, height);
	glutSetWindowTitle(titleInfo);

	glutPostRedisplay();
}


void glwin::GLUTviewer::doMenu(GLint value) {
	switch (value) {
	case MENU_EXIT_ESCAPE:  break;
	case MENU_DRAW_XYZ_AXIS: //Drawing Axis On/Off Switch
		if (bDrawAxis) bDrawAxis = false;
		else bDrawAxis = true;
		break;
	case MENU_DRAW_GRID_PLANE: //Drawing Grid Area On/Off Switch
		if (bDrawGrid) bDrawGrid = false;
		else bDrawGrid = true;
		break;
	case MENU_DRAW_LIGHTING: //Drawing 3D Lighting and Shading Render
		if (bDrawLight) bDrawLight = false;
		else bDrawLight = true;
		break;
	case MENU_DRAW_INFO: //Drawing Information Caption On/Off Switch
		if (bDrawCaption) bDrawCaption = false;
		else bDrawCaption = true;
		break;
	case MENU_DEBUG_SWITCH:
		if (bDebug) bDebug = false;
		else bDebug = true;
		break;
	}

	switch (value) {
	case MENU_DRAW_VX_COLOR:
		if (bDrawVxColor) bDrawVxColor = false;
		else bDrawVxColor = true;
		break;
	case COLOR_VX_BLACK:	voxelColor = LUT_BLACK; break;
	case COLOR_VX_WHITE:	voxelColor = LUT_WHITE; break;
	case COLOR_VX_GRAY:		voxelColor = LUT_GRAY; break;
	case COLOR_VX_RED:		voxelColor = LUT_RED; break;
	case COLOR_VX_GREEN:	voxelColor = LUT_GREEN; break;
	case COLOR_VX_BLUE:		voxelColor = LUT_BLUE; break;
	case COLOR_VX_CYAN:		voxelColor = LUT_CYAN; break;
	case COLOR_VX_MAGENTA:	voxelColor = LUT_MAGENTA; break;
	case COLOR_VX_YELLOW:	voxelColor = LUT_YELLOW; break;
	case COLOR_VX_ORANGE:	voxelColor = LUT_ORANGE; break;
	case COLOR_VX_NAVY:		voxelColor = LUT_NAVY; break;
	}

	switch (value) {
	case COLOR_BG_BLACK:	bgColor = LUT_BLACK; break;
	case COLOR_BG_WHITE:	bgColor = LUT_WHITE; break;
	case COLOR_BG_GRAY:		bgColor = LUT_GRAY; break;
	case COLOR_BG_RED:		bgColor = LUT_RED; break;
	case COLOR_BG_GREEN:	bgColor = LUT_GREEN; break;
	case COLOR_BG_BLUE:		bgColor = LUT_BLUE; break;
	case COLOR_BG_CYAN:		bgColor = LUT_CYAN; break;
	case COLOR_BG_MAGENTA:	bgColor = LUT_MAGENTA; break;
	case COLOR_BG_YELLOW:	bgColor = LUT_YELLOW; break;
	case COLOR_BG_ORANGE:	bgColor = LUT_ORANGE; break;
	case COLOR_BG_NAVY:		bgColor = LUT_NAVY; break;
	}

	glutPostRedisplay();
}


void glwin::GLUTviewer::doDisplay() {
	glClearColor(bgColor[0], bgColor[1], bgColor[2], 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	
	//Projection Transform using Camera Matrix
	glMatrixMode(GL_PROJECTION); {
		glLoadIdentity();
		glOrtho(this->vLeft, this->vRight, this->vBottom, this->vTop, this->vNear, this->vFar);
	}

	//Model-Viewing Transform : Arcball Viewing, Dynamic Modeling Transform
	glMatrixMode(GL_MODELVIEW); {
		//Bitmap Font Caption
		if (bDrawCaption) {
			glPushMatrix(); {
				glColor3f(1.f, 0.f, 0.f);
				glRasterPos2f(-0.5f*this->width + 10.f, 0.5f*this->height - 20.f);
				glutBitmapString(GLUT_BITMAP_HELVETICA_12, (const uchar*)"3D Point Cloud Viewer");
				for (int k = 0; k < this->caption.size(); ++k) {
					glRasterPos2f(-0.5f*this->width + 10.f, 0.5f*this->height - (35.f + (k*15.f)));
					glutBitmapString(GLUT_BITMAP_HELVETICA_12, (const uchar*)this->caption[k].c_str());
				}
			} glPopMatrix();
		}

		//Main Objects
		glPushMatrix(); {
			//Model Transform + Viewing Camera Transform
			glTranslatef(cameraTX, cameraTY, -2.f);
			glTranslatef(transX, transY, transZ);
			glRotatef(cameraRX, 0.f, 1.f, 0.f);
			glRotatef(cameraRY, 1.f, 0.f, 0.f);
			glScalef(scale, scale, scale);
			glRotatef(angleX, 1.f, 0.f, 0.f);
			glRotatef(angleY, 0.f, 1.f, 0.f);
			glRotatef(angleZ, 0.f, 0.f, 1.f);
			
			if (bDrawAxis) draw3dAxis(2.f); //drawing Axis
			if (bDrawGrid) drawGrid(1.f, 10); //Grid : Animation Field
			if (bDrawLight) doLightShading(); //Lighting & Shading Setting
			else glDisable(GL_LIGHTING);
			
			//drawing Model Point Cloud
			if (n_points) {
				if (bDrawVxColor) {
					for (int i = 0; i < ModelVertex.size(); ++i) {
						drawVoxel(ModelVertex[i], ModelColor[i]);
					}
				}
				else {
					for (int i = 0; i < ModelVertex.size(); ++i) {
						drawVoxel(ModelVertex[i], voxelColor);
					}
				}
			}
		} glPopMatrix();
	}

	//Swap Double Buffers for Graphics Video
	glutSwapBuffers();
}