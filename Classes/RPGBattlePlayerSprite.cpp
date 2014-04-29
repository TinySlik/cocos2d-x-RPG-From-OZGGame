//
//  RPGBattlePlayerSprite.cpp
//  OzgGameRPG
//
//  Created by ozg on 14-4-3.
//
//

#include "RPGBattlePlayerSprite.h"
#include "RPGBattleSceneLayer.h"
#include "OzgCCUtility.h"

RPGBattlePlayerSprite::RPGBattlePlayerSprite()
{

}

RPGBattlePlayerSprite::~RPGBattlePlayerSprite()
{
    this->m_spriteFramesNormal->release();
    this->m_spriteFramesHurt->release();
    this->m_spriteFramesDying->release();
    this->m_spriteFramesWin->release();
    this->m_spriteFramesAvoid->release();
    this->m_spriteFramesDeath->release();
    this->m_spriteFramesAttack->release();
    this->m_spriteFramesUseItem->release();
    this->m_spriteFramesSpecialAttack->release();
    this->m_spriteFramesChanting->release();
    
    CCLog("RPGBattlePlayerSprite 释放");
}

bool RPGBattlePlayerSprite::initWithPlayerData(RPGPlayer* data)
{
    CCArray *spriteFrames1 = OzgCCUtility::createSpriteFrames(CCString::createWithFormat("%s_1.png", data->m_texPrefix.c_str())->getCString(), 3, 4);
    CCArray *spriteFrames2 = OzgCCUtility::createSpriteFrames(CCString::createWithFormat("%s_2.png", data->m_texPrefix.c_str())->getCString(), 3, 4);
    CCArray *spriteFrames3 = OzgCCUtility::createSpriteFrames(CCString::createWithFormat("%s_3.png", data->m_texPrefix.c_str())->getCString(), 3, 4);
        
    this->m_spriteFramesNormal = new CCArray();
    this->m_spriteFramesNormal->init();
    this->m_spriteFramesNormal->addObject(spriteFrames1->objectAtIndex(0));
    this->m_spriteFramesNormal->addObject(spriteFrames1->objectAtIndex(1));
    this->m_spriteFramesNormal->addObject(spriteFrames1->objectAtIndex(2));
    this->m_spriteFramesNormal->addObject(spriteFrames1->objectAtIndex(1));
    
    this->m_spriteFramesHurt = new CCArray();
    this->m_spriteFramesHurt->init();
    this->m_spriteFramesHurt->addObject(spriteFrames1->objectAtIndex(3));
    this->m_spriteFramesHurt->addObject(spriteFrames1->objectAtIndex(4));
    this->m_spriteFramesHurt->addObject(spriteFrames1->objectAtIndex(5));
    
    this->m_spriteFramesDying = new CCArray();
    this->m_spriteFramesDying->init();
    this->m_spriteFramesDying->addObject(spriteFrames1->objectAtIndex(6));
    this->m_spriteFramesDying->addObject(spriteFrames1->objectAtIndex(7));
    this->m_spriteFramesDying->addObject(spriteFrames1->objectAtIndex(8));
    this->m_spriteFramesDying->addObject(spriteFrames1->objectAtIndex(7));
    
    this->m_spriteFramesWin = new CCArray();
    this->m_spriteFramesWin->init();
    this->m_spriteFramesWin->addObject(spriteFrames2->objectAtIndex(0));
    this->m_spriteFramesWin->addObject(spriteFrames2->objectAtIndex(1));
    this->m_spriteFramesWin->addObject(spriteFrames2->objectAtIndex(2));
    this->m_spriteFramesWin->addObject(spriteFrames2->objectAtIndex(1));
    
    this->m_spriteFramesAvoid = new CCArray();
    this->m_spriteFramesAvoid->init();
    this->m_spriteFramesAvoid->addObject(spriteFrames2->objectAtIndex(3));
    this->m_spriteFramesAvoid->addObject(spriteFrames2->objectAtIndex(4));
    this->m_spriteFramesAvoid->addObject(spriteFrames2->objectAtIndex(5));
    
    this->m_spriteFramesDeath = new CCArray();
    this->m_spriteFramesDeath->init();
    this->m_spriteFramesDeath->addObject(spriteFrames2->objectAtIndex(9));
    this->m_spriteFramesDeath->addObject(spriteFrames2->objectAtIndex(10));
    this->m_spriteFramesDeath->addObject(spriteFrames2->objectAtIndex(11));
    this->m_spriteFramesDeath->addObject(spriteFrames2->objectAtIndex(10));
    
    this->m_spriteFramesAttack = new CCArray();
    this->m_spriteFramesAttack->init();
    this->m_spriteFramesAttack->addObject(spriteFrames3->objectAtIndex(0));
    this->m_spriteFramesAttack->addObject(spriteFrames3->objectAtIndex(1));
    this->m_spriteFramesAttack->addObject(spriteFrames3->objectAtIndex(2));
    
    this->m_spriteFramesUseItem = new CCArray();
    this->m_spriteFramesUseItem->init();
    this->m_spriteFramesUseItem->addObject(spriteFrames3->objectAtIndex(3));
    this->m_spriteFramesUseItem->addObject(spriteFrames3->objectAtIndex(4));
    this->m_spriteFramesUseItem->addObject(spriteFrames3->objectAtIndex(5));
    
    this->m_spriteFramesSpecialAttack = new CCArray();
    this->m_spriteFramesSpecialAttack->init();
    this->m_spriteFramesSpecialAttack->addObject(spriteFrames3->objectAtIndex(6));
    this->m_spriteFramesSpecialAttack->addObject(spriteFrames3->objectAtIndex(7));
    this->m_spriteFramesSpecialAttack->addObject(spriteFrames3->objectAtIndex(8));
    
    this->m_spriteFramesChanting = new CCArray();
    this->m_spriteFramesChanting->init();
    this->m_spriteFramesChanting->addObject(spriteFrames3->objectAtIndex(9));
    this->m_spriteFramesChanting->addObject(spriteFrames3->objectAtIndex(10));
    this->m_spriteFramesChanting->addObject(spriteFrames3->objectAtIndex(11));
    this->m_spriteFramesChanting->addObject(spriteFrames3->objectAtIndex(10));
    
    if(CCSprite::initWithSpriteFrame((CCSpriteFrame*)this->m_spriteFramesNormal->objectAtIndex(0)))
    {
        this->m_data = data;
        this->setScale(1.75);
        
        this->m_isSelected = false;
        
        if(this->m_data->m_HP > 0)
        {
            this->m_data->m_status = kRPGDataStatusNormal;
            this->animNormal();
        }
        else
        {
            this->m_data->m_status = kRPGDataStatusDeath;
            this->animDeath();
        }
        
        return true;
    }
    return false;
}

RPGBattlePlayerSprite* RPGBattlePlayerSprite::createWithPlayerData(RPGPlayer* data)
{
    RPGBattlePlayerSprite *obj = new RPGBattlePlayerSprite();
    if(obj && obj->initWithPlayerData(data))
    {
        obj->autorelease();
        return obj;
    }
    CC_SAFE_DELETE(obj);
    return NULL;
}

void RPGBattlePlayerSprite::selected(bool isSelected)
{
    this->m_isSelected = isSelected;
    
    if(this->m_isSelected)
    {
        CCSprite *handCursor = (CCSprite*)this->getChildByTag(kRPGBattlePlayerSpriteTagCursor);
        if(!handCursor)
        {
            handCursor = CCSprite::createWithSpriteFrameName("gui_hand.png");
            handCursor->setScale(0.15);
            handCursor->setTag(kRPGBattlePlayerSpriteTagCursor);
            handCursor->setFlipX(true);
            handCursor->setPosition(ccp(-12, 10));
            this->addChild(handCursor);
        }
    }
    else
    {
        CCSprite *handCursor = (CCSprite*)this->getChildByTag(kRPGBattlePlayerSpriteTagCursor);
        if(handCursor)
            handCursor->removeFromParentAndCleanup(true);
    }
    
}

void RPGBattlePlayerSprite::animNormal()
{
    this->stopAllActions();
    
    if(this->getChildByTag(kRPGBattlePlayerSpriteTagArm))
        this->removeChildByTag(kRPGBattlePlayerSpriteTagArm, true);
    
    //战斗不能
    if(this->m_data->m_HP <= 0)
        this->m_data->m_status = kRPGDataStatusDeath;
    
    //临死
    else if(this->m_data->m_HP <= 9)
        this->m_data->m_status = kRPGDataStatusDying;
    
    if(this->m_data->m_status == kRPGDataStatusDeath)
    {
        this->animDeath();
        return;
    }
    else if(this->m_data->m_status == kRPGDataStatusDying)
    {
        this->animDying();
        return;
    }
    
    CCAnimation *animation = CCAnimation::createWithSpriteFrames(this->m_spriteFramesNormal, 0.2);
    CCRepeatForever *animate = CCRepeatForever::create(CCAnimate::create(animation));
    this->runAction(animate);
}

void RPGBattlePlayerSprite::animDying()
{
    this->stopAllActions();
    
    if(this->getChildByTag(kRPGBattlePlayerSpriteTagArm))
        this->removeChildByTag(kRPGBattlePlayerSpriteTagArm, true);
    
    CCAnimation *animation = CCAnimation::createWithSpriteFrames(this->m_spriteFramesDying, 0.2);
    CCRepeatForever *animate = CCRepeatForever::create(CCAnimate::create(animation));
    this->runAction(animate);
}

void RPGBattlePlayerSprite::animDeath()
{
    this->stopAllActions();
    
    if(this->getChildByTag(kRPGBattlePlayerSpriteTagArm))
        this->removeChildByTag(kRPGBattlePlayerSpriteTagArm, true);
    
    CCAnimation *animation = CCAnimation::createWithSpriteFrames(this->m_spriteFramesDeath, 0.2);
    CCRepeatForever *animate = CCRepeatForever::create(CCAnimate::create(animation));
    this->runAction(animate);
}

void RPGBattlePlayerSprite::animWin()
{
    this->stopAllActions();
    
    if(this->getChildByTag(kRPGBattlePlayerSpriteTagArm))
        this->removeChildByTag(kRPGBattlePlayerSpriteTagArm, true);
    
    CCAnimation *animation = CCAnimation::createWithSpriteFrames(this->m_spriteFramesWin, 0.2);
    CCRepeatForever *animate = CCRepeatForever::create(CCAnimate::create(animation));
    this->runAction(animate);
}

void RPGBattlePlayerSprite::animHurt()
{
    this->stopAllActions();
    
    if(this->getChildByTag(kRPGBattlePlayerSpriteTagArm))
        this->removeChildByTag(kRPGBattlePlayerSpriteTagArm, true);
    
    CCAnimation *animation = CCAnimation::createWithSpriteFrames(this->m_spriteFramesHurt, 0.2);
    
    CCSequence *animate = CCSequence::create(CCAnimate::create(animation), CCDelayTime::create(1.5), CCCallFunc::create(this, callfunc_selector(RPGBattlePlayerSprite::animNormal)), NULL);    
    this->runAction(animate);
}

void RPGBattlePlayerSprite::animAttack(CCObject* target, cocos2d::CCObject *targetObjData)
{
    this->stopAllActions();
        
    SimpleAudioEngine::sharedEngine()->playEffect("audio_battle_attack1.wav");
    
    CCAnimation *animation = CCAnimation::createWithSpriteFrames(this->m_spriteFramesAttack, 0.15);
    
    //挥动武器的动作
    CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo(CCString::createWithFormat("attack%i.ExportJson", this->m_data->m_dataId)->getCString());
	CCArmature* arm = CCArmature::create(CCString::createWithFormat("attack%i", this->m_data->m_dataId)->getCString());
	arm->getAnimation()->play("main_anim");
	arm->setPosition(CCPointZero);
    arm->setTag(kRPGBattlePlayerSpriteTagArm);
    arm->setScale(0.75);
	this->addChild(arm);
    
    //对应好arm位置
    switch (this->m_data->m_dataId)
    {
        case 2:
            arm->setPosition(ccp(arm->getPosition().x - 5, arm->getPosition().y));
            break;
        case 3:
            arm->setPosition(ccp(arm->getPosition().x - 5, arm->getPosition().y));
            break;
        case 4:
            
            break;
        default:
            arm->setPosition(ccp(arm->getPosition().x - 6, arm->getPosition().y + 5));
            break;
    }
    
    this->runAction(CCSequence::createWithTwoActions(CCAnimate::create(animation), CCCallFuncND::create(target, callfuncND_selector(RPGBattleSceneLayer::attackResults), (void*)targetObjData)));
}

void RPGBattlePlayerSprite::animSkill(cocos2d::CCObject *target, cocos2d::CCObject *targetObjData)
{
    this->stopAllActions();
    
    if(this->getChildByTag(kRPGBattlePlayerSpriteTagArm))
        this->removeChildByTag(kRPGBattlePlayerSpriteTagArm, true);
    
    CCAnimation *animation = CCAnimation::createWithSpriteFrames(this->m_spriteFramesChanting, 0.2);
    
    CCSequence *animate = CCSequence::create(CCAnimate::create(animation), CCAnimate::create(animation), CCDelayTime::create(0.25), NULL);
    this->runAction(CCSequence::createWithTwoActions(animate, CCCallFuncND::create(target, callfuncND_selector(RPGBattleSceneLayer::skillResults), (void*)targetObjData)));
}

void RPGBattlePlayerSprite::animUseItem(cocos2d::CCObject *target, cocos2d::CCObject *targetObjData)
{
    this->stopAllActions();
    
    if(this->getChildByTag(kRPGBattlePlayerSpriteTagArm))
        this->removeChildByTag(kRPGBattlePlayerSpriteTagArm, true);
    
    CCAnimation *animation = CCAnimation::createWithSpriteFrames(this->m_spriteFramesUseItem, 0.2);
    
    CCSequence *animate = CCSequence::create(CCAnimate::create(animation), CCDelayTime::create(0.5), CCCallFunc::create(this, callfunc_selector(RPGBattlePlayerSprite::animNormal)), NULL);
    this->runAction(CCSequence::createWithTwoActions(animate, CCCallFuncND::create(target, callfuncND_selector(RPGBattleSceneLayer::useItemResults), (void*)targetObjData)));
}

void RPGBattlePlayerSprite::showEffectResults(cocos2d::CCObject *target, int results, CCNode* srcAttackObj)
{
    //srcAttackObj为发起攻击的Sprite对象
    
    addLab(this, kRPGBattlePlayerSpriteTagEffectResults, CCString::createWithFormat("%i", results), 24, kCCTextAlignmentCenter, ccp(this->getContentSize().width / 2, this->getContentSize().height / 2));
    
    CCLabelTTF *effectResults = (CCLabelTTF*)this->getChildByTag(kRPGBattlePlayerSpriteTagEffectResults);
    
    //增加HP的话则使用绿色
    if(results > 0)
        effectResults->setFontFillColor(ccc3(80, 255, 90));
    
    CCMoveBy *up = CCMoveBy::create(0.5, ccp(0, 30));
    CCMoveBy *down = CCMoveBy::create(1, ccp(0, -30));
    
    this->m_results = results;
    
    effectResults->runAction(CCSequence::create(CCEaseExponentialOut::create(up), CCEaseBounceOut::create(down), CCDelayTime::create(0.5), CCCallFunc::create(this, callfunc_selector(RPGBattlePlayerSprite::showEffectResultsEnd)), CCCallFuncND::create(target, callfuncND_selector(RPGBattleSceneLayer::actionWithTargetEffectLabEnd), srcAttackObj), NULL));
    
}

//private
void RPGBattlePlayerSprite::showEffectResultsEnd()
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
