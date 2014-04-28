//
//  RPGStartSceneLayer.cpp
//  OzgGameRPG
//
//  Created by ozg on 14-3-17.
//
//

#include "RPGStartSceneLayer.h"
#include "OzgCCScrollBgNode.h"
#include "OzgCCUtility.h"
#include "OzgFileUtility.h"
#include "RPGLoadingSceneLayer.h"
#include "RPGMapSceneLayer.h"

RPGStartSceneLayer::RPGStartSceneLayer()
{
    
}

RPGStartSceneLayer::~RPGStartSceneLayer()
{
    this->removeAllChildrenWithCleanup(true);
    
    if(this->m_stringList)
    {
        this->m_stringList->release(); //释放语言数据
        this->m_stringList = NULL;
    }
    
    //CCTextureCache::sharedTextureCache()->dumpCachedTextureInfo();
}

bool RPGStartSceneLayer::init()
{
    if(RPGBaseSceneLayer::init())
    {
        //加载语言文件
        string languageFile = CCFileUtils::sharedFileUtils()->fullPathForFilename("scene_start_cns.plist");
        this->m_stringList = CCDictionary::createWithContentsOfFileThreadSafe(languageFile.c_str());
        this->m_stringList->retain();
        
        CCTextureCache::sharedTextureCache()->addImage("Window_b_00.png");
        
        //背景
        OzgCCScrollBgNode *bg = OzgCCScrollBgNode::create("start_bg.png");
        bg->setAnchorPoint(CCPointZero);
        bg->setPosition(CCPointZero);
        bg->setTag(kRPGStartSceneLayerTagBg);
        this->addChild(bg);
        
        CCMenu *mainMenu = CCMenu::create();
        mainMenu->setAnchorPoint(CCPointZero);
        mainMenu->setPosition(CCPointZero);
        mainMenu->setTag(kRPGStartSceneLayerTagMenu);
        this->addChild(mainMenu);
        
        CCString *menuItemSingleModeStr = (CCString*)this->m_stringList->objectForKey("menuitem_start");
        CCLabelTTF *menuItemSingleModeLabel = CCLabelTTF::create(OzgCCUtility::generalString(menuItemSingleModeStr->getCString()).c_str(), "FZCuYuan-M03S", 28, CCSizeMake(120, 40), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
        
        CCString *menuItemOnLineModeStr = (CCString*)this->m_stringList->objectForKey("menuitem_delete");
        CCLabelTTF *menuItemOnLineModeLabel = CCLabelTTF::create(OzgCCUtility::generalString(menuItemOnLineModeStr->getCString()).c_str(), "FZCuYuan-M03S", 28, CCSizeMake(120, 40), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
        
        CCMenuItemLabel *menuItemSingleMode = CCMenuItemLabel::create(menuItemSingleModeLabel, this, menu_selector(RPGStartSceneLayer::onMenu));
        menuItemSingleMode->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width / 2, 170));
        menuItemSingleMode->setTag(kRPGStartSceneLayerTagMenuItemStart);
        mainMenu->addChild(menuItemSingleMode);
        
        CCMenuItemLabel *menuItemOnLineMode = CCMenuItemLabel::create(menuItemOnLineModeLabel, this, menu_selector(RPGStartSceneLayer::onMenu));
        menuItemOnLineMode->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width / 2, 110));
        menuItemOnLineMode->setTag(kRPGStartSceneLayerTagMenuItemDelete);
        mainMenu->addChild(menuItemOnLineMode);
        
        //背景音乐
        if(!SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
            SimpleAudioEngine::sharedEngine()->playBackgroundMusic("audio_start.mp3", true);
        
        return true;
    }
    return false;
}

CCScene* RPGStartSceneLayer::scene()
{
    CCScene* s = CCScene::create();
    RPGStartSceneLayer *layer = RPGStartSceneLayer::create();
    s->addChild(layer);
    return s;
}

void RPGStartSceneLayer::onMenu(cocos2d::CCObject *pObject)
{
    CCMenuItem *menuItem = (CCMenuItem*)pObject;
    
    switch (menuItem->getTag())
    {
        case kRPGStartSceneLayerTagMenuItemDelete:
        {
            SimpleAudioEngine::sharedEngine()->playEffect("audio_effect_btn.wav");
            
            CCSize winSize = CCDirector::sharedDirector()->getWinSize();
            
            RPGDialogLayer *dialog = RPGDialogLayer::create(((CCString*)this->m_stringList->objectForKey("confirm_delete"))->getCString(), ((CCString*)this->m_stringList->objectForKey("confirm_ok"))->getCString(), kRPGStartSceneLayerTagDialogOK, ((CCString*)this->m_stringList->objectForKey("confirm_cancel"))->getCString(), kRPGStartSceneLayerTagDialogCancel, winSize.width, winSize.height, this, menu_selector(RPGStartSceneLayer::onDialog));
            dialog->setTag(kRPGStartSceneLayerTagDialog);
            this->addChild(dialog);
            
            CCMenu *mainMenu = (CCMenu*)this->getChildByTag(kRPGStartSceneLayerTagMenu);
            mainMenu->setEnabled(false);
        }
            break;
        default:
        {
            //kRPGStartSceneLayerTagMenuItemStart
            
            SimpleAudioEngine::sharedEngine()->playEffect("audio_start_btn.wav");
            this->goToMapScene();
        }
            break;
    }
    
}

void RPGStartSceneLayer::onDialog(cocos2d::CCObject *pObject)
{
    SimpleAudioEngine::sharedEngine()->playEffect("audio_effect_btn.wav");
    
    CCMenuItem *menuItem = (CCMenuItem*)pObject;
    
    switch (menuItem->getTag())
    {
        case kRPGStartSceneLayerTagDialogCancel:
        {
            this->removeChildByTag(kRPGStartSceneLayerTagDialog, true);
            
            CCMenu *mainMenu = (CCMenu*)this->getChildByTag(kRPGStartSceneLayerTagMenu);
            mainMenu->setEnabled(true);
        }
            break;
        default:
        {
            //kRPGStartSceneLayerTagMenuItemDialogOK
            
            //清除数据
            this->m_db.execDML("delete from save_data");
            this->m_db.execDML("delete from items_existing");
            this->m_db.execDML(GAME_INIT_SQL);
            
            this->goToMapScene();
        }
            break;
    }
}

void RPGStartSceneLayer::goToMapScene()
{
    CCArray *loadTextures = CCArray::create();
    loadTextures->addObject(CCString::create("map.png"));
    loadTextures->addObject(CCString::create("main.png"));
    loadTextures->addObject(CCString::create("joystick.png"));
    loadTextures->addObject(CCString::create("actor4_0.png"));
    loadTextures->addObject(CCString::create("actor111.png"));
    loadTextures->addObject(CCString::create("actor113.png"));
    loadTextures->addObject(CCString::create("actor114.png"));
    loadTextures->addObject(CCString::create("actor115.png"));
    loadTextures->addObject(CCString::create("actor117.png"));
    loadTextures->addObject(CCString::create("actor120.png"));
    
    CCMenu *mainMenu = (CCMenu*)this->getChildByTag(kRPGStartSceneLayerTagMenu);
    mainMenu->setEnabled(false);
    
    CCScene *s = RPGLoadingSceneLayer::scene(loadTextures, NULL, "single_map");
    CCTransitionFade *t = CCTransitionFade::create(GAME_SCENE, s);
    CCDirector::sharedDirector()->replaceScene(t);
}
