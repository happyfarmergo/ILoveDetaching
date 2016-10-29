#include "GameOverScene.h"
#include "SceneManager.h"
#include "GlobalDefine.h"

Scene *GameOverScene::createWithScore(int score)
{
	auto scene = Scene::create();
	auto layer = GameOverScene::create();
	layer->setScore(score);
	scene->addChild(layer);
	return scene;
}

bool GameOverScene::init()
{
	if (!Layer::init())
		return false;

	TTFConfig config("fonts/yuehei.otf", 48);

	auto labelScore = Label::createWithTTF(config, "  0  ");
	labelScore->setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 * 1.3);
	labelScore->setTag(LABELLAYER);
	this->addChild(labelScore,LABELLAYER);

	auto UI = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("GameOverScene/GameOverScene_1.ExportJson");
	UI->setAnchorPoint(Point(0.5, 0.5));
	UI->setPosition(Point(SCREEN_WIDTH / 2 - 10, SCREEN_HEIGHT / 2));
	UI->setTouchEnabled(false);
	this->addChild(UI);

	auto gameBackBtn = (Button *)Helper::seekWidgetByName(UI, "backBtn");
	auto gameRestartBtn= (Button *)Helper::seekWidgetByName(UI, "restartBtn");
	auto gameScore = (Button *)Helper::seekWidgetByName(UI, "ScoreBtn");
	gameScore->setTouchEnabled(false);

	gameBackBtn->addTouchEventListener(this, toucheventselector(GameOverScene::gameBackBtnOnClick));
	gameRestartBtn->addTouchEventListener(this, toucheventselector(GameOverScene::gameRestartBtnOnClick));

	return true;
}

void GameOverScene::setScore(int sc)
{
	auto labelScore = (Label *)this->getChildByTag(LABELLAYER);
	labelScore->setString(StringUtils::format(" %d ", sc));

	if (USER_DEFAULT->getIntegerForKey("Int") < sc)
	{
		if (USER_DEFAULT->getBoolForKey(SOUND_KEY))
			SimpleAudioEngine::getInstance()->playEffect("Music/music_win.wav", false);

		auto newRecord = Sprite::create("GameOverScene/sprite_newRecord.png");
		newRecord->setPosition(Point(SCREEN_WIDTH / 3.0, SCREEN_HEIGHT / 1.33));
		newRecord->setScale(10.0f);
		newRecord->runAction(ScaleTo::create(1.2f, 1.0));
		this->addChild(newRecord);

		USER_DEFAULT->setIntegerForKey("Int", sc);
		
	}
	else
	{
		if (USER_DEFAULT->getBoolForKey(MUSIC_KEY))
			SimpleAudioEngine::getInstance()->playEffect("Music/music_fail.mp3", false);
	}

}

void GameOverScene::gameBackBtnOnClick(Ref *pSender, TouchEventType type)
{
	if (type == TouchEventType::TOUCH_EVENT_ENDED)
	{
		SceneManager::getInstance()->changeScene(EnumSceneType::en_WelcomeScene);
	}
}

void GameOverScene::gameRestartBtnOnClick(Ref *pSender, TouchEventType type)
{
	if (type == TouchEventType::TOUCH_EVENT_ENDED)
	{
		SceneManager::getInstance()->changeScene(EnumSceneType::en_TollgateScene);
	}
}

void GameOverScene::cleanup()
{
	Layer::cleanup();
	SimpleAudioEngine::getInstance()->stopAllEffects();
}