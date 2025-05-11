#include"tetris.hpp"

int main () {

	piece p(4, 199);
	p(0,1) = true;
	p(0,2) = true;
	p(0,3) = true;
	p(0,0) = true;
	p(1,0) = true;
	p(2,0) = true;
	p(3,0) = true;
	p(2,2) = true;
	p.print_ascii_art(std::cout);
	p.cut_row(2);
	std::cout<<std::endl;
	p.print_ascii_art(std::cout);
	std::cout<<std::endl;
	return 0;
}