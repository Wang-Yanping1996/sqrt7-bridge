#include "BridgeSolver.h"
// #include <assert.h>


int BridgeSolver::Solve(std::vector<Hand>& hands, int start, Color trump)
{
	assert(hands.size() == 4 && start >= 0 && start < 4);
	uint64_t curState = getState(hands, start);
	if (m_State.find(curState) != m_State.end()) {
		return m_State[curState];
	}

	auto& startHand = hands[start];
	auto& secondHand = hands[(start + 1) % 4];
	auto& thirdHand = hands[(start + 2) % 4];
	auto& fourthHand = hands[(start + 3) % 4];

	for (auto card1 = startHand.GetHead(); card1 != nullptr; card1 = startHand.GetNext(card1)) {
		Color color = card1->Info.Color;
		for (auto card2 = secondHand.GetFirstValid(color); card2 != nullptr; card2 = secondHand.GetNextValid(card2, color)) {
			for (auto card3 = thirdHand.GetFirstValid(color); card3 != nullptr; card3 = thirdHand.GetNextValid(card3, color)) {
				for (auto card4 = fourthHand.GetFirstValid(color); card4 != nullptr; card4 = fourthHand.GetNextValid(card4, color)) {
					int winner = GetWinner(std::vector<Card>{card1->Info, card2->Info, card3->Info, card4->Info}, trump);
					
					// start win this trick
					int curCanWin = -1;
					if ((winner % 2) == 0) {
						curCanWin = 1 + Solve(hands, (winner + start) % 4, trump);
					}
					// opp win this trick, can win (remain - opp's max win)
					else {
						curCanWin = startHand.GetCards() - Solve(hands, (winner + start) % 4, trump);
					}
					m_State[curState] = std::max(m_State[curState], curCanWin);
				}
			}
		}
	}

	assert(m_State.find(curState) != m_State.end());
	return m_State[curState];
}
