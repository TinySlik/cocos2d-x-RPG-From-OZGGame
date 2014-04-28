//
//  RPGData.h
//  OzgGameRPG
//
//  Created by ozg on 14-4-5.
//
//

#ifndef __OzgGameRPG__RPGData__
#define __OzgGameRPG__RPGData__

#include "cocos2d.h"

USING_NS_CC;
using namespace std;

enum RPGDataStatus {
    kRPGDataStatusNormal = 1, //通常时
    kRPGDataStatusDeath = 2, //战斗不能
    kRPGDataStatusDying = 3 //临死
};

class RPGBaseRole : public CCObject
{
    
public:
    
    int m_dataId;
    string m_name;
    int m_maxHP;
    int m_HP; //子类是怪物的话，数据库里面没有这个字段
    int m_maxMP;
    int m_MP; //子类是怪物的话，数据库里面没有这个字段
    float m_attack;
    float m_defense;
    float m_speed;
    float m_skillAttack;
    float m_skillDefense;
    
    string m_skill;
    
    float m_progress; //进度条，数据库没有这个字段
    
    RPGDataStatus m_status; //对象状态，数据库暂时没有这个字段
    
    int m_exp; //子类是RPGPlayer的话，这个字段就是总经验值，怪物的话就是获得的经验值
    
    RPGBaseRole();
    virtual ~RPGBaseRole();
    //没有create方法
    
};

class RPGPlayer : public RPGBaseRole
{
    
public:
    
    int m_itemsIdArms;
    int m_itemsIdArmor;
    int m_level;
    
    int m_nextExp;
    string m_texPrefix;
    
    RPGPlayer();
    virtual ~RPGPlayer();
    
    static RPGPlayer* create();
    
};

class RPGMonster : public RPGBaseRole
{
    
public:
    
    string m_tex;
    int m_gold;
    
    int m_tag; //格式为 enum tag + 0到999的无重复数（就算同一个怪物也不能重复）
    
    RPGMonster();
    virtual ~RPGMonster();
    
    static RPGMonster* create();
    
};

class RPGSkill : public CCObject
{
    
public:
    int m_dataId;
    string m_name;
    float m_skillAttack;
    int m_MP;
    int m_type;
    int m_attr;
    
    RPGSkill();
    virtual ~RPGSkill();
    
    static RPGSkill* create();
};

//这个用于在战斗的选择技能列表、地图的技能菜单用到
class RPGSkillBtnData : public RPGSkill
{
    
public:
    
    bool m_enabled; //是否可点击
    
    static RPGSkillBtnData* create();
    
};

class RPGItems : public CCObject
{
    
public:
    
    int m_dataId;
    string m_name;
    float m_buy;
    float m_sell;
    int m_type;
    float m_attack;
    float m_defense;
    float m_speed;
    float m_skillAttack;
    float m_skillDefense;
    
    RPGItems();
    virtual ~RPGItems();
    
    static RPGItems* create();
    
};

//现有道具
class RPGExistingItems : public RPGItems
{
    
public:
    
    int m_total; //该道具的个数
    
    RPGExistingItems();
    virtual ~RPGExistingItems();
    
    static RPGExistingItems* create();
};

//存档
class RPGSaveData : public CCObject
{
    
public:
    
    int m_dataId;
    int m_mapId;
    float m_playerToX;
    float m_playerToY;
    string m_playerDirection;
    int m_gold;
    
    RPGSaveData();
    virtual ~RPGSaveData();
    
    static RPGSaveData* create();
};

#endif /* defined(__OzgGameRPG__RPGData__) */
