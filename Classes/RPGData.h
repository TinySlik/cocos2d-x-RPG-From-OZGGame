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

class RPGPlayer : public CCObject
{
    
public:
    
    int m_dataId;
    string m_name;
    int m_maxHP;
    int m_HP;
    int m_maxMP;
    int m_MP;
    float m_attack;
    float m_defense;
    float m_speed;
    float m_magicAttack;
    float m_magicDefense;
    int m_itemsIdArms;
    int m_itemsIdArmor;
    int m_level;
    string m_skill;
    int m_nextExp;
    string m_texPrefix;
    
    float m_progress;
    
    RPGPlayer();
    virtual ~RPGPlayer();
    
    static RPGPlayer* create();
    
};

class RPGMonster : public CCObject
{
    
public:
    
    int m_dataId;
    string m_name;
    int m_maxHP;
    int m_HP; //数据库里面没有这个字段
    float m_attack;
    float m_defense;
    float m_speed;
    float m_magicAttack;
    float m_magicDefense;
    int m_exp;
    string m_skill;
    string m_tex;
    
    int m_tag; //格式为 enum tag + 0到999的无重复数（就算同一个怪物也不能重复）
    
    float m_progress;
    
    RPGMonster();
    virtual ~RPGMonster();
    
    static RPGMonster* create();
    
};

class RPGSkill : public CCObject
{
    
public:
    string m_name;
    int m_mp;
    
    RPGSkill();
    virtual ~RPGSkill();
    
    static RPGSkill* create();
};

#endif /* defined(__OzgGameRPG__RPGData__) */
