#include "tetris.hpp"


//////////////
/*	Piece	*/
//////////////

piece() : m_side(0), m_color(0), m_grid(nullptr) {}

piece::piece(uint32_t s, uint8_t c) : piece() {
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
	for(int i = 0; i < m_side; i++) m_grid[i] = new bool[m_side];
}

piece::piece(piece const& rhs) : piece(rhs.m_side, rhs.m_color) {
	for(int i = 0; i < m_side; i++) {
		for(int j = 0; j < m_side; j++)
			m_grid[i][j] = rhs.m_grid[i][j];
	}
}

piece::piece(piece&& rhs) : piece()  {
	*this = std::move(rhs); //copy assignment operator
}

piece::~piece() {
	for(int i = 0; i < m_side; i++) delete[] m_grid[i];
	delete[] m_grid;
	m_grid = nullptr;
	m_side = 0;
	m_color = 0;
}

piece& piece::operator=(piece const& rhs) {
	if(this != &rhs) {
		for(int i = 0; i < m_side; i++) delete[] m_grid[i];
		delete[] m_grid;
		m_grid = nullptr;
		m_side = 0;
		m_color = 0;

		m_side = rhs.m_side;
		m_color = rhs.m_color;
		m_grid = new bool*[m_side];
		for(int i = 0; i < m_side; i++) m_grid[i] = new bool[m_side];
		for(int i = 0; i < m_side; i++) {
			for(int j = 0; j < m_side; j++)
				m_grid[i][j] = rhs.m_grid[i][j];
		}
	}
	return *this;
}

piece& piece::operator=(piece&& rhs) {
	if(this != &rhs) {
		for(int i = 0; i < m_side; i++) delete[] m_grid[i];
		delete[] m_grid;
		m_grid = nullptr;

		m_side = rhs.m_side;
		m_color = rhs.m_color;
		m_grid = rhs.m_grid;

		rhs.m_side = 0;
		rhs.m_color = 0;
		rhs.m_grid = nullptr;
	}
	return *this;
}

bool piece::operator==(piece const& rhs) const {}
bool piece::operator!=(piece const& rhs) const {}

bool& piece::operator()(uint32_t i, uint32_t j) {}
bool piece::operator()(uint32_t i, uint32_t j) const {}

bool piece::empty(uint32_t i, uint32_t j, uint32_t s) const {}
bool piece::full(uint32_t i, uint32_t j, uint32_t s) const {}
bool piece::empty() const {}
bool piece::full() const {}

void piece::rotate() {}
void piece::cut_row(uint32_t i) {}
void piece::print_ascii_art(std::ostream& os) const {}

uint32_t piece::side() const {}
int piece::color() const {}










