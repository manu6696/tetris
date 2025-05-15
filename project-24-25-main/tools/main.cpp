#include"tetris.hpp"

int main () {
	piece p;
	tetris s (64,64,0);
	try{
		std::cin >> p;
		s.containment(p, 0,0);
	} catch (tetris_exception const& e) {
		std::cout<<e.what()<<std::endl;
		return 1;
	}
	
	std::cout << p << std::endl;
	p.print_ascii_art(std::cout);
	return 0;
}