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
#include "RPGResultsLogic.h"

USING_NS_CC;
USING_NS_CC_EXT;

enum RPGMapSkillMenuLayerTag
{
    kRPGMapSkillMenuLayerTagMainMenu = 1,
    kRPGMapSkillMenuLayerTagMainMenuBack = 2,
    kRPGMapSkillMenuLayerTagBg = 3,
    kRPGMapSkillMenuLayerTagItemListTable = 4,
    kRPGMapSkillMenuLayerTagMainMenuPlayer = 100,
    kRPGMapSkillMenuLayerTagDialogOK = 5,
    kRPGMapSkillMenuLayerTagDialog = 86
};

class RPGMapSkillMenuLayer : public CCLayerColor, CCTableViewDelegate, CCTableViewDataSource
{
    
private:
    CCDictionary* m_stringList;
    CppSQLite3DB* m_db;
    
    CCArray* m_skillList;
    
    bool m_isDefault; //是否为第一次进入技能界面，如果是的话就不播放效果音了
    
    int m_selectedPlayerId; //选中上面的player的id
    RPGSkill* m_selectedSkill; //选中的技能
    
    void onMenu(cocos2d::CCObject *pObject);
    
    void setPlayerSkill(int dataId);
    
    void onButton(CCObject* pSender, CCControlEvent event); //点击了items列表的其中一个后触发
    void onChoicePlayer(CCObject* pObject); //在player选择框中，选中一个player后触发
    void onDialog(cocos2d::CCObject *pObject);
    
public:
    
    RPGMapSkillMenuLayer();
    virtual ~RPGMapSkillMenuLayer();
    
    bool init(CCDictionary* stringList, CppSQLite3DB* db, float width, float height);
    static RPGMapSkillMenuLayer* create(CCDictionary* stringList, CppSQLite3DB* db, float width, float height);
    
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

#endif /* defined(__OzgGameRPG__RPGMapSkillMenuLayer__) */
