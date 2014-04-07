//
//  RPGMapRoleSprite.h
//  OzgGameRPG
//
//  Created by ozg on 14-3-18.
//
//

#ifndef __OzgGameRPG__RPGMapRoleSprite__
#define __OzgGameRPG__RPGMapRoleSprite__

#include "cocos2d.h"

USING_NS_CC;
using namespace std;

enum RPGMapRoleSpriteMoveActTag
{
    kRPGMapRoleSpriteMoveActTagUp = 1,
    kRPGMapRoleSpriteMoveActTagDown = 2,
    kRPGMapRoleSpriteMoveActTagLeft = 3,
    kRPGMapRoleSpriteMoveActTagRight = 4
};

enum RPGMapRoleSpriteDirection
{
    kRPGMapRoleSpriteDirectionNone = 0,
    kRPGMapRoleSpriteDirectionUp = 1,
    kRPGMapRoleSpriteDirectionDown = 2,
    kRPGMapRoleSpriteDirectionLeft = 3,
    kRPGMapRoleSpriteDirectionRight = 4
};

enum RPGMapRoleSpriteType
{
    kRPGMapRoleSpriteTypePlayer = 1,
    kRPGMapRoleSpriteTypeNPC = 2
};

class RPGMapRoleSprite : public CCSprite
{
        
private:
    
    RPGMapRoleSpriteMoveActTag m_currMoveActTag;
    CCArray* m_roleSpriteFrames;
    bool m_isMoving;
    CCString* m_name;
    
    void startMove(RPGMapRoleSpriteMoveActTag actTag);
    
public:
    
    RPGMapRoleSpriteDirection m_direction; //该对象向着哪个方向
    
    RPGMapRoleSpriteType m_type;
    bool m_isPlayer;
    
    virtual ~RPGMapRoleSprite();
    bool init(CCString *roleTexture, CCString* name, bool isPlayer, int defaultSpriteFrameIndex);
    static RPGMapRoleSprite* create(CCString *roleTexture, CCString* name, bool isPlayer, int defaultSpriteFrameIndex);
    
    void startMoveUp();
    void startMoveDown();
    void startMoveLeft();
    void startMoveRight();
    
    void stopMove();
    
    void setDirection(RPGMapRoleSpriteDirection direction);
};

#endif /* defined(__OzgGameRPG__RPGMapRoleSprite__) */
