//
//  RPGMapDialogLayer.cpp
//  OzgGameRPG
//
//  Created by 欧志刚 on 14-3-31.
//
//

#include "RPGMapDialogLayer.h"
#include "RPGMapSceneLayer.h"

RPGMapDialogLayer::RPGMapDialogLayer()
{

}

RPGMapDialogLayer::~RPGMapDialogLayer()
{
    this->m_npc->startAutoMove(); //继续执行自动移动
    this->m_npc->release();

//    CCLog("RPGMapDialogLayer 释放");
}

bool RPGMapDialogLayer::init(RPGMapNPCRoleSprite* npc)
{
    if(CCLayer::init())
    {
        this->m_npc = npc;
        this->m_npc->retain();
        
        CCTMXTiledMap *mainBg = CCTMXTiledMap::create("dialog_style1.tmx");
        float x = (CCDirector::sharedDirector()->getWinSize().width - mainBg->getContentSize().width) / 2.0f;
        float y = 40;
        mainBg->setPosition(ccp(x, y));
        this->addChild(mainBg);
        
        string text = "       ";
        text.append(npc->m_content->getCString());
        
        CCLabelTTF *contentLab = CCLabelTTF::create(text.c_str(), "Arial", 25, CCSizeMake(mainBg->getContentSize().width - 20.0f, mainBg->getContentSize().height - 40.0f), kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
        contentLab->setPosition(ccp(mainBg->getContentSize().width / 2.0f + 35.0f, 125));
        this->addChild(contentLab);
        
        CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
        
        return true;
    }
    
    return false;
}

RPGMapDialogLayer* RPGMapDialogLayer::create(RPGMapNPCRoleSprite* npc)
{
    RPGMapDialogLayer *layer = new RPGMapDialogLayer();
    if(layer && layer->init(npc))
    {
        layer->autorelease();
        
        return layer;
    }
    return NULL;
}

bool RPGMapDialogLayer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    
    return true;
}

void RPGMapDialogLayer::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{

}

void RPGMapDialogLayer::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    CCTMXTiledMap *bgMap = (CCTMXTiledMap*)this->getParent()->getChildByTag(kRPGMapSceneLayerTagBgMap);
    
    CCMenu *mainMenu = (CCMenu*)bgMap->getChildByTag(kRPGMapSceneLayerTagMainMenu);
    mainMenu->setVisible(true);
    
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    this->removeFromParentAndCleanup(true);
}
