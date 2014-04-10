//
//  RPGMapEquipMenuLayer.h
//  OzgGameRPG
//
//  Created by ozg on 14-4-10.
//
//

#ifndef __OzgGameRPG__RPGMapEquipMenuLayer__
#define __OzgGameRPG__RPGMapEquipMenuLayer__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "CppSQLite3.h"
#include "JsonBox.h"
#include "RPGMapRoleSprite.h"
#include "RPGBaseSceneLayer.h"

USING_NS_CC;
USING_NS_CC_EXT;

enum RPGMapEquipMenuLayerTag
{
    kRPGMapEquipMenuLayerTagMainMenu = 1,
    kRPGMapEquipMenuLayerTagMainMenuBack = 2,
    kRPGMapEquipMenuLayerTagBg = 3
    
};

class RPGMapEquipMenuLayer : public CCLayerColor//, CCTableViewDelegate, CCTableViewDataSource
{
    
private:
    CCDictionary* m_stringList;
    CppSQLite3DB* m_db;
    
    void onMenu(cocos2d::CCObject *pObject);
    
public:
    
    RPGMapEquipMenuLayer();
    virtual ~RPGMapEquipMenuLayer();
    
    bool init(CCDictionary* stringList, CppSQLite3DB* db, float width, float height);
    static RPGMapEquipMenuLayer* create(CCDictionary* stringList, CppSQLite3DB* db, float width, float height);
    
};

#endif /* defined(__OzgGameRPG__RPGMapEquipMenuLayer__) */
