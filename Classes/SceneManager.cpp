#include "SceneManager.h"
#include "WelcomeScene.h"
#include "TollgateScene.h"
#include "GameOverScene.h"
#include "SetScene.h"
USING_NS_CC;

SceneManager *SceneManager::m_sceneManager = NULL;

SceneManager *SceneManager::getInstance()
{
	if (m_sceneManager == NULL)
	{
		m_sceneManager = new SceneManager();
		if (m_sceneManager&&m_sceneManager->init())
		{
			m_sceneManager->autorelease();
			m_sceneManager->retain();
		}
		else
		{
			CC_SAFE_FREE(m_sceneManager);
			m_sceneManager = NULL;
		}
	}
	return m_sceneManager;
}

bool SceneManager::init()
{
	return true;
}

void SceneManager::changeScene(EnumSceneType sceneType)
{
	Scene *pScene = NULL;
	switch (sceneType)
	{
	case en_WelcomeScene:			pScene = WelcomeScene::createScene();			break;
	case en_TollgateScene:			pScene = TollgateScene::createScene();			break;
	case en_SetScene:				pScene = SetScene::createScene();				break;
	default:
		break;
	}
	if (pScene == NULL)
		return;
	double time = 1.0;
	Director *pDirector = Director::getInstance();
	Scene *curScene = pDirector->getRunningScene();
	if (curScene == NULL)
		pDirector->runWithScene(pScene);
	else
	{
		switch (sceneType)
		{
		case en_WelcomeScene:		pDirector->replaceScene(TransitionSplitRows::create(time, pScene));			break;
		case en_TollgateScene:		pDirector->replaceScene(TransitionPageTurn::create(time, pScene,false));	break;
		case en_SetScene:			pDirector->pushScene(TransitionProgressInOut::create(time, pScene));		break;
		default:																								break;
		}
	}
}