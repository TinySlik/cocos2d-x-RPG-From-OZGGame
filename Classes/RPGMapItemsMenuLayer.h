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
#include "RPGMapChoicePlayerMenuLayer.h"

USING_NS_CC;
USING_NS_CC_EXT;

enum RPGMapItemsMenuLayerTag
{
    kRPGMapItemsMenuLayerTagMainMenu = 1,
    kRPGMapItemsMenuLayerTagMainMenuBack = 2,
    kRPGMapItemsMenuLayerTagBg = 3,
    kRPGMapItemsMenuLayerTagItemListTable = 4,
    kRPGMapItemsMenuLayerTagDialogOK = 5,
    kRPGMapItemsMenuLayerTagDialog = 86,
    kRPGMapItemsMenuLayerTagBtnDiscard = 85
};

class RPGMapItemsMenuLayer : public CCLayerColor, CCTableViewDelegate, CCTableViewDataSource
{
    
private:
    CCDictionary* m_stringList;
    CppSQLite3DB* m_db;
    
    CCArray* m_itemsList;
    
    RPGExistingItems* m_selectedItems; //选中的道具
    
    void loadItemsData(); //读取道具的数据
    void onMenu(cocos2d::CCObject *pObject);
    void onDialog(cocos2d::CCObject *pObject);
    
    void onButton(CCObject* pSender, CCControlEvent event); //点击了items列表的其中一个后触发
    void onChoicePlayer(CCObject* pObject); //在player选择框中，选中一个player后触发
    
public:
    
    RPGMapItemsMenuLayer();
    virtual ~RPGMapItemsMenuLayer();
    
    bool init(CCDictionary* stringList, CppSQLite3DB* db, float width, float height);
    static RPGMapItemsMenuLayer* create(CCDictionary* stringList, CppSQLite3DB* db, float width, float height);
    
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

#endif /* defined(__OzgGameRPG__RPGMapItemsMenuLayer__) */
