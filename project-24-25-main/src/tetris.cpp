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

	if(!power_of) throw tetris_exception("Side is not a power of 2.");

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
	*this = std::move(rhs);
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
	PIECE -> SIDE | COLOR | QUAD
	QUAD -> (TL, TR, BL, BR) | () | []

	EX. 4 75 ( ( []()[]() )  ( ()[]()[] )   ( []()()() )   ( ()[]()() ) )
*/

void skip(std::istream& is) {
    char c = 0;
    is >> c;
    is.putback(c);
}

piece QUAD(std::istream& is, uint32_t side, uint8_t color);

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
	// PIECE -> SIDE | COLOR | QUAD

	piece p;
	assert(p.empty());

	skip(is);
	uint8_t color = 0;
	uint32_t side = 0;

	char c = is.peek();
	if(c >= '0' and c <= '9') {
		double x = 0.0;
		is >> x;
		side = x;
		skip(is);
		if(c >= '0' and c <= '9') {
			double y = 0.0;
			is >> y;
			color = y;
			skip(is);	
		} else {
			throw tetris_exception("Invalid color.");
		}
		piece s(side,color);
		p = s;
	} else {
		throw tetris_exception("Invalid side.");
	}

	skip(is);
	c = is.peek();
	if (c == '(' || c == '[') {
		p = QUAD(is, side, color);
	} else {
		throw tetris_exception("Expected '(' or '['");
	}
	return p;
}

piece QUAD(std::istream& is, uint32_t side, uint8_t color) {
	// QUAD -> (TL, TR, BL, BR) | () | []

	piece p(side, color);
	piece tl(side, color);
	piece tr(side, color);
	piece bl(side, color);
	piece br(side, color);
	set_false(p);

	skip(is);
	char c = 0;
    is >> c;
    skip(is);
    if(c != '(' && c != '[' && c != ')' && c != ']') { throw tetris_exception("Invalid character."); }
    //std::cout<<"c prima = "<<c << " e size = " << side <<std::endl;
    char c_next = is.peek();
    if(c != '(' && c != '[' && c != ')' && c != ']') { throw tetris_exception("Invalid character."); }
    if(c_next != '(' && c_next != '[' && c_next != ')' && c_next != ']') { throw tetris_exception("Invalid character."); }
    if(c_next == ')' || c_next == ']') {
    	is >> c;
    	//std::cout<<"c dentro = "<<c <<std::endl;
    }

	if (c == '(' || c == '[') {
		tl = QUAD(is, side/2, color);
		//std::cout<<"tl ------> tr"<<std::endl;
		tr = QUAD(is, side/2, color);
		//std::cout<<"tr ------> bl"<<std::endl;
		bl = QUAD(is, side/2, color);
		//std::cout<<"bl ------> br"<<std::endl;
		br = QUAD(is, side/2, color);
		//std::cout<<"---------- end"<<std::endl;

		for(uint32_t i = 0; i < side / 2; i++) {			//TL
			for(uint32_t j = 0; j < side / 2; j++) {
				p(i,j) = tl(i,j);
			}
		}

		for(uint32_t i = 0; i < side / 2; i++) {			//TR
			for(uint32_t j = side / 2; j < side; j++) {
				p(i,j) = tr(i,j - side / 2);
			}
		}

		for(uint32_t i = side / 2; i < side; i++) {			//BL
			for(uint32_t j = 0; j < side / 2; j++) {
				p(i,j) = bl(i - side / 2,j);
			}
		}

		for(uint32_t i = side / 2; i < side; i++) {			//BR
			for(uint32_t j = side / 2; j < side; j++) {
				p(i,j) = br(i - side / 2,j - side / 2);
			}
		}

		skip(is);
		is >> c;
		if(c != ')') { throw tetris_exception("Missing ')'"); }
		//std::cout<<"c consumata dai livelli = "<<c<<std::endl;
	} else if (c == ']') {
		set_false(p);
	} else if (c == ')') {
		set_true(p);
	}
	return p;
}

std::istream& operator>>(std::istream& is, piece& p) {
    p = PIECE(is);
    return is;
}

int check(piece const& p) {
	int res = -1;
	uint32_t qty_empty = 0;
	uint32_t qty_full = 0;
	for(uint32_t i = 0; i < p.side(); i++) {
		for(uint32_t j = 0; j < p.side(); j++) {
			if(p(i,j)) qty_full += 1;
			else qty_empty += 1;
		}
	}
	if(qty_full == p.side() * p.side()) res = 1;
	if(qty_empty == p.side() * p.side()) res = 2;
	return res;
}

void scrivi(std::ostream& os, uint32_t side, piece const& p) {
	if(side > 1) {
		piece tl(side/2, 1);
		piece tr(side/2, 1);
		piece bl(side/2, 1);
		piece br(side/2, 1);

		for(uint32_t i = 0; i < side / 2; i++) {			//TL
			for(uint32_t j = 0; j < side / 2; j++) {
				tl(i,j) = p(i,j);
			}
		}

		for(uint32_t i = 0; i < side / 2; i++) {			//TR
			for(uint32_t j = 0; j < side / 2; j++) {
				tr(i,j) = p(i,j + side / 2);
			}
		}

		for(uint32_t i = 0; i < side / 2; i++) {			//BL
			for(uint32_t j = 0; j < side / 2; j++) {
				bl(i,j) = p(i + side / 2,j);
			}
		}

		for(uint32_t i = 0; i < side / 2; i++) {			//BR
			for(uint32_t j = 0; j < side / 2; j++) {
				br(i,j) = p(i + side / 2,j + side / 2);
			}
		}

		int skip_tl = check(tl);
		int skip_tr = check(tr);
		int skip_bl = check(bl);
		int skip_br = check(br);
		bool sk1 = (skip_tl == 1 && skip_tr == 1 && skip_bl == 1 && skip_br == 1);
		bool sk2 = (skip_tl == 2 && skip_tr == 2 && skip_bl == 2 && skip_br == 2);
		if(sk2) {
			os << '[';
		} else {
			os << '(';
		}
		
		if(!sk1 && !sk2) {
			scrivi(os, side / 2, tl);
			scrivi(os, side / 2, tr);
			scrivi(os, side / 2, bl);
			scrivi(os, side / 2, br);
		}

		if(sk2) {
			os << ']';
		} else {
			os << ')';
		}
	}

	if(side == 1) {
		if(p(0,0)) {
			os << '(';
			os << ')';
		} else {
			os << '[';
			os << ']';
		}
	}
}

std::ostream& operator<<(std::ostream& os, piece const& p) {
	os << p.side();
	os << ' ';
	os << p.color();
	os << ' ';
	scrivi(os, p.side(), p);
	return os;
}



//////////////
/*	Tetris	*/
//////////////

tetris::tetris() : m_score(0), m_width(0), m_height(0), m_field(nullptr) {}

tetris::tetris(uint32_t w, uint32_t h, uint32_t s) : tetris() {
	if(w == 0 || h == 0) {
		throw tetris_exception("Game board dimension can't be equal to 0.");
	}
	m_width = w;
	m_height = h;
	m_score = s;
}

tetris::tetris(tetris const& rhs) {
	m_width = rhs.m_width;
	m_height = rhs.m_height;
	m_score = rhs.m_score;

	node* pc = rhs.m_field;
	m_field = nullptr;
	node* tail = m_field;
	while(pc) {
		node* n = new node{pc->tp, nullptr};
		if(m_field == nullptr) {
			m_field = n;
		} else {
			tail->next = n;
		}
		tail = n;
		pc = pc->next;
	}
}

tetris::tetris(tetris&& rhs) {
	*this = std::move(rhs);
}

tetris::~tetris() {
	m_score = 0;
	m_width = 0;
	m_height = 0;

	while(m_field) {
		node* tmp = m_field;
		m_field = m_field->next;
		delete tmp;
	}
}

tetris& tetris::operator=(tetris const& rhs) {
	if(this != &rhs) {
		while(m_field) {
			node* tmp = m_field;
			m_field = m_field->next;
			delete tmp;
		}

		m_score = rhs.m_score;
		m_width = rhs.m_width;
		m_height = rhs.m_height;
		node* pc = rhs.m_field;
		m_field = nullptr;
		node* tail = m_field;
		while(pc) {
			node* n = new node{pc->tp, nullptr};
			if(m_field == nullptr) {
				m_field = n;
			} else {
				tail->next = n;
			}
			tail = n;
			pc = pc->next;
		}
	}
	return *this;
}

tetris& tetris::operator=(tetris&& rhs) {
	if(this != &rhs) {
		while(m_field) {
			node* tmp = m_field;
			m_field = m_field->next;
			delete tmp;
		}

		m_score = rhs.m_score;
		m_width = rhs.m_width;
		m_height = rhs.m_height;
		m_field = rhs.m_field;

		rhs.m_score = 0;
		rhs.m_width = 0;
		rhs.m_height = 0;
		rhs.m_field = nullptr;
	}
	return *this;
}

bool tetris::operator==(tetris const& rhs) const {
	bool equal = true;
	if(m_score != rhs.m_score) equal = false;
	if(m_width != rhs.m_width) equal = false;
	if(m_height != rhs.m_height) equal = false;

	node* pc_rhs = rhs.m_field;
	node* pc_this = this->m_field;
	while(equal && pc_rhs && pc_this) {
		if(pc_rhs->tp.x != pc_this->tp.x) equal = false;
		if(pc_rhs->tp.y != pc_this->tp.y) equal = false;
		if(pc_rhs->tp.p != pc_this->tp.p) equal = false;
		pc_this = pc_this->next;
		pc_rhs = pc_rhs->next;
	}

	if(pc_rhs && !pc_this) equal = false;
	if(!pc_rhs && pc_this) equal = false;
	return equal;
}

bool tetris::operator!=(tetris const& rhs) const {
	return !(*this == rhs);
}

bool tetris::containment(piece const& p, int x, int y) const {
	bool contain = true;
	int tmp_height = m_height;
	int tmp_width = m_width;

	int offset_y = y - (m_height - 1);
	int offset_x = x;

	if(y < 0) contain = false;
	bool a[m_height][m_width];

	for(uint32_t i = 0; i < m_width; i++) {
		for(uint32_t j = 0; j < m_height; j++) {
			a[j][i] = false;
		}
	}

	for(uint32_t i = 0; i < p.side(); i++) {
		for(uint32_t j = 0; j < p.side(); j++) {
			int py = offset_y + i;
			int px = offset_x + j;
			if (py >= 0 && py < tmp_height && px >= 0 && px < tmp_width){
				a[py][px] = p(i,j);
			} else {
				if(p(i,j)) contain = false;
			}
		}
	}
	
	if(contain) {
		node* pc = m_field;
		while(pc) {
			piece tmp = pc->tp.p;
			bool b[m_height][m_width];
			
			for(uint32_t i = 0; i < m_width; i++) {
				for(uint32_t j = 0; j < m_height; j++) {
					b[j][i] = false;
				}
			}

			for(uint32_t i = 0; i < tmp.side(); i++) {
				for(uint32_t j = 0; j < tmp.side(); j++) {
				int py = offset_y + i;
				int px = offset_x + j;
					if (py >= 0 && py < tmp_height && px >= 0 && px < tmp_width) {
						b[py][px] = tmp(i,j);
					}
				}
			}

			for(uint32_t i = 0; i < m_width; i++) {
				for(uint32_t j = 0; j < m_height; j++) {
					if(a[j][i] && b[j][i]) contain = false;
				}
			}

			pc = pc->next;
		}
		
	}
	return contain;
}

void tetris::add(piece const& p, int x, int y) {
	if(containment(p,x,y)) {
		tetris_piece np{p,x,y};
		node* new_piece = new node{np, nullptr};
		node* tmp = m_field;
		m_field = new_piece;
		m_field->next = tmp;
	} else {
		throw tetris_exception{"Piece cannot be contained."};
	}

}

void tetris::insert(piece const& p, int x) {
	int y = 0;
	bool contain = false;
	while(!contain && y < m_height) {
		contain = containment(p,x,y);
		y++;
	}

	if(!contain) {throw tetris_exception("GAME OVER");}

	add(p,x,y);

	for(uint32_t i = 0; i < m_height; i++) {
		bool cut = true;
		for(uint32_t j = 0; j < m_width; j++) {
			node* pc = m_field;
			while(pc && cut) {
				piece tmp = pc->tp.p;
				int offset_y = pc->tp.y + 1 - tmp.side();
				int offset_x = pc->tp.x;
				uint32_t a = j - offset_x;
				uint32_t b = i - offset_y;
				if(a < tmp.side() && b < tmp.side()) {
					if (!tmp(a,b)) {
				   		cut = false;
					} 
				}
				pc = pc->next;
			}
			if(cut) {
				node* pc_cut = m_field;
				while(pc_cut) {
					piece tmp = pc_cut->tp.p;
					tmp.cut_row(i);
					pc_cut = pc_cut->next;
				}
				m_score += m_width;
			}
			
		}

	}


}

void tetris::print_ascii_art(std::ostream& os) const {
	for(uint32_t i = 0; i < m_height; i++) {
		for(uint32_t j = 0; j < m_width; j++) {
			bool skip = false;
			if(j == 0) os << '|';
			node* pc = m_field;
			while(pc && !skip) {
				piece tmp = pc->tp.p;
				int offset_y = pc->tp.y + 1 - tmp.side();
				int offset_x = pc->tp.x;
				uint32_t a = j - offset_x;
				uint32_t b = i - offset_y;
				if(a < tmp.side() && b < tmp.side()) {
					if (tmp(a,b)) {
				   		os << "\033[48;5;" << int(tmp.color()) << "m" << ' ' << "\033[m";
						skip = true;
					} 
				}
				pc = pc->next;
			}
			if(!skip) os << ' ';
			if(j == m_width - 1) os << '|';
		}
		os << std::endl;
	}
	os << '_';
	for(uint32_t i = 0; i < m_height; i++) {
		os << '_';
	}
	os << '_';
	os << std::endl;
}














