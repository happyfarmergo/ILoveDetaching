#include "TollgateScene.h"
#include "TollgateDataLayer.h"
#include "GlobalInclude.h"
#include "GlobalDefine.h"
#include "SceneManager.h"
#include "GameOverScene.h"
#include "BoxManager.h"
#include "TimeCounter.h"
#include "EnumSceneType.h"

TollgateScene::TollgateScene()
:m_pt(NULL),
m_pauseBtn(NULL),
m_timeCounter(NULL),
m_time(DEADLINETIME)
{
}

Scene *TollgateScene::createScene()
{
	auto scene = Scene::create();
	auto layer = TollgateScene::create();
	scene->addChild(layer);
	return scene;
}

void TollgateScene::initBackground()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();

	Sprite *backGround = Sprite::create("TollgateScene/TollgateScene.png");
	this->addChild(backGround, 0);
	backGround->setAnchorPoint(Point(0.5, 0.5));
	backGround->setPosition(Point(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	backGround->setScale(1.0, 1.2);

	auto backItem = MenuItemImage::create("TollgateScene/btn_back01.png", "TollgateScene/btn_back02.png",CC_CALLBACK_1(TollgateScene::menuBackTCallBack, this));
	backItem->setPosition(Point(SCREEN_WIDTH - backItem->getContentSize().width / 2,backItem->getContentSize().height));

	auto pauseItem = MenuItemImage::create("TollgateScene/pause.png", "TollgateScene/go on.png");
	auto continueItem = MenuItemImage::create("TollgateScene/go on.png", "TollgateScene/pause.png");

	MenuItemToggle *pauseBtn = MenuItemToggle::createWithCallback(CC_CALLBACK_1(TollgateScene::menuPause, this),pauseItem, continueItem, NULL);
	pauseBtn->setPosition(Point(SCREEN_WIDTH - pauseItem->getContentSize().width, SCREEN_HEIGHT - pauseItem->getContentSize().height * 3));
	pauseBtn->setSelectedIndex(0);
	m_pauseBtn = pauseBtn;

	auto menu = Menu::create(backItem, pauseBtn, NULL);
	menu->setPosition(Point::ZERO);
	this->addChild(menu, 1);

	if (!USER_DEFAULT->getIntegerForKey("Int"))
		USER_DEFAULT->setIntegerForKey("Int", 0);

	TTFConfig config("fonts/haibaoti.ttf", 30);

	//最高分
	auto labelHighestScore = Label::createWithTTF(config, "Highest: 0");
	labelHighestScore->setPosition(Point(SCREEN_WIDTH - labelHighestScore->getContentSize().width,
		SCREEN_HEIGHT - labelHighestScore->getContentSize().height * 2));
	labelHighestScore->setString(StringUtils::format("Highest: %d", USER_DEFAULT->getIntegerForKey("Int")));
	this->addChild(labelHighestScore);

	//分数
	auto labelScore = Label::createWithTTF(config, "Score: 0");
	labelScore->setPosition(Point(labelScore->getContentSize().width, SCREEN_HEIGHT - labelScore->getContentSize().height * 4));
	labelScore->setTag(LABELSCORE);
	this->addChild(labelScore);

	// 时间
	auto labelTime = Label::createWithTTF(config, "Time: 60");
	labelTime->setPosition(Point(labelTime->getContentSize().width, SCREEN_HEIGHT - labelTime->getContentSize().height * 2));
	labelTime->setTag(LABELTIME);
	this->addChild(labelTime);

	//进度条
	auto pt = ProgressTimer::create(Sprite::create("TollgateScene/bar.png"));
	pt->setType(ProgressTimerType::BAR);
	pt->setBarChangeRate(Point(1, 0));
	pt->setMidpoint(Point(0, 0));
	pt->setPercentage(100);
	pt->setAnchorPoint(Point(0.5, 0.5));
	pt->setPosition(Point(pt->getContentSize().width / 2 + 50, visibleSize.height - pt->getContentSize().height * 1));
	this->addChild(pt, 3);
	m_pt = pt;
}

bool TollgateScene::init()
{
	if (!Layer::init())
		return false;

	if (UserDefault::getInstance()->getBoolForKey(MUSIC_KEY))
		SimpleAudioEngine::getInstance()->playBackgroundMusic("Music/music_bg.mp3", true);

	initBackground();

	auto timeCounter = TimeCounter::create();
	timeCounter->start();
	this->addChild(timeCounter);
	m_timeCounter = timeCounter;

	auto dataLayer = TollgateDataLayer::create();
	this->addChild(dataLayer, 12, DATALAYER);

	auto boxManager = BoxManager::create();
	this->addChild(boxManager, 10, BOXMANAGER);

	this->scheduleUpdate();
	this->schedule(schedule_selector(TollgateScene::myClock), 1.0f);

	return true;
}

void TollgateScene::myClock(float dt)
{
	--m_time;
	if (m_time == 0)
	{
		SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
		if (UserDefault::getInstance()->getBoolForKey(SOUND_KEY))
			SimpleAudioEngine::getInstance()->playEffect("Music/music_gameOver.mp3", false);

		Label *labelTime = (Label *)this->getChildByTag(LABELTIME);
		labelTime->setScale(0);
		auto boxmanager = (BoxManager *)getChildByTag(BOXMANAGER);
		boxmanager->pause();

		auto sprite = Sprite::create("GameOverScene/pic_gameover.png");
		sprite->setPosition(Point(SCREEN_WIDTH / 2, SCREEN_HEIGHT * 1.5));
		this->addChild(sprite,18);
		sprite->runAction(MoveTo::create(3.0f, Point(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2)));

		scheduleOnce(schedule_selector(TollgateScene::gameOver), 3.0f);

		return;
	}
	auto labelTime = (Label *)this->getChildByTag(LABELTIME);
	labelTime->setString(StringUtils::format("Time: %d", m_time));

}

void TollgateScene::gameOver(float dt)
{
	auto dataLayer = (TollgateDataLayer *)getChildByTag(DATALAYER);
	auto scene = GameOverScene::createWithScore(dataLayer->getScore());
	Director::getInstance()->replaceScene(TransitionFadeUp::create(1.0f, scene));
}

void TollgateScene::update(float dt)
{
	float newPercent = 100 * (1 - m_timeCounter->getCurTime() / (float)DEADLINETIME);
	m_pt->setPercentage(newPercent);
}

void TollgateScene::menuBackTCallBack(Ref *pSender)
{
	SceneManager::getInstance()->changeScene(EnumSceneType::en_WelcomeScene);
}

void TollgateScene::menuPause(Ref *pSender)
{
	if (m_pauseBtn->getSelectedIndex() == 1)
	{
		Director::getInstance()->pause();
		SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
		auto boxmanager = (BoxManager *)getChildByTag(BOXMANAGER);
		boxmanager->getListener()->setEnabled(false);
	}
	else
	{
		Director::getInstance()->resume();
		SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
		auto boxmanager = (BoxManager *)getChildByTag(BOXMANAGER);
		boxmanager->getListener()->setEnabled(true);
	}
}
