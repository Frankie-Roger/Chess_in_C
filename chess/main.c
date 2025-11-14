#include "chess.h"

int pawn_first_move_n = 1;
int pawn_transf_n = 7;

_Bool w_arrocco_right = 1;
_Bool b_arrocco_right = 1;
_Bool w_arrocco_left = 1;
_Bool b_arrocco_left = 1;

int main(void) {
    printf("WELCOME TO CHESS ON TERMINAL!!!\n\n\n");

    Player w_player, b_player, *p_turn = &b_player; Board board, copy_board;
    init_player(&w_player, 0); init_player(&b_player, 1);
    init_board(&board, &w_player, &b_player);
    init_board(&copy_board, &w_player, &b_player);

    _Bool play = 1, err = 0, show_score = 0, opponent_color;
    _Bool on_check = 0, on_check2, checkmate = 0, en_pas, err_en_pas;

    char move1[5], move2[5]; // 5 x quit
    char in[3], color[6];

    int pmoves1[MAXPM][2], pmoves2[8*8][2];
    int en_passant_to[2], en_passant_from[2][2];
    int l1 = 0, n1 = 0, l2 = 0, n2 = 0, err_count, i;
    int npm = 0, n_enp = 0; // npm numero di 'possible moves'


    while(play) {
        if (p_turn == &w_player) p_turn = &b_player; else p_turn = &w_player;

        if (p_turn == &w_player) {
            strcpy(color, "WHITE\0");
            opponent_color = 1;
            pawn_first_move_n = 1; pawn_transf_n = 7;
        } else {
            strcpy(color, "BLACK\0");
            opponent_color = 0;
            pawn_first_move_n = 6; pawn_transf_n = 0;
        }

        if (stalemate(&board, p_turn->color, pmoves2)) {
            print_board(&board);
            printf("\n\n       STALEMATE!\n      No one wins.\n");
            break;
        }


        if (show_score) print_score(&board);
        if (on_check) {printf("\n       'CHECK ON %s KING!'\n", color);}
        print_board(&board);
        if (p_turn == &w_player) printf("Turn--> White\n\n"); else printf("Turn--> Black\n\n");
        if (en_pas) err_en_pas = 1;
        err_count = 0;

        while (1) {
            if (err_count > 0 && err_count % 2 == 0) {
                if (show_score) print_score(&board);
                if (on_check) {printf("\n       'CHECK ON %s KING!'\n", color);}
                print_board(&board);
                if (p_turn == &w_player) printf("Turn--> White\n\n"); else printf("Turn--> Black\n\n");
            }
            err_count++; err = 0;

            printf("(ES. A1 in B2) insert 'quit' to end the game\nMove --> ");
            scanf("%s", move1);
            if (!strcmp("quit\0", move1)) {
                printf("\n\nThank you for playing!!!\nbye bye :)\n\n"); play = 0; break;
            }
            l1 = letter_to_number(move1[0]); n1 = atoi(&move1[1]); n1--;
            if (check_format(l1, n1)) { while ((getchar())!='\n'); continue; }

            scanf("%s", in); if (strcmp(in, "in\0")) {while ((getchar())!='\n'); check_format(10, 10); continue;}

            scanf("%s",move2); while ((getchar())!='\n');
            l2 = letter_to_number(move2[0]); n2 = atoi(&move2[1]); n2--;
            if (check_format(l2, n2)) continue;
            reset_pmoves(pmoves1, MAXPM); npm = 0;
            npm = possible_moves(&board, pmoves1, l1, n1, p_turn->color, 0);

            if (npm == -2) {
                printf("\nPiece in '%s' is not yours.\n\n", move1);
                sleep(1); continue;}
            if (npm == -1) {
                printf("\n'%s' is empty, select an other one.\n\n", move1);
                sleep(1); continue;}
            if (npm == 0) {printf("\nNo possibe moves for '%s'. Try an other move\n\n", move1); continue;}

            if (!on_check) {
                if (castling(&board, l1, n1, l2, n2, p_turn->color)) break;
            }

            if (en_pas || err_en_pas) {
                if (en_passant(&board, l1, n1, l2, n2, en_passant_from, en_passant_to, move1, move2,
                    n_enp, p_turn->color)) {en_pas = 0; break;}
            }
            if (!err_en_pas) {
                en_pas = check_en_passant(&board, l1, n1, l2, n2, en_passant_from, en_passant_to,
                    &n_enp, opponent_color);
            }

            for (i = 0; i < npm; i++) {
                if (pmoves1[i][0] == l2 && pmoves1[i][1] == n2) {
                    board_copy(&board, &copy_board);
                    copy_board.body[l2][n2] = copy_board.body[l1][n1]; copy_board.body[l1][n1] = NULL;
                    on_check2 = check_if_on_check(&copy_board, p_turn->color, pmoves2, 0);
                    if (on_check2) {
                        printf("Move not allowed, your king would die.\n");
                        err = 1; break;
                    }

                    if (board.body[l2][n2] != NULL) {
                        if (!show_score) show_score = 1;
                        printf("From %s eating in %s...\n\n", move1, move2);
                    } else printf("Moving from %s to %s...\n\n", move1, move2);

                    update_castling(l1, n1); sleep(1);

                    board.body[l2][n2] = board.body[l1][n1]; board.body[l1][n1] = NULL;

                    if (board.body[l2][n2]->type == 0 && n2 == pawn_transf_n) {
                        pawn_promotes(&board, l2, n2);
                    }
                    break;
                }
                if (i == npm - 1) {
                    printf("Move not allowed. Try again.\n\n");
                    err = 1;
                }
            }
            if (!err) break;
        } if (!play) return 0;
        err_en_pas = 0;

        on_check = check_if_on_check(&board, p_turn->color, pmoves2, 1);

        if (on_check) {
            checkmate = checkmate_check(&board, &copy_board, pmoves1, pmoves2, opponent_color);
            if (checkmate) {
                    print_board(&board);
                    printf("\n\nCHECKMATE !!!\n\nThe %s player won!!!\n\n", color);
                    return 0;
                }

        }
    }
    return 1;
}
