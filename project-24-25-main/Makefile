all: OPTIONS=-std=c++17 -O3 -DNDEBUG -Wall -Wextra
all: build/main 

debug: OPTIONS=-std=c++17 -O0 -g -DDEBUG -Wall -Wextra
debug: build/main

sanitizers: OPTIONS=-std=c++17 -fsanitize=address -fno-omit-frame-pointer -g -DDEBUG -Wall -Wextra
sanitizers: build/main

build/main: src/tetris.cpp include/tetris.hpp tools/main.cpp
	g++ ${OPTIONS} tools/main.cpp src/tetris.cpp -I include -o build/main

clean:
	rm -rf build/*