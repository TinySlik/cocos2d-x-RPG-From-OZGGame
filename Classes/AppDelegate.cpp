#include "cocos2d.h"
#include "CCEGLView.h"
#include "AppDelegate.h"
#include "CCLuaEngine.h"
#include "SimpleAudioEngine.h"
#include "Lua_extensions_CCB.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include "Lua_web_socket.h"
#endif

#include "OzgFileUtility.h"
#include "RPGStartSceneLayer.h"

using namespace CocosDenshion;

USING_NS_CC;

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
    SimpleAudioEngine::end();
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    CCDirector *pDirector = CCDirector::sharedDirector();
    pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());

    CCEGLView::sharedOpenGLView()->setDesignResolutionSize(960, 640, kResolutionShowAll);
    
    // turn on display FPS
    pDirector->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    // register lua engine
//    CCLuaEngine* pEngine = CCLuaEngine::defaultEngine();
//    CCScriptEngineManager::sharedManager()->setScriptEngine(pEngine);
//
//    CCLuaStack *pStack = pEngine->getLuaStack();
//    lua_State *tolua_s = pStack->getLuaState();
//    tolua_extensions_ccb_open(tolua_s);
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
//    pStack = pEngine->getLuaStack();
//    tolua_s = pStack->getLuaState();
//    tolua_web_socket_open(tolua_s);
//#endif
//    
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_BLACKBERRY)
//    CCFileUtils::sharedFileUtils()->addSearchPath("script");
//#endif
//
//    std::string path = CCFileUtils::sharedFileUtils()->fullPathForFilename("hello.lua");
//    pEngine->executeScriptFile(path.c_str());
    
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
    
    CCScene *mainScene = RPGStartSceneLayer::scene();
    pDirector->runWithScene(mainScene);
    
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->stopAnimation();

    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->startAnimation();

    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
