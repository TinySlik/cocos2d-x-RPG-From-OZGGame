//
//  RPGBattleMenu.cpp
//  OzgGameRPG
//
//  Created by ozg on 14-4-6.
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
    
    CCTMXTiledMap *bgLayer = (CCTMXTiledMap*)this->m_parentNode->getChildByTag(kRPGBattleMenuTagBg);
    if(bgLayer)
        bgLayer->removeFromParentAndCleanup(true);
    
    if(this->m_parentNode->getChildByTag(kRPGBattleMenuTagNameLab))
        this->m_parentNode->removeChildByTag(kRPGBattleMenuTagNameLab, true);
    
    if(this->m_parentNode->getChildByTag(kRPGBattleMenuTagSeparate))
        this->m_parentNode->removeChildByTag(kRPGBattleMenuTagSeparate, true);
    
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
        
        //角色名字
        addLab(this->m_parentNode, kRPGBattleMenuTagNameLab, CCString::create(playerData->m_name), 20, kCCTextAlignmentCenter, ccp(830, 292));
        CCLabelTTF *nameLab = (CCLabelTTF*)this->m_parentNode->getChildByTag(kRPGBattleMenuTagNameLab);
        nameLab->setFontFillColor(ccc3(144, 144, 144));
        
        //分割线
        CCSprite *separate = CCSprite::createWithSpriteFrameName("separate.png");
        separate->setTag(kRPGBattleMenuTagSeparate);
        separate->setPosition(ccp(830, 275));
        separate->setScaleX(0.34);
        this->m_parentNode->addChild(separate);
        
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
    CCTMXTiledMap *bgLayer = (CCTMXTiledMap*)this->m_parentNode->getChildByTag(kRPGBattleMenuTagBg);
    bgLayer->setVisible(true);
    
    CCLabelTTF *nameLab = (CCLabelTTF*)this->m_parentNode->getChildByTag(kRPGBattleMenuTagNameLab);
    nameLab->setVisible(true);
    
    CCSprite *separate = (CCSprite*)this->m_parentNode->getChildByTag(kRPGBattleMenuTagSeparate);
    separate->setVisible(true);
    
    CCMenuItem *menuAttack = (CCMenuItem*)this->getChildByTag(kRPGBattleMenuTagAttack);
    menuAttack->setVisible(true);
    
    CCMenuItem *menuSkill = (CCMenuItem*)this->getChildByTag(kRPGBattleMenuTagSkill);
    menuSkill->setVisible(true);
    
    CCMenuItem *menuItems = (CCMenuItem*)this->getChildByTag(kRPGBattleMenuTagItems);
    menuItems->setVisible(true);
    
    CCMenuItem *menuEscape = (CCMenuItem*)this->getChildByTag(kRPGBattleMenuTagEscape);
    menuEscape->setVisible(true);
}

void RPGBattleMenu::hideMenu()
{
    CCTMXTiledMap *bgLayer = (CCTMXTiledMap*)this->m_parentNode->getChildByTag(kRPGBattleMenuTagBg);
    bgLayer->setVisible(false);
    
    CCLabelTTF *nameLab = (CCLabelTTF*)this->m_parentNode->getChildByTag(kRPGBattleMenuTagNameLab);
    nameLab->setVisible(false);
    
    CCSprite *separate = (CCSprite*)this->m_parentNode->getChildByTag(kRPGBattleMenuTagSeparate);
    separate->setVisible(false);
    
    CCMenuItem *menuAttack = (CCMenuItem*)this->getChildByTag(kRPGBattleMenuTagAttack);
    menuAttack->setVisible(false);
    
    CCMenuItem *menuSkill = (CCMenuItem*)this->getChildByTag(kRPGBattleMenuTagSkill);
    menuSkill->setVisible(false);
    
    CCMenuItem *menuItems = (CCMenuItem*)this->getChildByTag(kRPGBattleMenuTagItems);
    menuItems->setVisible(false);
    
    CCMenuItem *menuEscape = (CCMenuItem*)this->getChildByTag(kRPGBattleMenuTagEscape);
    menuEscape->setVisible(false);
    
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
        case kRPGBattleMenuTagCancel:
        {
            CCLog("取消");
            this->showMenu();
            
            CCMenuItem *menuCancel = (CCMenuItem*)this->getChildByTag(kRPGBattleMenuTagCancel);
            menuCancel->removeFromParentAndCleanup(true);
            
            switch (this->m_selectedMenuTag)
            {
                case kRPGBattleMenuTagAttack:
                    ((RPGBattleSceneLayer*)this->m_parentNode)->enabledTouched(false);
                    ((RPGBattleSceneLayer*)this->m_parentNode)->cancelAllSelected();
                    
                    break;
                    
//                default:
//                    break;
            }
            
        }
            break;
        default:
        {
            CCLog("攻击");
            this->hideMenu();
            
            this->m_selectedMenuTag = kRPGBattleMenuTagAttack;
            
            CCMenuItemSprite *menuCancel = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("commons_btn_back_04.png"), CCSprite::createWithSpriteFrameName("commons_btn_back_04.png"), this, menu_selector(RPGBattleMenu::onMenu));
            menuCancel->setPosition(ccp(75, 660));
            menuCancel->setTag(kRPGBattleMenuTagCancel);
            menuCancel->setScale(0.75);
            this->addChild(menuCancel);
            
            ((RPGBattleSceneLayer*)this->m_parentNode)->enabledTouched(true);
            
        }
            break;
    }
    
}
