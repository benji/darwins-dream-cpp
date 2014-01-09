#include <GL/gl.h>
#include <GL/glut.h>
#include <ctime>
#include <unistd.h>
#include "rendering.h"
#include "common.h"
#include "World.h"
#include "Clocks.h"

using namespace std;

static int width = WINDOW_WIDTH;
static int height = WINDOW_HEIGHT;
static VoidFuncType drawWorldFunc;
static VoidFuncType drawDominantSpeciesFunc;

void Rendering::resize(int w, int h){
  width = w;
  height = h;
}

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

void Rendering::drawAxis(){
  // axis
  glColor3f(1, 0, 0);
  glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(10000, 0, 0);
  glEnd();
  glColor3f(0, 1, 0);
  glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 10000, 0);
  glEnd();
  glColor3f(0, 0, 1);
  glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 10000);
  glEnd();
}

void Rendering::prepareView(int x, int y, int dx, int dy){
  glViewport(x, y, dx, dy);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  GLfloat aspect = (GLfloat) dx / dy;
  gluPerspective(45, aspect, 1, 5*world.length);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void Rendering::display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // WORLD VIEW
  prepareView(0, 0, 2*width/3, height);
  gluLookAt(world.length*1.2, -world.length*.5, world.length*1, world.length/2, world.length/2, 0, 0.0, 0.0, 1.0);

  // grid
  glColor3f(.3f, .3f, .3f);
  for (int i=10; i<=1000; i+=10) {
    glBegin(GL_LINES);
      glVertex3f(i,   0, 0);
      glVertex3f(i, 1000, 0);
    glEnd();
    glBegin(GL_LINES);
      glVertex3f(0,   i, 0);
      glVertex3f(1000, i, 0);
    glEnd();
  }

  drawAxis();
  drawWorldFunc();

  // DOMINANT SPECIES VIEW
  prepareView(2*width/3, 0, width/3, height);
  gluLookAt(20, -15, 20, 0, 0, 6, 0.0, 0.0, 1.0);

  glPushMatrix();
    long t = time(0);
    float delta = (t-t%10) - Timer::now()/1000000.;
    glRotatef(- delta*36, 0, 0, 1);
    drawAxis();
    drawDominantSpeciesFunc();
  glPopMatrix();

  glutSwapBuffers();
}


void Rendering::initialize(VoidFuncType drawWorldF, VoidFuncType drawDominantSpeciesF)  {
  drawWorldFunc = drawWorldF;
  drawDominantSpeciesFunc = drawDominantSpeciesF;

  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
  glutInitWindowSize(width, height);
  glutCreateWindow("Darwin's dream");
  glutDisplayFunc(Rendering::display);
  glutReshapeFunc(Rendering::resize);
  glutIdleFunc(Rendering::display);

  glMatrixMode(GL_PROJECTION);
  resize(600,400);
  glMatrixMode(GL_MODELVIEW);
  glShadeModel( GL_SMOOTH );
  glClearDepth( 1.0f );
  glEnable( GL_DEPTH_TEST );
  glDepthFunc( GL_LEQUAL );
  glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
  glClearColor(0.0, 0.0, 0.0, 1.0);
}
