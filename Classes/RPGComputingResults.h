//
//  RPGComputingResults.h
//  OzgGameRPG
//
//  Created by ozg on 14-4-8.
//
//

#ifndef __OzgGameRPG__RPGComputingResults__
#define __OzgGameRPG__RPGComputingResults__

#include "cocos2d.h"
#include "RPGBaseSceneLayer.h"

USING_NS_CC;

class RPGComputingResults : public CCObject
{
    
public:
    
    static int attackResults(float attack, float defense); //一般攻击
    static int skillAttackResults(float playerSkillAttack, float skillAttack, RPGSkillAttr attackAttr, float skillDefense, RPGSkillAttr defenseAttr); //技能攻击
    static int skillCureResults(float playerSkillAttack, float skillAttack); //治疗
    
    static int itemCure(int itemsId); //道具治疗
    
};

#endif /* defined(__OzgGameRPG__RPGComputingResults__) */
