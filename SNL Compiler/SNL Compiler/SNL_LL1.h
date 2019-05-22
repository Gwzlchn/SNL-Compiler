#ifndef _SNL_LL1_H
#define _SNL_LL1_H


#include"SNL_Tokens.h"
#include "SNL_Production.h"

#include<map>
#include<string>
#include<vector>
#include<set>
#include<list>
#include<sstream>
using std::vector;
using std::string;
using std::map;
using std::pair;
using std::set;
using std::stringstream;
using std::ostream;



extern const map<SNL_TOKEN_TYPE, bool> Token_Terminal_Map;

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
	//���в���ʽ
	vector<Production> m_productions;
	//Follow Set
	map<SNL_TOKEN_TYPE, set<SNL_TOKEN_TYPE>> m_follow_sets;
	//First Set
	map<SNL_TOKEN_TYPE, set<SNL_TOKEN_TYPE>> m_first_sets;
	//Predict ���ϣ�����ʽ���->Token_type ����
	map<int, set<SNL_TOKEN_TYPE>> m_predict_set;
	//SNL ������
	vector<vector<int>> m_LL1_Analyse_Map;
	//�ռ�������
	set<SNL_TOKEN_TYPE> m_terminal;
	//���ռ�������
	set<SNL_TOKEN_TYPE> m_notTerminal;
	//������
	vector<SNL_TOKEN_TYPE> m_input_stream;
	//����ʽ˳��
	vector<Production> vTree;

public:

	ProductionSet(string prods_file_name);
	vector<Production> makePordsFromFile(const string& file_name)const;
	void setInputStrem(const vector<SNL_TOKEN_TYPE>& input_stream);
	
	//�ռ��������ռ���
	void setTokenTerminalOrNot();
	//First ��������
	void setTokenFirstSet();
	//Follow ��������
	void setTokenFollowSet();
	//Predict ��������
	void setPredictSet();
	//SNL_ ������
	void  setAnalyseMap();

	int treeIndex = 0;
	int global_id = 0;
	unsigned int global_token_index;//���ڽ�IDת��Ϊ���������ߺ�������

	string getTree();
	void dfsBuildTree(Node*& parent);
	int grammarAnalysis();
	

	//���켯��ʱ�õ��ĺ���
	set<SNL_TOKEN_TYPE> setRemoveBlank(const set<SNL_TOKEN_TYPE>& src) const;
	void setUnion(set<SNL_TOKEN_TYPE>& dst, const set<SNL_TOKEN_TYPE>& src);
	bool isNotTerDeriBlank(SNL_TOKEN_TYPE not_ter);
	int getTokenType(SNL_TOKEN_TYPE tok);
	//�Ҹ��ռ����Ƿ����Ҳ���ʽ����,�����򷵻�true,���򷵻�false;
	//�������,ͬʱ���ؽ������ַ���First��
	bool getAfterTokenInRightProd(const SNL_TOKEN_TYPE& to_find, const Production& prod, vector<SNL_TOKEN_TYPE>& after_token,int prod_inx=0);

	set<SNL_TOKEN_TYPE> getTokenVecFirst(const vector<SNL_TOKEN_TYPE>& tok_vec);
	set<SNL_TOKEN_TYPE> getOneProdPredict(const Production& prod);



	//***************��ӡ���****************
	//�������еõ�Token String
	template <typename T>
	static string get_token_str(const T& tok_vec);
	
	stringstream getAllSetMapToStr() const;

	friend ostream& operator <<(ostream& os, const vector<vector<int>>& LL1_Analyse_Map);
	friend ostream& operator <<(ostream& os, const map<SNL_TOKEN_TYPE, set<SNL_TOKEN_TYPE>>& sets);
	friend ostream& operator <<(ostream& os, const map<int, set<SNL_TOKEN_TYPE>>& predict_set);
};




#endif // !_SNL__H



