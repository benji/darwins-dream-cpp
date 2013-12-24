#include <stdio.h>
#include <iostream>
#include <GL/gl.h>
#include <GL/glut.h>
#include "rendering.h"

using namespace std;

static int width = 640;
static int height = 480;
static DrawFuncType drawFunc;
static DrawFuncType exitFunc;

void Rendering::drawCube(float x, float y, float z, float r, float g, float b) {
  glPushMatrix();
  glTranslatef(x, y, z);

  glBegin(GL_QUADS);

  glColor3f(r, g, b);

  // x=0
  glVertex3f(0, 0, 0);
  glVertex3f(0, 0, 1);
  glVertex3f(0, 1, 1);
  glVertex3f(0, 1, 0);

  // y=0
  glVertex3f(0, 0, 0);
  glVertex3f(0, 0, 1);
  glVertex3f(1, 0, 1);
  glVertex3f(1, 0, 0);

  // z=0
  glVertex3f(0, 0 ,0);
  glVertex3f(1, 0, 0);
  glVertex3f(1, 1, 0);
  glVertex3f(0, 1, 0);

  // x=1
  glVertex3f(1, 0, 0);
  glVertex3f(1, 0, 1);
  glVertex3f(1, 1, 1);
  glVertex3f(1, 1, 0);

  // y=1
  glVertex3f(0, 1, 0);
  glVertex3f(0, 1, 1);
  glVertex3f(1, 1, 1);
  glVertex3f(1, 1, 0);

  // z=1
  glVertex3f(0, 0 ,1 );
  glVertex3f(1, 0, 1);
  glVertex3f(1, 1, 1);
  glVertex3f(0, 1, 1);

  glEnd();

  glPopMatrix();
}

void Rendering::display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
  gluLookAt(20, -10, 20, 10, 0, 10, 0.0, 0.0, 1.0);

  // GRID
  glColor3f(1.0f, 1.0f, 1.0f);
  for (int i=1; i<100; i++) {
    glBegin(GL_LINES);
      glVertex3f(i,   0, 0);
      glVertex3f(i, 100, 0);
    glEnd();
    glBegin(GL_LINES);
      glVertex3f(0,   i, 0);
      glVertex3f(100, i, 0);
    glEnd();
  }

  // AXIS
  glColor3f(1, 0, 0);
  glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(100, 0, 0);
  glEnd();
  glColor3f(0, 1, 0);
  glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 100, 0);
  glEnd();
  glColor3f(0, 0, 1);
  glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 100);
  glEnd();

  drawFunc();

	glutSwapBuffers();
}

void Rendering::keyboard ( unsigned char key, int mouseX, int mouseY ) {
  switch ( key ) {
    case 27: // esc
      exitFunc();
      exit ( 0 );   
      break;      
    default:      
      break;
  }
}


void Rendering::initialize(DrawFuncType func1, DrawFuncType func2)  {
  drawFunc = func1;
  exitFunc = func2;
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
	glutInitWindowSize(width, height);
	glutCreateWindow("Darwin's dream");
	glutDisplayFunc(Rendering::display);
	glutIdleFunc(Rendering::display);
  glutKeyboardFunc(Rendering::keyboard);

  glMatrixMode(GL_PROJECTION);
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  GLfloat aspect = (GLfloat) width / height;
  gluPerspective(45, aspect, 1, 500);
  glMatrixMode(GL_MODELVIEW);
  glShadeModel( GL_SMOOTH );
  glClearDepth( 1.0f );
  glEnable( GL_DEPTH_TEST );
  glDepthFunc( GL_LEQUAL );
  glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
  glClearColor(0.0, 0.0, 0.0, 1.0);
}
