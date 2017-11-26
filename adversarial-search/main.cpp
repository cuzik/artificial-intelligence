// Bibliotecas GUI
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>

// Bibliotecas do sistema
#include <iostream>
#include <vector>

// Variaveis Fixas
int TAM_CELULA = 150;
int TAM_BORDA = 15;
int x_Dimension = 3;
int y_Dimension = 3;
int LARGURA_TELA = x_Dimension * TAM_CELULA + 2 * TAM_BORDA;
int ALTURA_TELA = y_Dimension * TAM_CELULA + 2 * TAM_BORDA;

// Variaveis da GUI
ALLEGRO_DISPLAY *janela = NULL;
ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
ALLEGRO_FONT *fonte = NULL;
ALLEGRO_BITMAP *player_1 = NULL;
ALLEGRO_BITMAP *player_2 = NULL;

//
typedef struct ${
    int x;
    int y;
} Ponto;

// Funções com GUI
bool inicializar();
void draw_tab();
void draw_tab_bob();
void read_keyboard();

// Funções do Jogo
bool marca_player(int pos_x, int pos_y);
void troca_jogador();
int verifica_vencedor(int matriz[3][3]);
void verifica_fim();
int min_max(int player, int profundidade, bool adversario, int matrix[3][3]);
void minimax(int player, int profundidade, bool adversario, int matrix[3][3]);
int alpha_beta(int player, int profundidade, bool adversario, int matrix[3][3], int alpha, int beta);
void poda_alpha_beta(int player, int profundidade, bool adversario, int matrix[3][3], int alpha, int beta);
void comp_move();
void player_player();
void comp_player();
void comp_comp();
void minimax_alphabeta();
void run_test();

// Variaives do Jogo
int tabuleiro[3][3] = {{0,0,0},{0,0,0},{0,0,0}};
int x=0;
int y=0;
bool run = true;
int expan = 0;
int player = -1;
int jogadas = 0;
int mode;
int alg;
int frames = 0;

int main(int argc, char const *argv[]){
    if(argc<3){
        std::cout << "Ta faltando paremetro ai brother\n\n Tenta assim:\n\n./hocus_pocus [MODE] [ALG]\n-> [MODE]\n\n(0) Player   x Player\n(1) Player   x Computer (Player   First)\n(2) Computer x Player   (Computer First)\n(3) Computer x Computer\n\n(4) Comp(MiniMax) x Comp(AlphaBeta)\n-> [ALG]: Algorithm\n\n(0) MiniMax\n(1) Alpha-Beta\n" << std::endl;
        return 0;
    }
    mode = atoi(argv[1]);
    alg = atoi(argv[2]);
    if(mode < 0 || mode > 5 || alg < 0 || alg > 1){
        std::cout << "Tem paremetro errado ai parça\n\n Tenta assim:\n\n./hocus_pocus [MODE] [ALG]\n-> [MODE]\n\n(0) Player   x Player\n(1) Player   x Computer (Player   First)\n(2) Computer x Player   (Computer First)\n(3) Computer x Computer\n(4) Comp(MiniMax) x Comp(AlphaBeta)\n\n-> [ALG]: Algorithm\n\n(0) MiniMax\n(1) Alpha-Beta\n" << std::endl;
        return 0;
    }
    srand( (unsigned)time(NULL) );
    inicializar();
    switch(mode){
        case 0:
            player_player();
            break;
        case 1:
            comp_player();
            break;
        case 2:
            player = 1;
            comp_player();
            break;
        case 3:
            comp_comp();
        case 4:
            minimax_alphabeta();
            break;
        case 5:
            run_test();
            break;
    }
    return 0;
}

void run_test(){
    int tabular_test[9][3][3]= {{{1,0,0},{0,0,0},{0,0,0}},
                            {{0,1,0},{0,0,0},{0,0,0}},
                            {{0,0,1},{0,0,0},{0,0,0}},
                            {{0,0,0},{1,0,0},{0,0,0}},
                            {{0,0,0},{0,1,0},{0,0,0}},
                            {{0,0,0},{0,0,1},{0,0,0}},
                            {{0,0,0},{0,0,0},{1,0,0}},
                            {{0,0,0},{0,0,0},{0,1,0}},
                            {{0,0,0},{0,0,0},{0,0,1}}};

    for(int i=0;i<9;i++){
        minimax(-1,1,false,tabular_test[i]);
    }
    std::cout << std::endl;
    for(int i=0;i<9;i++){
        poda_alpha_beta(-1,1,false,tabular_test[i],-100000,100000);
    }
}


void player_player(){
    while(run){
        frames++;
        verifica_fim();
        al_clear_to_color(al_map_rgb(239, 230, 230));
        read_keyboard();
        draw_tab();
        al_flip_display();
    }
}

void comp_player(){
    while(run){
        frames++;
        verifica_fim();
        al_clear_to_color(al_map_rgb(239, 230, 230));
        if(player==1){
            comp_move();
        }
        read_keyboard();
        draw_tab();
        al_flip_display();
    }
}

void comp_comp(){
    while(run){
        frames++;
        verifica_fim();
        al_clear_to_color(al_map_rgb(239, 230, 230));
        read_keyboard();
        comp_move();
        draw_tab();
        al_flip_display();
    }
}

void minimax_alphabeta(){
    while(run){
        frames++;
        verifica_fim();
        al_clear_to_color(al_map_rgb(239, 230, 230));
        if(player==-1){
            minimax(player,jogadas,false,tabuleiro);
        }else{
            poda_alpha_beta(player,jogadas,false,tabuleiro,-100000,100000);
        }
        read_keyboard();
        // comp_move();
        draw_tab();
        al_flip_display();
    }
}

void comp_move(){
    // usleep(500000);
    if(alg == 0){
        minimax(player,jogadas,false,tabuleiro);
    }else{
        poda_alpha_beta(player,jogadas,false,tabuleiro,-100000,100000);
    }
}

void verifica_fim(){
    if(verifica_vencedor(tabuleiro)!=0){
        if(verifica_vencedor(tabuleiro)==-1)
            std::cout << "Bob Wins" << std::endl;
        else
            std::cout << "Ana Wins" << std::endl;
        exit(1);
    }else if(jogadas == 9){
        std::cout << "A Tie" << std::endl;
        exit(1);
    }
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
                if(y<2){
                    y++;
                }
            }else if(evento.keyboard.keycode == ALLEGRO_KEY_DOWN){
                if(x<2){
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
                }
            }
        }else if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
            run = false;
        }
    }
}


void minimax(int player, int profundidade, bool adversario, int matrix[3][3]){
    expan = 0;

    std::vector< Ponto > ponto;
    std::vector<int> pesos;
    int aux,max=-10000;
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            if(matrix[i][j]==0){
                matrix[i][j]=player;
                aux = min_max(player,profundidade+1,true,matrix);
                matrix[i][j]=0;
                if(max < aux){
                    max = aux;
                }
                Ponto ponto_aux;
                ponto_aux.x = i;
                ponto_aux.y = j;
                ponto.push_back(ponto_aux);
                pesos.push_back(aux);
            }
        }
    }
    int a = (rand() % 9) + 1;
    int b = 0;
    for(int i =0; i<(int) pesos.size()*a;i++){
        if(pesos[i%pesos.size()] == max){
            b++;
        }
        if(a==b){
            tabuleiro[ponto[i%pesos.size()].x][ponto[i%pesos.size()].y] = player;
            jogadas++;
            troca_jogador();
            std::cout << "Nós exp: " << expan << std::endl;
            return;
        }
    }
}

int min_max(int player, int profundidade, bool adversario, int matrix[3][3]){
    expan++;
    int min = 10000;
    int max = -10000;
    int aux = 0;
    if(verifica_vencedor(matrix)!=0 || profundidade==9){
        return profundidade + (verifica_vencedor(matrix) * 10 * player);
    }
    if(adversario){
        for(int i=0;i<3;i++){
            for(int j=0;j<3;j++){
                if(matrix[i][j]==0){
                    matrix[i][j]=-player;
                    aux = min_max(player,profundidade+1,false,matrix);
                    matrix[i][j]=0;
                    if(min > aux){
                        min = aux;
                    }
                }
            }
        }
        return min;
    }else{
        for(int i=0;i<3;i++){
            for(int j=0;j<3;j++){
                if(matrix[i][j]==0){
                    matrix[i][j]=player;
                    aux = min_max(player,profundidade+1,true,matrix);
                    matrix[i][j]=0;
                    if(max < aux){
                        max = aux;
                    }
                }
            }
        }
        return max;
    }
}

void poda_alpha_beta(int player, int profundidade, bool adversario, int matrix[3][3], int alpha, int beta){
    expan = 0;

    std::vector< Ponto > ponto;
    std::vector<int> pesos;
    int aux,max=-10000;
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            if(matrix[i][j]==0){
                matrix[i][j]=player;
                aux = alpha_beta(player,profundidade+1,true,matrix,alpha,beta);
                matrix[i][j]=0;
                if(max < aux){
                    max = aux;
                }
                Ponto ponto_aux;
                ponto_aux.x = i;
                ponto_aux.y = j;
                ponto.push_back(ponto_aux);
                pesos.push_back(aux);
            }
        }
    }
    int a = (rand() % 9) + 1;
    int b = 0;
    for(int i =0; i<(int) pesos.size()*a;i++){
        if(pesos[i%pesos.size()] == max){
            b++;
        }
        if(a==b){
            tabuleiro[ponto[i%pesos.size()].x][ponto[i%pesos.size()].y] = player;
            jogadas++;
            troca_jogador();
            std::cout << "Nós exp: " << expan << std::endl;
            return;
        }
    }
}

int alpha_beta(int player, int profundidade, bool adversario, int matrix[3][3], int alpha, int beta){
    expan++;
    int min = 10000;
    int max = -10000;
    if(verifica_vencedor(matrix)!=0 || profundidade==9){
        return profundidade + (verifica_vencedor(matrix) * 10 * player);
    }
    if(adversario){
        for(int i=0;i<3;i++){
            for(int j=0;j<3;j++){
                if(matrix[i][j]==0){
                    matrix[i][j]=-player;
                    beta = alpha_beta(player,profundidade+1,false,matrix,alpha,beta);
                    matrix[i][j]=0;
                    if(min > beta){
                        min = beta;
                    }
                    if(min <= alpha){
                        return min;
                    }
                }
            }
        }
        return min;
    }else{
        for(int i=0;i<3;i++){
            for(int j=0;j<3;j++){
                if(matrix[i][j]==0){
                    matrix[i][j]=player;
                    alpha = alpha_beta(player,profundidade+1,true,matrix,alpha,beta);
                    matrix[i][j]=0;
                    if(max < alpha){
                        max = alpha;
                    }
                    if(max >= beta){
                        return max;
                    }
                }
            }
        }
        return max;
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
            if((i+j) % 2 == 0){
                al_draw_filled_rectangle(TAM_BORDA+(j*TAM_CELULA), TAM_BORDA+(i*TAM_CELULA),TAM_BORDA+((j+1)*TAM_CELULA), TAM_BORDA+((i+1)*TAM_CELULA), al_map_rgb(204, 199, 199));
            }
            if(tabuleiro[i][j]==1){
                // al_draw_circle(TAM_BORDA+(j*TAM_CELULA)+(TAM_CELULA/2), TAM_BORDA+(i*TAM_CELULA)+(TAM_CELULA/2), (TAM_CELULA/2), al_map_rgb(213, 219, 37), 2.0);
                al_draw_filled_circle(TAM_BORDA+(j*TAM_CELULA)+(TAM_CELULA/2), TAM_BORDA+(i*TAM_CELULA)+(TAM_CELULA/2), (TAM_CELULA/2), al_map_rgb(213, 219, 37));
            }else if(tabuleiro[i][j]==-1){
                // al_draw_circle(TAM_BORDA+(j*TAM_CELULA)+(TAM_CELULA/2), TAM_BORDA+(i*TAM_CELULA)+(TAM_CELULA/2), (TAM_CELULA/2), al_map_rgb(4, 84, 137), 2.0);
                al_draw_filled_circle(TAM_BORDA+(j*TAM_CELULA)+(TAM_CELULA/2), TAM_BORDA+(i*TAM_CELULA)+(TAM_CELULA/2), (TAM_CELULA/2), al_map_rgb(4, 84, 137));
            }
            if(i==x && j==y){
                if(tabuleiro[i][j]!=0){
                    al_draw_rectangle(TAM_BORDA+(j*TAM_CELULA)+3, TAM_BORDA+(i*TAM_CELULA)+3,TAM_BORDA+((j+1)*TAM_CELULA)-3, TAM_BORDA+((i+1)*TAM_CELULA)-3, al_map_rgb(244, 95, 95),6.0);
                    // al_draw_filled_rectangle(TAM_BORDA+(j*TAM_CELULA), TAM_BORDA+(i*TAM_CELULA),TAM_BORDA+((j+1)*TAM_CELULA), TAM_BORDA+((i+1)*TAM_CELULA), al_map_rgb(244, 95, 95));
                    // al_draw_circle(TAM_BORDA+(j*TAM_CELULA)+(TAM_CELULA/2), TAM_BORDA+(i*TAM_CELULA)+(TAM_CELULA/2), (TAM_CELULA/2)-3, al_map_rgb(244, 95, 95), 6.0);
                }else{
                    al_draw_rectangle(TAM_BORDA+(j*TAM_CELULA)+3, TAM_BORDA+(i*TAM_CELULA)+3,TAM_BORDA+((j+1)*TAM_CELULA)-3, TAM_BORDA+((i+1)*TAM_CELULA)-3, al_map_rgb(11, 132, 32),6.0);
                    // al_draw_filled_rectangle(TAM_BORDA+(j*TAM_CELULA), TAM_BORDA+(i*TAM_CELULA),TAM_BORDA+((j+1)*TAM_CELULA), TAM_BORDA+((i+1)*TAM_CELULA), al_map_rgb(11, 132, 32));
                    // al_draw_circle(TAM_BORDA+(j*TAM_CELULA)+(TAM_CELULA/2), TAM_BORDA+(i*TAM_CELULA)+(TAM_CELULA/2), (TAM_CELULA/2)-3, al_map_rgb(11, 132, 32), 6.0);
                }
            }
        }
    }
    if(player == -1){
        al_draw_textf(fonte, al_map_rgb(0, 0, 0), 10 , 0, 0, "Frames: %i \tJogadas: %i \tPlayer: Bob",frames,jogadas);
    }else{
        al_draw_textf(fonte, al_map_rgb(0, 0, 0), 10 , 0, 0, "Frames: %i \tJogadas: %i \tPlayer: Ana",frames,jogadas);
    }
}

void draw_tab_bob(){
    for(int i=0;i<x_Dimension;i++){
        for(int j=0;j<y_Dimension;j++){
            if((i+j) % 2 == 0){
                al_draw_filled_rectangle(TAM_BORDA+(j*TAM_CELULA), TAM_BORDA+(i*TAM_CELULA),TAM_BORDA+((j+1)*TAM_CELULA), TAM_BORDA+((i+1)*TAM_CELULA), al_map_rgb(204, 199, 199));
            }
            if(tabuleiro[i][j]==1){
                al_draw_bitmap(player_1, TAM_BORDA+(j*TAM_CELULA), TAM_BORDA+(i*TAM_CELULA), 0);
            }else if(tabuleiro[i][j]==-1){
                al_draw_bitmap(player_2, TAM_BORDA+(j*TAM_CELULA), TAM_BORDA+(i*TAM_CELULA), 0);
            }
            if(i==x && j==y){
                if(tabuleiro[i][j]!=0){
                    al_draw_rectangle(TAM_BORDA+(j*TAM_CELULA)+3, TAM_BORDA+(i*TAM_CELULA)+3,TAM_BORDA+((j+1)*TAM_CELULA)-3, TAM_BORDA+((i+1)*TAM_CELULA)-3, al_map_rgb(244, 95, 95),6.0);
                }else{
                    al_draw_rectangle(TAM_BORDA+(j*TAM_CELULA)+3, TAM_BORDA+(i*TAM_CELULA)+3,TAM_BORDA+((j+1)*TAM_CELULA)-3, TAM_BORDA+((i+1)*TAM_CELULA)-3, al_map_rgb(11, 132, 32),6.0);
                }
            }
        }
    }
    if(player == -1){
        al_draw_textf(fonte, al_map_rgb(0, 0, 0), 10 , 0, 0, "Frames: %i \tJogadas: %i \tPlayer: Bob",frames,jogadas);
    }else{
        al_draw_textf(fonte, al_map_rgb(0, 0, 0), 10 , 0, 0, "Frames: %i \tJogadas: %i \tPlayer: Ana",frames,jogadas);
    }
}

int verifica_vencedor(int matriz[3][3]){
    for(int i=0;i<3;i++){
        if((matriz[i][0] + matriz[i][1] + matriz[i][2]) == 3 || (matriz[0][i] + matriz[1][i] + matriz[2][i]) == 3){
            return 1;
        }
    }
    if((matriz[0][0] + matriz[1][1] + matriz[2][2]) == 3 || (matriz[2][0] + matriz[1][1] + matriz[0][2]) == 3){
        return 1;
    }
    for(int i=0;i<3;i++){
        if((matriz[i][0] + matriz[i][1] + matriz[i][2]) == -3 || (matriz[0][i] + matriz[1][i] + matriz[2][i]) == -3){
            return -1;
        }
    }
    if((matriz[0][0] + matriz[1][1] + matriz[2][2]) == -3 || (matriz[2][0] + matriz[1][1] + matriz[0][2]) == -3){
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
    al_init_image_addon();

    player_1 = al_load_bitmap("image/gre_1.jpg");
    player_2 = al_load_bitmap("image/gre_2.jpg");

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
