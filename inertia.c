#include "../../sdk/dexsdk.h"
#include "../../sdk/time.h"

// board elements
#define empty	0
#define block	1
#define stop	2
#define bomb	3
#define diamond 4
#define ball	5

#define	KEY_INS		0x90
#define	KEY_DEL		(KEY_INS + 1)
#define	KEY_HOME	(KEY_DEL + 1)
#define	KEY_END		(KEY_HOME + 1)
#define	KEY_PGUP	(KEY_END + 1)
#define	KEY_PGDN	(KEY_PGUP + 1)
#define	KEY_LFT		(KEY_PGDN + 1)
#define	KEY_UP		(KEY_LFT + 1)
#define	KEY_DN		(KEY_UP + 1)
#define	KEY_RT		(KEY_DN + 1)

void title_screen();
void help_screen();
void new_game();
void write_counter(int x, int y);

// board functions
void init_board();
void setup_level();
void draw_block(int x, int y);
void draw_stop(int x, int y);
void draw_diamond(int x, int y);
void draw_ball(int x, int y, char color);
void fill_area(int x, int y, int width, int height, char color);
void draw_arrow(int x, int y, int direction);
void draw_board();

// game controls
void move_ball(int xDirection, int yDirection);

int board[12][15];
int xPos, yPos;
int stop_flag;
int num_diamonds;
int dead;


int main(){
	unsigned int keypress;

	set_graphics(VGA_320X200X256);

	do {
		title_screen();
		
		while(!kb_ready());
		keypress = getch();

		if (keypress == 'x') break;
		else if (keypress == 'n') ;
		else continue;

		fill_area(7, 5, 307, 192, BLACK);
		new_game();
		do{
			if (num_diamonds == 36){
				write_text("YOU WIN", 140, 168, GREEN, 0);
				write_text("NEW GAME? Y/N", 100, 178, WHITE, 0);
				do{
					while(!kb_ready());
					keypress = getch();
					if (keypress=='y'){
						fill_area(135, 170, 55, 7, BLACK);
						fill_area(100, 178, 120, 7, BLACK);
						fill_area(130, 20, 153, 126, BLACK);
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
				write_text("NEW GAME? Y/N", 100, 178, WHITE, 0);
				do{
					while(!kb_ready());
					keypress = getch();
					if (keypress=='y'){
						fill_area(136, 168, 55, 7, BLACK);			//erase dead
						fill_area(100, 178, 120, 7, BLACK);			//erase new game? y/n
						fill_area(150, 30, 153, 126, BLACK);		//erase board
						new_game();
						break;
					}else if (keypress=='n'){
						keypress = 'x';
						break;
					}
				}while(1);
			}else{
				while(!kb_ready());
				keypress = getch();
				if (keypress==',')	move_ball(-1, 1);		//downleft
				if (keypress=='.') 	move_ball(0, 1);		//down
				if (keypress=='/') 	move_ball(1, 1);		//downright
				if (keypress=='k') 	move_ball(-1, 0);		//left
				if (keypress==';') 	move_ball(1, 0);		//right
				if (keypress=='i') 	move_ball(-1, -1);		//upleft
				if (keypress=='o') 	move_ball(0, -1);		//up
				if (keypress=='p') 	move_ball(1, -1);		//upright
				// if (keypress==KEY_END)	move_ball(-1, 1);		//downleft
				// if (keypress==KEY_DN) 	move_ball(0, 1);		//down
				// if (keypress==KEY_PGDN) move_ball(1, 1);		//downright
				// if (keypress==KEY_LFT) 	move_ball(-1, 0);		//left
				// if (keypress==KEY_RT) 	move_ball(1, 0);		//right
				// if (keypress==KEY_HOME) move_ball(-1, -1);		//upleft
				// if (keypress==KEY_UP) 	move_ball(0, -1);		//up
				// if (keypress==KEY_PGUP) move_ball(1, -1);		//upright
				if (keypress=='n'){
					write_text("NEW GAME? Y/N", 100, 178, WHITE, 0);
					do{
						while(!kb_ready());
						keypress = getch();
						if (keypress=='y'){
							fill_area(100, 178, 120, 7, BLACK);
							fill_area(150, 30, 153, 126, BLACK);
							new_game();
							break;
						}else if (keypress=='n'){
							fill_area(100, 178, 120, 7, BLACK);
							break;
						}
					}while(1);
				}
				if (keypress=='x'){
					write_text("EXIT GAME? Y/N", 100, 178, WHITE, 0);
					do{
						while(!kb_ready());
						keypress = getch();
						if (keypress=='y'){
							keypress = 'x';
							break;
						}else if (keypress=='n'){
							fill_area(100, 178, 130, 7, BLACK);
							break;
						}
					}while(1);
				}
			}
		}while(keypress != 'x');
		fill_area(7, 5, 307, 192, BLACK);
	}while(1);
	set_graphics(VGA_TEXT80X25X16);
	clrscr();
	return 0;
}

void title_screen(){
	fill_area(  7,  3,307,  2, RED);
	fill_area(  5,  5,  2,  2, RED);
	fill_area(314,  5,  2,  2, RED);
	fill_area(  3,  7,  2,187, RED);

	fill_area(316,  7,  2,187, RED);
	fill_area(  5,195,  2,  2, RED);
	fill_area(314,195,  2,  2, RED);
	fill_area(  7,197,307,  2, RED);

	write_text("INERTIA GAME", 110, 40, YELLOW, 1);
	write_text("NEW GAME", 130, 100, WHITE, 0);
	write_text("EXIT", 145, 130, WHITE, 0);
}

void help_screen(){
	write_text("CONTROLS", 37, 30, WHITE, 0);
	write_text("1 - MOVE ", 23, 40, WHITE, 0);	draw_arrow(96, 40, 1);
	write_text("2 - MOVE ", 23, 50, WHITE, 0);	draw_arrow(96, 50, 2);
	write_text("3 - MOVE ", 23, 60, WHITE, 0);	draw_arrow(96, 60, 3);
	write_text("4 - MOVE ", 23, 70, WHITE, 0);	draw_arrow(96, 70, 4);
	write_text("6 - MOVE ", 23, 80, WHITE, 0);	draw_arrow(96, 80, 6);
	write_text("7 - MOVE ", 23, 90, WHITE, 0);	draw_arrow(96, 90, 7);
	write_text("8 - MOVE ", 23, 100, WHITE, 0);	draw_arrow(96, 100, 8);
	write_text("9 - MOVE ", 23, 110, WHITE, 0);	draw_arrow(96, 110, 9);
	write_text("N - NEW GAME", 23, 130, WHITE, 0);
	write_text("X - EXIT GAME",23, 140, WHITE, 0);
}

void new_game(){
	init_board();
	setup_level();
	for (int i=0; i<10; i++) write_pixel(88+i, 15, YELLOW);
	write_text("INERTIA GAME", 100, 12, YELLOW, 0);
	for (int i=0; i<10; i++) write_pixel(210+i, 15, YELLOW);
	help_screen();
	draw_board();
	write_counter(235,160);
	dead = 0;
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

void write_counter(int x, int y){
	char print[8];

	fill_area(x, y, 90, 7, BLACK);
	sprintf(print, ": %2d/36", num_diamonds);
	draw_diamond(x, y);
	write_text(print, x+9, y, WHITE, 0);
}

void draw_arrow(int x, int y, int direction){
	// fucking hardcoded!!
	int i;
	switch(direction){
		case 1: //downleft
			for(i=4;i<6;i++) write_pixel(x+i, y  , WHITE);
			for(i=3;i<7;i++) write_pixel(x+i, y+1, WHITE);
			write_pixel(x, y+2, WHITE);
			for(i=2;i<7;i++) write_pixel(x+i, y+2, WHITE);
			for(i=0;i<6;i++) write_pixel(x+i, y+3, WHITE);
			for(i=0;i<5;i++) write_pixel(x+i, y+4, WHITE);
			for(i=0;i<4;i++) write_pixel(x+i, y+5, WHITE);
			for(i=0;i<5;i++) write_pixel(x+i, y+6, WHITE);
			break;
		case 2: //down
			for(i=2;i<5;i++) write_pixel(x+i, y  , WHITE);
			for(i=2;i<5;i++) write_pixel(x+i, y+1, WHITE);
			for(i=2;i<5;i++) write_pixel(x+i, y+2, WHITE);
			for(i=0;i<7;i++) write_pixel(x+i, y+3, WHITE);
			for(i=1;i<6;i++) write_pixel(x+i, y+4, WHITE);
			for(i=2;i<5;i++) write_pixel(x+i, y+5, WHITE);
			write_pixel(x+3, y+6, WHITE);
			break;
		case 3: //downright
			for(i=1;i<3;i++) write_pixel(x+i, y  , WHITE);
			for(i=0;i<4;i++) write_pixel(x+i, y+1, WHITE);
			for(i=0;i<5;i++) write_pixel(x+i, y+2, WHITE);
			write_pixel(x+6, y+2, WHITE);
			for(i=1;i<7;i++) write_pixel(x+i, y+3, WHITE);
			for(i=2;i<7;i++) write_pixel(x+i, y+4, WHITE);
			for(i=3;i<7;i++) write_pixel(x+i, y+5, WHITE);
			for(i=2;i<7;i++) write_pixel(x+i, y+6, WHITE);
			break;
		case 4: //left
			write_pixel(x+3, y, WHITE);
			for(i=2;i<5;i++) write_pixel(x+i, y+1, WHITE);
			for(i=1;i<7;i++) write_pixel(x+i, y+2, WHITE);
			for(i=0;i<7;i++) write_pixel(x+i, y+3, WHITE);
			for(i=1;i<7;i++) write_pixel(x+i, y+4, WHITE);
			for(i=2;i<5;i++) write_pixel(x+i, y+5, WHITE);
			write_pixel(x+3, y+6, WHITE);
			break;
		case 5: //circle
			draw_ball(x, y, WHITE);
			break;
		case 6: //right
			write_pixel(x+3, y, WHITE);
			for(i=3;i<5;i++) write_pixel(x+i, y+1, WHITE);
			for(i=0;i<6;i++) write_pixel(x+i, y+2, WHITE);
			for(i=0;i<7;i++) write_pixel(x+i, y+3, WHITE);
			for(i=0;i<6;i++) write_pixel(x+i, y+4, WHITE);
			for(i=3;i<5;i++) write_pixel(x+i, y+5, WHITE);
			write_pixel(x+3, y+6, WHITE);
			break;
		case 7: //upleft
			for (i=0;i<5;i++)write_pixel(x+i, y  , WHITE);
			for (i=0;i<4;i++)write_pixel(x+i, y+1, WHITE);
			for (i=0;i<5;i++)write_pixel(x+i, y+2, WHITE);
			for (i=0;i<6;i++)write_pixel(x+i, y+3, WHITE);
			write_pixel(x, y+4, WHITE);
			for (i=2;i<7;i++)write_pixel(x+i, y+4, WHITE);
			for (i=3;i<7;i++)write_pixel(x+i, y+5, WHITE);
			for (i=4;i<6;i++)write_pixel(x+i, y+6, WHITE);
			break;
		case 8: //up
			write_pixel(x+3, y, WHITE);
			for(i=2;i<5;i++) write_pixel(x+i, y+1, WHITE);
			for(i=1;i<6;i++) write_pixel(x+i, y+2, WHITE);
			for(i=0;i<7;i++) write_pixel(x+i, y+3, WHITE);
			for(i=2;i<5;i++) write_pixel(x+i, y+4, WHITE);
			for(i=2;i<5;i++) write_pixel(x+i, y+5, WHITE);
			for(i=2;i<5;i++) write_pixel(x+i, y+6, WHITE);
			break;
		case 9: //upright
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
    	if (i==8 || j==8)
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
	for (int i=0;i<5;i++)
		for (int j=0;j<5;j++)
			write_pixel(x+2+i, y+2+j, color);
  for (int i=0;i<3;i++){
  	write_pixel(x+3+i, y+1  , color);
  	write_pixel(x+3+i, y+7  , color);
  	write_pixel(x+1  , y+3+i, color);
  	write_pixel(x+7  , y+3+i, color);
  }
}

void fill_area(int x, int y, int width, int height, char color){
	for (int i=0; i<height; i++)
		for (int j=0; j<width; j++)
			write_pixel(x+j, y+i, color);
}

void draw_board(){
	int i, j;
	for (i=-1; i<13; i++){
		for (j=-1; j<16; j++){
			if (i==-1 || i==12 || j==-1 || j==15){
				draw_block(j*9+159, i*9+39);
				continue;
			}
			switch(board[i][j]){
				case block:
					draw_block(j*9+159, i*9+39);
					break;
				case stop:
					draw_stop(j*9+159, i*9+39);
					break;
				case bomb:
					write_text("X", j*9+160, i*9+40, RED, 0);
					break;
				case diamond:
					draw_diamond(j*9+159, i*9+39);
					break;
				case ball:
					if (stop_flag){
						draw_stop(j*9+159, i*9+39);
					}
					draw_ball(j*9+159, i*9+39, LIGHTGREEN);
					break;
			}
		}
	}
}

void move_ball(int xDirection, int yDirection){
	while (1){
		if(xPos+xDirection==15 || yPos+yDirection==12 || xPos+xDirection==-1 || yPos+yDirection==-1){
			// if ball encountered the boundary
			break;
		}else if(board[yPos+yDirection][xPos+xDirection]==block){
			//if ball encountered a block
			break;
		}else if (board[yPos+yDirection][xPos+xDirection]==stop){
			// if ball entered a stop
			if (stop_flag){
				board[yPos][xPos] = stop;
				fill_area(xPos*9+160, yPos*9+40, 7, 7, BLACK);
			}else{
				board[yPos][xPos] = empty;
				fill_area(xPos*9+159, yPos*9+39, 9, 9, BLACK);
			}
			stop_flag = 1;
			xPos += xDirection; yPos += yDirection;
			board[yPos][xPos] = ball;
			draw_ball(xPos*9+159, yPos*9+39, LIGHTGREEN);
			break;
		}else if (board[yPos+yDirection][xPos+xDirection]==bomb){
			// if ball hit a bomb
			if (stop_flag){
				board[yPos][xPos] = stop;
				fill_area(xPos*9+160, yPos*9+40, 7, 7, BLACK);
			}else{
				board[yPos][xPos] = empty;
				fill_area(xPos*9+159, yPos*9+39, 9, 9, BLACK);
			}
			xPos += xDirection; yPos += yDirection;
			board[yPos][xPos] = ball;
			fill_area(xPos*9+159, yPos*9+39, 9, 9, BLACK);
			draw_ball(xPos*9+159, yPos*9+39, RED);
			stop_flag = 0;
			dead = 1;
			break;
		}else if (board[yPos+yDirection][xPos+xDirection]==diamond){
			fill_area((xPos+xDirection)*9+159, (yPos+yDirection)*9+39, 9, 9, BLACK);
			num_diamonds += 1;
			write_counter(235, 160);
		}

		if (stop_flag){
			board[yPos][xPos] = stop;
			fill_area(xPos*9+160, yPos*9+40, 7, 7, BLACK);
		}else{
			board[yPos][xPos] = empty;
			fill_area(xPos*9+159, yPos*9+39, 9, 9, BLACK);
		}

		xPos += xDirection; yPos += yDirection;
		stop_flag = 0;
		board[yPos][xPos] = ball;
		draw_ball(xPos*9+159, yPos*9+39, LIGHTGREEN);
		delay(2);
	}
}
