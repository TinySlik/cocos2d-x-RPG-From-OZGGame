//
//  RPGBattleSceneLayer.cpp
//  OzgGameRPG
//
//  Created by ozg on 14-3-18.
//
//

#include "RPGBattleSceneLayer.h"
#include "RPGLoadingSceneLayer.h"
#include "RPGComputingResults.h"
#include "RPGStartSceneLayer.h"
#include "RPGResultsLogic.h"

#define MONSTER_QUERY "select * from monster where map_id = %i order by random() limit 1" //随机出现怪物

#define SAVE_GOLD "update save_data set gold = %i where id = 1" //保存金钱
#define UPDATE_PLAYER "update player set hp = %i, mp = %i, attack = %f, defense = %f, speed = %f, skill_attack = %f, skill_defense = %f, level = %i, skill = '%s', next_exp = %i, exp = %i where id = %i" //更新player数据，升级时用到
#define UPDATE_EXISTING_ITEMS "update items_existing set total = %i where id = %i" //更新已有道具

RPGBattleSceneLayer::RPGBattleSceneLayer()
{
    
}

RPGBattleSceneLayer::~RPGBattleSceneLayer()
{
    this->m_playerDataList->release();
    this->m_monsterDataList->release();
    this->m_playerList->release();
    this->m_monsterList->release();
    this->m_existingItems->release();
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("monsters.plist");
    
//    CCTextureCache::sharedTextureCache()->dumpCachedTextureInfo();
    CCLog("RPGBattleSceneLayer 释放");
}

bool RPGBattleSceneLayer::init()
{
    if(RPGBaseSceneLayer::init())
    {
        CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("monsters.plist");
        
        //加载语言文件
        string languageFile = CCFileUtils::sharedFileUtils()->fullPathForFilename("scene_battle_cns.plist");
        this->m_stringList = CCDictionary::createWithContentsOfFileThreadSafe(languageFile.c_str());
        this->m_stringList->retain();
        
        CCSprite *bg = CCSprite::create("battle_bg.png");
        bg->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width / 2, CCDirector::sharedDirector()->getWinSize().height / 2));
        bg->setTag(kRPGBattleSceneLayerTagBg);
        this->addChild(bg);
        
        //背景音乐
        if(SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
        {
            SimpleAudioEngine::sharedEngine()->stopBackgroundMusic(true);
            SimpleAudioEngine::sharedEngine()->playBackgroundMusic("audio_battle.mp3", true);
        }
        
        //下面的菜单
        CCTMXTiledMap *menuLayer = CCTMXTiledMap::create(CCString::createWithFormat("battle_menu_%s.tmx", CCUserDefault::sharedUserDefault()->getStringForKey(GAME_STYLE).c_str())->getCString());
        menuLayer->setTag(kRPGBattleSceneLayerTagMenuBg);
        menuLayer->setPosition(ccp((CCDirector::sharedDirector()->getWinSize().width - menuLayer->getContentSize().width) / 2.0, (CCDirector::sharedDirector()->getWinSize().height - bg->getContentSize().height) / 2));
        this->addChild(menuLayer);
        
        this->m_playerDataList = new CCArray();
        this->m_playerDataList->init();
        
        this->m_monsterDataList = new CCArray();
        this->m_monsterDataList->init();
        
        this->m_playerList = new CCArray();
        this->m_playerList->init();
        
        this->m_monsterList = new CCArray();
        this->m_monsterList->init();
        
        this->m_speedTotal = 0; //速度总值的初始化
        
        //加载已有道具
        this->m_existingItems = new CCArray();
        this->m_existingItems->init();
        
        CppSQLite3Query itemsQuery = this->m_db.execQuery(CCString::createWithFormat(ITEMS_EXISTING_QUERY_TYPE, 3)->getCString());
        while(!itemsQuery.eof())
        {
            RPGExistingItems *itemsData = RPGExistingItems::create();
            itemsData->m_dataId = itemsQuery.getIntField("id");
            itemsData->m_name = itemsQuery.getStringField("name_cns");
            itemsData->m_buy = itemsQuery.getIntField("buy");
            itemsData->m_sell = itemsQuery.getIntField("sell");
            itemsData->m_type = itemsQuery.getIntField("type");
            itemsData->m_attack = itemsQuery.getFloatField("attack");
            itemsData->m_defense = itemsQuery.getFloatField("defense");
            itemsData->m_speed = itemsQuery.getFloatField("speed");
            itemsData->m_skillAttack = itemsQuery.getFloatField("skill_attack");
            itemsData->m_skillDefense = itemsQuery.getFloatField("skill_defense");
            itemsData->m_total = itemsQuery.getIntField("total");
            this->m_existingItems->addObject(itemsData);
            
            itemsQuery.nextRow();
        }
        itemsQuery.finalize();
        //加载已有道具 end
        
        //player
        float playerY = 556;
        float separateY = 133;
        float playerLabY = 151;
        float progressY = 153;
        
        int i = 0;
        
        this->m_playerDataList->removeAllObjects();
        CppSQLite3Query query = this->m_db.execQuery(PLAYER_QUERY);
        while(!query.eof())
        {
            RPGPlayer *playerData = RPGPlayer::create();
            playerData->m_dataId = query.getIntField("id");
            playerData->m_maxHP = query.getIntField("max_hp");
            playerData->m_HP = query.getIntField("hp");
            playerData->m_maxMP = query.getIntField("max_mp");
            playerData->m_MP = query.getIntField("mp");
            playerData->m_attack = query.getFloatField("attack");
            playerData->m_defense = query.getFloatField("defense");
            playerData->m_speed = query.getFloatField("speed");
            playerData->m_skillAttack = query.getFloatField("skill_attack");
            playerData->m_skillDefense = query.getFloatField("skill_defense");
            playerData->m_level = query.getIntField("level");
            playerData->m_name = query.getStringField("name_cns");
            playerData->m_nextExp = query.getIntField("next_exp");
            playerData->m_itemsIdArmor = query.getIntField("items_id_armor");
            playerData->m_itemsIdArms = query.getIntField("items_id_arms");
            playerData->m_skill = query.getStringField("skill");
            playerData->m_texPrefix = query.getStringField("tex_prefix");
            playerData->m_exp = query.getIntField("exp");
            playerData->m_progress = 0.0; //computingProgress方法会重新计算这个值
            playerData->m_status = kRPGDataStatusNormal;
            
            if(playerData->m_HP <= 0)
                playerData->m_status = kRPGDataStatusDeath;
            
            playerData->m_speed = this->computingFloat(playerData->m_speed);
            this->m_speedTotal += playerData->m_speed; //累加速度总值
            
            this->m_playerDataList->addObject(playerData);
            
            //角色
            RPGBattlePlayerSprite *player = RPGBattlePlayerSprite::createWithPlayerData(playerData);
            player->setPosition(ccp(848, playerY));
            player->setTag(kRPGBattleSceneLayerTagPlayer + playerData->m_dataId);
            this->addChild(player);
            
            this->m_playerList->addObject(player);
            
            //分隔线
            if(i > 0)
            {
                CCSprite *separate = CCSprite::createWithSpriteFrameName("separate.png");
                separate->setPosition(ccp(633, separateY));
                separate->setScaleX(1.4);
                this->addChild(separate);
                
                separateY -= 40;
            }
            
            //名字
            addLab(this, kRPGBattleSceneLayerTagPlayerNameLab + playerData->m_dataId, CCString::create(playerData->m_name), 18, ccp(543, playerLabY));
            
            //进度条
            CCSprite *battleProgressBg = CCSprite::createWithSpriteFrameName("gui_battle_progress_bar01_a.png");
            battleProgressBg->setTag(kRPGBattleSceneLayerTagPlayerProgressBg + playerData->m_dataId);
            battleProgressBg->setPosition(ccp(848, progressY));
            battleProgressBg->setScaleX(0.4);
            battleProgressBg->setScaleY(0.5);
            this->addChild(battleProgressBg);
            
            CCProgressTimer *battleProgress = CCProgressTimer::create(CCSprite::createWithSpriteFrameName("gui_battle_progress_bar01_b.png"));
            battleProgress->setBarChangeRate(ccp(1, 0));    //设置进度条的长度和高度开始变化的大小
            battleProgress->setType(kCCProgressTimerTypeBar);    //设置进度条为水平
            battleProgress->setMidpoint(ccp(0, 0));
            battleProgress->setPosition(ccp(848, progressY));
            battleProgress->setScaleX(0.4);
            battleProgress->setScaleY(0.5);
            battleProgress->setTag(kRPGBattleSceneLayerTagPlayerProgress + playerData->m_dataId);
            this->addChild(battleProgress);
            
            //初始化进度条的进度
//            float progress = this->m_roleListWithInitProgress[role->dataId] * 100;
//            battleProgress->setPercentage(50);
            //CCLog("%f", progress);
            
            //HP
            addLab(this, kRPGBattleSceneLayerTagPlayerHP + playerData->m_dataId, CCString::createWithFormat("%i", playerData->m_HP), 18, kCCTextAlignmentRight, ccp(348, playerLabY));
            addLab(this, kRPGBattleSceneLayerTagPlayerHPSeparate + playerData->m_dataId, CCString::create("/"), 18, kCCTextAlignmentCenter, ccp(558, playerLabY));
            addLab(this, kRPGBattleSceneLayerTagPlayerMaxHP + playerData->m_dataId, CCString::createWithFormat("%i", playerData->m_maxHP), 18, kCCTextAlignmentLeft, ccp(768, playerLabY));
            
            //MP
            addLab(this, kRPGBattleSceneLayerTagPlayerMP + playerData->m_dataId, CCString::createWithFormat("%i", playerData->m_MP), 15, kCCTextAlignmentRight, ccp(478, playerLabY - 3));
            addLab(this, kRPGBattleSceneLayerTagPlayerMPSeparate + playerData->m_dataId, CCString::create("/"), 15, kCCTextAlignmentCenter, ccp(688, playerLabY - 3));
            addLab(this, kRPGBattleSceneLayerTagPlayerMaxMP + playerData->m_dataId, CCString::createWithFormat("%i", playerData->m_maxMP), 15, kCCTextAlignmentLeft, ccp(898, playerLabY - 3));
            
            playerY -= 100;
            playerLabY -= 40;
            progressY -= 40;
            
            i++;
            query.nextRow();
        }
        query.finalize();
        
        //怪物部分
        
        //第一次调用randomFloat的结果都是一样的，所以不需要
        OzgCCUtility::randomSeed(0);
        OzgCCUtility::randomFloat(0.0f, 1.0f);
        
        //生成怪物数据部分
        int mapId = CCUserDefault::sharedUserDefault()->getIntegerForKey("map_id");

        OzgCCUtility::rangeRand(1, 10);
        int monsterCount = OzgCCUtility::rangeRand(1, 4); //最多出现1到4种怪物
        while (monsterCount > 0)
        {
            //随机查询一个怪物
            CCString *sql = CCString::createWithFormat(MONSTER_QUERY, mapId);
            
            CppSQLite3Query query = this->m_db.execQuery(sql->getCString());
            while(!query.eof())
            {
                RPGMonster *monsterData = RPGMonster::create();
                monsterData->m_dataId = query.getIntField("id");
                monsterData->m_name = query.getStringField("name_cns");
                monsterData->m_maxHP = query.getIntField("max_hp");
                monsterData->m_HP = query.getIntField("max_hp"); //跟上面的一样
                monsterData->m_maxMP = query.getIntField("max_mp");
                monsterData->m_MP = query.getIntField("max_mp"); //跟上面的一样
                monsterData->m_attack = query.getFloatField("attack");
                monsterData->m_defense = query.getFloatField("defense");
                monsterData->m_speed = query.getFloatField("speed");
                monsterData->m_skillAttack = query.getFloatField("skill_attack");
                monsterData->m_skillDefense = query.getFloatField("skill_defense");
                monsterData->m_exp = query.getIntField("exp");
                monsterData->m_skill = query.getStringField("skill");
                monsterData->m_tex = query.getStringField("tex");
                monsterData->m_gold = query.getIntField("gold");
                monsterData->m_progress = 0.0; //computingProgress方法会重新计算这个值
                
                monsterData->m_speed = this->computingFloat(monsterData->m_speed);
                this->m_speedTotal += monsterData->m_speed; //累加速度总值
                
                CCArray *monsters = this->monsterDataListWithDataId(monsterData->m_dataId);
                if(monsters->count() > 0)
                {
                    do
                    {
                        monsterData->m_tag = kRPGBattleSceneLayerTagMonster + OzgCCUtility::rangeRand(0, 9999);
                    }
                    while (this->monsterDataListExistWithTag(monsterData->m_tag));
                    
                    monsters->addObject(monsterData);
                    
//                    CCLog("产生无重复的tag");
                }
                else
                {
                    monsterData->m_tag = kRPGBattleSceneLayerTagMonster + OzgCCUtility::rangeRand(0, 9999);
                    
                    monsters->addObject(monsterData);
                    this->m_monsterDataList->addObject(monsters);
                    
//                    CCLog("如果不存在对应的怪物则直接插入到怪物数组");
                }
                
//                CCLog("产生敌人：%i", monsterData->m_tag);
                
                query.nextRow();
            }
            query.finalize();
            
            monsterCount--;
        }

        //怪物显示部分
        float monsterNameLabY = 156;

        this->m_addedMonsters.clear();
        
        for (i = 0; i < this->m_monsterDataList->count(); i++)
        {
            CCArray *monsters = (CCArray*)this->m_monsterDataList->objectAtIndex(i);
            for (int j = 0; j < monsters->count(); j++)
            {
                RPGMonster *monsterData = (RPGMonster*)monsters->objectAtIndex(j);
                RPGBattleMonsterSprite *monster = RPGBattleMonsterSprite::createWithMonsterData(monsterData);
                this->setMonsterPosition(monster);
                monster->setTag(monsterData->m_tag);
                this->addChild(monster);
                
                this->m_addedMonsters.push_back(monster->getTag());
                
                this->m_monsterList->addObject(monster);
                
                //左下角的怪物名称
                if(j == 0)
                {
                    addLab(this, kRPGBattleSceneLayerTagMonsterNameLab + monsterData->m_dataId, CCString::createWithFormat("%s x %i", monsterData->m_name.c_str(), (int)monsters->count()), 18, ccp(238, monsterNameLabY));
                    
                    monsterNameLabY -= 40;
                }
            }
            
        }
        
        //怪物部分结束
        
        this->m_enabledTouched = false;
        
        this->computingProgress();
                
        this->scheduleUpdate();
        
        return true;
    }
    return false;
}

CCScene* RPGBattleSceneLayer::scene()
{
    CCScene* s = CCScene::create();
    RPGBattleSceneLayer *layer = RPGBattleSceneLayer::create();
    s->addChild(layer);
    return s;
}

void RPGBattleSceneLayer::update(float delta)
{
    float progressSpeed = 1.0;
    
    //更新player进度条
    for (int i = 0; i < this->m_playerDataList->count(); i++)
    {
        RPGPlayer *playerData = (RPGPlayer*)this->m_playerDataList->objectAtIndex(i);
        
        if(playerData->m_status == kRPGDataStatusDeath)
        {
            //战斗不能
            CCProgressTimer *battleProgress = (CCProgressTimer*)this->getChildByTag(kRPGBattleSceneLayerTagPlayerProgress + playerData->m_dataId);
            battleProgress->setPercentage(0);
            continue;
        }
        else
        {
            //通常时
            
            playerData->m_progress += progressSpeed;
            
            if(playerData->m_progress > 100)
                playerData->m_progress = 100;
            
            CCProgressTimer *battleProgress = (CCProgressTimer*)this->getChildByTag(kRPGBattleSceneLayerTagPlayerProgress + playerData->m_dataId);
            battleProgress->setPercentage(playerData->m_progress);
        }
        
        if(playerData->m_progress >= 100)
        {
            CCLog("显示player菜单");
            
            RPGBattleMenu *battleMenu = (RPGBattleMenu*)this->getChildByTag(kRPGBattleSceneLayerTagBattleMenu);
            if(battleMenu)
                battleMenu->removeFromParentAndCleanup(true);
            
            battleMenu = RPGBattleMenu::createWithParentNode(this->m_stringList, &this->m_db, this, playerData);
            battleMenu->setTag(kRPGBattleSceneLayerTagBattleMenu);
            this->addChild(battleMenu);
            
            this->unscheduleUpdate();
            return;
        }
        
    }
    
    //更新怪物的进度
    for (int i = 0; i < this->m_monsterDataList->count(); i++)
    {
        CCArray *monsters = (CCArray*)this->m_monsterDataList->objectAtIndex(i);
        for (int j = 0; j < monsters->count(); j++)
        {
            RPGMonster *monsterData = (RPGMonster*)monsters->objectAtIndex(j);
            
            //对于已被打倒的怪物，则跳过进度条的处理
            if(monsterData->m_status == kRPGDataStatusDeath)
                continue;
            
            RPGBattleMonsterSprite *monster = (RPGBattleMonsterSprite*)this->getChildByTag(monsterData->m_tag);
            
            monsterData->m_progress += progressSpeed;
            
            if(monsterData->m_progress > 100)
                monsterData->m_progress = 100;
            
            if(monsterData->m_progress >= 100)
            {
                CCLog("怪物攻击");
                
                //计算攻击哪个player
                
                //先确定哪个player不在战斗不能状态
                vector<int> playerIdList;
                for (int i = 0; i < this->m_playerList->count(); i++)
                {
                    RPGBattlePlayerSprite *player = (RPGBattlePlayerSprite*)this->m_playerList->objectAtIndex(i);
                    
                    if(player->m_data->m_status != kRPGDataStatusDeath)
                        playerIdList.push_back(player->m_data->m_dataId);
                }
                
                int randIndex = 0;
                if(playerIdList.size() > 1)
                    randIndex = OzgCCUtility::rangeRand(0, (int)playerIdList.size() - 1);
                
                int tagetPlayerId = playerIdList[randIndex];
                
                //计算攻击哪个player 完毕
                
                RPGBattlePlayerSprite *targetPlayer = (RPGBattlePlayerSprite*)this->getChildByTag(kRPGBattleSceneLayerTagPlayer + tagetPlayerId);
                monster->animAttack(this, targetPlayer->m_data);
                
                this->unscheduleUpdate();
                return;
            }
        }
    }
    
//    CCLog("%f", delta);
}

bool RPGBattleSceneLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    
    return true;
}

void RPGBattleSceneLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void RPGBattleSceneLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    CCPoint point = pTouch->getLocation();
    
    RPGBattleMenu *battleMenu = (RPGBattleMenu*)this->getChildByTag(kRPGBattleSceneLayerTagBattleMenu);
    RPGBattleMenu *winResultsDialog = (RPGBattleMenu*)this->getChildByTag(kRPGBattleSceneLayerTagWinResultsDialog);
    RPGBattleMenu *loseResultsDialog = (RPGBattleMenu*)this->getChildByTag(kRPGBattleSceneLayerTagLoseResultsDialog);
    
    if(battleMenu)
    {
        switch (battleMenu->m_selectedMenuTag)
        {
            case kRPGBattleMenuTagAttack:
            {
//                CCLog("攻击");
                
                //检测选中的player
                for (int i = 0; i < this->m_playerList->count(); i++)
                {
                    RPGBattlePlayerSprite *player = (RPGBattlePlayerSprite*)this->m_playerList->objectAtIndex(i);
                    if(player->boundingBox().containsPoint(point))
                    {
                        SimpleAudioEngine::sharedEngine()->playEffect("audio_effect_btn.wav");
                        
                        if(player->m_isSelected)
                        {
                            CCLog("攻击player");
                            
                            RPGBattleMenu *battleMenu = (RPGBattleMenu*)this->getChildByTag(kRPGBattleSceneLayerTagBattleMenu);
                            RPGPlayer *srcPlayerData = battleMenu->m_playerData;
                            
                            player->selected(false);
                            this->removeChildByTag(kRPGBattleSceneLayerTagBattleMenu, true);
                            
                            this->attack(srcPlayerData, player->m_data);
                        }
                        else
                            player->selected(true);
                    }
                    else
                        player->selected(false);
                    
                }
                
                //检测选中的怪物
                for (int i = 0; i < this->m_monsterList->count(); i++)
                {
                    RPGBattleMonsterSprite *monster = (RPGBattleMonsterSprite*)this->m_monsterList->objectAtIndex(i);
                    if(monster->boundingBox().containsPoint(point))
                    {
                        SimpleAudioEngine::sharedEngine()->playEffect("audio_effect_btn.wav");
                        
                        if(monster->m_isSelected)
                        {
                            CCLog("攻击怪物");
                            
                            RPGBattleMenu *battleMenu = (RPGBattleMenu*)this->getChildByTag(kRPGBattleSceneLayerTagBattleMenu);
                            RPGPlayer *srcPlayerData = battleMenu->m_playerData;
                            
                            monster->selected(false);
                            this->removeChildByTag(kRPGBattleSceneLayerTagBattleMenu, true);
                            
                            this->attack(srcPlayerData, monster->m_data);
                            
                        }
                        else
                            monster->selected(true);
                    }
                    else
                        monster->selected(false);
                    
                }
                
            }
                break;
            
            case kRPGBattleMenuTagSkill:
            {
                CCLog("技能");
                
                //检测选中的player
                for (int i = 0; i < this->m_playerList->count(); i++)
                {
                    RPGBattlePlayerSprite *player = (RPGBattlePlayerSprite*)this->m_playerList->objectAtIndex(i);
                    if(player->boundingBox().containsPoint(point))
                    {
                        SimpleAudioEngine::sharedEngine()->playEffect("audio_effect_btn.wav");
                        
                        if(player->m_isSelected)
                        {
                            CCLog("对player使用技能");
                            
                            RPGBattleMenu *battleMenu = (RPGBattleMenu*)this->getChildByTag(kRPGBattleSceneLayerTagBattleMenu);
                            RPGPlayer *playerData = battleMenu->m_playerData;

                            player->selected(false);
                            this->removeChildByTag(kRPGBattleSceneLayerTagBattleMenu, true);

                            this->skill(playerData, player->m_data);
                        }
                        else
                            player->selected(true);
                    }
                    else
                        player->selected(false);
                    
                }
                
                //检测选中的怪物
                for (int i = 0; i < this->m_monsterList->count(); i++)
                {
                    RPGBattleMonsterSprite *monster = (RPGBattleMonsterSprite*)this->m_monsterList->objectAtIndex(i);
                    if(monster->boundingBox().containsPoint(point))
                    {
                        SimpleAudioEngine::sharedEngine()->playEffect("audio_effect_btn.wav");
                        
                        if(monster->m_isSelected)
                        {
                            CCLog("对怪物使用技能");
                            
                            RPGBattleMenu *battleMenu = (RPGBattleMenu*)this->getChildByTag(kRPGBattleSceneLayerTagBattleMenu);
                            RPGPlayer *playerData = battleMenu->m_playerData;

                            monster->selected(false);
                            this->removeChildByTag(kRPGBattleSceneLayerTagBattleMenu, true);
                            
                            this->skill(playerData, monster->m_data);
                        }
                        else
                            monster->selected(true);
                    }
                    else
                        monster->selected(false);
                    
                }
                
            }
                break;
            
            case kRPGBattleMenuTagItems:
            {
                CCLog("道具");
                
                //检测选中的player
                for (int i = 0; i < this->m_playerList->count(); i++)
                {
                    RPGBattlePlayerSprite *player = (RPGBattlePlayerSprite*)this->m_playerList->objectAtIndex(i);
                    if(player->boundingBox().containsPoint(point))
                    {
                        SimpleAudioEngine::sharedEngine()->playEffect("audio_effect_btn.wav");
                        
                        if(player->m_isSelected)
                        {
                            CCLog("对player使用道具");
                            
                            RPGBattleMenu *battleMenu = (RPGBattleMenu*)this->getChildByTag(kRPGBattleSceneLayerTagBattleMenu);
                            RPGPlayer *srcPlayerData = battleMenu->m_playerData;
                            
                            player->selected(false);
                            this->removeChildByTag(kRPGBattleSceneLayerTagBattleMenu, true);
                            
                            this->useItem(srcPlayerData, player->m_data);
                        }
                        else
                            player->selected(true);
                    }
                    else
                        player->selected(false);
                    
                }
                
                //检测选中的怪物
                for (int i = 0; i < this->m_monsterList->count(); i++)
                {
                    RPGBattleMonsterSprite *monster = (RPGBattleMonsterSprite*)this->m_monsterList->objectAtIndex(i);
                    if(monster->boundingBox().containsPoint(point))
                    {
                        SimpleAudioEngine::sharedEngine()->playEffect("audio_effect_btn.wav");
                        
                        if(monster->m_isSelected)
                        {
                            CCLog("对怪物使用道具");
                            
                            RPGBattleMenu *battleMenu = (RPGBattleMenu*)this->getChildByTag(kRPGBattleSceneLayerTagBattleMenu);
                            RPGPlayer *srcPlayerData = battleMenu->m_playerData;
                            
                            monster->selected(false);
                            this->removeChildByTag(kRPGBattleSceneLayerTagBattleMenu, true);
                            
                            this->useItem(srcPlayerData, monster->m_data);
                        }
                        else
                            monster->selected(true);
                    }
                    else
                        monster->selected(false);
                    
                }
            }
                break;
                
//            default:
//                break;
        }
    }
    else if(winResultsDialog)
    {
        CCLog("战斗胜利后返回地图界面");
        
        //保存获得的金钱到数据库
        {
            this->m_totalGold = CCUserDefault::sharedUserDefault()->getIntegerForKey("gold") + this->m_totalGold;
            
            //最大值的处理
            if(this->m_totalGold > MAX_GOLD)
                this->m_totalGold = MAX_GOLD;
            
            RPGSaveData *saveDataObj = loadSaveData(&this->m_db);
            saveDataObj->m_gold = this->m_totalGold;
            
            saveData(&this->m_db, saveDataObj);
        }
        
        this->goToMap();
    }
    else if(loseResultsDialog)
    {
//        CCLog("战斗输了后返回初始界面");
        
        //输了不保存任何内容
        
        this->enabledTouched(false);
        this->removeAllChildrenWithCleanup(true);
        
        CCTextureCache::sharedTextureCache()->removeTextureForKey("monsters.png");
        CCTextureCache::sharedTextureCache()->removeTextureForKey("battle_bg.png");
        CCTextureCache::sharedTextureCache()->removeTextureForKey("attack10.png");
        CCTextureCache::sharedTextureCache()->removeTextureForKey("attack20.png");
        CCTextureCache::sharedTextureCache()->removeTextureForKey("attack30.png");
        CCTextureCache::sharedTextureCache()->removeTextureForKey("attack40.png");
        CCTextureCache::sharedTextureCache()->removeTextureForKey("attack_damage.png");
        
        for (int i = 0; i < this->m_playerDataList->count(); i++)
        {
            RPGPlayer *playerData = (RPGPlayer*)this->m_playerDataList->objectAtIndex(i);
            
            CCTextureCache::sharedTextureCache()->removeTextureForKey(CCString::createWithFormat("%s_1.png", playerData->m_texPrefix.c_str())->getCString());
            CCTextureCache::sharedTextureCache()->removeTextureForKey(CCString::createWithFormat("%s_2.png", playerData->m_texPrefix.c_str())->getCString());
            CCTextureCache::sharedTextureCache()->removeTextureForKey(CCString::createWithFormat("%s_3.png", playerData->m_texPrefix.c_str())->getCString());
        }
        
//        CCTextureCache::sharedTextureCache()->dumpCachedTextureInfo();
        
        if(SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
            SimpleAudioEngine::sharedEngine()->stopBackgroundMusic(true);
        SimpleAudioEngine::sharedEngine()->playBackgroundMusic("audio_start.mp3", true);
        CCScene *s = RPGStartSceneLayer::scene();
        CCTransitionFade *t = CCTransitionFade::create(GAME_SCENE, s);
        CCDirector::sharedDirector()->replaceScene(t);
    }
    
}

void RPGBattleSceneLayer::showMsg(cocos2d::CCString *msgText)
{
    this->showMsg(msgText, true);
}

void RPGBattleSceneLayer::showMsg(cocos2d::CCString *msgText, bool autoRelease)
{
    CCTMXTiledMap *bgLayer = (CCTMXTiledMap*)this->getChildByTag(kRPGBattleSceneLayerTagMsg);
    
    //之前已经存在了msg框就先释放
    if(bgLayer)
        bgLayer->removeFromParentAndCleanup(true);
    
    //如果正在执行自动释放msg就先停止
    if(this->m_autoReleaseMsg)
    {
        this->m_autoReleaseMsg = false;
        this->unschedule(schedule_selector(RPGBattleSceneLayer::hideMsg));
    }
    
    bgLayer = CCTMXTiledMap::create(CCString::createWithFormat("battle_msg_%s.tmx", CCUserDefault::sharedUserDefault()->getStringForKey(GAME_STYLE).c_str())->getCString());
    bgLayer->setPosition(ccp((CCDirector::sharedDirector()->getWinSize().width - bgLayer->getContentSize().width) / 2, 593));
    bgLayer->setTag(kRPGBattleSceneLayerTagMsg);
    this->addChild(bgLayer);
    
    CCLabelTTF *textLab = CCLabelTTF::create(msgText->getCString(), "Arial", 17, CCSizeMake(400, 25), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    textLab->setPosition(ccp(bgLayer->getContentSize().width / 2, bgLayer->getContentSize().height / 2));
    bgLayer->addChild(textLab);
    
    if(autoRelease)
    {
        this->m_autoReleaseMsg = true;
        this->scheduleOnce(schedule_selector(RPGBattleSceneLayer::hideMsg), 3.0);
    }
    else
        this->m_autoReleaseMsg = false;
}

void RPGBattleSceneLayer::hideMsg()
{
    this->hideMsg(0);
}

void RPGBattleSceneLayer::hideMsg(float dt)
{
    CCTMXTiledMap *bgLayer = (CCTMXTiledMap*)this->getChildByTag(kRPGBattleSceneLayerTagMsg);
    
    if(bgLayer)
        bgLayer->removeFromParentAndCleanup(true);
    
    this->m_autoReleaseMsg = false;
}

void RPGBattleSceneLayer::goToMap()
{
    //保存所在地图和位置到数据库
    {
        RPGSaveData *saveDataObj = loadSaveData(&this->m_db);
        saveDataObj->m_mapId = CCUserDefault::sharedUserDefault()->getIntegerForKey("map_id");
        saveDataObj->m_playerToX = CCUserDefault::sharedUserDefault()->getFloatForKey("player_to_x");
        saveDataObj->m_playerToY = CCUserDefault::sharedUserDefault()->getFloatForKey("player_to_y");
        saveDataObj->m_playerDirection = CCUserDefault::sharedUserDefault()->getStringForKey("player_direction");
        
        saveData(&this->m_db, saveDataObj);
    }
    
    //保存player的状态到数据库
    for (int i = 0; i < this->m_playerDataList->count(); i++)
    {
        RPGPlayer *playerData = (RPGPlayer*)this->m_playerDataList->objectAtIndex(i);
        
        CCString *sql = CCString::createWithFormat(UPDATE_PLAYER, playerData->m_HP, playerData->m_MP, playerData->m_attack, playerData->m_defense, playerData->m_speed, playerData->m_skillAttack, playerData->m_skillDefense, playerData->m_level, playerData->m_skill.c_str(), playerData->m_nextExp, playerData->m_exp, playerData->m_dataId);
//        CCLog("%s", sql->getCString());
        this->m_db.execDML(sql->getCString());
    }
    
    //保存现有道具到数据库
    for (int i = 0; i < this->m_existingItems->count(); i++)
    {
        RPGExistingItems *itemsData = (RPGExistingItems*)this->m_existingItems->objectAtIndex(i);
        this->m_db.execDML(CCString::createWithFormat(UPDATE_EXISTING_ITEMS, itemsData->m_total, itemsData->m_dataId)->getCString());
    }
    
    this->enabledTouched(false);
    
    CCArray *loadTextures = CCArray::create();
    loadTextures->addObject(CCString::create("map.png"));
    loadTextures->addObject(CCString::create("joystick.png"));
    loadTextures->addObject(CCString::create("actor4_0.png"));
    loadTextures->addObject(CCString::create("actor111.png"));
    loadTextures->addObject(CCString::create("actor113.png"));
    loadTextures->addObject(CCString::create("actor114.png"));
    loadTextures->addObject(CCString::create("actor115.png"));
    loadTextures->addObject(CCString::create("actor117.png"));
    loadTextures->addObject(CCString::create("actor120.png"));
    
    CCArray *releaseTextures = CCArray::create();
    releaseTextures->addObject(CCString::create("monsters.png"));
    releaseTextures->addObject(CCString::create("battle_bg.png"));
    releaseTextures->addObject(CCString::create("attack10.png"));
    releaseTextures->addObject(CCString::create("attack20.png"));
    releaseTextures->addObject(CCString::create("attack30.png"));
    releaseTextures->addObject(CCString::create("attack40.png"));
    releaseTextures->addObject(CCString::create("attack_damage.png"));
    
    for (int i = 0; i < this->m_playerDataList->count(); i++)
    {
        RPGPlayer *playerData = (RPGPlayer*)this->m_playerDataList->objectAtIndex(i);
        releaseTextures->addObject(CCString::createWithFormat("%s_1.png", playerData->m_texPrefix.c_str()));
        releaseTextures->addObject(CCString::createWithFormat("%s_2.png", playerData->m_texPrefix.c_str()));
        releaseTextures->addObject(CCString::createWithFormat("%s_3.png", playerData->m_texPrefix.c_str()));
    }
    
    CCScene *s = RPGLoadingSceneLayer::scene(loadTextures, releaseTextures, "single_map");
    CCTransitionFade *t = CCTransitionFade::create(GAME_SCENE, s);
    CCDirector::sharedDirector()->replaceScene(t);
}

void RPGBattleSceneLayer::enabledTouched(bool enabled)
{
    if(this->m_enabledTouched)
        CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    
    this->m_enabledTouched = enabled;
    
    if(this->m_enabledTouched)
        CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, true);
    
}

void RPGBattleSceneLayer::cancelAllSelected()
{
    for (int i = 0; i < this->m_playerList->count(); i++)
    {
        RPGBattlePlayerSprite *player = (RPGBattlePlayerSprite*)this->m_playerList->objectAtIndex(i);
        player->selected(false);
        
    }
    
    for (int i = 0; i < this->m_monsterList->count(); i++)
    {
        RPGBattleMonsterSprite *monster = (RPGBattleMonsterSprite*)this->m_monsterList->objectAtIndex(i);
        monster->selected(false);
        
    }
    
}

void RPGBattleSceneLayer::attack(RPGBaseRole *attackObjData, RPGBaseRole *targetObjData)
{
    //发起攻击的对象
    if(dynamic_cast<RPGPlayer*>(attackObjData) != NULL)
    {
        RPGBattlePlayerSprite *player = (RPGBattlePlayerSprite*)this->getChildByTag(kRPGBattleSceneLayerTagPlayer + ((RPGPlayer*)attackObjData)->m_dataId);
        player->animAttack(this, targetObjData);
    }
//    else if(dynamic_cast<RPGMonster*>(attackObjData) != NULL)
//    {
//        
//    }
    
}

void RPGBattleSceneLayer::attackResults(cocos2d::CCNode *sender, void *data)
{
    //sender为发起攻击的Sprite对象，data为被攻击的对象
    
    SimpleAudioEngine::sharedEngine()->playEffect("audio_battle_attack2.wav");
    
//    CCLog("计算伤害值");
    
    CCPoint particleSysPoint;
    
    //判断发起攻击的对象是player还是怪物
    if(dynamic_cast<RPGBattlePlayerSprite*>(sender) != NULL)
    {
        if(dynamic_cast<RPGMonster*>((RPGBaseRole*)data) != NULL)
        {
            //player攻击怪物
            
            RPGBattleMonsterSprite *targetMonster = (RPGBattleMonsterSprite*)this->getChildByTag(((RPGMonster*)data)->m_tag);
            particleSysPoint = targetMonster->getPosition();
            
            int results = RPGComputingResults::attackResults(((RPGBattlePlayerSprite*)sender)->m_data->m_attack, ((RPGMonster*)data)->m_defense);
            targetMonster->showEffectResults(this, -results, sender);
        }
        else if(dynamic_cast<RPGPlayer*>((RPGBaseRole*)data) != NULL)
        {
            //player攻击player
            RPGBattlePlayerSprite *targetPlayer = (RPGBattlePlayerSprite*)this->getChildByTag(kRPGBattleSceneLayerTagPlayer + ((RPGPlayer*)data)->m_dataId);
            targetPlayer->animHurt();
            
            particleSysPoint = targetPlayer->getPosition();
            
            int results = RPGComputingResults::attackResults(((RPGBattlePlayerSprite*)sender)->m_data->m_attack, ((RPGPlayer*)data)->m_defense);
            targetPlayer->showEffectResults(this, -results, sender);
            
        }
        
    }
    else if(dynamic_cast<RPGBattleMonsterSprite*>(sender) != NULL)
    {
        //怪物攻击player
        
        RPGBattlePlayerSprite *targetPlayer = (RPGBattlePlayerSprite*)this->getChildByTag(kRPGBattleSceneLayerTagPlayer + ((RPGPlayer*)data)->m_dataId);
        targetPlayer->animHurt();
        
        particleSysPoint = targetPlayer->getPosition();
        
        int HPResults = RPGComputingResults::attackResults(((RPGBattleMonsterSprite*)sender)->m_data->m_attack, ((RPGPlayer*)data)->m_defense);
        targetPlayer->showEffectResults(this, -HPResults, sender);
        
    }
    
    //显示攻击后的粒子效果
    CCParticleSystemQuad *particleSys = CCParticleSystemQuad::create("attack_damage.plist");
    particleSys->setAutoRemoveOnFinish(true);
    particleSys->setPosition(particleSysPoint);
    this->addChild(particleSys);
    
}

void RPGBattleSceneLayer::skill(RPGBaseRole *attackObjData, RPGBaseRole *targetObjData)
{
    //发起攻击的对象
    if(dynamic_cast<RPGPlayer*>(attackObjData) != NULL)
    {
        RPGBattlePlayerSprite *player = (RPGBattlePlayerSprite*)this->getChildByTag(kRPGBattleSceneLayerTagPlayer + ((RPGPlayer*)attackObjData)->m_dataId);
        player->animSkill(this, targetObjData);
    }
}

void RPGBattleSceneLayer::skillResults(cocos2d::CCNode *sender, void *data)
{
    //sender为发起技能的Sprite对象，data为被使用技能的对象
    
    CCPoint particleSysPoint;
    
    if(dynamic_cast<RPGBattlePlayerSprite*>(sender) != NULL)
    {
        if(dynamic_cast<RPGMonster*>((RPGBaseRole*)data) != NULL)
        {
            //对怪物使用技能
            
            RPGBattleMonsterSprite *targetMonster = (RPGBattleMonsterSprite*)this->getChildByTag(((RPGMonster*)data)->m_tag);
            particleSysPoint = targetMonster->getPosition();
            
            int results = RPGResultsLogic::battleSkill(&this->m_db, ((RPGBattlePlayerSprite*)sender)->m_data, CCUserDefault::sharedUserDefault()->getIntegerForKey(GAME_BATTLE_SELECTED_ID), (RPGMonster*)data);            
            targetMonster->showEffectResults(this, results, sender);
        }
        else if(dynamic_cast<RPGPlayer*>((RPGBaseRole*)data) != NULL)
        {
            //对player使用技能
            RPGBattlePlayerSprite *targetPlayer = (RPGBattlePlayerSprite*)this->getChildByTag(kRPGBattleSceneLayerTagPlayer + ((RPGPlayer*)data)->m_dataId);
            particleSysPoint = targetPlayer->getPosition();
            
            int results = RPGResultsLogic::battleSkill(&this->m_db, ((RPGBattlePlayerSprite*)sender)->m_data, CCUserDefault::sharedUserDefault()->getIntegerForKey(GAME_BATTLE_SELECTED_ID), (RPGPlayer*)data);
            targetPlayer->showEffectResults(this, results, sender);
        }
        
        //更新下面显示的MP值
        CCLabelTTF *mpLab = (CCLabelTTF*)this->getChildByTag(kRPGBattleSceneLayerTagPlayerMP + ((RPGBattlePlayerSprite*)sender)->m_data->m_dataId);
        mpLab->setString(CCString::createWithFormat("%i", ((RPGBattlePlayerSprite*)sender)->m_data->m_MP)->getCString());
    }
    
    //对应技能的声音和粒子效果
    CCParticleSystemQuad *particleSys = NULL;
    switch (CCUserDefault::sharedUserDefault()->getIntegerForKey(GAME_BATTLE_SELECTED_ID))
    {
        case 2:
            //回复
            SimpleAudioEngine::sharedEngine()->playEffect("audio_skill_cure.wav", false);
            particleSys = CCParticleSystemQuad::create("skill_cure.plist");
            break;
            
        default:
            //火焰
            SimpleAudioEngine::sharedEngine()->playEffect("audio_skill_fire.wav", false);
            particleSys = CCParticleSystemQuad::create("skill_fire.plist");
            break;
    }
    //显示使用技能后的粒子效果
    particleSys->setAutoRemoveOnFinish(true);
    particleSys->setPosition(particleSysPoint);
    particleSys->setScale(0.5);
    this->addChild(particleSys);
}

void RPGBattleSceneLayer::useItem(RPGBaseRole *useItemObjData, RPGBaseRole *targetObjData)
{
    RPGBattlePlayerSprite *srcPlayer = (RPGBattlePlayerSprite*)this->getChildByTag(kRPGBattleSceneLayerTagPlayer + ((RPGPlayer*)useItemObjData)->m_dataId);
    srcPlayer->animUseItem(this, targetObjData);
    
}

void RPGBattleSceneLayer::useItemResults(cocos2d::CCNode *sender, void *data)
{
    CCPoint particleSysPoint;
    
    if(dynamic_cast<RPGBattlePlayerSprite*>(sender) != NULL)
    {
        if(dynamic_cast<RPGMonster*>((RPGBaseRole*)data) != NULL)
        {
            //对怪物使用道具
            
            RPGBattleMonsterSprite *targetMonster = (RPGBattleMonsterSprite*)this->getChildByTag(((RPGMonster*)data)->m_tag);
            particleSysPoint = targetMonster->getPosition();
            
            int results = RPGResultsLogic::battleUseItems(this->m_existingItems, (RPGMonster*)data, CCUserDefault::sharedUserDefault()->getIntegerForKey(GAME_BATTLE_SELECTED_ID));
            targetMonster->showEffectResults(this, results, sender);
        }
        else if(dynamic_cast<RPGPlayer*>((RPGBaseRole*)data) != NULL)
        {
            //对player使用道具
            RPGBattlePlayerSprite *targetPlayer = (RPGBattlePlayerSprite*)this->getChildByTag(kRPGBattleSceneLayerTagPlayer + ((RPGPlayer*)data)->m_dataId);
            particleSysPoint = targetPlayer->getPosition();
            
            int results = RPGResultsLogic::battleUseItems(this->m_existingItems, (RPGPlayer*)data, CCUserDefault::sharedUserDefault()->getIntegerForKey(GAME_BATTLE_SELECTED_ID));
            targetPlayer->showEffectResults(this, results, sender);
        }
        
    }
    
    SimpleAudioEngine::sharedEngine()->playEffect("audio_use_items.wav", false);
    
    //显示使用道具后的粒子效果
    CCParticleSystemQuad *particleSys = CCParticleSystemQuad::create("use_item.plist");
    particleSys->setAutoRemoveOnFinish(true);
    particleSys->setPosition(particleSysPoint);
    this->addChild(particleSys);
}

void RPGBattleSceneLayer::actionWithTargetEffectLabEnd(cocos2d::CCNode *sender, void* data)
{
    //data为发起攻击的Sprite对象
    
    CCLog("攻击流程完毕");
    
    if(dynamic_cast<RPGBattlePlayerSprite*>(sender->getParent()) != NULL)
    {
        RPGBattlePlayerSprite *targetPlayer = (RPGBattlePlayerSprite*)sender->getParent();
        
        if(targetPlayer->m_data->m_HP <= 0)
        {
            //player战斗不能
            targetPlayer->m_data->m_status = kRPGDataStatusDeath;
            targetPlayer->animDeath();
        }
        else
        {
            //player复活
            targetPlayer->m_data->m_status = kRPGDataStatusNormal;
            targetPlayer->animNormal();
        }
        
        sender->removeFromParentAndCleanup(true);
        
        //更新下面显示的HP
        CCLabelTTF *targetPlayerHP = (CCLabelTTF*)this->getChildByTag(kRPGBattleSceneLayerTagPlayerHP + targetPlayer->m_data->m_dataId);
        targetPlayerHP->setString(CCString::createWithFormat("%i", targetPlayer->m_data->m_HP)->getCString());
        
        if(dynamic_cast<RPGBattlePlayerSprite*>((CCObject*)data) != NULL)
        {
            //发起攻击的player还原通常时状态，并重新计算进度条
            RPGBattlePlayerSprite *srcPlayer = (RPGBattlePlayerSprite*)data;
            
            if(srcPlayer->m_data->m_status != kRPGDataStatusDeath)
                srcPlayer->animNormal();
            
            srcPlayer->m_data->m_progress = 0;
            CCProgressTimer *battleProgress = (CCProgressTimer*)this->getChildByTag(kRPGBattleSceneLayerTagPlayerProgress + srcPlayer->m_data->m_dataId);
            battleProgress->setPercentage(srcPlayer->m_data->m_progress);
        }
        else if(dynamic_cast<RPGBattleMonsterSprite*>((CCObject*)data) != NULL)
        {
            //发起攻击的怪物重新计算进度条
            RPGBattleMonsterSprite *srcMonster = (RPGBattleMonsterSprite*)data;
            srcMonster->m_data->m_progress = 0;
        }
        
        //判断是否胜利了
        if(!this->judgeLose())
            this->scheduleUpdate();
        else
        {
//            CCLog("输了");
            SimpleAudioEngine::sharedEngine()->stopBackgroundMusic(true);
            SimpleAudioEngine::sharedEngine()->playBackgroundMusic("audio_battle_lose.mp3", false);
            
            this->showLoseResults();
        }
    }
    else if(dynamic_cast<RPGBattleMonsterSprite*>(sender->getParent()) != NULL)
    {
        
        if(((RPGBattleMonsterSprite*)sender->getParent())->m_data->m_HP <= 0)
        {
            //怪物战斗不能
            ((RPGBattleMonsterSprite*)sender->getParent())->m_data->m_status = kRPGDataStatusDeath;
            
            //打倒了一个怪物后，重新统计左下角的怪物
            for (int i = 0; i < this->m_monsterDataList->count(); i++)
            {
                CCArray *monsters = (CCArray*)this->m_monsterDataList->objectAtIndex(i);
                for (int j = 0; j < monsters->count(); j++)
                {
                    RPGMonster *monsterData = (RPGMonster*)monsters->objectAtIndex(j);
                    
                    if(monsterData == ((RPGBattleMonsterSprite*)sender->getParent())->m_data)
                    {
                        //统计活着的怪物个数
                        int total = monsters->count();
                        for (int k = 0; k < monsters->count(); k++)
                        {
                            RPGMonster *tmpMonsterData = (RPGMonster*)monsters->objectAtIndex(k);
                            if(tmpMonsterData->m_status == kRPGDataStatusDeath)
                                total--;
                        }
                        
                        if(total <= 0)
                            //该怪物完全消灭
                            this->removeChildByTag(kRPGBattleSceneLayerTagMonsterNameLab + monsterData->m_dataId, true);
                        else
                        {
                            //还有剩下的怪物
                            
                            CCLabelTTF *monsterNameLab = (CCLabelTTF*)this->getChildByTag(kRPGBattleSceneLayerTagMonsterNameLab + monsterData->m_dataId);
                            monsterNameLab->setString(CCString::createWithFormat("%s x %i", monsterData->m_name.c_str(), total)->getCString());
                            
                        }
                        
                    }
                }
                
            }
            
            this->m_monsterList->removeObject(sender->getParent());
            ((RPGBattleMonsterSprite*)sender->getParent())->removeFromParentAndCleanup(true);
            
            //this->m_monsterDataList 不清理是因为战斗结束后需要这个对象来计算经验值
            
        }
        else
            sender->removeFromParentAndCleanup(true);
        
        //发起攻击的player还原通常时状态，并重新计算进度条
        RPGBattlePlayerSprite *player = (RPGBattlePlayerSprite*)data;
        player->animNormal();
        
        player->m_data->m_progress = 0;
        CCProgressTimer *battleProgress = (CCProgressTimer*)this->getChildByTag(kRPGBattleSceneLayerTagPlayerProgress + player->m_data->m_dataId);
        battleProgress->setPercentage(player->m_data->m_progress);
        
        //判断是否胜利了
        if(!this->judgeWin())
            this->scheduleUpdate();
        else
        {
//            CCLog("胜利");
            SimpleAudioEngine::sharedEngine()->stopBackgroundMusic(true);
            SimpleAudioEngine::sharedEngine()->playBackgroundMusic("audio_battle_end.mp3", true);
            
            for (int i = 0; i < this->m_playerList->count(); i++)
            {
                RPGBattlePlayerSprite *player = (RPGBattlePlayerSprite*)this->m_playerList->objectAtIndex(i);
                if(player->m_data->m_status != kRPGDataStatusDeath)
                    player->animWin();
            }
            
            this->showWinResults();
        }
    }
    
}

//private

bool RPGBattleSceneLayer::monsterDataListExistWithTag(int tag)
{
    for (int i = 0; i < this->m_monsterDataList->count(); i++)
    {
        CCArray *monsters = (CCArray*)this->m_monsterDataList->objectAtIndex(i);
        
        for (int j = 0; j < monsters->count(); j++)
        {
            RPGMonster *monster = (RPGMonster*)monsters->objectAtIndex(j);
            if(monster->m_tag == tag)
                return true;
        }
    }
    
    return false;
}

CCArray* RPGBattleSceneLayer::monsterDataListWithDataId(int dataId)
{
    for (int i = 0; i < this->m_monsterDataList->count(); i++)
    {
        RPGMonster *monster = (RPGMonster*)((CCArray*)this->m_monsterDataList->objectAtIndex(i))->objectAtIndex(0);
        if(monster->m_dataId == dataId)
            return (CCArray*)this->m_monsterDataList->objectAtIndex(i);
    }
    
    return CCArray::create();
}

void RPGBattleSceneLayer::setMonsterPosition(RPGBattleMonsterSprite* monster)
{
//    CCLayerColor *l = CCLayerColor::create(ccc4(200, 200, 200, 200), 700, 420);
//    l->setPosition(ccp(18, 206));
//    this->addChild(l);
    
    bool overlap; //是否重叠
    do
    {
        overlap = false;
        
        float x = OzgCCUtility::randomFloat(18 + (monster->getContentSize().width * monster->getAnchorPoint().x), 718 - ((monster->getContentSize().width * monster->getAnchorPoint().x)));
        float y = OzgCCUtility::randomFloat(206 + (monster->getContentSize().height * monster->getAnchorPoint().y), 626 - (monster->getContentSize().height * monster->getAnchorPoint().y));
        
        monster->setPosition(ccp(x, y));
        
        for (int i = 0; i < (int)this->m_addedMonsters.size(); i++)
        {
            RPGBattleMonsterSprite *addedMonster = (RPGBattleMonsterSprite*)this->getChildByTag(this->m_addedMonsters[i]);
            
            if(addedMonster->boundingBox().intersectsRect(monster->boundingBox()))
            {
                overlap = true;
//                CCLog("重新计算怪物的显示位置");
                break;
            }
        }
        
    }
    while (overlap);
    
}

void RPGBattleSceneLayer::computingProgress()
{
//    CCLog("%f", this->m_speedTotal);
    
    //player
    for (int i = 0; i < this->m_playerDataList->count(); i++)
    {
        RPGPlayer *playerData = (RPGPlayer*)this->m_playerDataList->objectAtIndex(i);
        
        CCProgressTimer *battleProgress = (CCProgressTimer*)this->getChildByTag(kRPGBattleSceneLayerTagPlayerProgress + playerData->m_dataId);
        
        float val = playerData->m_speed / this->m_speedTotal * 100.0;
        playerData->m_progress = val;
        battleProgress->setPercentage(playerData->m_progress);
    }
    
    //monster
    for (int i = 0; i < this->m_monsterDataList->count(); i++)
    {
        CCArray *monsters = (CCArray*)this->m_monsterDataList->objectAtIndex(i);
        for (int j = 0; j < monsters->count(); j++)
        {
            RPGMonster *monsterData = (RPGMonster*)monsters->objectAtIndex(j);
            
            float val = monsterData->m_speed / this->m_speedTotal * 100.0;
            monsterData->m_progress = val;
        }
    }
    
}

float RPGBattleSceneLayer::computingFloat(float val)
{
    float min = val - (val * 0.1);
    float max = val + (val * 0.1);
    return OzgCCUtility::randomFloat(min, max);
}

bool RPGBattleSceneLayer::judgeWin()
{
    int total = 0; //统计活着的怪物
    
    for (int i = 0; i < this->m_monsterDataList->count(); i++)
    {
        CCArray *monsters = (CCArray*)this->m_monsterDataList->objectAtIndex(i);
        for (int j = 0; j < monsters->count(); j++)
        {
            RPGMonster *monsterData = (RPGMonster*)monsters->objectAtIndex(j);
            if(monsterData->m_status != kRPGDataStatusDeath)
                total++;
            
        }
        
    }
    
    if(total <= 0)
        return true;
    return false;
}

bool RPGBattleSceneLayer::judgeLose()
{
    int total = 0; //统计活着的player
    for (int i = 0; i < this->m_playerList->count(); i++)
    {
        RPGBattlePlayerSprite *player = (RPGBattlePlayerSprite*)this->m_playerList->objectAtIndex(i);
        if(player->m_data->m_status != kRPGDataStatusDeath)
            total++;
    }
    
    if(total <= 0)
        return true;
    return false;
}

void RPGBattleSceneLayer::showWinResults()
{
    CCTMXTiledMap *winResults = CCTMXTiledMap::create(CCString::createWithFormat("battle_select_%s.tmx", CCUserDefault::sharedUserDefault()->getStringForKey(GAME_STYLE).c_str())->getCString());
    winResults->setPosition(ccp((CCDirector::sharedDirector()->getWinSize().width - winResults->getContentSize().width) / 2, (CCDirector::sharedDirector()->getWinSize().height - winResults->getContentSize().height) / 2));
    winResults->setTag(kRPGBattleSceneLayerTagWinResultsDialog);
    this->addChild(winResults);
    
    //显示title
    addLab(winResults, 5, CCString::create(((CCString*)this->m_stringList->objectForKey("win_title"))->getCString()), 26, kCCTextAlignmentCenter, ccp(winResults->getContentSize().width / 2, 270));
    
    //统计经验值，怪物的总经验值 / 活着的player总数
    
    //player
    int totalPlayer = 0;
    for (int i = 0; i < this->m_playerList->count(); i++)
    {
        RPGBattlePlayerSprite *player = (RPGBattlePlayerSprite*)this->m_playerList->objectAtIndex(i);
        if(player->m_data->m_status != kRPGDataStatusDeath)
            totalPlayer++;
    }
    
    //经验值和金钱
    int totalExp = 0;
    int totalGold = 0;
    for (int i = 0; i < this->m_monsterDataList->count(); i++)
    {
        CCArray *monsters = (CCArray*)this->m_monsterDataList->objectAtIndex(i);
        for (int j = 0; j < monsters->count(); j++)
        {
            RPGMonster *monsterData = (RPGMonster*)monsters->objectAtIndex(j);
            totalExp += monsterData->m_exp;
            
            totalGold += monsterData->m_gold;
        }
    }
    this->m_totalGold = totalGold;
    
    //显示获得的金钱
    addLab(winResults, 6, CCString::createWithFormat(((CCString*)this->m_stringList->objectForKey("win_gold"))->getCString(), totalGold), 20, kCCTextAlignmentCenter, ccp(winResults->getContentSize().width / 2, 220));
    
    //显示各个player获得的经验值
    float y = 170;
    int exp = totalExp / totalPlayer;
    for (int i = 0; i < this->m_playerList->count(); i++)
    {
        RPGBattlePlayerSprite *player = (RPGBattlePlayerSprite*)this->m_playerList->objectAtIndex(i);
        
        if(player->m_data->m_status != kRPGDataStatusDeath)
        {
            player->m_data->m_exp += exp; //累加总经验
            player->m_data->m_nextExp -= exp; //累减下一级经验值
            if(player->m_data->m_nextExp <= 0)
            {
                //升级了
                player->m_data->m_level++;
                
                player->m_data->m_nextExp = player->m_data->m_level * player->m_data->m_exp; //计算下一级经验值
                
                //提升能力值
                
                switch (player->m_data->m_dataId)
                {
                    case 2:
                        player->m_data->m_maxHP += 12;
                        player->m_data->m_maxMP += 43;
                        player->m_data->m_attack += 1;
                        player->m_data->m_defense += 2;
                        player->m_data->m_speed += 2;
                        player->m_data->m_skillAttack += 5;
                        player->m_data->m_skillDefense += 4;
                        break;
                    case 3:
                        player->m_data->m_maxHP += 8;
                        player->m_data->m_maxMP += 48;
                        player->m_data->m_attack += 1;
                        player->m_data->m_defense += 3;
                        player->m_data->m_speed += 2;
                        player->m_data->m_skillAttack += 4;
                        player->m_data->m_skillDefense += 5;
                        break;
                    case 4:
                        player->m_data->m_maxHP += 23;
                        player->m_data->m_maxMP += 21;
                        player->m_data->m_attack += 3;
                        player->m_data->m_defense += 3;
                        player->m_data->m_speed += 6;
                        player->m_data->m_skillAttack += 2;
                        player->m_data->m_skillDefense += 2;
                        break;
                    default:
                        player->m_data->m_maxHP += 49;
                        player->m_data->m_maxMP += 8;
                        player->m_data->m_attack += 6;
                        player->m_data->m_defense += 4;
                        player->m_data->m_speed += 3;
                        player->m_data->m_skillAttack += 1;
                        player->m_data->m_skillDefense += 1;
                        break;
                }
                
                //最大值的处理
                if(player->m_data->m_maxHP > MAX_STATUS1) player->m_data->m_maxHP = MAX_STATUS1;
                if(player->m_data->m_maxMP > MAX_STATUS2) player->m_data->m_maxMP = MAX_STATUS2;
                if(player->m_data->m_attack > MAX_STATUS2) player->m_data->m_attack = MAX_STATUS2;
                if(player->m_data->m_defense > MAX_STATUS2) player->m_data->m_defense = MAX_STATUS2;
                if(player->m_data->m_speed > MAX_STATUS2) player->m_data->m_speed = MAX_STATUS2;
                if(player->m_data->m_skillAttack > MAX_STATUS2) player->m_data->m_skillAttack = MAX_STATUS2;
                if(player->m_data->m_skillDefense > MAX_STATUS2) player->m_data->m_skillDefense = MAX_STATUS2;
                
                addLab(winResults, player->m_data->m_dataId, CCString::createWithFormat(((CCString*)this->m_stringList->objectForKey("win_level_up"))->getCString(), player->m_data->m_name.c_str()), 18, kCCTextAlignmentLeft, ccp(230, y));
            }
            else
                addLab(winResults, player->m_data->m_dataId, CCString::createWithFormat(((CCString*)this->m_stringList->objectForKey("win_exp"))->getCString(), player->m_data->m_name.c_str(), exp), 18, kCCTextAlignmentLeft, ccp(230, y));
        }
        else
            addLab(winResults, player->m_data->m_dataId, CCString::createWithFormat(((CCString*)this->m_stringList->objectForKey("win_exp"))->getCString(), player->m_data->m_name.c_str(), 0), 18, kCCTextAlignmentLeft, ccp(230, y));
        
        y -= 40;
    }
    
}

void RPGBattleSceneLayer::showLoseResults()
{
    CCTMXTiledMap *loseResults = CCTMXTiledMap::create(CCString::createWithFormat("battle_select_%s.tmx", CCUserDefault::sharedUserDefault()->getStringForKey(GAME_STYLE).c_str())->getCString());
    loseResults->setPosition(ccp((CCDirector::sharedDirector()->getWinSize().width - loseResults->getContentSize().width) / 2, (CCDirector::sharedDirector()->getWinSize().height - loseResults->getContentSize().height) / 2));
    loseResults->setTag(kRPGBattleSceneLayerTagLoseResultsDialog);
    this->addChild(loseResults);
    
    this->enabledTouched(true);
    
    //显示title
    addLab(loseResults, 5, CCString::create(((CCString*)this->m_stringList->objectForKey("lose_title"))->getCString()), 28, kCCTextAlignmentCenter, ccp(loseResults->getContentSize().width / 2, loseResults->getContentSize().height / 2));
    
}
