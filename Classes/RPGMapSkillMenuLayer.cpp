//
//  RPGMapSkillMenuLayer.cpp
//  OzgGameRPG
//
//  Created by ozg on 14-4-10.
//
//

#include "RPGMapSkillMenuLayer.h"
#include "RPGMapMenuLayer.h"
#include "RPGMapSceneLayer.h"
#include "RPGMapChoicePlayerMenuLayer.h"
#include "OzgCCUtility.h"

RPGMapSkillMenuLayer::RPGMapSkillMenuLayer()
{
    
}

RPGMapSkillMenuLayer::~RPGMapSkillMenuLayer()
{
    this->m_stringList->release();
    this->m_skillList->release();
    
    CCLog("RPGMapSkillMenuLayer 释放");
}

bool RPGMapSkillMenuLayer::init(cocos2d::CCDictionary *stringList, CppSQLite3DB *db, float width, float height)
{
    if(CCLayerColor::initWithColor(ccc4(0, 0, 0, 200), width, height))
    {
        this->m_stringList = stringList;
        this->m_stringList->retain();
        
        this->m_db = db;
        
        this->m_skillList = new CCArray();
        this->m_skillList->init();
        
        this->m_isDefault = true;
        
        CCMenu *mainMenu = CCMenu::create();
        mainMenu->setTag(kRPGMapSkillMenuLayerTagMainMenu);
        mainMenu->setAnchorPoint(CCPointZero);
        mainMenu->setPosition(CCPointZero);
        this->addChild(mainMenu);
        
        CCMenuItemSprite *menuBack = (CCMenuItemSprite*)mainMenu->getChildByTag(kRPGMapSkillMenuLayerTagMainMenuBack);
        if(!menuBack)
        {
            menuBack = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("commons_btn_back_04.png"), CCSprite::createWithSpriteFrameName("commons_btn_back_04.png"), this, menu_selector(RPGMapSkillMenuLayer::onMenu));
            menuBack->setPosition(ccp(40, 600));
            menuBack->setTag(kRPGMapSkillMenuLayerTagMainMenuBack);
            menuBack->setScale(0.5);
            mainMenu->addChild(menuBack);
        }
        
        CCTMXTiledMap *mainBg = CCTMXTiledMap::create("map_menu5_style1.tmx");
        mainBg->setPosition(CCPointZero);
        mainBg->setTag(kRPGMapSkillMenuLayerTagBg);
        this->addChild(mainBg);
        
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
            
            CCMenuItemSprite *player = (CCMenuItemSprite*)mainMenu->getChildByTag(kRPGMapSkillMenuLayerTagMainMenuPlayer + query.getIntField("id"));
            if(!player)
            {
                player = CCMenuItemSprite::create(playerSprite, playerSprite, this, menu_selector(RPGMapSkillMenuLayer::onMenu));
                player->setPosition(ccp(playerX, 560));
                player->setScale(2);
                player->setTag(kRPGMapSkillMenuLayerTagMainMenuPlayer + query.getIntField("id"));
                mainMenu->addChild(player);
            }
            
            playerX += 200;
            query.nextRow();
        }
        query.finalize();
        
        //默认选中第一个
        this->onMenu(mainMenu->getChildByTag(kRPGMapSkillMenuLayerTagMainMenuPlayer + 1));
        
        return true;
    }
    return false;
}

RPGMapSkillMenuLayer* RPGMapSkillMenuLayer::create(cocos2d::CCDictionary *stringList, CppSQLite3DB *db, float width, float height)
{
    RPGMapSkillMenuLayer *obj = new RPGMapSkillMenuLayer();
    if(obj && obj->init(stringList, db, width, height))
    {
        obj->autorelease();
        return obj;
    }
    CC_SAFE_DELETE(obj);
    return NULL;
}

//CCTableView
void RPGMapSkillMenuLayer::scrollViewDidScroll(CCScrollView *scrollView)
{
    
}

void RPGMapSkillMenuLayer::scrollViewDidZoom(CCScrollView *scrollView)
{
    
}

void RPGMapSkillMenuLayer::tableCellTouched(CCTableView *tableView, CCTableViewCell *cell)
{
    
}

CCSize RPGMapSkillMenuLayer::cellSizeForTable(CCTableView *tableView)
{
    return CCSizeMake(tableView->getContentSize().width, 80);
}

CCTableViewCell* RPGMapSkillMenuLayer::tableCellAtIndex(CCTableView *tableView, unsigned int idx)
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
    for (int i = 0; i < 5; i++)
    {
        int index = idx * 5 + i;
        
        if(index >= this->m_skillList->count())
            break;
        
        RPGSkillBtnData *skillData = (RPGSkillBtnData*)this->m_skillList->objectAtIndex(index);
        
        CCControlButton *skillBtn = CCControlButton::create(CCString::createWithFormat("%s (%i)", skillData->m_name.c_str(), skillData->m_MP)->getCString(), "Arial", 22);
        skillBtn->setPosition(ccp(x, 0));
        skillBtn->setTag(skillData->m_dataId);
        skillBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(RPGMapSkillMenuLayer::onButton), CCControlEventTouchUpInside);
        cell->addChild(skillBtn);
        
        //如果是非回复类技能的话，则禁用点击
        if(!skillData->m_enabled)
        {
            skillBtn->setTitleColorForState(ccc3(144, 144, 144), CCControlStateNormal);
            skillBtn->setEnabled(false);
        }
        
        x += 170;
    }
    
    return cell;
}

unsigned int RPGMapSkillMenuLayer::numberOfCellsInTableView(CCTableView *tableView)
{
    int rowItemCount = 5;
    int size = this->m_skillList->count();
    
    if(size % rowItemCount == 0)
        return size / rowItemCount;
    else
        return size / rowItemCount + 1;
}
//CCTableView end

//private
void RPGMapSkillMenuLayer::onMenu(cocos2d::CCObject *pObject)
{
    //第一次显示数据的话则不播放效果音
    if(!this->m_isDefault)
        SimpleAudioEngine::sharedEngine()->playEffect("audio_effect_btn.wav");
    
    CCMenuItem *menuItem = (CCMenuItem*)pObject;
    
    switch (menuItem->getTag())
    {
        case kRPGMapSkillMenuLayerTagMainMenuBack:
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
    
    if(menuItem->getTag() >= kRPGMapSkillMenuLayerTagMainMenuPlayer && menuItem->getTag() <= kRPGMapSkillMenuLayerTagMainMenuPlayer + 99)
    {
//        CCLog("点击了上面4个player的其中一个");
        CCMenu *mainMenu = (CCMenu*)this->getChildByTag(kRPGMapSkillMenuLayerTagMainMenu);
        
        CppSQLite3Query query = this->m_db->execQuery(PLAYER_QUERY);
        while(!query.eof())
        {
            if(menuItem->getTag() == kRPGMapSkillMenuLayerTagMainMenuPlayer + query.getIntField("id"))
            {
                this->m_isDefault = false;
                
                int dataId = query.getIntField("id");
                this->setPlayerSkill(dataId);
                
                //选中的光标部分
                CCMenuItemSprite *player = (CCMenuItemSprite*)mainMenu->getChildByTag(kRPGMapSkillMenuLayerTagMainMenuPlayer + dataId);
                
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

void RPGMapSkillMenuLayer::setPlayerSkill(int dataId)
{
    CCTableView *tableView = (CCTableView*)this->getChildByTag(kRPGMapSkillMenuLayerTagItemListTable);
    if(!tableView)
    {
        tableView = CCTableView::create(this, CCSizeMake(850, 420));
        tableView->setDirection(kCCScrollViewDirectionVertical);
        tableView->setPosition(ccp(30, 30));
        tableView->setDelegate(this);
        tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
        tableView->setTag(kRPGMapSkillMenuLayerTagItemListTable);
        this->addChild(tableView);
    }
    
    CppSQLite3Query query = this->m_db->execQuery(CCString::createWithFormat(PLAYER_DETAIL_QUERY, dataId)->getCString());
    while(!query.eof())
    {
        this->m_selectedPlayerId = query.getIntField("id");
        
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
                RPGSkillBtnData *skill = RPGSkillBtnData::create();
                skill->m_dataId = skillQuery.getIntField("id");
                skill->m_name = skillQuery.getStringField("name_cns");
                skill->m_MP = skillQuery.getIntField("mp");
                skill->m_skillAttack = skillQuery.getIntField("skill_attack");
                skill->m_type = skillQuery.getIntField("type");
                skill->m_attr = skillQuery.getIntField("attr");
                skill->m_enabled = true;
                
                //不能使用技能的情况
                if(query.getFloatField("mp") <= 0 || query.getFloatField("hp") <= 0)
                    skill->m_enabled = false;
                else if(query.getFloatField("mp") < skill->m_MP)
                    skill->m_enabled = false;
                else if(skill->m_type != 2)
                    skill->m_enabled = false;
                
                this->m_skillList->addObject(skill);
                
                skillQuery.nextRow();
            }
            skillQuery.finalize();
        }
        
        tableView->reloadData();
        
        query.nextRow();
    }
    query.finalize();
    
}

void RPGMapSkillMenuLayer::onButton(cocos2d::CCObject *pSender, CCControlEvent event)
{
    SimpleAudioEngine::sharedEngine()->playEffect("audio_effect_btn.wav");
    
    CCControlButton *skillBtn = (CCControlButton*)pSender;
    for (int i = 0; i < this->m_skillList->count(); i++)
    {
        //判断选中道具
        RPGSkillBtnData *skillData = (RPGSkillBtnData*)this->m_skillList->objectAtIndex(i);
        if(skillBtn->getTag() == skillData->m_dataId)
        {
            this->m_selectedSkill = skillData;
            break;
        }
    }
    
    //因为动态获取地图的大小会导致了菜单层显示错位，所以定死了
    float width = 960;
    float height = 640;
    
    //临时背景
    CCTMXTiledMap *mainBg = CCTMXTiledMap::create("map_menu3_style1.tmx");
    mainBg->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width / 2, CCDirector::sharedDirector()->getWinSize().height / 2));
    mainBg->setAnchorPoint(ccp(0.5, 0.5));
    mainBg->setTag(kRPGMapSceneLayerTagChoicePlayerMenuLayerBg);
    this->getParent()->addChild(mainBg);
    
    CCString *title = CCString::createWithFormat(((CCString*)this->m_stringList->objectForKey("menu_skill_choice"))->getCString(), this->m_selectedSkill->m_name.c_str());
    RPGMapChoicePlayerMenuLayer *choicePlayer = RPGMapChoicePlayerMenuLayer::create(this->m_db, title, this, callfuncO_selector(RPGMapSkillMenuLayer::onChoicePlayer), width, height);
    choicePlayer->setTag(kRPGMapSceneLayerTagChoicePlayerMenuLayer);
    this->getParent()->addChild(choicePlayer);
    
    this->setVisible(false);
}

void RPGMapSkillMenuLayer::onChoicePlayer(CCObject* pObject)
{
    SimpleAudioEngine::sharedEngine()->playEffect("audio_effect_btn.wav");
    
    CCMenuItem *menuItem = (CCMenuItem*)pObject;
    if(menuItem->getTag() == kRPGMapChoicePlayerMenuLayerTagMainMenuBack)
    {
        //按了退出
        this->getParent()->removeChildByTag(kRPGMapSceneLayerTagChoicePlayerMenuLayerBg, true);
        this->getParent()->removeChildByTag(kRPGMapSceneLayerTagChoicePlayerMenuLayer, true);
        this->setVisible(true);
    }
    else
    {
        
        bool isSuccess = RPGResultsLogic::useSkillCure(this->m_db, this->m_selectedPlayerId, menuItem->getTag() - kRPGMapChoicePlayerMenuLayerTagMainMenuPlayer, this->m_selectedSkill->m_dataId);
        if(!isSuccess)
        {
            CCLog("技能使用失败");
            CCSize winSize = CCDirector::sharedDirector()->getWinSize();
            RPGDialogLayer *dialog = RPGDialogLayer::create(((CCString*)this->m_stringList->objectForKey("menu_skill_fail"))->getCString(), ((CCString*)this->m_stringList->objectForKey("confirm_exit_ok"))->getCString(), kRPGMapSkillMenuLayerTagDialogOK, winSize.width, winSize.height, this, menu_selector(RPGMapSkillMenuLayer::onDialog));
            dialog->setTag(kRPGMapSkillMenuLayerTagDialog);
            this->getParent()->addChild(dialog);

        }
        else
        {
            CCLog("技能使用成功");
            this->getParent()->removeChildByTag(kRPGMapSceneLayerTagChoicePlayerMenuLayerBg, true);
            this->getParent()->removeChildByTag(kRPGMapSceneLayerTagChoicePlayerMenuLayer, true);
            this->setVisible(true);
            
            this->setPlayerSkill(this->m_selectedPlayerId); //刷新正在显示的技能数据
        }
    }
    
}

void RPGMapSkillMenuLayer::onDialog(cocos2d::CCObject *pObject)
{
    this->getParent()->removeChildByTag(kRPGMapSkillMenuLayerTagDialog, true);
    this->getParent()->removeChildByTag(kRPGMapSceneLayerTagChoicePlayerMenuLayerBg, true);
    this->getParent()->removeChildByTag(kRPGMapSceneLayerTagChoicePlayerMenuLayer, true);
    this->setVisible(true);
}
