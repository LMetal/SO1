#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {

    int simpleSocket = 0;
    int simplePort = 0;
    int returnStatus = 0;
    char buffer[256] = "";
    struct sockaddr_in simpleServer;
    char x_string[10];
    char string[256];


    if (3 != argc) {

        fprintf(stderr, "Usage: %s <server> <port>\n", argv[0]);
        exit(1);

    }

    /* create a streaming socket      */
    simpleSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (simpleSocket == -1) {

        fprintf(stderr, "Could not create a socket!\n");
        exit(1);

    }
    else {
      fprintf(stderr, "Socket created!\n");
    }

    /* retrieve the port number for connecting */
    simplePort = atoi(argv[2]);

    /* setup the address structure */
    /* use the IP address sent as an argument for the server address  */
    //bzero(&simpleServer, sizeof(simpleServer));
    memset(&simpleServer, '\0', sizeof(simpleServer));
    simpleServer.sin_family = AF_INET;
    //inet_addr(argv[2], &simpleServer.sin_addr.s_addr);
    simpleServer.sin_addr.s_addr=inet_addr(argv[1]);
    simpleServer.sin_port = htons(simplePort);

    /*  connect to the address and port with our socket  */
    returnStatus = connect(simpleSocket, (struct sockaddr *)&simpleServer, sizeof(simpleServer));

    if (returnStatus == 0) {
      fprintf(stderr, "Connect successful!\n");
    }
    else {
      fprintf(stderr, "Could not connect to address!\n");
      close(simpleSocket);
      exit(1);
    }

    fprintf(stderr, "\nQuante stringe: ");
    scanf("%s", x_string);

    write(simpleSocket, x_string, strlen(x_string));


    for(int i=0; i<atoi(x_string); i++){
        fprintf(stderr, "\nInserire stringa %d: ", i);
        scanf("%s", string);

        write(simpleSocket, string, sizeof(string));

        /* get the message from the server   */
        returnStatus = read(simpleSocket, buffer, sizeof(buffer));

        if ( returnStatus > 0 ) {
            printf("FROM SERVER -> %d: %s\n\n", returnStatus, buffer);
        } else {
            fprintf(stderr, "Return Status = %d \n", returnStatus);
        }

    }



    close(simpleSocket);

    return 0;

}
