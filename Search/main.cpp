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
int **matrix_total_weight;
std::vector< std::vector< std::pair<int, std::vector< std::pair< int,int > > > > > matrix_mirror;
// int **matrix_mirror;

bool inicializar();
void matrix_draw_loading();
void matrix_draw();
void read_keyboard();
void draw_way(std::pair<int,int> steep);
void read_map();
std::vector<std::pair<int,int>> bfs(std::pair<int,int> a, std::pair<int,int> b);
std::vector<std::pair<int,int>> djikstra(std::pair<int,int> a, std::pair<int,int> b);
std::vector<std::pair<int,int>> a_star(std::pair<int,int> a, std::pair<int,int> b);
std::vector<std::pair<int,int>> search_way_teste();


bool run = true;

int color[4][3] = {
                    {137, 168, 0  }, // grass
                    {91 , 42 , 8  }, // mountain
                    {0  , 61 , 114}, // swamp
                    {226, 95 , 2  }  // fire
                  };

int type_weight[4] = {1,5,10,15};

int main(int argc, char const *argv[]){
    srand( (unsigned)time(NULL) );
    inicializar();
    read_map();
    
    std::pair<int,int> a;
    a.first  = atoi(argv[1]);
    a.second = atoi(argv[2]);
    std::pair<int,int> b;
    b.first  = atoi(argv[3]);
    b.second = atoi(argv[4]);

    std::vector<std::pair<int,int>> way;
    if(atoi(argv[5]) == 1){
        way = bfs(a,b);
    }else if(atoi(argv[5]) == 2){
        way = djikstra(a,b);
    }else if(atoi(argv[5]) == 3){
        way = a_star(a,b);
    }else{
        way = djikstra(a,b);
    }
    int len = (int) way.size();
    int wei = matrix_total_weight[b.first][b.second];

    while(run){
        read_keyboard();
        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_textf(fonte, al_map_rgb(255, 255, 0), 10 , 0, 0, "steeps: %i -\t weight: %i",len,wei);
        matrix_draw();
        for(int i=0; i<(int) way.size();i++){
            draw_way(way[i]);
            al_flip_display();
            usleep(30000);
        }
        al_flip_display();
    }

    al_destroy_event_queue(fila_eventos);
    al_destroy_display(janela);
    return 0;
}

void print_stage(std::vector<std::pair<int,int>> to_do){
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_textf(fonte, al_map_rgb(255, 255, 0), 10 , 0, 0, "to dos: %i ", (int) to_do.size());
    matrix_draw_loading();
    for(int i=0; i<(int) to_do.size();i++){
        draw_way(to_do[i]);
    }
    al_flip_display();
}

std::vector<std::pair<int,int>> bfs(std::pair<int,int> a, std::pair<int,int> b){
    std::vector<std::pair<int,int>> to_do;
    to_do.push_back(a);
    matrix_mirror[a.first][a.second].first = -1;
    while(true){
        std::vector<std::pair<int,int>> new_to_do;
        for(int i=0;i<(int) to_do.size();i++){
            std::vector<std::pair<int,int>> way = matrix_mirror[to_do[i].first][to_do[i].second].second;
            way.push_back(to_do[i]);
            if(to_do[i].first == b.first && to_do[i].second == b.second){
                return way;
            }
            // Up
            if(to_do[i].first-1>=0){
                std::pair< int, int> aux;
                aux.first = to_do[i].first-1;
                aux.second = to_do[i].second;
                if(matrix_mirror[aux.first][aux.second].first != -1){
                    new_to_do.push_back(aux);

                    matrix_mirror[aux.first][aux.second].first = -1;
                    matrix_mirror[aux.first][aux.second].second = way;
                    matrix_total_weight[aux.first][aux.second] = matrix_total_weight[to_do[i].first][to_do[i].second] + type_weight[matrix[aux.first][aux.second]];
                }
            }
            print_stage(to_do);
            // Rigth
            if(to_do[i].second+1<42){
                std::pair< int, int> aux;
                aux.first = to_do[i].first;
                aux.second = to_do[i].second+1;
                if(matrix_mirror[aux.first][aux.second].first != -1){
                    new_to_do.push_back(aux);

                    matrix_mirror[aux.first][aux.second].first = -1;
                    matrix_mirror[aux.first][aux.second].second = way;
                    matrix_total_weight[aux.first][aux.second] = matrix_total_weight[to_do[i].first][to_do[i].second] + type_weight[matrix[aux.first][aux.second]];
                }
            }
            print_stage(to_do);
            // Left
            if(to_do[i].second-1>=0){
                std::pair< int, int> aux;
                aux.first = to_do[i].first;
                aux.second = to_do[i].second-1;
                if(matrix_mirror[aux.first][aux.second].first != -1){
                    new_to_do.push_back(aux);
                    
                    matrix_mirror[aux.first][aux.second].first = -1;
                    matrix_mirror[aux.first][aux.second].second = way;
                    matrix_total_weight[aux.first][aux.second] = matrix_total_weight[to_do[i].first][to_do[i].second] + type_weight[matrix[aux.first][aux.second]];
                }
            }
            print_stage(to_do);
            // Down
            if(to_do[i].first+1<42){
                std::pair< int, int> aux;
                aux.first = to_do[i].first+1;
                aux.second = to_do[i].second;
                if(matrix_mirror[aux.first][aux.second].first != -1){
                    new_to_do.push_back(aux);
                    
                    matrix_mirror[aux.first][aux.second].first = -1;
                    matrix_mirror[aux.first][aux.second].second = way;
                    matrix_total_weight[aux.first][aux.second] = matrix_total_weight[to_do[i].first][to_do[i].second] + type_weight[matrix[aux.first][aux.second]];
                }
            }
            print_stage(to_do);
        }
        // print_stage(to_do);
        to_do = new_to_do;
    }
    return to_do;
}

std::vector<std::pair<int,int>> order(std::vector<std::pair<int,int>> vector){
    std::vector<int> weight;
    std::pair<int,int> aux;
    for(int i=0; i<(int) vector.size(); i++){
        weight.push_back(matrix_total_weight[vector[i].first][vector[i].second]);
    }
    for(int i = 1; i < (int) weight.size(); i++){
        for(int j = 0; j < (int) weight.size() - 1; j++){
            if(weight[j] > weight[j+1]){
                aux         = vector[j];
                vector[j]   = vector[j + 1];
                vector[j+1] = aux;
            }
        }
    }
    return vector;
}

std::vector<std::pair<int,int>> djikstra(std::pair<int,int> a, std::pair<int,int> b){
    std::vector<std::pair<int,int>> to_do;
    to_do.push_back(a);
    matrix_mirror[a.first][a.second].first = -1;
    int i=0;
    while(true){
        std::vector<std::pair<int,int>> way = matrix_mirror[to_do[i].first][to_do[i].second].second;
        way.push_back(to_do[i]);
        if(to_do[i].first == b.first && to_do[i].second == b.second){
            return way;
        }
        // Up
        if(to_do[i].first-1>=0){
            std::pair< int, int> aux;
            aux.first = to_do[i].first-1;
            aux.second = to_do[i].second;
            int new_weight = matrix_total_weight[to_do[i].first][to_do[i].second] + type_weight[matrix[aux.first][aux.second]];
            if(matrix_total_weight[aux.first][aux.second] == 0 || new_weight < matrix_total_weight[aux.first][aux.second]){
                to_do.push_back(aux);

                matrix_total_weight[aux.first][aux.second] = new_weight;
                matrix_mirror[aux.first][aux.second].second = way;
            }
        }
        // Rigth
        if(to_do[i].second+1<42){
            std::pair< int, int> aux;
            aux.first = to_do[i].first;
            aux.second = to_do[i].second+1;
            int new_weight = matrix_total_weight[to_do[i].first][to_do[i].second] + type_weight[matrix[aux.first][aux.second]];
            if(matrix_total_weight[aux.first][aux.second] == 0 || new_weight < matrix_total_weight[aux.first][aux.second]){
                to_do.push_back(aux);

                matrix_total_weight[aux.first][aux.second] = new_weight;
                matrix_mirror[aux.first][aux.second].second = way;
            }
        }
        // Left
        if(to_do[i].second-1>=0){
            std::pair< int, int> aux;
            aux.first = to_do[i].first;
            aux.second = to_do[i].second-1;
            int new_weight = matrix_total_weight[to_do[i].first][to_do[i].second] + type_weight[matrix[aux.first][aux.second]];
            if(matrix_total_weight[aux.first][aux.second] == 0 || new_weight < matrix_total_weight[aux.first][aux.second]){
                to_do.push_back(aux);

                matrix_total_weight[aux.first][aux.second] = new_weight;
                matrix_mirror[aux.first][aux.second].second = way;
            }
        }
        // Down
        if(to_do[i].first+1<42){
            std::pair< int, int> aux;
            aux.first = to_do[i].first+1;
            aux.second = to_do[i].second;
            int new_weight = matrix_total_weight[to_do[i].first][to_do[i].second] + type_weight[matrix[aux.first][aux.second]];
            if(matrix_total_weight[aux.first][aux.second] == 0 || new_weight < matrix_total_weight[aux.first][aux.second]){
                to_do.push_back(aux);

                matrix_total_weight[aux.first][aux.second] = new_weight;
                matrix_mirror[aux.first][aux.second].second = way;
            }
        }
        matrix_mirror[to_do[i].first][to_do[i].second].first = -1;
        print_stage(to_do);
        to_do.erase(to_do.begin());
        to_do = order(to_do);
    }
    return to_do;
}

double distance(std::pair<int,int> a, std::pair<int,int> b){
    return sqrt(pow(a.first-b.first,2) + pow(a.second-b.second,2));
}

std::vector<std::pair<int,int>> order_a(std::vector<std::pair<int,int>> vector,std::pair<int,int> a, std::pair<int,int> b){
    std::vector<double> weight;
    std::pair<int,int> aux;
    for(int i=0; i<(int) vector.size(); i++){
        double max = distance(a,matrix_mirror[vector[i].first][vector[i].second].second.back()) + distance(b,matrix_mirror[vector[i].first][vector[i].second].second.back());
        if(distance(a,vector[i]) + distance(b,vector[i]) > max){
            max = distance(a,vector[i]) + distance(b,vector[i]);
        }
        weight.push_back(max);
    }
    for(int i = 1; i < (int) weight.size(); i++){
        for(int j = 0; j < (int) weight.size() - 1; j++){
            if(weight[j] > weight[j+1]){
                aux         = vector[j];
                vector[j]   = vector[j + 1];
                vector[j+1] = aux;
            }
        }
    }
    return vector;
}

std::vector<std::pair<int,int>> a_star(std::pair<int,int> a, std::pair<int,int> b){
    std::vector<std::pair<int,int>> to_do;
    to_do.push_back(a);
    matrix_mirror[a.first][a.second].first = -1;
    int i=0;
    while(true){
        std::vector<std::pair<int,int>> way = matrix_mirror[to_do[i].first][to_do[i].second].second;
        way.push_back(to_do[i]);
        if(to_do[i].first == b.first && to_do[i].second == b.second){
            return way;
        }
        // Up
        if(to_do[i].first-1>=0){
            std::pair< int, int> aux;
            aux.first = to_do[i].first-1;
            aux.second = to_do[i].second;
            int new_weight = matrix_total_weight[to_do[i].first][to_do[i].second] + type_weight[matrix[aux.first][aux.second]];
            if(matrix_total_weight[aux.first][aux.second] == 0 || new_weight < matrix_total_weight[aux.first][aux.second]){
                to_do.push_back(aux);

                matrix_total_weight[aux.first][aux.second] = new_weight;
                matrix_mirror[aux.first][aux.second].second = way;
            }
        }
        // Rigth
        if(to_do[i].second+1<42){
            std::pair< int, int> aux;
            aux.first = to_do[i].first;
            aux.second = to_do[i].second+1;
            int new_weight = matrix_total_weight[to_do[i].first][to_do[i].second] + type_weight[matrix[aux.first][aux.second]];
            if(matrix_total_weight[aux.first][aux.second] == 0 || new_weight < matrix_total_weight[aux.first][aux.second]){
                to_do.push_back(aux);

                matrix_total_weight[aux.first][aux.second] = new_weight;
                matrix_mirror[aux.first][aux.second].second = way;
            }
        }
        // Left
        if(to_do[i].second-1>=0){
            std::pair< int, int> aux;
            aux.first = to_do[i].first;
            aux.second = to_do[i].second-1;
            int new_weight = matrix_total_weight[to_do[i].first][to_do[i].second] + type_weight[matrix[aux.first][aux.second]];
            if(matrix_total_weight[aux.first][aux.second] == 0 || new_weight < matrix_total_weight[aux.first][aux.second]){
                to_do.push_back(aux);

                matrix_total_weight[aux.first][aux.second] = new_weight;
                matrix_mirror[aux.first][aux.second].second = way;
            }
        }
        // Down
        if(to_do[i].first+1<42){
            std::pair< int, int> aux;
            aux.first = to_do[i].first+1;
            aux.second = to_do[i].second;
            int new_weight = matrix_total_weight[to_do[i].first][to_do[i].second] + type_weight[matrix[aux.first][aux.second]];
            if(matrix_total_weight[aux.first][aux.second] == 0 || new_weight < matrix_total_weight[aux.first][aux.second]){
                to_do.push_back(aux);

                matrix_total_weight[aux.first][aux.second] = new_weight;
                matrix_mirror[aux.first][aux.second].second = way;
            }
        }
        matrix_mirror[to_do[i].first][to_do[i].second].first = -1;
        print_stage(to_do);
        to_do.erase(to_do.begin());
        to_do = order_a(to_do,a,b);
    }
    return to_do;
}


std::vector<std::pair<int,int>> search_way_teste(){
    std::vector<std::pair<int,int>> way;
    std::pair<int,int> aux;
    aux.first  = 1;
    aux.second = 1;
    way.push_back(aux);    
    aux.first  = 1;
    aux.second = 1;
    way.push_back(aux);    
    aux.first  = 1;
    aux.second = 2;
    way.push_back(aux);    
    aux.first  = 1;
    aux.second = 3;
    way.push_back(aux);    
    aux.first  = 1;
    aux.second = 4;
    way.push_back(aux);    
    aux.first  = 2;
    aux.second = 4;
    way.push_back(aux);    
    aux.first  = 2;
    aux.second = 5;
    way.push_back(aux);    
    aux.first  = 2;
    aux.second = 6;
    way.push_back(aux);    
    aux.first  = 3;
    aux.second = 6;
    way.push_back(aux);    
    aux.first  = 4;
    aux.second = 6;
    way.push_back(aux);    
    aux.first  = 5;
    aux.second = 6;
    way.push_back(aux);

    return way; 
}

void read_map(){
    matrix = (int**) malloc (sizeof(int*)*x_Dimension);
    matrix_total_weight = (int**) malloc (sizeof(int*)*x_Dimension);
    for(int i = 0; i<x_Dimension;i++){
        matrix[i] = (int*) malloc (sizeof(int)*y_Dimension);
        matrix_total_weight[i] = (int*) malloc (sizeof(int)*y_Dimension);
    }
    FILE *in = fopen("mapa","r");
    for(int i = 0; i<x_Dimension; i++){
        std::vector< std::pair< int,std::vector< std::pair< int,int > > > > aux;
        for(int j = 0; j<y_Dimension; j++){
            matrix[i][j] = fgetc(in) - 48;
            matrix_total_weight[i][j] = 0;
            fgetc(in);
            std::pair<int,std::vector< std::pair< int,int > > > aux2;
            aux2.first = matrix[i][j];
            aux.push_back(aux2);
        }
        matrix_mirror.push_back(aux);
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

void matrix_draw_loading(){
    int i,j;
    for(i=0;i<x_Dimension;i++){
        for(j=0;j<y_Dimension;j++){
            if(matrix_mirror[i][j].first != -1){
                al_draw_filled_rectangle(TAM_BORDA+(j*TAM_CELULA), TAM_BORDA+(i*TAM_CELULA),TAM_BORDA+((j+1)*TAM_CELULA), TAM_BORDA+((i+1)*TAM_CELULA), al_map_rgb(color[matrix_mirror[i][j].first][0], color[matrix_mirror[i][j].first][1], color[matrix_mirror[i][j].first][2]));
                // al_draw_filled_circle(TAM_BORDA+(i*TAM_CELULA)+(TAM_CELULA/2), TAM_BORDA+(j*TAM_CELULA)+(TAM_CELULA/2), (TAM_CELULA/2), al_map_rgb(color[matrix[i][j]][0], color[matrix[i][j]][1], color[matrix[i][j]][2]));
            }
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

void draw_way(std::pair<int,int> steep){
    // al_draw_filled_rectangle(TAM_BORDA+(steep.first*TAM_CELULA), TAM_BORDA+(steep.second*TAM_CELULA),TAM_BORDA+((steep.first+1)*TAM_CELULA), TAM_BORDA+((steep.second+1)*TAM_CELULA), al_map_rgb(0, 0, 0));
    al_draw_filled_circle(TAM_BORDA+(steep.second*TAM_CELULA)+(TAM_CELULA/2), TAM_BORDA+(steep.first*TAM_CELULA)+(TAM_CELULA/2), (TAM_CELULA/2), al_map_rgb(160, 0, 128));
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
