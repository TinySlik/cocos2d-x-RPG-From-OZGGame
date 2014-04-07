
#include "OzgJoystickLayer.h"

#define STICK_BG_TAG 99999

OzgJoystickLayer::OzgJoystickLayer()
{

}

OzgJoystickLayer::~OzgJoystickLayer()
{

//    CCLog("OzgJoystickLayer 释放");
}

OzgJoystickLayer* OzgJoystickLayer::layerActivityJoystick(float aRadius, cocos2d::CCSprite *aJsSprite, cocos2d::CCSprite *aJsBg, bool aIsFollowRole)
{
    OzgJoystickLayer *jstick = new OzgJoystickLayer();
    if(jstick->initWithCenter(CCPointZero, aRadius, aJsSprite, aJsBg, true, aIsFollowRole))
    {
        jstick->autorelease();
        return jstick;
    }
    CC_SAFE_DELETE(jstick);
    return NULL;
}

OzgJoystickLayer* OzgJoystickLayer::layerStaticJoystick(CCPoint aPoint, float aRadius, CCSprite* aJsSprite, CCSprite* aJsBg, bool aIsFollowRole)
{
    OzgJoystickLayer *jstick = new OzgJoystickLayer();
    if(jstick->initWithCenter(aPoint, aRadius, aJsSprite, aJsBg, false, aIsFollowRole))
    {
        jstick->autorelease();
        return jstick;
    }
    CC_SAFE_DELETE(jstick);
    return NULL;
}

OzgJoystickLayer* OzgJoystickLayer::initWithCenter(CCPoint aPoint, float aRadius, CCSprite* aJsSprite, CCSprite* aJsBg, bool aIsActivityJoystick, bool aIsFollowRole)
{
    this->m_trigger = false; //没有触发摇杆
    this->m_isFollowRole = aIsFollowRole;
    this->m_active = false;
    this->m_radius = aRadius;
    if(!aIsFollowRole)
        this->m_centerPoint = aPoint;
    else
        this->m_centerPoint = ccp(0,0);
    
    this->m_activityJoystick = aIsActivityJoystick;
    
    this->m_currentPoint = this->m_centerPoint;
    this->m_jsSprite = aJsSprite;
    this->m_jsSprite->setPosition(this->m_centerPoint);
    aJsBg->setPosition(this->m_centerPoint);
    aJsBg->setTag(STICK_BG_TAG);
    this->addChild(aJsBg);
    this->addChild(this->m_jsSprite);
    if(this->m_isFollowRole)
        this->setVisible(false);
    
    //活动摇杆
    if(this->m_activityJoystick)
    {
        this->m_jsSprite->setVisible(false);
        aJsBg->setVisible(false);
    }
    
    this->active(); //激活摇杆
    return this;
}

//启动摇杆
void OzgJoystickLayer::active()
{
    if (!this->m_active)
    {
        this->m_active = true;
        this->schedule(schedule_selector(OzgJoystickLayer::updatePos)); //添加刷新函数
        CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 999, false); //添加触摸委托
    }
    //    else
    //    {
    //
    //    }
}

//解除摇杆
void OzgJoystickLayer::inactive()
{
    if (this->m_active)
    {
        this->m_active = false;
        this->unschedule(schedule_selector(OzgJoystickLayer::updatePos)); //删除刷新
        CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this); //删除委托
    }
    //    else
    //    {
    //        
    //    }
}

//摇杆方位
CCPoint OzgJoystickLayer::getDirection()
{
    return ccpNormalize(ccpSub(this->m_centerPoint, this->m_currentPoint));
}

//摇杆力度
float OzgJoystickLayer::getVelocity()
{
    return ccpDistance(this->m_centerPoint, this->m_currentPoint);
}

void OzgJoystickLayer::updatePos(float dt)
{
    this->m_jsSprite->setPosition(ccpAdd(this->m_jsSprite->getPosition(), ccpMult(ccpSub(this->m_currentPoint, this->m_jsSprite->getPosition()), 0.5)));
}

bool OzgJoystickLayer::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    if (!this->m_active)
        return false;
    
    this->setVisible(true);
    CCPoint touchPoint = touch->getLocation();
    
    //活动摇杆
    if(this->m_activityJoystick)
    {
        this->m_centerPoint = touchPoint;
        
        this->m_jsSprite->setVisible(true);
        this->m_jsSprite->setPosition(this->m_centerPoint);
        
        CCSprite* aJsBg = (CCSprite*)this->getChildByTag(STICK_BG_TAG);
        aJsBg->setPosition(this->m_centerPoint);
        aJsBg->setVisible(true);
    }
    
    if(!this->m_isFollowRole)
    {
        if (ccpDistance(touchPoint, this->m_centerPoint) > this->m_radius)
            return false;
        
    }
    
    this->m_trigger = true; //正在触发摇杆
    
    this->m_currentPoint = touchPoint;
    
    if(this->m_isFollowRole)
    {
        this->m_centerPoint = this->m_currentPoint;
        this->m_jsSprite->setPosition(this->m_currentPoint);
        
        this->getChildByTag(STICK_BG_TAG)->setPosition(this->m_currentPoint);
    }
    return true;
}

void OzgJoystickLayer::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
    CCPoint touchPoint = touch->getLocation();
    if (ccpDistance(touchPoint, this->m_centerPoint) > this->m_radius)
        this->m_currentPoint = ccpAdd(this->m_centerPoint, ccpMult(ccpNormalize(ccpSub(touchPoint, this->m_centerPoint)), this->m_radius));
    else
        this->m_currentPoint = touchPoint;
    
}

void OzgJoystickLayer::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
    this->m_currentPoint = this->m_centerPoint;
    if(this->m_isFollowRole)
        this->setVisible(false);
    
    this->m_trigger = false; //松开时设置为没有触发摇杆
    
    //活动摇杆
    if(this->m_activityJoystick)
    {
        this->m_jsSprite->setVisible(false);
        
        CCSprite* aJsBg = (CCSprite*)this->getChildByTag(STICK_BG_TAG);
        aJsBg->setPosition(this->m_centerPoint);
        aJsBg->setVisible(false);
    }
    
    if(this->m_target)
    {
        CCCallFunc *endedFunc = CCCallFunc::create(this->m_target, m_endedSelector);
        this->runAction(endedFunc);
    }
}
