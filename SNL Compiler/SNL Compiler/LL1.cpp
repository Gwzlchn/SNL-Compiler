#include "LL1.h"
#include <algorithm>
#include <iterator>

#define TERMIN true
#define NOTTER false


map<LL1Token, pair<string, bool>> TokenNameMap{
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

Production::Production(LL1Token left, vector<LL1Token> right, int id, int idx)
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

LL1Token Production::getProducitonLeft()
{
	return this->m_left;
}

vector<LL1Token> Production::getProductionRight()
{
	return this->m_right;
}

set<LL1Token> Production::getProdTer()
{
	set<LL1Token> ret;
	if (this->isLeftTerminal()) {
		ret.insert(this->m_left);
	}
	for (size_t i = 0; i < m_right.size(); i++) {
		auto iter = TokenNameMap.find(m_right[i]);
		if (iter->second.second) {
			ret.insert(this->m_right[i]);
		}
	}
	return ret;
}

set<LL1Token> Production::getProdNotTer()
{
	set<LL1Token> ret;
	if (!this->isLeftTerminal()) {
		ret.insert(this->m_left);
	}
	for (size_t i = 0; i < m_right.size(); i++) {
		auto iter = TokenNameMap.find(m_right[i]);
		if (!iter->second.second) {
			ret.insert(this->m_right[i]);
		}
	}
	return ret;

}


ProductionSet::ProductionSet() {
	//按书中76页构造
	Production prods[7];
	m_terminal = set<LL1Token>();
	m_notTerminal = set<LL1Token>();


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
		auto temp_ter = prods[i].getProdTer();
		auto temp_not = prods[i].getProdNotTer();
		//对终极符集合求并，对非终极符集合求并
		std::set_union(m_terminal.begin(), m_terminal.end(), temp_ter.begin(), temp_ter.end(), \
			std::inserter(m_terminal,m_terminal.end()));
		std::set_union(m_notTerminal.begin(), m_notTerminal.end(), temp_not.begin(), temp_not.end(), \
			std::inserter(m_notTerminal, m_notTerminal.end()));


	}
	std::sort(m_productions.begin(), m_productions.end());

}


