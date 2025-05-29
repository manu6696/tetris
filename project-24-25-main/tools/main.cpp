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

	//4 66 (([]()[]()) (()()[][]) ([]()[]()) []  )
	//4 81 (([][]()()) ([][]()[]) ([]()[][]) [])
	//4 84 (([][]()()) ([][]()[]) ([]()[][]) [])
	//4 87 (([][]()()) ([][]()[]) ([]()[][]) [])
	//8 251 ([][] (([][]()())([][]()())[][]) (([][]()[])([][][]())([][]()[])([]()[]())))
	//8 249 ([][]([][]([][]()())([][]()()))([][]([][]()())([][]()())))
	//2 5 ([]()[]())

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

		/*
		tetris t(8,6,0);
		while(true) {
			piece p1;
			std::cin >> p1;
			int x = 0;
			std::cin >> x;
			t.insert(p1, x);
			t.print_ascii_art(std::cout);
		}
		*/
		tetris s;
		is >> s;
		s.print_ascii_art(std::cout);
		std::cout << s;
		/*
		
		tetris s;
		is >> s;
		s.print_ascii_art(std::cout);
		std::cout<<std::endl;
		std::cout << s;
		
		
		tetris t(8,6,0);
		piece p1;
		piece p2;
		piece p3;
		piece p4;
		std::cin>>p1;
		std::cin>>p2;
		std::cin>>p3;
		std::cin>>p4;
		t.insert(p1,2);
		t.insert(p2,4);
		t.insert(p3,-4);
		t.insert(p4,1);

		
		if(s == t) std::cout<<"S e T sono uguali"<<std::endl;
		else std::cout<<"FAIL"<<std::endl;
		
		
		
		piece p;
		is >> p;
		p.print_ascii_art(std::cout);
		*/
		
			
	} catch (tetris_exception const& e) {
		std::cout<<e.what()<<std::endl;
		return 1;
	}
	
	return 0;
}
