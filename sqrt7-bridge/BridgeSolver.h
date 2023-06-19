#pragma once
#include "Card.h"
// #include <unordered_map>
// #include <ctype.h>
class BridgeSolver {
	std::unordered_map<uint64_t, int> m_State;	// key is state, 13 for each hand, 4 for current action 
public:
	BridgeSolver() {
		for (int i = 0; i < 4; i++) {
			int64_t state = 0;
			SET_BIT(state, 52 + i);
			m_State[state] = 0;
		}
	};
	~BridgeSolver() {};

	uint64_t getState(std::vector<Hand> &hands, int start) {
		uint64_t res = 0;
		SET_BIT(res, start + 52);
		for (auto& hand : hands) {
			res |= hand.GetState();
		}
		return res;
	};


	int GetWinner(std::vector<Card>&& cards, Color trump) {
		if (trump == Color::NoTrump)
			trump = cards[0].Color;

		int maxNum = -1;
		int res = -1;
		for (int i = 0; i < (int)cards.size(); i++) {
			if (trump == cards[i].Color) {
				if (cards[i].Number > maxNum) {
					res = i, maxNum = cards[i].Number;
				}
			}
		}

		if (res != -1)
			return res;

		return 0;
	};

	// given status, return the number number starter can win
	int Solve(std::vector<Hand> &hands, int start, Color color);
};

