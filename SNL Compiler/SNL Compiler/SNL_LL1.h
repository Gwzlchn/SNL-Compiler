#ifndef _SNL_LL1_H
#define _SNL_LL1_H


#include"SNL_Tokens.h"

#include<map>
#include<string>
#include<vector>
#include<set>
#include<stack>

using std::vector;
using std::string;
using std::map;
using std::pair;
using std::set;
using std::stack;


extern const map<SNL_TOKEN_TYPE, bool> Token_Terminal_Map;

class Production {
private:
	SNL_TOKEN_TYPE m_left;
	vector<SNL_TOKEN_TYPE> m_right;
	int m_id;
	int m_look_ahead_idx;	//��ǰչ�����ֽ磬��СԲ�㣻

public:
	bool operator < (const Production& prod) const;
	int get_id();
	Production();
	Production(SNL_TOKEN_TYPE left, vector<SNL_TOKEN_TYPE> right, int id, int idx = 0);

	bool isLeftTerminal();

	SNL_TOKEN_TYPE getProducitonLeft()const;
	vector<SNL_TOKEN_TYPE> getProductionRight() const;

	set<SNL_TOKEN_TYPE> getProdTer();
	set<SNL_TOKEN_TYPE> getProdNotTer();



};

class ProductionSet {
private:
	vector<Production> m_productions;
	//bool m_is_init;		//���set�����������ô���һ����������ȫ���ķ�����һ����������űհ���

	//ÿһ�����ռ�����Follow��
	map<SNL_TOKEN_TYPE, set<SNL_TOKEN_TYPE>> m_follow_sets;
	map<SNL_TOKEN_TYPE, set<SNL_TOKEN_TYPE>> m_first_sets;
	typedef  int Prod_Idx;
	//Predict ����
	map<Prod_Idx, set<SNL_TOKEN_TYPE>> m_predict_set;
	//SNL_ ������
	map<SNL_TOKEN_TYPE, vector<pair<SNL_TOKEN_TYPE, Prod_Idx>>> m_SNL_analyse_map;

	set<SNL_TOKEN_TYPE> m_terminal;
	set<SNL_TOKEN_TYPE> m_notTerminal;

	//������
	stack<SNL_TOKEN_TYPE, vector<SNL_TOKEN_TYPE>> m_input_stream;


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

	//�Ҹ��ռ����Ƿ����Ҳ����ʽ����,�����򷵻�true,���򷵻�false;
	//�������,ͬʱ���ؽ������ַ���First��
	//bool getAfterTokenInRightProd(const SNL_TOKEN_TYPE& to_find, const Production& prod, SNL_TOKEN_TYPE& after_token);

	bool getAfterTokenInRightProd(const SNL_TOKEN_TYPE& to_find, const Production& prod, vector<SNL_TOKEN_TYPE>& after_token);

	void setPredictSet();
	set<SNL_TOKEN_TYPE> getTokenVecFirst(const vector<SNL_TOKEN_TYPE>& tok_vec);
	set<SNL_TOKEN_TYPE> getOneProdPredict(const Production& prod);


	//SNL_ ������
	void setAnalyseMap();
	bool SNL_AnalyseProcess(const vector<SNL_TOKEN_TYPE>& token_input_vec);
	//bool SNL_AnalyseProcess();
	void printStack(const stack<SNL_TOKEN_TYPE>& tok_stack) const;
	int getProdIdFromAnalyseMap(SNL_TOKEN_TYPE ana_tok, SNL_TOKEN_TYPE in_tok);
	void pushProdToAnaylseStack(int prod_id, stack<SNL_TOKEN_TYPE>& ana_stack);
};






#endif // !_SNL__H


