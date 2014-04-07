//
//  RPGBattleMenu.cpp
//  OzgGameRPG
//
//  Created by 欧志刚 on 14-4-6.
//
//

#include "RPGBattleMenu.h"

RPGBattleMenu::RPGBattleMenu()
{
    
}

RPGBattleMenu::~RPGBattleMenu()
{
    this->m_stringList->release();
    this->m_parentNode->release();
    this->m_playerData->release();

    CCLog("RPGBattleMenu 释放");
}

bool RPGBattleMenu::initWithParentNode(CCDictionary* stringList, CCNode* parentNode, RPGPlayer* playerData)
{
    if(CCMenu::init())
    {
        this->m_stringList = stringList;
        this->m_stringList->retain();
        
        this->m_parentNode = parentNode;
        this->m_parentNode->retain();
        
        this->m_playerData = playerData;
        this->m_playerData->retain();
        
        this->setPosition(CCPointZero);
        
        CCTMXTiledMap *bgLayer = (CCTMXTiledMap*)this->m_parentNode->getChildByTag(kRPGBattleMenuTagBg);
        if(!bgLayer)
        {
            bgLayer = CCTMXTiledMap::create("battle_battlemenu_style1.tmx");
            bgLayer->setPosition(ccp(750, 64));
            bgLayer->setTag(kRPGBattleMenuTagBg);
            parentNode->addChild(bgLayer);
        }
        
        CCString *menuAttackStr = (CCString*)this->m_stringList->objectForKey("battle_playermenu_attack");
        this->createMenuItem(830, 250, menuAttackStr, kRPGBattleMenuTagAttack);

        CCString *menuSkillStr = (CCString*)this->m_stringList->objectForKey("battle_playermenu_skill");
        this->createMenuItem(830, 200, menuSkillStr, kRPGBattleMenuTagSkill);

        CCString *menuItemsStr = (CCString*)this->m_stringList->objectForKey("battle_playermenu_items");
        this->createMenuItem(830, 150, menuItemsStr, kRPGBattleMenuTagItems);
        
        CCString *menuEscapeStr = (CCString*)this->m_stringList->objectForKey("battle_playermenu_escape");
        this->createMenuItem(830, 100, menuEscapeStr, kRPGBattleMenuTagEscape);
        
        SimpleAudioEngine::sharedEngine()->playEffect("audio_battle_menu.wav");
        
        return true;
    }
    return false;
}

RPGBattleMenu* RPGBattleMenu::createWithParentNode(CCDictionary* stringList, CCNode* parentNode, RPGPlayer* playerData)
{
    RPGBattleMenu *obj = new RPGBattleMenu();
    if(obj && obj->initWithParentNode(stringList, parentNode, playerData))
    {
        obj->autorelease();
        return obj;
    }
    CC_SAFE_DELETE(obj);
    return NULL;
}

void RPGBattleMenu::showMenu()
{
    CCTMXTiledMap *bgLayer = (CCTMXTiledMap*)this->getChildByTag(kRPGBattleMenuTagBg);
    bgLayer->setVisible(true);
    
    this->setVisible(true);
}

void RPGBattleMenu::hideMenu()
{
    CCTMXTiledMap *bgLayer = (CCTMXTiledMap*)this->getChildByTag(kRPGBattleMenuTagBg);
    bgLayer->setVisible(false);
    
    this->setVisible(false);
}

//private

void RPGBattleMenu::createMenuItem(float x, float y, CCString *text, RPGBattleMenuTag tag)
{
    CCMenuItemLabel *menuItems = (CCMenuItemLabel*)this->getChildByTag(tag);
    if(!menuItems)
    {
        CCLabelTTF *menuItemsLabel = CCLabelTTF::create(OzgCCUtility::generalString(text->getCString()).c_str(), "Arial", 28, CCSizeMake(120, 30), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
        
        CCMenuItemLabel *menuItems = CCMenuItemLabel::create(menuItemsLabel, this, menu_selector(RPGBattleMenu::onMenu));
        menuItems->setPosition(ccp(x, y));
        menuItems->setTag(tag);
        this->addChild(menuItems);
        
    }
}

void RPGBattleMenu::onMenu(cocos2d::CCObject *pObject)
{
    CCMenuItem *menuItem = (CCMenuItem*)pObject;
    SimpleAudioEngine::sharedEngine()->playEffect("audio_effect_btn.wav");
    
    switch (menuItem->getTag())
    {
        case kRPGBattleMenuTagSkill:
        {
            CCLog("技能");
            
        }
            break;
        case kRPGBattleMenuTagItems:
        {
            CCLog("道具");
            
        }
            break;
        case kRPGBattleMenuTagEscape:
        {
//            CCLog("逃跑");
            
            float success = CCRANDOM_0_1();
            if(success >= 0.0 && success <= 0.5)
            {
//                CCLog("逃跑成功");
                
                this->setEnabled(false);
                this->removeFromParentAndCleanup(true);
                
                ((RPGBattleSceneLayer*)this->m_parentNode)->goToMap();
            }
            else
            {
                ((RPGBattleSceneLayer*)this->m_parentNode)->showMsg((CCString*)this->m_stringList->objectForKey("escape_fail"));
                
                //重置进度条
                this->m_playerData->m_progress = 0.0;
                CCProgressTimer *battleProgress = (CCProgressTimer*)this->m_parentNode->getChildByTag(kRPGBattleSceneLayerTagPlayerProgress + this->m_playerData->m_dataId);
                battleProgress->setPercentage(this->m_playerData->m_progress);
                
                ((RPGBattleSceneLayer*)this->m_parentNode)->scheduleUpdate();
                
                CCTMXTiledMap *bgLayer = (CCTMXTiledMap*)this->m_parentNode->getChildByTag(kRPGBattleMenuTagBg);
                bgLayer->removeFromParentAndCleanup(true);
                this->removeFromParentAndCleanup(true);
            }
            
        }
            break;
        default:
        {
            CCLog("攻击");
            
        }
            break;
    }
    
}
