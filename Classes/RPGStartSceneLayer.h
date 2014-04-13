//
//  RPGStartSceneLayer.h
//  OzgGameRPG
//
//  Created by ozg on 14-3-17.
//
//

#ifndef __OzgGameRPG__RPGStartSceneLayer__
#define __OzgGameRPG__RPGStartSceneLayer__

#include "RPGBaseSceneLayer.h"

//各个显示对象的tag
enum RPGStartSceneLayerTag
{
    kRPGStartSceneLayerTagBg = 1,
    kRPGStartSceneLayerTagMenu = 2,
    kRPGStartSceneLayerTagMenuItemStart = 3,
    kRPGStartSceneLayerTagMenuItemDelete = 4,
    kRPGStartSceneLayerTagDialogOK = 5,
    kRPGStartSceneLayerTagDialogCancel = 6,
    kRPGStartSceneLayerTagDialog = 7
};

class RPGStartSceneLayer : public RPGBaseSceneLayer
{
    
private:
    void onMenu(CCObject *pObject);
    void onDialog(CCObject *pObject);
    
public:
    
    RPGStartSceneLayer();
    virtual ~RPGStartSceneLayer();
    virtual bool init();
    CREATE_FUNC(RPGStartSceneLayer);
    
    static CCScene* scene();
    
    void goToMapScene();
    
};

#endif /* defined(__OzgGameRPG__RPGStartSceneLayer__) */
