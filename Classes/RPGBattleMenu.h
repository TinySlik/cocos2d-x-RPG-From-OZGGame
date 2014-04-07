//
//  RPGBattleMenu.h
//  OzgGameRPG
//
//  Created by 欧志刚 on 14-4-6.
//
//

#ifndef __OzgGameRPG__RPGBattleMenu__
#define __OzgGameRPG__RPGBattleMenu__

#include "cocos2d.h"
#include "RPGBattleSceneLayer.h"

USING_NS_CC;

enum RPGBattleMenuTag
{
    kRPGBattleMenuTagBg = 99,
    kRPGBattleMenuTagAttack = 2,
    kRPGBattleMenuTagSkill = 4,
    kRPGBattleMenuTagItems = 5,
    kRPGBattleMenuTagEscape = 6
    
};

class RPGBattleMenu : public CCMenu
{
    
private:
    CCNode* m_parentNode;
    
    CCDictionary* m_stringList;
    
    RPGPlayer* m_playerData;
    
    void createMenuItem(float x, float y, CCString *text, RPGBattleMenuTag tag);
    void onMenu(CCObject *pObject);
    
public:
    
    RPGBattleMenu();
    virtual ~RPGBattleMenu();
    bool initWithParentNode(CCDictionary* stringList, CCNode* parentNode, RPGPlayer* playerData);
    static RPGBattleMenu* createWithParentNode(CCDictionary* stringList, CCNode* parentNode, RPGPlayer* playerData);
    
    void showMenu(); //显示战斗菜单
    void hideMenu(); //隐藏战斗菜单
    
};

#endif /* defined(__OzgGameRPG__RPGBattleMenu__) */
