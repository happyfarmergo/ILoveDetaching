#ifndef _WELCOMESCENE_H_
#define _WELCOMESCENE_H_

#include "cocos2d.h"
#include "GlobalInclude.h"
USING_NS_CC;

class WelcomeScene :public Layer
{
public:
	static Scene *createScene();
	CREATE_FUNC(WelcomeScene);
	virtual bool init();
private:
	void gameStartBtnOnClick(Ref *,TouchEventType type);
	void gameExitBtnOnClick(Ref *, TouchEventType type);
	void gameSetBtnOnClick(Ref *, TouchEventType type);

};


#endif