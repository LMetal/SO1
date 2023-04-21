#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

char MESSAGE[100];

int main(int argc, char *argv[]) {

    int simpleSocket = 0;
    int simplePort = 0;
    int returnStatus = 0;
    struct sockaddr_in simpleServer;
    char buffer[256];
    char number_of_string[10];

    if (2 != argc) {

        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(1);

    }

    simpleSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (simpleSocket == -1) {

        fprintf(stderr, "Could not create a socket!\n");
        exit(1);

    }
    else {
	    fprintf(stderr, "Socket created!\n");
    }

    /* retrieve the port number for listening */
    simplePort = atoi(argv[1]);

    /* setup the address structure */
    /* use INADDR_ANY to bind to all local addresses  */
    memset(&simpleServer, '\0', sizeof(simpleServer));
    simpleServer.sin_family = AF_INET;
    simpleServer.sin_addr.s_addr = htonl(INADDR_ANY);
    simpleServer.sin_port = htons(simplePort);

    /*  bind to the address and port with our socket  */
    returnStatus = bind(simpleSocket,(struct sockaddr *)&simpleServer,sizeof(simpleServer));

    if (returnStatus == 0) {
	    fprintf(stderr, "Bind completed!\n");
    }
    else {
        fprintf(stderr, "Could not bind to address!\n");
	close(simpleSocket);
	exit(1);
    }

    /* lets listen on the socket for connections      */
    returnStatus = listen(simpleSocket, 5);

    if (returnStatus == -1) {
        fprintf(stderr, "Cannot listen on socket!\n");
	close(simpleSocket);
        exit(1);
    }

    while (1)

    {

        struct sockaddr_in clientName = { 0 };
    	int simpleChildSocket = 0;
    	int clientNameLength = sizeof(clientName);

    	/* wait here */

      simpleChildSocket = accept(simpleSocket,(struct sockaddr *)&clientName, &clientNameLength);

    	if (simpleChildSocket == -1) {

                fprintf(stderr, "Cannot accept connections!\n");
    	    close(simpleSocket);
    	    exit(1);

    	}

      // print on terminal
      fprintf(stderr, "Incoming request from address: %s\n\n", inet_ntoa(clientName.sin_addr));

      read(simpleChildSocket, number_of_string, 10);
      fprintf(stderr, "Numstringhe: %d\n", atoi(number_of_string));

      for(int i=0; i<atoi(number_of_string); i++){
          read(simpleChildSocket, buffer, sizeof(buffer));

          /* write out our message to the client */
          write(simpleChildSocket, buffer, strlen(buffer));
      }


        close(simpleChildSocket);

    }

    return 0;

}
