//
//  RPGBattleMonsterSprite.cpp
//  OzgGameRPG
//
//  Created by ozg on 14-4-4.
//
//

#include "RPGBattleMonsterSprite.h"

RPGBattleMonsterSprite::RPGBattleMonsterSprite()
{

}

RPGBattleMonsterSprite::~RPGBattleMonsterSprite()
{

    CCLog("RPGBattleMonsterSprite 释放");
}

bool RPGBattleMonsterSprite::initWithMonsterData(RPGMonster* data)
{
    if(CCSprite::initWithSpriteFrameName(data->m_tex.c_str()))
    {
        this->m_data = data;
        
        return true;
    }
    return false;
}

RPGBattleMonsterSprite* RPGBattleMonsterSprite::createWithMonsterData(RPGMonster* data)
{
    RPGBattleMonsterSprite *obj = new RPGBattleMonsterSprite();
    if(obj && obj->initWithMonsterData(data))
    {
        obj->autorelease();
        return obj;
    }
    CC_SAFE_DELETE(obj);
    return NULL;
}
