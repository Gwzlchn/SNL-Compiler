#ifndef _SNL_LL1_H
#define _SNL_LL1_H


#include"SNL_Tokens.h"

#include<map>
#include<string>
#include<vector>
#include<set>
#include<stack>
#include<list>
using std::vector;
using std::string;
using std::map;
using std::pair;
using std::set;
using std::stack;


extern const map<SNL_TOKEN_TYPE, bool> Token_Terminal_Map;

class Production {
private:

	SNL_TOKEN_TYPE m_left = TOKEN_BLANK;
	vector<SNL_TOKEN_TYPE> m_right = { TOKEN_BLANK };
	size_t m_id = -1;
	size_t m_look_ahead_idx = -1;	//当前展望符分界，即小圆点；

public:
	bool operator < (const Production& prod) const;
	size_t get_id();
	Production();
	Production(SNL_TOKEN_TYPE left, vector<SNL_TOKEN_TYPE> right, int id, int idx = 0);

	bool isLeftTerminal();

	SNL_TOKEN_TYPE getProducitonLeft()const;
	vector<SNL_TOKEN_TYPE> getProductionRight() const;

	set<SNL_TOKEN_TYPE> getProdTer();
	set<SNL_TOKEN_TYPE> getProdNotTer();



};



struct Node {
	SNL_TOKEN_TYPE curr;
	string curr_str;
	vector<Node*> children;
	Node* parent;
	int id;

	explicit Node(const SNL_TOKEN_TYPE& s, Node* parent, int id) : curr(s), parent(parent), id(id) {}
};



class ProductionSet {
private:
	vector<Production> m_productions;
	//bool m_is_init;		//这个set可能有两个用处，一个是用来存全部文法，另一个是用来存放闭包；

	//每一个非终极符的Follow集
	map<SNL_TOKEN_TYPE, set<SNL_TOKEN_TYPE>> m_follow_sets;
	map<SNL_TOKEN_TYPE, set<SNL_TOKEN_TYPE>> m_first_sets;
	typedef  size_t Prod_Idx;
	//Predict 集合
	map<Prod_Idx, set<SNL_TOKEN_TYPE>> m_predict_set;
	//SNL_ 分析表
	vector<vector<int>> m_LL1_Analyse_Map;

	set<SNL_TOKEN_TYPE> m_terminal;
	set<SNL_TOKEN_TYPE> m_notTerminal;

	//输入流
	//stack<SNL_TOKEN_TYPE, vector<SNL_TOKEN_TYPE>> m_input_stream;

	vector<SNL_TOKEN_TYPE> m_input_stream;


public:

	ProductionSet(string prods_file_name);

	vector<Production> makePordsFromFile(const string& file_name)const;



	//ProductionSet(vector<Production> productions, bool is_init);
	//set<Production> getProductionClosure(Production one_prod);
	void getProdsFirstSet();

	void setUnion(set<SNL_TOKEN_TYPE>& dst, const set<SNL_TOKEN_TYPE>& src);
	//bool isBlankInTokenFirst(const SNL_TOKEN_TYPE& tok) const ;
	template <typename T>
	string get_token_str(const T& tok_vec)const;


	void printSetMap(const map<SNL_TOKEN_TYPE, set<SNL_TOKEN_TYPE>>& sets) const;

	void printPredictMap() const;

	set<SNL_TOKEN_TYPE> setRemoveBlank(const set<SNL_TOKEN_TYPE>& src) const;

	bool isNotTerDeriBlank(SNL_TOKEN_TYPE not_ter);

	int getTokenType(SNL_TOKEN_TYPE tok);

	void getProdsFollowSet();

	//找该终极符是否在右侧表达式出现,出现则返回true,否则返回false;
	//如果出现,同时返回紧接着字符的First集
	//bool getAfterTokenInRightProd(const SNL_TOKEN_TYPE& to_find, const Production& prod, SNL_TOKEN_TYPE& after_token);

	bool getAfterTokenInRightProd(const SNL_TOKEN_TYPE& to_find, const Production& prod, vector<SNL_TOKEN_TYPE>& after_token);

	void setPredictSet();
	set<SNL_TOKEN_TYPE> getTokenVecFirst(const vector<SNL_TOKEN_TYPE>& tok_vec);
	set<SNL_TOKEN_TYPE> getOneProdPredict(const Production& prod);


	//SNL_ 分析表
	void setAnalyseMap();
	void PrintLL1AnalyseMap();
	bool SNL_AnalyseProcess(const vector<SNL_TOKEN_TYPE>& token_input_vec);
	//bool SNL_AnalyseProcess();
	void printStack(const stack<SNL_TOKEN_TYPE>& tok_stack) const;
	size_t getProdIdFromAnalyseMap(SNL_TOKEN_TYPE ana_tok, SNL_TOKEN_TYPE in_tok);
	void pushProdToAnaylseStack(int prod_id, stack<SNL_TOKEN_TYPE>& ana_stack);

	string getTree();




	int treeIndex = 0;
	vector<Production> vTree;
	int global_id = 0;
	//Node* root;
	unsigned int global_token_index;//用于将ID转化为变量名后者函数名等


	void dfsBuildTree(Node*& parent);


	/**
	 * 语法分析
	 */
	int grammarAnalysis();




	void setInputStrem(const vector<SNL_TOKEN_TYPE>& input_stream);

};




#endif // !_SNL__H



