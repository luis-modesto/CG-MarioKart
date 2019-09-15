// OpenGL Utility Toolkit
#include <GL/glut.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <stdlib.h>

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

vector<string> split(string texto, char sep){
	vector<string> retorno;
	stringstream ss(texto);
	string token;

	while(getline(ss, token, sep)) {
	    retorno.push_back(token);
	}

	return retorno;
}

void carrega_pista(){
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
					faces[1].push_back(atoi(indices_face[1].c_str())); // textura
					faces[2].push_back(atoi(indices_face[2].c_str())); // normal
				}
			} else if (conteudo[0]=="vn"){
				for (int i = 1; i<conteudo.size(); i++){
					normais[i-1].push_back(atof(conteudo[i].c_str()));
				}
			} else if (conteudo[0]=="vt"){
				for (int i = 1; i<conteudo.size(); i++){
					texturas[i-1].push_back(atof(conteudo[i].c_str()));
				}
			} else if (conteudo[0]=="usemtl" or conteudo[0]=="mtllib"){
				// Deus quem sabe
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
// Inicializa��es de OpenGL que devem ser
// executadas antes da exibi��o do desenho
void Inicializa(){

  // Define a janela de visualiza��o
  glMatrixMode(GL_PROJECTION);

  // Define o sistema de coordenadas
  glOrtho(-75.0, 75.0, -75.0, 75.0, -75.0, 75.0);

  // Define a cor de fundo da janela como azul
  glClearColor(0.0, 0.0, 0.0, 0.0);
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

// Fun��o callback chamada para fazer o desenho
void Desenha(){
  // Limpa a janela de visualiza��o com a cor
  // de fundo especificada
  glClear(GL_COLOR_BUFFER_BIT);

  // Define a cor de desenho como vermelho
  glColor3f(1.0, 1.0, 1.0);

  // Desenha um tri�ngulo
  GLint vert[vertices[0].size()*3];

  int j = 0;
  for(int i = 0; i < vertices[0].size(); i++){
    vert[j] = vertices[0][i];
    vert[j++] = vertices[1][i];
    vert[j++] = vertices[2][i];
  }
  
  GLubyte face[faces[0].size()];

  for(int i = 0; i < faces[0].size(); i++){
    face[i] = faces[0][i];
  }

  GLint normal[normais[0].size() * 3];

  j = 0;
  for(int i = 0; i < normais[0].size(); i++){
    normal[j] = normais[0][i];
    normal[j++] = normais[1][i];
    normal[j++] = normais[2][i];
  }


  glRotatef(angulo_x,1,0,0);
  glRotatef(angulo_y,0,1,0);
  glRotatef(angulo_z,0,0,1);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_INT, 0, vert);
  glDrawElements(GL_TRIANGLES, faces[0].size(), GL_UNSIGNED_BYTE, face);
  glNormalPointer(GL_INT, 0, normal);

  
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glDisableClientState(GL_VERTEX_ARRAY);

  // Executa os comandos OpenGL para renderiza��o
  glFlush();
}

// Programa principal
int main(int argc, char** argv){

  carrega_pista();

  // Inicia GLUT e processa argumentos passados por linha de comandos
  glutInit(&argc, argv);

  // Avisa a GLUT que tipo de modo de exibi��o deve ser usado quando a janela � criada
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

  // Cria uma janela GLUT que permite a execu��o de comandos OpenGL
  glutCreateWindow("Ol� Mundo!");

  //glutKeyboardFunc(keyboard);
  glutMouseFunc(mouse);

  // Define a fun��o respons�vel por redesenhar a janela OpenGL sempre que necess�rio
  glutDisplayFunc(Desenha);

  // Inicializa��es de OpenGL executadas antes da exibi��o do desenho
  Inicializa();

  // Inicia o processamento de eventos de GLUT. O controle do programa
  // passa a GLUT, que inicia o gerenciamento dos eventos
  glutMainLoop();

  return 0;
}
