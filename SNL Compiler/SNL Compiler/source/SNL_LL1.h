#ifndef SNL_LL1_H
#define SNL_LL1_H


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
	//所有产生式
	vector<Production> m_productions;
	//Follow Set
	map<SNL_TOKEN_TYPE, set<SNL_TOKEN_TYPE>> m_follow_sets;
	//First Set
	map<SNL_TOKEN_TYPE, set<SNL_TOKEN_TYPE>> m_first_sets;
	//Predict 集合，产生式标号->Token_type 集合
	map<int, set<SNL_TOKEN_TYPE>> m_predict_set;
	//SNL 分析表
	vector<vector<int>> m_LL1_Analyse_Map;
	//终极符集合
	set<SNL_TOKEN_TYPE> m_terminal;
	//非终极符集合
	set<SNL_TOKEN_TYPE> m_notTerminal;
	//输入流
	vector<SNL_TOKEN_TYPE> m_input_stream;
	//产生式顺序
	vector<Production> vTree;

    string analyse_err = "";

public:

	ProductionSet(string prods_file_name);
	vector<Production> makePordsFromFile(const string& file_name)const;
	void setInputStrem(const vector<SNL_TOKEN_TYPE>& input_stream);
	
	//终极符、非终极符
	void setTokenTerminalOrNot();
	//First 集合生成
	void setTokenFirstSet();
	//Follow 集合生成
	void setTokenFollowSet();
	//Predict 集合生成
	void setPredictSet();
	//SNL_ 分析表
	void  setAnalyseMap();

    map<SNL_TOKEN_TYPE, set<SNL_TOKEN_TYPE>> get_First_Sets()const;
    map<SNL_TOKEN_TYPE, set<SNL_TOKEN_TYPE>> get_Follow_Sets()const;
    map<int, set<SNL_TOKEN_TYPE>> get_Predict_Sets()const;
    set<SNL_TOKEN_TYPE> get_All_Terminals()const;
    set<SNL_TOKEN_TYPE> get_All_Not_Terminals()const;
    string get_errMsg() const;

    size_t treeIndex = 0;
    size_t global_id = 0;
	unsigned int global_token_index;//用于将ID转化为变量名后者函数名等

	string getTree();
	void dfsBuildTree(Node*& parent);
	int grammarAnalysis();
	

	//构造集合时用到的函数
	set<SNL_TOKEN_TYPE> setRemoveBlank(const set<SNL_TOKEN_TYPE>& src) const;
	void setUnion(set<SNL_TOKEN_TYPE>& dst, const set<SNL_TOKEN_TYPE>& src);
	bool isNotTerDeriBlank(SNL_TOKEN_TYPE not_ter);
	int getTokenType(SNL_TOKEN_TYPE tok);
	//找该终极符是否在右侧表达式出现,出现则返回true,否则返回false;
	//如果出现,同时返回紧接着字符的First集
    bool getAfterTokenInRightProd(const SNL_TOKEN_TYPE& to_find, const Production& prod, vector<SNL_TOKEN_TYPE>& after_token,size_t prod_inx=0);

	set<SNL_TOKEN_TYPE> getTokenVecFirst(const vector<SNL_TOKEN_TYPE>& tok_vec);
	set<SNL_TOKEN_TYPE> getOneProdPredict(const Production& prod);



	//***************打印相关****************
	//从容器中得到Token String
	template <typename T>
    static string get_token_vec_str(const T& tok_vec);
	stringstream getAllSetMapToStr() const;




	
};



ostream& operator <<(ostream& os, const vector<vector<int>>& LL1_Analyse_Map) ;
ostream& operator <<(ostream& os, const map<SNL_TOKEN_TYPE, set<SNL_TOKEN_TYPE>>& sets) ;
ostream& operator <<(ostream& os, const map<int, set<SNL_TOKEN_TYPE>>& predict_set) ;


#endif // !_SNL__H





