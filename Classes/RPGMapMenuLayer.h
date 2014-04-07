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
    kRPGMapMenuLayerTagMainMenuStatusBack = 12,
    kRPGMapMenuLayerTagMainMenuPlayerStatus = 100,
    kRPGMapMenuLayerTagHandCursor = 22,
    kRPGMapMenuLayerTagConfirmExitOK = 9,
    kRPGMapMenuLayerTagConfirmCancel = 10,
    kRPGMapMenuLayerTagConfirm = 11,
    kRPGMapMenuLayerTagStatusPlayerName = 13,
    kRPGMapMenuLayerTagStatusPlayerLevel = 14,
    kRPGMapMenuLayerTagStatusPlayerHP = 15,
    kRPGMapMenuLayerTagStatusPlayerMP = 16,
    kRPGMapMenuLayerTagStatusPlayerAttack = 17,
    kRPGMapMenuLayerTagStatusPlayerDefense = 18,
    kRPGMapMenuLayerTagStatusPlayerSpeed = 19,
    kRPGMapMenuLayerTagStatusPlayerMagicAttack = 20,
    kRPGMapMenuLayerTagStatusPlayerMagicDefense = 21,
    kRPGMapMenuLayerTagStatusPlayerEquipArms = 23,
    kRPGMapMenuLayerTagStatusPlayerEquipArmor = 24,
    kRPGMapMenuLayerTagStatusPlayerSkillTable = 25,
    kRPGMapMenuLayerTagDefualtPlayerTex = 200,
    kRPGMapMenuLayerTagDefualtPlayerName = 300,
    kRPGMapMenuLayerTagDefualtPlayerLevel = 400,
    kRPGMapMenuLayerTagDefualtPlayerHP = 500,
    kRPGMapMenuLayerTagDefualtPlayerMP = 600
    
};

class RPGMapMenuLayer : public CCLayerColor, public CCTableViewDelegate, public CCTableViewDataSource
{
    
private:
    
    CCArray* m_skillList; //状态界面的技能列表
    
    CCDictionary* m_stringList;
    CppSQLite3DB* m_db;
    
    float m_width;
    float m_height;
    
    bool m_statusIsDefault; //是否为第一次进入状态界面，如果是的话就不播放效果音了
    
    void onMenu(CCObject *pObject);
    void createMenuItem(float x, float y, CCString *text, RPGMapMenuLayerTag tag);
    
    void setMainMenuDefault(); //默认
    void setMainMenuStatus(); //状态
    
    void setDefaultInterface(); //显示默认界面
    void removeDefaultInterface(); //清理默认界面
    
    void setStatusInterface(); //显示状态界面
    void setStatusPlayer(int dataId); //显示状态界面的player数据部分
    void removeAllStatusPlayerLab(); //清理所有状态界面的lab
        
public:
    
    RPGMapMenuLayer();
    virtual ~RPGMapMenuLayer();
    bool init(CCDictionary* stringList, CppSQLite3DB* db, float width, float height);
    static RPGMapMenuLayer* create(CCDictionary* stringList, CppSQLite3DB* db, float width, float height);
    
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

#endif /* defined(__OzgGameRPG__RPGMapMenuLayer__) */
