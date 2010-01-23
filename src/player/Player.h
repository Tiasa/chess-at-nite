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

#ifndef PLAYER_H_
#define PLAYER_H_

#include "../common/define.h"
#include "../common/utils.h"
#include "../model/Board.h"
#include "../model/MoveGenerator.h"
#include "../common/extra_utils.h"

class Player {
public:
    Player();
    virtual ~Player();
    void set_board(Board* board);
    virtual move get_move() = 0;
    void set_max_thinking_time(int seconds);
    void set_max_search_depth(int depth);
    void set_show_best_score(bool show);
    void set_show_thinking(bool show);
protected:
    Board* board;
    int max_thinking_time;
    int max_search_depth;

    bool show_best_score;
    bool show_thinking;
};

#endif // PLAYER_H_
