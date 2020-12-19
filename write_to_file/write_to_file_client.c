/*

------------------------------------------
|                                        |
|     This code was written using C      |
|                                        |
|       Compile on Unix or Linux         |
|                                        |
|              2018.9.30                 |
------------------------------------------



This code attempts to connect to the server, 

communicate with the server, 

read the contents of the file, 

and print it.


*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 100

void error_handling(char *message);

int main(int argc, char *argv[])
{
	int sock, str_len, f;
	char message[BUF_SIZE];
	char buf[BUF_SIZE];
	struct sockaddr_in serv_addr;
	
	if(argc != 3)
	{
		printf("Usage : %s <IP> <Port>\n", argv[0]);
		exit(1);
	}
	
	//socket ���� | Create socket
	sock = socket(PF_INET, SOCK_STREAM, 0);

	//socket ������ ���� ���� �޼��� ��� | If can't create a socket, print error message
	if(sock == -1)
		error_handling("Not created socket....");

	//�ּ� ������ ���� ����ü �ʱ�ȭ | Initialize the structure of the address information
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));
	
	//���� �õ� | Connection attempt
	if(connect(sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1)
		error_handling("couldn't send request!");

	//�����κ��� ���� ���� �޼��� �ޱ� | Receive a connection success message from the server
	str_len = read(sock, message, sizeof(message)-1);
	if(str_len == -1)
		error_handling("Can't read server's message");
	
	//�����κ��� ���� ���� ���� �޼��� ��� | Print connection success message from server
	printf("\nMessage from server : %s \n", message);


	//test.txt������ �б� ���� ���� | Open the test.txt in Read mode
    	f = open("test.txt", O_RDONLY);
	
	//���� ���� ���� �� ���� �޼��� ��� | 	If can't open the file, print error message
    	if(f == -1)
        	error_handling("Couldn't created file");

	//������ ���� �б� | Read the file's data
	if(read(f, buf, sizeof(buf)) == -1)
        	error_handling("Can't read from the server's file");

	printf("File data : %s", buf);
	
	//���� �ݱ� | Close sockets
	close(sock);
	close(f);

	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

