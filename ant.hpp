class ant{
public:
	int x;
	int y;
	int last_x;
	int type_data;
	int last_y;
	bool carregada;
	int vision;
	int total_frame;
	int **matrix;
	int x_Dimension;
	int y_Dimension;
	ant();
	void setVision(int vision);
	void set_info_screen(int **matrix,int x_Dimension,int y_Dimension);
	void init_position(int x, int y,int vision);
	void draw(int TAM_BORDA,int TAM_CELULA);
	void move();
	void move_atraction();
	void pick_up();
	void drop();
};