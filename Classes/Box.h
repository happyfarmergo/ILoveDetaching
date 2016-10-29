#ifndef _BOX_H_
#define _BOX_H_

#include "cocos2d.h"
#include "EnumBoxType.h"

USING_NS_CC;

class Box :public Sprite
{
public:
	Box();
	static Box *createWithPos(int row, int column);
	bool initWithPos(int row, int column);
	CC_SYNTHESIZE(bool, m_ignoreCheck, IgnoreCheck);//仅在一次检查过程中有效
	CC_SYNTHESIZE(bool, m_isNeedRemove, IsNeedRemove);
	CC_SYNTHESIZE(int, m_row, Row);
	CC_SYNTHESIZE(int, m_col, Col);
	CC_SYNTHESIZE(int, m_index, Index);
	CC_SYNTHESIZE(EnumBoxType, m_boxType, BoxType);
	void setDisplayMode(EnumBoxType boxType);
};


#endif