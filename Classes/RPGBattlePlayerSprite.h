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
    kRPGBattlePlayerSpriteTagCursor = 1,
    kRPGBattlePlayerSpriteTagArm = 2,
    kRPGBattlePlayerSpriteTagEffectResults = 3
    
};

class RPGBattlePlayerSprite : public CCSprite
{
    
private:
    
    int m_HPResults; //临时保存HP效果值
    
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
    
    void showEffectResultsEnd(); //showEffectResults执行完毕后执行，然后再执行showEffectResults参数中的回调
    
public:
    
    RPGPlayer* m_data;
    
    bool m_isSelected;
    
    RPGBattlePlayerSprite();
    virtual ~RPGBattlePlayerSprite();
    bool initWithPlayerData(RPGPlayer* data);
    static RPGBattlePlayerSprite* createWithPlayerData(RPGPlayer* data);
    
    void selected(bool isSelected);
    
    void animNormal(); //执行通常时的状态，这个方法会根据实际HP的状态执行对应状态
    void animDying(); //执行临死的状态
    void animDeath(); //执行战斗不能的状态
    void animWin(); //执行胜利的状态
    void animHurt(); //执行受到攻击的状态，执行一次完毕后回到通常时状态，执行animNormal()
    
    void animAttack(CCObject* target, CCObject *targetObjData); //执行一般攻击的动作，target为回调对象，targetObjData为攻击的对象
    
    void animSkill(CCObject* target, CCObject *targetObjData); //执行技能，target为回调对象，targetObjData为目标对象
    
    void showEffectResults(CCObject* target, int HPResults, CCNode* srcAttackObj); //显示效果值，target为回调对象，攻击的话HPResults传入的是负值，回复的话为正值
    
};

#endif /* defined(__OzgGameRPG__RPGBattlePlayerSprite__) */
