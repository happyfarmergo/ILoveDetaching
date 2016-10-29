#ifndef _SETSCENE_H_
#define _SETSCENE_H_

#include "cocos2d.h"
USING_NS_CC;

class SetScene :public Layer
{
public:
	CREATE_FUNC(SetScene);
	static Scene* createScene();
	virtual bool init();

	void menuBackCallback(Ref *pSender);
	void menuMusicCallback(Ref *pSender);
	void menuSoundCallback(Ref *pSender);
};

#endif