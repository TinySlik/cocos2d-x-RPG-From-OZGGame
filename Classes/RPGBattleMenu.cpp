//
//  RPGBattleMenu.cpp
//  OzgGameRPG
//
//  Created by ozg on 14-4-6.
//
//

#include "RPGBattleMenu.h"

RPGBattleMenu::RPGBattleMenu()
{
    
}

RPGBattleMenu::~RPGBattleMenu()
{
    this->m_stringList->release();
    this->m_parentNode->release();
    this->m_playerData->release();
    
    this->m_tableItems->release();
    
    CCTMXTiledMap *bgLayer = (CCTMXTiledMap*)this->m_parentNode->getChildByTag(kRPGBattleMenuTagBg);
    if(bgLayer)
        bgLayer->removeFromParentAndCleanup(true);
    
    if(this->m_parentNode->getChildByTag(kRPGBattleMenuTagNameLab))
        this->m_parentNode->removeChildByTag(kRPGBattleMenuTagNameLab, true);
    
    if(this->m_parentNode->getChildByTag(kRPGBattleMenuTagSeparate))
        this->m_parentNode->removeChildByTag(kRPGBattleMenuTagSeparate, true);
    
    //清理技能和道具选择框
    if(this->m_parentNode->getChildByTag(kRPGBattleSceneLayerTagItemsSelectDialog))
        this->m_parentNode->removeChildByTag(kRPGBattleSceneLayerTagItemsSelectDialog, true);
    if(this->m_parentNode->getChildByTag(kRPGBattleSceneLayerTagSkillSelectDialog))
        this->m_parentNode->removeChildByTag(kRPGBattleSceneLayerTagSkillSelectDialog, true);
    
    CCLog("RPGBattleMenu 释放");
}

bool RPGBattleMenu::initWithParentNode(CCDictionary* stringList, CppSQLite3DB* db, CCNode* parentNode, RPGPlayer* playerData)
{
    if(CCMenu::init())
    {
        this->m_stringList = stringList;
        this->m_stringList->retain();
        
        this->m_db = db;
        
        this->m_parentNode = parentNode;
        this->m_parentNode->retain();
        
        this->m_playerData = playerData;
        this->m_playerData->retain();
        
        this->m_tableItems = new CCArray();
        this->m_tableItems->init();
                
        this->setPosition(CCPointZero);
        
        CCTMXTiledMap *bgLayer = (CCTMXTiledMap*)this->m_parentNode->getChildByTag(kRPGBattleMenuTagBg);
        if(!bgLayer)
        {
            bgLayer = CCTMXTiledMap::create("battle_battlemenu_style1.tmx");
            bgLayer->setPosition(ccp(718, 0));
            bgLayer->setTag(kRPGBattleMenuTagBg);
            parentNode->addChild(bgLayer);
        }
        
        //角色名字
        addLab(this->m_parentNode, kRPGBattleMenuTagNameLab, CCString::create(playerData->m_name), 20, kCCTextAlignmentCenter, ccp(798, 228));
        CCLabelTTF *nameLab = (CCLabelTTF*)this->m_parentNode->getChildByTag(kRPGBattleMenuTagNameLab);
        nameLab->setFontFillColor(ccc3(144, 144, 144));
        
        //分割线
        CCSprite *separate = CCSprite::createWithSpriteFrameName("separate.png");
        separate->setTag(kRPGBattleMenuTagSeparate);
        separate->setPosition(ccp(798, 211));
        separate->setScaleX(0.34);
        this->m_parentNode->addChild(separate);
        
        CCString *menuAttackStr = (CCString*)this->m_stringList->objectForKey("battle_playermenu_attack");
        this->createMenuItem(798, 186, menuAttackStr, kRPGBattleMenuTagAttack);

        CCString *menuSkillStr = (CCString*)this->m_stringList->objectForKey("battle_playermenu_skill");
        this->createMenuItem(798, 136, menuSkillStr, kRPGBattleMenuTagSkill);

        CCString *menuItemsStr = (CCString*)this->m_stringList->objectForKey("battle_playermenu_items");
        this->createMenuItem(798, 86, menuItemsStr, kRPGBattleMenuTagItems);
        
        CCString *menuEscapeStr = (CCString*)this->m_stringList->objectForKey("battle_playermenu_escape");
        this->createMenuItem(798, 36, menuEscapeStr, kRPGBattleMenuTagEscape);
        
        SimpleAudioEngine::sharedEngine()->playEffect("audio_battle_menu.wav");
        
        return true;
    }
    return false;
}

RPGBattleMenu* RPGBattleMenu::createWithParentNode(CCDictionary* stringList, CppSQLite3DB* db, CCNode* parentNode, RPGPlayer* playerData)
{
    RPGBattleMenu *obj = new RPGBattleMenu();
    if(obj && obj->initWithParentNode(stringList, db, parentNode, playerData))
    {
        obj->autorelease();
        return obj;
    }
    CC_SAFE_DELETE(obj);
    return NULL;
}

void RPGBattleMenu::showMenu()
{
    CCTMXTiledMap *bgLayer = (CCTMXTiledMap*)this->m_parentNode->getChildByTag(kRPGBattleMenuTagBg);
    bgLayer->setVisible(true);
    
    CCLabelTTF *nameLab = (CCLabelTTF*)this->m_parentNode->getChildByTag(kRPGBattleMenuTagNameLab);
    nameLab->setVisible(true);
    
    CCSprite *separate = (CCSprite*)this->m_parentNode->getChildByTag(kRPGBattleMenuTagSeparate);
    separate->setVisible(true);
    
    CCMenuItem *menuAttack = (CCMenuItem*)this->getChildByTag(kRPGBattleMenuTagAttack);
    menuAttack->setVisible(true);
    
    CCMenuItem *menuSkill = (CCMenuItem*)this->getChildByTag(kRPGBattleMenuTagSkill);
    menuSkill->setVisible(true);
    
    CCMenuItem *menuItems = (CCMenuItem*)this->getChildByTag(kRPGBattleMenuTagItems);
    menuItems->setVisible(true);
    
    CCMenuItem *menuEscape = (CCMenuItem*)this->getChildByTag(kRPGBattleMenuTagEscape);
    menuEscape->setVisible(true);
}

void RPGBattleMenu::hideMenu()
{
    CCTMXTiledMap *bgLayer = (CCTMXTiledMap*)this->m_parentNode->getChildByTag(kRPGBattleMenuTagBg);
    bgLayer->setVisible(false);
    
    CCLabelTTF *nameLab = (CCLabelTTF*)this->m_parentNode->getChildByTag(kRPGBattleMenuTagNameLab);
    nameLab->setVisible(false);
    
    CCSprite *separate = (CCSprite*)this->m_parentNode->getChildByTag(kRPGBattleMenuTagSeparate);
    separate->setVisible(false);
    
    CCMenuItem *menuAttack = (CCMenuItem*)this->getChildByTag(kRPGBattleMenuTagAttack);
    menuAttack->setVisible(false);
    
    CCMenuItem *menuSkill = (CCMenuItem*)this->getChildByTag(kRPGBattleMenuTagSkill);
    menuSkill->setVisible(false);
    
    CCMenuItem *menuItems = (CCMenuItem*)this->getChildByTag(kRPGBattleMenuTagItems);
    menuItems->setVisible(false);
    
    CCMenuItem *menuEscape = (CCMenuItem*)this->getChildByTag(kRPGBattleMenuTagEscape);
    menuEscape->setVisible(false);
    
}

//private

void RPGBattleMenu::createMenuItem(float x, float y, CCString *text, RPGBattleMenuTag tag)
{
    CCMenuItemLabel *menuItems = (CCMenuItemLabel*)this->getChildByTag(tag);
    if(!menuItems)
    {
        CCLabelTTF *menuItemsLabel = CCLabelTTF::create(OzgCCUtility::generalString(text->getCString()).c_str(), "Arial", 28, CCSizeMake(120, 30), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
        
        CCMenuItemLabel *menuItems = CCMenuItemLabel::create(menuItemsLabel, this, menu_selector(RPGBattleMenu::onMenu));
        menuItems->setPosition(ccp(x, y));
        menuItems->setTag(tag);
        this->addChild(menuItems);
        
    }
}

void RPGBattleMenu::onMenu(cocos2d::CCObject *pObject)
{
    CCMenuItem *menuItem = (CCMenuItem*)pObject;
    SimpleAudioEngine::sharedEngine()->playEffect("audio_effect_btn.wav");

    switch (menuItem->getTag())
    {
        case kRPGBattleMenuTagSkill:
        {
            CCLog("技能");
            
            this->hideMenu();
            
            this->m_selectedMenuTag = kRPGBattleMenuTagSkill;
            
            CCMenuItemSprite *menuCancel = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("commons_btn_back_04.png"), CCSprite::createWithSpriteFrameName("commons_btn_back_04.png"), this, menu_selector(RPGBattleMenu::onMenu));
            menuCancel->setPosition(ccp(43, 596));
            menuCancel->setTag(kRPGBattleMenuTagCancel);
            menuCancel->setScale(0.75);
            this->addChild(menuCancel);
            
            CCTMXTiledMap *selectLayer = CCTMXTiledMap::create("battle_select_style1.tmx");
            selectLayer->setPosition(ccp((CCDirector::sharedDirector()->getWinSize().width - selectLayer->getContentSize().width) / 2, (CCDirector::sharedDirector()->getWinSize().height - selectLayer->getContentSize().height) / 2));
            selectLayer->setTag(kRPGBattleSceneLayerTagSkillSelectDialog);
            
            ((RPGBattleSceneLayer*)this->m_parentNode)->addChild(selectLayer);            
            ((RPGBattleSceneLayer*)this->m_parentNode)->enabledTouched(true);
            
            //显示title和分隔线
            addLab(selectLayer, 199, (CCString*)this->m_stringList->objectForKey("skill_title"), 25, ccp(310, 285));
            CCLabelTTF *titleLab = (CCLabelTTF*)selectLayer->getChildByTag(199);
            titleLab->setFontFillColor(ccc3(144, 144, 144));
            
            CCSprite *separate = CCSprite::createWithSpriteFrameName("separate.png");
            separate->setPosition(ccp(selectLayer->getContentSize().width / 2, 260));
            separate->setScaleX(0.65);
            separate->setTag(198);
            selectLayer->addChild(separate);
            
            //加载技能数据
            CCTableView *tableView = (CCTableView*)selectLayer->getChildByTag(197);
            if(!tableView)
            {
                tableView = CCTableView::create(this, ccp(selectLayer->getContentSize().width, selectLayer->getContentSize().height - 80));
                tableView->setDirection(kCCScrollViewDirectionVertical);
                tableView->setPosition(CCSizeZero);
                tableView->setDelegate(this);
                tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
                tableView->setTag(197);
                selectLayer->addChild(tableView);
                
            }
            
            this->m_tableItems->removeAllObjects();

            string wq = "";
            JsonBox::Value json;
            json.loadFromString(this->m_playerData->m_skill.c_str());
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
                CppSQLite3Query query = this->m_db->execQuery(CCString::createWithFormat(SKILL_QUERY, wq.c_str())->getCString());
                while(!query.eof())
                {
                    RPGSkillBtnData *skill = RPGSkillBtnData::create();
                    skill->m_dataId = query.getIntField("id");
                    skill->m_name = query.getStringField("name_cns");
                    skill->m_MP = query.getIntField("mp");
                    skill->m_skillAttack = query.getIntField("skill_attack");
                    skill->m_type = query.getIntField("type");
                    skill->m_attr = query.getIntField("attr");
                    skill->m_enabled = true;
                    
                    //不能使用技能的情况
                    if(this->m_playerData->m_MP <= 0 || this->m_playerData->m_HP <= 0)
                        skill->m_enabled = false;
                    else if(this->m_playerData->m_MP < skill->m_MP)
                        skill->m_enabled = false;
                    
                    this->m_tableItems->addObject(skill);
                    
                    query.nextRow();
                }
                query.finalize();
            }
            
            tableView->reloadData();
            //加载技能数据 end
            
            ((RPGBattleSceneLayer*)this->m_parentNode)->enabledTouched(false);
        }
            break;
        case kRPGBattleMenuTagItems:
        {
            CCLog("道具");
            
            this->hideMenu();
            
            this->m_selectedMenuTag = kRPGBattleMenuTagItems;
            
            CCMenuItemSprite *menuCancel = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("commons_btn_back_04.png"), CCSprite::createWithSpriteFrameName("commons_btn_back_04.png"), this, menu_selector(RPGBattleMenu::onMenu));
            menuCancel->setPosition(ccp(43, 596));
            menuCancel->setTag(kRPGBattleMenuTagCancel);
            menuCancel->setScale(0.75);
            this->addChild(menuCancel);
            
            CCTMXTiledMap *selectLayer = CCTMXTiledMap::create("battle_select_style1.tmx");
            selectLayer->setPosition(ccp((CCDirector::sharedDirector()->getWinSize().width - selectLayer->getContentSize().width) / 2, (CCDirector::sharedDirector()->getWinSize().height - selectLayer->getContentSize().height) / 2));
            selectLayer->setTag(kRPGBattleSceneLayerTagItemsSelectDialog);
            
            ((RPGBattleSceneLayer*)this->m_parentNode)->addChild(selectLayer);
            ((RPGBattleSceneLayer*)this->m_parentNode)->enabledTouched(true);
            
            //显示title和分隔线
            addLab(selectLayer, 199, (CCString*)this->m_stringList->objectForKey("items_title"), 25, ccp(310, 285));
            CCLabelTTF *titleLab = (CCLabelTTF*)selectLayer->getChildByTag(199);
            titleLab->setFontFillColor(ccc3(144, 144, 144));
            
            CCSprite *separate = CCSprite::createWithSpriteFrameName("separate.png");
            separate->setPosition(ccp(selectLayer->getContentSize().width / 2, 260));
            separate->setScaleX(0.65);
            separate->setTag(198);
            selectLayer->addChild(separate);
            
            //加载道具数据
            CCTableView *tableView = (CCTableView*)selectLayer->getChildByTag(197);
            if(!tableView)
            {
                tableView = CCTableView::create(this, ccp(selectLayer->getContentSize().width, selectLayer->getContentSize().height - 80));
                tableView->setDirection(kCCScrollViewDirectionVertical);
                tableView->setPosition(CCSizeZero);
                tableView->setDelegate(this);
                tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
                tableView->setTag(197);
                selectLayer->addChild(tableView);
            }
            
            this->m_tableItems->removeAllObjects();
            
            CCArray *existingItems = ((RPGBattleSceneLayer*)this->m_parentNode)->m_existingItems;
            
            for (int i = 0; i < existingItems->count(); i++)
                this->m_tableItems->addObject(existingItems->objectAtIndex(i));
            
            tableView->reloadData();
            //加载道具数据 end
            
            ((RPGBattleSceneLayer*)this->m_parentNode)->enabledTouched(false);
        }
            break;
        case kRPGBattleMenuTagEscape:
        {
//            CCLog("逃跑");
            
            float success = CCRANDOM_0_1();
            if(success >= 0.0 && success <= 0.5)
            {
//                CCLog("逃跑成功");
                
                this->setEnabled(false);
                this->removeFromParentAndCleanup(true);
                
                ((RPGBattleSceneLayer*)this->m_parentNode)->goToMap();
            }
            else
            {
                ((RPGBattleSceneLayer*)this->m_parentNode)->showMsg((CCString*)this->m_stringList->objectForKey("escape_fail"));
                
                //重置进度条
                this->m_playerData->m_progress = 0.0;
                CCProgressTimer *battleProgress = (CCProgressTimer*)this->m_parentNode->getChildByTag(kRPGBattleSceneLayerTagPlayerProgress + this->m_playerData->m_dataId);
                battleProgress->setPercentage(this->m_playerData->m_progress);
                
                ((RPGBattleSceneLayer*)this->m_parentNode)->scheduleUpdate();
                
                CCTMXTiledMap *bgLayer = (CCTMXTiledMap*)this->m_parentNode->getChildByTag(kRPGBattleMenuTagBg);
                bgLayer->removeFromParentAndCleanup(true);
                this->removeFromParentAndCleanup(true);
            }
            
        }
            break;
        case kRPGBattleMenuTagCancel:
        {
//            CCLog("取消");
            this->showMenu();
            
            CCMenuItem *menuCancel = (CCMenuItem*)this->getChildByTag(kRPGBattleMenuTagCancel);
            menuCancel->removeFromParentAndCleanup(true);
            
            switch (this->m_selectedMenuTag)
            {
                case kRPGBattleMenuTagAttack:
                    CCLog("取消攻击");
                    ((RPGBattleSceneLayer*)this->m_parentNode)->enabledTouched(false);
                    ((RPGBattleSceneLayer*)this->m_parentNode)->cancelAllSelected();
                    
                    break;
                case kRPGBattleMenuTagSkill:
                    CCLog("取消技能列表");
                    
                    if(((RPGBattleSceneLayer*)this->m_parentNode)->getChildByTag(kRPGBattleSceneLayerTagSkillSelectDialog))
                        ((RPGBattleSceneLayer*)this->m_parentNode)->removeChildByTag(kRPGBattleSceneLayerTagSkillSelectDialog, true);
                    
                    ((RPGBattleSceneLayer*)this->m_parentNode)->enabledTouched(false);
                    ((RPGBattleSceneLayer*)this->m_parentNode)->cancelAllSelected();
                    
                    break;
                case kRPGBattleMenuTagItems:
                    CCLog("取消道具列表");
                    
                    if(((RPGBattleSceneLayer*)this->m_parentNode)->getChildByTag(kRPGBattleSceneLayerTagItemsSelectDialog))
                        ((RPGBattleSceneLayer*)this->m_parentNode)->removeChildByTag(kRPGBattleSceneLayerTagItemsSelectDialog, true);
                    
                    ((RPGBattleSceneLayer*)this->m_parentNode)->enabledTouched(false);
                    ((RPGBattleSceneLayer*)this->m_parentNode)->cancelAllSelected();
                    
                    break;
//                default:
//                    break;
            }
            
        }
            break;
        case kRPGBattleMenuTagCancel2:
        {
            //这个选择了一个技能或道具后，到了选择目标对象的一步，使用到的取消按钮
            
            CCMenuItemSprite *menuCancel = (CCMenuItemSprite*)this->getChildByTag(kRPGBattleMenuTagCancel);
            menuCancel->setVisible(true);
            
            CCMenuItemSprite *menuCancel2 = (CCMenuItemSprite*)this->getChildByTag(kRPGBattleMenuTagCancel2);
            menuCancel2->removeFromParentAndCleanup(true);
            
            if(this->m_parentNode->getChildByTag(kRPGBattleSceneLayerTagItemsSelectDialog))
                this->m_parentNode->getChildByTag(kRPGBattleSceneLayerTagItemsSelectDialog)->setVisible(true);
            
            if(this->m_parentNode->getChildByTag(kRPGBattleSceneLayerTagSkillSelectDialog))
                this->m_parentNode->getChildByTag(kRPGBattleSceneLayerTagSkillSelectDialog)->setVisible(true);
            
            ((RPGBattleSceneLayer*)this->m_parentNode)->enabledTouched(false);
            ((RPGBattleSceneLayer*)this->m_parentNode)->cancelAllSelected();
        }
            break;
        default:
        {
            CCLog("攻击");
            this->hideMenu();
            
            this->m_selectedMenuTag = kRPGBattleMenuTagAttack;
            
            CCMenuItemSprite *menuCancel = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("commons_btn_back_04.png"), CCSprite::createWithSpriteFrameName("commons_btn_back_04.png"), this, menu_selector(RPGBattleMenu::onMenu));
            menuCancel->setPosition(ccp(43, 596));
            menuCancel->setTag(kRPGBattleMenuTagCancel);
            menuCancel->setScale(0.75);
            this->addChild(menuCancel);
            
            ((RPGBattleSceneLayer*)this->m_parentNode)->enabledTouched(true);
            
        }
            break;
    }
    
}

void RPGBattleMenu::onButton(cocos2d::CCObject *pSender, CCControlEvent event)
{
    SimpleAudioEngine::sharedEngine()->playEffect("audio_effect_btn.wav");

    CCMenuItemSprite *menuCancel = (CCMenuItemSprite*)this->getChildByTag(kRPGBattleMenuTagCancel);
    menuCancel->setVisible(false);
    
    CCMenuItemSprite *menuCancel2 = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("commons_btn_back_04.png"), CCSprite::createWithSpriteFrameName("commons_btn_back_04.png"), this, menu_selector(RPGBattleMenu::onMenu));
    menuCancel2->setPosition(ccp(43, 596));
    menuCancel2->setTag(kRPGBattleMenuTagCancel2);
    menuCancel2->setScale(0.75);
    this->addChild(menuCancel2);
    
    if(this->m_parentNode->getChildByTag(kRPGBattleSceneLayerTagItemsSelectDialog))
        this->m_parentNode->getChildByTag(kRPGBattleSceneLayerTagItemsSelectDialog)->setVisible(false);
    
    if(this->m_parentNode->getChildByTag(kRPGBattleSceneLayerTagSkillSelectDialog))
        this->m_parentNode->getChildByTag(kRPGBattleSceneLayerTagSkillSelectDialog)->setVisible(false);
    
    ((RPGBattleSceneLayer*)this->m_parentNode)->enabledTouched(true);
    
    CCUserDefault::sharedUserDefault()->setIntegerForKey(GAME_BATTLE_SELECTED_ID, ((CCControlButton*)pSender)->getTag());
}

//CCTableView
void RPGBattleMenu::scrollViewDidScroll(CCScrollView *scrollView)
{
    
}

void RPGBattleMenu::scrollViewDidZoom(CCScrollView *scrollView)
{
    
}

void RPGBattleMenu::tableCellTouched(CCTableView *tableView, CCTableViewCell *cell)
{
    
}

CCSize RPGBattleMenu::cellSizeForTable(CCTableView *tableView)
{
    return CCSizeMake(tableView->getContentSize().width, 50);
}

CCTableViewCell* RPGBattleMenu::tableCellAtIndex(CCTableView *tableView, unsigned int idx)
{
    CCTableViewCell *cell = tableView->dequeueCell();
    if (!cell)
    {
        cell = new CCTableViewCell();
        cell->autorelease();
    }
    else
        cell->removeAllChildrenWithCleanup(true);
    
    if(dynamic_cast<RPGSkillBtnData*>(this->m_tableItems->objectAtIndex(idx)) != NULL)
    {
        //点击了技能项
        RPGSkillBtnData *itemsData = (RPGSkillBtnData*)this->m_tableItems->objectAtIndex(idx);
        
        CCControlButton *itemBtn = CCControlButton::create(CCString::createWithFormat("%s (%i)", itemsData->m_name.c_str(), itemsData->m_MP)->getCString(), "Arial", 22);
        itemBtn->setPosition(ccp(tableView->getContentSize().width / 2, 0));
        itemBtn->setTag(itemsData->m_dataId);
        itemBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(RPGBattleMenu::onButton), CCControlEventTouchUpInside);
        cell->addChild(itemBtn);
        
    }
    else if(dynamic_cast<RPGExistingItems*>(this->m_tableItems->objectAtIndex(idx)) != NULL)
    {
        //点击了道具项
        
        RPGExistingItems *itemsData = (RPGExistingItems*)this->m_tableItems->objectAtIndex(idx);
        
        CCControlButton *itemBtn = CCControlButton::create(CCString::createWithFormat("%s (%i)", itemsData->m_name.c_str(), itemsData->m_total)->getCString(), "Arial", 22);
        itemBtn->setPosition(ccp(tableView->getContentSize().width / 2, 0));
        itemBtn->setTag(itemsData->m_dataId);
        itemBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(RPGBattleMenu::onButton), CCControlEventTouchUpInside);
        cell->addChild(itemBtn);
        
    }
    
    return cell;
}

unsigned int RPGBattleMenu::numberOfCellsInTableView(CCTableView *tableView)
{
    return this->m_tableItems->count();
}
//CCTableView end
