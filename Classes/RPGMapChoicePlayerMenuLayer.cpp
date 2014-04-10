//
//  RPGMapChoicePlayerMenuLayer.cpp
//  OzgGameRPG
//
//  Created by ozg on 14-4-10.
//
//

#include "RPGMapChoicePlayerMenuLayer.h"

RPGMapChoicePlayerMenuLayer::RPGMapChoicePlayerMenuLayer()
{

}

RPGMapChoicePlayerMenuLayer::~RPGMapChoicePlayerMenuLayer()
{
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this->m_target, "onChoicePlayer");
    this->m_target->release();
    
    CCLog("RPGMapChoicePlayerMenuLayer 释放");
}

bool RPGMapChoicePlayerMenuLayer::init(CppSQLite3DB *db, CCObject* target, SEL_CallFuncO selector, float width, float height)
{
    if(CCLayerColor::initWithColor(ccc4(0, 0, 0, 200), width, height))
    {
        this->m_target = target;
        this->m_target->retain();
        
        this->m_db = db;
        
        CCTMXTiledMap *mainBg = CCTMXTiledMap::create("select_player_style1.tmx");
        mainBg->setPosition(ccp(270, 280));
        mainBg->setTag(kRPGMapChoicePlayerMenuLayerTagBg);
        this->addChild(mainBg);
        
        CCMenu *mainMenu = CCMenu::create();
        mainMenu->setTag(kRPGMapChoicePlayerMenuLayerTagMainMenu);
        mainMenu->setAnchorPoint(CCPointZero);
        mainMenu->setPosition(CCPointZero);
        this->addChild(mainMenu);
        
        this->setPosition(ccp((CCDirector::sharedDirector()->getWinSize().width - width) / 2, (CCDirector::sharedDirector()->getWinSize().height - height) / 2));
        
        //显示4个角色
        float playerX = 315;
        CppSQLite3Query query = this->m_db->execQuery(PLAYER_QUERY);
        while(!query.eof())
        {
            CCArray *frames = OzgCCUtility::createSpriteFrames(CCString::createWithFormat("%s_0.png", query.getStringField("tex_prefix"))->getCString(), 3, 4);
            
            CCSprite *playerSprite = CCSprite::createWithSpriteFrame((CCSpriteFrame*)frames->objectAtIndex(1));
            CCMenuItemSprite *player = (CCMenuItemSprite*)mainMenu->getChildByTag(kRPGMapChoicePlayerMenuLayerTagMainMenuPlayer + query.getIntField("id"));
            if(!player)
            {
                player = CCMenuItemSprite::create(playerSprite, playerSprite, this, menu_selector(RPGMapChoicePlayerMenuLayer::onMenu));
                player->cocos2d::CCNode::setPosition(ccp(playerX, 330));
                player->setScale(1.5);
                player->setTag(kRPGMapChoicePlayerMenuLayerTagMainMenuPlayer + query.getIntField("id"));
                mainMenu->addChild(player);
            }
            
            playerX += 120;
            query.nextRow();
        }
        query.finalize();
        
        CCNotificationCenter::sharedNotificationCenter()->addObserver(this->m_target, selector, "onChoicePlayer", NULL);
        
        return true;
    }
    return false;
}

RPGMapChoicePlayerMenuLayer* RPGMapChoicePlayerMenuLayer::create(CppSQLite3DB *db, CCObject* target, SEL_CallFuncO selector, float width, float height)
{
    RPGMapChoicePlayerMenuLayer *obj = new RPGMapChoicePlayerMenuLayer();
    if(obj && obj->init(db, target, selector, width, height))
    {
        obj->autorelease();
        return obj;
    }
    CC_SAFE_DELETE(obj);
    return NULL;
}

void RPGMapChoicePlayerMenuLayer::onMenu(cocos2d::CCObject *pObject)
{
    CCNotificationCenter::sharedNotificationCenter()->postNotification("onChoicePlayer", pObject);
}
