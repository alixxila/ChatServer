#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>

typedef struct User
{
	char nom[30];
	int age;
}User;


void *function(void *arg)
{
	int socket = *(int*)arg;
	const char msg[] = "quel est votre nom et votre age ?";
	User user;
	send(socket, msg, strlen(msg)+1,0);
	recv(socket, &user, sizeof(user), 0);
	printf("Vous êtes %s et vous avez %d\n", user.nom, user.age);
	close(socket);
	free(arg);
	pthread_exit(NULL);
}


int main(void)
{
	//Creation du Socket
	int socketServer = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in addrServer;

	//Configuration de l'addresse du serveur
	addrServer.sin_addr.s_addr = inet_addr("127.0.0.1");
	addrServer.sin_family = AF_INET;

	//Configuration de l'addresse du port
	addrServer.sin_port = htons(30000);

	//Lier le socket avec le port
	bind(socketServer, (const struct sockaddr*)&addrServer, sizeof(addrServer));
	printf("bind : %d\n", socketServer);

	//5 connexions peuvent être mises en attente au maximum
	listen(socketServer, 5);
	printf("listen\n");

	pthread_t threads[3];

	for(int i = 0; i < 3; i++)
	{
		//Nous acceptons les connexions au client + création du socket
		struct sockaddr_in addrClient;
		socklen_t csize = sizeof(addrClient);
		int socketClient = accept(socketServer, (struct sockaddr*)&addrClient, &csize);
		printf("Accept\n");

		printf("Client: %d\n", socketClient);

		int *arg = malloc(sizeof(int));
		*arg = socketClient;
		pthread_create(&threads[i], NULL, function, arg);
	}

	for(int i = 0; i < 3; i++)
	{
		pthread_join(threads[i], NULL);
	}

	close(socketServer);
	printf("close\n");

	return 0;

}