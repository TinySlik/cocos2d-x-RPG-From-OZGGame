//
//  RPGMapDialogLayer.h
//  OzgGameRPG
//
//  Created by 欧志刚 on 14-3-31.
//
//

#ifndef __OzgGameRPG__RPGMapDialogLayer__
#define __OzgGameRPG__RPGMapDialogLayer__

#include "cocos2d.h"
#include "RPGMapNPCRoleSprite.h"

USING_NS_CC;
using namespace std;

class RPGMapDialogLayer : public CCLayer
{
    
private:
    RPGMapNPCRoleSprite* m_npc;
    
public:
    
    RPGMapDialogLayer();
    virtual ~RPGMapDialogLayer();
    bool init(RPGMapNPCRoleSprite* npc);
    static RPGMapDialogLayer* create(RPGMapNPCRoleSprite* npc);
    
    bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
};

#endif /* defined(__OzgGameRPG__RPGMapDialogLayer__) */
