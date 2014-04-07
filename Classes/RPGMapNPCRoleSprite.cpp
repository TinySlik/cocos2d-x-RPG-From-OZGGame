//
//  RPGMapNPCRoleSprite.cpp
//  OzgGameRPG
//
//  Created by ozg on 14-1-17.
//
//

#include "RPGMapNPCRoleSprite.h"
#include "OzgCCUtility.h"
#include "GameCfg.h"

RPGMapNPCRoleSprite::~RPGMapNPCRoleSprite()
{
    this->m_content->release();
    
//    CCLog("RPGMapNPCRoleSprite 释放");
}

bool RPGMapNPCRoleSprite::init(CCString *roleTexture, CCString* headTexture, CCString* name, CCString* content, int defaultSpriteFrameIndex, bool autoMove)
{
    if(RPGMapRoleSprite::init(roleTexture, name, false, defaultSpriteFrameIndex))
    {
        this->m_autoMove = autoMove;
        
        this->m_content = content;
        this->m_content->retain();
        
        this->startAutoMove();
        
        this->m_type = kRPGMapRoleSpriteTypeNPC;
        
        return true;
    }
    return false;
}

//static
RPGMapNPCRoleSprite* RPGMapNPCRoleSprite::create(CCString *roleTexture, CCString* headTexture, CCString* name, CCString* content, int defaultSpriteFrameIndex, bool autoMove)
{
    RPGMapNPCRoleSprite *obj = new RPGMapNPCRoleSprite();
    if(obj && obj->init(roleTexture, headTexture, name, content, defaultSpriteFrameIndex, autoMove))
    {
        obj->autorelease();
        return obj;
    }
    CC_SAFE_DELETE(obj);
    return NULL;
}

//private
void RPGMapNPCRoleSprite::autoMove(float delay)
{
    //这里的逻辑跟RPGSingleModeMapSceneLayer::doMoving类似
    
    CCTMXTiledMap *bgMap = (CCTMXTiledMap*)this->getParent();
    CCTMXObjectGroup *obstaclesObjects = bgMap->objectGroupNamed("obstacles");
    
    bool doMoving;
    CCPoint point;
    
    //每一次重新将doMoving改为true，然后执行各个判断逻辑，直到doMoving不会改变为false为止
    
    do
    {
        doMoving = true;
        
        float val = CCRANDOM_0_1();
        if(val >= 0.0 && val <= 0.25)
        {
            this->startMoveUp();
            point = ccp(0, GAME_TMX_ROLE_HEIGHT);
        }
        else if(val > 0.25 && val <= 0.5)
        {
            this->startMoveDown();
            point = ccp(0, -GAME_TMX_ROLE_HEIGHT);
        }
        else if(val > 0.5 && val <= 0.75)
        {
            this->startMoveLeft();
            point = ccp(-GAME_TMX_ROLE_WIDTH, 0);
        }
        else
        {
            this->startMoveRight();
            point = ccp(GAME_TMX_ROLE_WIDTH, 0);
        }
        
        CCPoint targetPoint = ccpAdd(this->getPosition(), point);
        
        //地图上的障碍物
        for (int i = 0; i < obstaclesObjects->getObjects()->count(); i++)
        {
            CCDictionary *obstaclesObject = (CCDictionary*)obstaclesObjects->getObjects()->objectAtIndex(i);
            const CCString *x = obstaclesObject->valueForKey("x");
            const CCString *y = obstaclesObject->valueForKey("y");
            const CCString *width = obstaclesObject->valueForKey("width");
            const CCString *height = obstaclesObject->valueForKey("height");
            
            CCRect obstaclesRect = CCRectMake(stringToNumber<float>(x->getCString()), stringToNumber<float>(y->getCString()), stringToNumber<float>(width->getCString()), stringToNumber<float>(height->getCString()));
            
            if(obstaclesRect.containsPoint(targetPoint))
                doMoving = false;
            
        }
        
        //NPC或Player
        for (int i = 0; i < bgMap->getChildren()->count(); i++)
        {
            CCObject *item = bgMap->getChildren()->objectAtIndex(i);
            if(dynamic_cast<RPGMapRoleSprite*>(item) != NULL)
            {
                //如果为非自己的对象才会执行里面的判断
                if(item != this)
                {
                    if(((RPGMapRoleSprite*)item)->boundingBox().containsPoint(targetPoint))
                        doMoving = false;
                }
            }
        }
        
    }
    while (!doMoving);
    
    this->runAction(CCSequence::createWithTwoActions(CCMoveBy::create(0.5, point), CCCallFunc::create(this, callfunc_selector(RPGMapNPCRoleSprite::autoMoveEnd))));
        
}

void RPGMapNPCRoleSprite::stopAutoMove()
{
    
    this->unschedule(schedule_selector(RPGMapNPCRoleSprite::autoMove));
}

void RPGMapNPCRoleSprite::startAutoMove()
{
    if(this->m_autoMove)
    {
        float audoMoveTime = OzgCCUtility::randomFloat(1.0, 5.0);
        this->scheduleOnce(schedule_selector(RPGMapNPCRoleSprite::autoMove), audoMoveTime);
    }
}

//private
void RPGMapNPCRoleSprite::autoMoveEnd()
{
    this->stopMove();
    
    this->startAutoMove();
}
