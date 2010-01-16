/*
 * This file is part of the chess-at-nite project.
 *
 * Copyright (c) 2009 by
 *   Franziskus Domig
 *   Panayiotis Lipiridis
 *   Radoslav Petrik
 *   Thai Gia Tuong
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include "ComputerPlayer.h"
#include <memory.h>

ComputerPlayer::ComputerPlayer() :
Player() {
}

move ComputerPlayer::get_move() {
    move m;
    if (opening_book.book_open) {
        MoveGenerator generator = MoveGenerator(board);
        generator.generate_all_moves();
        vector<move> moves = generator.get_all_moves();
        if (opening_book.get_move(moves, board->history, m)) {
            cout << move_to_string(m) << endl;
            return m;
        }
    }
    cout << "thinking... " << endl;
    m = search_pv();
    return m;
}

move ComputerPlayer::search_pv() {
    int start_time = get_ms();
    board->stop_time = start_time + MAX_THINKING_TIME;
    board->time_exit = false;

    board->ply = 0;
    board->checked_nodes = 0;

    memset(board->pv, 0, sizeof (board->pv));
#ifdef SHOW_THINKING
    printf("ply     time    nodes    score   pv\n");
#endif
    move best_moves[MAX_SEARCH_DEPTH];
    int best_scores[MAX_SEARCH_DEPTH];
    int best_moves_plys[MAX_SEARCH_DEPTH];

    int depth = 1;
    bool found_mate = false;
    for (; depth < MAX_SEARCH_DEPTH && !board->time_exit; depth++) {
        board->follow_pv = true;
        int score = alpha_beta(depth, -EVALUATION_START, EVALUATION_START);
        best_moves[depth] = board->pv[0][0];
        best_moves_plys[depth] = board->pv_length[0];
        best_scores[depth] = score;

#ifdef SHOW_THINKING
        if (!board->time_exit) {
            printf("%3d  %7s  %7s  %7s  ", depth,
                    display_time(start_time, get_ms()),
                    display_nodes_count(board->checked_nodes),
                    display_score(score));
            for (int j = 0; j < board->pv_length[0]; ++j) {
                printf(" %s", move_to_string_basic(board->pv[0][j]).c_str());
            }
            printf("\n");
            fflush(stdout);
        }
#endif
        if (abs(score) >= MATE) {
            found_mate = true;
            break;
        }
    }

    move best_move;
    int best_move_plys;
    int best_score;

    if (board->time_exit) {
        best_move = best_moves[depth - 2];
        best_move_plys = best_moves_plys[depth - 2];
        best_score = best_scores[depth - 2];
    } else if (found_mate) {
        best_move = best_moves[depth];
        best_move_plys = best_moves_plys[depth];
        best_score = best_scores[depth];
    } else {
        best_move = best_moves[depth - 1];
        best_move_plys = best_moves_plys[depth - 1];
        best_score = best_scores[depth - 1];
    }

#ifdef SHOW_SEARCH_INFO
    float total_time = (float) (get_ms() - start_time) / 1000;
    printf("%s nodes searched in %.2f secs (%.1fK nodes/sec)\n",
            display_nodes_count(board->checked_nodes),
            total_time,
            (board->checked_nodes / 1000.0) / total_time);
#endif
    
#ifdef SHOW_BEST_SCORE
    cout << "Score for move " << move_to_string(best_move) << " is " << display_score(best_score);
    cout << " (" << best_move_plys << " plys)";
    cout << endl;
#endif

    return best_move;
}

int ComputerPlayer::alpha_beta(int depth, int alpha, int beta) {

    if (depth == 0) {
        return quiescence(alpha, beta);
    }
    board->checked_nodes++;

    // check the time every 4096 nodes
    if ((board->checked_nodes & 4095) == 0) {
        if (get_ms() > board->stop_time) {
            board->time_exit = true;
            return 0;
        }
    }

#ifdef USE_HASH_TABLE
    // EXPERIMENTAL: check for a hash entry
    switch (board->hash_probe(depth, &alpha, beta)) {
        case EXACT:
            if (alpha < beta) {

                board->pv_length[board->ply] = board->ply;
                htentry* entry = board->hash_entry(board->get_hash());
                board->pv[board->ply][board->ply] = entry->best;

                for (int j = board->ply + 1; j < board->pv_length[board->ply
                        + 1]; ++j) {
                    board->pv[board->ply][j] = board->pv[board->ply + 1][j];
                }
                board->pv_length[board->ply] = board->pv_length[board->ply + 1];

            }
            return alpha;
        case LOWER:
            return beta;
        case UPPER:
            return alpha;
        case NO:
            break;
    }
#endif // USE_HASH_TABLE
    board->pv_length[board->ply] = board->ply;

    MoveGenerator generator = MoveGenerator(board);
    generator.generate_all_moves();
    vector<move> moves = generator.get_all_moves();

    // are we in check? so we search deeper
    bool check = generator.king_under_check;
    if (check) {
        depth++;
    }

    // If this is a root node, we can't just return 0, we need a move.
    // else we check for a repetition and assume that this is a draw.
    if (board->ply > 0 && repetitions(board)) {
        return DRAW;
    }

    //TODO: maybe it's better to use the get_all_moves(move best_move) from
    // move generator, cause this function puts the best move in front without
    // changing of the moves.. don't forget that they are sorted
    if (board->follow_pv) {
        sort_pv(moves);
    }

    bool played_move = false;
    int score = 0;
    bool pvSearch = true;

#ifdef USE_HASH_TABLE
    int o_alpha = alpha;
    move best;
#endif

    for (unsigned index = 0; index < moves.size(); index++) {
        board->fake_move(moves[index]);
        played_move = true;
        if (pvSearch) {
            score = -alpha_beta(depth - 1, -beta, -alpha);
        } else {
            score = -alpha_beta(depth - 1, -alpha - 1, -alpha);
            if (score > alpha && score < beta) {
                // re-search
                score = -alpha_beta(depth - 1, -beta, -alpha);
            }
        }
        board->unfake_move();

        // we have no time left
        if (board->time_exit) {
            return 0;
        }

        if (score > alpha) {
            if (score >= beta) {

#ifdef USE_HASH_TABLE
                board->hash_store(depth, LOWER, score, moves[index]);
#endif // USE_HASH_TABLE
                return score;
            }
            alpha = score;

#ifdef USE_HASH_TABLE
            best = moves[index];
#endif
            pvSearch = false;

            // store the new, better alpha node in the path
            board->pv[board->ply][board->ply] = moves[index];
            for (int j = board->ply + 1; j < board->pv_length[board->ply + 1]; ++j)
                board->pv[board->ply][j] = board->pv[board->ply + 1][j];
            board->pv_length[board->ply] = board->pv_length[board->ply + 1];
        }
    }

    // if we didn't played a move, we are either checkmate or stalemate
    if (!played_move) {
        if (check) {
            return -(MATE + board->ply);
        }
        return DRAW;
    }

    if (board->fifty_moves >= 100) {
        return DRAW;
    }
#ifdef USE_HASH_TABLE
    htype type = (alpha == o_alpha) ? UPPER : EXACT;
    if (alpha != o_alpha) {
        // TODO: change PV
    }
    board->hash_store(depth, type, alpha, best);
#endif // USE_HASH_TABLE
    return alpha;
}

int ComputerPlayer::quiescence(int alpha, int beta) {

    board->checked_nodes++;

    // check the time every 4096 nodes
    if ((board->checked_nodes & 4095) == 0) {
        if (get_ms() > board->stop_time) {
            board->time_exit = true;
            return 0;
        }
    }

    board->pv_length[board->ply] = board->ply;

    // check with the evaluation function
    int e = evaluate(board);
    if (e >= beta)
        return beta;
    if (e > alpha)
        alpha = e;

    MoveGenerator generator = MoveGenerator(board);
    generator.generate_all_moves();
    vector<move> moves = generator.get_all_capture_moves();

    for (unsigned index = 0; index < moves.size(); index++) {
        board->fake_move(moves[index]);
        int score = -quiescence(-beta, -alpha);
        board->unfake_move();
        if (score > alpha) {
            if (score >= beta) {
                return beta;
            }
            alpha = score;

            // store the new, better alpha node in the path
            board->pv[board->ply][board->ply] = moves[index];
            for (int j = board->ply + 1; j < board->pv_length[board->ply + 1]; ++j)
                board->pv[board->ply][j] = board->pv[board->ply + 1][j];
            board->pv_length[board->ply] = board->pv_length[board->ply + 1];
        }
    }
    return alpha;

}

void ComputerPlayer::sort_pv(vector<move>& moves) {
    board->follow_pv = false;
    for (unsigned i = 0; i < moves.size(); i++) {
        if (moves[i].pos_new == board->pv[0][board->ply].pos_new
                && moves[i].pos_old == board->pv[0][board->ply].pos_old) {
            board->follow_pv = true;
            move tmp = moves[0];
            moves[0] = moves[i];
            moves[i] = tmp;
            return;
        }
    }
}
