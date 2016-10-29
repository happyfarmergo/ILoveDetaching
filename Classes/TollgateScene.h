#ifndef _TOLLGATESCENE_H_
#define _TOLLGATESCENE_H_

#include "cocos2d.h"
USING_NS_CC;

class TimeCounter;
class TollgateScene :public Layer
{
public:
	TollgateScene();
	static Scene *createScene();
	virtual bool init();
	virtual void update(float dt);

	CREATE_FUNC(TollgateScene);
	void menuBackTCallBack(Ref *pSender);
	void menuPause(Ref *pSender);
	void initBackground();
	void myClock(float dt);
	void gameOver(float dt);
private:
	MenuItemToggle *m_pauseBtn;
	TimeCounter *m_timeCounter;
	ProgressTimer *m_pt;
	int m_time;
};

#endif