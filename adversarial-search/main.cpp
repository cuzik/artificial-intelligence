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
#include <vector>
#include <cmath>

int TAM_CELULA = 50;
int TAM_BORDA = 15;
int x_Dimension = 3;
int y_Dimension = 3;

int LARGURA_TELA = x_Dimension * TAM_CELULA + 2 * TAM_BORDA;
int ALTURA_TELA = y_Dimension * TAM_CELULA + 2 * TAM_BORDA;

ALLEGRO_DISPLAY *janela = NULL;
ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
ALLEGRO_FONT *fonte = NULL;

bool inicializar();
void draw_tab();
void read_keyboard();
bool marca_player(int pos_x, int pos_y);
void troca_jogador();
int verifica_vencedor();

int tabuleiro[3][3] = {{0,0,0},{0,0,0},{0,0,0}};;

int x=0;
int y=0;

bool run = true;

int player = -1;

int jogadas = 0;

int main(int argc, char const *argv[]){
    srand( (unsigned)time(NULL) );
    inicializar();
    while(run){
        read_keyboard();
        al_clear_to_color(al_map_rgb(0, 0, 0));
        if(player == -1){
            al_draw_textf(fonte, al_map_rgb(255, 255, 0), 10 , 0, 0, "jogadas: %i \tPlayer",jogadas);
        }else{
            al_draw_textf(fonte, al_map_rgb(255, 255, 0), 10 , 0, 0, "jogadas: %i \tComputer",jogadas);
        }
        draw_tab();
        // usleep(3000000);
        al_flip_display();
    }
    return 0;
}


void read_keyboard(){
    while(!al_is_event_queue_empty(fila_eventos)){
        ALLEGRO_EVENT evento;
        al_wait_for_event(fila_eventos, &evento);
        if(evento.type == ALLEGRO_EVENT_KEY_DOWN){
            if(evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE){
                run = false;
            }else if(evento.keyboard.keycode == ALLEGRO_KEY_UP){
                if(x>0){
                    x--;
                }
            }else if(evento.keyboard.keycode == ALLEGRO_KEY_RIGHT){
                if(y<=3){
                    y++;
                }
            }else if(evento.keyboard.keycode == ALLEGRO_KEY_DOWN){
                if(x<=3){
                    x++;
                }
            }else if(evento.keyboard.keycode == ALLEGRO_KEY_LEFT){
                if(y>0){
                    y--;
                }
            }else if(evento.keyboard.keycode == ALLEGRO_KEY_ENTER){
                if(marca_player(x,y)){
                    jogadas++;
                    troca_jogador();
                    if(verifica_vencedor()!=0){
                        if(verifica_vencedor()==-1)
                            std::cout << "Player" << std::endl;
                        else
                            std::cout << "Computer" << std::endl;
                    }
                }
            }
        }else if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
            run = false;
        }
    }
}

void troca_jogador(){
    player = -player;
}

bool marca_player(int pos_x, int pos_y){
    if(tabuleiro[pos_x][pos_y]!=0)
        return false;
    tabuleiro[pos_x][pos_y] = player;
    return true;
} 

void draw_tab(){
    for(int i=0;i<x_Dimension;i++){
        for(int j=0;j<y_Dimension;j++){
            if(i==x && j==y){
                al_draw_filled_rectangle(TAM_BORDA+(j*TAM_CELULA), TAM_BORDA+(i*TAM_CELULA),TAM_BORDA+((j+1)*TAM_CELULA), TAM_BORDA+((i+1)*TAM_CELULA), al_map_rgb(0, 0, 255));
                // al_draw_filled_circle(TAM_BORDA+(i*TAM_CELULA)+(TAM_CELULA/2), TAM_BORDA+(j*TAM_CELULA)+(TAM_CELULA/2), (TAM_CELULA/2), al_map_rgb(color[matrix[i][j]][0], color[matrix[i][j]][1], color[matrix[i][j]][2]));
            }
            if(tabuleiro[i][j]==1){
                al_draw_circle(TAM_BORDA+(j*TAM_CELULA)+(TAM_CELULA/2), TAM_BORDA+(i*TAM_CELULA)+(TAM_CELULA/2), (TAM_CELULA/2), al_map_rgb(0, 255, 0), 2.0);
            }else if(tabuleiro[i][j]==-1){
                al_draw_circle(TAM_BORDA+(j*TAM_CELULA)+(TAM_CELULA/2), TAM_BORDA+(i*TAM_CELULA)+(TAM_CELULA/2), (TAM_CELULA/2), al_map_rgb(255, 0, 0), 2.0);
            }
        }
    }
}

int verifica_vencedor(){
    for(int i=0;i<3;i++){
        if((tabuleiro[i][0] + tabuleiro[i][1] + tabuleiro[i][2]) == 3
        || (tabuleiro[0][i] + tabuleiro[1][i] + tabuleiro[2][i]) == 3){
            return 1;
        }
    }
    if((tabuleiro[0][0] + tabuleiro[1][1] + tabuleiro[2][2]) == 3
    || (tabuleiro[2][0] + tabuleiro[1][1] + tabuleiro[0][2]) == 3){
        return 1;
    }
    for(int i=0;i<3;i++){
        if((tabuleiro[i][0] + tabuleiro[i][1] + tabuleiro[i][2]) == -3
        || (tabuleiro[0][i] + tabuleiro[1][i] + tabuleiro[2][i]) == -3){
            return -1;
        }
    }
    if((tabuleiro[0][0] + tabuleiro[1][1] + tabuleiro[2][2]) == -3
    || (tabuleiro[2][0] + tabuleiro[1][1] + tabuleiro[0][2]) == -3){
        return -1;
    }
    return 0;
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
