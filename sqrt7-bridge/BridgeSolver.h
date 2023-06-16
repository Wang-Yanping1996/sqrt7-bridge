#pragma once
#include "Card.h"
#include <unordered_map>
#include <ctype.h>
class BridgeSolver {
	std::unordered_map<uint64_t, int> m_State;	// key is state, 13 for each hand, 4 for current action 
public:
	BridgeSolver();
	~BridgeSolver();

	int Solve(std::vector<Hand> &hands, int start, Color color);
};

