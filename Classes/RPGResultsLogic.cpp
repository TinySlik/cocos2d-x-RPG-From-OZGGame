//
//  RPGResultsLogic.cpp
//  OzgGameRPG
//
//  Created by ozg on 14-4-13.
//
//

#include "RPGResultsLogic.h"

//获取一个player的数据
#define PLAYER_DETAIL_QUERY "select * from player where id = %i"

//使用道具后player的数据更新
#define ITEMS_USE_UPDATE "update player set %s = %i where id = %i"

//使用道具后现有道具的变化
#define SAVEDATA_ITEMS_UPDATE "update save_data set items = '%s' where id = 1"

void RPGResultsLogic::deductionsItems(CppSQLite3DB *db, int itemsId)
{
    CppSQLite3Query query = db->execQuery(SAVEDATA_QUERY);
    while(!query.eof())
    {
        JsonBox::Value json;
        json.loadFromString(query.getStringField("items"));
        
        JsonBox::Array resultsJsonArr; //输出结果
        
        JsonBox::Array items = json.getArray();
        for (int i = 0; i < (int)items.size(); i++)
        {
            JsonBox::Value item = items[i].getObject();
            
            //扣减目标id
            if(item["id"].getInt() == itemsId)
                item["total"] = item["total"].getInt() - 1;
            
            //如果该道具没有用完，则加入输出结果
            if(item["total"].getInt() > 0)
                resultsJsonArr.push_back(item);
        }
        
        ostringstream s;
        ((JsonBox::Value)resultsJsonArr).writeToStream(s);
        string result = s.str();
//        CCLog("%s", result.c_str());
        db->execDML(CCString::createWithFormat(SAVEDATA_ITEMS_UPDATE, result.c_str())->getCString());
        
        query.nextRow();
    }
    query.finalize();
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
        CCString *sql = CCString::createWithFormat(ITEMS_USE_UPDATE, field.c_str(), results, playerId);
        db->execDML(sql->getCString());
        
        CCLog("%s", sql->getCString());
    }
    
    //扣减道具数
    RPGResultsLogic::deductionsItems(db, itemsId);
    
    CCLog("对player%d 使用了一个道具%d", playerId, itemsId);
    return true;
}
