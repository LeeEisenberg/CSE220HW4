#include "hw4.h"

void initialize_game(ChessGame *game) {
    // strcpy(game->chessboard[0], "rnbqkbnr");
    // strcpy(game->chessboard[1], "pppppppp");
    // strcpy(game->chessboard[2], "........");
    // strcpy(game->chessboard[3], "........");
    // strcpy(game->chessboard[4], "........");
    // strcpy(game->chessboard[5], "........");
    // strcpy(game->chessboard[6], "PPPPPPPP");
    // strcpy(game->chessboard[7], "RNBQKBNR");
    game->chessboard[0][0] = 'r';
    game->chessboard[0][1] = 'n';
    game->chessboard[0][2] = 'b';
    game->chessboard[0][3] = 'q';
    game->chessboard[0][4] = 'k';
    game->chessboard[0][5] = 'b';
    game->chessboard[0][6] = 'n';
    game->chessboard[0][7] = 'r';
    game->chessboard[7][0] = 'R';
    game->chessboard[7][1] = 'N';
    game->chessboard[7][2] = 'B';
    game->chessboard[7][3] = 'Q';
    game->chessboard[7][4] = 'K';
    game->chessboard[7][5] = 'B';
    game->chessboard[7][6] = 'N';
    game->chessboard[7][7] = 'R';
    for (int i = 0; i < 8; i++){
        game->chessboard[1][i] = 'p';
        game->chessboard[6][i] = 'P';
        for (int j = 2; j < 6; j++){
            game->chessboard[j][i] = '.';
        }
    }
    game->moveCount = 0;
    game->capturedCount = 0;
    game->currentPlayer = WHITE_PLAYER;
}

void chessboard_to_fen(char fen[], ChessGame *game) {
    int index = 0;
    int count = 0;
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            if (game->chessboard[i][j] != '.'){
                if (count != 0){
                    fen[index] = count + '0';
                    index++;
                    count = 0;
                }
                fen[index] = game->chessboard[i][j];
                index++;
            } else{
                count++;
            }
        }
        if (count != 0){
            fen[index] = count + '0';
            index++;
            count = 0;
        }
        fen[index] = '/';
        index++;
    }
    fen[index-1] = ' ';
    fen[index] = (game->currentPlayer == WHITE_PLAYER ? 'w' : 'b');
    fen[index+1] = '\0';
    (void)fen;
    (void)game;
}

bool is_valid_pawn_move(char piece, int src_row, int src_col, int dest_row, int dest_col, ChessGame *game) {
    int direction = (piece == 'p' ? 1 : -1);
    if (dest_row - src_row == direction){
        if (src_col != dest_col){ //capture
            if (game->chessboard[dest_row][dest_col] == '.'){
                return false;
            }
            if ((src_col - dest_col) * (src_col - dest_col) != 1){
                return false;
            }
        } else{ //not capture
            if (game->chessboard[dest_row][dest_col] != '.'){
                return false;
            }
        }
    } else if (dest_row - src_row == direction * 2){ //double move
        if (src_col != dest_col){
            return false;
        }
        if (game->chessboard[src_row+direction][src_col] != '.' || game->chessboard[src_row+(2*direction)][src_col] != '.'){
            return false;
        }
        if (src_row != (direction == 1 ? 1 : 6)){
            return false;
        }
    } else{
        return false;
    }
    (void)piece;
    (void)src_row;
    (void)src_col;
    (void)dest_row;
    (void)dest_col;
    (void)game;
    return true;
}

bool is_valid_rook_move(int src_row, int src_col, int dest_row, int dest_col, ChessGame *game) {
    if (src_row == dest_row){
        for (int i = (src_col < dest_col ? src_col+1 : src_col-1); i != dest_col; (src_col < dest_col ? i++ : i--)){
            if (game->chessboard[src_row][i] != '.'){
                return false;
            }
        }
    } else if (src_col == dest_col){
        for (int i = (src_row < dest_row ? src_row+1 : src_row-1); i != dest_row; (src_row < dest_row ? i++ : i--)){
            if (game->chessboard[i][src_col] != '.'){
                return false;
            }
        }
    } else{
        return false;
    }
    (void)src_row;
    (void)src_col;
    (void)dest_row;
    (void)dest_col;
    (void)game;
    return true;
}

bool is_valid_knight_move(int src_row, int src_col, int dest_row, int dest_col) {
    if ((src_row - dest_row) * (src_row - dest_row) + (src_col - dest_col) * (src_col - dest_col) != 5){
        return false;
    }
    (void)src_row;
    (void)src_col;
    (void)dest_row;
    (void)dest_col;
    return true;
}

bool is_valid_bishop_move(int src_row, int src_col, int dest_row, int dest_col, ChessGame *game) {
    int idir = (dest_row - src_row > 0 ? 1 : -1);
    int jdir = (dest_col - src_col > 0 ? 1 : -1);
    int i = src_row + idir;
    int j = src_col + jdir;
    if ((src_row - dest_row) * (src_row - dest_row) != (src_col - dest_col) * (src_col - dest_col)){
        return false;
    }
    while (i != dest_row && j != dest_col){
        if (game->chessboard[i][j] != '.'){
            return false;
        }
        i += idir;
        j += jdir;
    }
    (void)src_row;
    (void)src_col;
    (void)dest_row;
    (void)dest_col;
    (void)game;
    return true;
}

bool is_valid_queen_move(int src_row, int src_col, int dest_row, int dest_col, ChessGame *game) {
    return (is_valid_bishop_move(src_row, src_col, dest_row, dest_col, game) || is_valid_rook_move(src_row, src_col, dest_row, dest_col, game));
}

bool is_valid_king_move(int src_row, int src_col, int dest_row, int dest_col) {
    if ((src_row - dest_row) * (src_row - dest_row) + (src_col - dest_col) * (src_col - dest_col) > 2){
        return false;
    }
    (void)src_row;
    (void)src_col;
    (void)dest_row;
    (void)dest_col;
    return true;
}

bool is_valid_move(char piece, int src_row, int src_col, int dest_row, int dest_col, ChessGame *game) {
    switch(piece){
        case 'P':
        case 'p':
            return is_valid_pawn_move(piece, src_row, src_col, dest_row, dest_col, game);
        case 'R':
        case 'r':
            return is_valid_rook_move(src_row, src_col, dest_row, dest_col, game);
        case 'N':
        case 'n':
            return is_valid_knight_move(src_row, src_col, dest_row, dest_col);
        case 'B':
        case 'b':
            return is_valid_bishop_move(src_row, src_col, dest_row, dest_col, game);
        case 'Q':
        case 'q':
            return is_valid_queen_move(src_row, src_col, dest_row, dest_col, game);
        case 'K':
        case 'k':
            return is_valid_king_move(src_row, src_col, dest_row, dest_col);
        default:
            return false;
    }
    (void)piece;
    (void)src_row;
    (void)src_col;
    (void)dest_row;
    (void)dest_col;
    (void)game;
    return false;
}

void fen_to_chessboard(const char *fen, ChessGame *game) {
    int index = 0;
    int spaces = 0;
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            if (spaces > 0){
                game->chessboard[i][j] = '.';
                spaces--;
            } else{
                if (fen[index] < '9' && fen[index] > '0'){ //its a number
                    spaces = fen[index] - '0';
                    game->chessboard[i][j] = '.';
                    spaces--;
                } else{
                    game->chessboard[i][j] = fen[index];
                }
            }
            if (spaces == 0){
                index++;
            }
        }
        index++;
    }
    game->moveCount = 0;
    game->capturedCount = 0;
    if (fen[index] == 'w'){
        game->currentPlayer = WHITE_PLAYER;
    } else{
        game->currentPlayer = BLACK_PLAYER;
    }
    (void)fen;
    (void)game;
}

int parse_move(const char *move, ChessMove *parsed_move) {
    if (strlen(move) != 4 && strlen(move) != 5){
        return PARSE_MOVE_INVALID_FORMAT;
    }
    if (move[0] < 'a' || move[0] > 'h' || move[2] < 'a' || move[2] > 'h'){
        return PARSE_MOVE_INVALID_FORMAT;
    }
    if (move[1] < '1' || move[1] > '8' || move[3] < '1' || move[3] > '8'){
        return PARSE_MOVE_OUT_OF_BOUNDS;
    }
    if (strlen(move) == 5 && move[3] != '1' && move[3] != '8'){
        return PARSE_MOVE_INVALID_DESTINATION;
    }
    if (strlen(move) == 5 && move[4] != 'q' && move[4] != 'r' && move[4] != 'b' && move[4] != 'n'){
        return PARSE_MOVE_INVALID_PROMOTION;
    }
    parsed_move->startSquare[0] = move[0];
    parsed_move->startSquare[1] = move[1];
    parsed_move->startSquare[2] = '\0';
    parsed_move->endSquare[0] = move[2];
    parsed_move->endSquare[1] = move[3];
    if (strlen(move) == 5){
        parsed_move->endSquare[2] = move[4];
        parsed_move->endSquare[3] = '\0';
    } else{
        parsed_move->endSquare[2] = '\0';
    }
    (void)move;
    (void)parsed_move;
    return 0;
}

int make_move(ChessGame *game, ChessMove *move, bool is_client, bool validate_move) {//assumes client is always white and goes first
    //display_chessboard(game);
    int startRow = '8' - move->startSquare[1];
    int startCol = move->startSquare[0] - 'a';
    int endRow = '8' - move->endSquare[1];
    int endCol = move->endSquare[0] - 'a';
    if (validate_move){
        if (is_client == game->moveCount % 2){ //try changing this
            return MOVE_OUT_OF_TURN;
        }
        if (game->chessboard[startRow][startCol] == '.'){
            return MOVE_NOTHING;
        }
        if ((!is_client && game->chessboard[startRow][startCol] < 'Z' && game->chessboard[startRow][startCol] > 'A') || (is_client && game->chessboard[startRow][startCol] > 'a' && game->chessboard[startRow][startCol] < 'z')){
            return MOVE_WRONG_COLOR;
        }
        if ((is_client && game->chessboard[endRow][endCol] < 'Z' && game->chessboard[endRow][endCol] > 'A') || (!is_client && game->chessboard[endRow][endCol] > 'a' && game->chessboard[endRow][endCol] < 'z')){
            return MOVE_SUS;
        }
        if (game->chessboard[startRow][startCol] != 'p' && game->chessboard[startRow][startCol] != 'P' && strlen(move->endSquare) == 3){
            return MOVE_NOT_A_PAWN;
        }
        if (((game->chessboard[startRow][startCol] == 'p' && endRow == 7) || (game->chessboard[startRow][startCol] == 'P' && endRow == 0)) && strlen(move->endSquare) != 3){
            return MOVE_MISSING_PROMOTION;
        }
        if (!is_valid_move(game->chessboard[startRow][startCol], startRow, startCol, endRow, endCol, game)){
            return MOVE_WRONG;
        }
    }
    if (game->chessboard[endRow][endCol] != '.'){
        game->capturedPieces[game->capturedCount] = game->chessboard[endRow][endCol];
        game->capturedCount += 1;
    }
    game->chessboard[endRow][endCol] = game->chessboard[startRow][startCol];
    game->chessboard[startRow][startCol] = '.';
    if ((is_client && endRow == 0 && game->chessboard[endRow][endCol] == 'P') || (!is_client && endRow == 7 && game->chessboard[endRow][endCol] == 'p')){ //promote
        game->chessboard[endRow][endCol] += (move->endSquare[2] - 'p');
    }
    game->moves[game->moveCount] = *move;
    game->moveCount += 1;
    game->currentPlayer = !game->currentPlayer;
    (void)game;
    (void)move;
    (void)is_client;
    (void)validate_move;
    return 0;
}

int send_command(ChessGame *game, const char *message, int socketfd, bool is_client) {
    char *message2 = malloc(strlen(message)+1);
    strcpy(message2, message);
    ChessMove *move = malloc(sizeof(ChessMove));
    if (!strncmp(message, "/move ", 6)){
        if (parse_move(&message[6], move) != 0){
            return COMMAND_ERROR;
        }
        if (make_move(game, move, is_client, true) != 0){
            return COMMAND_ERROR;
        }
        send(socketfd, message, sizeof(message), 0);
        return COMMAND_MOVE;
    } else if (!strncmp(message, "/forfeit", 8)){
        send(socketfd, message, sizeof(message), 0);
        close(socketfd);
        return COMMAND_FORFEIT;
    } else if (!strncmp(message, "/chessboard", 11)){
        display_chessboard(game);
        return COMMAND_DISPLAY;
    } else if (!strncmp(message, "/import ", 8)){ //what goes in the if statement???
        if (is_client == false){
            fen_to_chessboard(&message[8], game);
            send(socketfd, message, sizeof(message), 0);
            return COMMAND_IMPORT;
        } else{
            return COMMAND_ERROR;
        }
    } else if (!strncmp(message, "/load ", 6)){
        char *token = strtok(message2, " ");
        token = strtok(NULL, " ");
        if (token == NULL){
            return COMMAND_ERROR;
        }
        char *username = malloc(strlen(token)+1);
        strcpy(username, token);
        token = strtok(NULL, " ");
        if (token == NULL){
            return COMMAND_ERROR;
        }
        char *num = malloc(strlen(token)+1);
        strcpy(num, token);
        int n = strtol(num, &token, 10);
        if (load_game(game, username, "game_database.txt", n) != 0){
            return COMMAND_ERROR;
        }
        send(socketfd, message, sizeof(message), 0);
        return COMMAND_LOAD;
    } else if (!strncmp(message, "/save ", 6)){
        char *token = strtok(message2, " ");
        token = strtok(NULL, " ");
        if (token == NULL){
            return COMMAND_ERROR;
        }
        if (save_game(game, token, "game_database.txt") != 0){
            return COMMAND_ERROR;
        }
        return COMMAND_SAVE;
    } else{
        return COMMAND_UNKNOWN;
    }
    (void)game;
    (void)message;
    (void)socketfd;
    (void)is_client;
    return -777;
}

int receive_command(ChessGame *game, const char *message, int socketfd, bool is_client) {
    char *message2 = malloc(strlen(message)+1);
    strcpy(message2, message);
    ChessMove *move = malloc(sizeof(ChessMove));
    if (!strncmp(message, "/move ", 6)){
        if (parse_move(&message[6], move) != 0){
            return COMMAND_ERROR;
        }
        if (make_move(game, move, is_client, false) != 0){
            return COMMAND_ERROR;
        }
        send(socketfd, message, sizeof(message), 0);
        return COMMAND_MOVE;
    } else if (!strncmp(message, "/forfeit", 8)){
        close(socketfd);
        return COMMAND_FORFEIT;
    } else if (!strncmp(message, "/import ", 8)){
        if (is_client == true){
            fen_to_chessboard(&message[8], game);
            send(socketfd, message, sizeof(message), 0);
            return COMMAND_IMPORT;
        } else{
            return COMMAND_ERROR;
        }
    } else if (!strncmp(message, "/load ", 6)){
        char *token = strtok(message2, " ");
        token = strtok(NULL, " ");
        if (token == NULL){
            return COMMAND_ERROR;
        }
        char *username = malloc(strlen(token)+1);
        strcpy(username, token);
        token = strtok(NULL, " ");
        if (token == NULL){
            return COMMAND_ERROR;
        }
        char *num = malloc(strlen(token)+1);
        strcpy(num, token);
        int n = strtol(num, &token, 10);
        if (load_game(game, username, "game_database.txt", n) != 0){
            return COMMAND_ERROR;
        }
        send(socketfd, message, sizeof(message), 0);
        return COMMAND_LOAD;
    } else{
        return -1;
    }
    (void)game;
    (void)message;
    (void)socketfd;
    (void)is_client;
    return -888;
}

int save_game(ChessGame *game, const char *username, const char *db_filename) {
    for (int i = 0; i < (int) strlen(username); i++){
        if (username[i] == ' '){
            return -1;
        }
    }
    FILE *output = fopen(db_filename, "a");
    char fen[100];
    chessboard_to_fen(fen, game);
    fprintf(output, "%s:%s\n", username, fen);
    fclose(output);
    (void)game;
    (void)username;
    (void)db_filename;
    return 0;
}

int load_game(ChessGame *game, const char *username, const char *db_filename, int save_number) {
    if (save_number < 1){
        return -1;
    }
    FILE *input = fopen(db_filename, "r");
    if (input == NULL){
        return -1;
    }
    size_t l = 0;
    char *line = NULL;
    int read = getline(&line, &l, input);
    while (read != -1){
        if (strncmp(line, username, strlen(username)) == 0){
            save_number--;
        }
        if (save_number == 0){
            fen_to_chessboard(&line[strlen(username) + 1], game);
            break;
        }
        read = getline(&line, &l, input);
    }
    fclose(input);
    if (save_number > 0){
        return -1;
    }
    (void)game;
    (void)username;
    (void)db_filename;
    (void)save_number;
    return 0;
}

void display_chessboard(ChessGame *game) {
    printf("\nChessboard:\n");
    printf("  a b c d e f g h\n");
    for (int i = 0; i < 8; i++) {
        printf("%d ", 8 - i);
        for (int j = 0; j < 8; j++) {
            printf("%c ", game->chessboard[i][j]);
        }
        printf("%d\n", 8 - i);
    }
    printf("  a b c d e f g h\n");
}
