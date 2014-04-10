//
//  RPGMapItemsMenuLayer.h
//  OzgGameRPG
//
//  Created by ozg on 14-4-10.
//
//

#ifndef __OzgGameRPG__RPGMapItemsMenuLayer__
#define __OzgGameRPG__RPGMapItemsMenuLayer__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "CppSQLite3.h"
#include "JsonBox.h"
#include "RPGMapRoleSprite.h"
#include "RPGBaseSceneLayer.h"

USING_NS_CC;
USING_NS_CC_EXT;

enum RPGMapItemsMenuLayerTag
{
    kRPGMapItemsMenuLayerTagMainMenu = 1,
    kRPGMapItemsMenuLayerTagMainMenuBack = 2,
    kRPGMapItemsMenuLayerTagBg = 3
    
};

class RPGMapItemsMenuLayer : public CCLayerColor//, CCTableViewDelegate, CCTableViewDataSource
{
    
private:
    CCDictionary* m_stringList;
    CppSQLite3DB* m_db;
    
    void onMenu(cocos2d::CCObject *pObject);
    
public:
    
    RPGMapItemsMenuLayer();
    virtual ~RPGMapItemsMenuLayer();
    
    bool init(CCDictionary* stringList, CppSQLite3DB* db, float width, float height);
    static RPGMapItemsMenuLayer* create(CCDictionary* stringList, CppSQLite3DB* db, float width, float height);

};

#endif /* defined(__OzgGameRPG__RPGMapItemsMenuLayer__) */
