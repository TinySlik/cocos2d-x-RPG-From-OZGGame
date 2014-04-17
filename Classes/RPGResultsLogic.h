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
    static bool useItems(CppSQLite3DB* db, int playerId, int itemsId); //使用道具，返回成功或失败
    
    static bool useSkillCure(CppSQLite3DB* db, int srcPlayerId, int targetPlayerId, int srcSkillId); //使用回复技能
    
};

#endif /* defined(__OzgGameRPG__RPGResultsLogic__) */
