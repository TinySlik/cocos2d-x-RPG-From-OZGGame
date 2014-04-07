//
//  RPGMapSceneLayer.h
//  OzgGameRPG
//
//  Created by ozg on 14-3-17.
//
//

#ifndef __OzgGameRPG__RPGMapSceneLayer__
#define __OzgGameRPG__RPGMapSceneLayer__

#include "RPGBaseSceneLayer.h"
#include "RPGMapNPCRoleSprite.h"

enum RPGMapSceneLayerTag
{
    kRPGMapSceneLayerTagBgMap = 1,
    kRPGMapSceneLayerTagJoystick = 2,
    kRPGMapSceneLayerTagMainMenu = 3,
    kRPGMapSceneLayerTagMainMenuCommand = 4,
    kRPGMapSceneLayerTagMenuLayer = 5,
    kRPGMapSceneLayerTagPlayer = 1999,
    kRPGMapSceneLayerTagNPC = 2000
    
};

enum RPGMapSceneLayerActTag
{
    kRPGMapSceneLayerActTagPlayerMove = 20
    
};

enum RPGMapSceneLayerDialogDirection
{
    kRPGMapSceneLayerDialogDirectionNone = 0,
    kRPGMapSceneLayerDialogDirectionUp = 1,
    kRPGMapSceneLayerDialogDirectionDown = 2,
    kRPGMapSceneLayerDialogDirectionLeft = 3,
    kRPGMapSceneLayerDialogDirectionRight = 4
};

struct RPGMapData
{
    int mapId;
    string mapName;
    bool hasEnemy;
    string bgAudio;
    float playerToX;
    float playerToY;
    string playerDirection;
};

class RPGMapSceneLayer : public RPGBaseSceneLayer
{
    
private:
    
    CCMoveBy *m_playerMoveAct;
    float m_playerMoveSpeed; //player移动的速度
    
    RPGMapData m_mapData; //地图场景的公用数据
    
    RPGMapNPCRoleSprite* m_touchedDialogNPC; //点中了可对话的NPC，player存在NPC的四个方向才可以点中，如果不存在即使点中了也是为NULL
    RPGMapSceneLayerDialogDirection m_dialogDirection; //跟NPC对话时，player在NPC的哪个方向
        
    void startPlay(float delay);
    void onMenu(CCObject *pObject);
    void playerMoveEnd();
    
    bool doMoving(CCPoint targetPoint);
    
public:
    bool m_hasEnemy;
    bool m_releaseTexture;
        
    RPGMapSceneLayer();
    virtual ~RPGMapSceneLayer();
    bool init(float showObjectTime);
    static RPGMapSceneLayer* create(float showObjectTime);
    
    static CCScene* scene();
    static CCScene* scene(float showObjectTime);
    
    void update(float dt);
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    void enabledControl(bool enabled);
    
    void joystickEndedFunc(); //摇杆事件的结束回调
    
};

#endif /* defined(__OzgGameRPG__RPGMapSceneLayer__) */
