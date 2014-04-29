//
//  RPGResultsLogic.h
//  OzgGameRPG
//
//  Created by ozg on 14-4-13.
//
//

#ifndef __OzgGameRPG__RPGResultsLogic__
#define __OzgGameRPG__RPGResultsLogic__

#include "cocos2d.h"
#include "OzgCCUtility.h"
#include "RPGComputingResults.h"
#include "CppSQLite3.h"
#include "JsonBox.h"

USING_NS_CC;
using namespace std;

class RPGResultsLogic
{
    
public:
    static void discardItems(CppSQLite3DB* db, int itemsId); //丢弃道具
    static void deductionsItems(CppSQLite3DB* db, int itemsId); //扣减道具数
    static bool useItems(CppSQLite3DB* db, int playerId, int itemsId); //地图画面中使用道具，返回成功或失败
    static int battleUseItems(CCArray* existingItems, RPGBaseRole* targetData, int itemsId); //战斗画面中使用道具，existingItems为已有道具，objData为RPGPlayer、RPGMonster，返回的是该道具的效果值
    
    static bool useSkillCure(CppSQLite3DB* db, int srcPlayerId, int targetPlayerId, int srcSkillId); //使用回复技能
    
    static void removeEquip(CppSQLite3DB* db, int playerId, int type); //player卸掉一个装备，type为1表示武器，2表示防具
    static void equip(CppSQLite3DB* db, int playerId, int itemsId); //装备
    
};

#endif /* defined(__OzgGameRPG__RPGResultsLogic__) */
