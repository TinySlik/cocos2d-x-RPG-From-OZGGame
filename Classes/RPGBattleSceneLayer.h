//
//  RPGBattleSceneLayer.h
//  OzgGameRPG
//
//  Created by ozg on 14-3-18.
//
//

#ifndef __OzgGameRPG__RPGBattleSceneLayer__
#define __OzgGameRPG__RPGBattleSceneLayer__

#include "RPGBaseSceneLayer.h"
#include "RPGBattlePlayerSprite.h"
#include "RPGBattleMonsterSprite.h"
#include "RPGBattleMenu.h"
#include "RPGComputingResults.h"
#include "OzgCCUtility.h"

enum RPGBattleSceneLayerTag
{
    kRPGBattleSceneLayerTagBg = 1,
    kRPGBattleSceneLayerTagMenuBg = 2,
    kRPGBattleSceneLayerTagBattleMenu = 3,
    kRPGBattleSceneLayerTagMainMenu = 66,
    kRPGBattleSceneLayerTagPauseMask = 67,
    kRPGBattleSceneLayerTagMainMenuPause = 68,
    kRPGBattleSceneLayerTagBtnResume = 69,
    kRPGBattleSceneLayerTagWinResultsDialog = 70,
    kRPGBattleSceneLayerTagLoseResultsDialog = 71,
    kRPGBattleSceneLayerTagSkillSelectDialog = 72,
    kRPGBattleSceneLayerTagItemsSelectDialog = 73,
    kRPGBattleSceneLayerTagMsg = 98,
    kRPGBattleSceneLayerTagPlayer = 100,
    kRPGBattleSceneLayerTagPlayerNameLab = 200,
    kRPGBattleSceneLayerTagPlayerProgress = 300,
    kRPGBattleSceneLayerTagPlayerProgressBg = 400,
    kRPGBattleSceneLayerTagPlayerHP = 500,
    kRPGBattleSceneLayerTagPlayerHPSeparate = 600,
    kRPGBattleSceneLayerTagPlayerMaxHP = 700,
    kRPGBattleSceneLayerTagPlayerMP = 800,
    kRPGBattleSceneLayerTagPlayerMPSeparate = 900,
    kRPGBattleSceneLayerTagPlayerMaxMP = 1000,
    kRPGBattleSceneLayerTagMonster = 10000,
    kRPGBattleSceneLayerTagMonsterNameLab = 20000
    
};

class RPGBattleSceneLayer : public RPGBaseSceneLayer
{
    
private:
    CCArray* m_playerDataList;
    CCArray* m_monsterDataList; //因为同一个怪物会有N个，所以这个对象是二维的
    
    CCArray* m_playerList;
    CCArray* m_monsterList; //怪物列表，包括相同的
    
    vector<int> m_addedMonsters; //已显示的怪物tag，主要用在setMonsterPosition
    
    float m_speedTotal; //所有战斗对象的速度加起来的总值，主要用在computingProgress
    
    bool m_autoReleaseMsg; //是否正在执行自动释放msg框
    
    bool m_enabledTouched; //是否打开了可点击状态（主要可点击player或怪物）
    
    int m_totalGold; //临时保存战斗胜利后所获得的金钱，用来在胜利后执行goToMap()之前使用的
    
    //对应的怪物的tag是否存在m_monsterDataList
    bool monsterDataListExistWithTag(int tag);
    
    //获取对应怪物id的一组怪物，如果不存在则返回一个新的空数组
    CCArray* monsterDataListWithDataId(int dataId);
    
    void setMonsterPosition(RPGBattleMonsterSprite* monster);
    
    void computingProgress(); //计算战斗画面中的所有战斗对象的进度条
    float computingFloat(float val); //计算浮动值
    
    bool judgeWin(); //判断是否胜利，true为胜利，false继续战斗
    bool judgeLose(); //判断是否输了，true为输了，false继续战斗
    
    void showWinResults(); //显示胜利后获得的经验值和金钱，统计升级也在这里执行
    void showLoseResults(); //显示输了的界面
    
    void onMenu(cocos2d::CCObject *pObject);
    void onButton(CCObject* pSender, CCControlEvent event);
    
public:
    
    CCArray* m_existingItems; //已有道具
    
    RPGBattleSceneLayer();
    virtual ~RPGBattleSceneLayer();
    virtual bool init();
    CREATE_FUNC(RPGBattleSceneLayer);
    
    static CCScene* scene();
    
    virtual void update(float delta);
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    void showMsg(CCString* msgText); //显示msg
    void showMsg(CCString* msgText, bool autoRelease);
    void hideMsg();
    void hideMsg(float dt);
    
    void goToMap();
    
    void enabledTouched(bool enabled); //是否打开可点击状态（主要可点击player或怪物）
    void cancelAllSelected(); //取消全部选中的对象
    
    //攻击
    void attack(RPGBaseRole* attackObjData, RPGBaseRole* targetObjData); //player或怪物执行攻击
    void attackResults(CCNode* sender, void* data); //回调，计算攻击后的结果
    
    //技能
    void skill(RPGBaseRole* attackObjData, RPGBaseRole* targetObjData); //player或怪物执行技能
    void skillResults(CCNode* sender, void* data); //回调，计算攻击后的结果
    
    //道具
    void useItem(RPGBaseRole* useItemObjData, RPGBaseRole* targetObjData); //player执行使用道具
    void useItemResults(CCNode* sender, void* data); //回调，计算使用道具后的结果
    
    void actionWithTargetEffectLabEnd(CCNode* sender, void* data); //回调，攻击、技能、道具流程完毕后执行，这个sender是目标对象中显示的效果值CCLabelTTF
};

#endif /* defined(__OzgGameRPG__RPGBattleSceneLayer__) */
