#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <stdlib.h>

using namespace std;

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
			file >> linha;
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
