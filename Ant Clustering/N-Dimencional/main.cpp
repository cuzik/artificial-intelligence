#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <pthread.h>
 
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "ant.hpp"

int TAM_CELULA = 5;
int TAM_BORDA = 15;
int ant_alive = 25;
int ant_vision = 5;
int x_Dimension = 100;
int y_Dimension = 100;
int num_limit_steps = -1;

int N = 2;

int LARGURA_TELA = x_Dimension * TAM_CELULA + 2 * TAM_BORDA;
int ALTURA_TELA = y_Dimension * TAM_CELULA + 2 * TAM_BORDA; 

ALLEGRO_DISPLAY *janela = NULL;
ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
ALLEGRO_FONT *fonte = NULL;

double tempoInicial = 0;

void *ant_animated(void *arg);
void desenha_formiga(int x, int y);
void iniciarTimer();
double obterTempoTimer();
bool inicializar();
void position_dead_ants();
void matrix_draw();
ant *position_alive_ants();
void color_generate();
void init_ant_colony();
void read_keyboard();
void init_ants();
void draw_all_screen();

ant *ants = NULL;
Object **matrix;
bool run = true;
bool sair = false;

typedef struct color_struct{
    int r,g,b;
}Color;

Color *color;

int main(int argc, char const *argv[]){
    srand( (unsigned)time(NULL) );
    color_generate();
    inicializar();
    init_ant_colony();
    
    position_dead_ants();

    ants = position_alive_ants();

    init_ants();

    while(!sair){
        read_keyboard();
        draw_all_screen();
    }
    // draw_all_screen();
    // usleep(30000);
    al_destroy_event_queue(fila_eventos);
    al_destroy_display(janela);
    return 0;
}

void draw_all_screen(){
    al_clear_to_color(al_map_rgb(0, 0, 0));
    matrix_draw();
    for(int i=0;i<ant_alive;i++){
        ants[i].draw(TAM_BORDA,TAM_CELULA);
    }
    al_draw_textf(fonte, al_map_rgb(255, 255, 0), 10 , 0, 0, "Range:%i",ants[0].vision);
    al_flip_display();
}

void read_keyboard(){
    while(!al_is_event_queue_empty(fila_eventos)){
        ALLEGRO_EVENT evento;
        al_wait_for_event(fila_eventos, &evento);
        if(evento.type == ALLEGRO_EVENT_KEY_DOWN){
            if(evento.keyboard.keycode == ALLEGRO_KEY_DOWN){
                for(int i=0;i<ant_alive;i++){
                    ants[i].setVision(ants[i].vision-1);
                }
            }else if(evento.keyboard.keycode == ALLEGRO_KEY_UP){
                for(int i=0;i<ant_alive;i++){
                    ants[i].setVision(ants[i].vision+1);
                }
            }else if(evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE){
                run = false;
                sair = true;
            }
        }else if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
                run = false;
            sair = true;
        }
    }
}

void init_ants(){
    pthread_t cons[ant_alive];
    for(int i=0;i<ant_alive;i++){
        pthread_create(&cons[i], NULL, ant_animated,(void *)i);
    }
}

void color_generate(){
    color = (Color *)malloc(sizeof(Color)*20);
    for(int i=0;i<20;i++){
        color[i].r = rand() % 256;
        color[i].g = rand() % 256;
        color[i].b = rand() % 256;
    }
}

void init_ant_colony(){
    matrix = (Object **)malloc(sizeof(Object*)*x_Dimension);
    for(int i=0;i<x_Dimension;i++){
        matrix[i] = (Object *)malloc(sizeof(Object)*y_Dimension);
    }
    for(int i=0;i<x_Dimension;i++){
        for(int j=0;j<y_Dimension;j++){
            matrix[i][j].type = 0;
        }
    }
}

ant *position_alive_ants(){
    ant *ants_aux = new ant[ant_alive];
    int i;
    int x,y;
    for(i=0;i<ant_alive;i++){
        x = rand() % x_Dimension;
        y = rand() % y_Dimension;
        if(matrix[x][y].type != 0){
            i--;
        }else{
            ants_aux[i].init_position(x,y,ant_vision);
            ants_aux[i].set_info_screen(N,matrix,x_Dimension,y_Dimension);
        }
    }
    return ants_aux;
}

void position_dead_ants(){
    int x,y;
    int ant_dead;
    std::cin >> ant_dead;
    std::cin >> N;
    for(int i=0;i<x_Dimension;i++){
        for(int j=0;j<y_Dimension;j++){
            matrix[i][j].data = (double *)malloc(sizeof(double)*N);
            matrix[i][j].N = N;
        }
    }
    for(int i=0;i<ant_dead;i++){
        x = rand() % x_Dimension;
        y = rand() % y_Dimension;
        if(matrix[x][y].type != 0){
            i--;
        }else{
            for(int j=0;j<N;j++){
               std::cin >> matrix[x][y].data[j];
            }
            std::cin >> matrix[x][y].type;
        }
    }
}

void matrix_draw(){
    int i,j;
    for(i=0;i<x_Dimension;i++){
        for(j=0;j<y_Dimension;j++){
            if(matrix[i][j].type){
                al_draw_filled_rectangle(TAM_BORDA+(i*TAM_CELULA), TAM_BORDA+(j*TAM_CELULA),TAM_BORDA+((i+1)*TAM_CELULA), TAM_BORDA+((j+1)*TAM_CELULA), al_map_rgb(color[matrix[i][j].type].r, color[matrix[i][j].type].g, color[matrix[i][j].type].b));
                // al_draw_filled_circle(TAM_BORDA+(i*TAM_CELULA)+(TAM_CELULA/2), TAM_BORDA+(j*TAM_CELULA)+(TAM_CELULA/2), (TAM_CELULA/2), al_map_rgb(color[matrix[i][j]].r, color[matrix[i][j]].g, color[matrix[i][j]].b));
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

void *ant_animated(void *arg){
    int id = (int&) arg;
    int steps = 0;
    printf("Ant number %2d is on.\n",id);
    while(run && (steps != num_limit_steps || ants[id].carregada)){
        steps++;
        ants[id].move();
        ants[id].pick_up();
        ants[id].drop();
    }
    printf("Ant number %2d is off in %d.\n",id,steps);
}