#include "SetScene.h"
#include "SceneManager.h"
#include "GlobalInclude.h"
#include "GlobalDefine.h"

Scene *SetScene::createScene()
{
	auto scene = Scene::create();
	auto layer = SetScene::create();
	scene->addChild(layer);
	return scene;
}

bool SetScene::init()
{
	if (!Layer::init())
		return false;

	if (UserDefault::getInstance()->getBoolForKey(MUSIC_KEY))
		SimpleAudioEngine::getInstance()->playBackgroundMusic("Music/music_WelcomeScene.wav", true);

	auto UI = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("Set/Set_1.ExportJson");
	UI->setAnchorPoint(Point(0.5, 0.5));
	UI->setPosition(Point(SCREEN_WIDTH / 2 + 5, SCREEN_HEIGHT / 2));
	this->addChild(UI);
	UI->setTouchEnabled(false);

	auto music = (Button *)Helper::seekWidgetByName(UI, "music");
	auto sound = (Button *)Helper::seekWidgetByName(UI, "sound");
	music->setTouchEnabled(false);
	music->setTouchEnabled(false);

	auto backItem = MenuItemImage::create("Set/btn_return.png","Set/btn_return.png",CC_CALLBACK_1(SetScene::menuBackCallback, this));
	backItem->setPosition(Point(backItem->getContentSize().width / 2, SCREEN_HEIGHT - backItem->getContentSize().height));

	auto turnOn = MenuItemImage::create("Set/btn_musicOn.png", "Set/btn_musicOn.png");
	auto turnOff = MenuItemImage::create("Set/btn_musicOff.png", "Set/btn_musicOff.png");
	auto turnOn2 = MenuItemImage::create("Set/btn_musicOn.png", "Set/btn_musicOn.png");
	auto turnOff2 = MenuItemImage::create("Set/btn_musicOff.png", "Set/btn_musicOff.png");

	MenuItemToggle * musicItem = MenuItemToggle::createWithCallback(CC_CALLBACK_1(SetScene::menuMusicCallback, this), turnOn, turnOff, NULL);
	MenuItemToggle * soundItem = MenuItemToggle::createWithCallback(CC_CALLBACK_1(SetScene::menuSoundCallback, this), turnOn2, turnOff2, NULL);
	musicItem->setPosition(Point(SCREEN_WIDTH / 1.4, SCREEN_HEIGHT / 1.5));
	soundItem->setPosition(Point(SCREEN_WIDTH / 1.4, SCREEN_HEIGHT / 1.9));

	int index = USER_DEFAULT->getBoolForKey(MUSIC_KEY) ? 0 : 1;
	musicItem->setSelectedIndex(index);
	index = USER_DEFAULT->getBoolForKey(SOUND_KEY) ? 0 : 1;
	soundItem->setSelectedIndex(index);

	auto menu = Menu::create(backItem, musicItem, soundItem, NULL);
	menu->setPosition(Point::ZERO);
	this->addChild(menu);

	return true;
}

void SetScene::menuBackCallback(Ref *pSender)
{
	Director::getInstance()->popScene();
}

void SetScene::menuMusicCallback(Ref *pSender)
{
	if (USER_DEFAULT->getBoolForKey(MUSIC_KEY)) 
	{
		USER_DEFAULT->setBoolForKey(MUSIC_KEY, false);
	}
	else 
	{
		USER_DEFAULT->setBoolForKey(MUSIC_KEY, true);
	}
}

void SetScene::menuSoundCallback(Ref *pSender)
{
	if (USER_DEFAULT->getBoolForKey(SOUND_KEY)) 
		USER_DEFAULT->setBoolForKey(SOUND_KEY, false);
	else 
		USER_DEFAULT->setBoolForKey(SOUND_KEY, true);
}
