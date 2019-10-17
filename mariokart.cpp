// OpenGL Utility Toolkit
#include <GL/glut.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include <map>
#include "load_bmp.h"
#include<bits/stdc++.h>

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
vector<string> nomesTexturas;
map<string, string> mapmtl;
map<string, int> nomeIdMtl;

vector<string> split(string texto, char sep){
	vector<string> retorno;
	stringstream ss(texto);
	string token;

	while(getline(ss, token, sep)) {
	    retorno.push_back(token);
	}

	return retorno;
}

void carrega_mtl(){
  fstream file;
	file.open("N64 Toad's Turnpike.mtl");
  string utilmtl;
  int id = 0;

  if (file.is_open()){
    string linha;
		while(!file.eof()){
      getline(file, linha);

      vector<string> conteudo = split(linha, ' ');
      if(conteudo[0] == "newmtl"){
        utilmtl = conteudo[1];
        nomeIdMtl[utilmtl] = id;
        id++;
      }
      else if(conteudo[0] == "map_Kd"){
        string nome_lower = conteudo[1];
        transform(nome_lower.begin(), nome_lower.end(), nome_lower.begin(), ::tolower);
        mapmtl[utilmtl] = nome_lower;
      }
    }

  }
}

void carrega_pista(){
  carrega_mtl();
  string utilmtl;
	fstream file;
	file.open("N64 Toad's Turnpike.obj");

	if (file.is_open()){
		string linha;
		while(!file.eof()){
      getline(file, linha);

			vector<string> conteudo = split(linha, ' ');
			if (conteudo[0]=="v"){
				for (int i = 1; i<conteudo.size(); i++){
					vertices[i-1].push_back(atof(conteudo[i].c_str()));
				}
			} else if (conteudo[0]=="f"){
				for (int i = 1; i<conteudo.size(); i++){
					vector<string> indices_face = split(conteudo[i], '/');
					faces[0].push_back(atoi(indices_face[0].c_str())-1); // vertice
          nomesTexturas.push_back(utilmtl);
					faces[1].push_back(atoi(indices_face[1].c_str())); // textura
          nomesTexturas.push_back(utilmtl);
					faces[2].push_back(atoi(indices_face[2].c_str())); // normal
          nomesTexturas.push_back(utilmtl);
				}
			} else if (conteudo[0]=="vn"){
				for (int i = 1; i<conteudo.size(); i++){
					normais[i-1].push_back(atof(conteudo[i].c_str()));
				}
			} else if (conteudo[0]=="vt"){
				for (int i = 1; i<conteudo.size(); i++){
					texturas[i-1].push_back(atof(conteudo[i].c_str()));
				}
			} else if (conteudo[0]=="usemtl"){
				// Deus já sabe
        utilmtl = conteudo[1];
			} else if (conteudo[0]=="o"){
				// deve comecar outro objeto, nao sei
			} else if (conteudo[0]=="s"){
				//tambem nao sei o que significa
				if (conteudo[1]=="off"){
					//sabe Deus
				} else {
					// sabe Deus tambem
				}
			} else if (conteudo[0]=="#"){
				continue;
			}
		}
	}
}

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

GLuint textureID[79];
void init(void) 
{
  glClearColor (0.0, 0.0, 0.0, 0.0);
  glOrtho(-450.0, 450.0, -450.0, 450.0, -450.0, 450.0);
  // GLfloat light_position[] = { -450.0, -450.0, 450.0, 0.0 };
  // glLightfv(GL_LIGHT0, GL_POSITION,light_position);
  // glEnable(GL_LIGHTING);
  // glEnable(GL_LIGHT0);

  glGenTextures(79, textureID);
  unsigned int width, height;
  unsigned char *data;

  for(map<string, string>::iterator it = mapmtl.begin(); it != mapmtl.end(); it++){
    glBindTexture(GL_TEXTURE_2D, textureID[nomeIdMtl[it->first]]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    string caminho = "bmp/" + it->second;
    data = loadBMP(caminho.c_str(), width, height);
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
  }

  glEnable(GL_TEXTURE_2D);

  // GLfloat light_diffuse[]={1.0, 1.0, 1.0, 1.0};
  // glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);

  // GLfloat mat_diffuse[]={1.0, 1.0, 1.0, 1.0};
  // glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse);
}

// Fun��o callback chamada para fazer o desenho
void Desenha(){
  // Limpa a janela de visualiza��o com a cor
  // de fundo especificada
  glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);

  for(int i = 0; i < faces[0].size(); i+=3){
    glBindTexture(GL_TEXTURE_2D, textureID[nomeIdMtl[nomesTexturas[i]]]);
    glBegin(GL_TRIANGLES);
      glTexCoord2f(texturas[0][faces[1][i]], texturas[1][faces[1][i]]);
      glVertex3f(vertices[0][faces[0][i]], vertices[1][faces[0][i]], vertices[2][faces[0][i]]);

      glTexCoord2f(texturas[0][faces[1][i+1]], texturas[1][faces[1][i+1]]); 
      glVertex3f(vertices[0][faces[0][i+1]], vertices[1][faces[0][i+1]], vertices[2][faces[0][i+1]]);

      glTexCoord2f(texturas[0][faces[1][i+2]], texturas[1][faces[1][i+2]]);
      glVertex3f(vertices[0][faces[0][i+2]], vertices[1][faces[0][i+2]], vertices[2][faces[0][i+2]]); 
    glEnd();
  }

  // Desenha um tri�ngulo
  /*GLfloat vert[vertices[0].size()*3];

  int j = 0;
  for(int i = 0; i < vertices[0].size(); i++){
    vert[j] = vertices[0][i];
    vert[j+=1] = vertices[1][i];
    vert[j+=1] = vertices[2][i];
    j+=1;
  }
  
  GLint face[faces[0].size()];

  for(int i = 0; i < faces[0].size(); i++){
    face[i] = faces[0][i];
  }

  GLfloat normal[normais[0].size() * 3];

  j = 0;
  for(int i = 0; i < normais[0].size(); i++){
    normal[j] = normais[0][i];
    normal[j++] = normais[1][i];
    normal[j++] = normais[2][i];
  }

  GLint textura[texturas[0].size()];

  for(int i = 0; i < texturas[0].size(); i++){
    textura[i] = texturas[0][i];
  }


  glRotatef(angulo_x,1,0,0);
  glRotatef(angulo_y,0,1,0);
  glRotatef(angulo_z,0,0,1);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, vert);
  glDrawElements(GL_TRIANGLES, faces[0].size(), GL_UNSIGNED_INT, face);
  
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glDisableClientState(GL_VERTEX_ARRAY);

  // Executa os comandos OpenGL para renderiza��o*/
  glRotatef(angulo_x,1,0,0);
  glRotatef(angulo_y,0,1,0);
  glRotatef(angulo_z,0,0,1);
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
   carrega_pista();
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
