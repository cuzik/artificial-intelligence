#include <iostream>
#include "ant.hpp"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <pthread.h>

using namespace std;

ant::ant(){
	this->carregada = false;
}

void ant::set_info_screen(int **matrix,int x_Dimension,int y_Dimension){
	this->matrix = matrix;
	this->x_Dimension = x_Dimension;
	this->y_Dimension = y_Dimension;
}


void ant::setVision(int vision){
	if(vision>=1){
		this->vision = vision;
		this->total_frame = (((2*this->vision) + 1) * ((2*this->vision) + 1)) - 1;
	}
}

void ant::init_position(int x, int y, int vision){
	this->x = x;
	this->last_x = x;
	this->y = y;
	this->last_y = y;
	this->type_data = 0;
	this->vision = vision;
	this->total_frame = (((2*this->vision) + 1) * ((2*this->vision) + 1)) - 1;
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

/*
	0 1 2
	3 4 5
	6 7 8
*/

void ant::move(){
	int x_aux;
	int y_aux;
	do{
		x_aux = (this->x + ((rand() % 2)-1)+x_Dimension)%x_Dimension;
		y_aux = (this->y + ((rand() % 2)-1)+y_Dimension)%y_Dimension;
	}while(x_aux == this->last_x && y_aux == this->last_y);
	this->last_x = this->x;
	this->last_y = this->y;
	this->x = x_aux;
	this->y = y_aux;
}


pthread_mutex_t mtx;

void ant::pick_up(){
	int i,j;
	int aux_i;
	int aux_j;
	int aux;
	int frame_void = 0;
	if(!this->carregada && matrix[this->x][this->y]!=0){
		for(i=this->x-this->vision;i<=this->x+this->vision;i++){
			for(j=this->y-this->vision;j<=this->y+this->vision;j++){
				aux_i = i;
				aux_j = j;
				if(aux_i < 0){
					aux_i += x_Dimension;
				}
				if(aux_j < 0){
					aux_j += y_Dimension;
				}
				if(aux_i>=x_Dimension){
					aux_i -= x_Dimension;
				}
				if(aux_j>=y_Dimension){
					aux_j -= y_Dimension;
				}
				if(matrix[aux_i][aux_j]==matrix[this->x][this->y] && (aux_i!=this->x || aux_j!=this->y)){
					frame_void++;
				}
			}
		}
		frame_void = total_frame - frame_void;
    	aux = rand() % total_frame;
		pthread_mutex_lock(&mtx);
    	if(aux < frame_void && matrix[this->x][this->y]!=0){
    		this->carregada = true;
    		this->type_data = matrix[this->x][this->y];
    		matrix[this->x][this->y] = 0;
    	}
    	pthread_mutex_unlock(&mtx);
	}
}

void ant::drop(){
	int i,j;
	int aux_i;
	int aux_j;
	int aux;
	int frame_void = 0;
	if(this->carregada && matrix[this->x][this->y]==0){
		for(i=this->x-this->vision;i<=this->x+this->vision;i++){
			for(j=this->y-this->vision;j<=this->y+this->vision;j++){
				aux_i = i;
				aux_j = j;
				if(aux_i < 0){
					aux_i += x_Dimension;
				}
				if(aux_j < 0){
					aux_j += y_Dimension;
				}
				if(aux_i>=x_Dimension){
					aux_i -= x_Dimension;
				}
				if(aux_j>=y_Dimension){
					aux_j -= y_Dimension;
				}
				if(matrix[aux_i][aux_j]==this->type_data && (aux_i!=this->x || aux_j!=this->y)){
					frame_void++;
				}
			}
		}
		frame_void = total_frame - frame_void;
    	aux = rand() % total_frame;
		pthread_mutex_lock(&mtx);
    	if(aux >= frame_void && matrix[this->x][this->y]==0){
    		this->carregada = false;
    		matrix[this->x][this->y] = this->type_data;
    		this->type_data = 0;
    	}
    	pthread_mutex_unlock(&mtx);
	}
}
