#include "BridgeSolver.h"
#include <iostream>
int main() {
	Hand hand1({
		Card{Color::Spade, Number::Ace}, Card{Color::Spade, Number::Queen},
	});
	Hand hand2({
		Card{Color::Spade, Number::King}, Card{Color::Spade, Number::Three},
	});
	Hand hand3({
		Card{Color::Heart, Number::Ace}, Card{Color::Diamond, Number::Queen},
	});
	Hand hand4({
		Card{Color::Spade, Number::Two}, Card{Color::Heart, Number::Two},
	});

	std::vector<Hand> hands = { hand1, hand2, hand3, hand4 } ;
	BridgeSolver solver;
	int res = solver.Solve(hands, 0, Color::Spade);
	std::cout << "res: " << res << std::endl;
	return 0;
}