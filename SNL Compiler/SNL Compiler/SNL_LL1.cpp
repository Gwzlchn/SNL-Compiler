#include "SNL_LL1.h"
#include "SNL_Lexer.h"
#include "SNL_Tokens.h"

#include "Utils.h" 
#include <queue>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cctype>


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








ProductionSet::ProductionSet(string prods_file_name) {
	

	m_terminal = set<SNL_TOKEN_TYPE>();
	m_notTerminal = set<SNL_TOKEN_TYPE>();

	m_productions = makePordsFromFile(prods_file_name);
	this->setTokenTerminalOrNot();
	this->setTokenFirstSet();
	this->setTokenFollowSet();
	this->setPredictSet();
	this->setAnalyseMap();

	//std::cout << getSetMapToStr(m_first_sets).str();
	//std::cout << getSetMapToStr(m_follow_sets).str();
	//std::cout << getAllSetMapToStr().str();
	//std::cout << getPredictMapToStr().str();
	//std::cout << getLL1AnalyseMapToStr().str();

	int a = 1;
	//this->PrintLL1AnalyseMap();
}

vector<Production> ProductionSet::makePordsFromFile(const string& file_name) const
{
	vector<Production> ret = vector<Production>();
	std::ifstream infile;
	infile.open(file_name);
	if (!infile)
	{
		std::cerr << "�ļ���ʧ�ܣ�" << std::endl;
	}
	string line;//���ļ��ж�ȡһ�в���ʽ
	string substr;//���طָ����ַ���
	bool equalFlag = false;//��־�Ƿ��Ѷ���"::="�������
	int seqNum = 1;//��ǰ�кţ�Ҳ������ʽ�ı��


	while (getline(infile, line))
	{
		std::istringstream stream(line);//���ڷָ��ַ�
		SNL_TOKEN_TYPE cur_prod_left = TOKEN_ERROR;
		vector<SNL_TOKEN_TYPE> cur_prod_right = vector<SNL_TOKEN_TYPE>();

		//stream >> substr;//ÿ�еĵ�һ��Ԫ�رض����к�
		//substr.pop_back();
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




void ProductionSet::setTokenTerminalOrNot()
{
	for (auto prod_iter = m_productions.begin(); \
		prod_iter != m_productions.end(); prod_iter++) {

		auto temp_ter = prod_iter->getProdTer();
		auto temp_not = prod_iter->getProdNotTer();
		//���ռ��������󲢣��Է��ռ���������
		setUnion(m_notTerminal, temp_not);
		setUnion(m_terminal, temp_ter);
	}

}

void ProductionSet::setTokenFirstSet()
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
	return;
}





inline void ProductionSet::setUnion(set<SNL_TOKEN_TYPE>& dst, const set<SNL_TOKEN_TYPE>& src)
{
	std::set_union(dst.begin(), dst.end(), src.begin(), src.end(), \
		std::inserter(dst, dst.end()));
	return;
}

template <typename T>
static string ProductionSet::get_token_str(const T& tok_vec) 
{
	string ret = "";
	for (auto i = tok_vec.begin(); i != tok_vec.end(); i++) {
		ret += Token_Type_Name_Map.find(*i)->second;
		ret += " ";
	}
	return ret;
}

ostream& operator <<(ostream& os, const map<SNL_TOKEN_TYPE, set<SNL_TOKEN_TYPE>>& sets)
{
	os << "----------------------\n";
	for (auto iter = sets.begin(); iter != sets.end(); iter++) {
		if (!(Token_Terminal_Map.find(iter->first) ->second)) {
			os << Token_Type_Name_Map.find(iter->first)->second << "\t\t\t";
			os << ProductionSet::get_token_str<set<SNL_TOKEN_TYPE>>(iter->second) << std::endl;
		}
	}
	return os;
}



stringstream ProductionSet::getAllSetMapToStr() const
{
	stringstream ss;
	ss << "----------------------\n";
	auto first_iter = m_first_sets.begin();
	auto follow_iter = m_follow_sets.begin();
	int i = 1;
	for (; first_iter != m_first_sets.end();) {
		if (m_terminal.find(first_iter->first) == m_terminal.end()) {
			ss << i <<".1\t";
			ss << Token_Type_Name_Map.find(first_iter->first)->second << "\t\t\t";
			ss << get_token_str<set<SNL_TOKEN_TYPE>>(first_iter->second) << std::endl;
			ss << i << ".2\t";
			ss << Token_Type_Name_Map.find(follow_iter->first)->second << "\t\t\t";
			ss  << get_token_str<set<SNL_TOKEN_TYPE>>(follow_iter->second) << std::endl;
			i++;
		} 
		first_iter++;
		follow_iter++;
	}
	return ss;
}


ostream& operator <<(ostream& os, const map<int, set<SNL_TOKEN_TYPE>>& predict_set)
{

	os << "----------------------\n";
	for (auto iter = predict_set.begin(); iter != predict_set.end(); iter++) {

		os << iter->first << "\t";
		os << ProductionSet::get_token_str<set<SNL_TOKEN_TYPE>>(iter->second) << std::endl;

	}
	return os;
}


ostream& operator <<(ostream& os, const vector<vector<int>>& LL1_Analyse_Map) {
	const size_t size = LL1_Analyse_Map.size();
	for (size_t i = 0; i < size; i++) {
		for (size_t j = 0; j < size; j++) {
			if (LL1_Analyse_Map[i][j] == 0) {
				continue;
			}
			else {
				os << Token_Type_Name_Map.find((SNL_TOKEN_TYPE)i)->second << "\t\t";
				os << Token_Type_Name_Map.find((SNL_TOKEN_TYPE)j)->second << "\t\t";
				os << LL1_Analyse_Map[i][j] << std::endl;
			}
		}
	}
	return os;
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

void ProductionSet::setTokenFollowSet()
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

				const auto& cur_right = prod_iter->getProductionRight();
				size_t not_ter_in_right = std::count(cur_right.begin(), cur_right.end(), *not_iter);
				size_t now_prod_right_index = 0;
				
				if (not_ter_in_right == 1 ) {

					vector<SNL_TOKEN_TYPE> after = vector<SNL_TOKEN_TYPE>();
					getAfterTokenInRightProd(*not_iter, *prod_iter, after);
					set<SNL_TOKEN_TYPE> after_fisrt_set = this->getTokenVecFirst(after);
				

					set<SNL_TOKEN_TYPE> remove_blank = setRemoveBlank(after_fisrt_set);
					setUnion(cur_not_follow, remove_blank);

					//�Ҳ��ַ����Ƴ���,��������ַ���follow
					if (after_fisrt_set.find(TOKEN_BLANK)!=after_fisrt_set.end() ) {
						//std::cout << prod_iter->getProducitonLeft() << std::endl;
						setUnion(cur_not_follow, (m_follow_sets.find(prod_iter->getProducitonLeft()))->second);
					}
				}
				else if (not_ter_in_right >= 1) {
					for (int i = 0; i < cur_right.size(); i++) {
						vector<SNL_TOKEN_TYPE> after = vector<SNL_TOKEN_TYPE>();
						getAfterTokenInRightProd(*not_iter, *prod_iter, after, now_prod_right_index);

						set<SNL_TOKEN_TYPE> after_fisrt_set = this->getTokenVecFirst(after);
						set<SNL_TOKEN_TYPE> remove_blank = setRemoveBlank(after_fisrt_set);
						setUnion(cur_not_follow, remove_blank);

						//�Ҳ��ַ����Ƴ���,��������ַ���follow
						if (after_fisrt_set.find(TOKEN_BLANK) != after_fisrt_set.end()) {
							//std::cout << prod_iter->getProducitonLeft() << std::endl;
							setUnion(cur_not_follow, (m_follow_sets.find(prod_iter->getProducitonLeft()))->second);
						}

						now_prod_right_index++;
					}

				}
				if (before != cur_not_follow.size()) {
					expanded = true;
				}

			}
		}
	}
	return;
}

//��ǰ�ַ�����һ���ַ���ʲô
bool ProductionSet::getAfterTokenInRightProd(const SNL_TOKEN_TYPE& to_find, \
											const Production& prod, \
											vector<SNL_TOKEN_TYPE>& after_token,\
											int prod_inx)
{
	vector<SNL_TOKEN_TYPE> right = prod.getProductionRight();
	bool found = false;

	if (*(right.end() - 1) == to_find) {
		found = true;
		after_token = { TOKEN_BLANK };
		return found;
	}
	for (auto iter = right.begin()+ prod_inx; iter != right.end() - 1; iter++) {
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

void ProductionSet::setAnalyseMap(){

	//��ʼ��LL1 ������
	const size_t size = Token_Terminal_Map.size();
	m_LL1_Analyse_Map = vector<vector<int>>(size, vector<int>(size));
	for (size_t i = 0; i < size; i++) {
		//m_LL1_Analyse_Map[i] = new int[size];
		for (size_t j = 0; j < size; j++) {
			m_LL1_Analyse_Map[i][j] = 0;
		}
	}
	size_t ter_size = m_terminal.size();

	for (auto prod_iter = m_productions.begin(); \
		prod_iter != m_productions.end(); prod_iter++) {

		SNL_TOKEN_TYPE left = prod_iter->getProducitonLeft();
		int prod_id = prod_iter->get_id();
		//�ҵ���ǰPredict��
		set<SNL_TOKEN_TYPE>& cur_predict = m_predict_set.find(prod_iter->get_id())->second;
		for (auto ter_iter = cur_predict.begin(); \
			ter_iter != cur_predict.end(); ter_iter++) {
			SNL_TOKEN_TYPE cur_sigle_right = *ter_iter;
			m_LL1_Analyse_Map[left][cur_sigle_right] = prod_id;
		}
	}
}




void ProductionSet::dfsBuildTree(Node*& parent)
{
	// �������Ĳ���parentһ���Ƿ��ռ���

	if (Token_Terminal_Map.find(parent->curr)->second == true) {
		std::cerr << "[error] dfsBuildTree invalid param" << std::endl;
		exit(-1);
	}
	if (treeIndex >= vTree.size()) {
		std::cerr << "[error] dfsBuildTree treeIndex" << std::endl;
		exit(-1);
	}
	// �����ӽڵ�
	const auto& cur_right = vTree[treeIndex].getProductionRight();
	for (auto iter = cur_right.begin(); iter != cur_right.end(); iter++) {
		parent->children.emplace_back(new Node(*iter, parent, global_id++));
	}

	// �������Ƶ�ʽ�±�ͼӼ�
	treeIndex++;
	for (unsigned int i = 0; i < parent->children.size(); i++) {
		if (Token_Terminal_Map.find(parent->children[i]->curr)->second) {
			// ��IDת��Ϊʵ��ֵ
			if (parent->children[i]->curr == TOKEN_BLANK) {}
			else {
				//cout << tokenSymbols[global_token_index] <<"\t\ttokenlist"<< endl;
				//cout << parent->children[i]->curr << endl;
				//cout << endl;
				parent->children[i]->curr_str = Token_Type_Name_Map.find(m_input_stream[global_token_index])->second;
				global_token_index++;
			}
			continue;// �ռ�����Ҷ�ڵ�
		}
		dfsBuildTree(parent->children[i]);
	}
}


int ProductionSet::grammarAnalysis()
{
		// �� ����ջ #E   ������ i+i*i#
		std::list<SNL_TOKEN_TYPE> s;//��Ϊջ���ܱ������������list�������
		s.emplace_back(TOKEN_ENDFILE); // #
		s.emplace_back(m_productions.front().getProducitonLeft()); // �ķ���ʼ��
		m_input_stream.emplace_back(TOKEN_SHARP);
		int i = 0;//����������
		while (!s.empty()) {
			SNL_TOKEN_TYPE t = s.back();//ջ��Ԫ��
			SNL_TOKEN_TYPE r = m_input_stream[static_cast<unsigned int>(i)];//��������ǰɨ�����
			if (!Token_Terminal_Map.find(t)->second) {
				//���ռ��� Ѱ�� t->ɶ ��Predict�� ������������ǰ��������
				bool notFound = true;
				for (map<int, set<SNL_TOKEN_TYPE>>::iterator a = m_predict_set.begin(); a != m_predict_set.end();a++) {
					const Production& cur_prod = m_productions[a->first - 1];
					if (cur_prod.getProducitonLeft() != t)continue;
					set<SNL_TOKEN_TYPE>::iterator f;
					if (Token_Type_Name_Map.find(r)->second == "ID") {
						f = a->second.find(TOKEN_ID);
					}
					else {
						f = a->second.find(r);
					}
					if (f == a->second.end())continue;
					// �ҵ��� t->a->second��Predict������ r

					// �����﷨������
					vTree.emplace_back(cur_prod);

					notFound = false;
					s.pop_back();//�ȵ�ջ
					// Ȼ���Ƶ�ʽ������ջ
					const vector<SNL_TOKEN_TYPE>& cur_right = cur_prod.getProductionRight();
					for (auto it = cur_right.rbegin(); \
						it != cur_right.rend(); \
						it++) {
						s.emplace_back(*it);
					}
					break;
				}
				if (notFound) {
					system("pause");
					return 0;
					// ����
					/*errMsg = "[Grammatical error] line " +
						to_string(tokenSymbols[static_cast<unsigned int>(i)].line) + " Current parsing \'" +
						r.v + "\'";
					return E_GRAMMAR;*/
					//cerr << "[error] " << __LINE__ << " " << t << " can not predict " << r << endl;
					//cerr << "at line " << tokenSymbols[i].line << endl;
					//exit(-1);
				}
				else {
					continue;// ����
				}
			}
			else {
				if (Token_Type_Name_Map.find(t)->second == "ID") {
					// �ǹؼ��ֵı�ʶ��
					if (Token_Type_Name_Map.find(r)->second == "ID") {
						// ƥ����!!!
						s.pop_back();
						i++;
						continue;
					}
					else {
						system("pause");
						return 0;
						/*errMsg = "[Grammatical error] line " +
							to_string(tokenSymbols[static_cast<unsigned int>(i)].line) +
							" Current parsing \'" + r.v + "\'";
						return E_GRAMMAR;*/
						// ����
						//cerr << "[error] " << __LINE__ << " Top of stack is " << t << " but head of queue is " << r
						//	<< endl;
						//cerr << "at line " << tokenSymbols[i].line << endl;
						//exit(-1);
					}
				}
				else if (t == TOKEN_BLANK) {
					// �մ���ֱ�ӵ�ջ����
					s.pop_back();
					continue;
				}
				else if (Token_Type_Name_Map.find(t)->second == "INTC") {
					// �޷�������
					if (Token_Type_Name_Map.find(r)->second == "INTC") {
						// ƥ����!!!
						s.pop_back();
						i++;
						continue;
					}
					else {
						system("pause");
						return 0;
						/*errMsg = "[Grammatical error] line " +
							to_string(tokenSymbols[static_cast<unsigned int>(i)].line) +
							" Current parsing \'" + r.v + "\'";
						return E_GRAMMAR;*/
						// ����
						//cerr << "[error] " << __LINE__ << " Top of stack is " << t << " but head of queue is " << r
						//	<< endl;
						//cerr << "at line " << tokenSymbols[i].line << endl;
						//exit(-1);
					}

				}
				else {
					// ������
					if (t == r) {
						// ƥ����!!!
						if (t == TOKEN_ENDFILE) {
							// �����!!!
							break;
						}
						s.pop_back();
						i++;
						continue;
					}
					else {
						system("pause");
						return 0;
						//// ����
						//errMsg = "[Grammatical error] line " +
						//	to_string(tokenSymbols[static_cast<unsigned int>(i)].line) +
						//	" Current parsing \'" + r.v + "\'";
						//return E_GRAMMAR;
						//cerr << "[error] " << __LINE__ << " Top of stack is " << t << " but head of queue is " << r
						//	<< endl;
						//cerr << "at line " << tokenSymbols[i].line << endl;
						//exit(-1);
					}
				}
			}
		}
		//cout << endl << "Grammar analysis completed! 0 error!" << endl;
		return 0;



}

void ProductionSet::setInputStrem(const vector<SNL_TOKEN_TYPE>& input_stream)
{
	m_input_stream = input_stream;
}





string ProductionSet::getTree()
{
	


		int global_id = 1;

		Node* root = new Node(vTree[0].getProducitonLeft(), nullptr, global_id++);
		vector<SNL_TOKEN_TYPE> cur_right = vTree[0].getProductionRight();
		for (auto iter = cur_right.begin(); iter != cur_right.end();iter++) {
			root->children.emplace_back(new Node(*iter, root, global_id++));
		}
		treeIndex = 1;
		global_token_index = 0;
		for (unsigned int i = 0; i < root->children.size(); i++) {
			if (Token_Terminal_Map.find(root->children[i]->curr)->second)continue;
			dfsBuildTree(root->children[i]);
		}

		std::stringstream ss;//��DOT Language��ʾ���﷨�����ַ����ŵ������

		ss.clear();
		ss << "digraph GrammarTree {" << std::endl;
		std::queue<Node*> q;
		q.push(root);
		while (!q.empty()) {
			Node* c = q.front();
			q.pop();

			// �ýڵ����ʽ������
			if (Token_Terminal_Map.find(c->curr)->second) {
				if (c->curr == TOKEN_BLANK) {
					ss << "\"" << c->id << "\" [shape=square; style=filled; fillcolor=cornsilk; label=\""
						<< "��" << "\"];" << std::endl;
				}
				else if (Token_Type_Name_Map.find(c->curr)->second == "ID") {
					ss << "\"" << c->id << "\" [shape=square; style=filled; fillcolor=lightpink; label=\""
						<< c->curr_str << "\"];" << std::endl;
				}
				else {
					ss << "\"" << c->id << "\" [shape=square; style=filled; fillcolor=chartreuse1; label=\""
						<< c->curr_str << "\"];" << std::endl;
				}
			}
			else {
				ss << "\"" << c->id << "\" [style=filled; fillcolor=cyan; label=\""
					<< Token_Type_Name_Map.find(c->curr)->second << "\"];" << std::endl;
			}

			if (c->children.size() == 0) {
				// Ҷ�ӽ��
				continue;
			}

			// �������ڵ�Ĺ�ϵ
			string children = "";
			for (unsigned int i = 0; i < c->children.size(); i++) {
				children += "\"" + std::to_string(c->children[i]->id) + "\"; "; // "id; "
			}
			ss << "\"" << c->id << "\" -> {" << children << "}" << std::endl;
			ss << "{rank=same; " << children << "}" << std::endl;

			// ��ջ
			for (Node* nd : c->children) {
				q.push(nd);
			}
		}
		ss << "}" << std::endl;
		return ss.str();
}
