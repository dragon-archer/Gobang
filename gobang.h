/**
 * @file      gobang.h
 * @brief     Definition of Gobang
 * @version   0.1
 * @author    dragon-archer
 *
 * @copyright Copyright (c) 2022
 */

#ifndef _GOBANG_GOBANG_H_
#define _GOBANG_GOBANG_H_

#include "pch.h"

inline Player opposite_player(Player you) {
	switch(you) {
	case P_NONE:
		return P_NONE;
	case P_PLAYER1:
		return P_PLAYER2;
	case P_PLAYER2:
		return P_PLAYER1;
	}
	return P_NONE;
}

class Gobang {
private:
	std::vector<std::vector<Player>>		  m_board;
	int										  m_size;
	int										  m_win_size;
	std::vector<std::tuple<int, int, Player>> m_record;

public: // Constructor
	Gobang(int _size = DEFAULT_SIZE, int _win_size = WIN_SIZE)
		: m_size(_size)
		, m_win_size(_win_size) {
		m_board.resize(_size, std::vector<Player>(_size, P_NONE));
	}

public: // Trivial accessors
	inline int size() const noexcept {
		return m_size;
	}

	inline int win_size() const noexcept {
		return m_win_size;
	}

	inline const auto& board() const noexcept {
		return m_board;
	}

	inline const auto& record() const noexcept {
		return m_record;
	}

	inline std::vector<Player>& operator[](std::size_t n) {
		return m_board.at(n);
	}

	inline const std::vector<Player>& operator[](std::size_t n) const {
		return m_board.at(n);
	}

	inline Player get(std::pair<int, int> pos) const {
		return m_board.at(pos.first).at(pos.second);
	}

	inline size_t steps() const noexcept {
		return m_record.size();
	}

public: // Actions
	inline void resize(int _size = DEFAULT_SIZE) {
		m_size = _size;
		m_board.clear();
		m_board.resize(_size, std::vector<Player>(_size, P_NONE));
	}

	inline void reset() noexcept {
		for(auto& row : m_board) {
			row.assign(m_size, P_NONE);
		}
	}

	MoveResult move(int row, int col, Player player) noexcept {
		if(row < 0 || row >= m_size || col < 0 || col >= m_size) {
			return R_OUT_OF_RANGE;
		}
		if(player == P_NONE) {
			return R_INVALID_PLAYER;
		}
		if(m_board[row][col] != P_NONE) {
			return R_OCCUPIED;
		}
		m_board[row][col] = player;
		m_record.push_back({ row, col, player });

		Player w = winner();
		if(w == player) {
			return R_WIN;
		}
		if(w == P_NONE) {
			return R_OK;
		}
		return R_ERROR; // You should not lose on your move
	}

	bool revert() noexcept {
		if(m_record.empty()) {
			return false;
		}
		auto [row, col, _] = m_record.back();
		m_record.pop_back();
		m_board[row][col] = P_NONE;
		return true;
	}

	Player winner() const noexcept {
		Player cur = P_NONE;
		int	   cnt = 0;

		auto has_winner = [&](Player x) {
			if(x == P_NONE) {
				cur = P_NONE;
				cnt = 0;
			} else {
				if(x == cur) {
					++cnt;
					if(cnt == m_win_size) {
						return x;
					}
				} else {
					cur = x;
					cnt = 1;
				}
			}
			return P_NONE;
		};

		// Calculate row
		for(const auto& row : m_board) {
			for(const auto& x : row) {
				if(auto ret = has_winner(x); ret != P_NONE) {
					return ret;
				}
			}
			cur = P_NONE;
			cnt = 0;
		}
		// Calculate column
		for(int j = 0; j < m_size; ++j) {
			for(int i = 0; i < m_size; ++i) {
				if(auto ret = has_winner(m_board[i][j]); ret != P_NONE) {
					return ret;
				}
			}
			cur = P_NONE;
			cnt = 0;
		}
		// Calculate up-left to bottom-right
		for(int diff = m_size - m_win_size; diff >= m_win_size - m_size; --diff) {
			for(int i = std::max(-diff, 0); i < std::min(m_size - diff, m_size); ++i) {
				if(auto ret = has_winner(m_board[i][i + diff]); ret != P_NONE) {
					return ret;
				}
			}
			cur = P_NONE;
			cnt = 0;
		}
		// Calculate bottom-left to up-right
		for(int sum = m_win_size - 1; sum < 2 * m_size - m_win_size; ++sum) {
			for(int i = std::max(sum + 1 - m_size, 0); i < std::min(sum + 1, m_size); ++i) {
				if(auto ret = has_winner(m_board[i][sum - i]); ret != P_NONE) {
					return ret;
				}
			}
			cur = P_NONE;
			cnt = 0;
		}
		return P_NONE;
	}

public: // CUI
	std::string print() const {
		const std::string headline	   = "  | 1| 2| 3| 4| 5| 6| 7| 8| 9|10|11|12|13|14|15|16|17|18|19|";
		const std::string line_padding = std::string(3 * m_size + 3, '-') + '\n';
		const char*		  player_map[] = { "  ", "()", "><" };
		const char*		  header[]	   = { " 1", " 2", " 3", " 4", " 5", " 6", " 7", " 8", " 9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19" };

		std::string ret = headline.substr(0, 3 * m_size + 3) + '\n' + line_padding;
		int			i	= 0;
		for(const auto& row : m_board) {
			ret += header[i++];
			for(const auto& x : row) {
				ret += '|';
				ret += player_map[x];
			}
			ret += "|\n";
			ret += line_padding;
		}
		return ret;
	}

	std::string print_record() const {
		std::string ret;
		for(const auto& [row, col, player] : m_record) {
			ret += player_name[player] + std::to_string(row + 1) + ", " + std::to_string(col + 1) + '\n';
		}
		return ret;
	}
};

#endif // _GOBANG_GOBANG_H_
