//
//  RPGMapMenuLayer.cpp
//  OzgGameRPG
//
//  Created by ozg on 14-3-18.
//
//

#include "RPGMapMenuLayer.h"
#include "RPGMapSceneLayer.h"
#include "RPGConfirmLayer.h"
#include "RPGStartSceneLayer.h"
#include "JsonBox.h"
#include "OzgCCUtility.h"

//获取一个player的详细数据
#define PLAYER_QUERY_DETAIL "select p.*, arms.name_cns as arms_name, armor.name_cns as armor_name from player as p left join items as arms on p.items_id_arms = arms.id left join items as armor on p.items_id_armor = armor.id where p.id = %i"

//查询一个角色的技能
#define SKILL_QUERY "select * from skill where id in(%s)"

RPGMapMenuLayer::RPGMapMenuLayer()
{
    
}

RPGMapMenuLayer::~RPGMapMenuLayer()
{
    this->m_stringList->release();
    this->m_skillList->release();
    
//    CCTextureCache::sharedTextureCache()->dumpCachedTextureInfo();
    
//    CCLog("RPGMapMenuLayer释放");
}

bool RPGMapMenuLayer::init(CCDictionary* stringList, CppSQLite3DB* db, float width, float height)
{
    if(CCLayerColor::initWithColor(ccc4(0, 0, 0, 200), width, height))
    {
        this->m_stringList = stringList;
        this->m_stringList->retain();
        
        this->m_skillList = new CCArray();
        this->m_skillList->init();
        
        this->m_db = db;
        
        this->m_width = width;
        this->m_height = height;
        
        this->m_statusIsDefault = true;
        
//        CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("main.plist");
        
        CCMenu *mainMenu = CCMenu::create();
        mainMenu->setTag(kRPGMapMenuLayerTagMainMenu);
        mainMenu->setAnchorPoint(CCPointZero);
        mainMenu->setPosition(CCPointZero);
        this->addChild(mainMenu);
        
        this->setMainMenuDefault();
        
        CCTMXTiledMap *mainBg = CCTMXTiledMap::create("map_menu1_style1.tmx");
        mainBg->setPosition(CCPointZero);
        mainBg->setTag(kRPGMapMenuLayerTagMainBg);
        this->addChild(mainBg);
        
        this->setDefaultInterface();
        
        return true;
    }
    return false;
}

RPGMapMenuLayer* RPGMapMenuLayer::create(CCDictionary* stringList, CppSQLite3DB* db, float width, float height)
{
    RPGMapMenuLayer *obj = new RPGMapMenuLayer();
    if(obj && obj->init(stringList, db, width, height))
    {
        obj->autorelease();
        return obj;
    }
    
    CC_SAFE_DELETE(obj);
    return NULL;
}

//private
void RPGMapMenuLayer::onMenu(cocos2d::CCObject *pObject)
{
    CCMenuItem *menuItem = (CCMenuItem*)pObject;
    
    //除默认显示角色数据之外，其他所有的点击都有效果音
    if(!(this->m_statusIsDefault && menuItem->getTag() == kRPGMapMenuLayerTagMainMenuPlayerStatus + 1))
        SimpleAudioEngine::sharedEngine()->playEffect("audio_effect_btn.wav");
    
    switch (menuItem->getTag())
    {
        case kRPGMapMenuLayerTagMainMenuEquip:
        {
            CCLog("装备");
            
        }
            break;
        case kRPGMapMenuLayerTagMainMenuSkill:
        {
            CCLog("技能");
            
        }
            break;
        case kRPGMapMenuLayerTagMainMenuStatus:
        {
//            CCLog("状态");
            this->m_statusIsDefault = true;
            
            this->removeDefaultInterface();
            this->removeChildByTag(kRPGMapMenuLayerTagMainBg, true);
            
            this->setMainMenuStatus();
            
            CCTMXTiledMap *mainBg = CCTMXTiledMap::create("map_menu4_style1.tmx");
            mainBg->setPosition(CCPointZero);
            mainBg->setTag(kRPGMapMenuLayerTagMainBg);
            this->addChild(mainBg);
            
            this->setStatusInterface();
        }
            break;
        case kRPGMapMenuLayerTagMainMenuClose:
        {
//            CCLog("关闭");
            
            RPGMapSceneLayer *mapSceneLayer = (RPGMapSceneLayer*)this->getParent();
            mapSceneLayer->enabledControl(true);
            
            this->removeFromParentAndCleanup(true);
            
        }
            break;
        case kRPGMapMenuLayerTagMainMenuExit:
        {
            //退出
            RPGConfirmLayer *confirm = RPGConfirmLayer::create(((CCString*)this->m_stringList->objectForKey("confirm_exit"))->getCString(), ((CCString*)this->m_stringList->objectForKey("confirm_exit_ok"))->getCString(), kRPGMapMenuLayerTagConfirmExitOK, ((CCString*)this->m_stringList->objectForKey("confirm_cancel"))->getCString(), kRPGMapMenuLayerTagConfirmCancel, this->m_width, this->m_height, this, menu_selector(RPGMapMenuLayer::onMenu));
            confirm->setTag(kRPGMapMenuLayerTagConfirm);
            this->addChild(confirm);
            
            CCMenu *mainMenu = (CCMenu*)this->getChildByTag(kRPGMapMenuLayerTagMainMenu);
            mainMenu->setEnabled(false);
            
        }
            break;
        case kRPGMapMenuLayerTagMainMenuStatusBack:
        {
            //在状态界面中按后退
            this->removeChildByTag(kRPGMapMenuLayerTagMainBg, true);
            
            this->setMainMenuDefault();
            this->setDefaultInterface();
            
            CCTMXTiledMap *mainBg = CCTMXTiledMap::create("map_menu1_style1.tmx");
            mainBg->setPosition(CCPointZero);
            mainBg->setTag(kRPGMapMenuLayerTagMainBg);
            this->addChild(mainBg);
            
            //清理上面的4个角色
            CCMenu *mainMenu = (CCMenu*)this->getChildByTag(kRPGMapMenuLayerTagMainMenu);
            CppSQLite3Query query = this->m_db->execQuery(PLAYER_QUERY);
            while(!query.eof())
            {
                CCMenuItemSprite *player = (CCMenuItemSprite*)mainMenu->getChildByTag(kRPGMapMenuLayerTagMainMenuPlayerStatus + query.getIntField("id"));
                if(player)
                    player->removeFromParentAndCleanup(true);
                
                CCTextureCache::sharedTextureCache()->removeTextureForKey(CCString::createWithFormat("%s_0.png", query.getStringField("tex_prefix"))->getCString());
                
                query.nextRow();
            }
            query.finalize();
            
            //清理各个lab部分
            this->removeAllStatusPlayerLab();
            
            this->removeChildByTag(kRPGMapMenuLayerTagStatusPlayerSkillTable, true);
            this->m_skillList->removeAllObjects();
            
            //清理光标
            this->removeChildByTag(kRPGMapMenuLayerTagHandCursor, true);
        }
            break;
        case kRPGMapMenuLayerTagConfirmExitOK:
        {
            //确定退出
            RPGMapSceneLayer *mapSceneLayer = (RPGMapSceneLayer*)this->getParent();
            mapSceneLayer->m_releaseTexture = true;
            
            CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(mapSceneLayer);
            mapSceneLayer->unscheduleUpdate();
            
            SimpleAudioEngine::sharedEngine()->stopBackgroundMusic(true);
            
            CCScene *s = RPGStartSceneLayer::scene();
            CCTransitionFade *t = CCTransitionFade::create(GAME_SCENE, s);
            CCDirector::sharedDirector()->replaceScene(t);
        }
            break;
        case kRPGMapMenuLayerTagConfirmCancel:
        {
            this->removeChildByTag(kRPGMapMenuLayerTagConfirm, true);
            
            CCMenu *mainMenu = (CCMenu*)this->getChildByTag(kRPGMapMenuLayerTagMainMenu);
            mainMenu->setEnabled(true);
        }
            break;
        case kRPGMapMenuLayerTagMainMenuItems:
        {
            CCLog("道具");
            
        }
            break;
    }
    
    //player status
    if(menuItem->getTag() >= kRPGMapMenuLayerTagMainMenuPlayerStatus && menuItem->getTag() <= kRPGMapMenuLayerTagMainMenuPlayerStatus + 99)
    {
        CCMenu *mainMenu = (CCMenu*)this->getChildByTag(kRPGMapMenuLayerTagMainMenu);
        
        CppSQLite3Query query = this->m_db->execQuery(PLAYER_QUERY);
        while(!query.eof())
        {
            if(menuItem->getTag() == kRPGMapMenuLayerTagMainMenuPlayerStatus + query.getIntField("id"))
            {
                this->removeAllStatusPlayerLab();
                
                if(this->getChildByTag(kRPGMapMenuLayerTagStatusPlayerSkillTable))
                    this->removeChildByTag(kRPGMapMenuLayerTagStatusPlayerSkillTable, true);
                this->m_skillList->removeAllObjects();
                
                this->m_statusIsDefault = false;
                
//                CCLog("选中了id为%i的角色", query.getIntField("id"));
                int dataId = query.getIntField("id");
                this->setStatusPlayer(dataId);
                CCMenuItemSprite *player = (CCMenuItemSprite*)mainMenu->getChildByTag(kRPGMapMenuLayerTagMainMenuPlayerStatus + dataId);
                
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

void RPGMapMenuLayer::createMenuItem(float x, float y, CCString *text, RPGMapMenuLayerTag tag)
{
    CCMenu *mainMenu = (CCMenu*)this->getChildByTag(kRPGMapMenuLayerTagMainMenu);
    
    CCMenuItemLabel *menuItems = (CCMenuItemLabel*)mainMenu->getChildByTag(tag);
    if(!menuItems)
    {
        CCLabelTTF *menuItemsLabel = CCLabelTTF::create(OzgCCUtility::generalString(text->getCString()).c_str(), "FZCuYuan-M03S", 25, CCSizeMake(120, 40), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    
        CCMenuItemLabel *menuItems = CCMenuItemLabel::create(menuItemsLabel, this, menu_selector(RPGMapMenuLayer::onMenu));
        menuItems->setPosition(ccp(x, y));
        menuItems->setTag(tag);
        mainMenu->addChild(menuItems);
        
    }
}

void RPGMapMenuLayer::setMainMenuDefault()
{
    CCMenu *mainMenu = (CCMenu*)this->getChildByTag(kRPGMapMenuLayerTagMainMenu);
    
    CCString *menuItemsStr = (CCString*)this->m_stringList->objectForKey("menu_items");
    this->createMenuItem(835, 570, menuItemsStr, kRPGMapMenuLayerTagMainMenuItems);
    
    CCString *menuEquipStr = (CCString*)this->m_stringList->objectForKey("menu_equip");
    this->createMenuItem(835, 500, menuEquipStr, kRPGMapMenuLayerTagMainMenuEquip);
    
    CCString *menuSkillStr = (CCString*)this->m_stringList->objectForKey("menu_skill");
    this->createMenuItem(835, 430, menuSkillStr, kRPGMapMenuLayerTagMainMenuSkill);
    
    CCString *menuStatusStr = (CCString*)this->m_stringList->objectForKey("menu_status");
    this->createMenuItem(835, 360, menuStatusStr, kRPGMapMenuLayerTagMainMenuStatus);
    
    CCString *menuCloseStr = (CCString*)this->m_stringList->objectForKey("menu_close");
    this->createMenuItem(835, 290, menuCloseStr, kRPGMapMenuLayerTagMainMenuClose);
    
    CCString *menuExitStr = (CCString*)this->m_stringList->objectForKey("menu_exit");
    this->createMenuItem(835, 220, menuExitStr, kRPGMapMenuLayerTagMainMenuExit);
    
    CCObject *item = NULL;
    CCARRAY_FOREACH(mainMenu->getChildren(), item)
    {
        CCMenuItem *menuItem = (CCMenuItem*)item;
        
        if(menuItem->getTag() == kRPGMapMenuLayerTagMainMenuItems ||
           menuItem->getTag() == kRPGMapMenuLayerTagMainMenuEquip ||
           menuItem->getTag() == kRPGMapMenuLayerTagMainMenuSkill ||
           menuItem->getTag() == kRPGMapMenuLayerTagMainMenuStatus ||
           menuItem->getTag() == kRPGMapMenuLayerTagMainMenuClose ||
           menuItem->getTag() == kRPGMapMenuLayerTagMainMenuExit)
            menuItem->setVisible(true);
        else
            menuItem->setVisible(false);
        
    }
}

void RPGMapMenuLayer::setMainMenuStatus()
{
    CCMenu *mainMenu = (CCMenu*)this->getChildByTag(kRPGMapMenuLayerTagMainMenu);
    
    CCMenuItemSprite *menuBack = (CCMenuItemSprite*)mainMenu->getChildByTag(kRPGMapMenuLayerTagMainMenuStatusBack);
    if(!menuBack)
    {
        menuBack = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("commons_btn_back_04.png"), CCSprite::createWithSpriteFrameName("commons_btn_back_04.png"), this, menu_selector(RPGMapMenuLayer::onMenu));
        menuBack->setPosition(ccp(40, 600));
        menuBack->setTag(kRPGMapMenuLayerTagMainMenuStatusBack);
        menuBack->setScale(0.5);
        mainMenu->addChild(menuBack);
        
    }
    
    CCObject *item = NULL;
    CCARRAY_FOREACH(mainMenu->getChildren(), item)
    {
        CCMenuItem *menuItem = (CCMenuItem*)item;
        
        if(menuItem->getTag() == kRPGMapMenuLayerTagMainMenuStatusBack)
            menuItem->setVisible(true);
        else
            menuItem->setVisible(false);
        
    }
}

void RPGMapMenuLayer::setDefaultInterface()
{
    
    //显示上面的4个角色
    float playerY = 550;
    float nameY = 547;
    float levelY = 568;
    float hpY = 543;
    float mpY = 523;
    
    CppSQLite3Query query = this->m_db->execQuery(PLAYER_QUERY);
    while(!query.eof())
    {
        CCArray *frames = OzgCCUtility::createSpriteFrames(CCString::createWithFormat("%s_0.png", query.getStringField("tex_prefix"))->getCString(), 3, 4);
        
        CCSprite *playerSprite = CCSprite::createWithSpriteFrame((CCSpriteFrame*)frames->objectAtIndex(1));
        playerSprite->setPosition(ccp(150, playerY));
        playerSprite->setScale(2);
        playerSprite->setTag(kRPGMapMenuLayerTagDefualtPlayerTex + query.getIntField("id"));
        this->addChild(playerSprite);
        
        addLab(this, kRPGMapMenuLayerTagDefualtPlayerName + query.getIntField("id"), CCString::create(query.getStringField("name_cns")), 22, ccp(410, nameY));
        
        addLab(this, kRPGMapMenuLayerTagDefualtPlayerLevel + query.getIntField("id"), CCString::createWithFormat(((CCString*)this->m_stringList->objectForKey("status_level"))->getCString(), query.getIntField("level"), query.getIntField("next_exp")), 18, ccp(580, levelY));
        
        addLab(this, kRPGMapMenuLayerTagDefualtPlayerHP + query.getIntField("id"), CCString::createWithFormat(((CCString*)this->m_stringList->objectForKey("status_hp"))->getCString(), query.getIntField("hp"), query.getIntField("max_hp")), 16, ccp(601, hpY));
        
        addLab(this, kRPGMapMenuLayerTagDefualtPlayerMP + query.getIntField("id"), CCString::createWithFormat(((CCString*)this->m_stringList->objectForKey("status_mp"))->getCString(), query.getIntField("mp"), query.getIntField("max_mp")), 16, ccp(599, mpY));
        
        playerY -= 150;
        nameY -= 150;
        levelY -= 150;
        hpY -= 150;
        mpY -= 150;
        
        query.nextRow();
    }
    query.finalize();
    
}

void RPGMapMenuLayer::removeDefaultInterface()
{
    CppSQLite3Query query = this->m_db->execQuery(PLAYER_QUERY);
    while(!query.eof())
    {
        
        CCSprite *playerSprite = (CCSprite*)this->getChildByTag(kRPGMapMenuLayerTagDefualtPlayerTex + query.getIntField("id"));
        if(playerSprite)
            playerSprite->removeFromParentAndCleanup(true);
        
        CCLabelTTF *nameLab = (CCLabelTTF*)this->getChildByTag(kRPGMapMenuLayerTagDefualtPlayerName + query.getIntField("id"));
        if(nameLab)
            nameLab->removeFromParentAndCleanup(true);
        
        CCLabelTTF *levelLab = (CCLabelTTF*)this->getChildByTag(kRPGMapMenuLayerTagDefualtPlayerLevel + query.getIntField("id"));
        if(levelLab)
            levelLab->removeFromParentAndCleanup(true);
        
        CCLabelTTF *hpLab = (CCLabelTTF*)this->getChildByTag(kRPGMapMenuLayerTagDefualtPlayerHP + query.getIntField("id"));
        if(hpLab)
            hpLab->removeFromParentAndCleanup(true);
        
        CCLabelTTF *mpLab = (CCLabelTTF*)this->getChildByTag(kRPGMapMenuLayerTagDefualtPlayerMP + query.getIntField("id"));
        if(mpLab)
            mpLab->removeFromParentAndCleanup(true);
        
        query.nextRow();
    }
    query.finalize();
    
}

void RPGMapMenuLayer::setStatusInterface()
{
    CCMenu *mainMenu = (CCMenu*)this->getChildByTag(kRPGMapMenuLayerTagMainMenu);
    
    //显示上面的4个角色
    float playerX = 200;
    CppSQLite3Query query = this->m_db->execQuery(PLAYER_QUERY);
    while(!query.eof())
    {
        CCArray *frames = OzgCCUtility::createSpriteFrames(CCString::createWithFormat("%s_0.png", query.getStringField("tex_prefix"))->getCString(), 3, 4);
        
        CCSprite *playerSprite = CCSprite::createWithSpriteFrame((CCSpriteFrame*)frames->objectAtIndex(1));
        CCMenuItemSprite *player = (CCMenuItemSprite*)mainMenu->getChildByTag(kRPGMapMenuLayerTagMainMenuPlayerStatus + query.getIntField("id"));
        if(!player)
        {
            player = CCMenuItemSprite::create(playerSprite, playerSprite, this, menu_selector(RPGMapMenuLayer::onMenu));
            player->cocos2d::CCNode::setPosition(ccp(playerX, 560));
            player->setScale(2);
            player->setTag(kRPGMapMenuLayerTagMainMenuPlayerStatus + query.getIntField("id"));
            mainMenu->addChild(player);
        }
        
        playerX += 200;
        query.nextRow();
    }
    query.finalize();
    
    //默认选中第一个
    this->onMenu(mainMenu->getChildByTag(kRPGMapMenuLayerTagMainMenuPlayerStatus + 1));
}

void RPGMapMenuLayer::setStatusPlayer(int dataId)
{
    CppSQLite3Query query = this->m_db->execQuery(CCString::createWithFormat(PLAYER_QUERY_DETAIL, dataId)->getCString());
    while(!query.eof())
    {
        //左边
        addLab(this, kRPGMapMenuLayerTagStatusPlayerName, CCString::createWithFormat(((CCString*)this->m_stringList->objectForKey("status_name"))->getCString(), query.getStringField("name_cns")), ccp(250, 410));
        addLab(this, kRPGMapMenuLayerTagStatusPlayerLevel, CCString::createWithFormat(((CCString*)this->m_stringList->objectForKey("status_level"))->getCString(), query.getIntField("level"), query.getIntField("next_exp")), ccp(243, 370));
        addLab(this, kRPGMapMenuLayerTagStatusPlayerHP, CCString::createWithFormat(((CCString*)this->m_stringList->objectForKey("status_hp"))->getCString(), query.getIntField("hp"), query.getIntField("max_hp")), ccp(262, 330));
        addLab(this, kRPGMapMenuLayerTagStatusPlayerMP, CCString::createWithFormat(((CCString*)this->m_stringList->objectForKey("status_mp"))->getCString(), query.getIntField("mp"), query.getIntField("max_mp")), ccp(259, 290));
        addLab(this, kRPGMapMenuLayerTagStatusPlayerAttack, CCString::createWithFormat(((CCString*)this->m_stringList->objectForKey("status_attack"))->getCString(), query.getIntField("attack")), ccp(249, 250));
        addLab(this, kRPGMapMenuLayerTagStatusPlayerDefense, CCString::createWithFormat(((CCString*)this->m_stringList->objectForKey("status_defense"))->getCString(), query.getIntField("defense")), ccp(249, 210));
        addLab(this, kRPGMapMenuLayerTagStatusPlayerSpeed, CCString::createWithFormat(((CCString*)this->m_stringList->objectForKey("status_speed"))->getCString(), query.getIntField("speed")), ccp(249, 170));
        addLab(this, kRPGMapMenuLayerTagStatusPlayerMagicAttack, CCString::createWithFormat(((CCString*)this->m_stringList->objectForKey("status_magic_attack"))->getCString(), query.getIntField("magic_attack")), ccp(249, 130));
        addLab(this, kRPGMapMenuLayerTagStatusPlayerMagicDefense, CCString::createWithFormat(((CCString*)this->m_stringList->objectForKey("status_magic_defense"))->getCString(), query.getIntField("magic_defense")), ccp(249, 90));
        
        //右边装备部分
        addLab(this, kRPGMapMenuLayerTagStatusPlayerEquipArms, CCString::createWithFormat(((CCString*)this->m_stringList->objectForKey("status_equip_arms"))->getCString(), query.getStringField("arms_name")), ccp(570, 410));
        addLab(this, kRPGMapMenuLayerTagStatusPlayerEquipArmor, CCString::createWithFormat(((CCString*)this->m_stringList->objectForKey("status_equip_armor"))->getCString(), query.getStringField("armor_name")), ccp(570, 370));
        
        //右边技能部分
        
        //数据处理部分
        this->m_skillList->removeAllObjects();
        string wq = "";
        JsonBox::Value json;
        json.loadFromString(query.getStringField("skill"));
        JsonBox::Array jsonArr = json.getArray();
        for (int i = 0; i < jsonArr.size(); i++)
        {
            char* str = (char*)malloc(10 * sizeof(char));
            OzgCCUtility::itoa(jsonArr[i].getInt(), str);
            wq.append(str);
            
            if(i + 1 < jsonArr.size())
                wq.append(", ");
            
            free(str);
        }
        if((int)wq.length() > 0)
        {
            CppSQLite3Query skillQuery = this->m_db->execQuery(CCString::createWithFormat(SKILL_QUERY, wq.c_str())->getCString());
            while(!skillQuery.eof())
            {
                RPGSkill *skill = RPGSkill::create();
                skill->m_name = skillQuery.getStringField("name_cns");
                skill->m_mp = skillQuery.getIntField("mp");
                
                this->m_skillList->addObject(skill);
                
                skillQuery.nextRow();
            }
            skillQuery.finalize();
        }
        
        CCTableView *tableView = CCTableView::create(this, CCSizeMake(600, 265));
        tableView->setDirection(kCCScrollViewDirectionVertical);
        tableView->setPosition(ccp(340, 20));
        tableView->setDelegate(this);
        tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
        tableView->setTag(kRPGMapMenuLayerTagStatusPlayerSkillTable);
        this->addChild(tableView);
        tableView->reloadData();
        
        query.nextRow();
    }
    query.finalize();
    
}

void RPGMapMenuLayer::removeAllStatusPlayerLab()
{
    //清理各个lab部分
    if(this->getChildByTag(kRPGMapMenuLayerTagStatusPlayerName))
        this->removeChildByTag(kRPGMapMenuLayerTagStatusPlayerName, true);
    
    if(this->getChildByTag(kRPGMapMenuLayerTagStatusPlayerLevel))
        this->removeChildByTag(kRPGMapMenuLayerTagStatusPlayerLevel, true);
    
    if(this->getChildByTag(kRPGMapMenuLayerTagStatusPlayerHP))
        this->removeChildByTag(kRPGMapMenuLayerTagStatusPlayerHP, true);
    
    if(this->getChildByTag(kRPGMapMenuLayerTagStatusPlayerMP))
        this->removeChildByTag(kRPGMapMenuLayerTagStatusPlayerMP, true);
    
    if(this->getChildByTag(kRPGMapMenuLayerTagStatusPlayerAttack))
        this->removeChildByTag(kRPGMapMenuLayerTagStatusPlayerAttack, true);
    
    if(this->getChildByTag(kRPGMapMenuLayerTagStatusPlayerDefense))
        this->removeChildByTag(kRPGMapMenuLayerTagStatusPlayerDefense, true);
    
    if(this->getChildByTag(kRPGMapMenuLayerTagStatusPlayerSpeed))
        this->removeChildByTag(kRPGMapMenuLayerTagStatusPlayerSpeed, true);
    
    if(this->getChildByTag(kRPGMapMenuLayerTagStatusPlayerMagicAttack))
        this->removeChildByTag(kRPGMapMenuLayerTagStatusPlayerMagicAttack, true);
    
    if(this->getChildByTag(kRPGMapMenuLayerTagStatusPlayerMagicDefense))
        this->removeChildByTag(kRPGMapMenuLayerTagStatusPlayerMagicDefense, true);
    
    if(this->getChildByTag(kRPGMapMenuLayerTagStatusPlayerEquipArms))
        this->removeChildByTag(kRPGMapMenuLayerTagStatusPlayerEquipArms, true);
    
    if(this->getChildByTag(kRPGMapMenuLayerTagStatusPlayerEquipArmor))
        this->removeChildByTag(kRPGMapMenuLayerTagStatusPlayerEquipArmor, true);
}

//CCTableView
void RPGMapMenuLayer::scrollViewDidScroll(CCScrollView *scrollView)
{

}

void RPGMapMenuLayer::scrollViewDidZoom(CCScrollView *scrollView)
{
    
}

void RPGMapMenuLayer::tableCellTouched(CCTableView *tableView, CCTableViewCell *cell)
{

}

CCSize RPGMapMenuLayer::cellSizeForTable(CCTableView *tableView)
{
    return CCSizeMake(tableView->getContentSize().width, 80);
}

CCTableViewCell* RPGMapMenuLayer::tableCellAtIndex(CCTableView *tableView, unsigned int idx)
{
    CCTableViewCell *cell = tableView->dequeueCell();
    if (!cell)
    {
        cell = new CCTableViewCell();
        cell->autorelease();
    }
    else
        cell->removeAllChildrenWithCleanup(true);
    
    float x = 150;
    for (int i = 0; i < 3; i++)
    {
        int index = idx * 3 + i;
        
        if(index >= this->m_skillList->count())
            break;
        
        RPGSkill *skillData = (RPGSkill*)this->m_skillList->objectAtIndex(index);
        
        CCLabelTTF *textLab = CCLabelTTF::create(CCString::createWithFormat("%s (%i)", skillData->m_name.c_str(), skillData->m_mp)->getCString(), "Arial", 20, CCSizeMake(200, 25), kCCTextAlignmentLeft);
        textLab->setPosition(ccp(x, 0));
        cell->addChild(textLab);
        
        x += 200;
    }
    
    return cell;
}

unsigned int RPGMapMenuLayer::numberOfCellsInTableView(CCTableView *tableView)
{
    int rowItemCount = 3;
    int size = this->m_skillList->count();
    
    if(size % rowItemCount == 0)
        return size / rowItemCount;
    else
        return size / rowItemCount + 1;
}

//CCTableView end
