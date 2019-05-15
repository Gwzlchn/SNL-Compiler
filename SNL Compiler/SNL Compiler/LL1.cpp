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
	{Token_E_Hat,{"E^",NOTTER}},
	{Token_F,{"F",NOTTER}},
	{Token_T_Hat,{"T^",NOTTER}},

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

LL1Token Production::getProducitonLeft() const
{
	return this->m_left;
}

vector<LL1Token> Production::getProductionRight() const
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
	

	m_terminal = set<LL1Token>();
	m_notTerminal = set<LL1Token>();

	m_productions = makeProdsFromPage53();

	for (auto prod_iter = m_productions.begin(); \
		prod_iter != m_productions.end();prod_iter++) {

		auto temp_ter = prod_iter->getProdTer();
		auto temp_not = prod_iter->getProdNotTer();
		//对终极符集合求并，对非终极符集合求并
		setUnion(m_notTerminal, temp_not);
		setUnion(m_terminal, temp_ter);
	}

	//std::sort(m_productions.begin(), m_productions.end());


	this->getProdsFirstSet();
	this->getProdsFollowSet();
	this->setPredictSet();

}

//按书中76页构造

vector<Production> ProductionSet::makeProdsFromPage77()
{
	vector<Production> ret = vector<Production>();
	int i = 0;
	ret.push_back(Production(Token_S, { Token_E,Token_eof }, i++, 0));
	ret.push_back(Production(Token_E, { Token_E,Token_add,Token_T }, i++, 0));
	ret.push_back(Production(Token_E, { Token_T }, i++, 0));
	ret.push_back(Production(Token_T, { Token_T,Token_mul,Token_P }, i++, 0));
	ret.push_back(Production(Token_T, { Token_P }, i++, 0));
	ret.push_back(Production(Token_P, { Token_id }, i++, 0));
	ret.push_back(Production(Token_P, { Token_left_paren,Token_E,Token_right_paren }, i++, 0));
	return ret;

}

vector<Production> ProductionSet::makeProdsFromPage53()
{
	auto ret = vector<Production>();
	int i = 0;
	ret.push_back(Production(Token_E, { Token_T,Token_E_Hat }, i++, 0));
	ret.push_back(Production(Token_E_Hat, { Token_add,Token_T,Token_E_Hat }, i++, 0));
	ret.push_back(Production(Token_E_Hat, { Token_Blank }, i++, 0));
	ret.push_back(Production(Token_T, { Token_F,Token_T_Hat }, i++, 0));
	ret.push_back(Production(Token_T_Hat, { Token_mul,Token_F,Token_T_Hat }, i++, 0));
	ret.push_back(Production(Token_T_Hat, { Token_Blank }, i++, 0));
	ret.push_back(Production(Token_F, { Token_id }, i++, 0));
	ret.push_back(Production(Token_F, { Token_left_paren,Token_E,Token_right_paren }, i++, 0));

	return ret;


}




void ProductionSet::getProdsFirstSet()
{
	//first集合初始化
	for (auto ter_iter = m_terminal.begin(); \
		ter_iter != m_terminal.end(); ter_iter++) {
		set<LL1Token> temp = { *ter_iter };
		m_first_sets[*ter_iter] = temp;
	}
	for (auto not_iter = m_notTerminal.begin(); \
		not_iter != m_notTerminal.end(); not_iter++) {

		m_first_sets[*not_iter] = set<LL1Token>();
	}
	//进一步求非终极符的first集合

	//仍在扩大，标志位
	bool  expanded = true;
	while (expanded) {
		expanded = false;
		
		for (auto notTer_iter = m_notTerminal.begin();
			notTer_iter != m_notTerminal.end();
			notTer_iter++) {

			//当前非终极符First集合
			set<LL1Token>& cur_not_ter_first_set = m_first_sets.find(*notTer_iter)->second;
			//对所有产生式遍历
			for (auto prod_iter = m_productions.begin(); \
				prod_iter !=  m_productions.end();prod_iter++) {
				if (prod_iter->getProducitonLeft() == *notTer_iter) {

					size_t before = m_first_sets[*notTer_iter].size();
					vector<LL1Token> prod_right = prod_iter->getProductionRight();
					size_t prod_r_len = prod_right.size();

					
					for (size_t i = 0; i < prod_r_len; i++) {
						//如果第一个是终极符,直接退出
						if (getTokenType(prod_right[i]) == 2) {
							cur_not_ter_first_set.insert(prod_right[i]);
							break;
						}
						//如果第一个是空白符,继续查下一个字符
						else if (getTokenType(prod_right[i]) == 1) {
							cur_not_ter_first_set.insert(prod_right[i]);
							continue;
						}
						//如果第一个是非终极符,二者取交集
						else if (getTokenType(prod_right[i]) == 3) {
							set<LL1Token> r_first_set = m_first_sets.find(prod_right[i])->second;
							setUnion(cur_not_ter_first_set, setRemoveBlank(r_first_set));

							if (!isNotTerDeriBlank(prod_right[i])) {
								break;
							}
							
						}
					}
					//printSetMap(m_first_sets);
					//只要有一个非终极符First集扩大,则继续循环
					if (m_first_sets[*notTer_iter].size() != before) {
						expanded = true;
					}
				}
			}
		}
	}

}





inline void ProductionSet::setUnion(set<LL1Token>& dst, const set<LL1Token>& src)
{
	std::set_union(dst.begin(), dst.end(), src.begin(), src.end(), \
		std::inserter(dst, dst.end()));
	return;
}

template <typename T>
string ProductionSet::get_token_str(const T& tok_vec) const
{
	string ret = "";
	for (auto i = tok_vec.begin(); i != tok_vec.end(); i++) {
		ret += TokenNameMap.find(*i)->second.first;
		ret += " ";
	}
	return ret;
}

void ProductionSet::printSetMap(const map<LL1Token, set<LL1Token>>& sets) const
{
	std::cout << "----------------------" << std::endl;
	for (auto iter = sets.begin(); iter != sets.end(); iter++) {
		if (m_terminal.find(iter->first) == m_terminal.end()) {
			std::cout << TokenNameMap.find(iter->first)->second.first << "\t";
			std::cout << get_token_str<set<LL1Token>>(iter->second) << std::endl;
		}
	}
}


void ProductionSet::printPredictMap() const {
	std::cout << "----------------------" << std::endl;
	for (auto iter = m_predict_set.begin(); iter != m_predict_set.end(); iter++) {
		
			std::cout << iter->first << "\t";
			std::cout << get_token_str<set<LL1Token>>(iter->second) << std::endl;

	}
}


set<LL1Token> ProductionSet::setRemoveBlank(const set<LL1Token>& src) const
{
	auto blk_iter = src.find(Token_Blank);
	if (blk_iter == src.end()) {
		return src;
	}
	else {
		set<LL1Token> ret = src;
		ret.erase(Token_Blank);
		return ret;
	}
}

//存在,返回true
bool ProductionSet::isBlankInTokenFirst(const LL1Token& tok) const {
	set<LL1Token> tok_first = m_first_sets.find(tok)->second;
	if (tok_first.find(Token_Blank) != tok_first.end()) {
		
		return true;
	}
	else {
		return false;
	}

}

//非终极符是否能推出空
bool ProductionSet::isNotTerDeriBlank(LL1Token not_ter)
{
	auto iter = m_first_sets.find(not_ter);
	if (iter == m_first_sets.end()) {
		return false;
	}

	//集合里能找到,说明能推出空
	return iter->second.find(Token_Blank) != iter->second.end();
}



int ProductionSet::getTokenType(LL1Token tok)
{
	if (tok == Token_Blank) {
		return 1;
	}
	auto ter_iter = m_terminal.find(tok);
	auto not_iter = m_notTerminal.find(tok);
	if (ter_iter != m_terminal.end()) {
		return 2; //是终极符
	}
	if (not_iter != m_notTerminal.end()) {
		return 3;	//是非终极符
	}

	return 0;
}

void ProductionSet::getProdsFollowSet()
{
	for (auto ter_iter = m_terminal.begin(); \
		ter_iter != m_terminal.end(); ter_iter++) {
		m_follow_sets[*ter_iter] = set<LL1Token>();
	}
	for (auto not_iter = m_notTerminal.begin(); \
		not_iter != m_notTerminal.end(); not_iter++) {
		m_follow_sets[*not_iter] = set<LL1Token>();
	}

	//找到开始符
	LL1Token begin = m_productions[0].getProducitonLeft();
	m_follow_sets[begin].insert(Token_eof);

	bool expanded = true;
	while (expanded) {
		expanded = false;
		for (auto not_iter = m_notTerminal.begin(); \
			not_iter != m_notTerminal.end(); not_iter++) {

			set<LL1Token>& cur_not_follow = m_follow_sets[*not_iter];
			size_t before = cur_not_follow.size();
			for (auto prod_iter = m_productions.begin(); \
				prod_iter != m_productions.end(); prod_iter++) {
				LL1Token after;
				if (getAfterTokenInRightProd(*not_iter, *prod_iter, after)) {
					set<LL1Token> after_fisrt_set = m_first_sets.find(after)->second;
					set<LL1Token> remove_blank = setRemoveBlank(after_fisrt_set);
					setUnion(cur_not_follow, remove_blank);

					//右侧字符能推出空,并入左侧字符的follow
					if (getTokenType(after) == 1 || isBlankInTokenFirst(after) ) {
						std::cout << prod_iter->getProducitonLeft() << std::endl;
						setUnion(cur_not_follow, (m_follow_sets.find(prod_iter->getProducitonLeft()))->second);
					}
				}
				printSetMap(m_follow_sets);
				if (before != cur_not_follow.size()) {

					expanded = true;
				}

			}
		}
	}
	return;



}

bool ProductionSet::getAfterTokenInRightProd(const LL1Token& to_find, const Production& prod, LL1Token& after_token)
{
	vector<LL1Token> right = prod.getProductionRight();
	for (auto iter = right.begin(); iter != right.end(); iter++) {
		if (*iter == to_find) {
			if (iter == right.end() - 1) {
				after_token = Token_Blank;
				return true;
			}
			else {
				after_token = *(iter + 1);
				return true;
			}
		}
		
	}

	return false;
}

void ProductionSet::setPredictSet()
{
	for (auto prod_iter = m_productions.begin(); \
		prod_iter != m_productions.end(); prod_iter++) {
		m_predict_set[prod_iter->get_id()] = getOneProdPredict(*prod_iter);
	}
	printPredictMap();
	return;
}

set<LL1Token> ProductionSet::getTokenVecFirst(const vector<LL1Token>& tok_vec)  {
	set<LL1Token> ret_set = set<LL1Token>();
	for (auto iter = tok_vec.begin(); iter != tok_vec.end(); iter++) {
		const set<LL1Token>& cur_first = m_first_sets.find(*iter)->second;
		setUnion(ret_set, setRemoveBlank(cur_first));
		
		if (isBlankInTokenFirst(*iter)) {
			if (iter == tok_vec.end() - 1) {
				ret_set.insert(Token_Blank);
			}
			continue;
		}
		else {
			break;
		}
	}
	return ret_set;
}

set<LL1Token> ProductionSet::getOneProdPredict(const Production& prod)  {
	set<LL1Token> right_first = getTokenVecFirst(prod.getProductionRight());
	if (right_first.find(Token_Blank) == right_first.end()) {
		return right_first;
	}
	else {
		right_first.erase(Token_Blank);
		setUnion(right_first, m_follow_sets[prod.getProducitonLeft()]);
		return right_first;
	}
}


