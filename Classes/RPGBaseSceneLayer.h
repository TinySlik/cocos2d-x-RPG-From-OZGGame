//
//  RPGBaseSceneLayer.h
//  OzgGameRPG
//
//  Created by ozg on 14-3-17.
//
//

#ifndef __OzgGameRPG__RPGBaseSceneLayer__
#define __OzgGameRPG__RPGBaseSceneLayer__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "JsonBox.h"
#include "SimpleAudioEngine.h"
#include "CppSQLite3.h"
#include "GameCfg.h"
#include "RPGDialogLayer.h"
#include "RPGData.h"

USING_NS_CC;
USING_NS_CC_EXT;

using namespace std;
using namespace CocosDenshion;

//获取player列表
#define PLAYER_QUERY "select * from player order by id asc"

//获取save_data数据
#define SAVEDATA_QUERY "select * from save_data where id = 1"

//查询现有道具
#define ITEMS_EXISTING_QUERY "select i.*, ie.total from items_existing as ie inner join items as i on ie.id = i.id order by i.id asc"

//按type查询现有道具
#define ITEMS_EXISTING_QUERY_TYPE "select i.*, ie.total from items_existing as ie inner join items as i on ie.id = i.id where i.type = %i order by i.id asc"

#define EQUIP_EXISTING_QUERY "select i.*, ie.total from items_existing as ie inner join items as i on ie.id = i.id where i.type = 1 or i.type = 2 order by i.id asc"

//获取一个player的详细数据
#define PLAYER_DETAIL_QUERY "select p.*, arms.name_cns as arms_name, armor.name_cns as armor_name from player as p left join items as arms on p.items_id_arms = arms.id left join items as armor on p.items_id_armor = armor.id where p.id = %i"

//查询一个角色的技能
#define SKILL_QUERY "select * from skill where id in(%s)"

#define SAVEDATA_MAP_QUERY "select m.name as map_name, m.location_cns, sd.map_id, m.has_enemy, m.bg_audio, sd.player_to_x, sd.player_to_y, sd.player_direction, sd.gold from save_data as sd inner join map as m on sd.map_id = m.id where sd.id = 1" //获取存档和地图数据

//技能的属性
enum RPGSkillAttr
{
    kRPGSkillAttrNormal = 1, //通常属性
    kRPGSkillAttrFire = 2, //火
    kRPGSkillAttrIce = 3, //冰
    kRPGSkillAttrNone = 10, //无
};

void addLab(CCNode* parentNode, int tag, CCString* text, CCPoint point); //增加状态界面的lab
void addLab(CCNode* parentNode, int tag, CCString* text, float fontSize, CCPoint point);
void addLab(CCNode* parentNode, int tag, CCString* text, float fontSize, CCTextAlignment textAlignment, CCPoint point);

void saveData(CppSQLite3DB* db, RPGSaveData *saveData);
RPGSaveData* loadSaveData(CppSQLite3DB* db);
void savePlayerData(CppSQLite3DB* db, CCArray* playerDataList);

class RPGBaseSceneLayer : public CCLayer
{
    
protected:
    CCDictionary *m_stringList; //语言文件的数据
    CppSQLite3DB m_db;
        
public:
    
    RPGBaseSceneLayer();
    virtual ~RPGBaseSceneLayer();
    virtual bool init();
    
};

#endif /* defined(__OzgGameRPG__RPGBaseSceneLayer__) */
