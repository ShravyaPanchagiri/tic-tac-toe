#ifndef TICTACTOE_H
#define TICTACTOE_H

//Necessary kernel headers for synchronization primitives
#include <linux/spinlock.h>
#include <linux/wait.h>

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

	//Concurrency Primitive: Spinlock protects the data above from race conditions.
	spinlock_t lock;

	//Communication Primitive: Wait queue for blocking/unblocking players for turn enforcement.
	wait_queue_head_t wq;

}ttt_game_t;

#endif