//
//  RPGMapChoicePlayerMenuLayer.h
//  OzgGameRPG
//
//  Created by ozg on 14-4-10.
//
//

/*
 //test
 RPGMapChoicePlayerMenuLayer *choicePlayerMenuLayer = RPGMapChoicePlayerMenuLayer::create(&this->m_db, this, callfuncO_selector(RPGMapSceneLayer::test), 960, 640);
 this->addChild(choicePlayerMenuLayer);
 
 void RPGMapSceneLayer::test(CCObject* obj)
 {
 CCLog("aaa");
 }
 */

#ifndef __OzgGameRPG__RPGMapChoicePlayerMenuLayer__
#define __OzgGameRPG__RPGMapChoicePlayerMenuLayer__

#include "cocos2d.h"
#include "RPGBaseSceneLayer.h"
#include "RPGBaseSceneLayer.h"
#include "OzgCCUtility.h"

USING_NS_CC;

enum RPGMapChoicePlayerMenuLayerTag
{
    kRPGMapChoicePlayerMenuLayerTagMainMenu = 1,
    kRPGMapChoicePlayerMenuLayerTagMainMenuBack = 4,
    kRPGMapChoicePlayerMenuLayerTagBg = 2,
    kRPGMapChoicePlayerMenuLayerTagTitleLab = 3,
    kRPGMapChoicePlayerMenuLayerTagMainMenuPlayer = 100
};

class RPGMapChoicePlayerMenuLayer : public CCLayerColor
{
    
private:
    
    CppSQLite3DB* m_db;
    CCObject* m_target;
    
    void onMenu(CCObject *pObject);
    
public:
    
    RPGMapChoicePlayerMenuLayer();
    virtual ~RPGMapChoicePlayerMenuLayer();
    
    bool init(CppSQLite3DB *db, CCString* titleLab, CCObject* target, SEL_CallFuncO selector, float width, float height);
    static RPGMapChoicePlayerMenuLayer* create(CppSQLite3DB *db, CCString* titleLab, CCObject* target, SEL_CallFuncO selector, float width, float height);
    
    void setHidden(bool isHidden);
    
};

#endif /* defined(__OzgGameRPG__RPGMapChoicePlayerMenuLayer__) */
