#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "hw4.h"

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    ChessGame game;
    int connfd = 0;
    struct sockaddr_in serv_addr;

    // Connect to the server
    if ((connfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        exit(EXIT_FAILURE);
    }

    if (connect(connfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connect failed");
        exit(EXIT_FAILURE);
    }

    initialize_game(&game);
    display_chessboard(&game);

    char buffer[BUFFER_SIZE];
    size_t l;
    char *line = NULL;
    int out;
    while (1) {
        while (out != COMMAND_ERROR && out != COMMAND_UNKNOWN){
            printf("Enter command: ");
            out = getline(&line, &l, stdin);
            out = send_command(&game, line, connfd, true);
        }
        if (out == COMMAND_FORFEIT){
            break;
        }
        read(connfd, buffer, BUFFER_SIZE);
        out = receive_command(&game, buffer, connfd, true);
        if (out == COMMAND_FORFEIT){
            break;
        }
    }

    // Please ensure that the following lines of code execute just before your program terminates.
    // If necessary, copy and paste it to other parts of your code where you terminate your program.
    FILE *temp = fopen("./fen.txt", "w");
    char fen[200];
    chessboard_to_fen(fen, &game);
    fprintf(temp, "%s", fen);
    fclose(temp);
    free(line);
    //close(connfd);
    exit(1);
}
