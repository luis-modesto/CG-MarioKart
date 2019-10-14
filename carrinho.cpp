// OpenGL Utility Toolkit
#include <GL/glut.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include <math.h>

using namespace std;

// Apenas requerido pelas aplica��es em windows
//#include <windows.h>
int angulo_x = 0;
int angulo_y = 0;
int angulo_z = 0;

vector<float> vertices[3];
vector<int> faces[3];
vector<float> normais[3];
vector<float> texturas[2];

GLUquadricObj *quadratic;

// criar nova função para controle do teclado
void keyboard (unsigned char key, int x, int y){
  switch (key) {
    case 'x': 
      angulo_x++; 
      glutPostRedisplay(); 
      break;
    case 'y': 
      angulo_y++; 
      glutPostRedisplay(); 
      break;   
  }
}

// criar nova função para controle do mouse
void mouse(int button, int state, int x, int y){
  switch (button) {
    case GLUT_LEFT_BUTTON:
      if (state == GLUT_DOWN) {
        angulo_z++;
        glutPostRedisplay();
      }
      break;
    default:
      break;
  }
}

void init(void) 
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glOrtho(-5.0, 5.0, -5.0, 5.0, -5.0, 5.0);
}

// Fun��o callback chamada para fazer o desenho
void Desenha(){
  // Limpa a janela de visualiza��o com a cor
  // de fundo especificada
  glClear(GL_COLOR_BUFFER_BIT);

 glColor3f(1.0, 1.0, 0.0);

 // Desenha um tri�ngulo
  float radius = 1, halfLength = 0.1;
  int slices = 10000;
  for(int i=0; i<slices; i++) {
    float theta = ((float)i)*2.0*3.14;
    float nextTheta = ((float)i+1)*2.0*3.14;
    glBegin(GL_TRIANGLE_STRIP);
    /*vertex at middle of end */ glVertex3f(0.0, halfLength, 0.0);
    /*vertices at edges of circle*/ glVertex3f(radius*cos(theta), halfLength, radius*sin(theta));
                                              glVertex3f (radius*cos(nextTheta), halfLength, radius*sin(nextTheta));
    /* the same vertices at the bottom of the cylinder*/
     glVertex3f (radius*cos(nextTheta), -halfLength, radius*sin(nextTheta));
    glVertex3f(radius*cos(theta), -halfLength, radius*sin(theta));
    glVertex3f(0.0, -halfLength, 0.0);
    glEnd();
  }

glRotatef(45, 45, 0, 1);
glTranslatef(0, -2, 0);

  // Define a cor de desenho como vermelho
  glColor3f(0.0, 0.0, 1.0);

static GLint vertices[8*3] = { 
    -1, 1, -1,
    -1, 1, 1,
    -1, -1, -1,
    -1, -1, 1,
    3, 1, -1,
    3, 1, 1,
    3, -1, -1, 
    3, -1, 1
 };

 static GLubyte faces[6*4] = {
     0, 5, 7, 3,
     0, 4, 6, 2,
     0, 1, 3, 2,
     5, 4, 6, 7,
     0, 4, 5, 1,
     3, 7, 6, 2
 };

 glEnableClientState(GL_VERTEX_ARRAY);
 glVertexPointer(3, GL_INT, 0, vertices);
 glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, faces);
 glDisableClientState(GL_VERTEX_ARRAY);

  //glRotatef(angulo_x,1,0,0);
  //glRotatef(angulo_y,0,1,0);
  //glRotatef(angulo_z,0,0,1);
  // Executa os comandos OpenGL para renderiza��o
  glFlush();
}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   glFrustum(-450.0, 450.0, -450.0, 450.0, 1.0, 50.0);
   glMatrixMode (GL_MODELVIEW);
}

int main(int argc, char** argv)
{
   //carrega_pista();
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize (256, 256); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow ("Olá Mundo-Visualização");
   glutKeyboardFunc(keyboard);
   glutMouseFunc(mouse);
   init ();
   glutDisplayFunc(Desenha); 
   //glutReshapeFunc(reshape);
   glutMainLoop();
   return 0;
}
