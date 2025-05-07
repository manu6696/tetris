#include "tetris.hpp"


//////////////
/*	Piece	*/
//////////////

piece() : m_side(0), m_color(0), m_grid(nullptr) {}

piece(uint32_t s, uint8_t c) {

	if(c == 0 ) throw tetris_exception("Color not valid!");

	bool power_of = false;
	uint32_t i = 1;
	while(!power_of && s >= i ) {
		if(s == i) power_of = true;
		i *= 2;
	}

	if(!power_of) throw tetris_exception("Size is not a power of 2!");

	m_side = s;
	m_color = c;
	m_grid = new bool*[m_side];
	for(int i = 0; i < m_side; i++) m_grid.at(i) = new bool[m_side];

}

piece(piece const& rhs) {

	if(rhs.m_color == 0 ) throw tetris_exception("Color not valid!");

	bool power_of = false;
	uint32_t i = 1;
	while(!power_of && rhs.m_side >= i ) {
		if(rhs.m_side == i) power_of = true;
		i *= 2;
	}

	if(!power_of) throw tetris_exception("Size is not a power of 2!");

	m_color = rhs.m_color;
	m_side = rhs.m_side;
	m_grid = new bool*[m_side];
	for(int i = 0; i < m_side; i++) m_grid.at(i) = new bool[m_side];

	for(int i = 0; i < m_side; i++) {
		for(int j = 0; j < m_side; j++)
			m_grid.at(i).at(j) = rhs.at(i).at(j);
	}
}

piece(piece&& rhs) {}

~piece() {}

piece& operator=(piece const& rhs) {}
piece& operator=(piece&& rhs) {}

bool operator==(piece const& rhs) const {}
bool operator!=(piece const& rhs) const {}

bool& operator()(uint32_t i, uint32_t j) {}
bool operator()(uint32_t i, uint32_t j) const {}

bool empty(uint32_t i, uint32_t j, uint32_t s) const {}
bool full(uint32_t i, uint32_t j, uint32_t s) const {}
bool empty() const {}
bool full() const {}

void rotate() {}
void cut_row(uint32_t i) {}
void print_ascii_art(std::ostream& os) const {}

uint32_t side() const {}
int color() const {}