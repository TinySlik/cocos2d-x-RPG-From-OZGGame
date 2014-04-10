//
//  RPGMapStatusMenuLayer.h
//  OzgGameRPG
//
//  Created by ozg on 14-4-10.
//
//

#ifndef __OzgGameRPG__RPGMapStatusMenuLayer__
#define __OzgGameRPG__RPGMapStatusMenuLayer__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "CppSQLite3.h"
#include "JsonBox.h"
#include "RPGMapRoleSprite.h"
#include "RPGBaseSceneLayer.h"

USING_NS_CC;
USING_NS_CC_EXT;

enum RPGMapStatusMenuLayerTag
{
    kRPGMapStatusMenuLayerTagBg = 1,
    kRPGMapStatusMenuLayerTagMainMenu = 2,
    kRPGMapStatusMenuLayerTagMainMenuBack = 12,
    kRPGMapStatusMenuLayerTagPlayerSkillTable = 25,
    kRPGMapStatusMenuLayerTagMainMenuPlayer = 100
    
};

class RPGMapStatusMenuLayer : public CCLayerColor, CCTableViewDelegate, CCTableViewDataSource
{
    
private:
        
    bool m_statusIsDefault; //是否为第一次进入状态界面，如果是的话就不播放效果音了
    
    CCDictionary* m_stringList;
    CppSQLite3DB* m_db;
    
    CCArray* m_skillList; //状态界面的技能列表
    
    void onMenu(cocos2d::CCObject *pObject);
    void setStatusPlayer(int dataId);
    void removeAllPlayerLab(); //每次点击查看状态的时候都要将正在显示的lab部分清理掉
    
public:
    
    RPGMapStatusMenuLayer();
    virtual ~RPGMapStatusMenuLayer();
    
    bool init(CCDictionary* stringList, CppSQLite3DB* db, float width, float height);
    static RPGMapStatusMenuLayer* create(CCDictionary* stringList, CppSQLite3DB* db, float width, float height);
    
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

#endif /* defined(__OzgGameRPG__RPGMapStatusMenuLayer__) */
