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

void RPGBattleMonsterSprite::showEffectResults(cocos2d::CCObject *target, int results, CCNode* player)
{
    addLab(this, kRPGBattleMonsterSpriteTagEffectResults, CCString::createWithFormat("%i", results), 24, kCCTextAlignmentCenter, ccp(this->getContentSize().width / 2, this->getContentSize().height / 2));
    
    CCLabelTTF *effectResults = (CCLabelTTF*)this->getChildByTag(kRPGBattleMonsterSpriteTagEffectResults);
    
    //增加HP的话则使用绿色
    if(results > 0)
        effectResults->setFontFillColor(ccc3(80, 255, 90));
    
    CCMoveBy *up = CCMoveBy::create(0.5, ccp(0, 30));
    CCMoveBy *down = CCMoveBy::create(1, ccp(0, -30));
    
    this->m_results = results;
    
    effectResults->runAction(CCSequence::create(CCEaseExponentialOut::create(up), CCEaseBounceOut::create(down), CCDelayTime::create(0.5), CCCallFunc::create(this, callfunc_selector(RPGBattleMonsterSprite::showEffectResultsEnd)), CCCallFuncND::create(target, callfuncND_selector(RPGBattleSceneLayer::actionWithTargetEffectLabEnd), player), NULL));
    
}

void RPGBattleMonsterSprite::animAttack(CCObject* target, CCObject *targetObjData)
{
    this->stopAllActions();
    
    SimpleAudioEngine::sharedEngine()->playEffect("audio_battle_attack1.wav");
    
    CCActionInterval *tint = CCTintBy::create(0.3, -255, -255, -255);
    CCActionInterval *tintBack = tint->reverse();
    
    this->runAction(CCSequence::createWithTwoActions(CCSequence::createWithTwoActions(tint, tintBack), CCCallFuncND::create(target, callfuncND_selector(RPGBattleSceneLayer::attackResults), (void*)targetObjData)));
    
}

//private
void RPGBattleMonsterSprite::showEffectResultsEnd()
{    
    if(strcmp(CCUserDefault::sharedUserDefault()->getStringForKey(GAME_BATTLE_FIELD, "").c_str(), "mp") == 0)
    {
        this->m_data->m_MP += this->m_results;
        
        if(this->m_data->m_MP < 0)
            this->m_data->m_MP = 0;
        else if(this->m_data->m_MP > this->m_data->m_maxMP)
            this->m_data->m_MP = this->m_data->m_maxMP;
    }
    else
    {
        this->m_data->m_HP += this->m_results;
        
        if(this->m_data->m_HP < 0)
            this->m_data->m_HP = 0;
        else if(this->m_data->m_HP > this->m_data->m_maxHP)
            this->m_data->m_HP = this->m_data->m_maxHP;
    }
    
    CCUserDefault::sharedUserDefault()->setStringForKey(GAME_BATTLE_FIELD, "");
    
}
