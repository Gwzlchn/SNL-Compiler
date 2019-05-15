#ifndef _LL1_H
#define _LL1_H

#include<map>
#include<string>
#include<vector>
#include<set>
#include <iostream>
#include<stack>

using std::vector;
using std::string;
using std::map;
using std::pair;
using std::set;
using std::stack;


enum LL1Token {
	Token_S,
	Token_E,
	Token_T,
	Token_P,
	Token_E_Hat,
	Token_T_Hat,
	Token_F,
	Token_id,
	Token_left_paren,
	Token_right_paren,
	Token_add,
	Token_mul,
	Token_eof,
	Token_start,
	Token_Blank,
	


};


extern map<LL1Token, pair<string, bool>> TokenNameMap;


class Production {
private:
	LL1Token m_left;
	vector<LL1Token> m_right;
	int m_id;
	int m_look_ahead_idx;	//当前展望符分界，即小圆点；

public:
	bool operator < (const Production& prod) const;
	int get_id();
	Production();
	Production(LL1Token left, vector<LL1Token> right, int id, int idx = 0);

	bool isLeftTerminal();

	LL1Token getProducitonLeft()const;
	vector<LL1Token> getProductionRight() const ;

	set<LL1Token> getProdTer();
	set<LL1Token> getProdNotTer();

	

};

class ProductionSet {
private:
	vector<Production> m_productions;
	//bool m_is_init;		//这个set可能有两个用处，一个是用来存全部文法，另一个是用来存放闭包；
		
	//每一个非终极符的Follow集
	map<LL1Token, set<LL1Token>> m_follow_sets;
	map<LL1Token, set<LL1Token>> m_first_sets;
	typedef  int Prod_Idx;
	//Predict 集合
	map<Prod_Idx, set<LL1Token>> m_predict_set;
	//LL1 分析表
	map<LL1Token, vector<pair<LL1Token, Prod_Idx>>> m_LL1_analyse_map;

	set<LL1Token> m_terminal;
	set<LL1Token> m_notTerminal;

	//输入流
	stack<LL1Token, vector<LL1Token>> m_input_stream;
	

public:
		
	ProductionSet();
	stack<LL1Token, vector<LL1Token>> makeInputStreamFromPage61();
	vector<Production> makeProdsFromPage77();
	vector<Production> makeProdsFromPage53();
	//ProductionSet(vector<Production> productions, bool is_init);
	//set<Production> getProductionClosure(Production one_prod);
	void getProdsFirstSet();
	
	void setUnion(set<LL1Token>& dst, const set<LL1Token>& src);
	bool isBlankInTokenFirst(const LL1Token& tok) const ;
	template <typename T>
	string get_token_str(const T& tok_vec)const  ;

	
	void printSetMap(const map<LL1Token, set<LL1Token>>& sets) const;

	void printPredictMap() const;

	set<LL1Token> setRemoveBlank(const set<LL1Token>& src) const ;

	bool isNotTerDeriBlank(LL1Token not_ter);

	int getTokenType(LL1Token tok);

	void getProdsFollowSet();

	//找该终极符是否在右侧表达式出现,出现则返回true,否则返回false;
	//如果出现,同时返回紧接着字符的First集
	bool getAfterTokenInRightProd(const LL1Token& to_find, const Production& prod, LL1Token& after_token);

	void setPredictSet();
	set<LL1Token> getTokenVecFirst(const vector<LL1Token>& tok_vec) ;
	set<LL1Token> getOneProdPredict(const Production& prod) ;


	//LL1 分析表
	void setAnalyseMap();
	bool LL1AnalyseProcess();
	void printStack(const stack<LL1Token>& tok_stack) const;
	int getProdIdFromAnalyseMap(LL1Token ana_tok, LL1Token in_tok);
	void pushProdToAnaylseStack(int prod_id, stack<LL1Token>& ana_stack);
};






#endif // !_LL1_H



