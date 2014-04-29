//
//  RPGComputingResults.cpp
//  OzgGameRPG
//
//  Created by ozg on 14-4-8.
//
//

#include "RPGComputingResults.h"
#include "OzgCCUtility.h"

int RPGComputingResults::attackResults(float attack, float defense)
{
    attack = attack / 0.25;
    attack = OzgCCUtility::randomFloat(attack - (attack * 0.05), attack + (attack * 0.05));
    
    defense = defense / 0.5;
    defense = OzgCCUtility::randomFloat(defense - (defense * 0.025), defense + (defense * 0.025));
    
    int results = (int)(attack - defense);
    if(results < 0)
        results = 0;
    
    return results;
}

int RPGComputingResults::skillAttackResults(float srcSkillAttack, float skillAttack, RPGSkillAttr srcAttackAttr, float targetSkillDefense, RPGSkillAttr targetDefenseAttr)
{
    int results = RPGComputingResults::attackResults(srcSkillAttack, targetSkillDefense);
    
    skillAttack = skillAttack / 0.25;
    skillAttack = OzgCCUtility::randomFloat(skillAttack - (skillAttack * 0.05), skillAttack + (skillAttack * 0.05));
    results = results + (int)skillAttack;
    
    //下面是属性处理
    //火克冰
    if(srcAttackAttr == kRPGSkillAttrFire && targetDefenseAttr == kRPGSkillAttrIce)
        results = results + (results * 0.5);
    else if(srcAttackAttr == kRPGSkillAttrIce && targetDefenseAttr == kRPGSkillAttrFire)
        results = results - (results * 0.5);
    
    //无属性克制除自己属性以外的所有属性
    else if(srcAttackAttr == kRPGSkillAttrNone && targetDefenseAttr != kRPGSkillAttrNone)
        results = results + (results * 0.5);
    else if(srcAttackAttr != kRPGSkillAttrNone && targetDefenseAttr == kRPGSkillAttrNone)
        results = results - (results * 0.5);
    
    return results;
}

int RPGComputingResults::skillCureResults(float srcSkillAttack, float skillAttack)
{
    //不需要浮动值
    srcSkillAttack = srcSkillAttack / 0.5;
    skillAttack = skillAttack / 0.25;
    int results = (int)(srcSkillAttack + skillAttack);
    return results;
}

int RPGComputingResults::itemCure(int itemsId)
{
    switch (itemsId)
    {
        case 6:
            //草药治疗，返回的是HP
            return 100;
            break;
        case 7:
            //党参治疗，返回的是MP
            return 30;
            break;
        default:
            
            return 0;
            break;
    }
    
}
