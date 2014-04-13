//
//  RPGDialogLayer.h
//  OzgGameRPG
//
//  Created by ozg on 14-3-30.
//
//

#ifndef __OzgGameRPG__RPGDialogLayer__
#define __OzgGameRPG__RPGDialogLayer__

#include "cocos2d.h"

USING_NS_CC;
using namespace std;

enum RPGDialogLayerTag
{
    kRPGDialogLayerBg = 1,
    kRPGDialogLayerBgMenu = 2
    
};

class RPGDialogLayer : public CCLayerColor
{

private:
    void initCommons(string titleStr);
    
public:
    
    RPGDialogLayer();
    virtual ~RPGDialogLayer();
    
    //只有一个确定按钮
    bool init(string titleStr, string menuOKStr, int menuItemOKTag, float width, float height, CCObject* target, SEL_MenuHandler seletor);
    
    //有确定和取消的按钮
    bool init(string titleStr, string menuOKStr, int menuItemOKTag, string menuCancelStr, int menuItemCancelTag, float width, float height, CCObject* target, SEL_MenuHandler seletor);
    
    static RPGDialogLayer* create(string titleStr, string menuOKStr, int menuItemOKTag, float width, float height, CCObject* target, SEL_MenuHandler seletor);
    
    static RPGDialogLayer* create(string titleStr, string menuOKStr, int menuItemOKTag, string menuCancelStr, int menuItemCancelTag, float width, float height, CCObject* target, SEL_MenuHandler seletor);
    
};

#endif /* defined(__OzgGameRPG__RPGDialogLayer__) */
