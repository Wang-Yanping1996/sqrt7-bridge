#include "BridgeSolver.h"
// #include <assert.h>
#include <iostream>
#include <bitset>

/*
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

	for (auto card1 = startHand.TakeHeader(); card1 != nullptr; card1 = startHand.IterateTakeNext(card1)) {
		Color color = card1->Info.Color;
		for (auto card2 = secondHand.TakeFirstValid(color); card2 != nullptr; card2 = secondHand.IterateTakeNextValid(card2, color)) {
			for (auto card3 = thirdHand.TakeFirstValid(color); card3 != nullptr; card3 = thirdHand.IterateTakeNextValid(card3, color)) {
				for (auto card4 = fourthHand.TakeFirstValid(color); card4 != nullptr; card4 = fourthHand.IterateTakeNextValid(card4, color)) {
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
*/

int BridgeSolver::SingleSolve(std::vector<Hand>& hands, int curPlayer, Color color)
{
	assert(hands.size() == 4 && curPlayer >= 0 && curPlayer < 4);

	auto& curHand = hands[curPlayer];
	
	// first card in this round
	if (m_RoundCards.size() == 0) {
		uint64_t curState = getState(hands, curPlayer);
		if (m_State.find(curState) != m_State.end()) {
			return (curPlayer % 2 == 0 ? m_State[curState].NS : m_State[curState].EW);
		}

		for (auto card = curHand.TakeHeader(); card != nullptr; card = curHand.IterateTakeNext(card)) {
			// cur player can max win, is the minmium of oppo can max win
			m_RoundCards.push_back(card->Info);
			int curLose = SingleSolve(hands, (curPlayer + 1) % 4, card->Info.Color);
			m_RoundCards.pop_back();
			int canWin = curHand.GetCards() + 1 - curLose;

			UpdateRes(m_State[curState], curPlayer, canWin, curHand.GetCards() + 1);
		}

		assert(m_State.find(curState) != m_State.end());
		return (curPlayer % 2 == 0 ? m_State[curState].NS : m_State[curState].EW);
	}

	// 2nd or 3rd player 
	if (m_RoundCards.size() < 3) {
		Result res = { 0, 0 };
		for (auto card = curHand.TakeFirstValid(color); card != nullptr; card = curHand.IterateTakeNextValid(card, color)) {
			m_RoundCards.push_back(card->Info);
			int curLose = SingleSolve(hands, (curPlayer + 1) % 4, color);
			m_RoundCards.pop_back();
			int canWin = curHand.GetCards() + 1 - curLose;

			UpdateRes(res, curPlayer, canWin, curHand.GetCards() + 1);
		}
		return (curPlayer % 2 == 0 ? res.NS : res.EW);
	}

	// last play in this round 
	Result res = { 0, 0 };
	for (auto card = curHand.TakeFirstValid(color); card != nullptr; card = curHand.IterateTakeNextValid(card, color)) {
		m_RoundCards.push_back(card->Info);
		int winner = GetWinner(m_RoundCards, m_Trump);	// winner index in this round 
		m_RoundCards.pop_back();
		winner = (winner + curPlayer + 1) % 4;			// convert to sits 

		int canWin = -1;
		std::vector<Card> cards = m_RoundCards;
		m_RoundCards.clear();
		if ((winner % 2) == (curPlayer % 2)) {
			canWin = 1 + SingleSolve(hands, winner, Color::NoTrump);
		}
		else {
			int maxLose = 1 + SingleSolve(hands, winner, Color::NoTrump);
			canWin = curHand.GetCards() + 1 - maxLose;
		}
		m_RoundCards = cards;

		UpdateRes(res, curPlayer, canWin, curHand.GetCards() + 1);
	}
	
	return (curPlayer % 2 == 0 ? res.NS : res.EW);
}

void BridgeSolver::printState(uint64_t state, std::vector<Hand> const& hands)
{
	std::cout << "print state: " << std::hex << state << 
		" , NS res: " << m_State[state].NS <<  ", EW res: " << m_State[state].EW << std::endl;
	std::cout << "N: ";
	for (int i = 0; i < 13 && i < hands[0].GetCards(); i++) {
		if (state & (1LL << i)) {
			Card card = hands[0].GetCard(i);
			std::cout << card.PrintColor() << card.PrintNumber();
		}
	}
	std::cout << std::endl;

	std::cout << "E: ";
	for (int i = 0; i < 13 && i < hands[1].GetCards(); i++) {
		if (state & (1LL << (i + 13))) {
			Card card = hands[1].GetCard(i);
			std::cout << card.PrintColor() << card.PrintNumber();
		}
	}
	std::cout << std::endl;
	std::cout << "S: ";
	for (int i = 0; i < 13 && i < hands[2].GetCards(); i++) {
		if (state & (1LL << (i + 26))) {
			Card card = hands[2].GetCard(i);
			std::cout << card.PrintColor() << card.PrintNumber();
		}
	}
	std::cout << std::endl;
	std::cout << "W: ";
	for (int i = 0; i < 13 && i < hands[3].GetCards(); i++) {
		if (state & (1LL << (i + 39))) {
			Card card = hands[3].GetCard(i);
			std::cout << card.PrintColor() << card.PrintNumber();
		}
	}
	std::cout << std::endl;

	int i = 0;
	for (; i < 4; i++) {
		if (state & (1LL << (i + 52))) {
			break;
		}
	}
	std::cout << "cur player is: " << i << std::endl;
	std::cout << std::endl;
}

void BridgeSolver::printStates(std::vector<Hand> const& hands) {
	for (auto it = m_State.begin(); it != m_State.end(); it++) {
		printState(it->first, hands);
	}
}
