#include "hw4.h"

void initialize_game(ChessGame *game) {
    game = malloc(sizeof(ChessGame));
    *game->chessboard[0] = "rnbqkbnr";
    *game->chessboard[1] = "pppppppp";
    *game->chessboard[2] = "........";
    *game->chessboard[3] = "........";
    *game->chessboard[4] = "........";
    *game->chessboard[5] = "........";
    *game->chessboard[6] = "PPPPPPPP";
    *game->chessboard[7] = "RNBQKBNR";
    game->moveCount = 0;
    game->capturedCount = 0;
    game->currentPlayer = WHITE_PLAYER;
    return game;
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
    fen[index+1] = '\n';
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
        if (game->chessboard[src_row+1][src_col+1] != '.' || game->chessboard[src_row+2][src_col+2] != '.'){
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
        for (int i = src_col+1; i < dest_col; (src_col < dest_col ? i++ : i--)){
            if (game->chessboard[src_row][i] != '.'){
                return false;
            }
        }
    } else if (src_col == dest_col){
        for (int i = src_row+1; i < dest_row; (src_row < dest_row ? i++ : i--)){
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
            return NULL;
    }
    (void)piece;
    (void)src_row;
    (void)src_col;
    (void)dest_row;
    (void)dest_col;
    (void)game;
    return NULL;
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
                if (fen[index] < '9'){ //its a number
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
    if (strlen(move) == 5 && move[5] != 'q' && move[5] != 'r' && move[5] != 'b' && move[5] != 'n'){
        return PARSE_MOVE_INVALID_PROMOTION;
    }
    parsed_move->startSquare[0] = move[0];
    parsed_move->startSquare[1] = move[1];
    parsed_move->startSquare[2] = '\n';
    parsed_move->endSquare[0] = move[2];
    parsed_move->endSquare[1] = move[3];
    if (strlen(move) == 5){
        parsed_move->endSquare[2] = move[4];
        parsed_move->endSquare[3] = '\n';
    } else{
        parsed_move->endSquare[2] = '\n';
    }
    (void)move;
    (void)parsed_move;
    return 0;
}

int make_move(ChessGame *game, ChessMove *move, bool is_client, bool validate_move) {
    (void)game;
    (void)move;
    (void)is_client;
    (void)validate_move;
    return -999;
}

int send_command(ChessGame *game, const char *message, int socketfd, bool is_client) {
    (void)game;
    (void)message;
    (void)socketfd;
    (void)is_client;
    return -999;
}

int receive_command(ChessGame *game, const char *message, int socketfd, bool is_client) {
    (void)game;
    (void)message;
    (void)socketfd;
    (void)is_client;
    return -999;
}

int save_game(ChessGame *game, const char *username, const char *db_filename) {
    (void)game;
    (void)username;
    (void)db_filename;
    return -999;
}

int load_game(ChessGame *game, const char *username, const char *db_filename, int save_number) {
    (void)game;
    (void)username;
    (void)db_filename;
    (void)save_number;
    return -999;
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
