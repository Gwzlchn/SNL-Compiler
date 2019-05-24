
#include"SNL_Tokens.h"

#include<vector>
#include<set>

using std::vector;
using std::set;

class Production {
private:

	SNL_TOKEN_TYPE m_left = TOKEN_BLANK;
	std::vector<SNL_TOKEN_TYPE> m_right = { TOKEN_BLANK };
    size_t m_id = 0xFF;
    size_t m_look_ahead_idx =  0xFF;	//当前展望符分界，即小圆点；

public:
	//不用默认构造函数，以类内直接定义代替
    Production(SNL_TOKEN_TYPE left, std::vector<SNL_TOKEN_TYPE> right, size_t id, size_t idx = 0 );
	bool operator < (const Production& prod) const;
	size_t get_id();
	//左侧是否为终极符
	bool isLeftTerminal();
	SNL_TOKEN_TYPE getProducitonLeft()const;
	std::vector<SNL_TOKEN_TYPE> getProductionRight() const;
	std::set<SNL_TOKEN_TYPE> getProdTer();
	std::set<SNL_TOKEN_TYPE> getProdNotTer();
};

