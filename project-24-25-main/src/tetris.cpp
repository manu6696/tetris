#include "tetris.hpp"


//////////////
/*	Piece	*/
//////////////

piece::piece() : m_side(0), m_color(0), m_grid(nullptr) {}

piece::piece(uint32_t s, uint8_t c) : piece() {
	if(c == 0 ) throw tetris_exception("Color not valid.");

	bool power_of = false;
	uint32_t i = 1;
	while(!power_of && s >= i ) {
		if(s == i) power_of = true;
		i *= 2;
	}

	if(!power_of) throw tetris_exception("Size is not a power of 2.");

	m_side = s;
	m_color = c;
	m_grid = new bool*[m_side];
	for(uint32_t i = 0; i < m_side; i++) m_grid[i] = new bool[m_side];
}

piece::piece(piece const& rhs) : piece(rhs.m_side, rhs.m_color) {
	for(uint32_t i = 0; i < m_side; i++) {
		for(uint32_t j = 0; j < m_side; j++)
			m_grid[i][j] = rhs.m_grid[i][j];
	}
}

piece::piece(piece&& rhs) : piece()  {
	*this = std::move(rhs); //copy assignment operator
}

piece::~piece() {
	for(uint32_t i = 0; i < m_side; i++) delete[] m_grid[i];
	delete[] m_grid;
	m_grid = nullptr;
	m_side = 0;
	m_color = 0;
}

piece& piece::operator=(piece const& rhs) {
	if(this != &rhs) {
		for(uint32_t i = 0; i < m_side; i++) delete[] m_grid[i];
		delete[] m_grid;
		m_grid = nullptr;
		m_side = 0;
		m_color = 0;

		m_side = rhs.m_side;
		m_color = rhs.m_color;
		m_grid = new bool*[m_side];
		for(uint32_t i = 0; i < m_side; i++) m_grid[i] = new bool[m_side];
		for(uint32_t i = 0; i < m_side; i++) {
			for(uint32_t j = 0; j < m_side; j++)
				m_grid[i][j] = rhs.m_grid[i][j];
		}
	}
	return *this;
}

piece& piece::operator=(piece&& rhs) {
	if(this != &rhs) {
		for(uint32_t i = 0; i < m_side; i++) delete[] m_grid[i];
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

bool piece::operator==(piece const& rhs) const {
	if(rhs.m_color != m_color || rhs.m_side != m_side) return false;
	for(uint32_t i = 0; i < m_side ; i++) {
		for(uint32_t j = 0; j < m_side ; j++) {
			if(m_grid[i][j] != rhs.m_grid[i][j]) return false;
		}
	}
	return true;
}

bool piece::operator!=(piece const& rhs) const {
	return !(*this == rhs);
}

bool& piece::operator()(uint32_t i, uint32_t j) {
	if(i >= m_side || j >= m_side) throw tetris_exception("Out of bound access.");
	return m_grid[i][j];
}
bool piece::operator()(uint32_t i, uint32_t j) const {
	if(i >= m_side || j >= m_side) throw tetris_exception("Out of bound access.");
	return m_grid[i][j];
}

bool piece::empty(uint32_t i, uint32_t j, uint32_t s) const {
	if(i >= s || j >= s) throw tetris_exception("Out of bound access.");
	bool res = true;
	for(uint32_t a = i; a < s; a++) {
		for(uint32_t b = j; b < s; b++) {
			if(m_grid[i][j]) res = false;
		}
	}
	return res;
}

bool piece::full(uint32_t i, uint32_t j, uint32_t s) const {
	if(i >= s || j >= s) throw tetris_exception("Out of bound access.");
	bool res = true;
	for(uint32_t a = i; a < s; a++) {
		for(uint32_t b = j; b < s; b++) {
			if(!m_grid[i][j]) res = false;
		}
	}
	return res;
}

bool piece::empty() const {
	bool res = true;
	if(m_side > 0) {
		for(uint32_t i = 0; i < m_side; i++) {
			for(uint32_t j = 0; j < m_side; j++) {
				if(m_grid[i][j]) res = false;
			}
		}
	}
	return res;
}

bool piece::full() const {
	bool res = true;
	if(m_side > 0) {
		for(uint32_t i = 0; i < m_side; i++) {
			for(uint32_t j = 0; j < m_side; j++) {
				if(!m_grid[i][j]) res = false;
			}
		}
	} else {
		res = false;
	}
	return res;
}

void piece::rotate() {
	if(m_side > 0) {
		bool t_grid[m_side][m_side];
		for(uint32_t i = 0; i < m_side; i++) {
			for(uint32_t j = 0; j < m_side; j++) {
				t_grid[i][j] = m_grid[i][j];
			}
		}
		for(uint32_t i = 0; i < m_side; i++) {
			for(uint32_t j = 0; j < m_side; j++) {
				m_grid[j][m_side-i-1] = t_grid[i][j];
			}
		}
	}
}

void piece::cut_row(uint32_t i) {
	if(i >= m_side) throw tetris_exception("Wrong value for i.");
	for(uint32_t j = i ; j > 0; j--) {
		for(uint32_t k = 0; k < m_side; k++) {
			m_grid[j][k] = m_grid[j-1][k];
		}
	}
	for(uint32_t j = 0; j < m_side; j++) m_grid[0][j] = false;
}

void piece::print_ascii_art(std::ostream& os) const {
	for(uint32_t i = 0; i < m_side; i++) {
		for(uint32_t j = 0; j < m_side; j++) {
			 if (m_grid[i][j]) {
			    os << "\033[48;5;" << int(m_color) << "m" << ' ' << "\033[m";
			 } else {
			    os << ' ';
			 }
		}
		os << std::endl;
	}
}

uint32_t piece::side() const {
	return m_side;
}
int piece::color() const {
	return m_color;
}


/*
Context-free grammar:
	PIECE -> SIDE | COLOR | (TL , TR , BL , BR)
	TL -> [] | () | (PIECE)
	TR -> [] | () | (PIECE)
	BL -> [] | () | (PIECE)
	BR -> [] | () | (PIECE)
*/

void skip(std::istream& is) {
    char c = 0;
    is >> c;
    is.putback(c);
}

piece TL(std::istream& is);
piece TR(std::istream& is);
piece BL(std::istream& is);
piece BR(std::istream& is);

void set_true(piece& p)  {
	for(uint32_t i = 0; i < p.side(); i++) {
		for(uint32_t j = 0; j < p.side(); j++)
			p(i,j) = true;
	}
}

void set_false(piece& p)  {
	for(uint32_t i = 0; i < p.side(); i++) {
		for(uint32_t j = 0; j < p.side(); j++)
			p(i,j) = false;
	}
}

piece PIECE(std::istream& is) {
	// PIECE -> SIDE | COLOR | (TL , TR , BL , BR)

	piece p;
	assert(p.empty());

	skip(is);
	int color = 0;
	int side = 0;

	char c = is.peek();
	if(c != '(' && c != ')' && c != '[' && c != ']') {
		if((c <= '0' || c > '9') && c != '(' && c != ')') {throw tetris_exception("Wrong format.");}
		while(c > '0' && c <= '9'){
			is >> c;
			if(c <= '0' || c > '9') {throw tetris_exception("Wrong format.");}
			side += c - '0';
			c = is.peek();
			if(c > '0' && c <= '9') side *= 10;
		}
		skip(is);
		c = is.peek();
		if((c <= '0' || c > '9') && c != '(' && c != ')') {throw tetris_exception("Wrong format.");}
		while(c > '0' && c <= '9'){
			is >> c;
			if(c <= '0' || c > '9') {throw tetris_exception("Wrong format.");}
			color += c - '0';
			c = is.peek();
			if(c > '0' && c <= '9') color *= 10;
		}
		piece s(side,color);
		p = s;
	}

	
	skip(is);
	c = is.peek();

	if(c == '(') {
		p = TL(is);
		p = TR(is);
		p = BL(is);
		p = BR(is);
	}

	skip(is);
	return p;
}

piece TL(std::istream& is) {
	// TL -> [] | () | (PIECE)

	skip(is);
	piece p;

    char c = 0;
    is >> c;
    std::cout<<"c è "<<c<<std::endl;
    char h = is.peek();
    std::cout<<"h è "<<h<<std::endl;
    if (c != '(' && c != '[' ) { throw tetris_exception("Expected TL '(' or '['"); }
    
    skip(is);
    c = is.peek();
    if(c == ']') {
    	set_false(p);
        is >> c;
        skip(is);
        return p;
    } else if (c == ')') {
    	set_true(p);
        is >> c;
        skip(is);
        return p;
    } else {
    	skip(is);
    	piece n = PIECE(is);
    	is >> c;
    }


    skip(is);
    return p;
}

piece TR(std::istream& is) {
	// TR -> [] | () | (PIECE)

	skip(is);
	piece p;

    char c = 0;
    is >> c;
    if (c != '(' && c != '[' ) { throw tetris_exception("Expected TR '(' or '['"); }
    
    skip(is);
    c = is.peek();
    if(c == ']') {
    	set_false(p);
        is >> c;
        skip(is);
        return p;
    } else if (c == ')') {
    	set_true(p);
        is >> c;
        skip(is);
        return p;
    } else {
    	skip(is);
    	piece n = PIECE(is);
    	is >> c;
    }


    skip(is);
    return p;
}


piece BL(std::istream& is) {
	// BL -> [] | () | (PIECE)

	skip(is);
	piece p;

    char c = 0;
    is >> c;
    if (c != '(' && c != '[' ) { throw tetris_exception("Expected BL '(' or '['"); }
    
    skip(is);
    c = is.peek();
    if(c == ']') {
    	set_false(p);
        is >> c;
        skip(is);
        return p;
    } else if (c == ')') {
    	set_true(p);
        is >> c;
        skip(is);
        return p;
    } else {
    	skip(is);
    	piece n = PIECE(is);
    	is >> c;
    }


    skip(is);
    return p;
}

piece BR(std::istream& is) {
	// BR -> [] | () | (PIECE)

	skip(is);
	piece p;

    char c = 0;
    is >> c;
    if (c != '(' && c != '[' ) { throw tetris_exception("Expected BR '(' or '['"); }
    
    skip(is);
    c = is.peek();
    if(c == ']') {
    	set_false(p);
        is >> c;
        skip(is);
        return p;
    } else if (c == ')') {
    	set_true(p);
        is >> c;
        skip(is);
        return p;
    } else {
    	skip(is);
    	piece n = PIECE(is);
    	is >> c;
    }


    skip(is);
    return p;
}

std::istream& operator>>(std::istream& is, piece& p) {
    p = PIECE(is);
    return is;
}



std::ostream& operator<<(std::ostream& os, piece const& p) {

	os << p.side();
	os << ' ';
	os << p.color();
	os << ' ';
	return os;
}







