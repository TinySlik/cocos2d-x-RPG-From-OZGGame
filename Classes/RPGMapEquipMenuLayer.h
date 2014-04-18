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
#include "RPGResultsLogic.h"

USING_NS_CC;
USING_NS_CC_EXT;

enum RPGMapEquipMenuLayerTag
{
    kRPGMapEquipMenuLayerTagMainMenu = 1,
    kRPGMapEquipMenuLayerTagMainMenuBack = 2,
    kRPGMapEquipMenuLayerTagBg = 3,
    kRPGMapEquipMenuLayerTagMainMenuPlayer = 100,
    kRPGMapEquipMenuLayerTagName = 13,
    kRPGMapEquipMenuLayerTagAttack = 17,
    kRPGMapEquipMenuLayerTagDefense = 18,
    kRPGMapEquipMenuLayerTagSpeed = 19,
    kRPGMapEquipMenuLayerTagSkillAttack = 20,
    kRPGMapEquipMenuLayerTagSkillDefense = 21,
    kRPGMapEquipMenuLayerTagEquipArms = 23,
    kRPGMapEquipMenuLayerTagEquipArmor = 24,
    kRPGMapEquipMenuLayerTagBtnRemoveArms = 25,
    kRPGMapEquipMenuLayerTagBtnRemoveArmor = 26,
    kRPGMapEquipMenuLayerTagEquipTable = 27,
    kRPGMapEquipMenuLayerTagBtnEquipItems = 10000
};

class RPGMapEquipMenuLayer : public CCLayerColor, CCTableViewDelegate, CCTableViewDataSource
{
    
private:
    CCDictionary* m_stringList;
    CppSQLite3DB* m_db;
    
    CCArray* m_itemsList;
    
    int m_selectedPlayerId; //选中的player id
    
    bool m_isDefault; //是否为第一次进入装备界面，如果是的话就不播放效果音了
    
    void loadItemsData();
    void onMenu(cocos2d::CCObject *pObject);
    
    void setPlayerEquip(int dataId);
    
    void removeAllPlayerChildren();
    
    void onButton(CCObject* pSender, CCControlEvent event);
    
public:
    
    RPGMapEquipMenuLayer();
    virtual ~RPGMapEquipMenuLayer();
    
    bool init(CCDictionary* stringList, CppSQLite3DB* db, float width, float height);
    static RPGMapEquipMenuLayer* create(CCDictionary* stringList, CppSQLite3DB* db, float width, float height);
    
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

#endif /* defined(__OzgGameRPG__RPGMapEquipMenuLayer__) */
