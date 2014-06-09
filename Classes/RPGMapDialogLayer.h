//
//  RPGMapDialogLayer.h
//  OzgGameRPG
//
//  Created by ozg on 14-3-31.
//
//

#ifndef __OzgGameRPG__RPGMapDialogLayer__
#define __OzgGameRPG__RPGMapDialogLayer__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "RPGMapNPCRoleSprite.h"
#include "CppSQLite3.h"

USING_NS_CC;
USING_NS_CC_EXT;

using namespace std;

enum RPGMapDialogLayerTag {
    kRPGMapDialogLayerTagBtnBuy = 1,
    kRPGMapDialogLayerTagBtnSell = 2,
    kRPGMapDialogLayerTagBtnInn = 3,
    kRPGMapDialogLayerTagBtnCancel = 4,
    kRPGMapDialogLayerTagShopMenu = 8,
    kRPGMapDialogLayerTagShopMenuItems = 9,
    kRPGMapDialogLayerTagShopMenuArms = 10,
    kRPGMapDialogLayerTagShopMenuArmor = 11,
    kRPGMapDialogLayerTagShopMenuAll = 12,
    kRPGMapDialogLayerTagShopMenuCancel = 13,
    kRPGMapDialogLayerTagBgMask = 5,
    kRPGMapDialogLayerTagContentLab = 6,
    kRPGMapDialogLayerTagAlertDialog = 7,
    kRPGMapDialogLayerTagAlertDialogTable = 14,
    kRPGMapDialogLayerTagAlertDialogRight = 15,
    kRPGMapDialogLayerTagAlertDialogRightGold = 16,
    kRPGMapDialogLayerTagAlertDialogRightDescription = 17
    
};

enum RPGMapDialogLayerShopMode {
    kRPGMapDialogLayerShopModeBuy = 1,
    kRPGMapDialogLayerShopModeSell = 2
};

class RPGMapDialogLayer : public CCLayer, CCTableViewDelegate, CCTableViewDataSource
{
    
private:
    RPGMapNPCRoleSprite* m_npc;
    CCDictionary *m_stringList;
    CppSQLite3DB* m_db;
    
    CCArray* m_shopData;
    
    RPGMapDialogLayerShopMode m_shopMode;
    
    void onButton(CCObject* pSender, CCControlEvent event);
    void onButtonItemsName(CCObject* pSender, CCControlEvent event); //在物品列表里面点击了物品名称
    void onButtonItems(CCObject* pSender, CCControlEvent event); //在物品列表里面点击了买入卖出
    
    void onMenu(CCObject *pObject);
    
    void cancelScene(); //退出场景
    void inn(); //执行住宿
    void innEnd(); //住宿的动作完成后执行
    void buy(); //点击了买入
    void sell(); //点了了卖出
    void shopCommon(); //显示买入卖出的列表对话框
    void showShopList(RPGMapDialogLayerTag tag);
    void showShopDefault(); //显示商店的买入卖出的对话框
    
public:
    
    RPGMapDialogLayer();
    virtual ~RPGMapDialogLayer();
    bool init(CCDictionary *stringList, CppSQLite3DB* db, RPGMapNPCRoleSprite* npc);
    static RPGMapDialogLayer* create(CCDictionary *stringList, CppSQLite3DB* db, RPGMapNPCRoleSprite* npc);
    
    bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
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

#endif /* defined(__OzgGameRPG__RPGMapDialogLayer__) */
