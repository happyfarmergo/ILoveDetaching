#include "AppDelegate.h"
#include "WelcomeScene.h"
#include "SceneManager.h"
#include "GlobalDefine.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLView::create("I Love Detaching");
		glview->setFrameSize(320, 480);
        director->setOpenGLView(glview);
    }
	glview->setDesignResolutionSize(480, 800, ResolutionPolicy::NO_BORDER);
	
	SceneManager::getInstance()->changeScene(EnumSceneType::en_WelcomeScene);

	//加载图片、音乐以及音效
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Box/boxs.plist", "Box/boxs.png");

	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("Music/music_bg.mp3");
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("Music/music_welcomeScene.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("Music/music_explode.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("Music/music_explodeOne.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("Music/music_gameOver.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("Music/music_fail.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("Music/music_win.wav");

	UserDefault::getInstance()->setBoolForKey(MUSIC_KEY, true);
	UserDefault::getInstance()->setBoolForKey(SOUND_KEY, true);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
