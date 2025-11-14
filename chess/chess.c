#include "chess.h"

int pawn_first_move_n;
int pawn_transf_n;

_Bool king_loop;
_Bool w_arrocco_right;
_Bool b_arrocco_right;
_Bool w_arrocco_left;
_Bool b_arrocco_left;

void init_piece(Piece * p, _Bool color, ID id) {
    p->black = color;
    p->type = id;
}

void init_player(Player * p, _Bool color) {
    p->color = color;
    int np = 0;
    init_piece(&p->pieces[0], color, king); np++;
    init_piece(&p->pieces[1], color, queen); np++;
    for (int i = 0; i < 8; i++) {   // from 2 to 9 pawns
        init_piece(&p->pieces[np], color, pawn); np++;
    }
    for (int i = 0; i < 2; i++) {   // 10 and 11 knights
        init_piece(&p->pieces[np], color, knight); np++;
    }
    for (int i = 0; i < 2; i++) {   // 12 and 13 bishops
        init_piece(&p->pieces[np], color, bishop); np++;
    }
    for (int i = 0; i < 2; i++) {   // 14 and 15 rooks
        init_piece(&p->pieces[np], color, rook); np++;
    }


}

void init_board(Board * b, Player * white, Player * black) {

    for (int l = 0; l < 8; l++) {
        for (int n = 0; n < 8; n++) {
            b->body[l][n] = NULL;

            if (n == 1) b->body[l][n] = &white->pieces[l+2];
            if (n == 6) b->body[l][n] = &black->pieces[l+2];

            if (n == 0) {
                if (l == 0) b->body[l][n] = &white->pieces[14];
                if (l == 1) b->body[l][n] = &white->pieces[10];
                if (l == 2) b->body[l][n] = &white->pieces[12];
                if (l == 3) {
                    if (white->color) {
                        b->body[l][n] = &white->pieces[0];
                    } else { b->body[l][n] = &white->pieces[1]; }
                }
                if (l == 4) {
                    if (white->color) {
                        b->body[l][n] = &white->pieces[1];
                    } else { b->body[l][n] = &white->pieces[0]; }
                }
                    if (l == 5) b->body[l][n] = &white->pieces[13];
                    if (l == 6) b->body[l][n] = &white->pieces[11];
                    if (l == 7) b->body[l][n] = &white->pieces[15];

            }

            if (n == 7) {
                if (l == 0) b->body[l][n] = &black->pieces[14];
                if (l == 1) b->body[l][n] = &black->pieces[10];
                if (l == 2) b->body[l][n] = &black->pieces[12];
                if (l == 3) {
                    if (black->color) {
                        b->body[l][n] = &black->pieces[1];
                    } else { b->body[l][n] = &black->pieces[0]; }
                }
                if (l == 4) {
                    if (black->color) {
                        b->body[l][n] = &black->pieces[0];
                    } else { b->body[l][n] = &black->pieces[1]; }
                }
                if (l == 5) b->body[l][n] = &black->pieces[13];
                if (l == 6) b->body[l][n] = &black->pieces[11];
                if (l == 7) b->body[l][n] = &black->pieces[15];
            }
            }
        }
    }

void print_board(Board * b) {

    char white_img[][4] = {{"♙\0"}, {"♔\0"}, {"♕\0"}, {"♗\0"}, {"♘\0"}, {"♖\0"}};
    char black_img[][4] = {{"♟\0"}, {"♚\0"}, {"♛\0"}, {"♝\0"}, {"♞\0"}, {"♜\0"}};

    printf("   "); for (int i = 0; i < 31; i++) printf("_"); printf("\n");
    for (int n = 7; n >= 0; n-- ) {
        printf(" %d|", n+1);
        for (int l = 0; l < 8; l++) {
            if (b->body[l][n] == NULL) {
                if (l%2 == n%2) printf(" _ |"); else printf("___|");
            } else {
                if (b->body[l][n]->black) {
                    if (l%2 == n%2) printf(" %s |", black_img[b->body[l][n]->type]);
                    else printf("_%s_|", black_img[b->body[l][n]->type]);
                } else {
                    if (l%2 == n%2) printf(" %s |", white_img[b->body[l][n]->type]);
                    else printf("_%s_|", white_img[b->body[l][n]->type]);
                }
            }
        }
        printf("\n");
    }
    printf("    A   B   C   D   E   F   G   H\n\n");
}

void print_score(Board * b) {
    int w_pawns = 0, w_queens = 0, w_bishops = 0, w_knights = 0, w_rooks = 0;
    int b_pawns = 0, b_queens = 0, b_bishops = 0, b_knights = 0, b_rooks = 0;

    for (int l = 0; l < 8; l++) {
        for (int n = 0; n < 8; n++) {
            if (b->body[l][n] != NULL) {
                if (b->body[l][n]->black) {
                    if (b->body[l][n]->type == 0) b_pawns++;
                    if (b->body[l][n]->type == 2) b_queens++;
                    if (b->body[l][n]->type == 3) b_bishops++;
                    if (b->body[l][n]->type == 4) b_knights++;
                    if (b->body[l][n]->type == 5) b_rooks++;
                } else {
                    if (b->body[l][n]->type == 0) w_pawns++;
                    if (b->body[l][n]->type == 2) w_queens++;
                    if (b->body[l][n]->type == 3) w_bishops++;
                    if (b->body[l][n]->type == 4) w_knights++;
                    if (b->body[l][n]->type == 5) w_rooks++;
                }
            }
        }
    }

        w_queens--; b_queens--;
        w_pawns = w_pawns - 8; b_pawns = b_pawns - 8;
        w_bishops = w_bishops - 2; b_bishops = b_bishops - 2;
        w_knights = w_knights - 2; b_knights = b_knights - 2;
        w_rooks = w_rooks - 2; b_rooks = b_rooks - 2;

        printf("SCORE :\n_____________________________________________________________\n");
        printf("White captured --> ");
        if (b_queens < 0) printf("♛ ");
        while (b_bishops < 0) { printf("♝ "); b_bishops++;}
        while (b_knights < 0) { printf("♞ "); b_knights++;}
        while (b_rooks < 0) { printf("♜︎ "); b_rooks++;}
        while (b_pawns < 0) { printf("♟︎ "); b_pawns++;}
        printf("\n              ------------------------------------------\n");
        printf("Black captured --> ");
        if (w_queens < 0) printf("♕ ");
        while (w_bishops < 0) { printf("♗ "); w_bishops++;}
        while (w_knights < 0) { printf("♘ "); w_knights++;}
        while (w_rooks < 0) { printf("♖︎ "); w_rooks++;}
        while (w_pawns < 0) { printf("♙︎ "); w_pawns++;}
        printf("\n_____________________________________________________________\n");

}

int letter_to_number(char letter) {
    if (65 <= letter && letter <= 72) return (letter - 65); // from A to H
    if (97 <= letter && letter <= 104) return (letter - 97);// from a to h
    return -1;
}

_Bool check_format(int l, int n) {
    if (l < 0 || l > 7 || n < 0 || n > 7 ) {
        printf("\nFormat is incorrect!\nOnly insert a letter from 'A' to 'H' and a number from 1 to 8.\nMake"
               " sure the you have spaces between the 'in' and the spots.The 'in' must be lower case\n\n");
        sleep(2); return 1;
    }
    return 0;
}

void reset_pmoves(int moves[][2], int size) {
    for (int i = 0; i < size; i++) {
        moves[i][0] = 0; moves[i][1] = 0;
    }
}

void board_copy(Board * original, Board * copy) {
    for (int i = 0; i < 8; i ++) {
        for (int j = 0; j < 8; j++) {
            copy->body[i][j] = original->body[i][j];
        }
    }
}

int possible_moves(Board * board, int moves[MAXPM][2], int l, int n, _Bool is_black, int start) {
    if (board->body[l][n] == NULL) return -1;
    if (board->body[l][n]->black != is_black) return -2;

    int kind = board->body[l][n]->type; int pm = 0;

    if (kind == 0) { // pawn
        pm = pm_pawn(board, moves, l, n, is_black, start);
    }
    if (kind == 1) { // king
        if (king_loop) return 0;
        pm = pm_king(board, moves, l, n, is_black, start);
    }
    if (kind == 2) { //queen
        pm = pm_queen(board, moves, l, n, is_black, start);
    }
    if (kind == 3) { // bishop
        pm = pm_bishop(board, moves, l, n, is_black, start);
    }
    if (kind == 4) { // knight
        pm = pm_knight(board, moves, l, n, is_black, start);
    }
    if (kind == 5) { // rook
        pm = pm_rook(board, moves, l, n, is_black, start);
    }

    return pm;
}

int pm_pawn(Board * board, int moves[MAXPM][2], int l, int n, _Bool is_black, int start)
{
    int i = start , spot1 = 0, spot2 = 0;
    if (is_black) {
        spot1 = n-1; spot2 = n-2;
    } else {
        spot1 = n+1; spot2 = n+2;
    }

    if (board->body[l][spot1] == NULL) {
        moves[i][0] = l; moves[i][1] = spot1; i++;
    }

    if (pawn_first_move_n == n) { // seconda avanzata
        if (board->body[l][spot2] == NULL && board->body[l][spot1] == NULL) {
            moves[i][0] = l;
            moves[i][1] = spot2; i++;
        }
    }

    if (l > 0 && board->body[l-1][spot1] != NULL) {
        if (board->body[l-1][spot1]->black != is_black) {
            moves[i][0] = l-1; moves[i][1] = spot1; i++;
        }
    }

    if (l < 7 && board->body[l+1][spot1] != NULL) {
        if (board->body[l+1][spot1]->black != is_black) {
            moves[i][0] = l+1; moves[i][1] = spot1; i++;
        }
    }

    return i;

}

int pm_king(Board * board, int moves[MAXPM][2], int l, int n, _Bool is_black, int start) {

    Board board_c;

    int moves2[MAXPM][2]; reset_pmoves(moves2, MAXPM);
    int i = 0, il, in;
    for (int l2 = -1; l2 < 2; l2++) {
        for (int n2 = -1; n2 < 2; n2++) {
            il = l + l2; in = n + n2;
            if (il >= 0 && il < 8 && in >= 0 && in < 8) {
                if (board->body[il][in] == NULL) {
                    moves2[i][0] = il; moves2[i][1] = in; i++;
                } else {
                    if (board->body[l][n]->black != board->body[il][in]->black) {
                        moves2[i][0] = il; moves2[i][1] = in; i++;
                    }
                }
            }
        }
    }

    int other_pmoves[8*8][2], om = 0; reset_pmoves(other_pmoves, 8*8);
    int ipos = start, imax = i;
    for (i = 0; i < imax; i++) {
        board_copy(board, &board_c);
        board_c.body[moves2[i][0]][moves2[i][1]] = board_c.body[l][n]; board_c.body[l][n] = NULL;
        if (!check_if_on_check(&board_c, is_black, other_pmoves, 0)) {
            moves[ipos][0] = moves2[i][0]; moves[ipos][1] = moves2[i][1]; ipos++;
        }
    }

    return ipos;
}

int pm_rook(Board * board, int moves[MAXPM][2], int l, int n, _Bool is_black, int start) {

    int up = n + 1, down = n - 1, left = l - 1, right = l + 1, i = start;

    while (up < 8) {
        if (board->body[l][up] != NULL) {
            if (board->body[l][up]->black != is_black) {
                moves[i][0] = l; moves[i][1] = up; i++;
            } break;
        }
        moves[i][0] = l; moves[i][1] = up; i++;
        up++;
    }

    while (down >= 0) {
        if (board->body[l][down] != NULL) {
            if (board->body[l][down]->black != is_black) {
                moves[i][0] = l; moves[i][1] = down; i++;
            } break;
        }
        moves[i][0] = l; moves[i][1] = down; i++;
        down--;
    }

    while (right < 8) {
        if (board->body[right][n] != NULL) {
            if (board->body[right][n]->black != is_black) {
                moves[i][0] = right; moves[i][1] = n; i++;
            } break;
        }
        moves[i][0] = right; moves[i][1] = n; i++;
        right++;
    }

    while (left >= 0) {
        if (board->body[left][n] != NULL) {
            if (board->body[left][n]->black != is_black) {
                moves[i][0] = left; moves[i][1] = n; i++;
            } break;
        }
        moves[i][0] = left; moves[i][1] = n; i++;
        left--;
    }

    return i;
}

int pm_bishop(Board * board, int moves[MAXPM][2], int l, int n, _Bool is_black, int start) {

    int il, in, i = start;

    il = l - 1; in = n + 1; // su-sinistra
    while (il >= 0 && in < 8) {
        if (board->body[il][in] != NULL) {
            if (board->body[il][in]->black != is_black) {
                moves[i][0] = il; moves[i][1] = in; i++;
            } break;
        }
        moves[i][0] = il; moves[i][1] = in; i++;
        il--; in++;
    }

    il = l - 1; in = n - 1; // giu-sinistra
    while (il >= 0 && in >= 0) {
        if (board->body[il][in] != NULL) {
            if (board->body[il][in]->black != is_black) {
                moves[i][0] = il; moves[i][1] = in; i++;
            }
            break;
        }
        moves[i][0] = il; moves[i][1] = in; i++;
        il--; in--;
    }

    il = l + 1; in = n - 1; // giu-destra
    while (il < 8 && in >= 0) {
        if (board->body[il][in] != NULL) {
            if (board->body[il][in]->black != is_black) {
                moves[i][0] = il; moves[i][1] = in; i++;
            } break;
        }
        moves[i][0] = il; moves[i][1] = in; i++;
        il++; in--;
    }

    il = l + 1; in = n + 1; // su-destra
    while (il < 8 && in < 8) {
        if (board->body[il][in] != NULL) {
            if (board->body[il][in]->black != is_black) {
                moves[i][0] = il; moves[i][1] = in; i++;
            } break;
        }
        moves[i][0] = il; moves[i][1] = in; i++;
        il++; in++;
    }

    return i;
}

int pm_queen(Board * board, int moves[MAXPM][2], int l, int n, _Bool is_black, int start) {
    int i ;
    i = pm_rook(board, moves, l, n, is_black, start);
    i = pm_bishop(board, moves, l, n, is_black, i);
    return i;
}

int pm_knight(Board * board, int moves[MAXPM][2], int l, int n, _Bool is_black, int start) {

    int ipos = start, n1, n2, il, in;
    for (n1 = 1; n1 <= 2; n1++) {
        if (n1 == 2) n2 = 1; else n2 = 2;
        for (int i = -1; i < 2; i = i + 2) {
            for (int j = -1; j < 2; j = j + 2) {
                il = l + (n1*i); in = n + (n2*j);
                if (il >= 0 && il < 8 && in >= 0 && in < 8) {
                    if (board->body[il][in] == NULL || board->body[il][in]->black != is_black) {
                        moves[ipos][0] = il; moves[ipos][1] = in; ipos++;
                    }
                }

            }
        }
    }
    return ipos;
}

_Bool castling(Board * board, int l1, int n1, int l2, int n2, _Bool black) {
    if (!black) {
                if (l1 == 4 && n1 == 0) {
                    if (l2 == 6 && n2 == 0 && w_arrocco_right == 1 &&
                        board->body[l2][n2] == NULL && board->body[5][0] == NULL) {
                        board->body[l2][n2] = board->body[l1][n1]; board->body[l1][n1] = NULL;
                        board->body[5][0] = board->body[7][0]; board->body[7][0] = NULL;
                        w_arrocco_right = 0; w_arrocco_left = 0;
                        printf("White king short castling...\n\n"); sleep(1); return 1;
                    }
                    if (l2 == 2 && n2 == 0 && w_arrocco_left == 1 &&
                        board->body[l2][n2] == NULL && board->body[3][0] == NULL && board->body[1][0] == NULL) {
                        board->body[l2][n2] = board->body[l1][n1]; board->body[l1][n1] = NULL;
                        board->body[3][0] = board->body[0][0]; board->body[0][0] = NULL;
                        w_arrocco_right = 0; w_arrocco_left = 0;
                        printf("White king long castling...\n\n"); sleep(1); return 1;
                    }
                }
            } else {
                if (l1 == 4 && n1 == 7) {
                    if (l2 == 6 && n2 == 7 && b_arrocco_right == 1 &&
                        board->body[l2][n2] == NULL && board->body[5][7] == NULL) {
                        board->body[l2][n2] = board->body[l1][n1]; board->body[l1][n1] = NULL;
                        board->body[5][7] = board->body[7][7]; board->body[7][7] = NULL;
                        b_arrocco_right = 0; b_arrocco_left = 0;
                        printf("Black king long castling...\n"); sleep(1); return 1;
                        }
                    if (l2 == 2 && n2 == 7 && b_arrocco_left == 1 &&
                        board->body[l2][n2] == NULL && board->body[3][7] == NULL && board->body[1][7] == NULL) {
                        board->body[l2][n2] = board->body[l1][n1]; board->body[l1][n1] = NULL;
                        board->body[3][7] = board->body[0][7]; board->body[0][7] = NULL;
                        b_arrocco_right = 0; b_arrocco_left = 0;
                        printf("Black king short castling...\n"); sleep(1); return 1;
                        }
                }
            }
    return 0;
}

void update_castling(int l1, int n1) {
    if (l1 == 4 && n1 == 0) { // check for arrocco
        if (w_arrocco_right) w_arrocco_right = 0;
        if (w_arrocco_left) w_arrocco_left = 0;
    }
    if (l1 == 4 && n1 == 7) {
        if (b_arrocco_right) b_arrocco_right = 0;
        if (b_arrocco_left) b_arrocco_left = 0;
    }
    if (l1 == 7 && n1 == 0) {
        if (w_arrocco_right) w_arrocco_right = 0;
    }
    if (l1 == 0 && n1 == 0) {
        if (w_arrocco_left) w_arrocco_left = 0;
    }
    if (l1 == 7 && n1 == 7) {
        if (b_arrocco_right) b_arrocco_right = 0;
    }
    if (l1 == 0 && n1 == 7) {
        if (b_arrocco_left) b_arrocco_left = 0;
    }
}

void pawn_promotes(Board * board, int l, int n) {
    _Bool err = 1; int identity = -1;
    while (err) {
        err = 0; identity = -1;
        printf("\nIn what would you like to promote your pawn to?\nInsert one of the follwing numbers:\n\n"
               "Queen --> 2     Bishop --> 3\nKnight --> 4    Rook --> 5\n\nYour pick --> ");
        scanf("%d", &identity); while (getchar() != '\n');
        if (identity < 2 || identity > 5) {
            printf("Wrong number. Only pick a nuber from 2 to 5.\n");
            err = 1;
        }
        if (!err) board->body[l][n]->type = identity;
    }
}

_Bool en_passant(Board * board, int l1, int n1, int l2, int n2, int en_passant_from[][2], int en_passant_to[2],
    char * move1, char * move2, int n_enp, _Bool black) {
    if (board->body[l1][n1]->type == 0) {
        for (int j = 0; j < n_enp; j ++) {
            if(l1 == en_passant_from[j][0] &&  n1 == en_passant_from[j][1]) {
                if (black) {
                    if (n2 == en_passant_to[1] - 1 && l2 == en_passant_to[0]) {
                        printf("From %s 'en passant' to %s...\n\n", move1, move2); sleep(1);
                        board->body[l2][n2] = board->body[l1][n1]; board->body[l1][n1] = NULL;
                        board->body[en_passant_to[0]][en_passant_to[1]] = NULL; return 1;
                    }
                } else {
                    if (n2 == en_passant_to[1] + 1 && l2 == en_passant_to[0]) {
                        printf("From %s 'en passant' to %s...\n\n", move1, move2); sleep(1);
                        board->body[l2][n2] = board->body[l1][n1]; board->body[l1][n1] = NULL;
                        board->body[en_passant_to[0]][en_passant_to[1]] = NULL; return 1;
                    }
                }
            }
        }
    }
    return 0;
}

_Bool check_en_passant(Board * board, int l1, int n1, int l2, int n2, int en_passant_from[][2], int en_passant_to[2],
    int * n_enp, _Bool opponent_color) {
    _Bool en_pas = 0; int n = 0;
    reset_pmoves(en_passant_from, 2);en_passant_to[0] = -1; en_passant_to[1] = -1; n = 0;
    if (board->body[l1][n1]->type == 0 && n1 == pawn_first_move_n) {
        if (l2 + 1 < 8 && board->body[l2+1][n2] != NULL) {
            if (l2 != 7 && board->body[(l2+1)][n2]->type == 0 &&
                board->body[(l2+1)][n2]->black == opponent_color) {
                en_passant_from[n][0] = l2 + 1; en_passant_from[n][1] = n2; n++;
                en_pas = 1;
                }
        }
    }

    if (l2 - 1 >= 0 &&board->body[l2-1][n2] != NULL) {
        if (l2 != 0 && board->body[(l2-1)][n2]->type == 0 &&
            board->body[(l2-1)][n2]->black == opponent_color) {
            en_passant_from[n][0] = l2 - 1; en_passant_from[n][1] = n2; n++;
            en_pas = 1;
            }
    }

    if (en_pas) {en_passant_to[0] = l2; en_passant_to[1] = n2;}
    *n_enp = n;
    return en_pas;
}

_Bool check_if_on_check(Board * board, _Bool black, int pmoves[][2], _Bool opponent) {
    _Bool color;
    if (opponent) color = black; else {
        if (black) color = 0; else color = 1;
    }
    int npm2, kingl, kingn; _Bool on_check;
    kingl = -1; kingn = -1;
    // find king
    for (int i = 0; i < 8; i ++) {
        if (kingl >= 0 && kingn >= 0) break;
        for (int j = 0; j < 8; j++) {
            if (board->body[i][j] != NULL) {
                if (board->body[i][j]->black != color && board->body[i][j]->type == 1) {
                    kingl = i; kingn = j; break;
                }
            }
        }
    }

    on_check = 0; king_loop = 1;
    for (int i = 0; i < 8; i ++) {
        if (on_check == 1) break;
        for (int j = 0; j < 8; j++) {
            if (on_check == 1 ) break;
            if (board->body[i][j] != NULL) {
                if (board->body[i][j]->black == color) {
                    reset_pmoves(pmoves, 8*8); npm2 = 0;
                    npm2 = possible_moves(board, pmoves, i, j, board->body[i][j]->black ,npm2);
                    if (!on_check) {
                        for (int i2 = 0; i2 < npm2; i2 ++) {
                            if (pmoves[i2][0] == kingl && pmoves[i2][1] == kingn) {
                                on_check = 1; reset_pmoves(pmoves, 8*8); npm2 = 0; break;
                            }
                        }
                    }
                }
            }
        }
    }
    king_loop = 0;
    return on_check;
}

_Bool checkmate_check(Board * board, Board * copy_board,
    int pmoves1[MAXPM][2], int pmoves2[MAXPM][2], _Bool opponent_color) {

    _Bool stalemate; int npm = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board->body[i][j] != NULL && board->body[i][j]->black == opponent_color) {
                reset_pmoves(pmoves1, MAXPM); npm = 0;
                npm = possible_moves(board, pmoves1, i, j, opponent_color, 0);

                for (int m = 0; m < npm; m++) {
                    board_copy(board, copy_board); reset_pmoves(pmoves2, MAXPM);
                    copy_board->body[pmoves1[m][0]][pmoves1[m][1]] = copy_board->body[i][j];
                    copy_board->body[i][j] = NULL;
                    if (!check_if_on_check(copy_board, opponent_color, pmoves2, 0)) {
                        return 0;
                    }
                }
            }

        }
    }
    return 1;
}

_Bool stalemate(Board * board, _Bool color, int pmoves[][2]) {
    int npm;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board->body[i][j] != NULL && board->body[i][j]->black == color) {
                reset_pmoves(pmoves, 8*8);
                npm = 0;
                npm = possible_moves(board, pmoves, i, j, color, 0);
                // printf("%d-%d\n", i, j);
                // print_pmoves(pmoves, npm);
                if (npm > 0 ) return 0;

            }
        }
    }
    return 1;
}
