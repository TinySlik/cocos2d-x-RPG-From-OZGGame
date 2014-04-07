//
//  RPGBaseSceneLayer.h
//  OzgGameRPG
//
//  Created by ozg on 14-3-17.
//
//

#ifndef __OzgGameRPG__RPGBaseSceneLayer__
#define __OzgGameRPG__RPGBaseSceneLayer__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "JsonBox.h"
#include "SimpleAudioEngine.h"
#include "CppSQLite3.h"
#include "GameCfg.h"
#include "RPGConfirmLayer.h"
#include "RPGData.h"

USING_NS_CC;
USING_NS_CC_EXT;

using namespace std;
using namespace CocosDenshion;

//获取player列表
#define PLAYER_QUERY "select * from player order by id asc"

void addLab(CCNode* parentNode, int tag, CCString* text, CCPoint point); //增加状态界面的lab
void addLab(CCNode* parentNode, int tag, CCString* text, float fontSize, CCPoint point);
void addLab(CCNode* parentNode, int tag, CCString* text, float fontSize, CCTextAlignment textAlignment, CCPoint point);

void saveData(CppSQLite3DB* db, int mapId, float toX, float toY, string playerDirection);
void savePlayerData(CppSQLite3DB* db, CCArray* playerDataList);

class RPGBaseSceneLayer : public CCLayer
{
    
protected:
    CCDictionary *m_stringList; //语言文件的数据
    CppSQLite3DB m_db;
        
public:
    
    RPGBaseSceneLayer();
    virtual ~RPGBaseSceneLayer();
    virtual bool init();
    
};

#endif /* defined(__OzgGameRPG__RPGBaseSceneLayer__) */
