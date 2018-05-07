#include "../../sdk/dexsdk.h"
#include "../../sdk/time.h"

// board elements
#define empty	0
#define block	1
#define stop	2
#define bomb	3
#define diamond 4
#define ball	5

void set_graphics(int mode);
void write_pixel(int x, int y, char color);
void write_text(char *str, int x,int y, char color,int size);

// board functions
void init_board();
void setup_level();
void draw_block(int x, int y);
void draw_stop(int x, int y);
void draw_diamond(int x, int y);
void draw_ball(int x, int y, char color);
void fill_area(int x, int y, int width, int height);
void draw_arrow(int x, int y, int direction);
void draw_board();

// game controls
void move_ball(int xDirection, int yDirection);

int board[12][15];
int xPos, yPos;
int stop_flag;
int num_diamonds;
int dead;

void new_game(){
	init_board();
	setup_level();
	draw_board();
	dead = 0;
}

int main(){
	char keypress;

	set_graphics(VGA_320X200X256);

	for (int i=0; i<10; i++) write_pixel(88+i, 8, YELLOW);
	write_text("INERTIA GAME", 100, 5, YELLOW, 0);
	for (int i=0; i<10; i++) write_pixel(210+i, 8, YELLOW);

	write_text("CONTROLS", 22, 20, WHITE, 0);
	write_text("1 - MOVE ", 4, 39, WHITE, 0);	draw_arrow(77, 39, 1);
	write_text("2 - MOVE ", 4, 47, WHITE, 0);	draw_arrow(77, 47, 2);
	write_text("3 - MOVE ", 4, 55, WHITE, 0);	draw_arrow(77, 55, 3);
	write_text("4 - MOVE ", 4, 63, WHITE, 0);	draw_arrow(77, 63, 4);
	write_text("6 - MOVE ", 4, 71, WHITE, 0);	draw_arrow(77, 71, 6);
	write_text("7 - MOVE ", 4, 79, WHITE, 0);	draw_arrow(77, 79, 7);
	write_text("8 - MOVE ", 4, 87, WHITE, 0);	draw_arrow(77, 87, 8);
	write_text("9 - MOVE ", 4, 95, WHITE, 0);	draw_arrow(77, 95, 9);
	write_text("N - NEW GAME", 4, 120, WHITE, 0);
	write_text("X - EXIT GAME",4, 130, WHITE, 0);

	new_game();

	do{
		if (num_diamonds == 64){
			write_text("YOU WIN", 140, 168, GREEN, 0);
			write_text("NEW GAME? Y/N", 100, 178, RED, 0);
			do{
				keypress = (char) getch();
				if (keypress=='y'){
					fill_area(135, 170, 55, 7);
					fill_area(100, 178, 120, 7);
					fill_area(130, 20, 153, 126);
					new_game();
					break;
				}else if (keypress=='n'){
					keypress = 'x';
					break;
				}
			}while(1);
		}
		if (dead){
			write_text("DEAD", 136, 168, RED, 0);
			write_text("NEW GAME? Y/N", 100, 178, RED, 0);
			do{
				keypress = (char) getch();
				if (keypress=='y'){
					fill_area(135, 170, 55, 7);
					fill_area(100, 178, 120, 7);
					fill_area(130, 20, 153, 126);
					new_game();
					break;
				}else if (keypress=='n'){
					keypress = 'x';
					break;
				}
			}while(1);
		}else if (kb_ready()){
			keypress = (char)getch();
			switch(keypress){
				case ',': move_ball(-1, 1); break;	//downleft
				case '.': move_ball(0, 1); break;	//down
				case '/': move_ball(1, 1); break;	//downright
				case 'k': move_ball(-1, 0); break;	//left
				case ';': move_ball(1, 0); break;	//right
				case 'i': move_ball(-1, -1); break;	//upleft
				case 'o': move_ball(0, -1); break;	//up
				case 'p': move_ball(1, -1); break;	//upright
				case 'n':
					write_text("NEW GAME? Y/N", 100, 178, RED, 0);
					do{
						keypress = (char) getch();
						if (keypress=='y'){
							fill_area(100, 178, 120, 7);
							fill_area(130, 20, 153, 126);
							new_game();
							break;
						}else if (keypress=='n'){
							fill_area(100, 178, 120, 7);
							break;
						}
					}while(1);
			}
		}
	}while(keypress != 'x');
	set_graphics(VGA_TEXT80X25X16);
	clrscr();
	return 0;
}

void draw_arrow(int x, int y, int direction){
	// fucking hardcoded!!
	int i;
	switch(direction){
		case 1:
			for(i=4;i<6;i++) write_pixel(x+i, y  , WHITE);
			for(i=3;i<7;i++) write_pixel(x+i, y+1, WHITE);
			write_pixel(x, y+2, WHITE);
			for(i=2;i<7;i++) write_pixel(x+i, y+2, WHITE);
			for(i=0;i<6;i++) write_pixel(x+i, y+3, WHITE);
			for(i=0;i<5;i++) write_pixel(x+i, y+4, WHITE);
			for(i=0;i<4;i++) write_pixel(x+i, y+5, WHITE);
			for(i=0;i<5;i++) write_pixel(x+i, y+6, WHITE);
			break;
		case 2:
			for(i=2;i<5;i++) write_pixel(x+i, y  , WHITE);
			for(i=2;i<5;i++) write_pixel(x+i, y+1, WHITE);
			for(i=2;i<5;i++) write_pixel(x+i, y+2, WHITE);
			for(i=0;i<7;i++) write_pixel(x+i, y+3, WHITE);
			for(i=1;i<6;i++) write_pixel(x+i, y+4, WHITE);
			for(i=2;i<5;i++) write_pixel(x+i, y+5, WHITE);
			write_pixel(x+3, y+6, WHITE);
			break;
		case 3:
			for(i=1;i<3;i++) write_pixel(x+i, y  , WHITE);
			for(i=0;i<4;i++) write_pixel(x+i, y+1, WHITE);
			for(i=0;i<5;i++) write_pixel(x+i, y+2, WHITE);
			write_pixel(x+6, y+2, WHITE);
			for(i=1;i<7;i++) write_pixel(x+i, y+3, WHITE);
			for(i=2;i<7;i++) write_pixel(x+i, y+4, WHITE);
			for(i=3;i<7;i++) write_pixel(x+i, y+5, WHITE);
			for(i=2;i<7;i++) write_pixel(x+i, y+6, WHITE);
			break;
		case 4:
			write_pixel(x+3, y, WHITE);
			for(i=2;i<5;i++) write_pixel(x+i, y+1, WHITE);
			for(i=1;i<7;i++) write_pixel(x+i, y+2, WHITE);
			for(i=0;i<7;i++) write_pixel(x+i, y+3, WHITE);
			for(i=1;i<7;i++) write_pixel(x+i, y+4, WHITE);
			for(i=2;i<5;i++) write_pixel(x+i, y+5, WHITE);
			write_pixel(x+3, y+6, WHITE);
			break;
		case 6:
			write_pixel(x+3, y, WHITE);
			for(i=3;i<5;i++) write_pixel(x+i, y+1, WHITE);
			for(i=0;i<6;i++) write_pixel(x+i, y+2, WHITE);
			for(i=0;i<7;i++) write_pixel(x+i, y+3, WHITE);
			for(i=0;i<6;i++) write_pixel(x+i, y+4, WHITE);
			for(i=3;i<5;i++) write_pixel(x+i, y+5, WHITE);
			write_pixel(x+3, y+6, WHITE);
			break;
		case 7:
			for (i=0;i<5;i++)write_pixel(x+i, y  , WHITE);
			for (i=0;i<4;i++)write_pixel(x+i, y+1, WHITE);
			for (i=0;i<5;i++)write_pixel(x+i, y+2, WHITE);
			for (i=0;i<6;i++)write_pixel(x+i, y+3, WHITE);
			write_pixel(x, y+4, WHITE);
			for (i=2;i<7;i++)write_pixel(x+i, y+4, WHITE);
			for (i=3;i<7;i++)write_pixel(x+i, y+5, WHITE);
			for (i=4;i<6;i++)write_pixel(x+i, y+6, WHITE);
			break;
		case 8:
			write_pixel(x+3, y, WHITE);
			for(i=2;i<5;i++) write_pixel(x+i, y+1, WHITE);
			for(i=1;i<6;i++) write_pixel(x+i, y+2, WHITE);
			for(i=0;i<7;i++) write_pixel(x+i, y+3, WHITE);
			for(i=2;i<5;i++) write_pixel(x+i, y+4, WHITE);
			for(i=2;i<5;i++) write_pixel(x+i, y+5, WHITE);
			for(i=2;i<5;i++) write_pixel(x+i, y+6, WHITE);
			break;
		case 9:
			for(i=2;i<7;i++) write_pixel(x+i, y  , WHITE);
			for(i=3;i<7;i++) write_pixel(x+i, y+1, WHITE);
			for(i=2;i<7;i++) write_pixel(x+i, y+2, WHITE);
			for(i=1;i<7;i++) write_pixel(x+i, y+3, WHITE);
			for(i=0;i<5;i++) write_pixel(x+i, y+4, WHITE);
			write_pixel(x+6, y+4, WHITE);
			for(i=0;i<4;i++) write_pixel(x+i, y+5, WHITE);
			for(i=1;i<3;i++) write_pixel(x+i, y+6, WHITE);
			break;
	}
}


void draw_diamond(int x, int y){
  int blank=4;
  int fill=1;
  for (int i=0; i<9; i++){
    for(int j=0; j<fill; j++)
      write_pixel(x+blank+j, y+i, CYAN);
    if (i<4){
      blank-=1; fill+=2;
    }else{
      blank+=1; fill-=2;
    }
  }
}

void draw_block(int x, int y){
  for (int i=0; i<9; i++)
    for (int j=0; j<9; j++)
    	if (i==0 || j==0)
    		write_pixel(x+j, y+i, WHITE);
    	else if (i==8 || j==8)
    		write_pixel(x+j, y+i, 56); 
    	else
	    	write_pixel(x+j, y+i, LIGHTGRAY);
}

void draw_stop(int x, int y){
  for (int i=1; i<8; i++){
    if (i==4) continue;
    write_pixel(x+i, y  , YELLOW);
    write_pixel(x+i, y+8, YELLOW);
    write_pixel(x  , y+i, YELLOW);
    write_pixel(x+8, y+i, YELLOW);
  }
  write_pixel(x+1, y+1, YELLOW);
  write_pixel(x+1, y+7, YELLOW);
  write_pixel(x+7, y+1, YELLOW);
  write_pixel(x+7, y+7, YELLOW);
}

void draw_ball(int x, int y, char color){
  for(int i=3; i<6; i++){
    write_pixel(x+i, y+1, color);
    write_pixel(x+i, y+7, color);
  }
  for (int i=2; i<7; i++){
    write_pixel(x+i, y+2, color);
    write_pixel(x+i, y+6, color);
  }
  for (int i=1; i<8; i++){
    write_pixel(x+i, y+3, color);
    write_pixel(x+i, y+4, color);
    write_pixel(x+i, y+5, color);
  }
}

void fill_area(int x, int y, int width, int height){
	for (int i=0; i<width; i++)
		for (int j=0; j<height; j++)
			write_pixel(x+i, y+j, BLACK);
}

void draw_board(){
	int i, j;
	for (i=-1; i<13; i++){
		for (j=-1; j<16; j++){
			if (i==-1 || i==12 || j==-1 || j==15){
				draw_block(j*9+139, i*9+29);
				continue;
			}
			switch(board[i][j]){
				case block:
					draw_block(j*9+139, i*9+29);
					break;
				case stop:
					draw_stop(j*9+139, i*9+29);
					break;
				case bomb:
					write_text("X", j*9+140, i*9+30, RED, 0);
					break;
				case diamond:
					draw_diamond(j*9+139, i*9+29);
					break;
				case ball:
					if (stop_flag){
						draw_stop(j*9+139, i*9+29);
					}
					draw_ball(j*9+139, i*9+29, LIGHTGREEN);
					break;
			}
		}
	}
}

void init_board(){
	for (int i=0;i<12;i++)
		for (int j=0;j<15;j++)
			board[i][j] = 0;
}


void setup_level(){
	int x, y;
	int num_blocks = 36;
	int num_stops  = 36;
	int num_bombs  = 36;
	num_diamonds   = 36;

	time_t t;
	srand(time());

	while (num_blocks > 0){
		x = (rand()%15); y = (rand()%12);

		if (! board[y][x]){
			board[y][x] = block;
			num_blocks --;
		}
	}

	while (num_stops > 0){
		x = (rand()%15); y = (rand()%12);

		if (! board[y][x]){
			board[y][x] = stop;
			num_stops --;
		}
	}

	while (num_bombs > 0){
		x = (rand()%15); y = (rand()%12);

		if (! board[y][x]){
			board[y][x] = bomb;
			num_bombs --;
		}
	}

	while (num_diamonds > 0){
		x = (rand()%15); y = (rand()%12);

		if (! board[y][x]){
			board[y][x] = diamond;
			num_diamonds --;
		}
	}
	while (board[y][x] != 0){
		x = (rand()%15); y = (rand()%12);
	}
	board[y][x] = ball;
	xPos = x; yPos = y;
	stop_flag = 1;
}

void move_ball(int xDirection, int yDirection){
	while (1){
		if(xPos+xDirection==15 || yPos+yDirection==12 || xPos+xDirection==-1 || yPos+yDirection==-1){
			// if ball encountered the boundary
			break;
		}else if (board[yPos+yDirection][xPos+xDirection]==stop){
			// if ball entered a stop
			if (stop_flag){
				board[yPos][xPos] = stop;
				fill_area(xPos*9+140, yPos*9+30, 7, 7);
			}else{
				board[yPos][xPos] = empty;
				fill_area(xPos*9+139, yPos*9+29, 9, 9);
			}
			stop_flag = 1;
			xPos += xDirection; yPos += yDirection;
			board[yPos][xPos] = ball;
			draw_ball(xPos*9+139, yPos*9+29, LIGHTGREEN);
			break;
		}else if (board[yPos+yDirection][xPos+xDirection]==bomb){
			// if ball hit a bomb
			if (stop_flag){
				board[yPos][xPos] = stop;
				fill_area(xPos*9+140, yPos*9+30, 7, 7);
			}else{
				board[yPos][xPos] = empty;
				fill_area(xPos*9+139, yPos*9+29, 9, 9);
			}
			xPos += xDirection; yPos += yDirection;
			board[yPos][xPos] = ball;
			fill_area(xPos*9+139, yPos*9+29, 9, 9);
			draw_ball(xPos*9+139, yPos*9+29, RED);
			stop_flag = 0;
			dead = 1;
			break;
		}else if(board[yPos+yDirection][xPos+xDirection]==block){
			//if ball encountered a block
			break;
		}

		if (board[yPos+yDirection][xPos+xDirection]==diamond)
			num_diamonds += 1;

		if (stop_flag){
			board[yPos][xPos] = stop;
			fill_area(xPos*9+140, yPos*9+30, 7, 7);
		}else{
			board[yPos][xPos] = empty;
			fill_area(xPos*9+139, yPos*9+29, 9, 9);
		}

		xPos += xDirection; yPos += yDirection;
		stop_flag = 0;
		board[yPos][xPos] = ball;
		draw_ball(xPos*9+139, yPos*9+29, LIGHTGREEN);
		delay(5);
	}
}
