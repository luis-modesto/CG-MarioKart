#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <GL/glut.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include "load_bmp.h"
#include<bits/stdc++.h>

using namespace std;


vector<float> vertices[3];
vector<int> faces[3];
vector<float> normais[3];
vector<float> texturas[2];
vector<string> nomesTexturas;
map<string, string> mapmtl;
map<string, int> nomeIdMtl;

#define Vrt glVertex3s


GLuint textureID[79];


int proj = 1, new_mouse = 0, mouse_rot = 1, mouse_trn = 0;

float turn = 50.0, tipp = 30.0, twst = 0.0, x_click = 0, y_click = 0, xtm = 0.0, ytm = 0.0, scl = 100.0, xcr = 0.0;

GLfloat lit1_spc[] = {0.20, 0.20,  0.15, 1.00},
        lit1_dfs[] = {0.70, 0.70,  0.65, 1.00},
		lit1_amb[] = {0.25, 0.25,  0.20, 1.00},
		lit1_pos[] = {0.20, 2.50,  5.20, 1.00};

GLfloat lit2_spc[] = { 0.00,  0.00,  0.00, 1.00},
        lit2_dfs[] = { 0.30,  0.30,  0.35, 1.00},
		lit2_amb[] = { 0.20,  0.20,  0.25, 1.00},
		lit2_pos[] = { 1.50, -2.00, -4.00, 0.01};

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
        //cout << utilmtl << endl;
        //cout << id << endl;
        nomeIdMtl[utilmtl] = id;
        id++;
      }
      else if(conteudo[0] == "map_Kd"){
        string nome_lower = conteudo[1];
        transform(nome_lower.begin(), nome_lower.end(), nome_lower.begin(), ::tolower);
        mapmtl[utilmtl] = nome_lower;
        //cout << utilmtl << " " << nome_lower << endl;
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
					faces[1].push_back(atoi(indices_face[1].c_str())-1); // textura
                    //nomesTexturas.push_back(utilmtl);
					faces[2].push_back(atoi(indices_face[2].c_str())); // normal
                    //nomesTexturas.push_back(utilmtl);
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

//-----------------------------------------   Keybord   --------------------------------------------

void Keybord (unsigned char ch, int x, int y)
{
    printf ("   Keyboard -> '%c' pressed.", ch);

    switch (ch)  {
       case 'r' :  mouse_trn = 0;
                   mouse_rot = 1;            break;
       case 't' :  mouse_trn = 1;
                   mouse_rot = 0;            break;
       case 'z' :  turn = 0.0; tipp = 0.0;
                   xtm  = 0.0; ytm  = 0.0;
                   scl  = 1.0;               break;
       case '-' :  scl *= 0.92;              break;
       case '+' :  scl *= 1.06;              break;
       case 'd' :  xcr += 2.00;              break;
       case 'D' :  xcr -= 2.00;              break;

       case  27 :  exit (0);                 break;
    }

    glutPostRedisplay();
}

//--+----4----+----3----+----2----+----1----+----|----+----1----+----2----+----3----+----4----+----5
//------------------------------------------   Mouse   ---------------------------------------------

void Mouse (int button, int state, int x, int y)
{
    x_click = x;
    y_click = y;

    new_mouse = 1;
}

//--+----4----+----3----+----2----+----1----+----|----+----1----+----2----+----3----+----4----+----5
//------------------------------------------   Motion   --------------------------------------------

void Motion (int x, int y)
{
    float turn_del, tipp_del, x_del, y_del;

    static int c = 0, x_sav, y_sav;
    static float turn_save = 0.0, tipp_save = 0.0;

    // printf ("   Motion:  %5d ->   x = %5d, y = %5d.", ++c, x, y);

    if (new_mouse)  {
       new_mouse = 0;
       x_sav = x_click;
       y_sav = y_click;
    }

    if (mouse_rot)  {

       turn_del = x - x_sav;
       turn += turn_del;

       tipp_del = y - y_sav;
       tipp += tipp_del;

    }  else if (mouse_trn)  {

       x_del = float(x - x_sav) * 0.01;
       xtm  += x_del;

       y_del = float(y - y_sav) * 0.01;
       ytm  -= y_del;

    }

    x_sav = x;
    y_sav = y;

    glutPostRedisplay();
}

//---9----8----7----6----5----4----3----2----1---++---1----2----3----4----5----6----7----8----9---\\
//---------------------------------------   Init_Lights   ------------------------------------------

void Init_Lights (void)
{
    glLightfv (GL_LIGHT1, GL_SPECULAR, lit1_spc);
    glLightfv (GL_LIGHT1, GL_DIFFUSE , lit1_dfs);
    glLightfv (GL_LIGHT1, GL_AMBIENT , lit1_amb);
    glLightfv (GL_LIGHT1, GL_POSITION, lit1_pos);

    glLightfv (GL_LIGHT2, GL_SPECULAR, lit2_spc);
    glLightfv (GL_LIGHT2, GL_DIFFUSE , lit2_dfs);
    glLightfv (GL_LIGHT2, GL_AMBIENT , lit2_amb);
    glLightfv (GL_LIGHT2, GL_POSITION, lit2_pos);

    glEnable (GL_LIGHT1);
    glEnable (GL_LIGHT2);

    glEnable (GL_AUTO_NORMAL);
    glEnable (GL_NORMALIZE  );
}

void draw_moeda()
{
    float radius = 20, halfLength = 2;
    int slices = 10000;
    for(int i=0; i<slices; i++) {
        float theta = ((float)i)*2.0*3.14;
        float nextTheta = ((float)i+1)*2.0*3.14;
        glBegin(GL_TRIANGLE_STRIP);
            glVertex3f(0.0, halfLength, 0.0);
            glVertex3f(radius*cos(theta), halfLength, radius*sin(theta));
            glVertex3f (radius*cos(nextTheta), halfLength, radius*sin(nextTheta));
            glVertex3f (radius*cos(nextTheta), -halfLength, radius*sin(nextTheta));
            glVertex3f(radius*cos(theta), -halfLength, radius*sin(theta));
            glVertex3f(0.0, -halfLength, 0.0);
        glEnd();
    }
}

void draw_moeda_1()
{
    float radius = 20, halfLength = 2;
    int slices = 10000;
    for(int i=0; i<slices; i++) {
        float theta = ((float)i)*2.0*3.14;
        float nextTheta = ((float)i+1)*2.0*3.14;
        glBegin(GL_TRIANGLE_STRIP);
            glVertex3f(0.0, halfLength, 0.0);
            glVertex3f(radius*cos(theta), halfLength, radius*sin(theta));
            glVertex3f (radius*cos(nextTheta), halfLength, radius*sin(nextTheta));
            glVertex3f (radius*cos(nextTheta), -halfLength, radius*sin(nextTheta));
            glVertex3f(radius*cos(theta), -halfLength, radius*sin(theta));
            glVertex3f(0.0, -halfLength, 0.0);
        glEnd();
    }
}

//----------------------------------------   Draw_Road   -------------------------------------------

void Draw_Road (void)
{
    //glColor3f (0.30,0.25,0.30);
    glEnable (GL_LIGHTING);

    glPushMatrix();
       glTranslatef (0.0, -25.0, 0.0);
       glScalef     (10.0, 0.3,3.0);
       glutSolidCube (-20.0f);
    glPopMatrix();
}

//---------------------------------------   Render_Wheel   -----------------------------------------

void Render_Wheel (int xt, int yt, int zt)
{
    glPushMatrix();
       glTranslatef   (xt, yt, zt);
       glutSolidTorus (2,5,12,36);
    glPopMatrix();
}

//-----------------------------------------   Draw_Car   -------------------------------------------
void Draw_Car (void)
{
    glDisable (GL_LIGHTING);

    glScalef(0.05, 0.05, 0.05);
    glTranslatef(0, 250, 0);
    glRotatef(270,0,1,0);

     glBegin (GL_QUADS);
      // glColor3f (0.3, 0.6, 0.6);
       Vrt(-40,  0, 15); Vrt( 40,  0, 15); Vrt( 40,-15, 15); Vrt(-40,-15, 15);  // left
       Vrt(-40,  0,-15); Vrt( 40,  0,-15); Vrt( 40,-15,-15); Vrt(-40,-15,-15);  // right
       //glColor3f (0.4, 0.6, 0.6);
       Vrt( 40,  0,-15); Vrt( 40,  0, 15); Vrt( 40,-15, 15); Vrt( 40,-15,-15);  // back
       Vrt(-40,  0,-15); Vrt(-40,  0, 15); Vrt(-40,-15, 15); Vrt(-40,-15,-15);  // front
      // glColor3f (0.4, 0.6, 0.8);
       Vrt(-40,  0, 15); Vrt(-40,  0,-15); Vrt( 40,  0,-15); Vrt( 40,  0, 15);  // trunks
       Vrt(-10, 10, 15); Vrt(-10, 10,-15); Vrt( 20, 10,-15); Vrt( 20, 10, 15);  // roof
      // glColor3f (0.7, 0.7, 0.5);
       Vrt(-20,  0, 15); Vrt(-10, 10, 15); Vrt( 20, 10, 15); Vrt( 25,  0, 15);  // window l
       Vrt(-20,  0,-15); Vrt(-10, 10,-15); Vrt( 20, 10,-15); Vrt( 25,  0,-15);  // window r
      // glColor3f (0.7, 0.6, 0.5);
       Vrt(-10, 10, 15); Vrt(-20,  0, 15); Vrt(-20,  0,-15); Vrt(-10, 10,-15);  // windshield
       Vrt( 20, 10, 15); Vrt( 20, 10,-15); Vrt( 25,  0,-15); Vrt( 25,  0, 15);  // rear window
       //glColor3f (0.4, 0.5, 0.5);
       Vrt(-40,-15, 15); Vrt(-40,-15,-15); Vrt( 40,-15,-15); Vrt( 40,-15, 15);  // bottom
    glEnd ();


    // Render wheels using torus's with lighting on.

     glEnable     (GL_LIGHTING);
     //glColor3f    (0.3, 0.3, 0.3);
     Render_Wheel (-20,-15, 15);
     Render_Wheel ( 20,-15, 15);
     Render_Wheel (-20,-15,-15);
     Render_Wheel ( 20,-15,-15);
}

void Draw_pista (void)
{
    glDisable (GL_LIGHTING);

    int cont = 0;
    for(int i = 0; i < faces[0].size(); i+=3){
    //if(nomesTexturas[i] != "TreeTri_Leaf" && nomesTexturas[i] != "TreeTri_Bark"){
        glBindTexture(GL_TEXTURE_2D, textureID[nomeIdMtl[nomesTexturas[i]]]);
        /*if(nomesTexturas[i] == "depthshadow") {
            cout << "amada" << endl;
            cont++;
        }*/
        glBegin(GL_TRIANGLES);
          glTexCoord2f(texturas[0][faces[1][i]], texturas[1][faces[1][i]]);
          glVertex3f(vertices[0][faces[0][i]], vertices[1][faces[0][i]], vertices[2][faces[0][i]]);

          glTexCoord2f(texturas[0][faces[1][i+1]], texturas[1][faces[1][i+1]]);
          glVertex3f(vertices[0][faces[0][i+1]], vertices[1][faces[0][i+1]], vertices[2][faces[0][i+1]]);

          glTexCoord2f(texturas[0][faces[1][i+2]], texturas[1][faces[1][i+2]]);
          glVertex3f(vertices[0][faces[0][i+2]], vertices[1][faces[0][i+2]], vertices[2][faces[0][i+2]]);
        glEnd();
    //}
  }

    // Render wheels using torus's with lighting on.

     glEnable     (GL_LIGHTING);
}

//------------------------------------------   Poster's Display   ----------------------------------

void Display (void)
{
    static float x = 2.0;

    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    glMatrixMode   (GL_PROJECTION);
    glLoadIdentity ();

    glOrtho(-450.0, 450.0, -450.0, 450.0, -450.0, 450.0);

    glMatrixMode   (GL_MODELVIEW);
    glLoadIdentity ();

    glTranslatef (xtm, ytm, -4.0);       // Place objects between near and far clipping planes.
    glRotatef    (tipp,  1, 0, 0);       // Rotate around screen horizontal axis.
    glRotatef    (turn,  0, 1, 0);       // Rotate around world    vertical axis.
    glLightfv (GL_LIGHT1, GL_POSITION, lit1_pos);
    glLightfv (GL_LIGHT2, GL_POSITION, lit2_pos);
    glScalef     (scl, scl, scl );       // Scale world relative to its origin.
    glScalef     (0.02, 0.02, 0.02);

    //Draw_Road ();
    glTranslatef (-xcr, 0.0, 0.0);
    //Draw_Car  ();
    Draw_pista();
    Draw_Car  ();

    glTranslatef(-150, 10, 0);
    glRotatef(90,0,0,1);
    draw_moeda();
    //glTranslatef(150, -10, 0);

    glTranslatef(0, 100, 0);
    draw_moeda();

    glTranslatef(0, 100, 0);
    draw_moeda();

    glTranslatef(0, 300, -50);
    draw_moeda();

    glTranslatef(0, 100, 0);
    draw_moeda();

    glTranslatef(0, 100, 20);
    draw_moeda();

    glTranslatef(0, 300, 1000);
    glRotatef(90,0,0,1);
    draw_moeda();

    glTranslatef(-20, 0, 100);
    draw_moeda();

    glTranslatef(-20, 0, 100);
    draw_moeda();



    glutSwapBuffers();
}

//-----------------------------------------   Init_GL   --------------------------------------------

void Init_GL (void)
{
    Init_Lights ();

    glEnable (GL_DEPTH_TEST);
    glEnable (GL_LIGHTING  );
    glEnable (GL_COLOR_MATERIAL);

    glColorMaterial (GL_AMBIENT_AND_DIFFUSE, GL_FRONT);
    glColorMaterial (GL_SPECULAR           , GL_FRONT);

    glClearColor (0.2, 0.3, 0.3, 1.0);

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
}

//----------------------------------------   Init_Glut   -------------------------------------------

int Init_Glut (void)
{
    glutInitDisplayMode    (GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition (800, 200);
    glutInitWindowSize     (800, 800);
    glutCreateWindow       ("Carrinho");

    glutKeyboardFunc (Keybord);
    glutDisplayFunc  (Display);
    glutMouseFunc    (Mouse  );
    glutMotionFunc   (Motion );
}

//------------------------------------------   main   ----------------------------------------------

int main (int argc, char **argv)
{
    carrega_pista();
   // cout << nomesTexturas.size() << endl;
   // cout << faces[0].size() << endl;

    glutInit (&argc, argv);

    Init_Glut ();
    Init_GL   ();

    glutMainLoop ();

    return (1);
}
