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




ProductionSet::ProductionSet(string prods_file_name) {
	

	m_terminal = set<SNL_TOKEN_TYPE>();
	m_notTerminal = set<SNL_TOKEN_TYPE>();

	m_productions = makePordsFromFile(prods_file_name);
	this->setTokenTerminalOrNot();
	this->setTokenFirstSet();
	this->setTokenFollowSet();
	this->setPredictSet();
	this->setAnalyseMap();

    std::cout << m_first_sets;
   std::cout << m_follow_sets;
	//std::cout << getSetMapToStr(m_first_sets).str();
	//std::cout << getSetMapToStr(m_follow_sets).str();
	//std::cout << getAllSetMapToStr().str();
	//std::cout << getPredictMapToStr().str();
	//std::cout << getLL1AnalyseMapToStr().str();

    //int a = 1;
	//this->PrintLL1AnalyseMap();
}

vector<Production> ProductionSet::makePordsFromFile(const string& file_name) const
{
	vector<Production> ret = vector<Production>();
	std::ifstream infile;
	infile.open(file_name);
	if (!infile)
	{
		std::cerr << "文件打开失败！" << std::endl;
        exit(-1);
	}
	string line;//从文件中读取一行产生式
	string substr;//承载分割后的字符串
	bool equalFlag = false;//标志是否已读到"::="这个符号
    size_t seqNum = 1;//当前行号，也即产生式的编号


	while (getline(infile, line))
	{
		std::istringstream stream(line);//用于分割字符
		SNL_TOKEN_TYPE cur_prod_left = TOKEN_ERROR;
		vector<SNL_TOKEN_TYPE> cur_prod_right = vector<SNL_TOKEN_TYPE>();

		//stream >> substr;//每行的第一个元素必定是行号
		//substr.pop_back();
		while (stream >> substr)
		{
			if (substr == "::=")
			{
				equalFlag = true;
				//cout << "::=" << endl;
				continue;//对等号不做任何处理
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
		equalFlag = false;//一行内容结束，将标志归位
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
		//对终极符集合求并，对非终极符集合求并
		setUnion(m_notTerminal, temp_not);
		setUnion(m_terminal, temp_ter);
	}

}

void ProductionSet::setTokenFirstSet()
{
	//first集合初始化
	for (auto ter_iter = m_terminal.begin(); \
		ter_iter != m_terminal.end(); ter_iter++) {
		set<SNL_TOKEN_TYPE> temp = { *ter_iter };
		m_first_sets[*ter_iter] = temp;
	}
	for (auto not_iter = m_notTerminal.begin(); \
		not_iter != m_notTerminal.end(); not_iter++) {

		m_first_sets[*not_iter] = set<SNL_TOKEN_TYPE>();
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
			set<SNL_TOKEN_TYPE>& cur_not_ter_first_set = m_first_sets.find(*notTer_iter)->second;
			//对所有产生式遍历
			for (auto prod_iter = m_productions.begin(); \
				prod_iter !=  m_productions.end();prod_iter++) {
				if (prod_iter->getProducitonLeft() == *notTer_iter) {

					size_t before = m_first_sets[*notTer_iter].size();
					vector<SNL_TOKEN_TYPE> prod_right = prod_iter->getProductionRight();
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
					//只要有一个非终极符First集扩大,则继续循环
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
 string ProductionSet::get_token_vec_str(const T& tok_vec)
{
	string ret = "";
	for (auto i = tok_vec.begin(); i != tok_vec.end(); i++) {
        ret += get_Token_Str(*i);
		ret += " ";
	}
	return ret;
}

ostream& operator <<(ostream& os, const map<SNL_TOKEN_TYPE, set<SNL_TOKEN_TYPE>>& sets) 
{
	os << "----------------------\n";
	for (auto iter = sets.begin(); iter != sets.end(); iter++) {
        if (!is_Token_Terminal(iter->first)) {
            os <<get_Token_Str(iter->first) << "\t\t\t";
            os << ProductionSet::get_token_vec_str<set<SNL_TOKEN_TYPE>>(iter->second) << std::endl;
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
            ss << get_Token_Str(first_iter->first)<< "\t\t\t";
            ss << get_token_vec_str<set<SNL_TOKEN_TYPE>>(first_iter->second) << std::endl;
			ss << i << ".2\t";
            ss << get_Token_Str(follow_iter->first) << "\t\t\t";
            ss  << get_token_vec_str<set<SNL_TOKEN_TYPE>>(follow_iter->second) << std::endl;
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
        os << ProductionSet::get_token_vec_str<set<SNL_TOKEN_TYPE>>(iter->second) << std::endl;

	}
	return os;
}


ostream& operator <<(ostream& os, const vector<vector<int>>& LL1_Analyse_Map)   {
	const size_t size = LL1_Analyse_Map.size();
	for (size_t i = 0; i < size; i++) {
		for (size_t j = 0; j < size; j++) {
			if (LL1_Analyse_Map[i][j] == 0) {
				continue;
			}
			else {
                os << get_Token_Str((SNL_TOKEN_TYPE)i) << "\t\t";
                os <<get_Token_Str((SNL_TOKEN_TYPE)j) << "\t\t";
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


//非终极符是否能推出空
bool ProductionSet::isNotTerDeriBlank(SNL_TOKEN_TYPE not_ter)
{
	auto iter = m_first_sets.find(not_ter);
	if (iter == m_first_sets.end()) {
		return false;
	}

	//集合里能找到,说明能推出空
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
		return 2; //是终极符
	}
	if (not_iter != m_notTerminal.end()) {
		return 3;	//是非终极符
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

	//找到开始符
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

					//右侧字符能推出空,并入左侧字符的follow
					if (after_fisrt_set.find(TOKEN_BLANK)!=after_fisrt_set.end() ) {
						//std::cout << prod_iter->getProducitonLeft() << std::endl;
						setUnion(cur_not_follow, (m_follow_sets.find(prod_iter->getProducitonLeft()))->second);
					}
				}
				else if (not_ter_in_right >= 1) {
					for (size_t i = 0; i < cur_right.size(); i++) {
						vector<SNL_TOKEN_TYPE> after = vector<SNL_TOKEN_TYPE>();
						getAfterTokenInRightProd(*not_iter, *prod_iter, after, now_prod_right_index);

						set<SNL_TOKEN_TYPE> after_fisrt_set = this->getTokenVecFirst(after);
						set<SNL_TOKEN_TYPE> remove_blank = setRemoveBlank(after_fisrt_set);
						setUnion(cur_not_follow, remove_blank);

						//右侧字符能推出空,并入左侧字符的follow
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

//当前字符的下一个字符是什么
bool ProductionSet::getAfterTokenInRightProd(const SNL_TOKEN_TYPE& to_find, \
											const Production& prod, \
											vector<SNL_TOKEN_TYPE>& after_token,\
                                            size_t prod_inx)
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

	//初始化LL1 分析表
	const size_t size = Token_Terminal_Map.size();
	m_LL1_Analyse_Map = vector<vector<int>>(size, vector<int>(size));
	for (size_t i = 0; i < size; i++) {
		//m_LL1_Analyse_Map[i] = new int[size];
		for (size_t j = 0; j < size; j++) {
			m_LL1_Analyse_Map[i][j] = 0;
		}
	}
//	size_t ter_size = m_terminal.size();

	for (auto prod_iter = m_productions.begin(); \
		prod_iter != m_productions.end(); prod_iter++) {

		SNL_TOKEN_TYPE left = prod_iter->getProducitonLeft();
        size_t prod_id = prod_iter->get_id();
		//找到当前Predict集
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
	// 传过来的参数parent一定是非终极符

    if (is_Token_Terminal(parent->curr) == true) {
		std::cerr << "[error] dfsBuildTree invalid param" << std::endl;
		exit(-1);
	}
	if (treeIndex >= vTree.size()) {
		std::cerr << "[error] dfsBuildTree treeIndex" << std::endl;
		exit(-1);
	}
	// 构造子节点
	const auto& cur_right = vTree[treeIndex].getProductionRight();
	for (auto iter = cur_right.begin(); iter != cur_right.end(); iter++) {
		parent->children.emplace_back(new Node(*iter, parent, global_id++));
	}

	// 构造完推导式下标就加加
	treeIndex++;
	for (unsigned int i = 0; i < parent->children.size(); i++) {
        if (is_Token_Terminal(parent->children[i]->curr)) {
			// 将ID转化为实际值
			if (parent->children[i]->curr == TOKEN_BLANK) {}
			else {
				//cout << tokenSymbols[global_token_index] <<"\t\ttokenlist"<< endl;
				//cout << parent->children[i]->curr << endl;
				//cout << endl;
                parent->children[i]->curr_str = get_Token_Str(m_input_stream[global_token_index]);
				global_token_index++;
			}
			continue;// 终极符是叶节点
		}
		dfsBuildTree(parent->children[i]);
	}
}


int ProductionSet::grammarAnalysis()
{
		// 例 分析栈 #E   输入流 i+i*i#
		std::list<SNL_TOKEN_TYPE> s;//因为栈不能遍历输出所以用list链表代替
		s.emplace_back(TOKEN_ENDFILE); // #
		s.emplace_back(m_productions.front().getProducitonLeft()); // 文法开始符
		m_input_stream.emplace_back(TOKEN_SHARP);
		int i = 0;//输入流索引
		while (!s.empty()) {
			SNL_TOKEN_TYPE t = s.back();//栈顶元素
			SNL_TOKEN_TYPE r = m_input_stream[static_cast<unsigned int>(i)];//输入流当前扫描符号
            if (!is_Token_Terminal(t)) {
				//非终极符 寻找 t->啥 的Predict集 中有输入流当前索引符号
				bool notFound = true;
				for (map<int, set<SNL_TOKEN_TYPE>>::iterator a = m_predict_set.begin(); a != m_predict_set.end();a++) {
                    const Production& cur_prod = m_productions[size_t(a->first - 1)];
					if (cur_prod.getProducitonLeft() != t)continue;
					set<SNL_TOKEN_TYPE>::iterator f;
                    if (get_Token_Str(r) == "ID") {
						f = a->second.find(TOKEN_ID);
					}
					else {
						f = a->second.find(r);
					}
					if (f == a->second.end())continue;
					// 找到了 t->a->second的Predict集中有 r

					// 绘制语法树部分
					vTree.emplace_back(cur_prod);

					notFound = false;
					s.pop_back();//先弹栈
					// 然后推导式倒着入栈
					const vector<SNL_TOKEN_TYPE>& cur_right = cur_prod.getProductionRight();
					for (auto it = cur_right.rbegin(); \
						it != cur_right.rend(); \
						it++) {
						s.emplace_back(*it);
					}
					break;
				}
				if (notFound) {
					// 出错
                    analyse_err =  " Current parsing \'" +\
                            get_Token_Str(r)+ "\' and " + \
                            get_Token_Str(t) +  "\n";
                    return -1;
					//cerr << "[error] " << __LINE__ << " " << t << " can not predict " << r << endl;
					//cerr << "at line " << tokenSymbols[i].line << endl;
					//exit(-1);
				}
				else {
					continue;// 继续
				}
			}
			else {
                if (get_Token_Str(t) == "ID") {
					// 非关键字的标识符
                    if (get_Token_Str(r) == "ID") {
						// 匹配了!!!
						s.pop_back();
						i++;
						continue;
					}
					else {
                        analyse_err =  " Current parsing \'" +\
                                get_Token_Str(r) + "\' and " + \
                                get_Token_Str(t) +  "\n";
                        return -1;
						// 出错
						//cerr << "[error] " << __LINE__ << " Top of stack is " << t << " but head of queue is " << r
						//	<< endl;
						//cerr << "at line " << tokenSymbols[i].line << endl;
						//exit(-1);
					}
				}
				else if (t == TOKEN_BLANK) {
					// 空串，直接弹栈继续
					s.pop_back();
					continue;
				}
                else if (get_Token_Str(t) == "INTC") {
					// 无符号整数
                    if (get_Token_Str(r) == "INTC") {
						// 匹配了!!!
						s.pop_back();
						i++;
						continue;
					}
					else {
                        analyse_err =  " Current parsing \'" +\
                                get_Token_Str(r) + "\' and " + \
                                get_Token_Str(t) +  "\n";
                        return -1;
						// 出错
						//cerr << "[error] " << __LINE__ << " Top of stack is " << t << " but head of queue is " << r
						//	<< endl;
						//cerr << "at line " << tokenSymbols[i].line << endl;
						//exit(-1);
					}

				}
				else {
					// 单独的
					if (t == r) {
						// 匹配了!!!
						if (t == TOKEN_ENDFILE) {
							// 完成了!!!
							break;
						}
						s.pop_back();
						i++;
						continue;
					}
					else {

                        analyse_err =  " Current parsing \'" +\
                                get_Token_Str(r) + "\' and " + \
                                get_Token_Str(t) +  "\n";
                        return -1;
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
            if (is_Token_Terminal(root->children[i]->curr))continue;
			dfsBuildTree(root->children[i]);
		}

		std::stringstream ss;//把DOT Language表示的语法树的字符串放到这里边

		ss.clear();
		ss << "digraph GrammarTree {" << std::endl;
		std::queue<Node*> q;
		q.push(root);
		while (!q.empty()) {
			Node* c = q.front();
			q.pop();

			// 该节点的样式和内容
            if (is_Token_Terminal(c->curr)) {
				if (c->curr == TOKEN_BLANK) {
					ss << "\"" << c->id << "\" [shape=square; style=filled; fillcolor=cornsilk; label=\""
                        << "BLANK" << "\"];" << std::endl;
				}
                else if (get_Token_Str(c->curr)== "ID") {
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
                    << get_Token_Str(c->curr) << "\"];" << std::endl;
			}

			if (c->children.size() == 0) {
				// 叶子结点
				continue;
			}

			// 跟其他节点的关系
			string children = "";
			for (unsigned int i = 0; i < c->children.size(); i++) {
				children += "\"" + std::to_string(c->children[i]->id) + "\"; "; // "id; "
			}
			ss << "\"" << c->id << "\" -> {" << children << "}" << std::endl;
			ss << "{rank=same; " << children << "}" << std::endl;

			// 入栈
			for (Node* nd : c->children) {
				q.push(nd);
			}
		}
		ss << "}" << std::endl;
		return ss.str();
}


map<SNL_TOKEN_TYPE, set<SNL_TOKEN_TYPE>> ProductionSet::get_First_Sets() const {
    return m_first_sets;
}
map<SNL_TOKEN_TYPE, set<SNL_TOKEN_TYPE>> ProductionSet::get_Follow_Sets()const{
    return m_follow_sets;
}
map<int, set<SNL_TOKEN_TYPE>> ProductionSet::get_Predict_Sets()const{
    return m_predict_set;
}
set<SNL_TOKEN_TYPE> ProductionSet::get_All_Terminals()const{
   return m_terminal;
}
set<SNL_TOKEN_TYPE> ProductionSet::get_All_Not_Terminals()const{
    return m_notTerminal;
}

string ProductionSet::get_errMsg() const{
    return analyse_err;
}
