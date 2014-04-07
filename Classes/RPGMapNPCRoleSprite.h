//
//  RPGMapNPCRoleSprite.h
//  FantasyRPG
//
//  Created by 欧志刚 on 14-1-17.
//
//

#ifndef __FantasyRPG__RPGMapNPCRoleSprite__
#define __FantasyRPG__RPGMapNPCRoleSprite__

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

#endif /* defined(__FantasyRPG__RPGMapNPCRoleSprite__) */
