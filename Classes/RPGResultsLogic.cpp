//
//  RPGResultsLogic.cpp
//  OzgGameRPG
//
//  Created by ozg on 14-4-13.
//
//

#include "RPGResultsLogic.h"

//执行操作后player的数据更新
#define PLAYER_UPDATE "update player set %s = %i where id = %i"

//更新player的各个状态值
#define PLAYER_UPDATE_STATUS "update player set attack = %f, defense = %f, speed = %f, skill_attack = %f, skill_defense = %f, %s = %i where id = %i"

//获取一个已有道具的总数
#define EXISTING_ITEMS_TOTAL "select total from items_existing where id = %i"

//丢弃一个道具
#define ITEMS_DISCARD "delete from items_existing where id = %i"

//扣减一个道具
#define ITEMS_DEDUCTIONS "update items_existing set total = total - 1 where id = %i"

//查询一个现有道具的数量
#define ITEMS_GET "select * from items_existing where id = %i"

//查询一个技能的明细
#define SKILL_DETAIL_QUERY "select * from skill where id = %i"

//查询一条player数据
#define PLAYER_SINGLE_QUERY "select * from player where id = %i"

//查询一条items数据
#define ITEMS_SINGLE_QUERY "select * from items where id = %i"

//是否已存在该道具
#define EXIST_ITEMS "select count(id) from items_existing where id = %i"

//增加一个已有道具
#define ADD_EXISTING_ITEMS "insert into items_existing (id, total) values(%i, 1)"

//更新一个已有道具的数量
#define UPDATE_EXISTING_ITEMS "update items_existing set total = total + 1 where id = %i"

void RPGResultsLogic::discardItems(CppSQLite3DB *db, int itemsId)
{
    db->execDML(CCString::createWithFormat(ITEMS_DISCARD, itemsId)->getCString());
}

void RPGResultsLogic::deductionsItems(CppSQLite3DB *db, int itemsId)
{
    CCString *sql = NULL;
    CppSQLite3Query query = db->execQuery(CCString::createWithFormat(ITEMS_GET, itemsId)->getCString());
    while(!query.eof())
    {
        if(query.getIntField("total") <= 1)
            sql = CCString::createWithFormat(ITEMS_DISCARD, itemsId);
        else
            sql = CCString::createWithFormat(ITEMS_DEDUCTIONS, itemsId);
        
        query.nextRow();
    }
    query.finalize();
    
    if(sql)
        db->execDML(sql->getCString());

}

bool RPGResultsLogic::useItems(CppSQLite3DB* db, int playerId, int itemsId)
{
    /*
     6 草药
     7 党参
     8 复活药
     
     */
    
    string field;
    string maxField;
    if(itemsId == 7)
    {
        field = "mp"; //党参加mp
        maxField = "max_mp";
    }
    else
    {
        field = "hp";
        maxField = "max_hp";
    }
    
    //道具的效果值
    int results = 0;
    
    CppSQLite3Query query = db->execQuery(CCString::createWithFormat(PLAYER_DETAIL_QUERY, playerId)->getCString());
    while(!query.eof())
    {
        //死亡状态下不能直接恢复hp的道具
        if(query.getIntField("hp") <= 0 && itemsId != 8)
            return false;
        else if(query.getIntField("hp") <= 0 && itemsId == 8)
            results = 1; //复活后是1HP
        else if(query.getIntField("hp") > 0 && itemsId == 8)
            return false; //活着的人不能使用复活药
        else
        {
            //恢复HP或MP
            results = RPGComputingResults::itemCure(itemsId);
            results += query.getIntField(field.c_str());
        
            //超过最大值的处理
            if(results > query.getIntField(maxField.c_str()))
                results = query.getIntField(maxField.c_str());
        }
        
        query.nextRow();
    }
    query.finalize();
    
    //使用道具
    {
        CCString *sql = CCString::createWithFormat(PLAYER_UPDATE, field.c_str(), results, playerId);
        db->execDML(sql->getCString());
        
        CCLog("%s", sql->getCString());
    }
    
    //扣减道具数
    RPGResultsLogic::deductionsItems(db, itemsId);
    
    CCLog("对player%d 使用了一个道具%d", playerId, itemsId);
    return true;
}

bool RPGResultsLogic::useSkillCure(CppSQLite3DB *db, int srcPlayerId, int targetPlayerId, int srcSkillId)
{
    RPGPlayer *srcPlayerData;
    RPGSkill *srcSkillData;
    
    RPGPlayer *targetPlayerData;
    
    //查询使用技能的player
    CppSQLite3Query srcPlayerQuery = db->execQuery(CCString::createWithFormat(PLAYER_DETAIL_QUERY, srcPlayerId)->getCString());
    while(!srcPlayerQuery.eof())
    {
        srcPlayerData = RPGPlayer::create();
        srcPlayerData->m_dataId = srcPlayerQuery.getIntField("id");
        srcPlayerData->m_maxMP = srcPlayerQuery.getIntField("max_mp");
        srcPlayerData->m_MP = srcPlayerQuery.getIntField("mp");
        srcPlayerData->m_skillAttack = srcPlayerQuery.getFloatField("skill_attack");
        
        srcPlayerQuery.nextRow();
    }
    srcPlayerQuery.finalize();
    
    //查询使用的技能
    CppSQLite3Query srcSkillQuery = db->execQuery(CCString::createWithFormat(SKILL_DETAIL_QUERY, srcSkillId)->getCString());
    while(!srcSkillQuery.eof())
    {
        srcSkillData = RPGSkill::create();
        srcSkillData->m_dataId = srcSkillQuery.getIntField("id");
        srcSkillData->m_MP = srcSkillQuery.getIntField("mp");
        srcSkillData->m_name = srcSkillQuery.getStringField("name_cns");
        srcSkillData->m_skillAttack = srcSkillQuery.getFloatField("skill_attack");
        
        srcSkillQuery.nextRow();
    }
    srcSkillQuery.finalize();
    
    //查询目标player
    CppSQLite3Query targetPlayerQuery = db->execQuery(CCString::createWithFormat(PLAYER_DETAIL_QUERY, targetPlayerId)->getCString());
    while(!targetPlayerQuery.eof())
    {
        targetPlayerData = RPGPlayer::create();
        targetPlayerData->m_dataId = targetPlayerQuery.getIntField("id");
        targetPlayerData->m_maxHP = targetPlayerQuery.getIntField("max_hp");
        targetPlayerData->m_HP = targetPlayerQuery.getIntField("hp");
        
        targetPlayerQuery.nextRow();
    }
    targetPlayerQuery.finalize();
    
    //执行逻辑
    if(srcPlayerData && srcSkillData && targetPlayerData)
    {
        //计算player时候技能后的效果值
        int targetResults = RPGComputingResults::skillCureResults(srcPlayerData->m_skillAttack, srcSkillData->m_skillAttack);
        targetPlayerData->m_HP += targetResults;
        
        CCLog("player %i 回复了 %i HP", targetPlayerId, targetResults);
        
        //超过HP最大值
        if(targetPlayerData->m_HP > targetPlayerData->m_maxHP)
            targetPlayerData->m_HP = targetPlayerData->m_maxHP;        
        db->execDML(CCString::createWithFormat(PLAYER_UPDATE, "hp", targetPlayerData->m_HP, targetPlayerData->m_dataId)->getCString());
        
        //扣减使用者的MP
        srcPlayerData->m_MP -= srcSkillData->m_MP;
        if(srcPlayerData->m_MP < 0)
            srcPlayerData->m_MP = 0;
        
        CCLog("player %i 消耗了 %i MP", srcPlayerId, srcSkillData->m_MP);
        
        db->execDML(CCString::createWithFormat(PLAYER_UPDATE, "mp", srcPlayerData->m_MP, srcPlayerData->m_dataId)->getCString());
        
    }
    
    return true;
}

void RPGResultsLogic::removeEquip(CppSQLite3DB *db, int playerId, int type)
{
    //武器
    CCString *playerSql = NULL;
    CCString *itemsExistingSql = NULL;
    
    int itemsId;
    
    string itemField;
    if(type == 1)
        itemField = "items_id_arms";
    else if(type == 2)
        itemField = "items_id_armor";
    else
        return;
    
    {
        CppSQLite3Query query = db->execQuery(CCString::createWithFormat(PLAYER_SINGLE_QUERY, playerId)->getCString());
        while(!query.eof())
        {
            itemsId = query.getIntField(itemField.c_str());
            query.nextRow();
        }
        query.finalize();
    }
    
    float attack;
    float defense;
    float speed;
    float skillAttack;
    float skillDefense;
    
    {
        //player将装备卸下后各个值发生变化
        
        CppSQLite3Query query = db->execQuery(CCString::createWithFormat(ITEMS_SINGLE_QUERY, itemsId)->getCString());
        while(!query.eof())
        {
            attack = query.getFloatField("attack");
            defense = query.getFloatField("defense");
            speed = query.getFloatField("speed");
            skillAttack = query.getFloatField("skill_attack");
            skillDefense = query.getFloatField("skill_defense");
            
            query.nextRow();
        }
        query.finalize();
    }
    
    //各个值的处理
    {
        CppSQLite3Query query = db->execQuery(CCString::createWithFormat(PLAYER_SINGLE_QUERY, playerId)->getCString());
        while(!query.eof())
        {
            attack = query.getFloatField("attack") - attack;
            defense = query.getFloatField("defense") - defense;
            speed = query.getFloatField("speed") - speed;
            skillAttack = query.getFloatField("skill_attack") - skillAttack;
            skillDefense = query.getFloatField("skill_defense") - skillDefense;
            
            //最小值的处理
            if(attack < 0) attack = 0;
            if(defense < 0) defense = 0;
            if(speed < 0) speed = 0;
            if(skillAttack < 0) skillAttack = 0;
            if(skillDefense < 0) skillDefense = 0;
            
            query.nextRow();
        }
        query.finalize();
    }
    
    int equipId = 0;
    playerSql = CCString::createWithFormat(PLAYER_UPDATE_STATUS, attack, defense, speed, skillAttack, skillDefense, itemField.c_str(), equipId, playerId);
    
    {
        //将卸下的装备放回已有道具处
        CppSQLite3Query query = db->execQuery(CCString::createWithFormat(EXIST_ITEMS, itemsId)->getCString());
        while(!query.eof())
        {
            if(query.getIntField(0) <= 0)
                itemsExistingSql = CCString::createWithFormat(ADD_EXISTING_ITEMS, itemsId);
            else
                itemsExistingSql = CCString::createWithFormat(UPDATE_EXISTING_ITEMS, itemsId);
            
            query.nextRow();
        }
        query.finalize();
    }
    
    if(playerSql)
        db->execDML(playerSql->getCString());
    if(itemsExistingSql)
        db->execDML(itemsExistingSql->getCString());
}

void RPGResultsLogic::equip(CppSQLite3DB *db, int playerId, int itemsId)
{
    CCString *playerSql = NULL;
    CCString *itemsExistingSql = NULL;
    
    int type;
    float attack;
    float defense;
    float speed;
    float skillAttack;
    float skillDefense;
    
    {
        //查询该装备的类型和效果值
        
        CppSQLite3Query query = db->execQuery(CCString::createWithFormat(ITEMS_SINGLE_QUERY, itemsId)->getCString());
        while(!query.eof())
        {
            attack = query.getFloatField("attack");
            defense = query.getFloatField("defense");
            speed = query.getFloatField("speed");
            skillAttack = query.getFloatField("skill_attack");
            skillDefense = query.getFloatField("skill_defense");
            type = query.getIntField("type");
            
            query.nextRow();
        }
        query.finalize();
    }
    
    string itemField;
    if(type == 1)
        itemField = "items_id_arms";
    else if(type == 2)
        itemField = "items_id_armor";
    else
        return;
    
    //如果已装备了则先卸掉此装备
    {
        CppSQLite3Query query = db->execQuery(CCString::createWithFormat(PLAYER_SINGLE_QUERY, playerId)->getCString());
        while(!query.eof())
        {
            if(query.getIntField(itemField.c_str()) != 0)
                RPGResultsLogic::removeEquip(db, playerId, type);
            
            query.nextRow();
        }
        query.finalize();
    }
    
    //各个值的处理
    {
        CppSQLite3Query query = db->execQuery(CCString::createWithFormat(PLAYER_SINGLE_QUERY, playerId)->getCString());
        while(!query.eof())
        {
            attack = query.getFloatField("attack") + attack;
            defense = query.getFloatField("defense") + defense;
            speed = query.getFloatField("speed") + speed;
            skillAttack = query.getFloatField("skill_attack") + skillAttack;
            skillDefense = query.getFloatField("skill_defense") + skillDefense;
            
            //最大值的处理
            if(attack > MAX_STATUS2) attack = MAX_STATUS2;
            if(defense > MAX_STATUS2) defense = MAX_STATUS2;
            if(speed > MAX_STATUS2) speed = MAX_STATUS2;
            if(skillAttack > MAX_STATUS2) skillAttack = MAX_STATUS2;
            if(skillDefense > MAX_STATUS2) skillDefense = MAX_STATUS2;
            
            query.nextRow();
        }
        query.finalize();
    }
    
    //player装备了之后各个值发生变化
    playerSql = CCString::createWithFormat(PLAYER_UPDATE_STATUS, attack, defense, speed, skillAttack, skillDefense, itemField.c_str(), itemsId, playerId);
    
    {
        //扣减装备了的道具
        CppSQLite3Query query = db->execQuery(CCString::createWithFormat(EXISTING_ITEMS_TOTAL, itemsId)->getCString());
        while(!query.eof())
        {
            if(query.getIntField(0) <= 1)
                itemsExistingSql = CCString::createWithFormat(ITEMS_DISCARD, itemsId);
            else
                itemsExistingSql = CCString::createWithFormat(ITEMS_DEDUCTIONS, itemsId);
            
            query.nextRow();
        }
        query.finalize();
    }
    
    if(playerSql)
        db->execDML(playerSql->getCString());
    if(itemsExistingSql)
        db->execDML(itemsExistingSql->getCString());
    
}
