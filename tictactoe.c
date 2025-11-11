#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include "tictactoe.h"

#define DEVICE_NAME "tictactoe"
#define BUF_SIZE 100

static int major;
static ttt_game_t game;

//Helper function to check game status
static void check_game_status(ttt_game_t *game){
	int wins[8][3] = {
		{0,1,2},{3,4,5},{6,7,8},//rows
		{0,3,6},{1,4,7},{2,5,8},//cols
		{0,4,8},{2,4,6}  //diagonals
	};

	game->game_over = 0;
	game->winner = EMPTY;

	for(int i=0;i<8;i++){
		int a=wins[i][0], b=wins[i][1], c=wins[i][2];
		if(game->board[a]!= EMPTY &&
			game->board[a] == game->board[b] &&
			game->board[b] == game->board[c]){
			game->winner = game->board[a];
			game->game_over = 1;
			return;
		}
	}
	int draw=1;
	for(int i=0;i<BOARD_SIZE;i++){
		if(game->board[i] == EMPTY){
			draw=0;
			break;
		}
	}
	if(draw){
		game->game_over=1;
		game->winner= EMPTY;
	}
}

static int ttt_open(struct inode *inode, struct file *file){
	printk(KERN_INFO "Tictactoe device opened\n");
	return 0;
}

static int ttt_release(struct inode *inode, struct file *file){
	printk(KERN_INFO "Tictactoe device closed\n");
	return 0;
}

static ssize_t ttt_read(struct file *file, char __user *buf,size_t len, loff_t *off){
	char kbuf[BUF_SIZE];
	int i, pos=0;

	pos+=snprintf(kbuf+pos,BUF_SIZE-pos, "\nBoard:\n");
	for(i=0;i<BOARD_SIZE;i++){
		pos+=snprintf(kbuf+pos, BUF_SIZE-pos, "[%c]",game.board[i]);
		if((i+1)%3 ==0)
			pos+= snprintf(kbuf+pos, BUF_SIZE-pos,"\n");
	}

	if(game.game_over){
		if(game.winner == EMPTY)
			pos += snprintf(kbuf+pos,BUF_SIZE-pos,"Game Over: Draw!\n");
		else
			pos+= snprintf(kbuf+pos, BUF_SIZE-pos,"Game Over! Winner:%c\n",game.winner);
	}
	else 
		pos+= snprintf(kbuf+pos,BUF_SIZE-pos,"Turn: %c\n",game.turn);

	if(copy_to_user(buf, kbuf, pos))
		return -EFAULT;

	*off+= pos;
	return pos;
} 

static ssize_t ttt_write(struct file *file, const char __user *buf,size_t len,loff_t *off){
	char kbuf[BUF_SIZE];
	int index;
	char player;


	if(len >BUF_SIZE-1)
		return -EINVAL;
	if(copy_from_user(kbuf,buf,len))
		return -EFAULT;
	kbuf[len] = '\0';

//check for reset command
	if(strncmp(kbuf,"reset",5)==0){
		int i;
		for(i=0;i<BOARD_SIZE;i++)
			game.board[i]=EMPTY;
		game.turn=PLAYER_X;
		game.game_over=0;
		printk(KERN_INFO "TicTacToe: Game reset\n");
		return len;
	}
	if(game.game_over)
		return -EINVAL; // No moves after game over

	//Input format: "X0" or "04"
	if(sscanf(kbuf,"%c%d",&player,&index)!=2)
		return -EINVAL;
	if(index <0 || index >= BOARD_SIZE)
		return -EINVAL;
	if(game.board[index]!= EMPTY)
		return -EINVAL;
	if(player != game.turn)
		return -EINVAL;

	game.board[index] = player;
	check_game_status(&game);
	if(!game.game_over)
		game.turn = (game.turn == PLAYER_X) ? PLAYER_O :PLAYER_X;

	return len;
}

static struct file_operations fops = {
				.owner = THIS_MODULE,
				.open = ttt_open,
				.release = ttt_release,
				.read = ttt_read,
				.write = ttt_write,
};

static int __init ttt_init(void){
	int i;
	major = register_chrdev(0, DEVICE_NAME, &fops);
	if(major <0){
		printk(KERN_ALERT "Failed to register device\n");
		return major;
	}
	for(i=0;i<BOARD_SIZE;i++)
		game.board[i]= EMPTY;
	game.turn = PLAYER_X;
	game.game_over=0;
	game.winner=EMPTY;
	printk(KERN_INFO "Tictactoe module loaded, major=%d\n",major);
	return 0;
}

static void __exit ttt_exit(void){
	unregister_chrdev(major,DEVICE_NAME);
	printk(KERN_INFO "Tictactoe module unloaded\n");
}

module_init(ttt_init);
module_exit(ttt_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Shravya Panchagiri");
MODULE_DESCRIPTION("Tic-Tac-Toe Character Device Driver");