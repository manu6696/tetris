#include"tetris.hpp"

int main () {
	piece p;
	piece h;
	tetris s (4,4,0);
	try{
		std::cin >> p;
		std::cin >> h;
		s.add(p,0,3);
		s.add(h,0,3);
	} catch (tetris_exception const& e) {
		std::cout<<e.what()<<std::endl;
		return 1;
	}
	
	s.print_ascii_art(std::cout);
	return 0;
}

//4 75 ((()[][][]) [] [] [] )
//4 88 (([]()()[]) [] [] [] )

//4 88 ((()()()[]) [] [] [] )