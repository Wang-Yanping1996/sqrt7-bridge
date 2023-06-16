#pragma once
#include <vector>
enum Color { Club = 1, Diamond, Heart, Spade, NoTrump };
enum Number
{
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

typedef std::vector<Card> Hand;