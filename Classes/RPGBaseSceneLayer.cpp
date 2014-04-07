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

void saveData(CppSQLite3DB* db, int mapId, float toX, float toY, string playerDirection)
{
    
    db->execDML(CCString::createWithFormat("update save_data set map_id = %i, player_to_x = %f, player_to_y = %f, player_direction = '%s' where id = 1", mapId, toX, toY, playerDirection.c_str())->getCString());
}

void savePlayerData(CppSQLite3DB* db, CCArray* playerDataList)
{
    for (int i = 0; i < playerDataList->count(); i++)
    {
        RPGPlayer *playerData = (RPGPlayer*)playerDataList->objectAtIndex(i);
        
        CCString *sql = CCString::createWithFormat("update player set max_hp = %i, hp = %i, max_mp = %i, mp = %i, attack = %f, defense = %f, speed = %f, magic_attack = %f, magic_defense = %f, items_id_arms = %i, items_id_armor = %i, level = %i, skill = '%s', next_exp = %i where id = %i", playerData->m_maxHP, playerData->m_HP, playerData->m_maxMP, playerData->m_MP, playerData->m_attack, playerData->m_defense, playerData->m_speed, playerData->m_magicAttack, playerData->m_magicDefense, playerData->m_itemsIdArms, playerData->m_itemsIdArmor, playerData->m_level, playerData->m_skill.c_str(), playerData->m_nextExp, playerData->m_dataId);
        db->execDML(sql->getCString());
    }
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
