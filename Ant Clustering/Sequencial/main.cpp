#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
 
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "ant.hpp"

int TAM_CELULA = 1;
int TAM_BORDA = 15;
int ant_alive = 1000;
int ant_dead = 100000;
int ant_vision = 2;

int LARGURA_TELA = 640;
int ALTURA_TELA = 480;
 
int x_Dimension = 500;
int y_Dimension = 500;

ALLEGRO_DISPLAY *janela = NULL;
ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
ALLEGRO_FONT *fonte = NULL;

void ant_animated(int id);
void desenha_formiga(int x, int y);
bool inicializar();
void position_dead_ants(int **matrix, int type);
void matrix_draw(int **matrix);
ant *position_alive_ants(int **matrix);

ant *ants = NULL;

int main(int argc, char const *argv[]){
    int frame_show = 0;
    unsigned long long int frames = 0;
    int taxa_atualizacao_tela = 1000;
    srand( (unsigned)time(NULL) );
    int i,j;
    if(argc==2){
        ant_vision = atoi(argv[1]);
    }
    LARGURA_TELA = x_Dimension * TAM_CELULA + 2 * TAM_BORDA;
    ALTURA_TELA  = y_Dimension * TAM_CELULA + 2 * TAM_BORDA;

    bool sair = false;
 
    if(!inicializar()){
        return -1;
    }

    int **matrix = (int **)malloc(sizeof(int*)*x_Dimension);
    for(i=0;i<x_Dimension;i++){
        matrix[i] = (int *)malloc(sizeof(int)*y_Dimension);
    }
    for(i=0;i<x_Dimension;i++){
        for(j=0;j<y_Dimension;j++){
            matrix[i][j] = 0;
        }
    }
    position_dead_ants(matrix, 1);
    // position_dead_ants(matrix, 2);
    // position_dead_ants(matrix, 3);
    // position_dead_ants(matrix, 4);
    // position_dead_ants(matrix, 5);
    // position_dead_ants(matrix, 6);
    // position_dead_ants(matrix, 7);
    // position_dead_ants(matrix, 8);
    // ant *ants = position_alive_ants(matrix);
    ants = position_alive_ants(matrix);
    matrix_draw(matrix);
    for(i=0;i<ant_alive;i++){
        ants[i].draw(TAM_BORDA,TAM_CELULA);
    }
    al_flip_display();
    // usleep(3000000);

    while(!sair){
        /*le o input dp teclado*/
        while(!al_is_event_queue_empty(fila_eventos)){
            ALLEGRO_EVENT evento;
            al_wait_for_event(fila_eventos, &evento);
            if(evento.type == ALLEGRO_EVENT_KEY_DOWN){
                if(evento.keyboard.keycode == ALLEGRO_KEY_DOWN){
                    for(i=0;i<ant_alive;i++){
                        ants[i].setVision(ants[i].vision-1);
                    }
                }else if(evento.keyboard.keycode == ALLEGRO_KEY_UP){
                    for(i=0;i<ant_alive;i++){
                        ants[i].setVision(ants[i].vision+1);
                    }
                }else if(evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE){
                    sair = true;
                }
            }else if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
                sair = true;
            }
        }

        /*Apenas printado*/
        for(i=0;i<ant_alive;i++){
            ant_animated(i);
        }
        frame_show++;
        if(frame_show==taxa_atualizacao_tela){
            frame_show = 0;
            matrix_draw(matrix);
            for(i=0;i<ant_alive;i++){
                ants[i].draw(TAM_BORDA,TAM_CELULA);
            }
            frames++;
            al_draw_textf(fonte, al_map_rgb(255, 0, 255), 10 , 0, 0, "Passos:%lli visao:%i", frames*taxa_atualizacao_tela,ants[0].vision);
            al_flip_display();
            al_clear_to_color(al_map_rgb(0, 0, 30));
        }
    }
    /*destroi os objetos de ambiente*/
    al_destroy_event_queue(fila_eventos);
    al_destroy_display(janela);
    return 0;
}

ant *position_alive_ants(int **matrix){
    ant *ants_aux = new ant[ant_alive];
    int i;
    int x,y;
    for(i=0;i<ant_alive;i++){
        x = rand() % x_Dimension;
        y = rand() % y_Dimension;
        if(matrix[x][y] != 0){
            i--;
        }else{
            ants_aux[i].init_position(x,y,ant_vision);
            ants_aux[i].set_info_screen(matrix,x_Dimension,y_Dimension);
        }
    }
    return ants_aux;
}
void position_dead_ants(int **matrix, int type){
    int i;
    int x,y;
    for(i=0;i<ant_dead;i++){
        x = rand() % x_Dimension;
        y = rand() % y_Dimension;
        if(matrix[x][y] != 0){
            i--;
        }else{
            matrix[x][y] = type;
        }
    }
}

void matrix_draw(int **matrix){
    int i,j;
    for(i=0;i<x_Dimension;i++){
        for(j=0;j<y_Dimension;j++){
            switch(matrix[i][j]){
                case 1:
                    al_draw_filled_rectangle(TAM_BORDA+(i*TAM_CELULA), TAM_BORDA+(j*TAM_CELULA),TAM_BORDA+((i+1)*TAM_CELULA), TAM_BORDA+((j+1)*TAM_CELULA), al_map_rgb(255, 255, 0));
                    // al_draw_filled_circle(TAM_BORDA+(i*TAM_CELULA)+(TAM_CELULA/2), TAM_BORDA+(j*TAM_CELULA)+(TAM_CELULA/2), (TAM_CELULA/2), al_map_rgb(255, 255, 0));
                    break;
                case 2:
                    // al_draw_filled_rectangle(TAM_BORDA+(i*TAM_CELULA), TAM_BORDA+(j*TAM_CELULA),TAM_BORDA+((i+1)*TAM_CELULA), TAM_BORDA+((j+1)*TAM_CELULA), al_map_rgb(255, 0, 255));
                    al_draw_filled_circle(TAM_BORDA+(i*TAM_CELULA)+(TAM_CELULA/2), TAM_BORDA+(j*TAM_CELULA)+(TAM_CELULA/2), (TAM_CELULA/2), al_map_rgb(255, 0, 255));
                    break;
                case 3:
                    // al_draw_filled_rectangle(TAM_BORDA+(i*TAM_CELULA), TAM_BORDA+(j*TAM_CELULA),TAM_BORDA+((i+1)*TAM_CELULA), TAM_BORDA+((j+1)*TAM_CELULA), al_map_rgb(100, 100, 255));
                    al_draw_filled_circle(TAM_BORDA+(i*TAM_CELULA)+(TAM_CELULA/2), TAM_BORDA+(j*TAM_CELULA)+(TAM_CELULA/2), (TAM_CELULA/2), al_map_rgb(100, 100, 255));
                    break; 
                case 4:
                    // al_draw_filled_rectangle(TAM_BORDA+(i*TAM_CELULA), TAM_BORDA+(j*TAM_CELULA),TAM_BORDA+((i+1)*TAM_CELULA), TAM_BORDA+((j+1)*TAM_CELULA), al_map_rgb(255, 255, 255));
                    al_draw_filled_circle(TAM_BORDA+(i*TAM_CELULA)+(TAM_CELULA/2), TAM_BORDA+(j*TAM_CELULA)+(TAM_CELULA/2), (TAM_CELULA/2), al_map_rgb(255, 255, 255));
                    break; 
                case 5:
                    // al_draw_filled_rectangle(TAM_BORDA+(i*TAM_CELULA), TAM_BORDA+(j*TAM_CELULA),TAM_BORDA+((i+1)*TAM_CELULA), TAM_BORDA+((j+1)*TAM_CELULA), al_map_rgb(0, 0, 255));
                    al_draw_filled_circle(TAM_BORDA+(i*TAM_CELULA)+(TAM_CELULA/2), TAM_BORDA+(j*TAM_CELULA)+(TAM_CELULA/2), (TAM_CELULA/2), al_map_rgb(0, 0, 255));
                    break; 
                case 6:
                    // al_draw_filled_rectangle(TAM_BORDA+(i*TAM_CELULA), TAM_BORDA+(j*TAM_CELULA),TAM_BORDA+((i+1)*TAM_CELULA), TAM_BORDA+((j+1)*TAM_CELULA), al_map_rgb(155, 100, 100));
                    al_draw_filled_circle(TAM_BORDA+(i*TAM_CELULA)+(TAM_CELULA/2), TAM_BORDA+(j*TAM_CELULA)+(TAM_CELULA/2), (TAM_CELULA/2), al_map_rgb(155, 100, 100));
                    break;  
                case 7:
                    // al_draw_filled_rectangle(TAM_BORDA+(i*TAM_CELULA), TAM_BORDA+(j*TAM_CELULA),TAM_BORDA+((i+1)*TAM_CELULA), TAM_BORDA+((j+1)*TAM_CELULA), al_map_rgb(100, 0, 0));
                    al_draw_filled_circle(TAM_BORDA+(i*TAM_CELULA)+(TAM_CELULA/2), TAM_BORDA+(j*TAM_CELULA)+(TAM_CELULA/2), (TAM_CELULA/2), al_map_rgb(100, 0, 0));
                    break;   
                case 8:
                    // al_draw_filled_rectangle(TAM_BORDA+(i*TAM_CELULA), TAM_BORDA+(j*TAM_CELULA),TAM_BORDA+((i+1)*TAM_CELULA), TAM_BORDA+((j+1)*TAM_CELULA), al_map_rgb(0, 100, 0));
                    al_draw_filled_circle(TAM_BORDA+(i*TAM_CELULA)+(TAM_CELULA/2), TAM_BORDA+(j*TAM_CELULA)+(TAM_CELULA/2), (TAM_CELULA/2), al_map_rgb(0, 100, 0));
                    break;   
            }
        }
    }
}

bool inicializar(){
    if (!al_init()){
        fprintf(stderr, "Falha ao inicializar Allegro.\n");
        return false;
    }

    al_init_font_addon();
 
    if (!al_init_ttf_addon()){
        fprintf(stderr, "Falha ao inicializar allegro_ttf.\n");
        return false;
    }
 
    if (!al_init_primitives_addon()){
        fprintf(stderr, "Falha ao inicializar add-on allegro_primitives.\n");
        return false;
    }

    if (!al_install_keyboard()){
        fprintf(stderr, "Falha ao inicializar o teclado.\n");
        return false;
    }
 
    janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
    if (!janela){
        fprintf(stderr, "Falha ao criar janela.\n");
        return false;
    }
 
    al_set_window_title(janela, "Formigueiro");
 
    fila_eventos = al_create_event_queue();
    if (!fila_eventos){
        fprintf(stderr, "Falha ao criar fila de eventos.\n");
        al_destroy_display(janela);
        return false;
    }

    fonte = al_load_font("Font.ttf", 15, 0);
    if (!fonte)
    {
        fprintf(stderr, "Falha ao carregar fonte.\n");
        al_destroy_event_queue(fila_eventos);
        al_destroy_display(janela);
        return false;
    }
 
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    al_register_event_source(fila_eventos, al_get_display_event_source(janela));
 
    return true;
}

void ant_animated(int id){
    ants[id].move();
    ants[id].pick_up();
    ants[id].drop();
}