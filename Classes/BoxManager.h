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
	//������֮���ܲ�������������������������
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
	//���Box��λ��(��������ϵ)
	Point getPointOfBox(int row, int col);
	//���Box��λ��(����BoxMatrix)
	Box *getBoxOfPoint(Point *point);
	//����Box
	void createBox(int row, int col);
	//��ʼ����ͼ
	void initBox();
	// ����Ƿ��п���������  
	void checkAndRemoveBox();
	// ��ǿ����Ƴ��ľ���  �����������������������ɴ˲���
	void markRemove(Box* box);
	// �Ƴ�����  
	void removeBox();

	/* ����ı�ը�Ƴ� */

	//����һ��
	void explodeBox(Box* box);
	//����������Ч
	void explodeOneRow(Point point);
	//����������Ч
	void explodeOneCol(Point point);
	//����һ�����ο���Ч
	void explodeTriangle(Point point);
	//����ĳһ����Ч
	void explodeSpecialBox(Box *box, int index);

	// ������  
	void searchOneCol(Box *box, std::list<Box *> &chainList);
	// ������
	void searchOneRow(Box *box, std::list<Box *> &chainList);

	// ���ȱ
	void fillBox();
	//����Box(����BoxMatrix���Ӿ�)
	void swapBox();
	//����Box(����BoxMatrix)
	void swap();
	//��Ϸ�޷����������
	bool noBoxToDetach();

	int find(int r1, int c1, int r2, int c2, int r3, int c3);
};

#endif