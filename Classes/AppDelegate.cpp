#include "AppDelegate.h"
#include "SimpleAudioEngine.h"
#include "OzgFileUtility.h"
#include "RPGStartSceneLayer.h"

USING_NS_CC;

using namespace CocosDenshion;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();

    pDirector->setOpenGLView(pEGLView);
	
    CCEGLView::sharedOpenGLView()->setDesignResolutionSize(960, 640, kResolutionShowAll);
    
    // turn on display FPS
    pDirector->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    //将数据库复制到可写目录
    string dbSrc = CCFileUtils::sharedFileUtils()->fullPathForFilename(GAME_SYS_DB);
    string dbDes = CCFileUtils::sharedFileUtils()->getWritablePath();
    dbDes.append(GAME_SYS_DB);
    
    if(!OzgFileUtility::fileExists(dbDes))
        OzgFileUtility::copyFile(dbSrc.c_str(), dbDes.c_str());
    CCLog("%s", dbDes.c_str());
    
    //记录检查，如果没有记录则生成
    CppSQLite3DB db;
    db.open(dbDes.c_str());
    
    CppSQLite3Query query = db.execQuery("select count(id) from save_data");
    //    CCLog("%i", query.getIntField(0));
    if(query.getIntField(0) == 0)
        db.execDML(GAME_INIT_SQL);
    
    query.finalize();
    db.close();
    
    //初始化背景音乐和效果音的默认大小值
    SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(CCUserDefault::sharedUserDefault()->getFloatForKey(GAME_BG_AUDIO_VOLUME, 1.0));
    SimpleAudioEngine::sharedEngine()->setEffectsVolume(CCUserDefault::sharedUserDefault()->getFloatForKey(GAME_EFFECT_AUDIO_VOLUME, 1.0));
    
    // create a scene. it's an autorelease object
    CCScene *pScene = RPGStartSceneLayer::scene();

    // run
    pDirector->runWithScene(pScene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    CCDirector::sharedDirector()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    CCDirector::sharedDirector()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
