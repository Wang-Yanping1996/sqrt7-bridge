#include "BridgeSolver.h"
#include <iostream>
int main() {
	/*
	Hand hand1({ Card{Color::Spade, Number::Eight} });
	Hand hand2({ Card{Color::Spade, Number::Jack} });
	Hand hand3({ Card{Color::Diamond, Number::Five} });
	Hand hand4({ Card{Color::Club, Number::Seven} });
	*/

	/*
	Hand hand1({
		Card{Color::Spade, Number::Ace}, Card{Color::Spade, Number::Queen}, Card{Color::Diamond, Number::Ten}, 
	});
	Hand hand2({
		Card{Color::Spade, Number::King}, Card{Color::Spade, Number::Three}, Card{Color::Diamond, Number::King},
	});
	Hand hand3({
		Card{Color::Heart, Number::Ace}, Card{Color::Diamond, Number::Queen}, Card{Color::Diamond, Number::Eight},
	});
	Hand hand4({
		Card{Color::Spade, Number::Two}, Card{Color::Heart, Number::Two}, Card{Color::Diamond, Number::Four},
	})
	*/;
	
	/*
	Hand hand1("hT7 d5");
	Hand hand2("h9 dJ7");
	Hand hand3("s9 dQT");
	Hand hand4("hK6 d9");
	*/
	
	
	Hand hand1("d5 hT");
	Hand hand2("dJ7");
	Hand hand3("dQ s9");
	Hand hand4("d9 h6");
	

	std::vector<Hand> hands = { hand1, hand2, hand3, hand4 };

	BridgeSolver solver(Color::Heart);
	int res = solver.SingleSolve(hands, 3, Color::NoTrump);
	std::cout << "res: " << res << std::endl;
	solver.printStates(hands);
	return 0;
}