#ifndef _SLR1_H


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


enum SLR1Token {
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

#define TERMIN true
#define NOTTER false

extern map<SLR1Token, pair<string, bool>> TokenNameMap;

class Production {
private:
	SLR1Token m_left;
	vector<SLR1Token> m_right;
	int m_id;
	int m_look_ahead_idx;	//当前展望符分界，即小圆点；

public:
	bool operator < (const Production& prod) const;
	int get_id();
	Production();
	Production(SLR1Token left, vector<SLR1Token> right, int id, int idx = 0);

	bool isLeftTerminal();

	SLR1Token getProducitonLeft();
	vector<SLR1Token> getProductionRight();

};

class ProductionSet {
private:
	vector<Production> m_productions;
	bool m_is_init;		//这个set可能有两个用处，一个是用来存全部文法，另一个是用来存放闭包；
		
	//每一个非终极符的Follow集
	map<SLR1Token, set<SLR1Token>> FollowSets;
	map<SLR1Token, set<SLR1Token>> FirstSets;
	set<SLR1Token> getNotTerFollowSet(SLR1Token notTer);
	set<SLR1Token> getNotTerFirstSet(SLR1Token notTer);

public:
		
	ProductionSet();
	ProductionSet(vector<Production> productions, bool is_init);
	set<Production> getProductionClosure(Production one_prod);

};









#endif // !_SLR1_H



