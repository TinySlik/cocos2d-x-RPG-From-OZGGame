//
//  RPGDialogLayer.cpp
//  OzgGameRPG
//
//  Created by ozg on 14-3-30.
//
//

#include "RPGDialogLayer.h"
#include "OzgCCUtility.h"
#include "GameCfg.h"

RPGDialogLayer::RPGDialogLayer()
{

}

RPGDialogLayer::~RPGDialogLayer()
{
    CCLog("RPGDialogLayer 释放");
}

bool RPGDialogLayer::init(string titleStr, string menuOKStr, int menuItemOKTag, float width, float height, CCObject* target, SEL_MenuHandler seletor)
{
    if(CCLayerColor::initWithColor(ccc4(0, 0, 0, 200), width, height))
    {
        this->initCommons(titleStr);
        
        CCTMXTiledMap *mainBg = (CCTMXTiledMap*)this->getChildByTag(kRPGDialogLayerBg);
        CCMenu *menu = (CCMenu*)mainBg->getChildByTag(kRPGDialogLayerBgMenu);
        
        CCLabelTTF *menuOKLab = CCLabelTTF::create(OzgCCUtility::generalString(CCString::create(menuOKStr)->getCString()).c_str(), "Arial", 20, CCSizeMake(120, 25), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
        menuOKLab->setColor(ccc3(160, 160, 160));
        CCMenuItemLabel *menuOK = CCMenuItemLabel::create(menuOKLab, target, seletor);
        menuOK->setTag(menuItemOKTag);
        menuOK->setPosition(ccp(190, 60));
        menu->addChild(menuOK);
        
        return true;
    }
    return false;
}

bool RPGDialogLayer::init(string titleStr, string menuOKStr, int menuItemOKTag, string menuCancelStr, int menuItemCancelTag, float width, float height, CCObject* target, SEL_MenuHandler seletor)
{
    if(CCLayerColor::initWithColor(ccc4(0, 0, 0, 200), width, height))
    {
        this->initCommons(titleStr);
        
        CCTMXTiledMap *mainBg = (CCTMXTiledMap*)this->getChildByTag(kRPGDialogLayerBg);
        CCMenu *menu = (CCMenu*)mainBg->getChildByTag(kRPGDialogLayerBgMenu);
        
        CCLabelTTF *menuOKLab = CCLabelTTF::create(OzgCCUtility::generalString(CCString::create(menuOKStr)->getCString()).c_str(), "Arial", 20, CCSizeMake(120, 25), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
        menuOKLab->setColor(ccc3(160, 160, 160));
        CCMenuItemLabel *menuOK = CCMenuItemLabel::create(menuOKLab, target, seletor);
        menuOK->setTag(menuItemOKTag);
        menuOK->setPosition(ccp(100, 60));
        menu->addChild(menuOK);
        
        CCLabelTTF *menuCancelLab = CCLabelTTF::create(OzgCCUtility::generalString(CCString::create(menuCancelStr)->getCString()).c_str(), "Arial", 20, CCSizeMake(120, 25), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
        menuCancelLab->setColor(ccc3(160, 160, 160));
        CCMenuItemLabel *menuCancel = CCMenuItemLabel::create(menuCancelLab, target, seletor);
        menuCancel->setTag(menuItemCancelTag);
        menuCancel->setPosition(ccp(280, 60));
        menu->addChild(menuCancel);
        
        return true;
    }
    
    return false;
}

RPGDialogLayer* RPGDialogLayer::create(string titleStr, string menuOKStr, int menuItemOKTag, float width, float height, CCObject* target, SEL_MenuHandler seletor)
{
    RPGDialogLayer *layer = new RPGDialogLayer();
    if(layer && layer->init(titleStr, menuOKStr, menuItemOKTag, width, height, target, seletor))
    {
        layer->autorelease();
        
        return layer;
    }
    CC_SAFE_DELETE(layer);
    return NULL;
}

RPGDialogLayer* RPGDialogLayer::create(string titleStr, string menuOKStr, int menuItemOKTag, string menuCancelStr, int menuItemCancelTag, float width, float height, CCObject* target, SEL_MenuHandler seletor)
{
    RPGDialogLayer *layer = new RPGDialogLayer();
    if(layer && layer->init(titleStr, menuOKStr, menuItemOKTag, menuCancelStr, menuItemCancelTag, width, height, target, seletor))
    {
        layer->autorelease();
        
        return layer;
    }
    CC_SAFE_DELETE(layer);
    return NULL;
}

//private
void RPGDialogLayer::initCommons(string titleStr)
{
    CCTMXTiledMap *mainBg = CCTMXTiledMap::create(CCString::createWithFormat("alert_%s.tmx", CCUserDefault::sharedUserDefault()->getStringForKey(GAME_STYLE).c_str())->getCString());
    float x = (CCDirector::sharedDirector()->getWinSize().width - mainBg->getContentSize().width) / 2.0f;
    float y = (CCDirector::sharedDirector()->getWinSize().height - mainBg->getContentSize().height) / 2.0f;
    mainBg->setPosition(ccp(x, y));
    mainBg->setTag(kRPGDialogLayerBg);
    this->addChild(mainBg);
    
    CCLabelTTF *titleLab = CCLabelTTF::create(titleStr.c_str(), "Arial", 25, CCSizeMake(mainBg->getContentSize().width, 25), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    titleLab->setPosition(ccp(190, 135));
    titleLab->setColor(ccc3(255, 255, 255));
    mainBg->addChild(titleLab);
    
    CCMenu *menu = CCMenu::create();
    menu->setAnchorPoint(CCPointZero);
    menu->setPosition(CCPointZero);
    menu->setTag(kRPGDialogLayerBgMenu);
    mainBg->addChild(menu);
}
