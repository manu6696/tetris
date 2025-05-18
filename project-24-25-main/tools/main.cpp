#include"tetris.hpp"
#include <fstream>

int main (int argc, char** argv) {

	//4 76 (([][][]()) [] [] [] )
	//4 88 (([]()()[]) [] [] [] )
	//4 88 ((()()()[]) [] [] [] )
	//4 22 ([][]()())
	//4 66 ([][]()())
	//4 77 ([][]()())
	//4 22 ([][][]([][][]()))

	if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " input_filename" << std::endl;
        return 1;
    }

    std::ifstream is(argv[1]);
    if (!is.good()) {
        std::cerr << "file not found; something is wrong!" << std::endl;
        return 1;
    }

	try{
		tetris s;
		is >> s;
		s.print_ascii_art(std::cout);
		std::cout<<std::endl;
		std::cout << s;
		
	} catch (tetris_exception const& e) {
		std::cout<<e.what()<<std::endl;
		return 1;
	}
	
	return 0;
}

