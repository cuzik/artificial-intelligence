#include <iostream>
#include "ant.hpp"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

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
	    al_draw_filled_rectangle(TAM_BORDA+(this->x*TAM_CELULA), TAM_BORDA+(this->y*TAM_CELULA),TAM_BORDA+((this->x+1)*TAM_CELULA), TAM_BORDA+((this->y+1)*TAM_CELULA), al_map_rgb(0, 255, 0));
	    // al_draw_filled_circle(TAM_BORDA+(this->x*TAM_CELULA)+(TAM_CELULA/2), TAM_BORDA+(this->y*TAM_CELULA)+(TAM_CELULA/2), (TAM_CELULA/2), al_map_rgb(0, 255, 0));
	}else{
	    al_draw_filled_rectangle(TAM_BORDA+(this->x*TAM_CELULA), TAM_BORDA+(this->y*TAM_CELULA),TAM_BORDA+((this->x+1)*TAM_CELULA), TAM_BORDA+((this->y+1)*TAM_CELULA), al_map_rgb(255, 0, 0));
	    // al_draw_filled_circle(TAM_BORDA+(this->x*TAM_CELULA)+(TAM_CELULA/2), TAM_BORDA+(this->y*TAM_CELULA)+(TAM_CELULA/2), (TAM_CELULA/2), al_map_rgb(255, 0, 0));
	}
}

/*
	0 1 2
	3 x 4
	5 6 7
*/

void ant::move(){
    int x_aux = this->x;
    int y_aux = this->y;
    int direction;
    do{
    	x_aux = this->x;
		y_aux = this->y;
    	direction = rand() % 8;
	    switch(direction){
	    	case 0:
	    		x_aux--;
	    		y_aux--;
	    		break;
	    	case 1:
	    		// x_aux;
	    		y_aux--;
	    		break;
	    	case 2:
	    		x_aux++;
	    		y_aux--;
	    		break;
	    	case 3:
	    		x_aux--;
	    		// y_aux;
	    		break;
	    	case 4:
	    		x_aux++;
	    		// y_aux;
	    		break;
	    	case 5:
	    		x_aux--;
	    		y_aux++;
	    		break;
	    	case 6:
	    		// x_aux;
	    		y_aux++;
	    		break;
	    	case 7:
	    		x_aux++;
	    		y_aux++;
	    		break;
	    }
	}while(!(x_aux<x_Dimension && y_aux<y_Dimension && x_aux > -1 && y_aux > -1) || (x_aux == this->last_x && y_aux == this->last_y));
	this->last_x = this->x;
	this->last_y = this->y;
	this->x = x_aux;
 	this->y = y_aux;
}

int abss(int a){
	if(a < 0){
		return -a;
	}
	return a;
}

void ant::move_atraction(){
    int x_aux = 0;
    int y_aux = 0;
    int i,j;
    
    for(i=-this->vision;i<=this->vision;i++){
		for(j=-this->vision;j<=this->vision;j++){
			if(this->x+i<x_Dimension && this->y+j<y_Dimension && this->x+i > -1 && this->y+j > -1){
				if(matrix[this->x+i][this->y+j]!=0 && (i!=0 || j!=0)){
					x_aux += i;
					y_aux += j;
				}
			}
		}
	}
	// cout << "[" << x_aux << "," << y_aux << "]";
	if(!this->carregada){
		x_aux = -x_aux;
		y_aux = -y_aux;
	}
	/*colocando o x*/
	if(x_aux<0){
		x_aux = this->x - 1;
	}else if(x_aux>0){
		x_aux = this->x + 1;
	}else{
		x_aux = this->x;
	}

	/*colocando o y*/
	if(y_aux<0){
		y_aux = this->y - 1;
	}else if(y_aux>0){
		y_aux = this->y + 1;
	}else{
		y_aux = this->y;
	}

	// cout << "   ==    [" << x_aux - this->x << "," << y_aux - this->y << "]" << endl;
	
	if(!(x_aux<x_Dimension && y_aux<y_Dimension && x_aux > -1 && y_aux > -1) || (x_aux == this->last_x && y_aux == this->last_y)){
		this->move();
	}else{
		this->last_x = this->x;
		this->last_y = this->y;
		this->x = x_aux;
	 	this->y = y_aux;
	}
	
}


void ant::pick_up(){
	int i,j;
	int aux;
	int frame_void = 0;
	if(!this->carregada && matrix[this->x][this->y]!=0){
		for(i=this->x-this->vision;i<=this->x+this->vision;i++){
			for(j=this->y-this->vision;j<=this->y+this->vision;j++){
				if(i<x_Dimension && j<y_Dimension && i > -1 && j > -1){
					if(matrix[i][j]==matrix[this->x][this->y] && (i!=this->x || j!=this->y)){
						frame_void++;
					}
				}
			}
		}
		frame_void = total_frame - frame_void;
    	aux = rand() % total_frame;
    	if(aux < frame_void){
    		this->carregada = true;
    		this->type_data = matrix[this->x][this->y];
    		matrix[this->x][this->y] = 0;
    	}
	}
}

void ant::drop(){
	int i,j;
	int aux;
	int frame_void = 0;
	if(this->carregada && matrix[this->x][this->y]==0){
		for(i=this->x-this->vision;i<=this->x+this->vision;i++){
			for(j=this->y-this->vision;j<=this->y+this->vision;j++){
				if(i<x_Dimension && j<y_Dimension && i > -1 && j > -1){
					if(matrix[i][j]==this->type_data && (i!=this->x || j!=this->y)){
						frame_void++;
					}
				}
			}
		}
		frame_void = total_frame - frame_void;
    	aux = rand() % total_frame;
    	if(aux >= frame_void){
    		this->carregada = false;
    		matrix[this->x][this->y] = this->type_data;
    		this->type_data = 0;
    	}
	}
}
