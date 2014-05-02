//
//  RPGBaseSceneLayer.cpp
//  OzgGameRPG
//
//  Created by ozg on 14-3-17.
//
//

#include "RPGBaseSceneLayer.h"

void addLab(CCNode* parentNode, int tag, CCString* text, CCPoint point)
{
    addLab(parentNode, tag, text, 20, point);
}

void addLab(CCNode* parentNode, int tag, CCString* text, float fontSize, CCPoint point)
{
    addLab(parentNode, tag, text, fontSize, kCCTextAlignmentLeft, point);
}

void addLab(CCNode* parentNode, int tag, CCString* text, float fontSize, CCTextAlignment textAlignment, CCPoint point)
{
    CCLabelTTF *textLab = CCLabelTTF::create(text->getCString(), "Arial", fontSize, CCSizeMake(400, 25), textAlignment);
    textLab->setPosition(point);
    textLab->setTag(tag);
    parentNode->addChild(textLab);
}

void saveData(CppSQLite3DB* db, RPGSaveData *saveData)
{
    
    db->execDML(CCString::createWithFormat("update save_data set map_id = %i, player_to_x = %f, player_to_y = %f, player_direction = '%s', gold = %i where id = 1", saveData->m_mapId, saveData->m_playerToX, saveData->m_playerToY, saveData->m_playerDirection.c_str(), saveData->m_gold)->getCString());
}

RPGSaveData* loadSaveData(CppSQLite3DB* db)
{
    RPGSaveData *saveDataObj = RPGSaveData::create();
    CppSQLite3Query query = db->execQuery(SAVEDATA_QUERY);
    
    saveDataObj->m_dataId = query.getIntField("id");
    saveDataObj->m_mapId = query.getIntField("map_id");
    saveDataObj->m_playerToX = query.getFloatField("player_to_x");
    saveDataObj->m_playerToY = query.getFloatField("player_to_y");
    saveDataObj->m_playerDirection = query.getStringField("player_direction");
    saveDataObj->m_gold = query.getIntField("gold");
    
    query.finalize();
    
    return saveDataObj;
}

RPGBaseSceneLayer::RPGBaseSceneLayer()
{
    
}

RPGBaseSceneLayer::~RPGBaseSceneLayer()
{
    this->m_db.close();
    
}

bool RPGBaseSceneLayer::init()
{
    if(CCLayer::init())
    {
        string dbPath = CCFileUtils::sharedFileUtils()->getWritablePath();
        dbPath.append(GAME_SYS_DB);
        this->m_db.open(dbPath.c_str());
        
        return true;
    }
    
    return false;
}
