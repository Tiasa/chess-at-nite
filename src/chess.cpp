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

#include "common/globals.h"
#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>

#include "common/define.h"
#include "model/MoveGenerator.h"
#include "model/Game.h"
#include "player/Player.h"
#include "player/HumanPlayer.h"
#include "player/ComputerPlayer.h"
#include "control/CLI.h"
#include "common/extra_utils.h"

void test();

int main(int argc, char **argv) {
#ifdef DEBUG
    cout << "!!!!!!!!!!!!!!!!!!!!!!!!"<< endl;
    cout << "!!! DEBUG MODE IS ON !!!"<< endl;
    cout << "!!!!!!!!!!!!!!!!!!!!!!!!"<< endl;
#endif

    init_globals();
    int user_option = 0;
    if (argc > 1) {
        user_option = atoi(argv[1]);
    }

#ifdef COMMAND_LINE
    if (user_option > 0) {
        CLI cli(user_option);
    } else {
        CLI cli;
    }
#else
    // for testing
    test();
#endif
    return 0;
}

/*
 * For testing purposes during developing.
 */
void test() {
    string default_board = DEFAULT_FEN;

    default_board = "5q2/k113PP/2Q5/8/2K4r/3P4/4P3/8 w - - 0 20";

    Board board = Board(default_board);

    Player* white_player = new HumanPlayer();
    Player* black_player = new ComputerPlayer();

    Game game = Game(&board, white_player, black_player);
    game.start_game();

    delete white_player;
    delete black_player;
}