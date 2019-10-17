// OpenGL Utility Toolkit
#include <GL/glut.h>
#include "load_bmp.h"

// Apenas requerido pelas aplica��es em windows
//#include <windows.h>

GLuint textureID[2]; 
// Inicializa��es de OpenGL que devem ser
// executadas antes da exibi��o do desenho
void Inicializa(){

  // Define a janela de visualiza��o
  glMatrixMode(GL_PROJECTION);

  // Define o sistema de coordenadas
  glOrtho(-2.0, 2.0, -2.0, 2.0, -2.0, 2.0);

  // Define a cor de fundo da janela como azul
  glClearColor(0.0, 0.0, 0.0, 1.0);

  glGenTextures(2, textureID);
  unsigned int width, height;
  unsigned char *data;

  glBindTexture(GL_TEXTURE_2D, textureID[0]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  data = loadBMP("parede_512x512.bmp", width, height);
  glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

  glBindTexture(GL_TEXTURE_2D, textureID[1]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  data = loadBMP("img_test_128x128.bmp", width, height);
  glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

  glEnable(GL_TEXTURE_2D);

}

// Fun��o callback chamada para fazer o desenho
void Desenha(){

  // Limpa a janela de visualiza��o com a cor
  // de fundo especificada
  glClear(GL_COLOR_BUFFER_BIT);

  // Define a cor de desenho como vermelho
  //glColor3f(1.0, 0.0, 0.0);


  glBindTexture(GL_TEXTURE_2D, textureID[0]);
  glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex3f(-1, -1, 0.0);
    glTexCoord2f(0, 1);
    glVertex3f(-1, 1, 0.0);
    glTexCoord2f(1, 1);
    glVertex3f(1, 1, 0.0);
    glTexCoord2f(1, 0);
    glVertex3f(1, -1, 0.0);
  glEnd();

  // Desenha um tri�ngulo

  glBindTexture(GL_TEXTURE_2D, textureID[1]);
  glBegin(GL_TRIANGLES);
    glTexCoord2f(1, 1);
    glVertex3f(0.5, 0.5, 0.0);
    glTexCoord2f(0, 1);
    glVertex3f(-0.5, 0.5, 0.0);
    glTexCoord2f(0.5, 0);
    glVertex3f(0.0, -0.5, 0.0);
  glEnd();

  // Executa os comandos OpenGL para renderiza��o
  glFlush();
}

// Programa principal
int main(int argc, char** argv){

  // Inicia GLUT e processa argumentos passados por linha de comandos
  glutInit(&argc, argv);

  // Avisa a GLUT que tipo de modo de exibi��o deve ser usado quando a janela � criada
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

  // Cria uma janela GLUT que permite a execu��o de comandos OpenGL
  glutCreateWindow("Ol� Mundo!");

  // Define a fun��o respons�vel por redesenhar a janela OpenGL sempre que necess�rio
  glutDisplayFunc(Desenha);

  // Inicializa��es de OpenGL executadas antes da exibi��o do desenho
  Inicializa();

  // Inicia o processamento de eventos de GLUT. O controle do programa
  // passa a GLUT, que inicia o gerenciamento dos eventos
  glutMainLoop();

  return 0;
}