//
//  RPGMapRoleSprite.cpp
//  OzgGameRPG
//
//  Created by ozg on 14-3-18.
//
//

#include "RPGMapRoleSprite.h"
#include "OzgCCUtility.h"

#define MOVE_ACT 0.25 //行走动画的下一帧时间

RPGMapRoleSprite::~RPGMapRoleSprite()
{
    if(this->m_roleSpriteFrames) this->m_roleSpriteFrames->release();
    this->m_name->release();
    
    CCLog("RPGMapRoleSprite 释放");
}

bool RPGMapRoleSprite::init(CCString *roleTexture, CCString* name, bool isPlayer, int defaultSpriteFrameIndex)
{
    this->m_roleSpriteFrames = OzgCCUtility::createSpriteFrames(roleTexture->getCString(), 3, 4);
    this->m_roleSpriteFrames->retain();
    
    this->m_name = name;
    this->m_name->retain();
    
    this->m_isPlayer = isPlayer;
    
    if(CCSprite::initWithSpriteFrame((CCSpriteFrame*)this->m_roleSpriteFrames->objectAtIndex(defaultSpriteFrameIndex)))
    {
        CCLabelTTF *nameLab = CCLabelTTF::create(this->m_name->getCString(), "Arial", 15, CCSizeMake(this->getContentSize().width * 3.0, 20), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
        nameLab->setPosition(ccp(15, 45));
        
        if(this->m_isPlayer)
            nameLab->setColor(ccc3(255, 50, 40));
        else
            nameLab->setColor(ccc3(0, 0, 0));
        
        this->addChild(nameLab);
        
        this->m_direction = kRPGMapRoleSpriteDirectionNone;
        
        return true;
    }
    
    this->m_roleSpriteFrames->release();
    return false;
}

//static
RPGMapRoleSprite* RPGMapRoleSprite::create(CCString *roleTexture, CCString* name, bool isPlayer, int defaultSpriteFrameIndex)
{
    RPGMapRoleSprite *obj = new RPGMapRoleSprite();
    if(obj && obj->init(roleTexture, name, isPlayer, defaultSpriteFrameIndex))
    {
        obj->autorelease();
        return obj;
    }
    CC_SAFE_DELETE(obj);
    return NULL;
}

////private
void RPGMapRoleSprite::startMove(RPGMapRoleSpriteMoveActTag actTag)
{
    if(this->m_isMoving && this->m_currMoveActTag != actTag)
    {
        this->stopActionByTag(this->m_currMoveActTag);
        this->m_isMoving = false;
    }
    
    if(!this->m_isMoving)
    {
        CCArray *spriteFrames = CCArray::create();
        
        switch (actTag)
        {
            case kRPGMapRoleSpriteMoveActTagUp:
            {
                spriteFrames->addObject(this->m_roleSpriteFrames->objectAtIndex(9));
                spriteFrames->addObject(this->m_roleSpriteFrames->objectAtIndex(10));
                spriteFrames->addObject(this->m_roleSpriteFrames->objectAtIndex(11));
                spriteFrames->addObject(this->m_roleSpriteFrames->objectAtIndex(10));
                
                this->m_direction = kRPGMapRoleSpriteDirectionUp;
            }
                break;
            case kRPGMapRoleSpriteMoveActTagDown:
            {
                spriteFrames->addObject(this->m_roleSpriteFrames->objectAtIndex(0));
                spriteFrames->addObject(this->m_roleSpriteFrames->objectAtIndex(1));
                spriteFrames->addObject(this->m_roleSpriteFrames->objectAtIndex(2));
                spriteFrames->addObject(this->m_roleSpriteFrames->objectAtIndex(1));
                
                this->m_direction = kRPGMapRoleSpriteDirectionDown;
            }
                break;
            case kRPGMapRoleSpriteMoveActTagLeft:
            {
                spriteFrames->addObject(this->m_roleSpriteFrames->objectAtIndex(3));
                spriteFrames->addObject(this->m_roleSpriteFrames->objectAtIndex(4));
                spriteFrames->addObject(this->m_roleSpriteFrames->objectAtIndex(5));
                spriteFrames->addObject(this->m_roleSpriteFrames->objectAtIndex(4));
                
                this->m_direction = kRPGMapRoleSpriteDirectionLeft;
            }
                break;
            case kRPGMapRoleSpriteMoveActTagRight:
            {
                spriteFrames->addObject(this->m_roleSpriteFrames->objectAtIndex(6));
                spriteFrames->addObject(this->m_roleSpriteFrames->objectAtIndex(7));
                spriteFrames->addObject(this->m_roleSpriteFrames->objectAtIndex(8));
                spriteFrames->addObject(this->m_roleSpriteFrames->objectAtIndex(7));
                
                this->m_direction = kRPGMapRoleSpriteDirectionRight;
            }
                break;
            default:
                break;
        }
        
        CCAnimation *animation = CCAnimation::createWithSpriteFrames(spriteFrames, MOVE_ACT);
        CCRepeatForever *animate = CCRepeatForever::create(CCAnimate::create(animation));
        animate->setTag(actTag);
        this->m_currMoveActTag = actTag;
        
        this->runAction(animate);
        
        this->m_isMoving = true;
    }
}

void RPGMapRoleSprite::startMoveUp()
{
    this->startMove(kRPGMapRoleSpriteMoveActTagUp);
    
}

void RPGMapRoleSprite::startMoveDown()
{
    this->startMove(kRPGMapRoleSpriteMoveActTagDown);
    
}

void RPGMapRoleSprite::startMoveLeft()
{
    this->startMove(kRPGMapRoleSpriteMoveActTagLeft);
    
}

void RPGMapRoleSprite::startMoveRight()
{
    this->startMove(kRPGMapRoleSpriteMoveActTagRight);
    
}

void RPGMapRoleSprite::stopMove()
{
    if(!this->m_isMoving)
        return;
    
    switch (this->m_currMoveActTag)
    {
        case kRPGMapRoleSpriteMoveActTagUp:
        {
            this->stopActionByTag(kRPGMapRoleSpriteMoveActTagUp);
            this->setDisplayFrame((CCSpriteFrame*)this->m_roleSpriteFrames->objectAtIndex(10));
        }
            break;
        case kRPGMapRoleSpriteMoveActTagDown:
        {
            this->stopActionByTag(kRPGMapRoleSpriteMoveActTagDown);
            this->setDisplayFrame((CCSpriteFrame*)this->m_roleSpriteFrames->objectAtIndex(1));
        }
            break;
        case kRPGMapRoleSpriteMoveActTagLeft:
        {
            this->stopActionByTag(kRPGMapRoleSpriteMoveActTagLeft);
            this->setDisplayFrame((CCSpriteFrame*)this->m_roleSpriteFrames->objectAtIndex(4));
        }
            break;
        case kRPGMapRoleSpriteMoveActTagRight:
        {
            this->stopActionByTag(kRPGMapRoleSpriteMoveActTagRight);
            this->setDisplayFrame((CCSpriteFrame*)this->m_roleSpriteFrames->objectAtIndex(7));
        }
            break;
        default:
            break;
    }
    
    this->m_isMoving = false;
}

void RPGMapRoleSprite::setDirection(RPGMapRoleSpriteDirection direction)
{
    this->m_direction = direction;
    
    switch (this->m_direction)
    {
        case kRPGMapRoleSpriteDirectionUp:
            this->setDisplayFrame((CCSpriteFrame*)this->m_roleSpriteFrames->objectAtIndex(10));
            break;
        case kRPGMapRoleSpriteDirectionDown:
            this->setDisplayFrame((CCSpriteFrame*)this->m_roleSpriteFrames->objectAtIndex(1));
            break;
        case kRPGMapRoleSpriteDirectionLeft:
            this->setDisplayFrame((CCSpriteFrame*)this->m_roleSpriteFrames->objectAtIndex(4));
            break;
        case kRPGMapRoleSpriteDirectionRight:
            this->setDisplayFrame((CCSpriteFrame*)this->m_roleSpriteFrames->objectAtIndex(7));
            break;
        default:
            break;
    }
    
}
