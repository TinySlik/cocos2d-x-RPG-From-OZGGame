//
//  RPGMapMenuLayer.h
//  OzgGameRPG
//
//  Created by ozg on 14-3-18.
//
//

#ifndef __OzgGameRPG__RPGMapMenuLayer__
#define __OzgGameRPG__RPGMapMenuLayer__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "CppSQLite3.h"
#include "JsonBox.h"
#include "RPGMapRoleSprite.h"
#include "RPGBaseSceneLayer.h"

USING_NS_CC;
USING_NS_CC_EXT;

enum RPGMapMenuLayerTag
{
    kRPGMapMenuLayerTagMainBg = 1,
    kRPGMapMenuLayerTagMainMenu = 2,
    kRPGMapMenuLayerTagMainMenuItems = 3,
    kRPGMapMenuLayerTagMainMenuEquip = 4,
    kRPGMapMenuLayerTagMainMenuSkill = 5,
    kRPGMapMenuLayerTagMainMenuStatus = 6,
    kRPGMapMenuLayerTagMainMenuClose = 7,
    kRPGMapMenuLayerTagMainMenuExit = 8,
    kRPGMapMenuLayerTagHandCursor = 22,
    kRPGMapMenuLayerTagDialogExitOK = 9,
    kRPGMapMenuLayerTagDialogCancel = 10,
    kRPGMapMenuLayerTagDialog = 11,
    kRPGMapMenuLayerTagStatusPlayerName = 13,
    kRPGMapMenuLayerTagStatusPlayerLevel = 14,
    kRPGMapMenuLayerTagStatusPlayerHP = 15,
    kRPGMapMenuLayerTagStatusPlayerMP = 16,
    kRPGMapMenuLayerTagStatusPlayerAttack = 17,
    kRPGMapMenuLayerTagStatusPlayerDefense = 18,
    kRPGMapMenuLayerTagStatusPlayerSpeed = 19,
    kRPGMapMenuLayerTagStatusPlayerSkillAttack = 20,
    kRPGMapMenuLayerTagStatusPlayerSkillDefense = 21,
    kRPGMapMenuLayerTagStatusPlayerEquipArms = 23,
    kRPGMapMenuLayerTagStatusPlayerEquipArmor = 24,
    kRPGMapMenuLayerTagDefualtPlayerTex = 200,
    kRPGMapMenuLayerTagDefualtPlayerName = 300,
    kRPGMapMenuLayerTagDefualtPlayerLevel = 400,
    kRPGMapMenuLayerTagDefualtPlayerHP = 500,
    kRPGMapMenuLayerTagDefualtPlayerMP = 600
    
};

class RPGMapMenuLayer : public CCLayerColor
{
    
private:
        
    CCDictionary* m_stringList;
    CppSQLite3DB* m_db;
    
    float m_width;
    float m_height;
        
    void onMenu(CCObject *pObject);
    void onDialog(CCObject *pObject);
    
    void createMenuItem(float x, float y, CCString *text, RPGMapMenuLayerTag tag);
    
    void setMainMenuDefault(); //显示右边的菜单
        
    void setDefaultInterface(); //显示左边的界面
        
public:
    
    RPGMapMenuLayer();
    virtual ~RPGMapMenuLayer();
    bool init(CCDictionary* stringList, CppSQLite3DB* db, float width, float height);
    static RPGMapMenuLayer* create(CCDictionary* stringList, CppSQLite3DB* db, float width, float height);
        
};

#endif /* defined(__OzgGameRPG__RPGMapMenuLayer__) */
