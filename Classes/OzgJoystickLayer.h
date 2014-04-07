
#ifndef __ozgcc2dxutils__OzgJoystickLayer__
#define __ozgcc2dxutils__OzgJoystickLayer__

/*
 demo
 
 释放时需要调用inactive()
 
 bool HelloWorld::init()
 {
 if ( !CCLayer::init() )
 {
 return false;
 }
 
 CCSprite *spJoystick = CCSprite::createWithSpriteFrameName("joystick01.png");
 CCSprite *joystickBg = CCSprite::createWithSpriteFrameName("joystick02.png");
 //        OzgJoystickLayer *joystick = OzgJoystickLayer::layerActivityJoystick(50, spJoystick, joystickBg, false);
 OzgJoystickLayer *joystick = OzgJoystickLayer::layerStaticJoystick(ccp(200, 100), 50, spJoystick, joystickBg, false);
 joystick->setTag(1);
 this->addChild(joystick);
 
 this->scheduleUpdate();
 
 return true;
 }
 
 void HelloWorld::update(float dt)
 {
 OzgJoystickLayer *joystick = (OzgJoystickLayer*)this->getChildByTag(1);
 if(joystick->m_trigger)
 CCLog("%f %f %f %f %f", joystick->m_centerPoint.x, joystick->m_centerPoint.y, joystick->m_currentPoint.x, joystick->m_currentPoint.y, dt);
 
 }
 
 */

#include "cocos2d.h"

USING_NS_CC;

class OzgJoystickLayer : public CCLayer
{
    
public:
    
    CCPoint m_centerPoint; //摇杆中心
    CCPoint m_currentPoint; //摇杆当前位置
    bool m_trigger; //是否正在触发摇杆
    bool m_active; //是否激活摇杆
    float m_radius; //摇杆半径
    CCSprite* m_jsSprite;
    
    bool m_activityJoystick; //是否为活动摇杆
    
    bool m_isFollowRole; //是否跟随用户点击
    
    CCObject* m_target;
    SEL_CallFunc m_endedSelector;
    
    OzgJoystickLayer();
    virtual ~OzgJoystickLayer();
    
    //初始化 aPoint是摇杆中心 aRadius是摇杆半径 aJsSprite是摇杆控制点 aJsBg是摇杆背景
    static OzgJoystickLayer* layerActivityJoystick(float aRadius, CCSprite* aJsSprite, CCSprite* aJsBg, bool aIsFollowRole);
    static OzgJoystickLayer* layerStaticJoystick(CCPoint aPoint, float aRadius, CCSprite* aJsSprite, CCSprite* aJsBg, bool aIsFollowRole);
    
    OzgJoystickLayer* initWithCenter(CCPoint aPoint, float aRadius, CCSprite* aJsSprite, CCSprite* aJsBg, bool aIsActivityJoystick, bool aIsFollowRole);
    
    //启动摇杆
    void active();
    
    //解除摇杆
    void inactive();    
    
    CCPoint getDirection();
    float getVelocity();
    void updatePos(float dt);
    
    CREATE_FUNC(OzgJoystickLayer);
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
};

#endif /* defined(__ozgcc2dxutils__OzgJoystickLayer__) */
