/**
 * @file      pch.h
 * @brief     Pre-compiled header
 * @version   0.1
 * @author    dragon-archer
 *
 * @copyright Copyright (c) 2022
 */

#ifndef _GOBANG_PCH_H_
#define _GOBANG_PCH_H_

#include <algorithm>	// std::min
#include <cassert>		// assert
#include <iomanip>		// std::setw
#include <iostream>		// std::cout
#include <string>		// std::string
#include <tuple>		// std::pair
#include <vector>		// std::vector

#ifndef NDEBUG
	#define DEBUG_AI
	#ifdef _DEBUG
		#define DEBUG_AI_FULL
	#endif
#endif

constexpr int		  DEFAULT_SIZE	= 15;
constexpr int		  WIN_SIZE		= 5;
constexpr const char* player_name[] = { "[NONE] ", "[Player1] ", "[Player2] " };

enum Player {
	P_NONE	  = 0,
	P_PLAYER1 = 1,
	P_PLAYER2 = 2
};

enum MoveResult {
	R_OK = 0,
	R_OCCUPIED,
	R_OUT_OF_RANGE,
	R_INVALID_PLAYER,
	R_WIN,
	R_ERROR
};

#endif // _GOBANG_PCH_H_
