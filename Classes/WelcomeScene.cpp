#include "WelcomeScene.h"
#include "SceneManager.h"
#include "GlobalDefine.h"
#include "TollgateScene.h"
#include "EnumSceneType.h"

Scene *WelcomeScene::createScene()
{
	auto scene = Scene::create();
	auto layer = WelcomeScene::create();
	scene->addChild(layer);
	return scene;
}

bool WelcomeScene::init()
{
	if (!Layer::init())
		return false;

	if (UserDefault::getInstance()->getBoolForKey(MUSIC_KEY))
		SimpleAudioEngine::getInstance()->playBackgroundMusic("Music/music_welcomeScene.mp3", true);

	auto UI = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("WelcomeScene/Welcome_1.ExportJson");

	UI->setAnchorPoint(Point(0.5,0.5));
	UI->setPosition(Point(SCREEN_WIDTH / 2 - 10, SCREEN_HEIGHT / 2));
	this->addChild(UI);
	UI->setTouchEnabled(false);

	auto gameStartBtn = (Button *)Helper::seekWidgetByName(UI, "gameStartBtn");
	auto gameExitBtn = (Button *)Helper::seekWidgetByName(UI, "gameExitBtn");
	auto gameSetBtn = (Button *)Helper::seekWidgetByName(UI, "gameSetBtn");
	
	gameStartBtn->addTouchEventListener(this, toucheventselector(WelcomeScene::gameStartBtnOnClick));
	gameExitBtn->addTouchEventListener(this, toucheventselector(WelcomeScene::gameExitBtnOnClick));
	gameSetBtn->addTouchEventListener(this, toucheventselector(WelcomeScene::gameSetBtnOnClick));

	return true;
}

void WelcomeScene::gameStartBtnOnClick(Ref *, TouchEventType type)
{
	if (type == TouchEventType::TOUCH_EVENT_ENDED)
	{
		SceneManager::getInstance()->changeScene(EnumSceneType::en_TollgateScene);
	}
}

void WelcomeScene::gameExitBtnOnClick(Ref *, TouchEventType type)
{
	if (type == TouchEventType::TOUCH_EVENT_ENDED)
	{
		Director::getInstance()->end();
	}
}

void WelcomeScene::gameSetBtnOnClick(Ref *, TouchEventType type)
{
	if (type == TouchEventType::TOUCH_EVENT_ENDED)
	{
		SceneManager::getInstance()->changeScene(EnumSceneType::en_SetScene);
	}
}
