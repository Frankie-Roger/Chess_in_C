#ifndef CHESS_H
#define CHESS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define MAXPM (8*8)/2 // MAX Possibile moves

enum chessID {
    pawn,  // 0
    king,  // 1
    queen, // 2
    bishop,// 3
    knight,// 4
    rook   // 5
}; typedef enum chessID ID;

struct chess_piece {
    _Bool black;  // O white ; 1 black
    ID type;
}; typedef struct chess_piece Piece;

struct chess_player {
    _Bool color;
    Piece pieces[16];
}; typedef struct chess_player Player;

struct chess_board {
    Piece * body[8][8];
}; typedef struct chess_board Board;

void init_piece(Piece * p, _Bool color, ID id);
void init_player(Player * p, _Bool color);
void init_board(Board * b, Player * white, Player * black);

void print_board(Board * b);
void print_score(Board * b);

int letter_to_number(char letter);
_Bool check_format(int l, int n);
void reset_pmoves(int moves[][2], int size);
void board_copy(Board * original, Board * copy);

int possible_moves(Board * board, int moves[MAXPM][2], int l, int n, _Bool is_black, int start);

int pm_pawn(Board * board, int moves[MAXPM][2], int l, int n, _Bool is_black, int start);
int pm_king(Board * board, int moves[MAXPM][2], int l, int n, _Bool is_black, int start);
int pm_rook(Board * board, int moves[MAXPM][2], int l, int n, _Bool is_black, int start);
int pm_bishop(Board * board, int moves[MAXPM][2], int l, int n, _Bool is_black, int start);
int pm_queen(Board * board, int moves[MAXPM][2], int l, int n, _Bool is_black, int start);
int pm_knight(Board * board, int moves[MAXPM][2], int l, int n, _Bool is_black, int start);

void update_castling(int l1, int n1);
_Bool castling(Board * board, int l1, int n1, int l2, int n2, _Bool black);

void pawn_promotes(Board * board, int l, int n);

_Bool check_en_passant(Board * board, int l1, int n1, int l2, int n2, int en_passant_from[][2], int en_passant_to[2],
    int * n_enp, _Bool opponent_color);
_Bool en_passant(Board * board, int l1, int n1, int l2, int n2, int en_passant_from[][2], int en_passant_to[2],
    char * move1, char * move2, int n_enp, _Bool black);

_Bool check_if_on_check(Board * board, _Bool black, int pmoves[][2], _Bool opponent);
_Bool checkmate_check(Board * board, Board * copy_board,
    int pmoves1[MAXPM][2], int pmoves2[MAXPM][2], _Bool opponent_color);
_Bool stalemate(Board * board, _Bool color, int pmoves[][2]);

#endif //CHESS_H
