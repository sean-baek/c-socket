/*

------------------------------------------
|                                        |
|     This code was written using C      |
|                                        |
|       Compile on Unix or Linux         |
|                                        |
|              2018.9.30                 |
------------------------------------------



This code creates a new file called test.txt, 

waits for a request from the client, 

and sends a connection message when it is connected to the client.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(char *message);

int main(int argc, char *argv[])
{
	int sock, clnt_sock, f;
	char message[] = "Hi! sean! HAHAHAHA\n\n";
	char message_[] = "Connected to server\n";
	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size;
	
	//���� ���� | Create socket
	sock = socket(PF_INET, SOCK_STREAM, 0);
	
	//���� ���� ���� | Create file socket
	f = open("test.txt" , O_CREAT|O_WRONLY|O_TRUNC);
	
	//���� f�� ���� ��ũ���Ͱ� -1�̸� ���� �޼��� ��� | If f's File descriptor is -1, print error message
	if(f == -1)
		error_handling("Can't creat file....");
	
	//���Ͽ� ���� �Է� | Write on file
	if(write(f, message, sizeof(message)) == -1)
		error_handling("Can't write to file!");
	
	if(argc != 2)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	
	//���� sock�� ���� ��ũ���Ͱ� -1�̸� ���� �޼��� ��� | If sock's File descriptor is -1, print error message
	if(sock == -1)
		error_handling("Not created socket....");
	
	//�����ǿ� ��Ʈ��ȣ�� ���� ����ü�� �ʱ�ȭ | Initialize the structure to hold the IP and port number
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));
	
	//���Ͽ� �����ǿ� ��Ʈ��ȣ �ο� | Give IP and port number to socket
	if(bind(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
			error_handling("Can't assign Ip and Port number!\n");
	
	//Ŭ���̾�Ʈ�� ��û�� ���� �� �ֵ��� ��� ���·� ���� | Wait until client's request comes in
	if(listen(sock, 5) == -1)
		error_handling("Can't listen of the client's request!\n");
	else
		printf("Watting the client....\n");
	
	//Ŭ���̾�Ʈ ���� | Response client's request
	clnt_addr_size = sizeof(clnt_addr);
	clnt_sock = accept(sock, (struct sockaddr*) &clnt_addr, &clnt_addr_size);
	
	//Ŭ���̾�Ʈ�� �������� ������ �� ���� �޼��� ��� | printf an error message when the client is not accepted
	if(clnt_sock == -1)
		error_handling("Can't response client's request!");
	
	//Ŭ���̾�Ʈ���� ���� ���� �޼��� ������ | Send a connection success message to the client
	write(clnt_sock, message_, sizeof(message_));

	//���ϵ� �ݱ� | Close sockets
	close(sock);
	close(clnt_sock);
	close(f);
	
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

