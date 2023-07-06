#pragma once
#include <vector>
#include <unordered_map>
#include <assert.h>
#include <algorithm>

#define SET_BIT_64(x, y) (x |= (1LL << (y)))
#define SET_LOW_BITS_64(x, y) (x |= ((1LL << (y)) - 1))

#define CLEAR_BIT_64(x, y) (x &= ~(1LL << (y)))

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

static std::unordered_map<Color, char> Color2Char = {
	{Color::Club, 'c'},
	{Color::Diamond, 'd'},
	{Color::Heart, 'h'},
	{Color::Spade, 's'}
};

static std::unordered_map<Number, char> Number2Char = {
	{Number::Two, '2'},
	{Number::Three, '3'},
	{Number::Four, '4'},
	{Number::Five, '5'},
	{Number::Six, '6'},
	{Number::Seven, '7'},
	{Number::Eight, '8'},
	{Number::Nine, '9'},
	{Number::Ten, 'T'},
	{Number::Jack, 'J'},
	{Number::Queen, 'Q'},
	{Number::King, 'K'},
	{Number::Ace, 'A'},
};

struct Card
{
	Color  Color;
	Number Number;
	char PrintNumber() { return Number2Char[this->Number]; };
	char PrintColor() { return Color2Char[this->Color]; };
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

	void deleteAllNodes() {
		for (auto it = m_Indices.begin(); it != m_Indices.end(); it++) {
			free(*it);
		}
	};
	void initialByVector(std::vector<Card> cards) {
		deleteAllNodes();
		m_Indices.clear();
		m_Entries.clear();

		m_Entries[Color::Club] = nullptr;
		m_Entries[Color::Diamond] = nullptr;
		m_Entries[Color::Heart] = nullptr;
		m_Entries[Color::Spade] = nullptr;

		std::sort(cards.begin(), cards.end(), [](Card& x, Card& y) {
			return x.Color == y.Color ? x.Number > y.Number : x.Color > y.Color;
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
			m_Indices.push_back(cur);
		}

		m_Nums = cards.size();
		m_State = 0;
		SET_LOW_BITS_64(m_State, m_Nums);
	};
public:
	std::unordered_map<char, Color> colorMap = {
			{'S', Color::Spade}, {'s', Color::Spade},
			{'H', Color::Heart}, {'h', Color::Heart},
			{'D', Color::Diamond}, {'d', Color::Diamond},
			{'C', Color::Club}, {'c', Color::Club},
	};
	std::unordered_map<char, Number> numberMap = {
		{'2', Number::Two},
		{'3', Number::Three},
		{'4', Number::Four},
		{'5', Number::Five},
		{'6', Number::Six},
		{'7', Number::Seven},
		{'8', Number::Eight},
		{'9', Number::Nine},
		{'T', Number::Ten},		{'t', Number::Ten},
		{'J', Number::Jack},	{'j', Number::Jack},
		{'Q', Number::Queen},	{'q', Number::Queen},
		{'K', Number::King},	{'k', Number::King},
		{'A', Number::Ace},		{'a', Number::Ace},
	};
	Hand() {
		m_Entries[Color::Club] = nullptr;
		m_Entries[Color::Diamond] = nullptr;
		m_Entries[Color::Heart] = nullptr;
		m_Entries[Color::Spade] = nullptr;
		m_Nums = 0;
		m_State = 0;
	};
	Hand(const char* str) {
		std::vector<Card> cards;
		Color color = Color::NoTrump;
		int len = strlen(str);
		for (int i = 0; i < len; i++) {
			char ch = str[i];
			if (colorMap.find(ch) != colorMap.end()) {
				color = colorMap[ch];
			}
			else if (numberMap.find(ch) != numberMap.end()) {
				cards.push_back(Card{ color, numberMap[ch] });
			}
			else {
				// error symbol
			}
		}
		initialByVector(cards);
	};
	Hand(std::vector<Card>&& cards) {
		initialByVector(cards);
	};
	Hand(Hand const& hand) {
		if (this == &hand) {
			return;
		}

		m_Entries[Color::Club] = nullptr;
		m_Entries[Color::Diamond] = nullptr;
		m_Entries[Color::Heart] = nullptr;
		m_Entries[Color::Spade] = nullptr;
		CardNode* prev = nullptr;
		for (int i = 0; i < (int)hand.m_Indices.size(); i++) {
			CardNode* cur = new CardNode();
			cur->Index = i;
			cur->Info = hand.m_Indices[i]->Info;
			cur->IsValid = hand.m_Indices[i]->IsValid;
			cur->Prev = prev;
			if (prev != nullptr)
				prev->Next = cur;
			prev = cur;
			if (this->m_Entries[cur->Info.Color] == nullptr) {
				this->m_Entries[cur->Info.Color] = cur;
			}
			this->m_Indices.push_back(cur);
		}
		this->m_State = hand.m_State, this->m_Nums = hand.m_Nums;
		return;
	}

	~Hand() {
		deleteAllNodes();
	};

	Hand& operator= (Hand const& hand) {
		if (this == &hand) {
			return *this;
		}

		m_Entries[Color::Club] = nullptr;
		m_Entries[Color::Diamond] = nullptr;
		m_Entries[Color::Heart] = nullptr;
		m_Entries[Color::Spade] = nullptr;
		CardNode* prev = nullptr;
		for (int i = 0; i < (int)hand.m_Indices.size(); i++) {
			CardNode* cur = new CardNode();
			cur->Index = i;
			cur->Info = hand.m_Indices[i]->Info;
			cur->IsValid = hand.m_Indices[i]->IsValid;
			cur->Prev = prev;
			if (prev != nullptr)
				prev->Next = cur;
			prev = cur;
			if (this->m_Entries[cur->Info.Color] == nullptr) {
				this->m_Entries[cur->Info.Color] = cur;
			}
			this->m_Indices.push_back(cur);
		}
		this->m_State = hand.m_State, this->m_Nums = hand.m_Nums;
		return *this;
	}

	Card GetCard(int i) const { return m_Indices[i]->Info; };
	int GetCards() const { return m_Nums; };
	
	CardNode* getHead() const {
		for (int i = Color::Spade; i >= Color::Club; i--) {
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
		SET_BIT_64(m_State, card->Index);

		Color color = card->Info.Color;
		auto entry = m_Entries[color];
		if (entry != nullptr && card->Info.Number < entry->Info.Number) {
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
		CLEAR_BIT_64(m_State, node->Index);
		return;
	}

	CardNode* getNext(CardNode* card) const {
		assert(card != nullptr);
//		if (card == nullptr) {
//			return getHead();
//		}
		return card->Next;
	};

	CardNode* getFirstValid(Color color) const {
		auto it = m_Entries.find(color);
		if (it == m_Entries.end() || it->second == nullptr)
			return getHead();
		return it->second;
	};

	CardNode* getNextValid(CardNode* card, Color color) const {
		assert(card != nullptr);
//		if (card == nullptr)
//			return getFirstValid(color);
		
		// assert(card->Info.Color == color);
		auto next = card->Next;
		if (card->Info.Color != color)
			return next;
		if (next != nullptr && next->Info.Color == card->Info.Color)
			return next;
		return nullptr;
	};

	uint64_t GetState() const { return m_State; };

	// will modify hand
	CardNode* TakeHeader() {
		auto header = getHead();
		if (header == nullptr)
			return nullptr;
		Remove(header);
		return header;
	}
	CardNode* IterateTakeNext(CardNode* card) {
		auto next = getNext(card);
		ImmediateInsert(card);
		if (next == nullptr)
			return nullptr;
		Remove(next);
		return next;
	}

	CardNode* TakeFirstValid(Color color) {
		auto card = getFirstValid(color);
		if (card == nullptr)
			return nullptr;
		Remove(card);
		return card;
	};
	CardNode* IterateTakeNextValid(CardNode* card, Color color) {
		auto next = getNextValid(card, color);
		ImmediateInsert(card);
		if (next == nullptr)
			return nullptr;
		Remove(next);
		return next;
	};

	bool isSameInHand(Card c1, Card c2) {

	};

};
