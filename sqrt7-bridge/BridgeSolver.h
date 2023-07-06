#pragma once
#include "Card.h"
#include <math.h>
#include <map>
// #include <unordered_map>
// #include <ctype.h>
struct Result {
	int NS, EW;
};
class BridgeSolver {
	std::unordered_map<uint64_t, Result> m_State;	// key is state, 13 for each hand, 4 for current action 
	Color m_Trump;
	std::vector<Card> m_RoundCards;
	std::map<Color, std::map<Number, Card> > m_TotalHand;
public:
	BridgeSolver() = delete;
	BridgeSolver(Color trump): m_Trump(trump) {
		for (int i = 0; i < 4; i++) {
			uint64_t state = 0;
			SET_BIT_64(state, 52 + i);
			m_State[state].NS = 0;
			m_State[state].EW = 0;
		}
	};
	~BridgeSolver() {};

	uint64_t getState(std::vector<Hand> &hands, int start) {
		uint64_t res = 0;
		SET_BIT_64(res, start + 52);
		int i = 0;
		for (auto& hand : hands) {
			uint64_t state = hand.GetState();
			res |= state << i;
			i += 13;
		}
		return res;
	};

	Result UpdateRes(Result& res, int curPlayer, int canWin, int remain) {
		// NS
		if ((curPlayer % 2) == 0) {
			res.NS = std::max(res.NS, canWin);
			res.EW = remain - res.NS;
			return res;
		}

		// EW
		res.EW = std::max(res.EW, canWin);
		res.NS = remain - res.EW;
		return res;
	};

	bool isFirstLarger(const Card& c1, const Card c2) {
		if (c1.Color == c2.Color)
			return c1.Number > c2.Number;
		if (c1.Color == m_Trump && c2.Color != m_Trump)
			return true;
		if (c1.Color != m_Trump && c2.Color == m_Trump)
			return false;
		if (c1.Color != c2.Color)
			return true;

		assert(false);
		return false;
	};

	int GetWinner(std::vector<Card>& cards, Color trump) {
		assert(cards.size() == 4);

		int res = 0;
		for (int i = 1; i < (int)cards.size(); i++) {
			if (!isFirstLarger(cards[res], cards[i])) {
				res = i;
			}
		}
		return res;
	};

	// given status, return the number starter can win
	int Solve(std::vector<Hand>& hands, int start, Color color) {
		assert(hands.size() == 4 && start >= 0 && start < 4);

		std::vector<Card> cards;
		for (auto& hand : hands) {
			for (auto card = hand.getHead(); card != nullptr; card = hand.getNext(card)) {
				m_TotalHand[card->Info.Color][card->Info.Number] = card->Info;
			}
		}

		return SingleSolveWithCut(hands, start, color, Result{ hands[0].GetCards(), hands[0].GetCards() });
	};

	// given status, return the maxmium number curPlayer can win 
	int SingleSolve(std::vector<Hand>& hands, int curPlayer, Color color);	
	int SingleSolveWithCut(std::vector<Hand>& hands, int curPlayer, Color color, Result maxLose);

	void printState(uint64_t state, std::vector<Hand> const& hands);
	void printStates(std::vector<Hand> const& hands);
	bool isNS(int curPlayer) { return (curPlayer % 2) == 0; };
	bool isEW(int curPlayer) { return !isNS(curPlayer); };

	bool isSame(Card const& c1, Card const& c2) {
		if (c1.Color != c2.Color)
			return false;
		if (c1.Number > c2.Number)
			return isSame(c2, c1);
		// c1 < c2
		auto it = m_TotalHand.find(c1.Color);
		if (it == m_TotalHand.end())
			return false;
		auto iter = it->second.find(c1.Number);
		if (iter == it->second.end())
			return false;
		return (*(++iter)).second.Number == c2.Number;
	};
	void removeFromTotal(std::vector<Card>& cards) {
		for (auto card : cards) {
			auto it = m_TotalHand.find(card.Color);
			it->second.erase(card.Number);
		}
	}
	void addIntoTotal(std::vector<Card>& cards) {
		for (auto card : cards) {			
			m_TotalHand[card.Color][card.Number] = card;
		}
	}
};

