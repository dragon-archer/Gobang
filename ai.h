/**
 * @file      ai.h
 * @brief     Declartion of gobang ai
 * @version   0.1
 * @author    dragon-archer
 *
 * @copyright Copyright (c) 2022
 */

#ifndef _GOBANG_AI_H_
#define _GOBANG_AI_H_

#include "pch.h"
#include "gobang.h"

using pos_t = std::pair<int, int>;

int	  ai_score_row(const std::vector<Player>& vec, Player role);
int	  ai_score_board(const Gobang& game, Player role);
pos_t ai_move(Gobang game, Player role);

#endif // _GOBANG_AI_H_
