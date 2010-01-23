/*
 * This file is part of the chess-at-nite project.
 *
 * Copyright (c) 2009-2010 by
 *   Franziskus Domig
 *   Panayiotis Lipiridis
 *   Radoslav Petrik
 *   Thai Gia Tuong
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifndef CLI_H_
#define CLI_H_

#include <stdio.h>
#include <iostream>
#include <string>
#include "../model/Game.h"
#include "../model/Board.h"
#include "../model/MoveGenerator.h"
#include "../player/ComputerPlayer.h"
#include "../player/HumanPlayer.h"
#include "../common/utils.h"

class Player;
class Game;
class Board;

#define QUIT            0
#define HUMAN_VS_CPU    1
#define CPU_VS_HUMAN    2
#define HUMAN_VS_HUMAN  3
#define CPU_VS_CPU      4
#define SELECT_FEN      5
#define SHOW_HELP       6
#define BENCHMARK       7
#define WAC             8

#define SETTINGS        9

//setings defines
#define SET_MAX_TIME         1
#define SET_MAX_DEPTH        2
#define SET_SHOW_BEST_SCORE  3
#define SET_SHOW_THINKING    4


#define BENCHMARK_FEN "r3r1k1/pp3pbp/1qp1b1p1/2B5/2BP4/Q1n2N2/P4PPP/3R1K1R w - - 0 18"

class CLI {
public:
    CLI();
    CLI(int option);
    virtual ~CLI();

    void start();

private:
    void init();
    Player* white_player;
    Player* black_player;
    Board* board;
    Game* game;
    //when the only human player is black.. 
    bool rotated_board;
    bool both_human;
    //time is in seconds
    int max_thinking_time;
    int max_search_depth;
    bool show_best_score;
    bool show_thinking;
    std::string fen;

    void show_options();
    void apply_option(int option);
    void show_settings();
    void settings();
    void apply_settings(int option);
    void set_max_time_from_user();
    void set_max_depth_from_user();
    int get_user_option();
    void init_game(int game_type);
    void start_game();
    void end_game();
    void show_about();
    void select_fen();
    void run_benchmark();
    void run_wac_test();
    std::string get_line();

    bool compare_found_move(string found, string should);
};

#endif /* CLI_H_ */
