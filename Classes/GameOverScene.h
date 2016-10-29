#ifndef _GAMEOVERSCENE_H_
#define _GAMEOVERSCENE_H_

#include "cocos2d.h"
#include "GlobalInclude.h"
USING_NS_CC;

class GameOverScene :public Layer
{
public:
	static Scene *createWithScore(int score);
	virtual bool init();
	virtual void cleanup();
	CREATE_FUNC(GameOverScene);
	void setScore(int sc);
	void gameBackBtnOnClick(Ref *pSender, TouchEventType type);
	void gameRestartBtnOnClick(Ref *pSender, TouchEventType type);
};

#endif