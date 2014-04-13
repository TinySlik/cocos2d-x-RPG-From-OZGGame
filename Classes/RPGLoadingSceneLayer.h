//
//  RPGLoadingSceneLayer.h
//  OzgGameRPG
//
//  Created by ozg on 14-1-5.
//
//

#ifndef __OzgGameRPG__RPGLoadingSceneLayer__
#define __OzgGameRPG__RPGLoadingSceneLayer__

#include "RPGBaseSceneLayer.h"

USING_NS_CC;

class RPGLoadingSceneLayer : public RPGBaseSceneLayer
{
    
private:
    int m_loadingIndex;
    CCArray* m_loadTextures;
    CCArray* m_releaseTextures;
    string m_nextScene;
    
public:
    virtual ~RPGLoadingSceneLayer();
    bool init(CCArray* loadTextures, CCArray* releaseTextures, string nextScene);
    
    static RPGLoadingSceneLayer* create(CCArray* loadTextures, CCArray* releaseTextures, string nextScene);
    static CCScene* scene(CCArray* loadTextures, CCArray* releaseTextures, string nextScene);
    
    void executeLoading(float delay);
    void loadCallBack(CCObject* sender);
    void goToNextScene(float delay);
};

#endif /* defined(__OzgGameRPG__RPGLoadingSceneLayer__) */
