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
	
	/*I
	Hand hand1("dK5 hAT7");
	Hand hand2("dJ76 h98");
	Hand hand3("dQT4 s98");
	Hand hand4("dA9 hK64");
	*/

	/*
	Hand hand1("sA753 d65");
	Hand hand2("sJ2 hT6 d87");
	Hand hand3("sQ8 dA c874");
	Hand hand4("sKT96 cQT");
	*/

	
	Hand hand1("sA753 hA87 dK652 c63");
	Hand hand2("sJ2 hKQT62 d8743 cKJ");
	Hand hand3("sQ8 hJ43 dAQT cA8754");
	Hand hand4("sKT964 h95 dJ9 cQT92");
	

	std::vector<Hand> hands = { hand1, hand2, hand3, hand4 };

	BridgeSolver solver(Color::NoTrump);
	int res = solver.Solve(hands, 2, Color::NoTrump);
//	int res = solver.SingleSolve(hands, 2, Color::NoTrump);
//	int res = solver.SingleSolveWithCut(hands, 2, Color::NoTrump, Result{ hand1.GetCards(), hand1.GetCards() });
	std::cout << "res: " << res << std::endl;
	solver.printStates(hands);
	return 0;
}