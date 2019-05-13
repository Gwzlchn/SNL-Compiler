#include "SLR1.h"
#include <algorithm>

map<SLR1Token, pair<string, bool>> TokenNameMap{
	{Token_S,{"S",NOTTER}},
	{Token_E,{"E",NOTTER}},
	{Token_T,{"T",NOTTER}},
	{Token_P,{"P",NOTTER}},
	{Token_id,{"id",TERMIN}},
	{Token_left_paren,{"(",TERMIN}},
	{Token_right_paren,{")",TERMIN}},
	{Token_add,{"+",TERMIN}},
	{Token_mul,{"*",TERMIN}},
	{Token_eof,{"$",TERMIN}},
	{Token_start,{"#",TERMIN}},
	{Token_Blank,{"~",TERMIN}}
};

bool Production::operator<(const Production& prod) const
{
	return this->m_id < prod.m_id;
}

int Production::get_id() {
	return this->m_id;
}
Production::Production() {
	m_left = Token_Blank;
	m_right = { Token_Blank };
	m_id = -1;
	m_look_ahead_idx = -1;
}

Production::Production(SLR1Token left, vector<SLR1Token> right, int id, int idx)
{
	//先验证产生式是否正确
	if (TokenNameMap.find(left) == TokenNameMap.end()) {
		std::cerr << "this Token  cannot identified" << std::endl;
		return;
	};
	size_t right_size = right.size();
	for (size_t i = 0; i < right_size; i++) {
		if (TokenNameMap.find(right[i]) == TokenNameMap.end()) {
			std::cerr << "this Token  cannot identified" << std::endl;
			return;
		}
	}


	m_left = left;
	m_right = right;
	m_id = id;
	m_look_ahead_idx = idx;
	return;
}

inline bool Production::isLeftTerminal() {
	auto iter = TokenNameMap.find(m_left);
	return iter->second.second;
}

SLR1Token Production::getProducitonLeft()
{
	return this->m_left;
}

vector<SLR1Token> Production::getProductionRight()
{
	return this->m_right;
}


ProductionSet::ProductionSet() {
	//按书中76页构造
	Production prods[7];

	int i = 0;
	prods[0] = Production(Token_S, { Token_E,Token_eof },i++,0);
	prods[1] = Production(Token_E, { Token_E,Token_add,Token_T }, i++, 0);
	prods[2] = Production(Token_E, { Token_T }, i++, 0);
	prods[3] = Production(Token_T, { Token_T,Token_mul,Token_P }, i++, 0);
	prods[4] = Production(Token_T, { Token_P }, i++, 0);
	prods[5] = Production(Token_P, { Token_id }, i++, 0);
	prods[6] = Production(Token_P, { Token_left_paren,Token_E,Token_right_paren }, i++, 0);

	for (int i = 6; i >= 0; i--) {
		m_productions.push_back(prods[i]);
	}
	std::sort(m_productions.begin(), m_productions.end());

}


ProductionSet::ProductionSet(vector<Production> productions, bool is_init)
{
	m_productions = productions;
	m_is_init = is_init;
	if (is_init) {
		
	}
}
