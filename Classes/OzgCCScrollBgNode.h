/*
//滚动背景
//目前只支持左右滚动

std::vector<std::string> bgList;
bgList.push_back("stage1_bg1.png");
bgList.push_back("stage1_bg2.png");

OzgCCScrollBgNode *bg = OzgCCScrollBgNode::create(bgList);
this->addChild(bg);

*/

#ifndef __ozgcc2dxutils__OzgCCScrollBgNode__
#define __ozgcc2dxutils__OzgCCScrollBgNode__

#include <iostream>
#include "cocos2d.h"

class OzgCCScrollBgNode : public cocos2d::CCParallaxNode
{
private:
    
	std::vector<std::string> m_bgList;

public:

	float m_scrollSpeed;

    virtual ~OzgCCScrollBgNode();

	bool init(std::vector<std::string> bgList);
	static OzgCCScrollBgNode* create(std::vector<std::string> bgList);

    void update(float delta);

};

#endif /* defined(__ozgcc2dxutils__OzgCCScrollBgNode__) */
