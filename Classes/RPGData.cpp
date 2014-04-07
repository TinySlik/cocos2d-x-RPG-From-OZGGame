//
//  RPGData.cpp
//  OzgGameRPG
//
//  Created by 欧志刚 on 14-4-5.
//
//

#include "RPGData.h"

RPGPlayer::RPGPlayer()
{

}

RPGPlayer::~RPGPlayer()
{
    CCLog("RPGPlayer 释放");
    
}

RPGPlayer* RPGPlayer::create()
{
    RPGPlayer *obj = new RPGPlayer();
    if(obj)
    {
        obj->autorelease();
        return obj;
    }
    CC_SAFE_DELETE(obj);
    return NULL;
}

//RPGPlayer end

RPGMonster::RPGMonster()
{

}

RPGMonster::~RPGMonster()
{
    CCLog("RPGMonster 释放");
    
}

RPGMonster* RPGMonster::create()
{
    RPGMonster *obj = new RPGMonster();
    if(obj)
    {
        obj->autorelease();
        return obj;
    }
    CC_SAFE_DELETE(obj);
    return NULL;
}

//RPGMonster end

RPGSkill::RPGSkill()
{
    
}

RPGSkill::~RPGSkill()
{
//    CCLog("RPGSkill 释放");
    
}

RPGSkill* RPGSkill::create()
{
    RPGSkill *obj = new RPGSkill();
    if(obj)
    {
        obj->autorelease();
        return obj;
    }
    CC_SAFE_DELETE(obj);
    return NULL;
}

//RPGSkill end
