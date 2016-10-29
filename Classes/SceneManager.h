#ifndef _SCENEMANAGER_H_
#define _SCENEMANAGER_H_

#include "cocos2d.h"
#include "EnumSceneType.h"

class SceneManager :public cocos2d::Ref
{
public:
	static SceneManager *getInstance();
	virtual bool init();

	void changeScene(EnumSceneType sceneType);
private:
	static SceneManager *m_sceneManager;
};


#endif