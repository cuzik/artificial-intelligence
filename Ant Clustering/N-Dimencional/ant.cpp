#include <iostream>
#include "ant.hpp"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <pthread.h>
#include <math.h>


// double alpha = 35;
// double k1 = 0.1;
// double k2 = 0.3;

double alpha = 1.6;
double k1 = 0.4;
double k2 = 0.8;

// double alpha = 11;
// double k1 = 0.2;
// double k2 = 0.4;

using namespace std;

double pp(double f){
	return pow((k1/(k1+f)),2);
}

double pd(double f){
	return pow((f/(k2+f)),2);
}

double distancia_enclidiana(Object a, Object b){
	double ret = 0;
	for(int i=0;i<b.N;i++){
		ret += pow((a.data[i] - b.data[i]),2);
	}
	// cout << sqrt(ret) << endl;
	return sqrt(ret);
}

double funcao_similaridade(Object data, Object **matrix, int x_Dimension, int y_Dimension, int vision, int x, int y){
	double soma = 0;
	double dist = 0;
	int x_aux;
	int y_aux;
	for(int i=x-vision;i<=x+vision;i++){
		for(int j=y-vision;j<=y+vision;j++){
			x_aux = (i+x_Dimension)%x_Dimension;
			y_aux = (j+y_Dimension)%y_Dimension;
			if(i!=x || j!=y){
				if(matrix[x_aux][y_aux].type!=0){
					dist = distancia_enclidiana(matrix[x_aux][y_aux], data);
					soma += (1.0 - (dist / alpha));
				}
			}
		}
	}
	double ret = (1.0/pow(vision,2)) * soma;
	if(ret < 0){
		ret = 0;
	}
	return ret;
}

ant::ant(){
	this->carregada = false;
}

void ant::set_info_screen(int N, Object **matrix,int x_Dimension,int y_Dimension){
	this->data_up.N = N;
	this->matrix = matrix;
	this->x_Dimension = x_Dimension;
	this->y_Dimension = y_Dimension;
}

void ant::setVision(int vision){
	if(vision>=1){
		this->vision = vision;
	}
}

void ant::init_position(int x, int y, int vision){
	this->x = x;
	this->y = y;
	this->last_x = x;
	this->last_y = y;
	this->data_up.type = 0;
	this->vision = vision;
}

void ant::draw(int TAM_BORDA,int TAM_CELULA){
	if(this->carregada){
	    // al_draw_filled_rectangle(TAM_BORDA+(this->x*TAM_CELULA), TAM_BORDA+(this->y*TAM_CELULA),TAM_BORDA+((this->x+1)*TAM_CELULA), TAM_BORDA+((this->y+1)*TAM_CELULA), al_map_rgb(0, 255, 0));
	    al_draw_filled_circle(TAM_BORDA+(this->x*TAM_CELULA)+(TAM_CELULA/2), TAM_BORDA+(this->y*TAM_CELULA)+(TAM_CELULA/2), (TAM_CELULA/2), al_map_rgb(0, 255, 0));
	}else{
	    // al_draw_filled_rectangle(TAM_BORDA+(this->x*TAM_CELULA), TAM_BORDA+(this->y*TAM_CELULA),TAM_BORDA+((this->x+1)*TAM_CELULA), TAM_BORDA+((this->y+1)*TAM_CELULA), al_map_rgb(255, 0, 0));
	    al_draw_filled_circle(TAM_BORDA+(this->x*TAM_CELULA)+(TAM_CELULA/2), TAM_BORDA+(this->y*TAM_CELULA)+(TAM_CELULA/2), (TAM_CELULA/2), al_map_rgb(255, 0, 0));
	}
}

void ant::move(){
	int x_aux;
	int y_aux;
	do{
		x_aux = (this->x + ((rand() % 3)-1)+x_Dimension)%x_Dimension;
		y_aux = (this->y + ((rand() % 3)-1)+y_Dimension)%y_Dimension;
	}while(x_aux == this->last_x && y_aux == this->last_y);
	this->last_x = this->x;
	this->last_y = this->y;
	this->x = x_aux;
	this->y = y_aux;
}

pthread_mutex_t mtx;

void ant::pick_up(){
	int aux;
	if(!this->carregada && matrix[this->x][this->y].type!=0){
		double f = funcao_similaridade(matrix[this->x][this->y],this->matrix,this->x_Dimension,this->y_Dimension,this->vision,this->x,this->y);
    	double probabilidade = pp(f) * 100;
    	aux = rand() % 100;
		pthread_mutex_lock(&mtx);
    	if(aux < probabilidade && matrix[this->x][this->y].type!=0){
    		this->carregada = true;
    		this->data_up.type = matrix[this->x][this->y].type;
    		matrix[this->x][this->y].type = 0;
    		this->data_up.data = matrix[this->x][this->y].data;
    	}
    	pthread_mutex_unlock(&mtx);
	}
}

void ant::drop(){
	int aux;
	if(this->carregada && matrix[this->x][this->y].type==0){
		double f = funcao_similaridade(this->data_up,this->matrix,this->x_Dimension,this->y_Dimension,this->vision,this->x,this->y);
    	double probabilidade = pd(f) * 100;
    	aux = rand() % 100;
    	// cout << probabilidade << endl;
		pthread_mutex_lock(&mtx);
    	if(aux < probabilidade && matrix[this->x][this->y].type==0){
    		this->carregada = false;
    		matrix[this->x][this->y].type = this->data_up.type;
    		this->data_up.type = 0;
    		matrix[this->x][this->y].data = this->data_up.data;
    	}
    	pthread_mutex_unlock(&mtx);
	}
}
