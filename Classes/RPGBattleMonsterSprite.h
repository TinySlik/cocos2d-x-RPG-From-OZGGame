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
#include "RPGBattlePlayerSprite.h"

USING_NS_CC;
using namespace std;

enum RPGBattleMonsterSpriteTag
{
    kRPGBattleMonsterSpriteTagCursor = 1,
    kRPGBattleMonsterSpriteTagEffectResults = 2
    
};

class RPGBattleMonsterSprite : public CCSprite
{
    
private:
    
    int m_HPResults; //临时保存HP效果值
    
    void showEffectResultsEnd(); //showEffectResults执行完毕后执行，然后再执行showEffectResults参数中的回调
        
public:
    bool m_isSelected;
    
    RPGMonster* m_data;
    
    RPGBattleMonsterSprite();
    virtual ~RPGBattleMonsterSprite();
    bool initWithMonsterData(RPGMonster* data);
    
    static RPGBattleMonsterSprite* createWithMonsterData(RPGMonster* data);
    
    void selected(bool isSelected);
    
    void showEffectResults(CCObject* target, int HPResults, CCNode* player); //显示效果值，target为回调对象，攻击的话HPResults传入的是负值，回复的话为正值
    
    void animAttack(CCObject* target, CCObject *targetObjData); //执行一般攻击或技能攻击的动作，target为回调对象，targetObjData为攻击的对象
    
};

#endif /* defined(__OzgGameRPG__RPGBattleMonsterSprite__) */
