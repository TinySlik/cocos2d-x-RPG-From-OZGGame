//
//  RPGMapSceneLayer.cpp
//  OzgGameRPG
//
//  Created by ozg on 14-3-17.
//
//

#include "RPGMapSceneLayer.h"
#include "OzgCCUtility.h"
#include "OzgJoystickLayer.h"
#include "RPGMapRoleSprite.h"
#include "RPGMapMenuLayer.h"
#include "RPGMapDialogLayer.h"
#include "RPGLoadingSceneLayer.h"

RPGMapSceneLayer::RPGMapSceneLayer()
{
    
//    CCLog("RPGMapSceneLayer实例化");
}

RPGMapSceneLayer::~RPGMapSceneLayer()
{
    OzgJoystickLayer *joystick = (OzgJoystickLayer*)this->getChildByTag(kRPGMapSceneLayerTagJoystick);
    joystick->m_target = NULL;
    joystick->inactive();
    
    this->removeAllChildrenWithCleanup(true);
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("joystick.plist");
    
    if(this->m_stringList)
    {
        this->m_stringList->release(); //释放语言数据
        this->m_stringList = NULL;
    }
    
    if(this->m_releaseTexture)
    {
        CCTextureCache::sharedTextureCache()->removeTextureForKey("map.png");
        CCTextureCache::sharedTextureCache()->removeTextureForKey("main.png");
        
        //数据库部分，释放npc的纹理
        CppSQLite3Query query = this->m_db.execQuery("select * from npc order by id desc");
        while(!query.eof())
        {
            CCTextureCache::sharedTextureCache()->removeTextureForKey(query.getStringField("map_texture"));
            query.nextRow();
        }
        query.finalize();
        
        //player部分
        CCTextureCache::sharedTextureCache()->removeTextureForKey("actor4_0.png");
        CCTextureCache::sharedTextureCache()->removeTextureForKey("joystick.png");
        
    }
    
//    CCLog("RPGMapSceneLayer释放");
}

bool RPGMapSceneLayer::init(float showObjectTime)
{
    if(RPGBaseSceneLayer::init())
    {        
        CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("joystick.plist");
        CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("main.plist");
        
        //加载语言文件
        string languageFile = CCFileUtils::sharedFileUtils()->fullPathForFilename("scene_map_cns.plist");
        this->m_stringList = CCDictionary::createWithContentsOfFileThreadSafe(languageFile.c_str());
        this->m_stringList->retain();
        
        //数据库部分，读取进度记录
        CppSQLite3Query query = this->m_db.execQuery("select m.name as map_name, sd.map_id, m.has_enemy, m.bg_audio, sd.player_to_x, sd.player_to_y, sd.player_direction from save_data as sd inner join map as m on sd.map_id = m.id where sd.id = 1");
        this->m_mapData.mapId = query.getIntField("map_id");
        this->m_mapData.mapName = query.getStringField("map_name");
        this->m_mapData.hasEnemy = query.getIntField("has_enemy") == 1 ? true : false;
        this->m_mapData.bgAudio = query.getStringField("bg_audio");
        this->m_mapData.playerToX = query.getFloatField("player_to_x");
        this->m_mapData.playerToY = query.getFloatField("player_to_y");
        this->m_mapData.playerDirection = query.getStringField("player_direction");
        query.finalize();
        
        CCTMXTiledMap *bgMap = CCTMXTiledMap::create(this->m_mapData.mapName.c_str());
        bgMap->setTag(kRPGMapSceneLayerTagBgMap);
        bgMap->setPosition(ccp((CCDirector::sharedDirector()->getWinSize().width - bgMap->getContentSize().width) / 2.0, (CCDirector::sharedDirector()->getWinSize().height - bgMap->getContentSize().height) / 2.0));
        this->addChild(bgMap);
        
        //背景音乐
        if(SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
        {
            SimpleAudioEngine::sharedEngine()->stopBackgroundMusic(true);
            SimpleAudioEngine::sharedEngine()->playBackgroundMusic(this->m_mapData.bgAudio.c_str(), true);
        }
        
        this->m_playerMoveAct = NULL;
        this->m_playerMoveSpeed = GAME_PLAYER_MOVESPEED;
        this->m_touchedDialogNPC = NULL;
        this->m_dialogDirection = kRPGMapSceneLayerDialogDirectionNone;
        
        this->m_hasEnemy = this->m_mapData.hasEnemy; //是否会遇敌
        this->m_releaseTexture = false; //是否在释构方法里面释放占用纹理
        
        this->scheduleOnce(schedule_selector(RPGMapSceneLayer::startPlay), showObjectTime);
        
        return true;
    }
    return false;
}

RPGMapSceneLayer* RPGMapSceneLayer::create(float showObjectTime)
{
    RPGMapSceneLayer *layer = new RPGMapSceneLayer();
    if(layer && layer->init(showObjectTime))
    {
        layer->autorelease();
        return layer;
    }
    CC_SAFE_DELETE(layer);
    return NULL;
}

CCScene* RPGMapSceneLayer::scene()
{
    return RPGMapSceneLayer::scene(GAME_SCENE);
}

CCScene* RPGMapSceneLayer::scene(float showObjectTime)
{
    CCScene* s = CCScene::create();
    RPGMapSceneLayer *layer = RPGMapSceneLayer::create(showObjectTime);
    s->addChild(layer);
    return s;
}

void RPGMapSceneLayer::update(float dt)
{
    OzgJoystickLayer *joystick = (OzgJoystickLayer*)this->getChildByTag(kRPGMapSceneLayerTagJoystick);
    if(!this->m_playerMoveAct && joystick && joystick->m_trigger)
    {
        CCTMXTiledMap *bgMap = (CCTMXTiledMap*)this->getChildByTag(kRPGMapSceneLayerTagBgMap);
        RPGMapRoleSprite *player = (RPGMapRoleSprite*)bgMap->getChildByTag(kRPGMapSceneLayerTagPlayer);

        float dx = joystick->m_currentPoint.x - joystick->m_centerPoint.x;
        float dy = joystick->m_currentPoint.y - joystick->m_centerPoint.y;
        
        //CCLog("%f %f", dx, dy);
        if(dx > 0 && dy > 0)
        {
            if(fabsf(dx) > fabsf(dy))
            {
                CCPoint targetPoint = ccp(GAME_TMX_ROLE_WIDTH, 0);
                player->startMoveRight();
                
                if(this->doMoving(targetPoint))
                    this->m_playerMoveAct = CCMoveBy::create(this->m_playerMoveSpeed, targetPoint);
            }
            else
            {
                CCPoint targetPoint = ccp(0, GAME_TMX_ROLE_HEIGHT);
                player->startMoveUp();
                
                if(this->doMoving(targetPoint))
                    this->m_playerMoveAct = CCMoveBy::create(this->m_playerMoveSpeed, targetPoint);
            }
            //            CCLog("右上");
        }
        else if(dx > 0 && dy < 0)
        {
            if(fabsf(dx) > fabsf(dy))
            {
                CCPoint targetPoint = ccp(GAME_TMX_ROLE_WIDTH, 0);
                player->startMoveRight();
                
                if(this->doMoving(targetPoint))
                    this->m_playerMoveAct = CCMoveBy::create(this->m_playerMoveSpeed, targetPoint);
            }
            else
            {
                CCPoint targetPoint = ccp(0, -GAME_TMX_ROLE_HEIGHT);
                player->startMoveDown();
                
                if(this->doMoving(targetPoint))
                    this->m_playerMoveAct = CCMoveBy::create(this->m_playerMoveSpeed, targetPoint);
            }
            //            CCLog("右下");
        }
        else if(dx < 0 && dy < 0)
        {
            if(fabsf(dx) > fabsf(dy))
            {
                CCPoint targetPoint = ccp(-GAME_TMX_ROLE_WIDTH, 0);
                player->startMoveLeft();
                
                if(this->doMoving(targetPoint))
                    this->m_playerMoveAct = CCMoveBy::create(this->m_playerMoveSpeed, targetPoint);
            }
            else
            {
                CCPoint targetPoint = ccp(0, -GAME_TMX_ROLE_HEIGHT);
                player->startMoveDown();
                
                if(this->doMoving(targetPoint))
                    this->m_playerMoveAct = CCMoveBy::create(this->m_playerMoveSpeed, targetPoint);
            }
            //            CCLog("左下");
        }
        else if(dx < 0 && dy > 0)
        {
            if(fabsf(dx) > fabsf(dy))
            {
                CCPoint targetPoint = ccp(-GAME_TMX_ROLE_WIDTH, 0);
                player->startMoveLeft();
                
                if(this->doMoving(targetPoint))
                    this->m_playerMoveAct = CCMoveBy::create(this->m_playerMoveSpeed, targetPoint);
            }
            else
            {
                CCPoint targetPoint = ccp(0, GAME_TMX_ROLE_HEIGHT);
                player->startMoveUp();
                
                if(this->doMoving(targetPoint))
                    this->m_playerMoveAct = CCMoveBy::create(this->m_playerMoveSpeed, targetPoint);
            }
            //            CCLog("左上");
        }
        else if(dx == 0 && dy > 0)
        {
            CCPoint targetPoint = ccp(0, GAME_TMX_ROLE_HEIGHT);
            player->startMoveUp();
            
            if(this->doMoving(targetPoint))
                this->m_playerMoveAct = CCMoveBy::create(this->m_playerMoveSpeed, targetPoint);
            //            CCLog("上");
        }
        else if(dx == 0 && dy < 0)
        {
            CCPoint targetPoint = ccp(0, -GAME_TMX_ROLE_HEIGHT);
            player->startMoveDown();
            
            if(this->doMoving(targetPoint))
                this->m_playerMoveAct = CCMoveBy::create(this->m_playerMoveSpeed, targetPoint);
            //            CCLog("下");
        }
        else if(dx < 0 && dy == 0)
        {
            CCPoint targetPoint = ccp(-GAME_TMX_ROLE_WIDTH, 0);
            player->startMoveLeft();
            
            if(this->doMoving(targetPoint))
                this->m_playerMoveAct = CCMoveBy::create(this->m_playerMoveSpeed, targetPoint);
            //            CCLog("左");
        }
        else if(dx > 0 && dy == 0)
        {
            CCPoint targetPoint = ccp(GAME_TMX_ROLE_WIDTH, 0);
            player->startMoveRight();
            
            if(this->doMoving(targetPoint))
                this->m_playerMoveAct = CCMoveBy::create(this->m_playerMoveSpeed, targetPoint);
            //            CCLog("右");

        }

        if(this->m_playerMoveAct)
        {
            this->m_playerMoveAct->setTag(kRPGMapSceneLayerActTagPlayerMove);
            player->runAction(CCSequence::createWithTwoActions(this->m_playerMoveAct, CCCallFunc::create(this, callfunc_selector(RPGMapSceneLayer::playerMoveEnd))));
        }

    }
    
}

bool RPGMapSceneLayer::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    CCTMXTiledMap *bgMap = (CCTMXTiledMap*)this->getChildByTag(kRPGMapSceneLayerTagBgMap);
    RPGMapRoleSprite *player = (RPGMapRoleSprite*)bgMap->getChildByTag(kRPGMapSceneLayerTagPlayer);
    
    //循环判断点击了哪一个NPC，如果有点中则m_isTouchedNPC为true
    for (int i = 0; i < bgMap->getChildren()->count(); i++)
    {
        CCObject *item = bgMap->getChildren()->objectAtIndex(i);
        if(dynamic_cast<RPGMapRoleSprite*>(item) != NULL)
        {
            RPGMapNPCRoleSprite *npc = (RPGMapNPCRoleSprite*)item;
            if(npc->m_type == kRPGMapRoleSpriteTypeNPC)
            {
                //判断player是否存在该NPC的四个方向（距离为一格），存在则为true
                bool exist = false;
                
                if(player->getPosition().equals(ccpAdd(npc->getPosition(), ccp(0, GAME_TMX_ROLE_HEIGHT))))
                {
                    //上
                    exist = true;
                    this->m_dialogDirection = kRPGMapSceneLayerDialogDirectionUp;
                }
                else if(player->getPosition().equals(ccpAdd(npc->getPosition(), ccp(0, -GAME_TMX_ROLE_HEIGHT))))
                {
                    //下
                    exist = true;
                    this->m_dialogDirection = kRPGMapSceneLayerDialogDirectionDown;
                }
                else if(player->getPosition().equals(ccpAdd(npc->getPosition(), ccp(-GAME_TMX_ROLE_WIDTH, 0))))
                {
                    //左
                    exist = true;
                    this->m_dialogDirection = kRPGMapSceneLayerDialogDirectionLeft;
                }
                else if(player->getPosition().equals(ccpAdd(npc->getPosition(), ccp(GAME_TMX_ROLE_WIDTH, 0))))
                {
                    //右
                    exist = true;
                    this->m_dialogDirection = kRPGMapSceneLayerDialogDirectionRight;
                }
                
                if(exist)
                {
                    CCPoint point = touch->getLocation();
                    point = bgMap->convertToNodeSpace(point);
                    if(npc->boundingBox().containsPoint(point))
                    {
                        this->m_touchedDialogNPC = npc;
                        break;
                    }
                }
                
            }
        }
    }
    
    //如果是点中了NPC则进入ccTouchEnded的处理
    if(this->m_touchedDialogNPC)
        return true;
    else
    {
        //传入到摇杆的事件处理
        CCMenu *mainMenu = (CCMenu*)bgMap->getChildByTag(kRPGMapSceneLayerTagMainMenu);
        mainMenu->setVisible(false);
        return false;
    }
    
}

void RPGMapSceneLayer::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
    
}

void RPGMapSceneLayer::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
    if(this->m_touchedDialogNPC)
    {
        CCTMXTiledMap *bgMap = (CCTMXTiledMap*)this->getChildByTag(kRPGMapSceneLayerTagBgMap);
        
        CCPoint point = touch->getLocation();
        point = bgMap->convertToNodeSpace(point);
        
        if(this->m_touchedDialogNPC->boundingBox().containsPoint(point))
        {
            this->m_touchedDialogNPC->stopAutoMove(); //暂时停止自动移动
            
            string content = this->m_touchedDialogNPC->m_content->getCString();
            RPGMapDialogLayer *dialogLayer = RPGMapDialogLayer::create(this->m_touchedDialogNPC);
            this->addChild(dialogLayer);
            
            CCMenu *mainMenu = (CCMenu*)bgMap->getChildByTag(kRPGMapSceneLayerTagMainMenu);
            mainMenu->setVisible(false);
            
            //player和NPC互相对着
            RPGMapRoleSprite *player = (RPGMapRoleSprite*)bgMap->getChildByTag(kRPGMapSceneLayerTagPlayer);
            switch (this->m_dialogDirection)
            {
                case kRPGMapSceneLayerDialogDirectionUp:
                {
                    player->setDirection(kRPGMapRoleSpriteDirectionDown);
                    this->m_touchedDialogNPC->setDirection(kRPGMapRoleSpriteDirectionUp);
                }
                    break;
                case kRPGMapSceneLayerDialogDirectionDown:
                {
                    player->setDirection(kRPGMapRoleSpriteDirectionUp);
                    this->m_touchedDialogNPC->setDirection(kRPGMapRoleSpriteDirectionDown);
                }
                    break;
                case kRPGMapSceneLayerDialogDirectionLeft:
                {
                    player->setDirection(kRPGMapRoleSpriteDirectionRight);
                    this->m_touchedDialogNPC->setDirection(kRPGMapRoleSpriteDirectionLeft);
                }
                    break;
                case kRPGMapSceneLayerDialogDirectionRight:
                {
                    player->setDirection(kRPGMapRoleSpriteDirectionLeft);
                    this->m_touchedDialogNPC->setDirection(kRPGMapRoleSpriteDirectionRight);
                }
                    break;
                default:
                    break;
            }
            
        }
    }
    
    this->m_touchedDialogNPC = NULL;
    this->m_dialogDirection = kRPGMapSceneLayerDialogDirectionNone;
}

void RPGMapSceneLayer::enabledControl(bool enabled)
{
    CCTMXTiledMap *bgMap = (CCTMXTiledMap*)this->getChildByTag(kRPGMapSceneLayerTagBgMap);
    CCMenu *mainMenu = (CCMenu*)bgMap->getChildByTag(kRPGMapSceneLayerTagMainMenu);
    
    OzgJoystickLayer *joystick = (OzgJoystickLayer*)this->getChildByTag(kRPGMapSceneLayerTagJoystick);
    
    if(enabled)
    {
        CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, true);
        
        joystick->active();
        mainMenu->setVisible(true);
        
    }
    else
    {
        CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
        
        joystick->inactive();
        mainMenu->setVisible(false);
        
    }
}

void RPGMapSceneLayer::joystickEndedFunc()
{
    CCTMXTiledMap *bgMap = (CCTMXTiledMap*)this->getChildByTag(kRPGMapSceneLayerTagBgMap);
    RPGMapRoleSprite *player = (RPGMapRoleSprite*)bgMap->getChildByTag(kRPGMapSceneLayerTagPlayer);
    
    CCMenu *mainMenu = (CCMenu*)bgMap->getChildByTag(kRPGMapSceneLayerTagMainMenu);
    mainMenu->setVisible(true);
    
    player->stopMove();
}

//private
void RPGMapSceneLayer::startPlay(float delay)
{
    CCTMXTiledMap *bgMap = (CCTMXTiledMap*)this->getChildByTag(kRPGMapSceneLayerTagBgMap);
    
    //数据库部分，读取npc数据
    CppSQLite3Query query = this->m_db.execQuery(CCString::createWithFormat("select * from npc where map_id = %i order by id desc", this->m_mapData.mapId)->getCString());
    while(!query.eof())
    {
        float x = (stringToNumber<float>(query.getStringField("tmx_x")) + 0.5) * GAME_TMX_ROLE_WIDTH;
        float y = (stringToNumber<float>(query.getStringField("tmx_y")) + 0.5) * GAME_TMX_ROLE_HEIGHT;
        bool autoMove = query.getIntField("auto_move") == 1 ? true : false;
        
        RPGMapNPCRoleSprite *npc = RPGMapNPCRoleSprite::create(CCString::create(query.getStringField("map_texture")), CCString::create("head_texture"), CCString::create(query.getStringField("name_cns")), CCString::create(query.getStringField("content_cns")), 1, autoMove);
        npc->setTag(kRPGMapSceneLayerTagNPC + query.getIntField("id"));
        npc->setPosition(ccp(x, y));
        bgMap->addChild(npc);
        
        query.nextRow();
    }
    query.finalize();
    
    //player
    int defaultSpriteFrameIndex = 1; //默认为向下
    if(this->m_mapData.playerDirection.compare("up") == 0)
    {
        defaultSpriteFrameIndex = 10;
    }
    else if(this->m_mapData.playerDirection.compare("left") == 0)
    {
        defaultSpriteFrameIndex = 4;
    }
    else if(this->m_mapData.playerDirection.compare("right") == 0)
    {
        defaultSpriteFrameIndex = 7;
    }
    RPGMapRoleSprite *player = RPGMapRoleSprite::create(CCString::create("actor4_0.png"), CCString::create("Player"), true, defaultSpriteFrameIndex);
    player->setPosition(ccp(this->m_mapData.playerToX * GAME_TMX_ROLE_WIDTH, this->m_mapData.playerToY * GAME_TMX_ROLE_HEIGHT));
    player->setTag(kRPGMapSceneLayerTagPlayer);
    player->m_type = kRPGMapRoleSpriteTypePlayer;
    bgMap->addChild(player);
    
    CCSprite *spJoystick = CCSprite::createWithSpriteFrameName("Ball.png");
    CCSprite *joystickBg = CCSprite::createWithSpriteFrameName("Dock.png");
    OzgJoystickLayer *joystick = OzgJoystickLayer::layerActivityJoystick(50, spJoystick, joystickBg, false);
    joystick->setTag(kRPGMapSceneLayerTagJoystick);
    joystick->m_target = this;
    joystick->m_endedSelector = callfunc_selector(RPGMapSceneLayer::joystickEndedFunc);
    this->addChild(joystick);
    
    //menu
    CCMenu *mainMenu = CCMenu::create();
    mainMenu->setTag(kRPGMapSceneLayerTagMainMenu);
    mainMenu->setAnchorPoint(ccp(0.5, 0.5));
    mainMenu->setPosition(CCPointZero);
    mainMenu->setContentSize(bgMap->getContentSize());
    bgMap->addChild(mainMenu);

    CCMenuItemSprite *menuCommand = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("commons_btn_back_04.png"), CCSprite::createWithSpriteFrameName("commons_btn_back_04.png"), this, menu_selector(RPGMapSceneLayer::onMenu));
    menuCommand->setPosition(ccp(mainMenu->getContentSize().width - 35, mainMenu->getContentSize().height - 35));
    menuCommand->setTag(kRPGMapSceneLayerTagMainMenuCommand);
    menuCommand->setRotation(180);
    menuCommand->setScale(0.75);
    mainMenu->addChild(menuCommand);

//    CCTextureCache::sharedTextureCache()->dumpCachedTextureInfo();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, true);
    this->scheduleUpdate();
    
}

//private
void RPGMapSceneLayer::onMenu(CCObject *pObject)
{
    SimpleAudioEngine::sharedEngine()->playEffect("audio_effect_btn.wav");
    
    switch (((CCMenuItem*)pObject)->getTag())
    {
        default:
        {
            //点击了右上角的菜单按钮
//            SimpleAudioEngine::sharedEngine()->playEffect("effect_btn.wav", false);
            
            RPGMapMenuLayer *menuLayer = (RPGMapMenuLayer*)this->getChildByTag(kRPGMapSceneLayerTagMenuLayer);
            if(!menuLayer)
            {
                this->enabledControl(false);
                
                //因为动态获取地图的大小会导致了菜单层显示错位，所以定死了
                float width = 960;
                float height = 640;
                
                menuLayer = RPGMapMenuLayer::create(this->m_stringList, &this->m_db, width, height);
                menuLayer->setPosition(ccp((CCDirector::sharedDirector()->getWinSize().width - width) / 2.0, (CCDirector::sharedDirector()->getWinSize().height - height) / 2.0));
                menuLayer->setTag(kRPGMapSceneLayerTagMenuLayer);
                this->addChild(menuLayer);
            }
            
        }
            break;
    }
    
}

void RPGMapSceneLayer::playerMoveEnd()
{
    CCTMXTiledMap *bgMap = (CCTMXTiledMap*)this->getChildByTag(kRPGMapSceneLayerTagBgMap);
    RPGMapRoleSprite *player = (RPGMapRoleSprite*)bgMap->getChildByTag(kRPGMapSceneLayerTagPlayer);
    player->stopMove();
    
    player->stopActionByTag(kRPGMapSceneLayerActTagPlayerMove);

    this->m_playerMoveAct = NULL;
    
    //地图切换点的判断
    CCTMXObjectGroup *transitionsObjects = bgMap->objectGroupNamed("transitions");
    for (int i = 0; i < transitionsObjects->getObjects()->count(); i++)
    {
        CCDictionary *transitionsObject = (CCDictionary*)transitionsObjects->getObjects()->objectAtIndex(i);
        const CCString *x = transitionsObject->valueForKey("x");
        const CCString *y = transitionsObject->valueForKey("y");
        const CCString *width = transitionsObject->valueForKey("width");
        const CCString *height = transitionsObject->valueForKey("height");
        
        const CCString *mapId = transitionsObject->valueForKey("map_id");
        const CCString *playerToX = transitionsObject->valueForKey("player_to_x");
        const CCString *playerToY = transitionsObject->valueForKey("player_to_y");
        const CCString *playerDirection = transitionsObject->valueForKey("player_direction");
        
        CCRect transitionsRect = CCRectMake(stringToNumber<float>(x->getCString()), stringToNumber<float>(y->getCString()), stringToNumber<float>(width->getCString()), stringToNumber<float>(height->getCString()));
        
        if(transitionsRect.containsPoint(player->getPosition()))
        {
//            CCLog("切换场景%s", mapId->getCString());
            
            //数据库部分，更新进度记录
            
            //修正偏差位置，因为无法得知下一个地图的大小，所以目前只能在tmx上面定死
            float toX = stringToNumber<float>(playerToX->getCString()) + 0.5;
            float toY = stringToNumber<float>(playerToY->getCString()) + 0.5;
            
            //保存数据
            saveData(&this->m_db, stringToNumber<int>(mapId->getCString()), toX, toY, playerDirection->getCString());
            
            CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
            this->unscheduleUpdate();
            
            CCScene *s = RPGMapSceneLayer::scene(0.0);
            CCTransitionFade *t = CCTransitionFade::create(GAME_SCENE, s);
            CCDirector::sharedDirector()->replaceScene(t);
            return;
        }
        
    }
    
    //遇敌处理
//    this->m_hasEnemy = true; //test
    if(this->m_hasEnemy)
    {
        float val = CCRANDOM_0_1();
        val = CCRANDOM_0_1();
        if(val >= 0.0 && val <= 0.5)
        {
//            CCLog("遇敌!");
            CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
            this->unscheduleUpdate();
            
            //保存进度
            CCTMXTiledMap *bgMap = (CCTMXTiledMap*)this->getChildByTag(kRPGMapSceneLayerTagBgMap);
            
            RPGMapRoleSprite *player = (RPGMapRoleSprite*)bgMap->getChildByTag(kRPGMapSceneLayerTagPlayer);
            float toX = player->getPositionX();
            float toY = player->getPositionY();
            toX = toX / GAME_TMX_ROLE_WIDTH;
            toY = toY / GAME_TMX_ROLE_HEIGHT;
            
            string playerDirection;
            switch (player->m_direction)
            {
                case kRPGMapRoleSpriteDirectionDown:
                    playerDirection = "down";
                    
                    break;
                case kRPGMapRoleSpriteDirectionLeft:
                    playerDirection = "left";
                    
                    break;
                case kRPGMapRoleSpriteDirectionRight:
                    playerDirection = "right";
                    
                    break;
                default:
                    //kRPGMapRoleSpriteDirectionUp
                    playerDirection = "up";
                
                    break;
            }
            
            saveData(&this->m_db, this->m_mapData.mapId, toX, toY, playerDirection);
            
            //保存进度结束
            
            CCArray *loadTextures = CCArray::create();
            loadTextures->addObject(CCString::create("monsters.png"));
            loadTextures->addObject(CCString::create("battle_bg.png"));
            
            CCArray *releaseTextures = CCArray::create();
            releaseTextures->addObject(CCString::create("map.png"));
            releaseTextures->addObject(CCString::create("joystick.png"));
            releaseTextures->addObject(CCString::create("actor4_0.png"));
            releaseTextures->addObject(CCString::create("actor111.png"));
            releaseTextures->addObject(CCString::create("actor113.png"));
            releaseTextures->addObject(CCString::create("actor114.png"));
            releaseTextures->addObject(CCString::create("actor115.png"));
            releaseTextures->addObject(CCString::create("actor117.png"));
            releaseTextures->addObject(CCString::create("actor120.png"));
            
            CCMenu *mainMenu = (CCMenu*)bgMap->getChildByTag(kRPGMapSceneLayerTagMainMenu);
            mainMenu->setEnabled(false);
            
            SimpleAudioEngine::sharedEngine()->playEffect("audio_battle_start.wav");
            
//            CCLog("%i", this->m_saveData.mapId);
            CCUserDefault::sharedUserDefault()->setIntegerForKey("map_id", this->m_mapData.mapId);
            
            CCScene *s = RPGLoadingSceneLayer::scene(loadTextures, releaseTextures, "single_battle");
            CCTransitionFade *t = CCTransitionFade::create(GAME_SCENE, s);
            CCDirector::sharedDirector()->replaceScene(t);
        }
    }
    
}

bool RPGMapSceneLayer::doMoving(cocos2d::CCPoint targetPoint)
{
    //这里的逻辑跟RPGMapNPCRoleSprite::autoMove类似
    
    CCTMXTiledMap *bgMap = (CCTMXTiledMap*)this->getChildByTag(kRPGMapSceneLayerTagBgMap);
    RPGMapRoleSprite *player = (RPGMapRoleSprite*)bgMap->getChildByTag(kRPGMapSceneLayerTagPlayer);
    CCTMXObjectGroup *obstaclesObjects = bgMap->objectGroupNamed("obstacles");
    
    targetPoint = ccpAdd(player->getPosition(), targetPoint);
    
    //地图上的障碍物
    for (int i = 0; i < obstaclesObjects->getObjects()->count(); i++)
    {
        CCDictionary *obstaclesObject = (CCDictionary*)obstaclesObjects->getObjects()->objectAtIndex(i);
        const CCString *x = obstaclesObject->valueForKey("x");
        const CCString *y = obstaclesObject->valueForKey("y");
        const CCString *width = obstaclesObject->valueForKey("width");
        const CCString *height = obstaclesObject->valueForKey("height");
        
        CCRect obstaclesRect = CCRectMake(stringToNumber<float>(x->getCString()), stringToNumber<float>(y->getCString()), stringToNumber<float>(width->getCString()), stringToNumber<float>(height->getCString()));
        
        if(obstaclesRect.containsPoint(targetPoint))
            return false;
    }
    
    //NPC或Player
    for (int i = 0; i < bgMap->getChildren()->count(); i++)
    {
        CCObject *item = bgMap->getChildren()->objectAtIndex(i);
        if(dynamic_cast<RPGMapRoleSprite*>(item) != NULL)
        {
            //如果为非player的对象才会执行里面的判断
            if(item != player)
            {
                if(((RPGMapRoleSprite*)item)->boundingBox().containsPoint(targetPoint))
                    return false;
            }
        }
    }
    
    return true;
}
