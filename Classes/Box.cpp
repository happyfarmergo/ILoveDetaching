#include "Box.h"
#include "GlobalDefine.h"

Box::Box()
:
m_col(0), 
m_row(0), 
m_index(0),
m_isNeedRemove(false),
m_ignoreCheck(false),
m_boxType(en_Normal_Mode)
{
}

Box *Box::createWithPos(int row, int column)
{
	Box *box = new Box();
	if (box&&box->initWithPos(row, column))
	{
		box->autorelease();
		box->retain();
		box->initWithSpriteFrameName(BoxNormal[box->getIndex()]);
	}
	else
	{
		CC_SAFE_DELETE(box);
		box = NULL;
	}
	return box;
}

bool Box::initWithPos(int row, int column)
{
	if (!Sprite::init())
		return false;
	this->setRow(row);
	this->setCol(column);
	this->setIndex(rand() % BOX_NUM);
	return true;
}

void Box::setDisplayMode(EnumBoxType boxType)
{
	setBoxType(boxType);
	SpriteFrame *frame = NULL;
	switch (boxType)
	{
	case en_Horizontal_Mode:
		frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(BoxHorizontal[this->getIndex()]);
		break;
	case en_Vertical_Mode:
		frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(BoxVertical[this->getIndex()]);
		break;
	case en_Triangle_Mode:
		frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(BoxTriangle[this->getIndex()]);
		break;
	case en_Eat_Mode:
		frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(BoxEaten);
		break;
	default:
		break;
	}
	setDisplayFrame(frame);
}