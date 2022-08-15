/**
 * @file      ai.cpp
 * @brief     AI for gobang
 * @version   0.1
 * @author    dragon-archer
 *
 * @copyright Copyright (c) 2022
 */

#include "pch.h"
#include "ai.h"

using array_pos_t = std::vector<pos_t>;

constexpr int AI_MAX = 1e6;
constexpr int AI_MIN = -1e6;

// version 3.1
// Assume next drop is role
int ai_score_row(const std::vector<Player>& vec, Player role) {
	// 1: whether next drop is yours (0: no, 1:yes)
	// 2: how many same block
	// 3: how many free size
	constexpr int max_free_size					 = 4;
	constexpr int score[2][6][max_free_size + 1] = {
		{
			// next drop isn't yours
			{ 0, 0, 0, 0, 0 }, // 0, Should not appear
			{ 0, 0, 0, 0, 16 }, // 1
			{ 0, 0, 0, 40, 80 }, // 2
			{ 0, 0, 80, 400, 800 }, // 3
			{ 0, 400, 800, 8000, 300000 }, // 4
			{ AI_MAX, AI_MAX, AI_MAX, AI_MAX, AI_MAX } // 5
		},
		{
			// next drop is yours
			{ 0, 0, 0, 0, 0 }, // 0, Should not appear
			{ 0, 0, 0, 0, 20 }, // 1
			{ 0, 0, 0, 100, 500 }, // 2
			{ 0, 0, 1000, 5000, 10000 }, // 3
			{ 0, 100000, 200000, 300000, 400000 }, // 4
			{ AI_MAX, AI_MAX, AI_MAX, AI_MAX, AI_MAX } // 5
		}
	};
	assert(role != P_NONE);
	struct record_t {
		int	   start  = -1;
		int	   length = -1;
		Player role	  = P_NONE;
	};
	int					  ret = 0;
	std::vector<record_t> rec;
	int					  start = 0;
	Player				  cur	= vec[0];
	for(int i = 1; i < int(vec.size()); ++i) {
		if(vec[i] != cur) {
#ifdef DEBUG_AI_FULL
			std::cout << "Record #" << rec.size() << " start = " << start << ", length = " << i - start << ", role = " << player_name[cur] << "\n";
#endif
			rec.push_back({ start, i - start, cur });
			start = i;
			cur	  = vec[i];
		}
	}
#ifdef DEBUG_AI_FULL
	std::cout << "Record #" << rec.size() << " start = " << start << ", length = " << vec.size() - start << ", role = " << player_name[cur] << "\n";
#endif
	rec.push_back({ start, int(vec.size()) - start, cur });

	for(size_t i = 0; i < rec.size(); ++i) {
		if(rec[i].role == P_NONE) {
			continue;
		}
		int blank = 0, side = 0;
		if(i > 0 && rec[i - 1].role == P_NONE) {
			blank += rec[i - 1].length;
			++side;
		}
		if(i + 1 < rec.size() && rec[i + 1].role == P_NONE) {
			blank += rec[i + 1].length;
			++side;
		}
		if(i + 2 < rec.size() && rec[i + 1].role == P_NONE && rec[i + 2].role == rec[i].role && rec[i].length + rec[i + 1].length + rec[i + 2].length <= 5) {
			if(i + 3 < rec.size() && rec[i + 3].role == P_NONE) {
				blank += rec[i + 3].length;
				++side;
			}
			blank = std::min(blank, max_free_size);
			if(side < 2 && blank > 1) {
				blank /= 2;
			} else if(side < 3 && blank > 0) {
				--blank;
			}
			ret += (rec[i].role == role ? 1 : -1) * score[rec[i].role == role ? 1 : 0][rec[i].length + rec[i + 2].length][blank];
#ifdef DEBUG_AI_FULL
			std::cout << "Found block " << player_name[rec[i].role] << "in [" << rec[i].start << ", " << rec[i + 2].start + rec[i + 2].length << ") with " << side << " side, " << blank << " effective blanks, ret is " << ret << "\n";
#endif
			++i; // Not += 2 to caculate next again
		} else if(i < 2 || rec[i - 1].role != P_NONE || rec[i - 2].role != rec[i].role || rec[i].length + rec[i - 1].length + rec[i - 2].length > 5) { // Not merged -> need caculate it self
			blank = std::min(blank, max_free_size);
			if(side < 2 && blank > 1) {
				blank /= 2;
			}
			ret += (rec[i].role == role ? 1 : -1) * score[rec[i].role == role ? 1 : 0][rec[i].length][blank];
#ifdef DEBUG_AI_FULL
			std::cout << "Found single " << player_name[rec[i].role] << "in [" << rec[i].start << ", " << rec[i].start + rec[i].length << ") with " << side << " side, " << blank << " effective blanks, ret is " << ret << "\n";
#endif
		}
	}
	return ret;
}

int ai_score_board(const Gobang& game, Player role) {
	static std::vector<array_pos_t> win_array;
	static int						size   = 0;
	static bool						inited = false;
	assert(role != P_NONE);
	assert(game.win_size() == 5);
	// Init cache
	if(!inited || game.size() != size) {
		inited = true;
		size   = game.size();
		win_array.clear();
		array_pos_t l1, l2;
		// Row & Column
		for(int i = 0; i < size; ++i) {
			l1.clear();
			l2.clear();
			for(int j = 0; j < size; ++j) {
				l1.push_back({ i, j });
				l2.push_back({ j, i });
			}
			win_array.push_back(l1);
			win_array.push_back(l2);
		}
		// Top-left to bottom-right & inv
		for(int diff = size - 5; diff >= 0; --diff) {
			l1.clear();
			l2.clear();
			for(int i = 0; i < size - diff; ++i) {
				const int j = i + diff;
				l1.push_back({ i, j });
				if(diff != 0) {
					l2.push_back({ j, i });
				}
			}
			win_array.push_back(l1);
			if(diff != 0) {
				win_array.push_back(l2);
			}
		}
		// Top-right to bottom-left
		for(int sum = 4; sum < 2 * size - 5; ++sum) {
			l1.clear();
			for(int i = std::max(sum + 1 - size, 0); i < std::min(sum + 1, size); ++i) {
				const int j = sum - i;
				l1.push_back({ i, j });
			}
			win_array.push_back(l1);
		}
	}
	int					ret = 0, tmp = 0;
	std::vector<Player> a;
	for(const auto& v : win_array) {
		a.clear();
		for(const auto& x : v) {
			a.push_back(game.get(x));
		}
		tmp = -ai_score_row(a, opposite_player(role)); // Use opposite because next drop isn't AI
		if(tmp == AI_MAX) {
			return AI_MAX;
		}
		if(tmp == AI_MIN) {
			return AI_MIN;
		}
		ret += tmp;
	}
	return ret;
}

pos_t ai_move(Gobang game, Player role) {
	assert(role != P_NONE);
	assert(game.win_size() == 5);
	const int					  size = game.size();
	pos_t						  best_pos;
	int							  best_score = AI_MIN, tmp = 0, cnt = 0;
	std::vector<std::vector<int>> score;
	score.resize(size, std::vector<int>(size, 0));
#ifdef DEBUG_AI
	std::cout << std::string(9 * size + 1, '-') << '\n';
#endif
	for(int i = 0; i < size; ++i) {
		for(int j = 0; j < size; ++j) {
			if(game[i][j] != P_NONE) {
#ifdef DEBUG_AI
				std::cout << "|" << ((game[i][j] == P_PLAYER1) ? " Player " : "   AI   ");
#endif
				continue;
			}
			game[i][j] = role;
			tmp		   = ai_score_board(game, role);
			if(best_score < tmp) {
				cnt		   = 1;
				best_score = tmp;
				best_pos   = { i, j };
			} else if(best_score == tmp) {
				++cnt;
				if(rand() % cnt == 0) {
					best_pos = { i, j };
				}
			}
			score[i][j] = tmp;
			game[i][j]	= P_NONE;
#ifdef DEBUG_AI
			std::cout << "|" << std::setw(8) << tmp;
#endif
		}
#ifdef DEBUG_AI
		std::cout << "|\n"
				  << std::string(9 * size + 1, '-')
				  << "\n";
#endif
	}
	return best_pos;
}
