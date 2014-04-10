//
//  RPGMapSkillMenuLayer.h
//  OzgGameRPG
//
//  Created by ozg on 14-4-10.
//
//

#ifndef __OzgGameRPG__RPGMapSkillMenuLayer__
#define __OzgGameRPG__RPGMapSkillMenuLayer__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "CppSQLite3.h"
#include "JsonBox.h"
#include "RPGMapRoleSprite.h"
#include "RPGBaseSceneLayer.h"

USING_NS_CC;
USING_NS_CC_EXT;

enum RPGMapSkillMenuLayerTag
{
    kRPGMapSkillMenuLayerTagMainMenu = 1,
    kRPGMapSkillMenuLayerTagMainMenuBack = 2,
    kRPGMapSkillMenuLayerTagBg = 3
    
};

class RPGMapSkillMenuLayer : public CCLayerColor//, CCTableViewDelegate, CCTableViewDataSource
{
    
private:
    CCDictionary* m_stringList;
    CppSQLite3DB* m_db;
    
    void onMenu(cocos2d::CCObject *pObject);
    
public:
    
    RPGMapSkillMenuLayer();
    virtual ~RPGMapSkillMenuLayer();
    
    bool init(CCDictionary* stringList, CppSQLite3DB* db, float width, float height);
    static RPGMapSkillMenuLayer* create(CCDictionary* stringList, CppSQLite3DB* db, float width, float height);
    
};

#endif /* defined(__OzgGameRPG__RPGMapSkillMenuLayer__) */
