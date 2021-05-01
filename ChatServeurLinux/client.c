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

int main(void)
{

	//Creation du Socket
	int socketClient = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in addrClient;

	//Configuration de l'addresse du serveur
	addrClient.sin_addr.s_addr = inet_addr("127.0.0.1");
	addrClient.sin_family = AF_INET;

	//Configuration de l'addresse du port
	addrClient.sin_port = htons(30000);

	//Lier le socket avec le port
	connect(socketClient, (const struct sockaddr*)&addrClient, sizeof(addrClient));
	printf("Le sereur est connecté");

	//Prendre les infos de l'utilisateur 
	User user;
	char msg[33];
	recv(socketClient, msg, 32, 0);
	printf("%s\n", msg);
	scanf("%s %d", user.nom, &user.age);
	send(socketClient, &user, sizeof(user), 0);


	close(socketClient);

	return 0;
}