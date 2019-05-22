#pragma once
#include"SNL_Tokens.h"

#include<vector>
#include<set>

using std::vector;
using std::set;

class Production {
private:

	SNL_TOKEN_TYPE m_left = TOKEN_BLANK;
	std::vector<SNL_TOKEN_TYPE> m_right = { TOKEN_BLANK };
	size_t m_id = -1;
	size_t m_look_ahead_idx = -1;	//��ǰչ�����ֽ磬��СԲ�㣻

public:
	//����Ĭ�Ϲ��캯����������ֱ�Ӷ������
	Production(SNL_TOKEN_TYPE left, std::vector<SNL_TOKEN_TYPE> right, int id, int idx = 0);
	bool operator < (const Production& prod) const;
	size_t get_id();
	//����Ƿ�Ϊ�ռ���
	bool isLeftTerminal();
	SNL_TOKEN_TYPE getProducitonLeft()const;
	std::vector<SNL_TOKEN_TYPE> getProductionRight() const;
	std::set<SNL_TOKEN_TYPE> getProdTer();
	std::set<SNL_TOKEN_TYPE> getProdNotTer();
};

