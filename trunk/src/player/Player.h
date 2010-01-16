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
#include "../model/Board.h"
#include "../model/MoveGenerator.h"
#include "../common/utils.h"

class Player {
protected:
    Board* board;
public:
    Player();
    virtual ~Player();
    void set_board(Board* board);
    virtual move get_move() = 0;
};

#endif // PLAYER_H_
