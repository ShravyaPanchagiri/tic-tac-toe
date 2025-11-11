#ifndef TICTACTOE_H
#define TICTACTOE_H

#define BOARD_SIZE 9

typedef enum{
	EMPTY = ' ',
	PLAYER_X = 'X',
	PLAYER_O = 'O'
}cell_t;

typedef struct{
	cell_t board[BOARD_SIZE];
	cell_t turn;
	int game_over;
	cell_t winner;
}ttt_game_t;

#endif