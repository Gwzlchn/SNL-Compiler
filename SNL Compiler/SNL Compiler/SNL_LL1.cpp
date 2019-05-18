#include "SNL_LL1.h"
#include "SNL_Lexer.h"
#include "SNL_Tokens.h"

#include <algorithm>
#include <iterator>
#include <fstream>
#include <sstream>
#include <iostream>


#define TERMIN true
#define NOTTER false
template<typename T, typename F>
const map<F, bool> make_terminal_map(map<T, F> full_map) {
	map<F, bool> ret;
	for (auto iter = full_map.begin(); \
		iter != full_map.end(); iter++) {

		if (std::all_of(iter->first.begin(), iter->first.end(), [](unsigned char c) { return !std::isalpha(c) || std::isupper(c); })) {
			ret.insert({ iter->second, TERMIN });
		}
		else {
			ret.insert({ iter->second, NOTTER });
		}
	}
	return ret;
}
const map<SNL_TOKEN_TYPE, bool> Token_Terminal_Map = make_terminal_map<string, SNL_TOKEN_TYPE>(Token_Name_Type_Map);



bool Production::operator<(const Production& prod) const
{
	return this->m_id < prod.m_id;
}

int Production::get_id() {
	return this->m_id;
}
Production::Production() {
	m_left = TOKEN_BLANK;
	m_right = { TOKEN_BLANK };
	m_id = -1;
	m_look_ahead_idx = -1;
}

Production::Production(SNL_TOKEN_TYPE left, vector<SNL_TOKEN_TYPE> right, int id, int idx)
{
	//����֤����ʽ�Ƿ���ȷ
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
	return;
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





ProductionSet::ProductionSet(string prods_file_name) {
	

	m_terminal = set<SNL_TOKEN_TYPE>();
	m_notTerminal = set<SNL_TOKEN_TYPE>();

	m_productions = makePordsFromFile(prods_file_name);
	for (auto prod_iter = m_productions.begin(); \
		prod_iter != m_productions.end();prod_iter++) {

		auto temp_ter = prod_iter->getProdTer();
		auto temp_not = prod_iter->getProdNotTer();
		//���ռ��������󲢣��Է��ռ���������
		setUnion(m_notTerminal, temp_not);
		setUnion(m_terminal, temp_ter);
	}


	this->getProdsFirstSet();
	this->getProdsFollowSet();
	this->setPredictSet();


}

vector<Production> ProductionSet::makePordsFromFile(const string& file_name) const
{
	vector<Production> ret = vector<Production>();
	//������ʽlist�ͷ��ռ���set
	/*****************************************************************************************/
	/*****************************************************************************************/
	std::ifstream infile;
	infile.open(file_name);
	if (!infile)
	{
		std::cerr << "�ļ���ʧ�ܣ�" << std::endl;
	}
	string line;//���ļ��ж�ȡһ�в���ʽ
	string substr;//���طָ����ַ���
	//production* ptrProduc;//ָ��һ������ʽ
	bool equalFlag = false;//��־�Ƿ��Ѷ���"::="�������
	int seqNum = 1;//��ǰ�кţ�Ҳ������ʽ�ı��


	while (getline(infile, line))
	{
		std::istringstream stream(line);//���ڷָ��ַ�
		//Production* prod = new Production();//һ�ж�Ӧһ������ʽ
		SNL_TOKEN_TYPE cur_prod_left = TOKEN_ERROR;
		vector<SNL_TOKEN_TYPE> cur_prod_right = vector<SNL_TOKEN_TYPE>();

		stream >> substr;//ÿ�еĵ�һ��Ԫ�رض����к�
		substr.pop_back();

		//cout << "Sequence:" << seqNum << endl;

		while (stream >> substr)
		{
			if (substr == "::=")
			{
				equalFlag = true;
				//cout << "::=" << endl;
				continue;//�ԵȺŲ����κδ���
			}

			if (!equalFlag)
			{
				 auto left = Token_Name_Type_Map.find(substr);
				 if (left == Token_Name_Type_Map.end()) {
					 std::cerr << substr << std::endl;
					 //break;
					 system("pause");
				 }
				 cur_prod_left = left->second;
			}
			else
			{
				auto right = Token_Name_Type_Map.find(substr);
				if (right == Token_Name_Type_Map.end()) {
					std::cerr << substr << std::endl;
					//break;
					system("pause");

				}
				cur_prod_right.push_back(right->second);
			}
		}


		ret.push_back(Production(cur_prod_left, cur_prod_right, seqNum++));
		equalFlag = false;//һ�����ݽ���������־��λ
		line.clear();
		substr.clear();
		//cout <<"This line is over!\n\n\n\n"<< endl;
	}
	infile.close();
	return ret;
}




void ProductionSet::getProdsFirstSet()
{
	//first���ϳ�ʼ��
	for (auto ter_iter = m_terminal.begin(); \
		ter_iter != m_terminal.end(); ter_iter++) {
		set<SNL_TOKEN_TYPE> temp = { *ter_iter };
		m_first_sets[*ter_iter] = temp;
	}
	for (auto not_iter = m_notTerminal.begin(); \
		not_iter != m_notTerminal.end(); not_iter++) {

		m_first_sets[*not_iter] = set<SNL_TOKEN_TYPE>();
	}
	//��һ������ռ�����first����

	//�������󣬱�־λ
	bool  expanded = true;
	while (expanded) {
		expanded = false;
		
		for (auto notTer_iter = m_notTerminal.begin();
			notTer_iter != m_notTerminal.end();
			notTer_iter++) {

			//��ǰ���ռ���First����
			set<SNL_TOKEN_TYPE>& cur_not_ter_first_set = m_first_sets.find(*notTer_iter)->second;
			//�����в���ʽ����
			for (auto prod_iter = m_productions.begin(); \
				prod_iter !=  m_productions.end();prod_iter++) {
				if (prod_iter->getProducitonLeft() == *notTer_iter) {

					size_t before = m_first_sets[*notTer_iter].size();
					vector<SNL_TOKEN_TYPE> prod_right = prod_iter->getProductionRight();
					size_t prod_r_len = prod_right.size();

					
					for (size_t i = 0; i < prod_r_len; i++) {
						//�����һ�����ռ���,ֱ���˳�
						if (getTokenType(prod_right[i]) == 2) {
							cur_not_ter_first_set.insert(prod_right[i]);
							break;
						}
						//�����һ���ǿհ׷�,��������һ���ַ�
						else if (getTokenType(prod_right[i]) == 1) {
							cur_not_ter_first_set.insert(prod_right[i]);
							continue;
						}
						//�����һ���Ƿ��ռ���,����ȡ����
						else if (getTokenType(prod_right[i]) == 3) {
							set<SNL_TOKEN_TYPE> r_first_set = m_first_sets.find(prod_right[i])->second;
							setUnion(cur_not_ter_first_set, setRemoveBlank(r_first_set));
							if (!isNotTerDeriBlank(prod_right[i])) {
								break;
							}
							else if (i == prod_r_len - 1 && isNotTerDeriBlank(prod_right[i])) {
								cur_not_ter_first_set.insert(TOKEN_BLANK);
							}
							
						}
					}
					//ֻҪ��һ�����ռ���First������,�����ѭ��
					if (m_first_sets[*notTer_iter].size() != before) {
						expanded = true;
					}
				}
			}
		}
	}
	printSetMap(m_first_sets);
	return;

}





inline void ProductionSet::setUnion(set<SNL_TOKEN_TYPE>& dst, const set<SNL_TOKEN_TYPE>& src)
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
		ret += Token_Type_Name_Map.find(*i)->second;
		ret += " ";
	}
	return ret;
}

void ProductionSet::printSetMap(const map<SNL_TOKEN_TYPE, set<SNL_TOKEN_TYPE>>& sets) const
{
	std::cout << "----------------------" << std::endl;
	for (auto iter = sets.begin(); iter != sets.end(); iter++) {
		if (m_terminal.find(iter->first) == m_terminal.end()) {
			std::cout << Token_Type_Name_Map.find(iter->first)->second << "\t\t\t";
			std::cout << get_token_str<set<SNL_TOKEN_TYPE>>(iter->second) << std::endl;
		}
	}
}


void ProductionSet::printPredictMap() const {
	std::cout << "----------------------" << std::endl;
	for (auto iter = m_predict_set.begin(); iter != m_predict_set.end(); iter++) {
		
			std::cout << iter->first << "\t";
			std::cout << get_token_str<set<SNL_TOKEN_TYPE>>(iter->second) << std::endl;

	}
}


set<SNL_TOKEN_TYPE> ProductionSet::setRemoveBlank(const set<SNL_TOKEN_TYPE>& src) const
{
	auto blk_iter = src.find(TOKEN_BLANK);
	if (blk_iter == src.end()) {
		return src;
	}
	else {
		set<SNL_TOKEN_TYPE> ret = src;
		ret.erase(TOKEN_BLANK);
		return ret;
	}
}


//���ռ����Ƿ����Ƴ���
bool ProductionSet::isNotTerDeriBlank(SNL_TOKEN_TYPE not_ter)
{
	auto iter = m_first_sets.find(not_ter);
	if (iter == m_first_sets.end()) {
		return false;
	}

	//���������ҵ�,˵�����Ƴ���
	return iter->second.find(TOKEN_BLANK) != iter->second.end();
}


int ProductionSet::getTokenType(SNL_TOKEN_TYPE tok)
{
	if (tok == TOKEN_BLANK) {
		return 1;
	}
	auto ter_iter = m_terminal.find(tok);
	auto not_iter = m_notTerminal.find(tok);
	if (ter_iter != m_terminal.end()) {
		return 2; //���ռ���
	}
	if (not_iter != m_notTerminal.end()) {
		return 3;	//�Ƿ��ռ���
	}

	return 0;
}

void ProductionSet::getProdsFollowSet()
{
	for (auto ter_iter = m_terminal.begin(); \
		ter_iter != m_terminal.end(); ter_iter++) {
		m_follow_sets[*ter_iter] = set<SNL_TOKEN_TYPE>();
	}
	for (auto not_iter = m_notTerminal.begin(); \
		not_iter != m_notTerminal.end(); not_iter++) {
		m_follow_sets[*not_iter] = set<SNL_TOKEN_TYPE>();
	}

	//�ҵ���ʼ��
	SNL_TOKEN_TYPE begin = m_productions[0].getProducitonLeft();
	m_follow_sets[begin].insert(TOKEN_ENDFILE);

	bool expanded = true;
	while (expanded) {
		expanded = false;
		for (auto not_iter = m_notTerminal.begin(); \
			not_iter != m_notTerminal.end(); not_iter++) {

			set<SNL_TOKEN_TYPE>& cur_not_follow = m_follow_sets[*not_iter];
			size_t before = cur_not_follow.size();
			for (auto prod_iter = m_productions.begin(); \
				prod_iter != m_productions.end(); prod_iter++) {

				vector<SNL_TOKEN_TYPE> after;
				if (getAfterTokenInRightProd(*not_iter, *prod_iter, after)) {
					set<SNL_TOKEN_TYPE> after_fisrt_set = this->getTokenVecFirst(after);
					
					set<SNL_TOKEN_TYPE> remove_blank = setRemoveBlank(after_fisrt_set);
					setUnion(cur_not_follow, remove_blank);

					//�Ҳ��ַ����Ƴ���,��������ַ���follow
					if (after_fisrt_set.find(TOKEN_BLANK)!=after_fisrt_set.end() ) {
						//std::cout << prod_iter->getProducitonLeft() << std::endl;
						setUnion(cur_not_follow, (m_follow_sets.find(prod_iter->getProducitonLeft()))->second);
					}
				}			
				if (before != cur_not_follow.size()) {
					expanded = true;
				}

			}
		}
	}
	printSetMap(m_follow_sets);
	return;
}

//��ǰ�ַ�����һ���ַ���ʲô
bool ProductionSet::getAfterTokenInRightProd(const SNL_TOKEN_TYPE& to_find, \
											const Production& prod, \
											vector<SNL_TOKEN_TYPE>& after_token)
{
	vector<SNL_TOKEN_TYPE> right = prod.getProductionRight();
	bool found = false;

	if (*(right.end() - 1) == to_find) {
		found = true;
		after_token = { TOKEN_BLANK };
		return found;
	}
	for (auto iter = right.begin(); iter != right.end() - 1; iter++) {
		if (*iter == to_find || found) {
			found = true;
			after_token.push_back(*(iter + 1));
			}
		}
	return found;
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

set<SNL_TOKEN_TYPE> ProductionSet::getTokenVecFirst(const vector<SNL_TOKEN_TYPE>& tok_vec)  {
	set<SNL_TOKEN_TYPE> ret_set = set<SNL_TOKEN_TYPE>();
	for (auto iter = tok_vec.begin(); iter != tok_vec.end(); iter++) {
		const set<SNL_TOKEN_TYPE>& cur_first = m_first_sets.find(*iter)->second;
		setUnion(ret_set, setRemoveBlank(cur_first));
		
		if (isNotTerDeriBlank(*iter)) {
			if (iter == tok_vec.end() - 1) {
				ret_set.insert(TOKEN_BLANK);
			}
			continue;
		}
		else {
			break;
		}
	}
	return ret_set;
}

set<SNL_TOKEN_TYPE> ProductionSet::getOneProdPredict(const Production& prod)  {
	set<SNL_TOKEN_TYPE> right_first = getTokenVecFirst(prod.getProductionRight());
	if (right_first.find(TOKEN_BLANK) == right_first.end()) {
		return right_first;
	}
	else {
		right_first.erase(TOKEN_BLANK);
		setUnion(right_first, m_follow_sets[prod.getProducitonLeft()]);
		return right_first;
	}
}

void ProductionSet::setAnalyseMap()
{
	m_SNL_analyse_map = map<SNL_TOKEN_TYPE, vector<pair<SNL_TOKEN_TYPE, Prod_Idx>>>();
	size_t ter_size = m_terminal.size();
	for (auto not_ter_iter = m_notTerminal.begin(); \
		not_ter_iter != m_notTerminal.end(); not_ter_iter++) {
	}
	for (auto prod_iter = m_productions.begin(); \
		prod_iter != m_productions.end(); prod_iter++) {
		//�ҵ���ǰPredict��
		set<SNL_TOKEN_TYPE>& cur_predict = m_predict_set.find(prod_iter->get_id())->second;
		for (auto ter_iter = cur_predict.begin(); \
			ter_iter != cur_predict.end(); ter_iter++) {
			m_SNL_analyse_map[prod_iter->getProducitonLeft()].push_back({ *ter_iter,prod_iter->get_id() });
			
		}
	}
}


bool ProductionSet::SNL_AnalyseProcess(const vector<SNL_TOKEN_TYPE>& token_input_vec) {
	
	//std::reverse(token_input_vec.begin()token_input_vec.end());
	for (auto iter = token_input_vec.begin(); \
		iter != token_input_vec.end(); iter++) {
		m_input_stream.push(*iter);
	}

	//��ʼ������ջ,���������� frontΪջ�ף�endջ��

	SNL_TOKEN_TYPE tok_start = m_productions[0].getProducitonLeft();
	stack<SNL_TOKEN_TYPE> LL1_analyse_stack;
	LL1_analyse_stack.push(TOKEN_ENDFILE);
	LL1_analyse_stack.push(tok_start);

	while (m_input_stream.size() != 0) {
		SNL_TOKEN_TYPE cur_input_tok = m_input_stream.top();
		SNL_TOKEN_TYPE cur_analyse_tok = LL1_analyse_stack.top();
		//�������ͷ���ջûmatch��������������
		if (cur_analyse_tok != cur_input_tok) {
			int prod_id = getProdIdFromAnalyseMap(cur_analyse_tok, cur_input_tok);
			if (prod_id <= 0) {
				system("pause");
			}
			LL1_analyse_stack.pop();
			this->pushProdToAnaylseStack(prod_id, LL1_analyse_stack);
			continue;
		}
		else { //Match
			LL1_analyse_stack.pop();
			m_input_stream.pop();
		}

		//int i = 1;
	}

	return true;

}


void ProductionSet::printStack(const stack<SNL_TOKEN_TYPE>& tok_stack)const{
	
}

int ProductionSet::getProdIdFromAnalyseMap(SNL_TOKEN_TYPE ana_tok, SNL_TOKEN_TYPE in_tok) {
	auto iter = m_SNL_analyse_map.find(ana_tok);

	for (vector<pair<SNL_TOKEN_TYPE, Prod_Idx>>::iterator vec_iter = iter->second.begin();\
		vec_iter != (iter->second).end(); vec_iter++) {

		if (vec_iter->first == in_tok) {
			return vec_iter->second;
		}
	}
	return -1;
}

void ProductionSet::pushProdToAnaylseStack(int prod_id, stack<SNL_TOKEN_TYPE>& ana_stack) {
	const vector<SNL_TOKEN_TYPE>& prod_right = m_productions[prod_id-1].getProductionRight();
	for (auto res_iter = prod_right.rbegin(); res_iter != prod_right.rend(); res_iter++) {
		if (*res_iter != TOKEN_BLANK) {
			ana_stack.push(*res_iter);
		}	
	}
	return;
}
