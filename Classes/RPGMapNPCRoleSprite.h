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
    bool m_autoMove = false;
    
    void autoMove(float delay);
    void autoMoveEnd();
    
public:
    
    CCString* m_content;
    
    virtual ~RPGMapNPCRoleSprite();
    virtual bool init(CCString *roleTexture, CCString* headTexture, CCString* name, CCString* content, int defaultSpriteFrameIndex, bool autoMove);
    static RPGMapNPCRoleSprite* create(CCString *roleTexture, CCString* headTexture, CCString* name, CCString* content, int defaultSpriteFrameIndex, bool autoMove);
    
    void stopAutoMove();
    void startAutoMove();
    
};

#endif /* defined(__OzgGameRPG__RPGMapNPCRoleSprite__) */
