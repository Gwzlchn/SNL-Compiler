#include"SNL_Production.h"
#include"SNL_LL1.h"
#include<iostream>

bool Production::operator<(const Production& prod) const
{
	return this->m_id < prod.m_id;
}

size_t Production::get_id() {
	return this->m_id;
}



Production::Production(SNL_TOKEN_TYPE left, vector<SNL_TOKEN_TYPE> right, size_t id, size_t idx)
{
	//先验证产生式是否正确
	if (Token_Terminal_Map.find(left) == Token_Terminal_Map.end()) {
		std::cerr << "this Token  cannot identified" << std::endl;
		return;
	};
	size_t right_size = right.size();
	for (size_t i = 0; i < right_size; i++) {
		if (Token_Terminal_Map.find(right[i]) == Token_Terminal_Map.end()) {
			std::cerr << "this Token  cannot identified" << std::endl;
			return;
		}
	}


	m_left = left;
	m_right = right;
	m_id = id;
	m_look_ahead_idx = idx;

}

inline bool Production::isLeftTerminal() {
	auto iter = Token_Terminal_Map.find(m_left);
	return iter->second;
}

SNL_TOKEN_TYPE Production::getProducitonLeft() const
{
	return this->m_left;
}

vector<SNL_TOKEN_TYPE> Production::getProductionRight() const
{
	return this->m_right;
}

set<SNL_TOKEN_TYPE> Production::getProdTer()
{
	set<SNL_TOKEN_TYPE> ret;
	if (this->isLeftTerminal()) {
		ret.insert(this->m_left);
	}
	for (size_t i = 0; i < m_right.size(); i++) {
		auto iter = Token_Terminal_Map.find(m_right[i]);
		if (iter->second) {
			ret.insert(this->m_right[i]);
		}
	}
	return ret;
}

set<SNL_TOKEN_TYPE> Production::getProdNotTer()
{
	set<SNL_TOKEN_TYPE> ret;
	if (!this->isLeftTerminal()) {
		ret.insert(this->m_left);
	}
	for (size_t i = 0; i < m_right.size(); i++) {
		auto iter = Token_Terminal_Map.find(m_right[i]);
		if (!iter->second) {
			ret.insert(this->m_right[i]);
		}
	}
	return ret;

}
