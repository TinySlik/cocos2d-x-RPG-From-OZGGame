//
//  RPGMapMenuLayer.cpp
//  OzgGameRPG
//
//  Created by ozg on 14-3-18.
//
//

#include "RPGMapMenuLayer.h"
#include "RPGMapSceneLayer.h"
#include "RPGConfirmLayer.h"
#include "RPGStartSceneLayer.h"
#include "JsonBox.h"
#include "OzgCCUtility.h"
#include "RPGMapStatusMenuLayer.h"
#include "RPGMapItemsMenuLayer.h"
#include "RPGMapEquipMenuLayer.h"
#include "RPGMapSkillMenuLayer.h"

RPGMapMenuLayer::RPGMapMenuLayer()
{
    
}

RPGMapMenuLayer::~RPGMapMenuLayer()
{
    this->m_stringList->release();
    
//    CCTextureCache::sharedTextureCache()->dumpCachedTextureInfo();
    
//    CCLog("RPGMapMenuLayer释放");
}

bool RPGMapMenuLayer::init(CCDictionary* stringList, CppSQLite3DB* db, float width, float height)
{
    if(CCLayerColor::initWithColor(ccc4(0, 0, 0, 200), width, height))
    {
        this->m_stringList = stringList;
        this->m_stringList->retain();
                
        this->m_db = db;
        
        this->m_width = width;
        this->m_height = height;
        
//        CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("main.plist");
        
        CCMenu *mainMenu = CCMenu::create();
        mainMenu->setTag(kRPGMapMenuLayerTagMainMenu);
        mainMenu->setAnchorPoint(CCPointZero);
        mainMenu->setPosition(CCPointZero);
        this->addChild(mainMenu);
        
        this->setMainMenuDefault();
        
        CCTMXTiledMap *mainBg = CCTMXTiledMap::create("map_menu1_style1.tmx");
        mainBg->setPosition(CCPointZero);
        mainBg->setTag(kRPGMapMenuLayerTagMainBg);
        this->addChild(mainBg);
        
        this->setDefaultInterface();
        
        return true;
    }
    return false;
}

RPGMapMenuLayer* RPGMapMenuLayer::create(CCDictionary* stringList, CppSQLite3DB* db, float width, float height)
{
    RPGMapMenuLayer *obj = new RPGMapMenuLayer();
    if(obj && obj->init(stringList, db, width, height))
    {
        obj->autorelease();
        return obj;
    }
    
    CC_SAFE_DELETE(obj);
    return NULL;
}

//private
void RPGMapMenuLayer::onMenu(cocos2d::CCObject *pObject)
{
    CCMenuItem *menuItem = (CCMenuItem*)pObject;    
    SimpleAudioEngine::sharedEngine()->playEffect("audio_effect_btn.wav");
    
    switch (menuItem->getTag())
    {
        case kRPGMapMenuLayerTagMainMenuEquip:
        {
//            CCLog("装备");
            
            //因为动态获取地图的大小会导致了菜单层显示错位，所以定死了
            float width = 960;
            float height = 640;
            RPGMapEquipMenuLayer *menuLayer = RPGMapEquipMenuLayer::create(this->m_stringList, this->m_db, width, height);
            menuLayer->setPosition(ccp((CCDirector::sharedDirector()->getWinSize().width - width) / 2.0, (CCDirector::sharedDirector()->getWinSize().height - height) / 2.0));
            menuLayer->setTag(kRPGMapSceneLayerTagMenuLayer);
            this->getParent()->addChild(menuLayer);
            
            this->removeFromParentAndCleanup(true);
            
        }
            break;
        case kRPGMapMenuLayerTagMainMenuSkill:
        {
//            CCLog("技能");
            
            //因为动态获取地图的大小会导致了菜单层显示错位，所以定死了
            float width = 960;
            float height = 640;
            RPGMapSkillMenuLayer *menuLayer = RPGMapSkillMenuLayer::create(this->m_stringList, this->m_db, width, height);
            menuLayer->setPosition(ccp((CCDirector::sharedDirector()->getWinSize().width - width) / 2.0, (CCDirector::sharedDirector()->getWinSize().height - height) / 2.0));
            menuLayer->setTag(kRPGMapSceneLayerTagMenuLayer);
            this->getParent()->addChild(menuLayer);
            
            this->removeFromParentAndCleanup(true);
            
        }
            break;
        case kRPGMapMenuLayerTagMainMenuStatus:
        {
//            CCLog("状态");
            
            //因为动态获取地图的大小会导致了菜单层显示错位，所以定死了
            float width = 960;
            float height = 640;
            RPGMapStatusMenuLayer *menuLayer = RPGMapStatusMenuLayer::create(this->m_stringList, this->m_db, width, height);
            menuLayer->setPosition(ccp((CCDirector::sharedDirector()->getWinSize().width - width) / 2.0, (CCDirector::sharedDirector()->getWinSize().height - height) / 2.0));
            menuLayer->setTag(kRPGMapSceneLayerTagMenuLayer);
            this->getParent()->addChild(menuLayer);
            
            this->removeFromParentAndCleanup(true);
        }
            break;
        case kRPGMapMenuLayerTagMainMenuClose:
        {
//            CCLog("关闭");
            
            RPGMapSceneLayer *mapSceneLayer = (RPGMapSceneLayer*)this->getParent();
            mapSceneLayer->enabledControl(true);
            
            this->removeFromParentAndCleanup(true);
            
        }
            break;
        case kRPGMapMenuLayerTagMainMenuExit:
        {
            //退出
            RPGConfirmLayer *confirm = RPGConfirmLayer::create(((CCString*)this->m_stringList->objectForKey("confirm_exit"))->getCString(), ((CCString*)this->m_stringList->objectForKey("confirm_exit_ok"))->getCString(), kRPGMapMenuLayerTagConfirmExitOK, ((CCString*)this->m_stringList->objectForKey("confirm_cancel"))->getCString(), kRPGMapMenuLayerTagConfirmCancel, this->m_width, this->m_height, this, menu_selector(RPGMapMenuLayer::onMenu));
            confirm->setTag(kRPGMapMenuLayerTagConfirm);
            this->addChild(confirm);
            
            CCMenu *mainMenu = (CCMenu*)this->getChildByTag(kRPGMapMenuLayerTagMainMenu);
            mainMenu->setEnabled(false);
            
        }
            break;
        case kRPGMapMenuLayerTagConfirmExitOK:
        {
            //确定退出
            RPGMapSceneLayer *mapSceneLayer = (RPGMapSceneLayer*)this->getParent();
            mapSceneLayer->m_releaseTexture = true;
            
            CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(mapSceneLayer);
            mapSceneLayer->unscheduleUpdate();
            
            SimpleAudioEngine::sharedEngine()->stopBackgroundMusic(true);
            
            CCScene *s = RPGStartSceneLayer::scene();
            CCTransitionFade *t = CCTransitionFade::create(GAME_SCENE, s);
            CCDirector::sharedDirector()->replaceScene(t);
        }
            break;
        case kRPGMapMenuLayerTagConfirmCancel:
        {
            this->removeChildByTag(kRPGMapMenuLayerTagConfirm, true);
            
            CCMenu *mainMenu = (CCMenu*)this->getChildByTag(kRPGMapMenuLayerTagMainMenu);
            mainMenu->setEnabled(true);
        }
            break;
        case kRPGMapMenuLayerTagMainMenuItems:
        {
//            CCLog("道具");
            
            //因为动态获取地图的大小会导致了菜单层显示错位，所以定死了
            float width = 960;
            float height = 640;
            RPGMapItemsMenuLayer *menuLayer = RPGMapItemsMenuLayer::create(this->m_stringList, this->m_db, width, height);
            menuLayer->setPosition(ccp((CCDirector::sharedDirector()->getWinSize().width - width) / 2.0, (CCDirector::sharedDirector()->getWinSize().height - height) / 2.0));
            menuLayer->setTag(kRPGMapSceneLayerTagMenuLayer);
            this->getParent()->addChild(menuLayer);
            
            this->removeFromParentAndCleanup(true);
            
        }
            break;
    }
        
}

void RPGMapMenuLayer::createMenuItem(float x, float y, CCString *text, RPGMapMenuLayerTag tag)
{
    CCMenu *mainMenu = (CCMenu*)this->getChildByTag(kRPGMapMenuLayerTagMainMenu);
    
    CCMenuItemLabel *menuItems = (CCMenuItemLabel*)mainMenu->getChildByTag(tag);
    if(!menuItems)
    {
        CCLabelTTF *menuItemsLabel = CCLabelTTF::create(OzgCCUtility::generalString(text->getCString()).c_str(), "FZCuYuan-M03S", 25, CCSizeMake(120, 40), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    
        CCMenuItemLabel *menuItems = CCMenuItemLabel::create(menuItemsLabel, this, menu_selector(RPGMapMenuLayer::onMenu));
        menuItems->setPosition(ccp(x, y));
        menuItems->setTag(tag);
        mainMenu->addChild(menuItems);
        
    }
}

void RPGMapMenuLayer::setMainMenuDefault()
{
    CCMenu *mainMenu = (CCMenu*)this->getChildByTag(kRPGMapMenuLayerTagMainMenu);
    
    CCString *menuItemsStr = (CCString*)this->m_stringList->objectForKey("menu_items");
    this->createMenuItem(835, 570, menuItemsStr, kRPGMapMenuLayerTagMainMenuItems);
    
    CCString *menuEquipStr = (CCString*)this->m_stringList->objectForKey("menu_equip");
    this->createMenuItem(835, 500, menuEquipStr, kRPGMapMenuLayerTagMainMenuEquip);
    
    CCString *menuSkillStr = (CCString*)this->m_stringList->objectForKey("menu_skill");
    this->createMenuItem(835, 430, menuSkillStr, kRPGMapMenuLayerTagMainMenuSkill);
    
    CCString *menuStatusStr = (CCString*)this->m_stringList->objectForKey("menu_status");
    this->createMenuItem(835, 360, menuStatusStr, kRPGMapMenuLayerTagMainMenuStatus);
    
    CCString *menuCloseStr = (CCString*)this->m_stringList->objectForKey("menu_close");
    this->createMenuItem(835, 290, menuCloseStr, kRPGMapMenuLayerTagMainMenuClose);
    
    CCString *menuExitStr = (CCString*)this->m_stringList->objectForKey("menu_exit");
    this->createMenuItem(835, 220, menuExitStr, kRPGMapMenuLayerTagMainMenuExit);
    
    CCObject *item = NULL;
    CCARRAY_FOREACH(mainMenu->getChildren(), item)
    {
        CCMenuItem *menuItem = (CCMenuItem*)item;
        
        if(menuItem->getTag() == kRPGMapMenuLayerTagMainMenuItems ||
           menuItem->getTag() == kRPGMapMenuLayerTagMainMenuEquip ||
           menuItem->getTag() == kRPGMapMenuLayerTagMainMenuSkill ||
           menuItem->getTag() == kRPGMapMenuLayerTagMainMenuStatus ||
           menuItem->getTag() == kRPGMapMenuLayerTagMainMenuClose ||
           menuItem->getTag() == kRPGMapMenuLayerTagMainMenuExit)
            menuItem->setVisible(true);
        else
            menuItem->setVisible(false);
        
    }
}

void RPGMapMenuLayer::setDefaultInterface()
{
    
    //显示左边的4个角色
    float playerY = 550;
    float nameY = 547;
    float levelY = 568;
    float hpY = 543;
    float mpY = 523;
    
    CppSQLite3Query query = this->m_db->execQuery(PLAYER_QUERY);
    while(!query.eof())
    {
        CCArray *frames = OzgCCUtility::createSpriteFrames(CCString::createWithFormat("%s_0.png", query.getStringField("tex_prefix"))->getCString(), 3, 4);
        
        CCSprite *playerSprite = CCSprite::createWithSpriteFrame((CCSpriteFrame*)frames->objectAtIndex(1));
        playerSprite->setPosition(ccp(150, playerY));
        playerSprite->setScale(2);
        playerSprite->setTag(kRPGMapMenuLayerTagDefualtPlayerTex + query.getIntField("id"));
        this->addChild(playerSprite);
        
        addLab(this, kRPGMapMenuLayerTagDefualtPlayerName + query.getIntField("id"), CCString::create(query.getStringField("name_cns")), 22, ccp(410, nameY));
        
        addLab(this, kRPGMapMenuLayerTagDefualtPlayerLevel + query.getIntField("id"), CCString::createWithFormat(((CCString*)this->m_stringList->objectForKey("status_level"))->getCString(), query.getIntField("level"), query.getIntField("next_exp")), 18, ccp(580, levelY));
        
        addLab(this, kRPGMapMenuLayerTagDefualtPlayerHP + query.getIntField("id"), CCString::createWithFormat(((CCString*)this->m_stringList->objectForKey("status_hp"))->getCString(), query.getIntField("hp"), query.getIntField("max_hp")), 16, ccp(601, hpY));
        
        addLab(this, kRPGMapMenuLayerTagDefualtPlayerMP + query.getIntField("id"), CCString::createWithFormat(((CCString*)this->m_stringList->objectForKey("status_mp"))->getCString(), query.getIntField("mp"), query.getIntField("max_mp")), 16, ccp(599, mpY));
        
        playerY -= 150;
        nameY -= 150;
        levelY -= 150;
        hpY -= 150;
        mpY -= 150;
        
        query.nextRow();
    }
    query.finalize();
    
}
