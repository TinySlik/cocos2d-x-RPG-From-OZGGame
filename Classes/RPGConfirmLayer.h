//
//  RPGConfirmLayer.h
//  OzgGameRPG
//
//  Created by ozg on 14-3-30.
//
//

#ifndef __OzgGameRPG__RPGConfirmLayer__
#define __OzgGameRPG__RPGConfirmLayer__

#include "cocos2d.h"

USING_NS_CC;
using namespace std;

class RPGConfirmLayer : public CCLayerColor
{
        
public:
    
    RPGConfirmLayer();
    virtual ~RPGConfirmLayer();
    bool init(string titleStr, string menuOKStr, int menuItemOKTag, string menuCancelStr, int menuItemCancelTag, float width, float height, CCObject* target, SEL_MenuHandler seletor);
    static RPGConfirmLayer* create(string titleStr, string menuOKStr, int menuItemOKTag, string menuCancelStr, int menuItemCancelTag, float width, float height, CCObject* target, SEL_MenuHandler seletor);
    
};

#endif /* defined(__OzgGameRPG__RPGConfirmLayer__) */
