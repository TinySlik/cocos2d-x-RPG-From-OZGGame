//
//  RPGBattleMonsterSprite.h
//  OzgGameRPG
//
//  Created by ozg on 14-4-4.
//
//

#ifndef __OzgGameRPG__RPGBattleMonsterSprite__
#define __OzgGameRPG__RPGBattleMonsterSprite__

#include "cocos2d.h"
#include "RPGData.h"

USING_NS_CC;
using namespace std;

enum RPGBattleMonsterSpriteTag
{
    kRPGBattleMonsterSpriteTagCursor = 1
    
};

class RPGBattleMonsterSprite : public CCSprite
{
    
private:
    
public:
    bool m_isSelected;
    
    RPGMonster* m_data;
    
    RPGBattleMonsterSprite();
    virtual ~RPGBattleMonsterSprite();
    bool initWithMonsterData(RPGMonster* data);
    
    static RPGBattleMonsterSprite* createWithMonsterData(RPGMonster* data);
    
    void selected(bool isSelected);
    
};

#endif /* defined(__OzgGameRPG__RPGBattleMonsterSprite__) */
