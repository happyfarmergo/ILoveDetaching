#ifndef _BOXMANAGER_H_
#define _BOXMANAGER_H_

#include "cocos2d.h"
#include "GlobalDefine.h"
USING_NS_CC;

class Box;
class BoxManager :public Node
{
public:
	BoxManager();
	~BoxManager();
	CREATE_FUNC(BoxManager);
	virtual bool init();
	virtual void update(float dt);
	bool onTouchBegan(Touch *touch, Event *unused);
	//看交换之后能不能消除，忽视了消除的种类
	void onTouchMoved(Touch *touch, Event *unused);
	CC_SYNTHESIZE(EventListenerTouchOneByOne *, m_listener, Listener);
private:
	Point mapOrigion;
	Box *m_boxMatrix[ROW][COLUMN];
	Box *m_startBox;
	Box *m_endBox;
	bool m_isMoving;
	bool m_isTouchEnable;
	bool m_isNeedToFill;
	bool fourDetachRow;
private:
	//获得Box的位置(基于坐标系)
	Point getPointOfBox(int row, int col);
	//获得Box的位置(基于BoxMatrix)
	Box *getBoxOfPoint(Point *point);
	//创造Box
	void createBox(int row, int col);
	//初始化地图
	void initBox();
	// 检测是否有可消除精灵  
	void checkAndRemoveBox();
	// 标记可以移除的精灵  横向、纵向、其他消除方法均由此产生
	void markRemove(Box* box);
	// 移除精灵  
	void removeBox();

	/* 精灵的爆炸移除 */

	//消除一个
	void explodeBox(Box* box);
	//消除整行特效
	void explodeOneRow(Point point);
	//消除整列特效
	void explodeOneCol(Point point);
	//消除一个矩形块特效
	void explodeTriangle(Point point);
	//消除某一类特效
	void explodeSpecialBox(Box *box, int index);

	// 纵向检查  
	void searchOneCol(Box *box, std::list<Box *> &chainList);
	// 横向检查
	void searchOneRow(Box *box, std::list<Box *> &chainList);

	// 填补空缺
	void fillBox();
	//交换Box(基于BoxMatrix和视觉)
	void swapBox();
	//交换Box(基于BoxMatrix)
	void swap();
	//游戏无法消除的情况
	bool noBoxToDetach();

	int find(int r1, int c1, int r2, int c2, int r3, int c3);
};

#endif