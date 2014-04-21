//
//  RPGBattleMonsterSprite.cpp
//  OzgGameRPG
//
//  Created by ozg on 14-4-4.
//
//

#include "RPGBattleMonsterSprite.h"
#include "RPGBattleSceneLayer.h"

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
        this->m_data->m_status = kRPGDataStatusNormal;
        
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

void RPGBattleMonsterSprite::showHurtResults(cocos2d::CCObject *target, int HPResults, CCNode* player)
{
    addLab(this, kRPGBattleMonsterSpriteTagHurtResults, CCString::createWithFormat("%i", HPResults), 24, kCCTextAlignmentCenter, ccp(this->getContentSize().width / 2, this->getContentSize().height / 2));
    
    CCLabelTTF *hurtResults = (CCLabelTTF*)this->getChildByTag(kRPGBattleMonsterSpriteTagHurtResults);
    
    //增加HP的话则使用绿色
    if(HPResults > 0)
        hurtResults->setFontFillColor(ccc3(80, 255, 90));
    
    CCMoveBy *up = CCMoveBy::create(0.5, ccp(0, 30));
    CCMoveBy *down = CCMoveBy::create(1, ccp(0, -30));
    
    this->m_HPResults = HPResults;
    
    hurtResults->runAction(CCSequence::create(CCEaseExponentialOut::create(up), CCEaseBounceOut::create(down), CCCallFunc::create(this, callfunc_selector(RPGBattleMonsterSprite::showHurtResultsEnd)), CCCallFuncND::create(target, callfuncND_selector(RPGBattleSceneLayer::attackWithTargetHurtLabEnd), player), NULL));
    
}

//private
void RPGBattleMonsterSprite::showHurtResultsEnd()
{    
    this->m_data->m_HP += this->m_HPResults;
    
    if(this->m_data->m_HP < 0)
        this->m_data->m_HP = 0;
    
}
