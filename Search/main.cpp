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

int TAM_CELULA = 15;
int TAM_BORDA = 15;
int x_Dimension = 42;
int y_Dimension = 42;

int LARGURA_TELA = x_Dimension * TAM_CELULA + 2 * TAM_BORDA;
int ALTURA_TELA = y_Dimension * TAM_CELULA + 2 * TAM_BORDA;

ALLEGRO_DISPLAY *janela = NULL;
ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
ALLEGRO_FONT *fonte = NULL;
int **matrix;

bool inicializar();
void matrix_draw();
void read_keyboard();
void draw_way(int way[][2]);
void read_map();

bool run = true;

int color[4][3] = {
                    {137, 168, 0  }, // grass
                    {91 , 42 , 8  }, //  mountain
                    {0  , 61 , 114}, // swamp
                    {226, 95 , 2  } // fire
                  };

int main(int argc, char const *argv[]){
    srand( (unsigned)time(NULL) );
    inicializar();
    read_map();
    int way[10][2] =  {
                        {1,1},
                        {1,2},
                        {1,3},
                        {1,4},
                        {2,4},
                        {2,5},
                        {2,6},
                        {3,6},
                        {4,6},
                        {5,6}
                      };
    while(run){
        read_keyboard();
        al_clear_to_color(al_map_rgb(0, 0, 0));
        matrix_draw();
        draw_way(way);
        al_draw_textf(fonte, al_map_rgb(255, 255, 0), 10 , 0, 0, "oiee");
        al_flip_display();
    }

    al_destroy_event_queue(fila_eventos);
    al_destroy_display(janela);
    return 0;
}

void read_map(){
    matrix = (int**) malloc (sizeof(int*)*x_Dimension);
    for(int i = 0; i<x_Dimension;i++){
        matrix[i] = (int*) malloc (sizeof(int)*y_Dimension);
    }
    FILE *in = fopen("mapa","r");
    for(int i = 0; i<x_Dimension; i++){
        for(int j = 0; j<y_Dimension; j++){
            matrix[i][j] = fgetc(in) - 48;
            fgetc(in);
        }
    }
}

void read_keyboard(){
    while(!al_is_event_queue_empty(fila_eventos)){
        ALLEGRO_EVENT evento;
        al_wait_for_event(fila_eventos, &evento);
        if(evento.type == ALLEGRO_EVENT_KEY_DOWN){
            if(evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE){
                run = false;
            }
        }else if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
            run = false;
        }
    }
}

void matrix_draw(){
    int i,j;
    for(i=0;i<x_Dimension;i++){
        for(j=0;j<y_Dimension;j++){
            al_draw_filled_rectangle(TAM_BORDA+(j*TAM_CELULA), TAM_BORDA+(i*TAM_CELULA),TAM_BORDA+((j+1)*TAM_CELULA), TAM_BORDA+((i+1)*TAM_CELULA), al_map_rgb(color[matrix[i][j]][0], color[matrix[i][j]][1], color[matrix[i][j]][2]));
            // al_draw_filled_circle(TAM_BORDA+(i*TAM_CELULA)+(TAM_CELULA/2), TAM_BORDA+(j*TAM_CELULA)+(TAM_CELULA/2), (TAM_CELULA/2), al_map_rgb(color[matrix[i][j]][0], color[matrix[i][j]][1], color[matrix[i][j]][2]));
        }
    }
}

void draw_way(int way[][2]){
    int i;
    for(i=0;i<10;i++){
        // al_draw_filled_rectangle(TAM_BORDA+(way[i][1]*TAM_CELULA), TAM_BORDA+(way[i][0]*TAM_CELULA),TAM_BORDA+((way[i][1]+1)*TAM_CELULA), TAM_BORDA+((way[i][0]+1)*TAM_CELULA), al_map_rgb(0, 0, 0));
        al_draw_filled_circle(TAM_BORDA+(way[i][0]*TAM_CELULA)+(TAM_CELULA/2), TAM_BORDA+(way[i][1]*TAM_CELULA)+(TAM_CELULA/2), (TAM_CELULA/2), al_map_rgb(160, 0, 128));
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
