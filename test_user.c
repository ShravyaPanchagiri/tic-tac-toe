#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(){

	int fd = open("/dev/tictactoe", O_RDWR);
	if(fd <0)
	{
		perror("open");
		return -1;
	}

	char buffer[100];
	read(fd, buffer, sizeof(buffer));
	printf("%s\n",buffer);

	//play moves(simulate 2 players)
	write(fd,"X0",2);
	write(fd,"04",2);
	write(fd,"X1",2);
	write(fd,"08",2);

	read(fd, buffer, sizeof(buffer));
	printf("%s\n", buffer);

	close(fd);
	return 0;

}