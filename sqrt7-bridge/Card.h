#pragma once
#include <vector>
#include <unordered_map>
#include <assert.h>
#include <algorithm>

#define SET_BIT(x, y) (x |= (1 << (y)))
#define SET_LOW_BITS(x, y) (x |= ((1 << (y)) - 1))

#define CLEAR_BIT(x, y) (x &= (1 << (y)))

enum Color { InvalidColor = -1, Club = 1, Diamond, Heart, Spade, NoTrump };
enum Number
{
	InvalidNumber = -1,
	Two = 1,
	Three,
	Four,
	Five,
	Six,
	Seven,
	Eight,
	Nine,
	Ten,
	Jack,
	Queen,
	King,
	Ace
};

struct Card
{
	Color  Color;
	Number Number;
};

// typedef std::vector<Card> Hand;
// typedef std::map<Color, std::map<Number, int> > SortedHand;


struct CardNode
{
	CardNode* Prev, * Next;
	Card Info;
	int Index;
	bool IsValid;
};


class Hand {
	std::vector<CardNode*> m_Indices;
	std::unordered_map<Color, CardNode*> m_Entries;
	int m_Nums;
	uint64_t m_State;
public:
	Hand(std::vector<Card>&& cards) {
		m_Entries[Color::Club] = nullptr;
		m_Entries[Color::Diamond] = nullptr;
		m_Entries[Color::Heart] = nullptr;
		m_Entries[Color::Spade] = nullptr;

		std::sort(cards.begin(), cards.end(), [](Card& x, Card& y) {
			return x.Color == y.Color ? x.Number < y.Number : x.Color < y.Color;
		});

		CardNode* prev = nullptr;
		for (int i = 0; i < (int)cards.size(); i++) {
			CardNode* cur = new CardNode();
			cur->Index = i;
			cur->Info = cards[i];
			cur->IsValid = true;
			cur->Prev = prev;
			cur->Next = nullptr;
			if (prev != nullptr)
				prev->Next = cur;
			prev = cur;

			if (m_Entries[cur->Info.Color] == nullptr) {
				m_Entries[cur->Info.Color] = cur;
			}
		}
		
		m_Nums = cards.size();
		m_State = 0;
		SET_LOW_BITS(m_State, m_Nums);
	};
	~Hand() {
		for (auto it = m_Indices.begin(); it != m_Indices.end(); it++) {
			free(*it);
		}
	};

	int GetCards() const { return m_Nums; };
	
	CardNode* GetHead() const {
		for (int i = Color::Club; i <= Color::Spade; i++) {
			Color color = (Color)(i);
			auto it = m_Entries.find(color);
			if (it != m_Entries.end() && it->second != nullptr)
				return it->second;
		}
		return nullptr;
	};

	void ImmediateInsert(CardNode* card) {
		assert(card != nullptr);

		int index = card->Index;
		auto next = card->Next, prev = card->Prev;
		
		if (next != nullptr) {
			assert(next->Prev == prev);
			next->Prev = card;
		}
		if (prev != nullptr) {
			assert(prev->Next == next);
			prev->Next = card;
		}
		card->IsValid = true;
		m_Nums++;
		SET_BIT(m_State, card->Index);

		Color color = card->Info.Color;
		auto entry = m_Entries[color];
		if (entry != nullptr && card->Info.Number > entry->Info.Number) {
			return;
		}

		m_Entries[color] = card;
		return;
	};

	void Remove(CardNode* node) {
		assert(node != nullptr);

		auto next = node->Next, prev = node->Prev;
		if (prev != nullptr) {
			prev->Next = next;
		}
		if (next != nullptr) {
			next->Prev = prev;
		}
		node->IsValid = false;

		for (auto it = m_Entries.begin(); it != m_Entries.end(); it++) {
			if (it->second == node) {
				if (next != nullptr && next->Info.Color == node->Info.Color)
					it->second = next;
				else
					it->second = nullptr;
				break;
			}
		}

		m_Nums--;
		CLEAR_BIT(m_State, node->Index);
		return;
	}

	CardNode* GetNext(CardNode* card) const {
		if (card == nullptr) {
			return GetHead();
		}
		return card->Next;
	};

	CardNode* GetFirstValid(Color color) const {
		auto it = m_Entries.find(color);
		if (it == m_Entries.end() || it->second == nullptr)
			return GetHead();
		return it->second;
	};

	CardNode* GetNextValid(CardNode* card, Color color) const {
		if (card == nullptr)
			return GetFirstValid(color);
		
		assert(card->Info.Color == color);
		auto next = card->Next;
		if (next != nullptr && next->Info.Color == card->Info.Color)
			return next;
		return nullptr;
	};

	uint64_t GetState() const { return m_State; };
};
