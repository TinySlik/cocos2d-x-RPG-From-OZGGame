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

class RPGBattleMonsterSprite : public CCSprite
{
    
private:
    
    RPGMonster* m_data;
    
public:
    RPGBattleMonsterSprite();
    virtual ~RPGBattleMonsterSprite();
    bool initWithMonsterData(RPGMonster* data);
    
    static RPGBattleMonsterSprite* createWithMonsterData(RPGMonster* data);
    
};

#endif /* defined(__OzgGameRPG__RPGBattleMonsterSprite__) */
