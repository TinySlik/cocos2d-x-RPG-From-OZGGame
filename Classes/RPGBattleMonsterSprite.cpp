//
//  RPGBattleMonsterSprite.cpp
//  OzgGameRPG
//
//  Created by ozg on 14-4-4.
//
//

#include "RPGBattleMonsterSprite.h"

RPGBattleMonsterSprite::RPGBattleMonsterSprite()
{

}

RPGBattleMonsterSprite::~RPGBattleMonsterSprite()
{

    CCLog("RPGBattleMonsterSprite 释放");
}

bool RPGBattleMonsterSprite::initWithMonsterData(RPGMonster* data)
{
    if(CCSprite::initWithSpriteFrameName(data->m_tex.c_str()))
    {
        this->m_data = data;
        
        this->m_isSelected = false;
        
        return true;
    }
    return false;
}

RPGBattleMonsterSprite* RPGBattleMonsterSprite::createWithMonsterData(RPGMonster* data)
{
    RPGBattleMonsterSprite *obj = new RPGBattleMonsterSprite();
    if(obj && obj->initWithMonsterData(data))
    {
        obj->autorelease();
        return obj;
    }
    CC_SAFE_DELETE(obj);
    return NULL;
}

void RPGBattleMonsterSprite::selected(bool isSelected)
{
    this->m_isSelected = isSelected;
    
    if(this->m_isSelected)
    {
        CCSprite *handCursor = (CCSprite*)this->getChildByTag(kRPGBattleMonsterSpriteTagCursor);
        if(!handCursor)
        {
            handCursor = CCSprite::createWithSpriteFrameName("gui_hand.png");
            handCursor->setScale(0.3);
            handCursor->setTag(kRPGBattleMonsterSpriteTagCursor);
            handCursor->setPosition(ccp(this->getContentSize().width + 30, this->getContentSize().height / 2 - 10));
            this->addChild(handCursor);
        }
    }
    else
    {
        CCSprite *handCursor = (CCSprite*)this->getChildByTag(kRPGBattleMonsterSpriteTagCursor);
        if(handCursor)
            handCursor->removeFromParentAndCleanup(true);
    }
    
}
