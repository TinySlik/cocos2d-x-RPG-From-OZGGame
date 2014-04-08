//
//  RPGBattlePlayerSprite.h
//  OzgGameRPG
//
//  Created by ozg on 14-4-3.
//
//

#ifndef __OzgGameRPG__RPGBattlePlayerSprite__
#define __OzgGameRPG__RPGBattlePlayerSprite__

#include "cocos2d.h"
#include "RPGData.h"

USING_NS_CC;
using namespace std;

enum RPGBattlePlayerSpriteTag
{
    kRPGBattlePlayerSpriteTagCursor = 1
    
};

class RPGBattlePlayerSprite : public CCSprite
{
    
private:
        
    CCArray* m_spriteFramesNormal; //通常
    CCArray* m_spriteFramesHurt; //受到攻击
    CCArray* m_spriteFramesDying; //临死
    CCArray* m_spriteFramesWin; //胜利
    CCArray* m_spriteFramesAvoid; //回避
    CCArray* m_spriteFramesDeath; //战斗不能
    CCArray* m_spriteFramesAttack; //一般攻击
    CCArray* m_spriteFramesUseItem; //使用道具
    CCArray* m_spriteFramesSpecialAttack; //特殊攻击
    CCArray* m_spriteFramesChanting; //咏唱
    
public:
    
    RPGPlayer* m_data;
    
    bool m_isSelected;
    
    RPGBattlePlayerSprite();
    virtual ~RPGBattlePlayerSprite();
    bool initWithPlayerData(RPGPlayer* data);
    static RPGBattlePlayerSprite* createWithPlayerData(RPGPlayer* data);
    
    void selected(bool isSelected);
    
};

#endif /* defined(__OzgGameRPG__RPGBattlePlayerSprite__) */
