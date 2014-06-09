//
//  RPGMapNPCRoleSprite.h
//  OzgGameRPG
//
//  Created by ozg on 14-1-17.
//
//

#ifndef __OzgGameRPG__RPGMapNPCRoleSprite__
#define __OzgGameRPG__RPGMapNPCRoleSprite__

#include "RPGMapRoleSprite.h"

class RPGMapNPCRoleSprite : public RPGMapRoleSprite
{
    
private:
    bool m_autoMove;
    
    void autoMove(float delay);
    void autoMoveEnd();
    
public:
    
    int m_dataId;
    CCString* m_content;
    
    virtual ~RPGMapNPCRoleSprite();
    virtual bool init(int dataId, CCString *roleTexture, CCString* headTexture, CCString* name, CCString* content, int defaultSpriteFrameIndex, bool autoMove);
    static RPGMapNPCRoleSprite* create(int dataId, CCString *roleTexture, CCString* headTexture, CCString* name, CCString* content, int defaultSpriteFrameIndex, bool autoMove);
    
    void stopAutoMove();
    void startAutoMove();
    
};

#endif /* defined(__OzgGameRPG__RPGMapNPCRoleSprite__) */
