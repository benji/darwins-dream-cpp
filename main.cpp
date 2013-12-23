#include <stdio.h>

#ifdef _WIN32
#	include <windows.h>
#else
#	include <sys/time.h>
#endif

#include <GL/gl.h>
#include <GL/glut.h>

#define KEY_ESCAPE 27

int width = 640;
int height = 480;

void drawCube(float x, float y, float z) {
  glPushMatrix();
  glTranslatef(x, y, z);

  glBegin(GL_QUADS); 
  glVertex3f(0,0 ,0);
  glVertex3f(1.0f, 0, 0);
  glVertex3f(1.0f, 1.0f, 0);
  glVertex3f(0, 1.0f, 0);

  glVertex3f(0, 0, 0);
  glVertex3f(0, 0, 1.0f);
  glVertex3f(0, 1.0f, 0);
  glVertex3f(0, 1.0f, 1.0f);

  glVertex3f(0, 0, 0);
  glVertex3f(0, 0, 1.0f);
  glVertex3f(1.0f, 0, 1.0f);
  glVertex3f(1.0f, 0, 0);

  glVertex3f(0, 0, 1.0f);
  glVertex3f(1.0f, 0, 1.0f);
  glVertex3f(1.0f, 1.0f, 1.0f);
  glVertex3f(0, 1.0f, 1.0f);

  glVertex3f(0,0 ,1.0f );
  glVertex3f(1.0f, 0, 1.0f);
  glVertex3f(1.0f, 1.0f, 1.0f);
  glVertex3f(0, 1.0f, 1.0f);

  glVertex3f(0, 1.0f, 0);
  glVertex3f(0, 1.0f, 1.0f);
  glVertex3f(1.0f, 1.0f, 1.0f);
  glVertex3f(1.0f, 1.0f, 0);
  glEnd();

  glPopMatrix();
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
  gluLookAt(20, -10, 10, 10, 0, 0, 0.0, 0.0, 1.0);

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

  glColor3f(.5, .2, .8);
  drawCube(0,0,0);

	glutSwapBuffers();
}

void keyboard ( unsigned char key, int mouseX, int mouseY ) { 
  switch ( key ) {
    case KEY_ESCAPE:        
      exit ( 0 );   
      break;      
    default:      
      break;
  }
}


void initialize()  {
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
	glutInitWindowSize(width, height);
	glutCreateWindow("Darwin's dream");
	glutDisplayFunc(display);
	glutIdleFunc( display );
  glutKeyboardFunc(keyboard);

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


int main(int argc, char **argv) {
	glutInit(&argc, argv);
	initialize();
	glutMainLoop();

	return 0;
}


