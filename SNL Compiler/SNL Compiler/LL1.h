#ifndef _LL1_H
#define _LL1_H

#include<map>
#include<string>
#include<vector>
#include<set>
#include <iostream>


using std::vector;
using std::string;
using std::map;
using std::pair;
using std::set;


enum LL1Token {
	Token_S,
	Token_E,
	Token_T,
	Token_P,
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

	LL1Token getProducitonLeft();
	vector<LL1Token> getProductionRight();

	set<LL1Token> getProdTer();
	set<LL1Token> getProdNotTer();

};

class ProductionSet {
private:
	vector<Production> m_productions;
	//bool m_is_init;		//这个set可能有两个用处，一个是用来存全部文法，另一个是用来存放闭包；
		
	//每一个非终极符的Follow集
	map<LL1Token, set<LL1Token>> FollowSets;
	map<LL1Token, set<LL1Token>> FirstSets;

	set<LL1Token> m_terminal;
	set<LL1Token> m_notTerminal;

public:
		
	ProductionSet();
	//ProductionSet(vector<Production> productions, bool is_init);
	//set<Production> getProductionClosure(Production one_prod);
	void getProdsFirstSet();
	void getProdsFollowSet();

};









#endif // !_LL1_H



