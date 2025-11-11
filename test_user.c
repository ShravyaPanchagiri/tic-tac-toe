#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define BUF_SIZE 200

int main(){

	int fd = open("/dev/tictactoe", O_RDWR);
	if(fd <0)
	{
		perror("open");
		return -1;
	}

	char buffer[BUF_SIZE];
	char move[3];
	int ret;
	printf("Tic-Tac-Toe Game Started!\n");
	while(1)
	{
		ret= read(fd, buffer, sizeof(buffer)-1);
		if(ret < 0){
			perror("read");
			break;
		}
		buffer[ret] = '\0';
		printf("%s\n",buffer);

		if(strstr(buffer, "Game Over")){
			printf("Type 'reset' to start a new game or 'q' to quit:");
			scanf("%s",move);

			if(strcmp(move, "reset") == 0) {
				write(fd,move,strlen(move));
				continue;
			}
			else
				break;
		}

		printf("Enter move(format X0,04,etc.) or q to quit:");
		scanf("%2s",move);

		if(move[0] == 'q' || move[0] == 'Q')
		break;

		ret = write(fd,move,2);
		if(ret <0) {
			perror("write");
			printf("Invalid move! Try again\n");
		}
	}
	close(fd);
	printf("Game ended.\n");
	return 0;


}