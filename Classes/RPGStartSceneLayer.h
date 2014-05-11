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
    kRPGStartSceneLayerTagMainMenu = 2,
    kRPGStartSceneLayerTagMainMenuItemStart = 3,
    kRPGStartSceneLayerTagMainMenuItemRestart = 4,
    kRPGStartSceneLayerTagMainMenuItemSettings = 9,
    kRPGStartSceneLayerTagDialogOK = 5,
    kRPGStartSceneLayerTagDialogCancel = 6,
    kRPGStartSceneLayerTagDialog = 7,
    kRPGStartSceneLayerTagSettingsMenu = 8,
    kRPGStartSceneLayerTagSettingsStyleLeft = 10,
    kRPGStartSceneLayerTagSettingsStyleRight = 12,
    kRPGStartSceneLayerTagSettingsRestore = 14,
    kRPGStartSceneLayerTagSettingsBack = 13,
    kRPGStartSceneLayerTagLabStyle = 11,
    kRPGStartSceneLayerTagLabCurrStyle = 15
};

class RPGStartSceneLayer : public RPGBaseSceneLayer
{
    
private:
    
    vector< pair<string, string> > m_styleData;
    int m_selectedStyleIndex;
    
    void onMenu(CCObject *pObject);
    void onDialog(CCObject *pObject);
    
    void showMainMenu();
    void showSettingsMenu();
    
public:
    
    RPGStartSceneLayer();
    virtual ~RPGStartSceneLayer();
    virtual bool init();
    CREATE_FUNC(RPGStartSceneLayer);
    
    static CCScene* scene();
    
    void goToMapScene();
    
};

#endif /* defined(__OzgGameRPG__RPGStartSceneLayer__) */
