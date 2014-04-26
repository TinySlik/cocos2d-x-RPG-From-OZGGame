//
//  RPGData.cpp
//  OzgGameRPG
//
//  Created by ozg on 14-4-5.
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

RPGSkillBtnData* RPGSkillBtnData::create()
{
    RPGSkillBtnData *obj = new RPGSkillBtnData();
    if(obj)
    {
        obj->autorelease();
        return obj;
    }
    CC_SAFE_DELETE(obj);
    return NULL;
}

//RPGSkillBtnData end

RPGItems::RPGItems()
{
    
}

RPGItems::~RPGItems()
{
    CCLog("RPGItems 释放");
}

RPGItems* RPGItems::create()
{
    RPGItems *obj = new RPGItems();
    if(obj)
    {
        obj->autorelease();
        return obj;
    }
    CC_SAFE_DELETE(obj);
    return NULL;
}

//RPGItems end

RPGExistingItems::RPGExistingItems()
{
    
}

RPGExistingItems::~RPGExistingItems()
{
    CCLog("RPGExistingItems 释放");
}

RPGExistingItems* RPGExistingItems::create()
{
    RPGExistingItems *obj = new RPGExistingItems();
    if(obj)
    {
        obj->autorelease();
        return obj;
    }
    CC_SAFE_DELETE(obj);
    return NULL;
}

//RPGExistingItems end

RPGSaveData::RPGSaveData()
{
    
}

RPGSaveData::~RPGSaveData()
{
    CCLog("RPGSaveData 释放");
}

RPGSaveData* RPGSaveData::create()
{
    RPGSaveData *obj = new RPGSaveData();
    if(obj)
    {
        obj->autorelease();
        return obj;
    }
    CC_SAFE_DELETE(obj);
    return NULL;
}

//RPGSaveData
