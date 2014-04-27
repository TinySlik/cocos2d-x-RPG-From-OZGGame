//
//  RPGBattleMenu.h
//  OzgGameRPG
//
//  Created by ozg on 14-4-6.
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
    kRPGBattleMenuTagSeparate = 78,
    kRPGBattleMenuTagNameLab = 79,
    kRPGBattleMenuTagAttack = 2,
    kRPGBattleMenuTagSkill = 4,
    kRPGBattleMenuTagItems = 5,
    kRPGBattleMenuTagEscape = 6,
    kRPGBattleMenuTagCancel = 7,
    kRPGBattleMenuTagCancel2 = 9, //这个选择了一个技能或道具后，到了选择目标对象的一步，使用到的取消按钮
    kRPGBattleMenuTagSelect = 8
    
};

class RPGBattleMenu : public CCMenu, CCTableViewDelegate, CCTableViewDataSource
{
    
private:
    CppSQLite3DB* m_db;
    CCNode* m_parentNode;
    
    CCDictionary* m_stringList;
    
    CCArray* m_tableItems; //技能数据或道具数据，需要在RPGBattleSceneLayer里面将数据加载进来
    
    void createMenuItem(float x, float y, CCString *text, RPGBattleMenuTag tag);
    void onMenu(CCObject *pObject);
    
    void onButton(CCObject* pSender, CCControlEvent event); //技能列表和道具列表的item点击了之后触发
    
public:
    
    RPGPlayer* m_playerData;
    
    int m_selectedMenuTag; //选中了哪个项(攻击、技能、道具)
        
    RPGBattleMenu();
    virtual ~RPGBattleMenu();
    bool initWithParentNode(CCDictionary* stringList, CppSQLite3DB* db, CCNode* parentNode, RPGPlayer* playerData);
    static RPGBattleMenu* createWithParentNode(CCDictionary* stringList, CppSQLite3DB* db, CCNode* parentNode, RPGPlayer* playerData);
    
    void showMenu(); //显示战斗菜单
    void hideMenu(); //隐藏战斗菜单
    
    //CCTableViewDelegate继承自CCScrollViewDelegate
    virtual void scrollViewDidScroll(CCScrollView* scrollView);
    virtual void scrollViewDidZoom(CCScrollView* scrollView);
    
    //点击哪个cell
    virtual void tableCellTouched(CCTableView* tableView, CCTableViewCell* cell);
    //每个cell的size
    virtual CCSize cellSizeForTable(CCTableView *tableView);
    //生成cell
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *tableView, unsigned int idx);
    //cell的数量
    virtual unsigned int numberOfCellsInTableView(CCTableView *tableView);
};

#endif /* defined(__OzgGameRPG__RPGBattleMenu__) */
