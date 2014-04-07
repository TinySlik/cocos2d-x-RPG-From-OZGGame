
#include "OzgCCScrollBgNode.h"

#define SCROLL_SPEED -1.0f //滚动速度，负数为向左，正数为向右

OzgCCScrollBgNode::~OzgCCScrollBgNode()
{
    this->unscheduleUpdate();
    this->m_bgSprite1->removeFromParentAndCleanup(true);
    this->m_bgSprite2->removeFromParentAndCleanup(true);
    CCTextureCache::sharedTextureCache()->removeTextureForKey(this->m_bg.c_str());
    
}

bool OzgCCScrollBgNode::init(const char* bg)
{
    if(CCNode::init())
    {
        this->m_bg = string(bg);
        this->m_bgSprite1 = CCSprite::create(this->m_bg.c_str());
        this->m_bgSprite1->setAnchorPoint(CCPointZero);
        this->m_bgSprite1->setPosition(CCPointZero);
        this->addChild(this->m_bgSprite1);
        
        this->m_bgSprite2 = CCSprite::create(this->m_bg.c_str());
        this->m_bgSprite2->setAnchorPoint(CCPointZero);
        this->m_bgSprite2->setPosition(ccp(this->m_bgSprite1->getContentSize().width, 0));
        this->addChild(this->m_bgSprite2);
        
        this->scheduleUpdate();
        return true;
    }
    return false;
}

OzgCCScrollBgNode* OzgCCScrollBgNode::create(const char *bg)
{
    OzgCCScrollBgNode *obj = new OzgCCScrollBgNode();
    if(obj && obj->init(bg))
    {
        obj->autorelease();
        return obj;
    }
    
    CC_SAFE_DELETE(obj);
    return NULL;
}

void OzgCCScrollBgNode::update(float delta)
{
    this->m_bgSprite1->setPositionX(this->m_bgSprite1->getPositionX() + SCROLL_SPEED);
    this->m_bgSprite2->setPositionX(this->m_bgSprite2->getPositionX() + SCROLL_SPEED);
    
    if(this->m_bgSprite1->getPositionX() <= -this->m_bgSprite1->getContentSize().width)
        this->m_bgSprite1->setPosition(ccp(this->m_bgSprite1->getContentSize().width, 0));
    else if(this->m_bgSprite2->getPositionX() <= -this->m_bgSprite2->getContentSize().width)
        this->m_bgSprite2->setPosition(ccp(this->m_bgSprite2->getContentSize().width, 0));
    
}
