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

class RPGComputingResults
{
    
public:
    
    static int attackResults(float attack, float defense); //一般攻击
    static int skillAttackResults(float srcSkillAttack, float skillAttack, RPGSkillAttr srcAttackAttr, float targetSkillDefense, RPGSkillAttr targetDefenseAttr); //技能攻击，srcSkillAttack为RPGBaseRole的m_skillAttack，skillAttack为RPGSkill的m_skillAttack，targetSkillDefense为被使用技能的对象的技能防御，targetDefenseAttr为被使用技能的对象的属性
    static int skillCureResults(float srcSkillAttack, float skillAttack); //技能治疗，srcSkillAttack为RPGBaseRole的m_skillAttack，skillAttack为RPGSkill的m_skillAttack
    
    static int itemCure(int itemsId); //道具治疗
    
};

#endif /* defined(__OzgGameRPG__RPGComputingResults__) */
