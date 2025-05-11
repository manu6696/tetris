#include"tetris.hpp"

int main () {

try{

	return 0;
} catch (tetris_exception const& e) {
	std::cout<<e.what()<<std::endl;
	return 1;
}

}