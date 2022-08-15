#include "pch.h"
#include "gobang.h"
#include "ai.h"

using namespace std;

void cls() {
#ifndef DEBUG_AI
#if _WIN32
	(void)system("cls");
#else
	(void)system("clear");
#endif
#endif
}

int main() {
	srand(time(nullptr));
	Gobang game;
	Player player = P_PLAYER1;
	int	   posx = 0, posy = 0;
	bool   should_countinue = true;
	char   op;
	while(should_countinue) {
		cin.clear();
		if(player == P_PLAYER2) {
			// AI
			tie(posx, posy) = ai_move(game, P_PLAYER2);
			int ret			= game.move(posx, posy, player);
			cout << "AI choose (" << posx + 1 << ", " << posy + 1 << ")" << endl;
			if(ret == R_WIN) {
				cout << game.print();
				cout << player_name[player] << "You lose!\n";
				break;
			}
			player = P_PLAYER1;
		}
		cout << game.print();
		cout << player_name[player] << "Please input your choice(q to quit, r n to revert n step, p to print record list): ";
		cin >> op;
		if(op == 'q') {
			break;
		} else if(op == 'r') {
			int cnt = 0;
			cin >> cnt;
			cout << cnt << endl;
			while(cnt--) {
				if(!game.revert()) {
					break;
				} else {
					player = opposite_player(player);
				}
			}
			if(cnt == -1) {
				cls();
				cout << "Revert successfully!\n";
				continue;
			} else {
				cls();
				cout << "Failed to revert!\n";
				break;
			}
		} else if(op == 'p') {
			cls();
			cout << game.print_record();
			continue;
		} else if(isdigit(op)) {
			cin.unget();
			cin >> posx >> posy;
			--posx;
			--posy;
		} else {
			cout << "Error input!\n";
			string _;
			getline(cin, _);
			continue;
		}
		auto ret = game.move(posx, posy, player);
		switch(ret) {
		case R_OK:
			cls();
			player = opposite_player(player);
			break;
		case R_OCCUPIED:
			cls();
			cout << player_name[player] << "The place have been occupied, please choose another place.\n";
			break;
		case R_OUT_OF_RANGE:
			cls();
			cout << player_name[player] << "The place is out of range, please choose another place.\n";
			break;
		case R_WIN:
			cls();
			cout << game.print();
			cout << player_name[player] << "You win!\n";
			should_countinue = false;
			break;
		case R_INVALID_PLAYER:
			[[fallthrough]];
		case R_ERROR:
			cout << "Internal error!\n";
			should_countinue = false;
			break;
		}
	}
	cout << "Total steps: " << game.steps() << endl;
	cin.get();
	cin.get();
	return 0;
}
