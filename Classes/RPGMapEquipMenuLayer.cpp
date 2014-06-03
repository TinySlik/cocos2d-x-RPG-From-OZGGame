//
//  RPGMapEquipMenuLayer.cpp
//  OzgGameRPG
//
//  Created by ozg on 14-4-10.
//
//

#include "RPGMapEquipMenuLayer.h"
#include "RPGMapMenuLayer.h"
#include "RPGMapSceneLayer.h"
#include "OzgCCUtility.h"

RPGMapEquipMenuLayer::RPGMapEquipMenuLayer()
{
    
}

RPGMapEquipMenuLayer::~RPGMapEquipMenuLayer()
{
    this->m_stringList->release();
    this->m_itemsList->release();
    
    CCLog("RPGMapEquipMenuLayer 释放");
}

bool RPGMapEquipMenuLayer::init(cocos2d::CCDictionary *stringList, CppSQLite3DB *db, float width, float height)
{
    if(CCLayerColor::initWithColor(ccc4(0, 0, 0, 200), width, height))
    {
        this->m_stringList = stringList;
        this->m_stringList->retain();
        
        this->m_db = db;
        
        this->m_itemsList = new CCArray();
        this->m_itemsList->init();
        
        CCTMXTiledMap *mainBg = CCTMXTiledMap::create(CCString::createWithFormat("map_menu2_%s.tmx", CCUserDefault::sharedUserDefault()->getStringForKey(GAME_STYLE).c_str())->getCString());
        mainBg->setPosition(CCPointZero);
        mainBg->setTag(kRPGMapEquipMenuLayerTagBg);
        this->addChild(mainBg);
        
        CCMenu *mainMenu = CCMenu::create();
        mainMenu->setTag(kRPGMapEquipMenuLayerTagMainMenu);
        mainMenu->setAnchorPoint(CCPointZero);
        mainMenu->setPosition(CCPointZero);
        this->addChild(mainMenu);
        
        CCMenuItemSprite *menuBack = (CCMenuItemSprite*)mainMenu->getChildByTag(kRPGMapEquipMenuLayerTagMainMenuBack);
        if(!menuBack)
        {
            menuBack = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("commons_btn_back_04.png"), CCSprite::createWithSpriteFrameName("commons_btn_back_04.png"), this, menu_selector(RPGMapEquipMenuLayer::onMenu));
            menuBack->setPosition(ccp(40, 600));
            menuBack->setTag(kRPGMapEquipMenuLayerTagMainMenuBack);
            menuBack->setScale(0.5);
            mainMenu->addChild(menuBack);
        }
        
        //显示上面的4个角色
        float playerX = 200;
        CppSQLite3Query query = this->m_db->execQuery(PLAYER_QUERY);
        while(!query.eof())
        {
            CCSprite *playerSprite = NULL;
            if(query.getIntField("hp") <= 0)
            {
                //死亡
                CCArray *frames = OzgCCUtility::createSpriteFrames(CCString::createWithFormat("%s_2.png", query.getStringField("tex_prefix"))->getCString(), 3, 4);
                playerSprite = CCSprite::createWithSpriteFrame((CCSpriteFrame*)frames->objectAtIndex(10));
            }
            else
            {
                //正常
                CCArray *frames = OzgCCUtility::createSpriteFrames(CCString::createWithFormat("%s_0.png", query.getStringField("tex_prefix"))->getCString(), 3, 4);
                playerSprite = CCSprite::createWithSpriteFrame((CCSpriteFrame*)frames->objectAtIndex(1));
            }
            
            CCMenuItemSprite *player = (CCMenuItemSprite*)mainMenu->getChildByTag(kRPGMapEquipMenuLayerTagMainMenuPlayer + query.getIntField("id"));
            if(!player)
            {
                player = CCMenuItemSprite::create(playerSprite, playerSprite, this, menu_selector(RPGMapEquipMenuLayer::onMenu));
                player->setPosition(ccp(playerX, 560));
                player->setScale(2);
                player->setTag(kRPGMapEquipMenuLayerTagMainMenuPlayer + query.getIntField("id"));
                mainMenu->addChild(player);
            }
            
            playerX += 200;
            query.nextRow();
        }
        query.finalize();
        
        //默认选中第一个
        this->onMenu(mainMenu->getChildByTag(kRPGMapEquipMenuLayerTagMainMenuPlayer + 1));
        
        return true;
    }
    return false;
}

RPGMapEquipMenuLayer* RPGMapEquipMenuLayer::create(cocos2d::CCDictionary *stringList, CppSQLite3DB *db, float width, float height)
{
    RPGMapEquipMenuLayer *obj = new RPGMapEquipMenuLayer();
    if(obj && obj->init(stringList, db, width, height))
    {
        obj->autorelease();
        return obj;
    }
    CC_SAFE_DELETE(obj);
    return NULL;
}

//CCTableView
void RPGMapEquipMenuLayer::scrollViewDidScroll(CCScrollView *scrollView)
{
    
}

void RPGMapEquipMenuLayer::scrollViewDidZoom(CCScrollView *scrollView)
{
    
}

void RPGMapEquipMenuLayer::tableCellTouched(CCTableView *tableView, CCTableViewCell *cell)
{
    
}

CCSize RPGMapEquipMenuLayer::cellSizeForTable(CCTableView *tableView)
{
    return CCSizeMake(tableView->getContentSize().width, 80);
}

CCTableViewCell* RPGMapEquipMenuLayer::tableCellAtIndex(CCTableView *tableView, unsigned int idx)
{
    CCTableViewCell *cell = tableView->dequeueCell();
    if (!cell)
    {
        cell = new CCTableViewCell();
        cell->autorelease();
    }
    else
        cell->removeAllChildrenWithCleanup(true);
    
    float x = 100;
    for (int i = 0; i < 4; i++)
    {
        int index = idx * 4 + i;

        if(index >= this->m_itemsList->count())
            break;

        RPGExistingItems *itemsData = (RPGExistingItems*)this->m_itemsList->objectAtIndex(index);
        
        CCControlButton *itemBtn = CCControlButton::create(CCString::createWithFormat("%s (%i)", itemsData->m_name.c_str(), itemsData->m_total)->getCString(), "Arial", 22);
        itemBtn->setPosition(ccp(x, 0));
        itemBtn->setTag(kRPGMapEquipMenuLayerTagBtnEquipItems + itemsData->m_dataId);
        itemBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(RPGMapEquipMenuLayer::onButton), CCControlEventTouchUpInside);
        cell->addChild(itemBtn);
        
        x += 200;
    }
    
    return cell;
}

unsigned int RPGMapEquipMenuLayer::numberOfCellsInTableView(CCTableView *tableView)
{
    int rowItemCount = 4;
    int size = this->m_itemsList->count();
    
    if(size % rowItemCount == 0)
        return size / rowItemCount;
    else
        return size / rowItemCount + 1;
}
//CCTableView end

//private
void RPGMapEquipMenuLayer::loadItemsData()
{
    //装备数据
    this->m_itemsList->removeAllObjects();
    
    CppSQLite3Query query = this->m_db->execQuery(EQUIP_EXISTING_QUERY);
    while(!query.eof())
    {
        string players = query.getStringField("players");
        if((int)players.find(CCString::createWithFormat("%i,", this->m_selectedPlayerId)->getCString()) > -1)
        {
            RPGExistingItems *itemsData = RPGExistingItems::create();
            itemsData->m_dataId = query.getIntField("id");
            itemsData->m_name = query.getStringField("name_cns");
            itemsData->m_buy = query.getIntField("buy");
            itemsData->m_sell = query.getIntField("sell");
            itemsData->m_type = query.getIntField("type");
            itemsData->m_attack = query.getFloatField("attack");
            itemsData->m_defense = query.getFloatField("defense");
            itemsData->m_speed = query.getFloatField("speed");
            itemsData->m_skillAttack = query.getFloatField("skill_attack");
            itemsData->m_skillDefense = query.getFloatField("skill_defense");
            itemsData->m_total = query.getIntField("total");
            this->m_itemsList->addObject(itemsData);
        }
        
        query.nextRow();
    }
    query.finalize();
    
    CCTableView *tableView = (CCTableView*)this->getChildByTag(kRPGMapEquipMenuLayerTagEquipTable);
    tableView->reloadData();
}

void RPGMapEquipMenuLayer::onMenu(cocos2d::CCObject *pObject)
{
    //第一次显示数据的话则不播放效果音
    if(!this->m_isDefault)
        SimpleAudioEngine::sharedEngine()->playEffect("audio_effect_btn.wav");
    
    CCMenuItem *menuItem = (CCMenuItem*)pObject;
    
    switch (menuItem->getTag())
    {
        case kRPGMapEquipMenuLayerTagMainMenuBack:
        {
//            CCLog("后退");
            
            //因为动态获取地图的大小会导致了菜单层显示错位，所以定死了
            float width = 960;
            float height = 640;
            RPGMapMenuLayer *menuLayer = menuLayer = RPGMapMenuLayer::create(this->m_stringList, this->m_db, width, height);
            menuLayer->setPosition(ccp((CCDirector::sharedDirector()->getWinSize().width - width) / 2.0, (CCDirector::sharedDirector()->getWinSize().height - height) / 2.0));
            menuLayer->setTag(kRPGMapSceneLayerTagMenuLayer);
            this->getParent()->addChild(menuLayer);
            
            this->removeFromParentAndCleanup(true);
            
        }
            break;
            
        default:
            break;
    }
    
    if(menuItem->getTag() >= kRPGMapEquipMenuLayerTagMainMenuPlayer && menuItem->getTag() <= kRPGMapEquipMenuLayerTagMainMenuPlayer + 99)
    {
//        CCLog("点击了上面4个player的其中一个");
        CCMenu *mainMenu = (CCMenu*)this->getChildByTag(kRPGMapEquipMenuLayerTagMainMenu);
        
        CppSQLite3Query query = this->m_db->execQuery(PLAYER_QUERY);
        while(!query.eof())
        {
            if(menuItem->getTag() == kRPGMapEquipMenuLayerTagMainMenuPlayer + query.getIntField("id"))
            {
                this->m_isDefault = false;
                
                int dataId = query.getIntField("id");
                this->setPlayerEquip(dataId);
                
                //选中的光标部分
                CCMenuItemSprite *player = (CCMenuItemSprite*)mainMenu->getChildByTag(kRPGMapEquipMenuLayerTagMainMenuPlayer + dataId);
                
                CCSprite *handCursor = (CCSprite*)this->getChildByTag(kRPGMapMenuLayerTagHandCursor);
                if(!handCursor)
                {
                    handCursor = CCSprite::createWithSpriteFrameName("gui_hand.png");
                    handCursor->setScale(0.3);
                    handCursor->setTag(kRPGMapMenuLayerTagHandCursor);
                    this->addChild(handCursor);
                }
                
                //光标的位置
                handCursor->setPosition(ccp(player->getPositionX() + 60, 555));
                
                break;
            }
            query.nextRow();
        }
        query.finalize();
    }
    
}

void RPGMapEquipMenuLayer::setPlayerEquip(int dataId)
{
    this->removeAllPlayerChildren();
    
    this->m_selectedPlayerId = dataId;
    
    CppSQLite3Query query = this->m_db->execQuery(CCString::createWithFormat(PLAYER_DETAIL_QUERY, dataId)->getCString());
    while(!query.eof())
    {
        //左边
        addLab(this, kRPGMapEquipMenuLayerTagName, CCString::create(query.getStringField("name_cns")), 18, ccp(270, 443));
        addLab(this, kRPGMapEquipMenuLayerTagAttack, CCString::createWithFormat(((CCString*)this->m_stringList->objectForKey("menu_equip_attack"))->getCString(), query.getIntField("attack")), 18, ccp(270, 408));
        addLab(this, kRPGMapEquipMenuLayerTagDefense, CCString::createWithFormat(((CCString*)this->m_stringList->objectForKey("menu_equip_defense"))->getCString(), query.getIntField("defense")), 18, ccp(270, 373));
        addLab(this, kRPGMapEquipMenuLayerTagSpeed, CCString::createWithFormat(((CCString*)this->m_stringList->objectForKey("menu_equip_speed"))->getCString(), query.getIntField("speed")), 18, ccp(270, 338));
        addLab(this, kRPGMapEquipMenuLayerTagSkillAttack, CCString::createWithFormat(((CCString*)this->m_stringList->objectForKey("menu_equip_skill_attack"))->getCString(), query.getIntField("skill_attack")), 18, ccp(270, 303));
        addLab(this, kRPGMapEquipMenuLayerTagSkillDefense, CCString::createWithFormat(((CCString*)this->m_stringList->objectForKey("menu_equip_skill_defense"))->getCString(), query.getIntField("skill_defense")), 18, ccp(270, 268));
        
        //右边的装备部分
        if(strlen(query.getStringField("arms_name")) > 0)
        {
            addLab(this, kRPGMapEquipMenuLayerTagEquipArms, CCString::createWithFormat(((CCString*)this->m_stringList->objectForKey("menu_equip_arms"))->getCString(), query.getStringField("arms_name")), 26, ccp(600, 405));
            
            CCControlButton *btnRemoveArms = CCControlButton::create("卸掉", "Arial", 26);
            btnRemoveArms->setPosition(ccp(700, 405));
            btnRemoveArms->setTitleColorForState(ccc3(177, 177, 177), CCControlStateNormal);
            btnRemoveArms->setTag(kRPGMapEquipMenuLayerTagBtnRemoveArms);
            btnRemoveArms->addTargetWithActionForControlEvents(this, cccontrol_selector(RPGMapEquipMenuLayer::onButton), CCControlEventTouchUpInside);
            this->addChild(btnRemoveArms);
        }
        else
            addLab(this, kRPGMapEquipMenuLayerTagEquipArms, CCString::createWithFormat(((CCString*)this->m_stringList->objectForKey("menu_equip_arms"))->getCString(), ((CCString*)this->m_stringList->objectForKey("menu_equip_none"))->getCString()), 26, ccp(600, 405));
        
        if(strlen(query.getStringField("armor_name")) > 0)
        {
            addLab(this, kRPGMapEquipMenuLayerTagEquipArmor, CCString::createWithFormat(((CCString*)this->m_stringList->objectForKey("menu_equip_armor"))->getCString(), query.getStringField("armor_name")), 26, ccp(600, 320));
            
            CCControlButton *btnRemoveArmor = CCControlButton::create("卸掉", "Arial", 26);
            btnRemoveArmor->setPosition(ccp(700, 320));
            btnRemoveArmor->setTitleColorForState(ccc3(177, 177, 177), CCControlStateNormal);
            btnRemoveArmor->setTag(kRPGMapEquipMenuLayerTagBtnRemoveArmor);
            btnRemoveArmor->addTargetWithActionForControlEvents(this, cccontrol_selector(RPGMapEquipMenuLayer::onButton), CCControlEventTouchUpInside);
            this->addChild(btnRemoveArmor);
        }
        else
            addLab(this, kRPGMapEquipMenuLayerTagEquipArmor, CCString::createWithFormat(((CCString*)this->m_stringList->objectForKey("menu_equip_armor"))->getCString(), ((CCString*)this->m_stringList->objectForKey("menu_equip_none"))->getCString()), 26, ccp(600, 320));
        
        //下面的装备列表
        CCTableView *tableView = CCTableView::create(this, CCSizeMake(900, 180));
        tableView->setDirection(kCCScrollViewDirectionVertical);
        tableView->setPosition(ccp(80, 30));
        tableView->setDelegate(this);
        tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
        tableView->setTag(kRPGMapEquipMenuLayerTagEquipTable);
        this->addChild(tableView);
        
        this->loadItemsData();
        
        query.nextRow();
    }
    query.finalize();
        
}

void RPGMapEquipMenuLayer::removeAllPlayerChildren()
{
    //左边
    if(this->getChildByTag(kRPGMapEquipMenuLayerTagName))
        this->removeChildByTag(kRPGMapEquipMenuLayerTagName, true);
    if(this->getChildByTag(kRPGMapEquipMenuLayerTagAttack))
        this->removeChildByTag(kRPGMapEquipMenuLayerTagAttack, true);
    if(this->getChildByTag(kRPGMapEquipMenuLayerTagDefense))
        this->removeChildByTag(kRPGMapEquipMenuLayerTagDefense, true);
    if(this->getChildByTag(kRPGMapEquipMenuLayerTagSpeed))
        this->removeChildByTag(kRPGMapEquipMenuLayerTagSpeed, true);
    if(this->getChildByTag(kRPGMapEquipMenuLayerTagSkillAttack))
        this->removeChildByTag(kRPGMapEquipMenuLayerTagSkillAttack, true);
    if(this->getChildByTag(kRPGMapEquipMenuLayerTagSkillDefense))
        this->removeChildByTag(kRPGMapEquipMenuLayerTagSkillDefense, true);
    
    //右边的装备部分
    if(this->getChildByTag(kRPGMapEquipMenuLayerTagEquipArms))
        this->removeChildByTag(kRPGMapEquipMenuLayerTagEquipArms, true);
    if(this->getChildByTag(kRPGMapEquipMenuLayerTagEquipArmor))
        this->removeChildByTag(kRPGMapEquipMenuLayerTagEquipArmor, true);
    if(this->getChildByTag(kRPGMapEquipMenuLayerTagBtnRemoveArms))
        this->removeChildByTag(kRPGMapEquipMenuLayerTagBtnRemoveArms, true);
    if(this->getChildByTag(kRPGMapEquipMenuLayerTagBtnRemoveArmor))
        this->removeChildByTag(kRPGMapEquipMenuLayerTagBtnRemoveArmor, true);
    
    //下面的装备列表
    if(this->getChildByTag(kRPGMapEquipMenuLayerTagEquipTable))
        this->removeChildByTag(kRPGMapEquipMenuLayerTagEquipTable, true);
}

void RPGMapEquipMenuLayer::onButton(cocos2d::CCObject *pSender, CCControlEvent event)
{
    SimpleAudioEngine::sharedEngine()->playEffect("audio_effect_btn.wav");
    
    CCControlButton *itemBtn = (CCControlButton*)pSender;
    
    if(itemBtn->getTag() == kRPGMapEquipMenuLayerTagBtnRemoveArms)
        RPGResultsLogic::removeEquip(this->m_db, this->m_selectedPlayerId, 1);
    else if(itemBtn->getTag() == kRPGMapEquipMenuLayerTagBtnRemoveArmor)
        RPGResultsLogic::removeEquip(this->m_db, this->m_selectedPlayerId, 2);
    else    
        RPGResultsLogic::equip(this->m_db, this->m_selectedPlayerId, itemBtn->getTag() - kRPGMapEquipMenuLayerTagBtnEquipItems);
        
    //刷新数据
    this->setPlayerEquip(this->m_selectedPlayerId);
}
