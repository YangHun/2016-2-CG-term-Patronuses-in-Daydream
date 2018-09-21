#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

#include <Windows.h>
#include <iostream>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/glu.h>

#include "glm\glm.hpp"
#include "glm\ext.hpp"

#include "Engine.h"

using namespace std;

//Window Size
int						WINDOW_WIDTH;
int						WINDOW_HEIGHT;
//Camera
float					g_Zoom;				// view zoom
float					g_Rotate[3];		// x,y
float					g_Translate[3];		// x,y
glm::vec3				g_CameraLook;
//Mouse
int						g_Mouse_Coord[2];	// previous mouse coordinates
unsigned char			g_Mouse_Event[3];	// mouse event handler

Engine		g_Engine;

glm::mat4 viewMat;
glm::mat4 projMat;
glm::mat4 modelMat;

int width;
int height;

void Print_ViewMat()
{
	cout << "viewMatrix : "<< endl;
	cout << viewMat[0][0]		<< " "		<< viewMat[0][1]		<< " "		<< viewMat[0][2]		<< " "		<< viewMat[0][3]<< endl;
	cout << viewMat[1][0]		<< " "		<< viewMat[1][1]		<< " "		<< viewMat[1][2]		<< " "		<< viewMat[1][3]<< endl;
	cout << viewMat[2][0]		<< " "		<< viewMat[2][1]		<< " "		<< viewMat[2][2]		<< " "		<< viewMat[2][3]<< endl;
	cout << viewMat[3][0]		<< " "		<< viewMat[3][1]		<< " "		<< viewMat[3][2]		<< " "		<< viewMat[3][3]<< endl;
	cout << endl;

	cout << "modelMatrix : "<< endl;
	cout << modelMat[0][0]		<< " "		<< modelMat[0][1]		<< " "		<< modelMat[0][2]		<< " "		<< modelMat[0][3]<< endl;
	cout << modelMat[1][0]		<< " "		<< modelMat[1][1]		<< " "		<< modelMat[1][2]		<< " "		<< modelMat[1][3]<< endl;
	cout << modelMat[2][0]		<< " "		<< modelMat[2][1]		<< " "		<< modelMat[2][2]		<< " "		<< modelMat[2][3]<< endl;
	cout << modelMat[3][0]		<< " "		<< modelMat[3][1]		<< " "		<< modelMat[3][2]		<< " "		<< modelMat[3][3]<< endl;
	cout << endl;
}

void Print_CameraInfo()
{
	cout << "Zoom : "		<< g_Zoom			<< endl;
	cout << "Rotate : "		<< g_Rotate[0]		<< " "		<< g_Rotate[1]		<< " "		<< g_Rotate[2]		<< endl;
	cout << "Translate : "	<< g_Translate[0]	<< " "		<< g_Translate[1]	<< " "		<< g_Translate[2]	<< endl;
	cout << endl;
}

void Initialize(void)
{
	//To do : Initialize Code
	g_Engine.Initialize();

	//Console Window 위치 변경
	HWND hWnd = ::FindWindow("ConsoleWindowClass" ,NULL );
	::MoveWindow(hWnd , 0 , 0 , 500 , 500, TRUE );

	g_Zoom = 33.8f;

	for(int i=0; i<3; i++)
	{
		g_Rotate[i] = 0.0f;
		g_Translate[i] = 0.0f;
		g_Mouse_Coord[i] = 0.0f;
	}

	g_Translate[0] = 0.0f;
	g_Translate[1] = 0.0f;
	g_Translate[2] = 0.0f;
	g_Rotate[0]	   = 2.0f;
	g_Rotate[1]	   = -0.4f;

	g_Mouse_Event[0] = g_Mouse_Event[1] = g_Mouse_Event[2] = 0;

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.2f, 1.0f );
}

void Render(void)
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glm::vec3 eyeVec = glm::vec3(0.0, 0.0, 3.0);
	glm::vec3 lookAtVec = glm::vec3(0.0, 0.0, 0.0);
	glm::vec3 upVec = glm::vec3(0.0, 1.0, 0.0);
	
	g_CameraLook = lookAtVec;

	viewMat = glm::lookAt(eyeVec, lookAtVec, upVec);
	projMat = glm::perspective(45.0f, (float)width/height, 0.1f, 500.0f);
	modelMat = glm::mat4(1.0f);

	glEnable (GL_LIGHTING);
	glEnable(GL_LIGHT0);

	float light_pos[] = {2.0f, 2.0f, 2.0f, 1.0f};
	float light_dir[] = {-0.5f, -0.5f, 0.0f};
	float light_ambient[] = {0.5f, 0.5f, 0.5f, 1.0f};
	float light_diffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
	float light_specular[] ={1.0f, 1.0f, 1.0f, 1.0f};

	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	//glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light_dir);
	//glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 60.0f);
	//glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 2.0f);

	float noMat[] = {0.0f, 0.0f, 0.0f, 1.0f};
	float matAmb[] = {0.3f, 0.3f, 0.3f, 1.0f};
	float matDif[] ={1.0f, 1.0f, 1.0f, 1.0f};
	float matSpec[] = {1.0f, 1.0f, 1.0f, 1.0f};
	float matShininess = 40.0f;

	glMaterialfv(GL_FRONT, GL_EMISSION, noMat);
	glMaterialfv(GL_FRONT, GL_AMBIENT, matAmb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, matDif);
	glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
	glMaterialf(GL_FRONT, GL_SHININESS, matShininess);

	glPushMatrix();
//		glTranslatef(light_pos[0], light_pos[1], light_pos[2]);
	
		g_Translate[2] = -g_Zoom;

		glTranslatef(g_Translate[0], g_Translate[1], g_Translate[2]);
		glRotatef(g_Rotate[0], 1.0, 0.0, 0.0);
		glRotatef(g_Rotate[1], 0.0, 1.0, 0.0);
	
	//	float Emission[] = {1.0f, 1.0f, 1.0f, 1.0f};

		//glMaterialfv(GL_FRONT, GL_EMISSION, Emission);


		modelMat = glm::translate(modelMat, g_Translate[0], g_Translate[1], g_Translate[2]);
		modelMat = glm::rotate(modelMat,  g_Rotate[0], glm::vec3(1.0, 0.0, 0.0));
		modelMat = glm::rotate(modelMat,  g_Rotate[1], glm::vec3(0.0, 1.0, 0.0));

		//To do :: Rendering Scene
		g_Engine.Render(viewMat, modelMat, projMat);
	glPopMatrix();

	glutSwapBuffers();
	glutPostRedisplay();
}

void Reshape(int w, int h)
{
	width = w;
	height = h;

	if(w==0) 
	{
		h = 1;
	}
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (float)w/h, 0.1, 500);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
}


void Mouse(int mouse_event, int state, int x, int y)
{
	g_Mouse_Coord[0] = x;
	g_Mouse_Coord[1] = y;

	switch(mouse_event)
	{
	case GLUT_LEFT_BUTTON:
		g_Mouse_Event[0] = ((GLUT_DOWN==state)?1:0);
		break;
	case GLUT_MIDDLE_BUTTON:
		g_Mouse_Event[1] = ((GLUT_DOWN==state)?1:0);
		break;
	case GLUT_RIGHT_BUTTON:
		g_Mouse_Event[2] = ((GLUT_DOWN==state)?1:0);
		break;
	default:
		break;		
	}

	glutPostRedisplay();
}


void Motion(int x, int y)
{
	int diffx=x-g_Mouse_Coord[0];
	int diffy=y-g_Mouse_Coord[1];

	g_Mouse_Coord[0] = x;
	g_Mouse_Coord[1] = y;

	if(g_Mouse_Event[0])
	{
		g_Rotate[0] += (float) 0.1 * diffy;
		g_Rotate[1] += (float) 0.1 * diffx;
	}
	if(g_Mouse_Event[1])
	{
		g_Translate[0] += (float) 0.1 * diffx;
		g_Translate[1] -= (float) 0.1 * diffy;
	}
	if(g_Mouse_Event[2])
	{
		g_Zoom += (float) 0.1 * diffy;
	}

	glutPostRedisplay();
}


void Keyboard(unsigned char key, int x, int y)
{
	//To do : Keyboard Input
	g_Engine.Key(key, x, y);

	switch(key) 
	{
	case 'a' :
		g_Translate[0] += 0.3f;
		break;
	case 'w' :
		g_Zoom -= 0.3f;
		break;
	case 'd' :
		g_Translate[0] -= 0.3f;
		break;
	case 's' :
		g_Zoom += 0.3f;
		break;

		case VK_ESCAPE:
			exit(0);
			break;
		case VK_RETURN:

			break;
	}

	glutPostRedisplay();
}

void SpecialKey(int key, int x, int y)
{
	//To do : Keyboard Input
	switch(key) 
	{
		case GLUT_KEY_F1:
			Print_CameraInfo();
			Print_ViewMat();
			break;
	}

	glutPostRedisplay();
}

void Idle(void)
{
	g_Engine.Update(viewMat,modelMat,projMat);

	glutPostRedisplay();
}


int main(int argc, char ** argv)
{
	glutInit(&argc, argv);
	WINDOW_WIDTH = 500;
	WINDOW_HEIGHT = 500;

	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowPosition(501, 0);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("Term ; 2014210079_양은정");

	Initialize();

	glutDisplayFunc(Render);
	glutReshapeFunc(Reshape);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(SpecialKey);
	glutIdleFunc(Idle);

	

	glutMainLoop();

	return 0;
}

