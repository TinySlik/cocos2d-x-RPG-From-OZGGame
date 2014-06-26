//
//  RPGStartSceneLayer.cpp
//  OzgGameRPG
//
//  Created by ozg on 14-3-17.
//
//

#include "RPGStartSceneLayer.h"
#include "OzgCCScrollBgNode.h"
#include "OzgCCUtility.h"
#include "OzgFileUtility.h"
#include "RPGLoadingSceneLayer.h"
#include "RPGMapSceneLayer.h"

RPGStartSceneLayer::RPGStartSceneLayer()
{
    
}

RPGStartSceneLayer::~RPGStartSceneLayer()
{
    this->removeAllChildrenWithCleanup(true);
    
    if(this->m_stringList)
    {
        this->m_stringList->release(); //释放语言数据
        this->m_stringList = NULL;
    }
    
    //CCTextureCache::sharedTextureCache()->dumpCachedTextureInfo();
}

bool RPGStartSceneLayer::init()
{
    if(RPGBaseSceneLayer::init())
    {
        //加载语言文件
        string languageFile = CCFileUtils::sharedFileUtils()->fullPathForFilename("scene_start_cns.plist");
        this->m_stringList = CCDictionary::createWithContentsOfFileThreadSafe(languageFile.c_str());
        this->m_stringList->retain();
        
        //样式数据
        this->m_styleData.push_back(pair<string, string>("style1", ((CCString*)this->m_stringList->objectForKey("settingsmenu_style01"))->getCString()));
        this->m_styleData.push_back(pair<string, string>("style2", ((CCString*)this->m_stringList->objectForKey("settingsmenu_style02"))->getCString()));
        this->m_styleData.push_back(pair<string, string>("style3", ((CCString*)this->m_stringList->objectForKey("settingsmenu_style03"))->getCString()));
        
        //使用数据库的当前样式
        RPGSaveData *saveDataObj = loadSaveData(&this->m_db);
        CCLog("%s", saveDataObj->m_windowStyle.c_str());
        CCUserDefault::sharedUserDefault()->setStringForKey(GAME_STYLE, saveDataObj->m_windowStyle);
        
        //背景
        OzgCCScrollBgNode *bg = OzgCCScrollBgNode::create("start_bg.png");
        bg->setAnchorPoint(CCPointZero);
        bg->setPosition(CCPointZero);
        bg->setTag(kRPGStartSceneLayerTagBg);
        this->addChild(bg);
        
        this->showMainMenu();
        
        //背景音乐
        if(!SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
            SimpleAudioEngine::sharedEngine()->playBackgroundMusic("audio_start.mp3", true);
        
        return true;
    }
    return false;
}

CCScene* RPGStartSceneLayer::scene()
{
    CCScene* s = CCScene::create();
    RPGStartSceneLayer *layer = RPGStartSceneLayer::create();
    s->addChild(layer);
    return s;
}

void RPGStartSceneLayer::onMenu(cocos2d::CCObject *pObject)
{
    CCMenuItem *menuItem = (CCMenuItem*)pObject;
    
    switch (menuItem->getTag())
    {
        case kRPGStartSceneLayerTagMainMenuItemRestart:
        {
            //重新开始
            SimpleAudioEngine::sharedEngine()->playEffect("audio_effect_btn.wav");
            
            CCSize winSize = CCDirector::sharedDirector()->getWinSize();
            
            RPGDialogLayer *dialog = RPGDialogLayer::create(((CCString*)this->m_stringList->objectForKey("confirm_delete"))->getCString(), ((CCString*)this->m_stringList->objectForKey("confirm_ok"))->getCString(), kRPGStartSceneLayerTagDialogOK, ((CCString*)this->m_stringList->objectForKey("confirm_cancel"))->getCString(), kRPGStartSceneLayerTagDialogCancel, winSize.width, winSize.height, this, menu_selector(RPGStartSceneLayer::onDialog));
            dialog->setTag(kRPGStartSceneLayerTagDialog);
            this->addChild(dialog);
            
            CCMenu *mainMenu = (CCMenu*)this->getChildByTag(kRPGStartSceneLayerTagMainMenu);
            mainMenu->setEnabled(false);
        }
            break;
        case kRPGStartSceneLayerTagMainMenuItemSettings:
        {
            //游戏设置
            SimpleAudioEngine::sharedEngine()->playEffect("audio_effect_btn.wav");
            
            this->removeChildByTag(kRPGStartSceneLayerTagMainMenu, true);
            this->showSettingsMenu();
            
        }
            break;
        case kRPGStartSceneLayerTagSettingsBack:
        {
            //游戏设置里面的返回
            SimpleAudioEngine::sharedEngine()->playEffect("audio_effect_btn.wav");
            
            this->removeChildByTag(kRPGStartSceneLayerTagSettingsMenu, true);
            this->removeChildByTag(kRPGStartSceneLayerTagLabStyle, true);
            this->removeChildByTag(kRPGStartSceneLayerTagLabCurrStyle, true);
            this->removeChildByTag(kRPGStartSceneLayerTagLabBgAudio, true);
            this->removeChildByTag(kRPGStartSceneLayerTagLabEffectAudio, true);
            this->showMainMenu();
            
        }
            break;
        case kRPGStartSceneLayerTagSettingsRestore:
        {
            //点击了还原设置
            SimpleAudioEngine::sharedEngine()->playEffect("audio_effect_btn.wav");
            
            //还原样式设置
            this->m_selectedStyleIndex = 1;
            CCLabelTTF *labCurrStyleLabel = (CCLabelTTF*)this->getChildByTag(kRPGStartSceneLayerTagLabCurrStyle);
            labCurrStyleLabel->setString(this->m_styleData[this->m_selectedStyleIndex].second.c_str());
            
            //更新数据库
            RPGSaveData *saveDataObj = loadSaveData(&this->m_db);
            saveDataObj->m_windowStyle = this->m_styleData[this->m_selectedStyleIndex].first;
            saveData(&this->m_db, saveDataObj);
            
            //更新当前样式
            CCUserDefault::sharedUserDefault()->setStringForKey(GAME_STYLE, saveDataObj->m_windowStyle);
            
            CCMenu *settingsMenu = (CCMenu*)this->getChildByTag(kRPGStartSceneLayerTagSettingsMenu);
            
            //还原背景音乐
            CCUserDefault::sharedUserDefault()->setFloatForKey(GAME_BG_AUDIO_VOLUME, 1.0);
            ((CCMenuItemLabel*)settingsMenu->getChildByTag(kRPGStartSceneLayerTagSettingsBgAudioBtn))->setString(((CCString*)this->m_stringList->objectForKey("settingsmenu_audio_off"))->getCString());
            SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(1);
            
            //还原效果声音
            CCUserDefault::sharedUserDefault()->setFloatForKey(GAME_EFFECT_AUDIO_VOLUME, 1.0);
            ((CCMenuItemLabel*)settingsMenu->getChildByTag(kRPGStartSceneLayerTagSettingsEffectAudioBtn))->setString(((CCString*)this->m_stringList->objectForKey("settingsmenu_audio_off"))->getCString());
            SimpleAudioEngine::sharedEngine()->setEffectsVolume(1);
            
        }
            break;
        case kRPGStartSceneLayerTagSettingsStyleLeft:
        {
            //点击了样式选择的<<
            SimpleAudioEngine::sharedEngine()->playEffect("audio_effect_btn.wav");
            
            this->m_selectedStyleIndex--;
            if(this->m_selectedStyleIndex < 0)
                this->m_selectedStyleIndex = 0;
            
            CCLabelTTF *labCurrStyleLabel = (CCLabelTTF*)this->getChildByTag(kRPGStartSceneLayerTagLabCurrStyle);
            labCurrStyleLabel->setString(this->m_styleData[this->m_selectedStyleIndex].second.c_str());
            
            //更新数据
            RPGSaveData *saveDataObj = loadSaveData(&this->m_db);
            saveDataObj->m_windowStyle = this->m_styleData[this->m_selectedStyleIndex].first;
            saveData(&this->m_db, saveDataObj);
            
            //更新当前样式
            CCUserDefault::sharedUserDefault()->setStringForKey(GAME_STYLE, saveDataObj->m_windowStyle);
        }
            break;
        case kRPGStartSceneLayerTagSettingsStyleRight:
        {
            //点击了样式选择的>>
            SimpleAudioEngine::sharedEngine()->playEffect("audio_effect_btn.wav");
            
            this->m_selectedStyleIndex++;
            int max = (int)this->m_styleData.size() - 1;
            if(this->m_selectedStyleIndex > max)
                this->m_selectedStyleIndex = max;
            
            CCLabelTTF *labCurrStyleLabel = (CCLabelTTF*)this->getChildByTag(kRPGStartSceneLayerTagLabCurrStyle);
            labCurrStyleLabel->setString(this->m_styleData[this->m_selectedStyleIndex].second.c_str());
            
            //更新数据
            RPGSaveData *saveDataObj = loadSaveData(&this->m_db);
            saveDataObj->m_windowStyle = this->m_styleData[this->m_selectedStyleIndex].first;
            saveData(&this->m_db, saveDataObj);
            
            //更新当前样式
            CCUserDefault::sharedUserDefault()->setStringForKey(GAME_STYLE, saveDataObj->m_windowStyle);
        }
            break;
        case kRPGStartSceneLayerTagSettingsBgAudioBtn:
        {
            //点击了背景音乐的开关
            SimpleAudioEngine::sharedEngine()->playEffect("audio_effect_btn.wav");
            
            if(CCUserDefault::sharedUserDefault()->getFloatForKey(GAME_BG_AUDIO_VOLUME, 1.0) == 0)
            {
                CCUserDefault::sharedUserDefault()->setFloatForKey(GAME_BG_AUDIO_VOLUME, 1.0);
                
                ((CCMenuItemLabel*)menuItem)->setString(((CCString*)this->m_stringList->objectForKey("settingsmenu_audio_off"))->getCString());
                SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(1);
            }
            else
            {
                CCUserDefault::sharedUserDefault()->setFloatForKey(GAME_BG_AUDIO_VOLUME, 0);
                
                ((CCMenuItemLabel*)menuItem)->setString(((CCString*)this->m_stringList->objectForKey("settingsmenu_audio_on"))->getCString());
                SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0);
            }
            
        }
            break;
        case kRPGStartSceneLayerTagSettingsEffectAudioBtn:
        {
            //点击了效果声音的开关
            SimpleAudioEngine::sharedEngine()->playEffect("audio_effect_btn.wav");
            
            if(CCUserDefault::sharedUserDefault()->getFloatForKey(GAME_EFFECT_AUDIO_VOLUME, 1.0) == 0)
            {
                CCUserDefault::sharedUserDefault()->setFloatForKey(GAME_EFFECT_AUDIO_VOLUME, 1.0);
                
                ((CCMenuItemLabel*)menuItem)->setString(((CCString*)this->m_stringList->objectForKey("settingsmenu_audio_off"))->getCString());
                SimpleAudioEngine::sharedEngine()->setEffectsVolume(1);
            }
            else
            {
                CCUserDefault::sharedUserDefault()->setFloatForKey(GAME_EFFECT_AUDIO_VOLUME, 0);
                
                ((CCMenuItemLabel*)menuItem)->setString(((CCString*)this->m_stringList->objectForKey("settingsmenu_audio_on"))->getCString());
                SimpleAudioEngine::sharedEngine()->setEffectsVolume(0);
            }
        }
            break;
        default:
        {
            //开始游戏
            
            SimpleAudioEngine::sharedEngine()->playEffect("audio_start_btn.wav");
            this->goToMapScene();
        }
            break;
    }
    
}

void RPGStartSceneLayer::onDialog(cocos2d::CCObject *pObject)
{
    SimpleAudioEngine::sharedEngine()->playEffect("audio_effect_btn.wav");
    
    CCMenuItem *menuItem = (CCMenuItem*)pObject;
    
    switch (menuItem->getTag())
    {
        case kRPGStartSceneLayerTagDialogCancel:
        {
            this->removeChildByTag(kRPGStartSceneLayerTagDialog, true);
            
            CCMenu *mainMenu = (CCMenu*)this->getChildByTag(kRPGStartSceneLayerTagMainMenu);
            mainMenu->setEnabled(true);
        }
            break;
        default:
        {
            //kRPGStartSceneLayerTagMenuItemDialogOK
            
            //清除数据
            this->m_db.execDML("delete from save_data");
            this->m_db.execDML("delete from items_existing");
            this->m_db.execDML(GAME_INIT_SQL);
            
            //默认设置
            CCUserDefault::sharedUserDefault()->setStringForKey(GAME_STYLE, this->m_styleData[1].first);
            
            this->goToMapScene();
        }
            break;
    }
}

void RPGStartSceneLayer::showMainMenu()
{
    CCMenu *mainMenu = CCMenu::create();
    mainMenu->setAnchorPoint(CCPointZero);
    mainMenu->setPosition(CCPointZero);
    mainMenu->setTag(kRPGStartSceneLayerTagMainMenu);
    this->addChild(mainMenu);
    
    CCString *menuStartStr = (CCString*)this->m_stringList->objectForKey("mainmenu_start");
    CCLabelTTF *menuStartLabel = CCLabelTTF::create(menuStartStr->getCString(), "FZCuYuan-M03S", 28, CCSizeMake(120, 40), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    
    CCString *menuRestartStr = (CCString*)this->m_stringList->objectForKey("mainmenu_restart");
    CCLabelTTF *menuRestartLabel = CCLabelTTF::create(menuRestartStr->getCString(), "FZCuYuan-M03S", 28, CCSizeMake(120, 40), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    
    CCString *menuSettingsStr = (CCString*)this->m_stringList->objectForKey("mainmenu_settings");
    CCLabelTTF *menuSettingsLabel = CCLabelTTF::create(menuSettingsStr->getCString(), "FZCuYuan-M03S", 28, CCSizeMake(120, 40), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    
    CCMenuItemLabel *menuStart = CCMenuItemLabel::create(menuStartLabel, this, menu_selector(RPGStartSceneLayer::onMenu));
    menuStart->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width / 2, 230));
    menuStart->setTag(kRPGStartSceneLayerTagMainMenuItemStart);
    mainMenu->addChild(menuStart);
    
    CCMenuItemLabel *menuRestart = CCMenuItemLabel::create(menuRestartLabel, this, menu_selector(RPGStartSceneLayer::onMenu));
    menuRestart->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width / 2, 170));
    menuRestart->setTag(kRPGStartSceneLayerTagMainMenuItemRestart);
    mainMenu->addChild(menuRestart);
    
    CCMenuItemLabel *menuSettings = CCMenuItemLabel::create(menuSettingsLabel, this, menu_selector(RPGStartSceneLayer::onMenu));
    menuSettings->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width / 2, 110));
    menuSettings->setTag(kRPGStartSceneLayerTagMainMenuItemSettings);
    mainMenu->addChild(menuSettings);
}

void RPGStartSceneLayer::showSettingsMenu()
{
    RPGSaveData *saveDataObj = loadSaveData(&this->m_db);
    
    CCMenu *settingsMenu = CCMenu::create();
    settingsMenu->setAnchorPoint(CCPointZero);
    settingsMenu->setPosition(CCPointZero);
    settingsMenu->setTag(kRPGStartSceneLayerTagSettingsMenu);
    this->addChild(settingsMenu);
    
    //设置样式的标题
    CCString *labStyleStr = (CCString*)this->m_stringList->objectForKey("settingslab_style");
    CCLabelTTF *labStyleLabel = CCLabelTTF::create(labStyleStr->getCString(), "FZCuYuan-M03S", 28, CCSizeMake(120, 40), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
    labStyleLabel->setFontFillColor(ccc3(188, 188, 188));
    labStyleLabel->setTag(kRPGStartSceneLayerTagLabStyle);
    labStyleLabel->setPosition(ccp(300, 550));
    this->addChild(labStyleLabel);
    
    //设置样式的按钮
    CCLabelTTF *menuStyleLeftLabel = CCLabelTTF::create("<<", "FZCuYuan-M03S", 28, CCSizeMake(120, 40), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    CCMenuItemLabel *menuStyleLeft = CCMenuItemLabel::create(menuStyleLeftLabel, this, menu_selector(RPGStartSceneLayer::onMenu));
    menuStyleLeft->setPosition(ccp(450, 550));
    menuStyleLeft->setTag(kRPGStartSceneLayerTagSettingsStyleLeft);
    settingsMenu->addChild(menuStyleLeft);
    
    CCLabelTTF *menuStyleRightLabel = CCLabelTTF::create(">>", "FZCuYuan-M03S", 28, CCSizeMake(120, 40), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    CCMenuItemLabel *menuStyleRight = CCMenuItemLabel::create(menuStyleRightLabel, this, menu_selector(RPGStartSceneLayer::onMenu));
    menuStyleRight->setPosition(ccp(650, 550));
    menuStyleRight->setTag(kRPGStartSceneLayerTagSettingsStyleRight);
    settingsMenu->addChild(menuStyleRight);
    
    //设置样式的当前使用样式
    CCString *labCurrStyleStr = NULL;
    for (int i = 0; i < this->m_styleData.size(); i++)
    {
        pair<string, string> item = this->m_styleData[i];
        if(strcmp(item.first.c_str(), saveDataObj->m_windowStyle.c_str()) == 0)
        {
            this->m_selectedStyleIndex = i;
            labCurrStyleStr = CCString::create(item.second);
            break;
        }
    }
    if(labCurrStyleStr)
    {
        CCLabelTTF *labCurrStyleLabel = CCLabelTTF::create(labCurrStyleStr->getCString(), "FZCuYuan-M03S", 28, CCSizeMake(120, 40), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
        labCurrStyleLabel->setFontFillColor(ccc3(188, 188, 188));
        labCurrStyleLabel->setTag(kRPGStartSceneLayerTagLabCurrStyle);
        labCurrStyleLabel->setPosition(ccp(550, 550));
        this->addChild(labCurrStyleLabel);
    }
    
    //背景音乐的开关
    CCString *labBgAudioStr = (CCString*)this->m_stringList->objectForKey("settingsmenu_bgaudio");
    CCLabelTTF *labBgAudioLabel = CCLabelTTF::create(labBgAudioStr->getCString(), "FZCuYuan-M03S", 28, CCSizeMake(120, 40), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
    labBgAudioLabel->setFontFillColor(ccc3(188, 188, 188));
    labBgAudioLabel->setTag(kRPGStartSceneLayerTagLabBgAudio);
    labBgAudioLabel->setPosition(ccp(300, 470));
    this->addChild(labBgAudioLabel);
    
    //背景音乐的开关的按钮
    CCLabelTTF *labBgAudioBtnLabel = NULL;
    if(CCUserDefault::sharedUserDefault()->getFloatForKey(GAME_BG_AUDIO_VOLUME, 1.0) == 0)
        labBgAudioBtnLabel = CCLabelTTF::create(((CCString*)this->m_stringList->objectForKey("settingsmenu_audio_on"))->getCString(), "FZCuYuan-M03S", 28, CCSizeMake(120, 40), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    else
        labBgAudioBtnLabel = CCLabelTTF::create(((CCString*)this->m_stringList->objectForKey("settingsmenu_audio_off"))->getCString(), "FZCuYuan-M03S", 28, CCSizeMake(120, 40), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    CCMenuItemLabel *labBgAudioBtn = CCMenuItemLabel::create(labBgAudioBtnLabel, this, menu_selector(RPGStartSceneLayer::onMenu));
    labBgAudioBtn->setPosition(ccp(550, 470));
    labBgAudioBtn->setTag(kRPGStartSceneLayerTagSettingsBgAudioBtn);
    settingsMenu->addChild(labBgAudioBtn);
    
    //效果声音的开关
    CCString *labEffectAudioStr = (CCString*)this->m_stringList->objectForKey("settingsmenu_effectaudio");
    CCLabelTTF *labEffectAudioLabel = CCLabelTTF::create(labEffectAudioStr->getCString(), "FZCuYuan-M03S", 28, CCSizeMake(120, 40), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
    labEffectAudioLabel->setFontFillColor(ccc3(188, 188, 188));
    labEffectAudioLabel->setTag(kRPGStartSceneLayerTagLabEffectAudio);
    labEffectAudioLabel->setPosition(ccp(300, 390));
    this->addChild(labEffectAudioLabel);
    
    //效果声音的开关的按钮
    CCLabelTTF *labEffectAudioBtnLabel = NULL;
    if(CCUserDefault::sharedUserDefault()->getFloatForKey(GAME_EFFECT_AUDIO_VOLUME, 1.0) == 0)
        labEffectAudioBtnLabel = CCLabelTTF::create(((CCString*)this->m_stringList->objectForKey("settingsmenu_audio_on"))->getCString(), "FZCuYuan-M03S", 28, CCSizeMake(120, 40), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    else
        labEffectAudioBtnLabel = CCLabelTTF::create(((CCString*)this->m_stringList->objectForKey("settingsmenu_audio_off"))->getCString(), "FZCuYuan-M03S", 28, CCSizeMake(120, 40), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    CCMenuItemLabel *labEffectAudioBtn = CCMenuItemLabel::create(labEffectAudioBtnLabel, this, menu_selector(RPGStartSceneLayer::onMenu));
    labEffectAudioBtn->setPosition(ccp(550, 390));
    labEffectAudioBtn->setTag(kRPGStartSceneLayerTagSettingsEffectAudioBtn);
    settingsMenu->addChild(labEffectAudioBtn);
    
    //恢复默认
    CCString *menuRestoreStr = (CCString*)this->m_stringList->objectForKey("settingsmenu_restore");
    CCLabelTTF *menuRestoreLabel = CCLabelTTF::create(menuRestoreStr->getCString(), "FZCuYuan-M03S", 28, CCSizeMake(120, 40), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
    CCMenuItemLabel *menuRestore = CCMenuItemLabel::create(menuRestoreLabel, this, menu_selector(RPGStartSceneLayer::onMenu));
    menuRestore->setPosition(ccp(300, 170));
    menuRestore->setTag(kRPGStartSceneLayerTagSettingsRestore);
    settingsMenu->addChild(menuRestore);
    
    //返回
    CCString *menuBackStr = (CCString*)this->m_stringList->objectForKey("settingsmenu_back");
    CCLabelTTF *menuBackLabel = CCLabelTTF::create(menuBackStr->getCString(), "FZCuYuan-M03S", 28, CCSizeMake(120, 40), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
    CCMenuItemLabel *menuBack = CCMenuItemLabel::create(menuBackLabel, this, menu_selector(RPGStartSceneLayer::onMenu));
    menuBack->setPosition(ccp(300, 100));
    menuBack->setTag(kRPGStartSceneLayerTagSettingsBack);
    settingsMenu->addChild(menuBack);
    
}

//public
void RPGStartSceneLayer::goToMapScene()
{
    CCArray *loadTextures = CCArray::create();
    loadTextures->addObject(CCString::create("map.png"));
    loadTextures->addObject(CCString::create("main.png"));
    loadTextures->addObject(CCString::create("joystick.png"));
    loadTextures->addObject(CCString::create("actor4_0.png"));
    loadTextures->addObject(CCString::create("actor111.png"));
    loadTextures->addObject(CCString::create("actor113.png"));
    loadTextures->addObject(CCString::create("actor114.png"));
    loadTextures->addObject(CCString::create("actor115.png"));
    loadTextures->addObject(CCString::create("actor117.png"));
    loadTextures->addObject(CCString::create("actor120.png"));
    
    CCMenu *mainMenu = (CCMenu*)this->getChildByTag(kRPGStartSceneLayerTagMainMenu);
    mainMenu->setEnabled(false);
    
    CCScene *s = RPGLoadingSceneLayer::scene(loadTextures, NULL, "single_map");
    CCTransitionFade *t = CCTransitionFade::create(GAME_SCENE, s);
    CCDirector::sharedDirector()->replaceScene(t);
}
