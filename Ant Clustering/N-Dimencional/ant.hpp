#include "Object.h"

double pp(double f);
double pd(double f);
double distancia_enclidiana(Object a, Object b);
double funcao_similaridade(Object data, Object **matrix, int x_Dimension, int y_Dimension, int vision, int x, int y);

class ant{
public:
	int x;
	int y;
	int last_x;
	Object data_up;
	int last_y;
	bool carregada;
	int vision;
	Object **matrix;
	int x_Dimension;
	int y_Dimension;
	ant();
	void setVision(int vision);
	void set_info_screen(int N, Object **matrix,int x_Dimension,int y_Dimension);
	void init_position(int x, int y,int vision);
	void draw(int TAM_BORDA,int TAM_CELULA);
	void move();
	void pick_up();
	void drop();
};