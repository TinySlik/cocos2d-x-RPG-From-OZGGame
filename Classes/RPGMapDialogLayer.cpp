//
//  RPGMapDialogLayer.cpp
//  OzgGameRPG
//
//  Created by ozg on 14-3-31.
//
//

#include "RPGMapDialogLayer.h"
#include "RPGMapSceneLayer.h"

#define INN_UPDATE "update player set hp = max_hp, mp = max_mp where id = %i" //住宿时，回复数值时使用

#define ITEMS_QUERY1 "select * from items order by id asc, type desc"
#define ITEMS_QUERY2 "select * from items where type = 1 order by id asc, type desc"
#define ITEMS_QUERY3 "select * from items where type = 2 order by id asc, type desc"
#define ITEMS_QUERY4 "select * from items where type = 3 order by id asc, type desc"

#define ITEMS_EXISTING_QUERY1 "select i.*, ie.total from items_existing as ie inner join items as i on ie.id = i.id order by i.id asc"
#define ITEMS_EXISTING_QUERY2 "select i.*, ie.total from items_existing as ie inner join items as i on ie.id = i.id where i.type = 1 order by i.id asc"
#define ITEMS_EXISTING_QUERY3 "select i.*, ie.total from items_existing as ie inner join items as i on ie.id = i.id where i.type = 2 order by i.id asc"
#define ITEMS_EXISTING_QUERY4 "select i.*, ie.total from items_existing as ie inner join items as i on ie.id = i.id where i.type = 3 order by i.id asc"

#define CHECK_ITEMS "select count(id) from items_existing where id = %i"

#define ITEMS_BUY_ADD "insert into items_existing (id, total) values(%i, 1)"
#define ITEMS_BUY_UPDATE "update items_existing set total = total + 1 where id = %i"

#define ITEMS_SELL_ALL "delete from items_existing where id = %i"
#define ITEMS_SELL_UPDATE "update items_existing set total = total - 1 where id = %i"

RPGMapDialogLayer::RPGMapDialogLayer()
{

}

RPGMapDialogLayer::~RPGMapDialogLayer()
{
    this->m_stringList->release();
    
    this->m_npc->startAutoMove(); //继续执行自动移动
    this->m_npc->release();
    
    this->m_shopData->release();
    
    CCLog("RPGMapDialogLayer 释放");
}

bool RPGMapDialogLayer::init(CCDictionary *stringList, CppSQLite3DB* db, RPGMapNPCRoleSprite* npc)
{
    if(CCLayer::init())
    {
        this->m_stringList = stringList;
        this->m_stringList->retain();
        
        this->m_db = db;
        
        this->m_npc = npc;
        this->m_npc->retain();
        
        this->m_shopData = new CCArray();
        this->m_shopData->init();
        
        CCTMXTiledMap *mainBg = CCTMXTiledMap::create(CCString::createWithFormat("dialog_%s.tmx", CCUserDefault::sharedUserDefault()->getStringForKey(GAME_STYLE).c_str())->getCString());
        float x = (CCDirector::sharedDirector()->getWinSize().width - mainBg->getContentSize().width) / 2.0f;
        float y = 40;
        mainBg->setPosition(ccp(x, y));
        this->addChild(mainBg);
        
        string text = "       ";
        text.append(this->m_npc->m_content->getCString());
        CCLabelTTF *contentLab = CCLabelTTF::create(text.c_str(), "Arial", 25, CCSizeMake(mainBg->getContentSize().width - 20.0f, mainBg->getContentSize().height - 40.0f), kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
        contentLab->setPosition(ccp(mainBg->getContentSize().width / 2.0f + 35.0f, 125));
        contentLab->setTag(kRPGMapDialogLayerTagContentLab);
        this->addChild(contentLab);
        
        if(this->m_npc->m_dataId == GAME_MAPSHOP_NPC_ID || this->m_npc->m_dataId == GAME_MAPINN_NPC_ID)
        {
            //特定NPC对话后出现的选择
            
            //背景层
            CCLayerColor *bgMask = CCLayerColor::create(ccc4(0, 0, 0, 200));
            bgMask->setTag(kRPGMapDialogLayerTagBgMask);
            this->addChild(bgMask, 0);
            this->reorderChild(mainBg, 1);
            this->reorderChild(contentLab, 2);
            
            this->showShopDefault();
            
        }
        else
            CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, true);
        
        return true;
    }
    
    return false;
}

RPGMapDialogLayer* RPGMapDialogLayer::create(CCDictionary *stringList, CppSQLite3DB* db, RPGMapNPCRoleSprite* npc)
{
    RPGMapDialogLayer *layer = new RPGMapDialogLayer();
    if(layer && layer->init(stringList, db, npc))
    {
        layer->autorelease();
        
        return layer;
    }
    return NULL;
}

bool RPGMapDialogLayer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    
    return true;
}

void RPGMapDialogLayer::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{

}

void RPGMapDialogLayer::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    
    this->cancelScene();
}

//CCTableView

void RPGMapDialogLayer::scrollViewDidScroll(cocos2d::extension::CCScrollView *scrollView)
{
    
}

void RPGMapDialogLayer::scrollViewDidZoom(cocos2d::extension::CCScrollView *scrollView)
{
    
}

void RPGMapDialogLayer::tableCellTouched(cocos2d::extension::CCTableView *tableView, cocos2d::extension::CCTableViewCell *cell)
{
    
}

CCSize RPGMapDialogLayer::cellSizeForTable(cocos2d::extension::CCTableView *tableView)
{
    return CCSizeMake(tableView->getContentSize().width, 80);
}

CCTableViewCell* RPGMapDialogLayer::tableCellAtIndex(cocos2d::extension::CCTableView *tableView, unsigned int idx)
{
    CCTableViewCell *cell = tableView->dequeueCell();
    if (!cell)
    {
        cell = new CCTableViewCell();
        cell->autorelease();
    }
    else
        cell->removeAllChildrenWithCleanup(true);
    
    RPGItems *items = (RPGItems*)this->m_shopData->objectAtIndex(idx);
    
    //类别
    string typeName;
    switch (items->m_type)
    {
        case 1:
            typeName = ((CCString*)this->m_stringList->objectForKey("npc_shop_category_arms"))->getCString();
            break;
        case 2:
            typeName = ((CCString*)this->m_stringList->objectForKey("npc_shop_category_armor"))->getCString();
            break;
        case 3:
            typeName = ((CCString*)this->m_stringList->objectForKey("npc_shop_category_items"))->getCString();
            break;
        default:
            
            break;
    }
    CCLabelTTF *typeLab = CCLabelTTF::create(typeName.c_str(), "FZCuYuan-M03S", 20, CCSizeMake(120, 40), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    typeLab->setPosition(ccp(400, 0));
    typeLab->setFontFillColor(ccc3(144, 144, 144));
    cell->addChild(typeLab);
    
    if(this->m_shopMode == kRPGMapDialogLayerShopModeBuy)
    {
        //买入
        
        //名称
        CCControlButton *nameBtn = CCControlButton::create(CCString::createWithFormat("%s （%i）", items->m_name.c_str(), (int)items->m_buy)->getCString(), "FZCuYuan-M03S", 20);
        nameBtn->setPosition(ccp(150, 0));
        nameBtn->setTag(items->m_dataId);
        nameBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(RPGMapDialogLayer::onButtonItemsName), CCControlEventTouchUpInside);
        cell->addChild(nameBtn);
        
        //买入按钮
        CCControlButton *itemsBtn = CCControlButton::create(((CCString*)this->m_stringList->objectForKey("npc_dialog_buy"))->getCString(), "FZCuYuan-M03S", 20);
        itemsBtn->setPosition(ccp(550, 0));
        itemsBtn->setTag(items->m_dataId);
        itemsBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(RPGMapDialogLayer::onButtonItems), CCControlEventTouchUpInside);
        cell->addChild(itemsBtn);
    }
    else if(this->m_shopMode == kRPGMapDialogLayerShopModeSell)
    {
        //卖出
        
        //名称
        CCControlButton *nameBtn = CCControlButton::create(CCString::createWithFormat("%s （%i）", items->m_name.c_str(), (int)items->m_sell)->getCString(), "FZCuYuan-M03S", 20);
        nameBtn->setPosition(ccp(150, 0));
        nameBtn->setTag(items->m_dataId);
        nameBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(RPGMapDialogLayer::onButtonItemsName), CCControlEventTouchUpInside);
        cell->addChild(nameBtn);
        
        typeLab->setPosition(ccp(350, 0));
        
        //卖出按钮
        CCControlButton *itemsBtn = CCControlButton::create(CCString::createWithFormat("%s（剩余%i个）", ((CCString*)this->m_stringList->objectForKey("npc_dialog_sell"))->getCString(), ((RPGExistingItems*)items)->m_total)->getCString(), "FZCuYuan-M03S", 20);
        itemsBtn->setPosition(ccp(520, 0));
        itemsBtn->setTag(items->m_dataId);
        itemsBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(RPGMapDialogLayer::onButtonItems), CCControlEventTouchUpInside);
        cell->addChild(itemsBtn);
    }
    
    return cell;
}

unsigned int RPGMapDialogLayer::numberOfCellsInTableView(cocos2d::extension::CCTableView *tableView)
{
    
    return this->m_shopData->count();
}

//CCTableView end

//private
void RPGMapDialogLayer::onButton(cocos2d::CCObject *pSender, CCControlEvent event)
{
    SimpleAudioEngine::sharedEngine()->playEffect("audio_effect_btn.wav", false);
    
    CCControlButton *btn = (CCControlButton*)pSender;
    switch (btn->getTag())
    {
        case kRPGMapDialogLayerTagBtnBuy:
            this->buy(); //买入
            
            break;
        case kRPGMapDialogLayerTagBtnSell:
            this->sell(); //卖出
            
            break;
        case kRPGMapDialogLayerTagBtnInn:
            this->inn(); //住宿
            
            break;
        case kRPGMapDialogLayerTagBtnCancel:
            this->cancelScene(); //点击了取消
            
            break;
        default:
            break;
    }
}

void RPGMapDialogLayer::onButtonItemsName(cocos2d::CCObject *pSender, CCControlEvent event)
{
    SimpleAudioEngine::sharedEngine()->playEffect("audio_effect_btn.wav", false);
    
    CCControlButton *nameBtn = (CCControlButton*)pSender;
    
    CCObject *obj;
    CCARRAY_FOREACH(this->m_shopData, obj)
    {
        RPGItems *item = (RPGItems*)obj;
        if(nameBtn->getTag() == item->m_dataId)
        {
            CCTMXTiledMap *alertDialogRight = (CCTMXTiledMap*)this->getChildByTag(kRPGMapDialogLayerTagAlertDialogRight);
            
            //更新物品描述
            CCLabelTTF *descriptionLab = (CCLabelTTF*)alertDialogRight->getChildByTag(kRPGMapDialogLayerTagAlertDialogRightDescription);
            descriptionLab->setString(item->m_description.c_str());
            
            break;
        }
    }
    
}

void RPGMapDialogLayer::onButtonItems(cocos2d::CCObject *pSender, CCControlEvent event)
{
    SimpleAudioEngine::sharedEngine()->playEffect("audio_effect_btn.wav", false);
    
    CCControlButton *itemsBtn = (CCControlButton*)pSender;
    
    CCObject *obj;
    CCARRAY_FOREACH(this->m_shopData, obj)
    {
        RPGItems *item = (RPGItems*)obj;
        if(itemsBtn->getTag() == item->m_dataId)
        {
            CCLabelTTF *contentLab = (CCLabelTTF*)this->getChildByTag(kRPGMapDialogLayerTagContentLab);
            RPGSaveData *saveDataObj = loadSaveData(this->m_db);
            
            if(this->m_shopMode == kRPGMapDialogLayerShopModeBuy)
            {
                //买入
                
                //判断金钱，扣减金钱
                if(saveDataObj->m_gold < (int)item->m_buy)
                {
                    //金钱不够
                    contentLab->setString(CCString::createWithFormat("       %s", ((CCString*)this->m_stringList->objectForKey("npc_dialog_buy_msg2"))->getCString())->getCString());
                    return;
                }
                
                CCString *sql = CCString::createWithFormat(CHECK_ITEMS, item->m_dataId);
                int total = this->m_db->execScalar(sql->getCString());
                
                //越过了界限
                if(total == MAX_ITEMS_TOTAL)
                {
                    contentLab->setString(CCString::createWithFormat("       %s", ((CCString*)this->m_stringList->objectForKey("npc_dialog_buy_msg4"))->getCString())->getCString());
                    return;
                }
                
                saveDataObj->m_gold -= (int)item->m_buy;
                saveData(this->m_db, saveDataObj);
                
                //更新右上角的金钱
                CCTMXTiledMap *alertDialogRight = (CCTMXTiledMap*)this->getChildByTag(kRPGMapDialogLayerTagAlertDialogRight);
                CCLabelTTF *goldLab = (CCLabelTTF*)alertDialogRight->getChildByTag(kRPGMapDialogLayerTagAlertDialogRightGold);
                goldLab->setString(CCString::createWithFormat("%s%i", ((CCString*)this->m_stringList->objectForKey("menu_gold"))->getCString(), saveDataObj->m_gold)->getCString());
                
                if(total == 0)
                    //原来没有这个物品的话就新增
                    this->m_db->execDML(CCString::createWithFormat(ITEMS_BUY_ADD, item->m_dataId)->getCString());
                else
                    //对于已有物品就加1
                    this->m_db->execDML(CCString::createWithFormat(ITEMS_BUY_UPDATE, item->m_dataId)->getCString());
                
                //更新下面的对话框
                contentLab->setString(CCString::createWithFormat("       %s", ((CCString*)this->m_stringList->objectForKey("npc_dialog_buy_msg3"))->getCString())->getCString());
            }
            else if(this->m_shopMode == kRPGMapDialogLayerShopModeSell)
            {
                //卖出
                
                saveDataObj->m_gold += (int)item->m_sell;
                saveData(this->m_db, saveDataObj);
                
                //更新右上角的金钱
                CCTMXTiledMap *alertDialogRight = (CCTMXTiledMap*)this->getChildByTag(kRPGMapDialogLayerTagAlertDialogRight);
                CCLabelTTF *goldLab = (CCLabelTTF*)alertDialogRight->getChildByTag(kRPGMapDialogLayerTagAlertDialogRightGold);
                goldLab->setString(CCString::createWithFormat("%s%i", ((CCString*)this->m_stringList->objectForKey("menu_gold"))->getCString(), saveDataObj->m_gold)->getCString());
                
                ((RPGExistingItems*)item)->m_total--;
                if(((RPGExistingItems*)item)->m_total == 0)
                {
                    this->m_shopData->removeObject(item);
                    
                    //卖到没有了的话就删除
                    this->m_db->execDML(CCString::createWithFormat(ITEMS_SELL_ALL, item->m_dataId)->getCString());
                }
                else
                    //对于已有物品就减1
                    this->m_db->execDML(CCString::createWithFormat(ITEMS_SELL_UPDATE, item->m_dataId)->getCString());
                
                CCTMXTiledMap *alertDialog = (CCTMXTiledMap*)this->getChildByTag(kRPGMapDialogLayerTagAlertDialog);
                CCTableView *tableView = (CCTableView*)alertDialog->getChildByTag(kRPGMapDialogLayerTagAlertDialogTable);
                
                float x = tableView->getContentOffset().x;
                float y = tableView->getContentOffset().y;
                
                tableView->reloadData();
                tableView->setContentOffset(ccp(x, y));
                
                //更新下面的对话框
                contentLab->setString(CCString::createWithFormat("       %s", ((CCString*)this->m_stringList->objectForKey("npc_dialog_sell_msg2"))->getCString())->getCString());
            }
            
            break;
        }
    }
}

void RPGMapDialogLayer::onMenu(cocos2d::CCObject *pObject)
{
    SimpleAudioEngine::sharedEngine()->playEffect("audio_effect_btn.wav", false);
    
    CCMenuItem *menuItem = (CCMenuItem*)pObject;
    switch (menuItem->getTag())
    {
        case kRPGMapDialogLayerTagShopMenuItems:
            this->showShopList((RPGMapDialogLayerTag)menuItem->getTag()); //道具
            
            break;
        case kRPGMapDialogLayerTagShopMenuArms:
            this->showShopList((RPGMapDialogLayerTag)menuItem->getTag()); //武器
            
            break;
        case kRPGMapDialogLayerTagShopMenuArmor:
            this->showShopList((RPGMapDialogLayerTag)menuItem->getTag()); //防具
            
            break;
        case kRPGMapDialogLayerTagShopMenuAll:
            this->showShopList((RPGMapDialogLayerTag)menuItem->getTag()); //全部
            
            break;
        case kRPGMapDialogLayerTagShopMenuCancel:
        {
            //取消回到默认状态
            this->removeChildByTag(kRPGMapDialogLayerTagShopMenu, true);
            this->removeChildByTag(kRPGMapDialogLayerTagAlertDialog, true);
            this->removeChildByTag(kRPGMapDialogLayerTagAlertDialogRight, true);
            
            this->showShopDefault();
        }
            break;
        default:
            break;
    }
    
}

void RPGMapDialogLayer::cancelScene()
{
    CCTMXTiledMap *bgMap = (CCTMXTiledMap*)this->getParent()->getChildByTag(kRPGMapSceneLayerTagBgMap);
    
    CCMenu *mainMenu = (CCMenu*)bgMap->getChildByTag(kRPGMapSceneLayerTagMainMenu);
    mainMenu->setVisible(true);
    
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    
    ((RPGMapSceneLayer*)this->getParent())->enabledControl(true);
    
    this->removeFromParentAndCleanup(true);
}

void RPGMapDialogLayer::inn()
{
    //判断金钱，扣减金钱
    RPGSaveData *saveDataObj = loadSaveData(this->m_db);
    if(saveDataObj->m_gold < GAME_INN)
    {
        //金钱不够
        CCLabelTTF *contentLab = (CCLabelTTF*)this->getChildByTag(kRPGMapDialogLayerTagContentLab);
        contentLab->setString(CCString::createWithFormat("       %s", ((CCString*)this->m_stringList->objectForKey("npc_dialog_inn_msg"))->getCString())->getCString());
        return;
    }
    
    //播放住宿的声音
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->playEffect("audio_inn.mp3", false);
    
    saveDataObj->m_gold -= GAME_INN;
    saveData(this->m_db, saveDataObj);
    
    //回复状态
    CCArray *sqlList = CCArray::create(); //需要执行的sql
    CppSQLite3Query query = this->m_db->execQuery(PLAYER_QUERY);
    while(!query.eof())
    {
        CCString *sql = CCString::createWithFormat(INN_UPDATE, query.getIntField("id"));
        sqlList->addObject(sql);
        query.nextRow();
    }
    query.finalize();
    
    //统一执行需要变动的数据
    CCObject *obj = NULL;
    CCARRAY_FOREACH(sqlList, obj)
    {
        CCString *sql = (CCString*)obj;
        this->m_db->execDML(sql->getCString());
    }
    
    this->removeAllChildrenWithCleanup(true);
    CCLayerColor *bgMask = CCLayerColor::create(ccc4(0, 0, 0, 0));
    bgMask->setTag(kRPGMapDialogLayerTagBgMask);
    this->addChild(bgMask);
    
    CCFadeIn *act1 = CCFadeIn::create(0.5);
    CCActionInterval *act2 = act1->reverse();
    
    bgMask->runAction(CCSequence::create(act1, CCDelayTime::create(4.0), act2, CCCallFunc::create(this, callfunc_selector(RPGMapDialogLayer::innEnd)), NULL));
}

void RPGMapDialogLayer::innEnd()
{
    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
    this->cancelScene();
}

void RPGMapDialogLayer::buy()
{
    this->m_shopMode = kRPGMapDialogLayerShopModeBuy;
    this->shopCommon();
    
    CCLabelTTF *contentLab = (CCLabelTTF*)this->getChildByTag(kRPGMapDialogLayerTagContentLab);
    contentLab->setString(CCString::createWithFormat("       %s", ((CCString*)this->m_stringList->objectForKey("npc_dialog_buy_msg"))->getCString())->getCString());
}

void RPGMapDialogLayer::sell()
{
    this->m_shopMode = kRPGMapDialogLayerShopModeSell;
    this->shopCommon();
    
    CCLabelTTF *contentLab = (CCLabelTTF*)this->getChildByTag(kRPGMapDialogLayerTagContentLab);
    contentLab->setString(CCString::createWithFormat("       %s", ((CCString*)this->m_stringList->objectForKey("npc_dialog_sell_msg"))->getCString())->getCString());
}

void RPGMapDialogLayer::shopCommon()
{
    //更换对话框
    this->removeChildByTag(kRPGMapDialogLayerTagAlertDialog, true);
    
    CCTMXTiledMap *alertDialog = CCTMXTiledMap::create(CCString::createWithFormat("shop_list_%s.tmx", CCUserDefault::sharedUserDefault()->getStringForKey(GAME_STYLE).c_str())->getCString());
    alertDialog->setPosition(ccp(110, 210));
    alertDialog->setTag(kRPGMapDialogLayerTagAlertDialog);
    this->addChild(alertDialog);
    
    //右边的框
    CCTMXTiledMap *alertDialogRight = CCTMXTiledMap::create(CCString::createWithFormat("shop_menu_%s.tmx", CCUserDefault::sharedUserDefault()->getStringForKey(GAME_STYLE).c_str())->getCString());
    alertDialogRight->setPosition(ccp(760, 210));
    alertDialogRight->setTag(kRPGMapDialogLayerTagAlertDialogRight);
    this->addChild(alertDialogRight);
    
    RPGSaveData *saveDataObj = loadSaveData(this->m_db);
    addLab(alertDialogRight, kRPGMapDialogLayerTagAlertDialogRightGold, CCString::createWithFormat("%s%i", ((CCString*)this->m_stringList->objectForKey("menu_gold"))->getCString(), saveDataObj->m_gold), 18, kCCTextAlignmentLeft, ccp(225, 373));
    
    //物品描述
    CCLabelTTF *descriptionLab = CCLabelTTF::create("", "Arial", 20, CCSizeMake(alertDialogRight->getContentSize().width - 35, alertDialogRight->getContentSize().height - 120), kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
    descriptionLab->setPosition(ccp(105, 160));
    descriptionLab->setTag(kRPGMapDialogLayerTagAlertDialogRightDescription);
    alertDialogRight->addChild(descriptionLab);
    
    //右边的框 end
    
    CCMenu *shopMenu = CCMenu::create();
    shopMenu->setPosition(CCPointZero);
    shopMenu->setAnchorPoint(CCPointZero);
    shopMenu->setTag(kRPGMapDialogLayerTagShopMenu);
    this->addChild(shopMenu);
    
    //上面的4个分类按钮
    
    //全部
    CCLabelTTF *shopMenuAllLab = CCLabelTTF::create(((CCString*)this->m_stringList->objectForKey("npc_shop_category_all"))->getCString(), "Arial", 25);
    CCMenuItemLabel *shopMenuAll = CCMenuItemLabel::create(shopMenuAllLab, this, menu_selector(RPGMapDialogLayer::onMenu));
    shopMenuAll->setPosition(ccp(180, 585));
    shopMenuAll->setTag(kRPGMapDialogLayerTagShopMenuAll);
    shopMenu->addChild(shopMenuAll);
    
    //道具
    CCLabelTTF *shopMenuItemsLab = CCLabelTTF::create(((CCString*)this->m_stringList->objectForKey("npc_shop_category_items"))->getCString(), "Arial", 25);
    CCMenuItemLabel *shopMenuItems = CCMenuItemLabel::create(shopMenuItemsLab, this, menu_selector(RPGMapDialogLayer::onMenu));
    shopMenuItems->setPosition(ccp(347, 585));
    shopMenuItems->setTag(kRPGMapDialogLayerTagShopMenuItems);
    shopMenu->addChild(shopMenuItems);
    
    //武器
    CCLabelTTF *shopMenuArmsLab = CCLabelTTF::create(((CCString*)this->m_stringList->objectForKey("npc_shop_category_arms"))->getCString(), "Arial", 25);
    CCMenuItemLabel *shopMenuArms = CCMenuItemLabel::create(shopMenuArmsLab, this, menu_selector(RPGMapDialogLayer::onMenu));
    shopMenuArms->setPosition(ccp(514, 585));
    shopMenuArms->setTag(kRPGMapDialogLayerTagShopMenuArms);
    shopMenu->addChild(shopMenuArms);
    
    //防具
    CCLabelTTF *shopMenuArmorLab = CCLabelTTF::create(((CCString*)this->m_stringList->objectForKey("npc_shop_category_armor"))->getCString(), "Arial", 25);
    CCMenuItemLabel *shopMenuArmor = CCMenuItemLabel::create(shopMenuArmorLab, this, menu_selector(RPGMapDialogLayer::onMenu));
    shopMenuArmor->setPosition(ccp(681, 585));
    shopMenuArmor->setTag(kRPGMapDialogLayerTagShopMenuArmor);
    shopMenu->addChild(shopMenuArmor);
    
    //上面的4个分类按钮 end

    //返回按钮
    CCMenuItemSprite *menuCancel = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("commons_btn_back_04.png"), CCSprite::createWithSpriteFrameName("commons_btn_back_04.png"), this, menu_selector(RPGMapDialogLayer::onMenu));
    menuCancel->setPosition(ccp(40, 600));
    menuCancel->setTag(kRPGMapDialogLayerTagShopMenuCancel);
    menuCancel->setScale(0.5);
    shopMenu->addChild(menuCancel);
    
    //物品列表
    this->showShopList(kRPGMapDialogLayerTagShopMenuAll);
}

void RPGMapDialogLayer::showShopList(RPGMapDialogLayerTag tag)
{
    CCMenu *shopMenu = (CCMenu*)this->getChildByTag(kRPGMapDialogLayerTagShopMenu);
    
    CCObject *obj = NULL;
    CCARRAY_FOREACH(shopMenu->getChildren(), obj)
    {
        CCMenuItem *itemMenu = (CCMenuItem*)obj;
        
        if(itemMenu->getTag() == tag)
            itemMenu->setEnabled(false);
        else
            itemMenu->setEnabled(true);
    }

    this->m_shopData->removeAllObjects();
    
    if(this->m_shopMode == kRPGMapDialogLayerShopModeBuy)
    {
        //买入
        
        string sql;
        switch (tag)
        {
            case kRPGMapDialogLayerTagShopMenuItems:
                sql = ITEMS_QUERY4;
                break;
            case kRPGMapDialogLayerTagShopMenuArmor:
                sql = ITEMS_QUERY3;
                break;
            case kRPGMapDialogLayerTagShopMenuArms:
                sql = ITEMS_QUERY2;
                break;
            default:
                sql = ITEMS_QUERY1;
                break;
        }
        CppSQLite3Query query = this->m_db->execQuery(sql.c_str());
        while(!query.eof())
        {
            RPGItems *items = RPGItems::create();
            items->m_dataId = query.getIntField("id");
            items->m_name = query.getStringField("name_cns");
            items->m_buy = query.getFloatField("buy");
            items->m_sell = query.getFloatField("sell");
            items->m_type = query.getIntField("type");
            items->m_attack = query.getFloatField("attack");
            items->m_defense = query.getFloatField("defense");
            items->m_speed = query.getFloatField("speed");
            items->m_skillAttack = query.getFloatField("skill_attack");
            items->m_skillDefense = query.getFloatField("skill_defense");
            items->m_description = query.getStringField("description");
            
            this->m_shopData->addObject(items);
            
            query.nextRow();
        }
        query.finalize();
    }
    else if(this->m_shopMode == kRPGMapDialogLayerShopModeSell)
    {
        //卖出
        
        string sql;
        switch (tag)
        {
            case kRPGMapDialogLayerTagShopMenuItems:
                sql = ITEMS_EXISTING_QUERY4;
                break;
            case kRPGMapDialogLayerTagShopMenuArmor:
                sql = ITEMS_EXISTING_QUERY3;
                break;
            case kRPGMapDialogLayerTagShopMenuArms:
                sql = ITEMS_EXISTING_QUERY2;
                break;
            default:
                sql = ITEMS_EXISTING_QUERY1;
                break;
        }
        CppSQLite3Query query = this->m_db->execQuery(sql.c_str());
        while(!query.eof())
        {
            RPGExistingItems *items = RPGExistingItems::create();
            items->m_dataId = query.getIntField("id");
            items->m_name = query.getStringField("name_cns");
            items->m_buy = query.getIntField("buy");
            items->m_sell = query.getIntField("sell");
            items->m_type = query.getIntField("type");
            items->m_attack = query.getFloatField("attack");
            items->m_defense = query.getFloatField("defense");
            items->m_speed = query.getFloatField("speed");
            items->m_skillAttack = query.getFloatField("skill_attack");
            items->m_skillDefense = query.getFloatField("skill_defense");
            items->m_total = query.getIntField("total");
            items->m_description = query.getStringField("description");
            
            this->m_shopData->addObject(items);
            
            query.nextRow();
        }
        query.finalize();
    }
    
    CCTMXTiledMap *alertDialog = (CCTMXTiledMap*)this->getChildByTag(kRPGMapDialogLayerTagAlertDialog);
    CCTableView *tableView = (CCTableView*)alertDialog->getChildByTag(kRPGMapDialogLayerTagAlertDialogTable);
    if(!tableView)
    {
        tableView = CCTableView::create(this, CCSizeMake(610, 320));
        tableView->setDirection(kCCScrollViewDirectionVertical);
        tableView->setPosition(ccp(20, 10));
        tableView->setDelegate(this);
        tableView->setVerticalFillOrder(kCCTableViewFillBottomUp);
        tableView->setTag(kRPGMapDialogLayerTagAlertDialogTable);
        alertDialog->addChild(tableView);
    }
    tableView->reloadData();
    
}

void RPGMapDialogLayer::showShopDefault()
{
    //选择框的背景
    CCTMXTiledMap *alertDialog = CCTMXTiledMap::create(CCString::createWithFormat("alert_%s.tmx", CCUserDefault::sharedUserDefault()->getStringForKey(GAME_STYLE).c_str())->getCString());
    alertDialog->setPosition(ccp((CCDirector::sharedDirector()->getWinSize().width - alertDialog->getContentSize().width) / 2, (CCDirector::sharedDirector()->getWinSize().height - alertDialog->getContentSize().height) / 2));
    alertDialog->setTag(kRPGMapDialogLayerTagAlertDialog);
    this->addChild(alertDialog);
    
    string text = "       ";
    text.append(this->m_npc->m_content->getCString());
    CCLabelTTF *contentLab = (CCLabelTTF*)this->getChildByTag(kRPGMapDialogLayerTagContentLab);
    
    if(this->m_npc->m_dataId == GAME_MAPSHOP_NPC_ID)
    {
        //道具屋
        
        contentLab->setString(text.c_str());
        
        CCControlButton *btnBuy = CCControlButton::create(((CCString*)this->m_stringList->objectForKey("npc_dialog_buy"))->getCString(), "Arial", 28);
        btnBuy->setPosition(ccp(70, 100));
        btnBuy->addTargetWithActionForControlEvents(this, cccontrol_selector(RPGMapDialogLayer::onButton), CCControlEventTouchUpInside);
        btnBuy->setTag(kRPGMapDialogLayerTagBtnBuy);
        alertDialog->addChild(btnBuy);
        
        CCControlButton *btnSell = CCControlButton::create(((CCString*)this->m_stringList->objectForKey("npc_dialog_sell"))->getCString(), "Arial", 28);
        btnSell->setPosition(ccp(190, 100));
        btnSell->addTargetWithActionForControlEvents(this, cccontrol_selector(RPGMapDialogLayer::onButton), CCControlEventTouchUpInside);
        btnSell->setTag(kRPGMapDialogLayerTagBtnSell);
        alertDialog->addChild(btnSell);
        
        CCControlButton *btnCancel = CCControlButton::create(((CCString*)this->m_stringList->objectForKey("npc_dialog_cancel"))->getCString(), "Arial", 28);
        btnCancel->setPosition(ccp(310, 100));
        btnCancel->addTargetWithActionForControlEvents(this, cccontrol_selector(RPGMapDialogLayer::onButton), CCControlEventTouchUpInside);
        btnCancel->setTag(kRPGMapDialogLayerTagBtnCancel);
        alertDialog->addChild(btnCancel);
    }
    else if(this->m_npc->m_dataId == GAME_MAPINN_NPC_ID)
    {
        //旅馆
        
        contentLab->setString(CCString::createWithFormat(text.c_str(), GAME_INN)->getCString());
        
        CCControlButton *btnInn = CCControlButton::create(((CCString*)this->m_stringList->objectForKey("npc_dialog_inn"))->getCString(), "Arial", 28);
        btnInn->setPosition(ccp(100, 100));
        btnInn->addTargetWithActionForControlEvents(this, cccontrol_selector(RPGMapDialogLayer::onButton), CCControlEventTouchUpInside);
        btnInn->setTag(kRPGMapDialogLayerTagBtnInn);
        alertDialog->addChild(btnInn);
        
        CCControlButton *btnCancel = CCControlButton::create(((CCString*)this->m_stringList->objectForKey("npc_dialog_cancel"))->getCString(), "Arial", 28);
        btnCancel->setPosition(ccp(285, 100));
        btnCancel->addTargetWithActionForControlEvents(this, cccontrol_selector(RPGMapDialogLayer::onButton), CCControlEventTouchUpInside);
        btnCancel->setTag(kRPGMapDialogLayerTagBtnCancel);
        alertDialog->addChild(btnCancel);
    }
}
