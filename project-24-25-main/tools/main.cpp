#include"tetris.hpp"

int main () {
	piece p;
	
	try{
		std::cin >> p;
		
	} catch (tetris_exception const& e) {
		std::cout<<e.what()<<std::endl;
		return 1;
	}
	
	std::cout << p << std::endl;
	p.print_ascii_art(std::cout);
	return 0;
}