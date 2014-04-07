//
//  RPGLoadingSceneLayer.cpp
//  OzgGameRPG
//
//  Created by ozg on 14-1-5.
//
//

#include "RPGLoadingSceneLayer.h"
#include "RPGMapSceneLayer.h"
#include "RPGBattleSceneLayer.h"
#include "OzgCCUtility.h"

RPGLoadingSceneLayer::~RPGLoadingSceneLayer()
{
    if(this->m_loadTextures) this->m_loadTextures->release();
    if(this->m_releaseTextures) this->m_releaseTextures->release();
    
//    CCTextureCache::sharedTextureCache()->dumpCachedTextureInfo();
}

bool RPGLoadingSceneLayer::init(CCArray* loadTextures, CCArray* releaseTextures, string nextScene)
{
    if(RPGBaseSceneLayer::init())
    {
        this->m_loadTextures = loadTextures;
        this->m_releaseTextures = releaseTextures;
        this->m_nextScene = nextScene;
        
        if(this->m_loadTextures) this->m_loadTextures->retain();
        if(this->m_releaseTextures) this->m_releaseTextures->retain();
        
        //加载语言文件
        string languageFile = CCFileUtils::sharedFileUtils()->fullPathForFilename("scene_loading_cns.plist");
        
        this->m_stringList = CCDictionary::createWithContentsOfFileThreadSafe(languageFile.c_str());
        
        CCString *loadingStr = (CCString*)this->m_stringList->objectForKey("loading");
        CCLabelTTF *loadingLabel = CCLabelTTF::create(OzgCCUtility::generalString(loadingStr->getCString()).c_str(), "FZCuYuan-M03S", 20, CCSizeMake(CCDirector::sharedDirector()->getWinSize().width, 30), kCCTextAlignmentCenter);
        
        loadingLabel->setColor(ccc3(255, 255, 255));
        loadingLabel->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width / 2, 100));
        this->addChild(loadingLabel);
        
        if(!this->m_loadTextures)
            this->scheduleOnce(schedule_selector(RPGLoadingSceneLayer::goToNextScene), GAME_SCENE + 0.5);
        else
            this->scheduleOnce(schedule_selector(RPGLoadingSceneLayer::executeLoading), GAME_SCENE + 0.5);
        
        return true;
    }
    return false;
}

RPGLoadingSceneLayer* RPGLoadingSceneLayer::create(CCArray* loadTextures, CCArray* releaseTextures, string nextScene)
{
    RPGLoadingSceneLayer* layer = new RPGLoadingSceneLayer();
    if(layer && layer->init(loadTextures, releaseTextures, nextScene))
    {
        layer->autorelease();
        return layer;
    }
    CC_SAFE_DELETE(layer);
    return NULL;
}

CCScene* RPGLoadingSceneLayer::scene(CCArray* loadTextures, CCArray* releaseTextures, string nextScene)
{
    CCScene *scene = CCScene::create();
    RPGLoadingSceneLayer *layer = RPGLoadingSceneLayer::create(loadTextures, releaseTextures, nextScene);
    scene->addChild(layer);
    return scene;
}

void RPGLoadingSceneLayer::executeLoading(float delay)
{
    //释放旧纹理
    if(this->m_releaseTextures)
    {
        CCObject *item = NULL;
        CCARRAY_FOREACH(this->m_releaseTextures, item)
        {
            CCString *releaseTexture = (CCString*)item;
            CCTextureCache::sharedTextureCache()->removeTextureForKey(releaseTexture->getCString());
            
        }
        
    }
    
    //读取新纹理
    if(this->m_loadTextures)
    {
        this->m_loadingIndex = 0;
        CCString *loadTexture = (CCString*)this->m_loadTextures->objectAtIndex(this->m_loadingIndex);
        CCTextureCache::sharedTextureCache()->addImageAsync(loadTexture->getCString(), this, callfuncO_selector(RPGLoadingSceneLayer::loadCallBack));
    }
    else
        this->goToNextScene();
    
}

void RPGLoadingSceneLayer::loadCallBack(cocos2d::CCObject *sender)
{
    this->m_loadingIndex++;
    if(this->m_loadingIndex >= this->m_loadTextures->count())
    {
        //完毕
        //CCLog("loading 100%%");
        this->goToNextScene();
    }
    else
    {
        //往下一个读取
        //int val = (int)((float)this->m_loadingIndex / (float)this->m_loadTextures->count() * 100.0f);
        //CCLog("loading %i%%", val);
        
        CCString *loadTexture = (CCString*)this->m_loadTextures->objectAtIndex(this->m_loadingIndex);
        CCTextureCache::sharedTextureCache()->addImageAsync(loadTexture->getCString(), this, callfuncO_selector(RPGLoadingSceneLayer::loadCallBack));
    }
    
}

void RPGLoadingSceneLayer::goToNextScene()
{
    //CCTextureCache::sharedTextureCache()->dumpCachedTextureInfo();
    CCScene *s = NULL;
    
    //跳转到目标场景
    if(this->m_nextScene.compare("single_map") == 0)
        s = RPGMapSceneLayer::scene();
    else if(this->m_nextScene.compare("single_battle") == 0)
        s = RPGBattleSceneLayer::scene();
    
    if(s)
    {
        CCTransitionFade *t = CCTransitionFade::create(GAME_SCENE, s);
        CCDirector::sharedDirector()->replaceScene(t);
    }
    
}
